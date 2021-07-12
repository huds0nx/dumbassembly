#include "StdInc.h"

using namespace fastdelegate;

namespace dotnetpe
{

    int ILInstr::GetLength ( byte* pInstr )
    {
        ILOPCODE_STRUCT opcode;
        GetSingleMSILInstr ( pInstr, 0x7FFFFFFF, 0, &opcode );
        return opcode.Size;
    }

    bool ILInstr::IsConstIntLoad ( byte* pInstr )
    {
        return *pInstr >= ILOPCODE_LDC_I4_M1 && *pInstr <= ILOPCODE_LDC_I4;
    }

    int ILInstr::GetConstIntLoadValue ( byte* pInstr )
    {
        if ( *pInstr == ILOPCODE_LDC_I4_M1 )
        {
            return -1;
        }
        else if ( *pInstr >= ILOPCODE_LDC_I4_0 && *pInstr <= ILOPCODE_LDC_I4_8 )
        {
            return *pInstr - ILOPCODE_LDC_I4_0;
        }
        else if ( *pInstr == ILOPCODE_LDC_I4_S )
        {
            return *(byte *)(pInstr + 1);
        }
        else if ( *pInstr == ILOPCODE_LDC_I4 )
        {
            return *(int *)(pInstr + 1);
        }
        else
        {
            assert ( false );
            return 0;
        }
    }

    bool ILInstr::IsBranch ( byte* pInstr )
    {
        return *pInstr >= ILOPCODE_BR_S && *pInstr <= ILOPCODE_BLT_UN;
    }

    bool ILInstr::IsConditionalBranch ( byte* pInstr )
    {
        return (*pInstr >= ILOPCODE_BRFALSE_S && *pInstr <= ILOPCODE_BLT_UN_S) ||
               (*pInstr >= ILOPCODE_BRFALSE   && *pInstr <= ILOPCODE_BLT_UN);
    }

    bool ILInstr::IsUnconditionalBranch ( byte* pInstr )
    {
        return *pInstr == ILOPCODE_BR_S || *pInstr == ILOPCODE_BR;
    }

    byte ILInstr::GetBranchType ( byte* pInstr )
    {
        if ( *pInstr >= ILOPCODE_BR_S && *pInstr <= ILOPCODE_BLT_UN_S )
            return *pInstr + (ILOPCODE_BR - ILOPCODE_BR_S);
        else if ( *pInstr >= ILOPCODE_BR && *pInstr <= ILOPCODE_BLT_UN )
            return *pInstr;
        else
            return ILOPCODE_NOP;
    }

    byte* ILInstr::GetBranchTarget ( byte* pInstr )
    {
        if ( *pInstr >= ILOPCODE_BR_S && *pInstr <= ILOPCODE_BLT_UN_S )
        {
            return pInstr + 2 + *(char *)(pInstr + 1);
        }
        else if ( *pInstr >= ILOPCODE_BR && *pInstr <= ILOPCODE_BLT_UN )
        {
            return pInstr + 5 + *(int *)(pInstr + 1);
        }
        else
        {
            return NULL;
        }
    }

    bool ILInstr::IsLeave ( byte* pInstr )
    {
        return *pInstr == ILOPCODE_LEAVE_S || *pInstr == ILOPCODE_LEAVE;
    }

    byte* ILInstr::GetLeaveTarget ( byte* pInstr )
    {
        if ( *pInstr == ILOPCODE_LEAVE_S )
        {
            return pInstr + 2 + *(char *)(pInstr + 1);
        }
        else if ( *pInstr == ILOPCODE_LEAVE )
        {
            return pInstr + 5 + *(int *)(pInstr + 1);
        }
        else
        {
            return NULL;
        }
    }

    bool ILInstr::IsCall ( byte* pInstr )
    {
        return IsDirectCall ( pInstr ) || IsIndirectCall ( pInstr );
    }

    bool ILInstr::IsDirectCall ( byte* pInstr )
    {
        return *pInstr == ILOPCODE_CALL || *pInstr == ILOPCODE_CALLVIRT;
    }

    bool ILInstr::IsIndirectCall ( byte* pInstr )
    {
        return *pInstr == ILOPCODE_CALLI;
    }

    bool ILInstr::IsSwitch ( byte* pInstr )
    {
        return *pInstr == ILOPCODE_SWITCH;
    }

    dword ILInstr::NumSwitchTargets ( byte* pInstr )
    {
        assert ( IsSwitch ( pInstr ) );
        return *(dword *)(pInstr + 1);
    }

    byte* ILInstr::GetSwitchTarget ( byte* pInstr, dword dwTargetIndex )
    {
        assert ( IsSwitch ( pInstr ) && dwTargetIndex < NumSwitchTargets ( pInstr ) );
        return pInstr + GetLength ( pInstr ) + *(int *)( pInstr + 1 + 4*(1 + dwTargetIndex) );
    }

    void ILInstr::WriteBranch ( byte ucType, byte* pAt, byte* pTarget )
    {
        assert ( ucType >= ILOPCODE_BR && ucType <= ILOPCODE_BLT_UN );
        WriteBranch ( ucType + (ILOPCODE_BR_S - ILOPCODE_BR), ucType, pAt, pTarget );
    }

    void ILInstr::WriteBranch ( byte ucShortOpcode, byte ucLongOpcode, byte* pAt, byte* pTarget )
    {
        int iOffset = pTarget - (pAt + 2);
        if ( iOffset >= -0x80 && iOffset < 0x80 )
        {
            *pAt = ucShortOpcode;
            *(pAt + 1) = static_cast < char > ( iOffset );
        }
        else
        {
            iOffset = pTarget - (pAt + 5);
            *pAt = ucLongOpcode;
            *(int *)(pAt + 1) = iOffset;
        }
    }

