#include "StdInc.h"

using namespace dotnetpe;
using namespace fastdelegate;

/*

    The purpose of code splicing is to jumble the code around a bit so it becomes harder
    for decompilers to understand. Selected instructions are moved to the end of the
    method (or the end of the current try-block), a branch is placed at their original
    location to redirect execution to their new location, and afterwards another branch
    is done to return to the instruction that comes after the original location.
    Sometimes a small block of instructions is moved, in that case it's possible that
    some more jumping around is done at the new location before returning to the
    original location.
    RestoreSplices() takes care of this obfuscation. Spliced-out instructions are
    moved back to their original location and the (now superfluous) branches are removed.

    SmartAssembly 6 builds a bit further on this and replaces some of the (unconditional)
    branches by a ldc.i4 instruction and a conditional branch. The end result is the same
    (an unconditional branch or a nop), but again trips off decompilers - they might e.g.
    think there is an if() when there really isn't.
    This trick is handled by FixPseudoConditionals(). It replaces the described pattern
    by unconditional jumps or nops, ready to be cleaned up by RestoreSplices().

*/

void AntiCodeSplicing::PreProcess ( const wchar_t* pwszFile )
{

}

void AntiCodeSplicing::Init ( const ProtectionStripperArgs& args )
{
    wprintf ( L"Fixing spliced code...\n" );
}

void AntiCodeSplicing::StripMethod ( const ProtectionStripperArgs& args )
{
    m_BasicBlocks.FromMethod ( args.m_pMethod );
	FixPseudoConditionals ( args.m_pMethod );
    RestoreSplices ();
    m_BasicBlocks.ToMethod ( args.m_pMethod );
}

void AntiCodeSplicing::Finish ( const ProtectionStripperArgs& args )
{

}

void AntiCodeSplicing::PostProcess ( const wchar_t* pwszFile )
{

}

void AntiCodeSplicing::FixPseudoConditionals ( MethodDef* pMethod )
{
    prevector < dword, 0x10 > fixedUnconditionalBranches;

    do
    {
        BasicBlock* pBB = m_BasicBlocks.GetFirstBB ();
        while ( pBB )
        {
            BasicBlock* pNextBB = pBB->GetOutputInfo ().m_pNextBB;

            byte* ppLastInstrs[2];
            memset ( ppLastInstrs, 0, sizeof(ppLastInstrs) );

            byte* pInstr = pBB->Code ();
            byte* pEnd = pInstr + pBB->GetLength ();
            int iNumLastInstrs = 0;
            for ( ; pInstr < pEnd; pInstr += ILInstr::GetLength ( pInstr ) )
            {
                memmove ( &ppLastInstrs[1], &ppLastInstrs[0], sizeof(ppLastInstrs) - sizeof(byte*) );
                ppLastInstrs[0] = pInstr;
                if ( iNumLastInstrs < sizeof(ppLastInstrs) / sizeof(byte*) )
                    iNumLastInstrs++;
            }

            if ( iNumLastInstrs == 0 )
            {
                pBB = pNextBB;
                continue;
            }

            BasicBlock* pFallthroughBB = pBB->GetFallthrough ();

            // Is there a dup instruction before the branch?
            byte* pDupInstr = ppLastInstrs[0];
            bool bDup =
                iNumLastInstrs >= 2 &&
                *pDupInstr == ILOPCODE_DUP;
            if ( !bDup )
                pDupInstr = NULL;

            // Find and inspect the ldc instruction
            byte* pConstInstr = ppLastInstrs[bDup ? 1 : 0];
            bool bZeroConst = *pConstInstr == ILOPCODE_LDC_I4_0;
            bool bNonzeroConst = *pConstInstr == ILOPCODE_LDC_I4_M1 || ( *pConstInstr >= ILOPCODE_LDC_I4_1 && *pConstInstr <= ILOPCODE_LDC_I4_8 );
            if ( !bZeroConst && !bNonzeroConst )
                pConstInstr = NULL;

            // Is the constant used purely for the pseudo branch or is it also part of the
            // original, unobfuscated program? (more specifically, if it is duplicated,
            // are both instances used by pseudo branches or only one)
            bool bConstUnused =
                !bDup || (
                    pFallthroughBB &&
                    pFallthroughBB->GetLength () == 0 &&
                    pFallthroughBB->GetBranchType () == pBB->GetBranchType () &&
                    pFallthroughBB->NumIncoming () == 1
                );

            bool bNop = false;
            bool bBranch = false;
            switch ( pBB->GetBranchType () )
            {
                case ILOPCODE_BRFALSE:
                    bNop = bNonzeroConst;
                    bBranch = bZeroConst;
                    break;

                case ILOPCODE_BRTRUE:
                    bNop = bZeroConst;
                    bBranch = bNonzeroConst;
                    break;
            }

            if ( bNop || bBranch )
            {
                if ( bConstUnused )
                    pBB->SetLength ( pConstInstr - pBB->Code () );
                else if ( bDup )
                    pBB->SetLength ( pDupInstr - pBB->Code () );
            }

            if ( bNop )
            {
                if ( pBB->GetLength () > 0 )
                {
                    pBB->SetTarget ( NULL );
                    pBB->SetBranchType ( ILOPCODE_NOP );
                }
                else
                {
                    m_BasicBlocks.Delete ( pBB );
                }
                if ( bDup && bConstUnused )
                {
                    pNextBB = pFallthroughBB->GetOutputInfo ().m_pNextBB;
                    m_BasicBlocks.Delete ( pFallthroughBB );
                }
            }
            else if ( bBranch )
            {
                pBB->SetFallthrough ( NULL );
                pBB->SetBranchType ( ILOPCODE_BR );
                fixedUnconditionalBranches.push_back ( pBB->GetOffset () );
            }

            pBB = pNextBB;
        }
    } while ( m_BasicBlocks.MergeBlocks ( pMethod ) );

    // There may be dummy stack rebalancing code after unconditional pseudobranches
    // (dead code, just to appease the CLR verifier). Now that we've made them into
    // actual unconditional branches we can remove this code.
    for ( auto it = fixedUnconditionalBranches.begin (); it != fixedUnconditionalBranches.end (); it++ )
    {
        dword dwOffset = *it;
        BasicBlock* pBB = m_BasicBlocks.GetBBContainingOrBefore ( *it );
        if ( pBB )
            pBB = pBB->GetOutputInfo ().m_pNextBB;
        if ( pBB && pBB->NumIncoming () == 0 )
            m_BasicBlocks.Delete ( pBB );
    }
}

