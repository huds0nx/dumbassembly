#include "StdInc.h"

using namespace fastdelegate;

namespace dotnetpe
{

    BasicBlockPool::BasicBlockPool ()
		: m_EntryBB ( this ), m_pFirstBB ( NULL ), m_pLastBB ( NULL )
    {
        m_iNextLoopID = 0;
    }

    BasicBlockPool::BasicBlockPool ( MethodDef* pMethod )
		: m_EntryBB ( this ), m_pFirstBB ( NULL ), m_pLastBB ( NULL )
    {
        m_iNextLoopID = 0;
        FromMethod ( pMethod );
    }

    BasicBlockPool::BasicBlockPool ( byte* pCode, dword dwSize )
		: m_EntryBB ( this ), m_pFirstBB ( NULL ), m_pLastBB ( NULL )
    {
        m_iNextLoopID = 0;
        FromCode ( pCode, dwSize );
    }

    BasicBlockPool::~BasicBlockPool ()
    {

    }

    void BasicBlockPool::FromMethod ( MethodDef* pMethod )
    {
        Clear ();
        m_InputOffsetToBB [ 0 ] = Create ( this, 0, pMethod->CodeSize () );

        pMethod->ForEachEH (
            [=] ( MethodDef::EHInfo& eh )
            {
                SplitBB ( eh.m_iTryOffset, 0, true );
                SplitBB ( eh.m_iTryOffset + eh.m_iTryLength, 0, false );

                if ( eh.m_iFilterOffset >= 0 )
                    SplitBB ( eh.m_iFilterOffset, 0, false );

                SplitBB ( eh.m_iCatchOffset, 0, false );
                SplitBB ( eh.m_iCatchOffset + eh.m_iCatchLength, 0, false );
            }
        );

        FromCode ( pMethod->Code (), pMethod->CodeSize (), false );
    }

    void BasicBlockPool::FromCode ( byte* pCode, dword dwSize )
    {
        FromCode ( pCode, dwSize, true );
    }

    void BasicBlockPool::FromCode ( byte* pCode, dword dwSize, bool bClear )
    {
        if ( bClear )
        {
            Clear ();
            m_InputOffsetToBB [ 0 ] = Create ( this, 0, dwSize );
        }

        m_pFirstBB = &*Begin ();

        // Split up the blocks at specific instructions: branches, switches, returns, ...
        int iBB = 0;
        while ( iBB < Size () )
        {
            BasicBlock* pBB = &At ( iBB );
            byte* pInstr = pCode + pBB->GetOffset ();
            byte* pEnd = pCode + pBB->GetOffset () + pBB->GetLength ();
            iBB++;

            int iLength;
            for ( ; pInstr < pEnd; pInstr += iLength )
            {
                iLength = ILInstr::GetLength ( pInstr );

                if ( iLength == 1 && (*pInstr == ILOPCODE_RET || *pInstr == ILOPCODE_THROW || *pInstr == ILOPCODE_ENDFINALLY) ||
					 iLength == 2 && (*(word*)pInstr == ILOPCODE_RETHROW || *(word*)pInstr == ILOPCODE_ENDFILTER) )
                {
                    SplitBB ( pBB, pInstr + iLength - pCode, 0, false );
                    break;
                }
                else if ( ILInstr::IsBranch ( pInstr ) )
                {
                    SplitBB ( pBB, pInstr - pCode, iLength, !ILInstr::IsUnconditionalBranch ( pInstr ) );
                    pBB->SetBranchType ( ILInstr::GetBranchType ( pInstr ) );

                    BasicBlock* pTargetBB = SplitBB ( ILInstr::GetBranchTarget ( pInstr ) - pCode, 0, true );
                    assert ( pTargetBB );

                    pBB = GetBBContaining ( pInstr - pCode );
                    pBB->SetTarget ( pTargetBB );
                    break;
                }
                else if ( ILInstr::IsLeave ( pInstr ) )
                {
                    SplitBB ( pBB, pInstr - pCode, iLength, false );
                    pBB->SetBranchType ( ILOPCODE_LEAVE );

                    BasicBlock* pTargetBB = SplitBB ( ILInstr::GetLeaveTarget ( pInstr ) - pCode, 0, true );
                    assert ( pTargetBB );

                    pBB = GetBBContaining ( pInstr - pCode );
                    pBB->SetTarget ( pTargetBB );
                    break;
                }
                else if ( ILInstr::IsSwitch ( pInstr ) )
                {
                    SplitBB ( pBB, pInstr - pCode, iLength, true );
                    pBB->SetBranchType ( ILOPCODE_SWITCH );

                    dword dwNumTargets = ILInstr::NumSwitchTargets ( pInstr );
                    for ( dword i = 0; i < dwNumTargets; i++ )
                    {
                        byte* pTarget = ILInstr::GetSwitchTarget ( pInstr, i );
                        BasicBlock* pTargetBB = SplitBB ( pTarget - pCode, 0, true );
                        assert ( pTargetBB );

                        pBB = GetBBContaining ( pInstr - pCode );
                        pBB->AddTarget ( pTargetBB );
                    }
                    break;
                }
            }
        }

        // By default, basic blocks will be output in the same order as the input
        int iIndex = 0;
        BasicBlock* pBB = m_pFirstBB;
        do
        {
            BasicBlock::InputInfo& inputInfo = pBB->GetInputInfo ();
            BasicBlock::OutputInfo& outputInfo = pBB->GetOutputInfo ();
            inputInfo.m_iIndex = iIndex;
            outputInfo.m_iIndex = iIndex;
            outputInfo.m_pPrevBB = inputInfo.m_pPrevBB;
            outputInfo.m_pNextBB = inputInfo.m_pNextBB;

            pBB->FetchCodeFromMethod ( pCode );

            m_pLastBB = pBB;
            pBB = inputInfo.m_pNextBB;
            iIndex++;
        } while ( pBB );
    }