    void ILInstr::WriteBranch ( byte ucType, Stream* pStream, int iOffset )
    {
        assert ( ucType >= ILOPCODE_BR && ucType <= ILOPCODE_BLT_UN );
        WriteBranch ( ucType + (ILOPCODE_BR_S - ILOPCODE_BR), ucType, pStream, iOffset );
    }

    void ILInstr::WriteBranch ( byte ucShortOpcode, byte ucLongOpcode, Stream* pStream, int iOffset )
    {
        if ( iOffset >= -0x80 && iOffset < 0x80 )
        {
            pStream->Write ( ucShortOpcode );
            pStream->Write ( static_cast < byte > ( iOffset ) );
        }
        else
        {
            pStream->Write ( ucLongOpcode );
            pStream->Write ( iOffset );
        }
    }

    void ILInstr::WriteSwitch ( byte* pAt, dword dwNumTargets, byte** ppTargets )
    {
        byte* pNextInstr = pAt + 1 + 4*(1 + dwNumTargets);
        *pAt = ILOPCODE_SWITCH;
        *(dword *)( pAt + 1 ) = dwNumTargets;
        for ( dword i = 0; i < dwNumTargets; i++ )
        {
            *(int *)( pAt + 1 + 4*(1 + i) ) = ppTargets[i] - pNextInstr;
        }
    }

    void ILInstr::WriteSwitch ( byte* pAt, dword dwNumTargets, FastDelegate1 < dword, byte* > dgGetTarget )
    {
        byte* pNextInstr = pAt + 1 + 4*(1 + dwNumTargets);
        *pAt = ILOPCODE_SWITCH;
        *(dword *)( pAt + 1 ) = dwNumTargets;
        for ( dword i = 0; i < dwNumTargets; i++ )
        {
            *(int *)( pAt + 1 + 4*(1 + i) ) = dgGetTarget ( i ) - pNextInstr;
        }
    }

    void ILInstr::WriteSwitch ( Stream* pStream, dword dwNumTargets, int* pOffsets )
    {
        pStream->Write ( (byte)ILOPCODE_SWITCH );
        pStream->Write ( dwNumTargets );
        for ( dword i = 0; i < dwNumTargets; i++ )
        {
            pStream->Write ( pOffsets[i] );
        }
    }

    void ILInstr::WriteSwitch ( Stream* pStream, dword dwNumTargets, FastDelegate1 < dword, int > dgGetOffset )
    {
        pStream->Write ( (byte)ILOPCODE_SWITCH );
        pStream->Write ( dwNumTargets );
        for ( dword i = 0; i < dwNumTargets; i++ )
        {
            pStream->Write ( dgGetOffset ( i ) );
        }
    }

    void ILInstr::RetargetBranches ( byte* pBranchesBegin, int iBranchesLength, byte* pTargetBegin, int iTargetLength, byte* pNewTarget )
    {
        // A target code area has moved to a new position. Update all branches in a given branch area that point
        // to this target area.

        if ( iBranchesLength <= 0 )
            return;

        int iLength = 0;
        for ( byte* pInstr = pBranchesBegin; pInstr < pBranchesBegin + iBranchesLength; pInstr += iLength )
        {
            iLength = GetLength ( pInstr );
            if ( IsBranch ( pInstr ) || IsLeave ( pInstr ) )
            {
                byte* pTarget = IsBranch ( pInstr ) ? GetBranchTarget ( pInstr ) :
                    GetLeaveTarget ( pInstr );
                if ( pTarget < pTargetBegin || pTarget >= pTargetBegin + iTargetLength )
                    continue;

                if ( iLength == 2 )
                {
                    int iOffset = *(char *)(pInstr + 1);
                    iOffset += pNewTarget - pTargetBegin;
                    assert ( iOffset >= -128 && iOffset < 128 );
                    *(char *)(pInstr + 1) = static_cast < char > ( iOffset );
                }
                else if ( iLength == 5 )
                {
                    int iOffset = *(int *)(pInstr + 1);
                    iOffset += pNewTarget - pTargetBegin;
                    *(int *)(pInstr + 1) = iOffset;
                }
            }
        }
    }

    void ILInstr::MoveCode ( byte* pTo, byte* pFrom, int iSize )
    {
        // Move a block of code from one location to another, making sure that the branches
        // in it that point outside of it retain their correct target.

        if ( pTo == pFrom || iSize <= 0 )
            return;

        int iLength = 0;
        for ( byte* pInstr = pFrom; pInstr < pFrom + iSize; pInstr += iLength )
        {
            iLength = GetLength ( pInstr );
            if ( IsBranch ( pInstr ) || IsLeave ( pInstr ) )
            {
                byte* pTarget = IsBranch ( pInstr ) ? GetBranchTarget ( pInstr ) : GetLeaveTarget ( pInstr );
                if ( pTarget >= pFrom && pTarget < pFrom + iSize )
                    continue;

                if ( iLength == 2 )
                {
                    int iOffset = *(char *)(pInstr + 1);
                    iOffset -= pTo - pFrom;
                    assert ( iOffset >= -128 && iOffset < 128 );
                    *(char *)(pInstr + 1) = static_cast < char > ( iOffset );
                }
                else if ( iLength == 5 )
                {
                    int iOffset = *(int *)(pInstr + 1);
                    iOffset -= pTo - pFrom;
                    *(int *)(pInstr + 1) = iOffset;
                }
            }
        }

        if ( pTo < pFrom )
        {
            for ( int i = 0; i < iSize; i++ )
            {
                pTo[i] = pFrom[i];
            }
        }
        else if ( pTo > pFrom )
        {
            for ( int i = iSize - 1; i >= 0; i-- )
            {
                pTo[i] = pFrom[i];
            }
        }
    }

}