void AntiCodeSplicing::RestoreSplices ()
{
    BasicBlock* pBB = m_BasicBlocks.GetFirstBB ();
	while ( pBB )
	{
		if ( pBB->GetBranchType () != ILOPCODE_BR )
        {
            pBB = pBB->GetOutputInfo ().m_pNextBB;
            continue;
        }

        BasicBlock* pSpliceStartBB = pBB;
        BasicBlock* pSpliceEndBB = pBB->GetOutputInfo ().m_pNextBB;
        if ( !pSpliceEndBB )
        {
            pBB = pBB->GetOutputInfo ().m_pNextBB;
            continue;
        }

        // First check if this is a splice, ie if the instruction after the
        // branch can be reached through a series of branches
        BasicBlock* pLookaheadBB = pBB->GetTarget ();
        while ( pLookaheadBB && pLookaheadBB != pSpliceStartBB && pLookaheadBB != pSpliceEndBB )
        {
            if ( pLookaheadBB->NumIncoming () != 1 || pLookaheadBB->GetLength () > 0x10 )
                pLookaheadBB = NULL;
            else if ( pLookaheadBB->GetBranchType () == ILOPCODE_NOP )
                pLookaheadBB = pLookaheadBB->GetFallthrough ();
            else if ( pLookaheadBB->GetBranchType () == ILOPCODE_BR )
                pLookaheadBB = pLookaheadBB->GetTarget ();
            else
                pLookaheadBB = NULL;
        }

        if ( pLookaheadBB != pSpliceEndBB )
        {
            pBB = pBB->GetOutputInfo ().m_pNextBB;
            continue;
        }

        // It's a splice - restore it
        do
        {
            BasicBlock* pNextBB = NULL;
            if ( pBB->GetBranchType () == ILOPCODE_NOP )
            {
                pNextBB = pBB->GetFallthrough ();
                pBB->GetOutputInfo ().InsertAfter ( pNextBB );
            }
            else if ( pBB->GetBranchType () == ILOPCODE_BR )
            {
                pNextBB = pBB->GetTarget ();
                pBB->GetOutputInfo ().InsertAfter ( pNextBB );
                pBB->RemoveTarget ( pNextBB );
                pBB->SetBranchType ( ILOPCODE_NOP );
                pBB->SetFallthrough ( pNextBB );
            }
            
            pBB = pNextBB;
        } while ( pBB != pSpliceStartBB && pBB != pSpliceEndBB );
        pBB = pSpliceEndBB;
	}
}