    BasicBlock* BasicBlockPool::ReorderBlocks ( MethodDef* pMethod )
    {
		m_iNextLoopID = 0;
        for ( iterator it = Begin (); it != End (); it++ )
            it->GetOutputInfo ().Reset ();

        // Provide a dummy fallthrough incoming for the first BB to assist in loop detection
        m_EntryBB.SetFallthrough ( m_pFirstBB );
        m_EntryBB.GetOutputInfo ().m_iIndex = 0;

		SetCatchOutgoings ( pMethod );
        ResetReachabilities ();
        BuildFullReachabilities ( pMethod );
        RemoveDeadBlocksInternal ();
        AssignTryCatchBBPriorities ( pMethod );
        for ( iterator it = Begin (); it != End (); it++ )
        {
            AssignLoopBBPriorities ( &(*it) );
        }
		ClearCatchOutgoings ();

        ResetReachabilities ();
        BuildPartialReachabilities ( pMethod );
        BasicBlock* pLastBlock = OrderBlocks ( pMethod, m_pFirstBB );

		int iEHsLeft = 0;
		pMethod->ForEachEH (
			[&] ( MethodDef::EHInfo& eh )
			{
				if ( !GetFirstBBContainedIn ( eh.m_iCatchOffset, eh.m_iCatchLength )->GetOutputInfo ().IsOrdered () )
					iEHsLeft++;
			}
		);

		bool bHandledEH = false;
		while ( iEHsLeft > 0 )
		{
			pMethod->ForEachEH (
				[&] ( MethodDef::EHInfo& eh )
				{
					if ( eh.m_iTryLength == 0 || eh.m_iCatchLength == 0 )
						return;

					BasicBlock* pLastTryBB = NULL;
					BasicBlock* pNextTryBB = GetFirstBBContainedIn ( eh.m_iTryOffset, eh.m_iTryLength );
					BasicBlock* pFirstCatchBB = GetFirstBBContainedIn ( eh.m_iCatchOffset, eh.m_iCatchLength );
					if ( !pNextTryBB->GetOutputInfo ().IsOrdered () ||
						 pFirstCatchBB->GetOutputInfo ().IsOrdered () )
						return;

					while ( pNextTryBB && pNextTryBB->IsContainedIn ( eh.m_iTryOffset, eh.m_iTryLength ) )
					{
						pLastTryBB = pNextTryBB;
						pNextTryBB = pNextTryBB->GetOutputInfo ().m_pNextBB;
					}

					BasicBlock* pLastCatchBB = OrderBlocks ( pMethod, pFirstCatchBB );

					BasicBlock* pAfterEHBB = pLastTryBB->GetOutputInfo ().LinkNext ( pFirstCatchBB );
					if ( pAfterEHBB )
						pAfterEHBB->GetOutputInfo ().LinkPrevious ( pLastCatchBB );

					iEHsLeft--;
					bHandledEH = true;
				}
			);

			assert ( bHandledEH );
			bHandledEH = false;
		}

        return pLastBlock;
    }

    BasicBlock* BasicBlockPool::ReorderBlocks ()
    {
        return ReorderBlocks ( NULL );
    }

    void BasicBlockPool::RemoveDeadBlocks ( MethodDef* pMethod )
    {
        ResetReachabilities ();
        BuildFullReachabilities ( pMethod );
        RemoveDeadBlocksInternal ();
    }

    bool BasicBlockPool::MergeBlocks ( MethodDef* pMethod )
    {
        bool bMergedAny = false;
        prevector < BasicBlock*, 8 > nonMergeableBBs;
        pMethod->ForEachEH (
            [&] ( MethodDef::EHInfo& eh )
            {
                BasicBlockPool::InputOffsetToBB_t::iterator bbs[5];
                bbs[0] = m_InputOffsetToBB.lower_bound ( eh.m_iTryOffset );
                bbs[1] = m_InputOffsetToBB.lower_bound ( eh.m_iTryOffset + eh.m_iTryLength );
                if ( eh.m_iFilterOffset >= 0 )
                    bbs[2] = m_InputOffsetToBB.lower_bound ( eh.m_iFilterOffset );
                else
                    bbs[2] = m_InputOffsetToBB.end ();
                bbs[3] = m_InputOffsetToBB.lower_bound ( eh.m_iCatchOffset );
                bbs[4] = m_InputOffsetToBB.lower_bound ( eh.m_iCatchOffset + eh.m_iCatchLength );

                for ( int i = 0; i < sizeof(bbs)/sizeof(bbs[0]); i++ )
                {
                    if ( bbs[i] != m_InputOffsetToBB.end () && !nonMergeableBBs.contains ( bbs[i]->second ) )
                        nonMergeableBBs.push_back ( bbs[i]->second );
                }
            }
        );

        for ( BasicBlock* pBB = GetLastBB (); pBB; pBB = pBB->GetOutputInfo ().m_pPrevBB )
        {
            if ( pBB->GetBranchType () == ILOPCODE_NOP &&
                 pBB->GetFallthrough () &&
                 pBB->GetFallthrough ()->NumIncoming () == 1 &&
                 !nonMergeableBBs.contains ( pBB->GetFallthrough () ) )
            {
                MergeBB ( pBB );
                bMergedAny = true;
            }
        }

        return bMergedAny;
    }

    void BasicBlockPool::ToMethod ( MethodDef* pMethod )
    {
        ToCode ( pMethod->Code (), pMethod->CodeSize () );
        FixEH ( pMethod );
    }

    dword BasicBlockPool::ToCode ( byte* pCode, dword dwSize )
    {
        CalcBranchLengths ();
        dword dwNewCodeSize = WriteBlocks ( pCode, dwSize );

        if ( dwNewCodeSize < dwSize )
        {
            // Instead of padding with nops at the end of the method, move the code
            // to the back of the method and pad with nops at the front. Otherwise
            // the CLR verifier will think execution flow can fall off the end of the method.
            memmove ( pCode + dwSize - dwNewCodeSize, pCode, dwNewCodeSize );
            memset ( pCode, ILOPCODE_NOP, dwSize - dwNewCodeSize );

            for ( auto it = Begin (); it != End (); it++ )
            {
                it->GetOutputInfo ().m_iOffset += dwSize - dwNewCodeSize;
            }
        }

        return dwNewCodeSize;
    }

    void BasicBlockPool::Delete ( BasicBlock* pBB )
    {
        if ( pBB == m_pFirstBB )
            m_pFirstBB = m_pFirstBB->GetOutputInfo ().m_pNextBB;

        if ( pBB == m_pLastBB )
            m_pLastBB = m_pLastBB->GetOutputInfo ().m_pPrevBB;

        m_InputOffsetToBB.erase ( pBB->GetOffset () );
        Pool::Delete ( pBB );
    }

    void BasicBlockPool::Clear ()
    {
        m_InputOffsetToBB.clear ();
        Pool::Clear ();
    }

    BasicBlock* BasicBlockPool::GetBBContaining ( dword dwOffset ) const
    {
        if ( m_InputOffsetToBB.empty () )
            return NULL;

        auto it = m_InputOffsetToBB.upper_bound ( dwOffset );
        if ( it == m_InputOffsetToBB.begin () )
            return NULL;

        it--;
        BasicBlock* pBB = it->second;
        if ( pBB->Contains ( dwOffset ) )
            return pBB;
        else if ( dwOffset == pBB->GetOffset () + pBB->GetLength () && pBB->GetBranchType () != ILOPCODE_NOP )
            return pBB;
        else
            return NULL;
    }

    BasicBlock* BasicBlockPool::GetFirstBBContainedIn ( dword dwOffset, dword dwLength ) const
    {
        if ( m_InputOffsetToBB.empty () )
            return NULL;

        auto it = m_InputOffsetToBB.lower_bound ( dwOffset );
        if ( it == m_InputOffsetToBB.end () || !it->second->IsContainedIn ( dwOffset, dwLength ) )
            return NULL;

        return it->second;
    }

    BasicBlock* BasicBlockPool::GetBBContainingOrBefore ( dword dwOffset ) const
    {
        if ( m_InputOffsetToBB.empty () )
            return NULL;

        InputOffsetToBB_t::const_iterator it = m_InputOffsetToBB.upper_bound ( dwOffset );
        if ( it == m_InputOffsetToBB.begin () )
            return NULL;

        it--;
        return it->second;
    }

    BasicBlock* BasicBlockPool::GetBBContainingOrAfter ( dword dwOffset ) const
    {
        if ( m_InputOffsetToBB.empty () )
            return NULL;

        return m_InputOffsetToBB.lower_bound ( dwOffset )->second;
    }

    BasicBlock* BasicBlockPool::SplitBB ( dword dwOffset, int iSkip, bool bLink )
    {
        return SplitBB ( GetBBContaining ( dwOffset ), dwOffset, iSkip, bLink );
    }

    BasicBlock* BasicBlockPool::SplitBB ( BasicBlock* pBB, dword dwOffset, int iSkip, bool bLink )
    {
        if ( !pBB )
            return NULL;

        // Splitting at the beginning or end of a block?
        if ( dwOffset + iSkip == pBB->GetOffset () )
        {
            return pBB;
        }
        else if ( dwOffset + iSkip == pBB->GetOffset () + pBB->GetLength () )
        {
            if ( iSkip > 0 )
            {
                pBB->SetLength ( dwOffset - pBB->GetOffset () );
                if ( !bLink )
                    pBB->SetFallthrough ( NULL );

                return pBB->GetInputInfo ().m_pNextBB;
            }
            else if ( iSkip == 0 && pBB->GetBranchType () == ILOPCODE_NOP )
            {
                if ( !bLink )
                    pBB->SetFallthrough ( NULL );

                return pBB->GetInputInfo ().m_pNextBB;
            }
        }

        assert ( ( pBB->Contains(dwOffset) && pBB->Contains(dwOffset + iSkip) ) ||
                 ( dwOffset + iSkip == pBB->GetOffset() + pBB->GetLength() ) );

        // Perform the split
        BasicBlock* pAfterSplitBB = Create ( this, dwOffset + iSkip, pBB->GetOffset () + pBB->GetLength () - (dwOffset + iSkip) );
        m_InputOffsetToBB [ dwOffset + iSkip ] = pAfterSplitBB;
        pBB->SetLength ( dwOffset - pBB->GetOffset () );
        pBB->GetInputInfo ().InsertAfter ( pAfterSplitBB );

        // Move the outgoing block references
        BasicBlock* pFallthroughBB = pBB->GetFallthrough ();
        if ( bLink )
            pBB->SetFallthrough ( pAfterSplitBB );
        else
            pBB->SetFallthrough ( NULL );
        pAfterSplitBB->SetFallthrough ( pFallthroughBB );

        pAfterSplitBB->SetBranchType ( pBB->GetBranchType () );
        pBB->SetBranchType ( ILOPCODE_NOP );
        while ( pBB->NumTargets () > 0 )
        {
            BasicBlock* pTargetBB = pBB->GetTarget ( 0 );
            pBB->RemoveTarget ( pTargetBB );
            pAfterSplitBB->AddTarget ( pTargetBB );
        }
    
        return pAfterSplitBB;
    }

    void BasicBlockPool::MergeBB ( BasicBlock* pBB )
    {
        BasicBlock* pNextBB = pBB->GetFallthrough ();
        assert ( pBB->GetBranchType () == ILOPCODE_NOP && pNextBB && pNextBB->NumIncoming () == 1 );

        InstrBuffer& instrs = pBB->GetInstrBuffer ();
        instrs.AppendInstrBuffer ( pNextBB->GetInstrBuffer () );
        
        BasicBlock* pNextBBFallthrough = pNextBB->GetFallthrough ();
        pNextBB->SetFallthrough ( NULL );
        pBB->SetBranchType ( pNextBB->GetBranchType () );
        pBB->SetFallthrough ( pNextBBFallthrough );
        for ( int i = 0; i < pNextBB->NumTargets (); i++ )
        {
            pBB->AddTarget ( pNextBB->GetTarget ( i ) );
        }

        Delete ( pNextBB );
    }

    void BasicBlockPool::AssignTryCatchBBPriorities ( MethodDef* pMethod )
    {
        ClearDFSState ();
        pMethod->ForEachEH (
            [&] ( MethodDef::EHInfo& eh )
            {
                if ( eh.m_iCatchLength == 0 )
                    return;

                DFSNoClear (
                    [=] ( BasicBlock* pBB ) -> bool
                    {
                        if ( !pBB->IsContainedIn ( eh.m_iCatchOffset, eh.m_iCatchLength ) )
                            return false;

                        pBB->SetFlag ( IN_CATCH );
                        pBB->GetOutputInfo ().m_iPriority++;
                        return true;
                    },
                    GetFirstBBContainedIn ( eh.m_iCatchOffset, eh.m_iCatchLength )
                );
            }
        );
    }

    bool BasicBlockPool::AssignLoopBBPriorities ( BasicBlock* pHeaderBB )
    {
        // This can only be a loop header block if there's at least one backjump
        if ( !pHeaderBB->GetIncoming ( [=](BasicBlock* pInBB) { return pInBB->IsReachableFrom(pHeaderBB); } ) )
            return false;

        int iNumLoopBBs = 0;

        // 1) All loop nodes are reachable from the header node
        // 2) The header node is reachable from all loop nodes
        for ( iterator it = Begin (); it != End (); it++ )
        {
            BasicBlock* pBB = &(*it);
            if ( pBB->IsReachableFrom ( pHeaderBB ) && pHeaderBB->IsReachableFrom ( pBB ) )
            {
                pBB->SetFlag ( IN_LOOP );
                iNumLoopBBs++;
            }
            else
            {
                pBB->ClearFlag ( IN_LOOP );
            }
        }

        // 3) No branches from outside the loop to inside the loop (excluding the header node)
        bool bScanRemoveAgain;
        do
        {
            bScanRemoveAgain = false;

            for ( iterator it = Begin (); it != End (); it++ )
            {
                BasicBlock* pBB = &(*it);
                if ( !pBB->HasFlag ( IN_LOOP ) || pBB == pHeaderBB )
                    continue;

                if ( !pBB->GetIncoming ( [=](BasicBlock* pInBB) { return !pInBB->HasFlag(this->IN_LOOP); } ) )
                    continue;

                pBB->ClearFlag ( IN_LOOP );
                iNumLoopBBs--;

                prevector < BasicBlock* > bbRemoveQueue;
                for ( int i = 0; i < pBB->NumIncoming (); i++ )
                {
                    BasicBlock* pInBB = pBB->GetIncoming ( i );
                    if ( pInBB->HasFlag ( IN_LOOP ) )
                        bbRemoveQueue.push_back ( pInBB );
                }
                while ( !bbRemoveQueue.empty () )
                {
                    BasicBlock* pRemoveBB = bbRemoveQueue.back ();
                    bbRemoveQueue.pop_back ();

                    if ( IsReachableWithFlag ( pRemoveBB, pHeaderBB, IN_LOOP ) )
                        continue;

                    pRemoveBB->ClearFlag ( IN_LOOP );
                    iNumLoopBBs--;
                    assert ( iNumLoopBBs >= 1 );

                    for ( int i = 0; i < pRemoveBB->NumIncoming (); i++ )
                    {
                        BasicBlock* pInBB = pRemoveBB->GetIncoming ( i );
                        if ( pInBB->HasFlag ( IN_LOOP ) && !bbRemoveQueue.contains ( pInBB ) )
                            bbRemoveQueue.push_back ( pInBB );
                    }
                }

                bScanRemoveAgain = true;
            }
        } while ( bScanRemoveAgain );

        // Is there a loop left at all?
        if ( iNumLoopBBs == 1 && !pHeaderBB->HasTarget ( pHeaderBB ) )
            return false;

        // Increase the priority of all BB's in the loop body so they will stick
        // together while ordering
        pHeaderBB->SetFlag ( IS_HEADER );
        for ( iterator it = Begin (); it != End (); it++ )
        {
            if ( !it->HasFlag ( IN_LOOP ) )
                continue;

			BasicBlock* pLoopBB = &(*it);
            BasicBlock::OutputInfo& outputInfo = it->GetOutputInfo ();
            outputInfo.m_iPriority++;
			outputInfo.m_LoopIDs.push_back ( m_iNextLoopID );
        }
        m_iNextLoopID++;
        return true;
    }

	void BasicBlockPool::SetCatchOutgoings ( MethodDef* pMethod )
	{
		dword dwLastTryOffset = 0;
		dword dwLastTryLength = 0;

		pMethod->ForEachEH (
			[&] ( MethodDef::EHInfo& eh )
			{
				if ( dwLastTryLength && (eh.m_iTryOffset != dwLastTryOffset || eh.m_iTryLength != dwLastTryLength) )
				{
					for ( iterator it = Begin (); it != End (); it++ )
					{
						if ( it->IsContainedIn ( dwLastTryOffset, dwLastTryLength ) )
							it->SetFlag ( HANDLED_EH, true );
					}
				}
				dwLastTryOffset = eh.m_iTryOffset;
				dwLastTryLength = eh.m_iTryLength;

				BasicBlock* pCatchBB = GetFirstBBContainedIn ( eh.m_iCatchOffset, eh.m_iCatchLength );
				for ( iterator it = Begin (); it != End (); it++ )
				{
					BasicBlock* pBB = &(*it);
					if ( !pBB->HasFlag ( HANDLED_EH ) && pBB->GetBranchType () == ILOPCODE_LEAVE &&
						 pBB->IsContainedIn ( eh.m_iTryOffset, eh.m_iTryLength ) )
					{
						pBB->AddEH ( pCatchBB );
					}
				}
			},
			[=] ( const MethodDef::EHInfo& eh1, const MethodDef::EHInfo& eh2 )
			{
				return eh1.m_iTryLength < eh2.m_iTryLength;
			}
		);
	}

	void BasicBlockPool::ClearCatchOutgoings ()
	{
		for ( iterator it = Begin (); it != End (); it++ )
		{
			it->ClearFlag ( HANDLED_EH );
			it->ClearEHs ();
		}
	}

	void BasicBlockPool::BuildFullReachabilities ( BasicBlock* pStartBB )
    {
        if ( pStartBB->HasFlag ( HANDLED_REACHABILITY ) )
            return;

        prevector < BasicBlock*, 0x40 > bbPath;
        pStartBB->AddReachableFrom ( pStartBB );
        bbPath.push_back ( pStartBB );

        while ( !bbPath.empty () )
        {
            BasicBlock* pBB = bbPath.back ();
            pBB->SetFlag ( HANDLED_REACHABILITY );
            BasicBlock* pNextBB = NULL;
            for ( int i = 0; i < pBB->NumOutgoing (); i++ )
            {
                BasicBlock* pOutBB = pBB->GetOutgoing ( i );
                if ( !pNextBB && pOutBB->AddReachableFrom ( pBB ) )
                {
                    pNextBB = pOutBB;
                    break;
                }
            }

            if ( pNextBB )
                bbPath.push_back ( pNextBB );
            else
                bbPath.pop_back ();
        }
    }

	void BasicBlockPool::BuildFullReachabilities ( MethodDef* pMethod )
	{
		BuildFullReachabilities ( m_pFirstBB );

		pMethod->ForEachEH (
			[=] ( MethodDef::EHInfo& eh )
			{
				if ( eh.m_iCatchLength > 0 )
					BuildFullReachabilities ( GetFirstBBContainedIn ( eh.m_iCatchOffset, eh.m_iCatchLength ) );
			}
		);
	}

    void BasicBlockPool::BuildPartialReachabilities ( BasicBlock* pStartBB, bool bConnectLoopEndStart )
    {
        if ( pStartBB->HasFlag ( HANDLED_REACHABILITY ) )
            return;

        prevector < BasicBlock*, 0x40 > bbPath;
        pStartBB->AddReachableFrom ( pStartBB );
        bbPath.push_back ( pStartBB );

        while ( !bbPath.empty () )
        {
            BasicBlock* pBB = bbPath.back ();
            pBB->SetFlag ( HANDLED_REACHABILITY );
            BasicBlock* pNextBB = NULL;
            for ( int i = 0; i < pBB->NumOutgoing (); i++ )
            {
                BasicBlock* pOutBB = pBB->GetOutgoing ( i );
				if ( !pOutBB->HasFlag ( IS_HEADER ) ||
					 pOutBB->NumLoops () > pBB->NumLoops () ||
					 (pOutBB->NumLoops () == pBB->NumLoops () && !pOutBB->IsInSameLoops(pBB)) )
				{
					if ( pOutBB->AddReachableFrom ( pBB ) )
					{
						pNextBB = pOutBB;
						break;
					}
				}
            }

            if ( pNextBB )
                bbPath.push_back ( pNextBB );
            else
                bbPath.pop_back ();
        }

		if ( bConnectLoopEndStart )
			ConnectPartialReachabilityLoopEndStart ();
    }

    void BasicBlockPool::BuildPartialReachabilities ( MethodDef* pMethod )
    {
        BuildPartialReachabilities ( m_pFirstBB, false );
        pMethod->ForEachEH (
            [&] ( MethodDef::EHInfo& eh )
            {
                if ( eh.m_iCatchLength > 0 )
                    BuildPartialReachabilities ( GetFirstBBContainedIn ( eh.m_iCatchOffset, eh.m_iCatchLength ), false );
            }
        );

		ConnectPartialReachabilityLoopEndStart ();
    }

	void BasicBlockPool::ConnectPartialReachabilityLoopEndStart ()
	{
		for ( iterator it = Begin (); it != End (); it++ )
		{
			BasicBlock* pBB = &(*it);
			for ( int i = 0; i < pBB->NumOutgoing (); i++ )
			{
				BasicBlock* pOutBB = pBB->GetOutgoing ( i );
				if ( pOutBB->HasFlag ( IS_HEADER ) && pOutBB->IsInSameLoops ( pBB ) )
					pOutBB->AddReachableFrom ( pBB );
			}
		}
	}

    void BasicBlockPool::ResetReachabilities ()
    {
        for ( iterator it = Begin (); it != End (); it++ )
        {
            it->ClearFlag ( HANDLED_REACHABILITY );
            it->ResetReachability ();
        }
    }

    void BasicBlockPool::RemoveDeadBlocksInternal ()
    {
        for ( iterator it = Begin (); it != End (); it++ )
        {
            if ( !it->HasFlag ( HANDLED_REACHABILITY ) )
                Delete ( &(*it) );
        }
    }

    BasicBlock* BasicBlockPool::OrderBlocks ( MethodDef* pMethod, BasicBlock* pStartBB )
    {
        typedef prioritystack < BasicBlock*, 0x20 > Queue_t;
        Queue_t bbQueue;

        #define Enqueue(_pBB)                                                    \
        {                                                                        \
            bbQueue.remove ( (_pBB) );                                           \
            bbQueue.push_back ( (_pBB)->GetOutputInfo ().m_iPriority, (_pBB) );  \
        }

		/*
		pMethod->ForEachEH (
			[&] ( MethodDef::EHInfo& eh )
			{
				BasicBlock* pCatchBB = GetFirstBBContainedIn ( eh.m_pCatchBegin, eh.m_iCatchLength );
				if ( !pCatchBB->GetOutputInfo ().IsOrdered () )
					bbQueue.push_back ( -1, pCatchBB );
			}
		);
		*/

        Enqueue ( pStartBB );
        BasicBlock* pPrevBB = NULL;
		int iIndex = 0;
        while ( !bbQueue.empty () )
        {
			if ( (bbQueue.end () - 1).priority () < 0 )
			{
				bbQueue.pop_back ();
				continue;
			}

            BasicBlock* pBB = bbQueue.back ();
            bbQueue.pop_back ();

            if ( pBB->GetOutputInfo ().IsOrdered () )
                continue;

			if ( !pPrevBB || pPrevBB->GetFallthrough () != pBB )
			{
				if ( pBB->HasFlag ( IS_HEADER ) )
				{
					bool bIsLoopEnd = false;
					if ( !pBB->GetFallthroughIncoming () )
					{
						bIsLoopEnd = pBB->GetIncoming (
							[=] ( BasicBlock* pIncomingBB ) -> bool
							{
								return !pIncomingBB->IsInSameLoops ( pBB );
							}
						) != NULL;
					}
					else
					{
						bIsLoopEnd = !pBB->GetFallthroughIncoming ()->GetOutputInfo ().IsOrdered();
					}

					if ( bIsLoopEnd )
					{
						// For loops that are (probably) not a do-while, find the loop end BB as such:
						// - is in the same loop(s)
						// - has a target BB with the same loop id and which has no fallthrough incoming
						// - has a fallthrough that leaves the loop
						// the target BB will be the new loop begin

						BasicBlock* pLoopStartBB = NULL;
						DFS (
							[=, &pLoopStartBB] ( BasicBlock* pLoopEndBB ) -> bool
							{
								BasicBlock* pHeaderBB = pBB;

								if ( !pLoopEndBB->IsInSameLoops ( pHeaderBB ) )
									return true;

								if ( !pLoopEndBB->GetFallthrough () ||
									 pLoopEndBB->GetFallthrough ()->IsInSameLoops ( pHeaderBB ) )
									return true;

								pLoopStartBB = pLoopEndBB->GetTarget (
									[=] ( BasicBlock* pLoopStartBB ) -> bool
									{
										return pLoopStartBB->IsInSameLoops ( pHeaderBB ) &&
											   !pLoopStartBB->GetFallthroughIncoming ();
									}
								);
								return !pLoopStartBB;
							},
							pBB
						);

						if ( pLoopStartBB && !pLoopStartBB->GetOutputInfo ().IsOrdered () )
							pBB = pLoopStartBB;
					}
				}

				if ( !bbQueue.empty () )
				{
					// If there is already a BB in the queue from which we can reach the current
					// BB, use that one instead. (e.g. make sure to order all cases of a switch before
					// leaving the switch)
					Queue_t::iterator it = bbQueue.end ();
					BasicBlock* pReachBB = NULL;
					do
					{
						--it;
						if ( pBB->IsReachableFrom ( *it ) && *it != pBB && /*!(*it)->HasFlag ( IS_HEADER ) &&*/
							 /*!( pBB->HasFlag(IS_HEADER) && (*it)->HasTarget(pBB) && (*it)->IsInSameLoops(pBB) ) &&*/
							 IsReachable ( *it, pBB, [](BasicBlock* pBB) { return !pBB->GetOutputInfo ().IsOrdered(); } ) )
						{
							pReachBB = *it;
							break;
						}
					} while ( it != bbQueue.begin () );

					if ( pReachBB )
					{
						if ( pBB->GetOutputInfo ().m_iPriority <= pReachBB->GetOutputInfo ().m_iPriority )
							Enqueue ( pBB );

						Enqueue ( pReachBB );
						continue;
					}
				}
			}

			pBB->GetOutputInfo ().m_iIndex = iIndex++;
            pBB->GetOutputInfo ().LinkPrevious ( pPrevBB );
            pPrevBB = pBB;

            for ( int i = pBB->NumTargets () - 1; i >= 0; i-- )
            {
                BasicBlock* pTargetBB = pBB->GetTarget ( i );
                if ( !pTargetBB->GetOutputInfo ().IsOrdered () )
                    Enqueue ( pTargetBB );
            }
            if ( pBB->GetFallthrough () )
            {
                BasicBlock* pFallthroughBB = pBB->GetFallthrough ();
                assert ( !pFallthroughBB->GetOutputInfo ().IsOrdered () );
                bbQueue.remove ( pFallthroughBB );
                bbQueue.push_back ( 9000 + pFallthroughBB->GetOutputInfo ().m_iPriority, pFallthroughBB );
            }
        }

        #undef Enqueue
        return pPrevBB;
    }

    void BasicBlockPool::CalcBranchLengths ()
    {
        BasicBlock* pBB = m_pFirstBB;
        int iIndex = 0;
        int iOffset = 0;
        do
        {
            BasicBlock::OutputInfo& outputInfo = pBB->GetOutputInfo ();
            outputInfo.m_iIndex = iIndex;
            outputInfo.m_iOffset = iOffset;

            if ( pBB->NumTargets () == 0 )
            {
                outputInfo.m_iBranchLength = 0;
            }
            else if ( pBB->GetBranchType () == ILOPCODE_SWITCH )
            {
                outputInfo.m_iBranchLength = 1 + 4*( 1 + pBB->NumTargets () );
            }
            else
            {
                outputInfo.m_iBranchLength = 2;
            }

            iIndex++;
            iOffset += pBB->GetLength () + outputInfo.m_iBranchLength;

            pBB = outputInfo.m_pNextBB;
        } while ( pBB );

        bool bChanged;
        do
        {
            bChanged = false;
            pBB = m_pFirstBB;
            do
            {
                BasicBlock::OutputInfo& outputInfo = pBB->GetOutputInfo ();

                if ( outputInfo.m_iBranchLength == 2 )
                {
                    BasicBlock* pTargetBB = pBB->GetTarget ();
                    BasicBlock::OutputInfo& targetOutputInfo = pTargetBB->GetOutputInfo ();

                    bool bNeed5 = false;
                    int iBranchEndOffset = outputInfo.m_iOffset + pBB->GetLength () +
                        outputInfo.m_iBranchLength;
                    if ( targetOutputInfo.m_iIndex <= outputInfo.m_iIndex )
                    {
                        bNeed5 = targetOutputInfo.m_iOffset - iBranchEndOffset < -0x80;
                    }
                    else
                    {
                        bNeed5 = targetOutputInfo.m_iOffset - iBranchEndOffset >= 0x80;
                    }

                    if ( bNeed5 )
                    {
                        outputInfo.m_iBranchLength = 5;
                        BasicBlock* pLoopBB = outputInfo.m_pNextBB;
                        while ( pLoopBB )
                        {
                            BasicBlock::OutputInfo& loopOutputInfo = pLoopBB->GetOutputInfo ();
                            loopOutputInfo.m_iOffset += 5 - 2;
                            pLoopBB = loopOutputInfo.m_pNextBB;
                        }

                        bChanged = true;
                    }
                }

                pBB = outputInfo.m_pNextBB;
            } while ( pBB );
        } while ( bChanged );
    }

    int BasicBlockPool::WriteBlocks ( byte* pCode, int iSize )
    {
        int iOutputSize = 0;
        for ( BasicBlock* pBB = m_pFirstBB; pBB; pBB = pBB->GetOutputInfo ().m_pNextBB )
        {
            BasicBlock::OutputInfo& outputInfo = pBB->GetOutputInfo ();
            int iBBEndOffset = outputInfo.m_iOffset + pBB->GetLength () + outputInfo.m_iBranchLength;
            assert ( outputInfo.m_iOffset >= 0 && iBBEndOffset <= iSize );

            iOutputSize = std::max ( iOutputSize, iBBEndOffset );

            memcpy ( pCode + outputInfo.m_iOffset, pBB->Code (), pBB->GetLength () );
        
            if ( pBB->NumTargets () == 0 )
                continue;

            byte* pBranch = pCode + outputInfo.m_iOffset + pBB->GetLength ();

            if ( pBB->GetBranchType () == ILOPCODE_SWITCH )
            {
                m_BBTargetOutAddrProvider.Setup ( pBB, pCode );
                ILInstr::WriteSwitch ( pBranch, pBB->NumTargets (),
                    MakeDelegate ( &m_BBTargetOutAddrProvider, &BBTargetOutAddrProvider::GetTargetAddr ) );
                continue;
            }

            BasicBlock* pTargetBB = pBB->GetTarget ();
            byte* pBranchTarget = pCode + pTargetBB->GetOutputInfo ().m_iOffset;
        
            if ( pBB->GetBranchType () == ILOPCODE_LEAVE )
            {
                ILInstr::WriteLeave ( pBranch, pBranchTarget );
            }
            else
            {
                ILInstr::WriteBranch ( pBB->GetBranchType (), pBranch, pBranchTarget ); 
            }
        }
        return iOutputSize;
    }

    void BasicBlockPool::FixEH ( MethodDef* pMethod )
    {
        byte* pCode = pMethod->Code ();

        pMethod->ForEachEH(
            [&] ( MethodDef::EHInfo& eh )
            {
                BasicBlock* pTryBB = GetFirstBBContainedIn ( eh.m_iTryOffset, eh.m_iTryLength );
                eh.m_iTryLength = GetOutputLength ( eh.m_iTryOffset, eh.m_iTryLength );
                eh.m_iTryOffset = pTryBB->GetOutputInfo ().m_iOffset;

                if ( eh.m_iFilterOffset >= 0 )
                {
                    BasicBlock* pFilterBB = GetBBContainingOrAfter ( eh.m_iFilterOffset );
                    eh.m_iFilterOffset = pFilterBB->GetOutputInfo ().m_iOffset;
                }
            
                BasicBlock* pCatchBB = GetFirstBBContainedIn ( eh.m_iCatchOffset, eh.m_iCatchLength );
                eh.m_iCatchLength = GetOutputLength ( eh.m_iCatchOffset, eh.m_iCatchLength );
                eh.m_iCatchOffset = pCatchBB->GetOutputInfo ().m_iOffset;
            }
        );
    }

	bool BasicBlockPool::IsReachable ( BasicBlock* pFromBB, BasicBlock* pToBB, std::tr1::function < bool (BasicBlock*) > predicate )
	{
		bool bReachable = false;
		DFS (
			[=, &bReachable] ( BasicBlock* pBB ) -> bool
			{
				if ( bReachable )
					return false;

				if ( !predicate ( pBB ) )
					return false;

				if ( pBB == pToBB )
				{
					bReachable = true;
					return false;
				}

				return true;
			},
			pFromBB
		);
		return bReachable;
	}

	bool BasicBlockPool::IsReachableWithFlag( BasicBlock* pFromBB, BasicBlock* pToBB, dword dwFlag )
	{
		return IsReachable ( pFromBB, pToBB, [=] ( BasicBlock* pBB ) { return pBB->HasFlag ( dwFlag ); } );
    }

	bool BasicBlockPool::IsReachableWithoutFlag ( BasicBlock* pFromBB, BasicBlock* pToBB, dword dwFlag )
	{
		return IsReachable ( pFromBB, pToBB, [=] ( BasicBlock* pBB ) { return !pBB->HasFlag ( dwFlag ); } );
	}

    dword BasicBlockPool::GetOutputLength ( dword dwInputOffset, dword dwInputLength )
    {
        BasicBlock* pBB = GetFirstBBContainedIn ( dwInputOffset, dwInputLength );
        if ( !pBB )
            return 0;

        dword dwOutputLength = 0;
        do
        {
            BasicBlock::OutputInfo& outputInfo = pBB->GetOutputInfo ();
            dwOutputLength += pBB->GetLength () + outputInfo.m_iBranchLength;
            pBB = outputInfo.m_pNextBB;
        } while ( pBB && pBB->IsContainedIn ( dwInputOffset, dwInputLength ) );
        return dwOutputLength;
    }

    // ---------------------------------------
    //  BBTargetAddrProvider inner class
    // ---------------------------------------

    BasicBlockPool::BBTargetOutAddrProvider::BBTargetOutAddrProvider ()
    {
        m_pBB = NULL;
    }

    void BasicBlockPool::BBTargetOutAddrProvider::Setup ( BasicBlock* pBB, byte* pOutputCode )
    {
        m_pBB = pBB;
        m_pOutputCode = pOutputCode;
    }

    byte* BasicBlockPool::BBTargetOutAddrProvider::GetTargetAddr ( dword i )
    {
        return m_pOutputCode + m_pBB->GetTarget ( i )->GetOutputInfo ().m_iOffset;
    }

}
