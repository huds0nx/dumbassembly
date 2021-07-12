#include "StdInc.h"

namespace dotnetpe
{

    BasicBlock::BasicBlock ( BasicBlockPool* pPool )
        : m_InputInfo ( this ), m_OutputInfo ( this )
    {
        m_pPool = pPool;
        m_dwLength = 0;
        m_dwFlags = 0;
        m_pFallthroughBB = NULL;
        m_ucBranchType = ILOPCODE_NOP;
        m_bFetched = false;
    }

    BasicBlock::BasicBlock ( BasicBlockPool* pPool, dword dwOffset, dword dwLength )
        : m_InputInfo ( this ), m_OutputInfo ( this )
    {
        m_pPool = pPool;
        m_InputInfo.m_iOffset = dwOffset;
        m_dwLength = dwLength;
        m_dwFlags = 0;
        m_pFallthroughBB = NULL;
        m_ucBranchType = ILOPCODE_NOP;
        m_bFetched = false;
    }

    BasicBlock::~BasicBlock ()
    {
        for ( int i = NumTargets () - 1; i >= 0; i--)
        {
            SetTarget ( i, NULL );
        }
        SetFallthrough ( NULL );

        while ( NumIncoming () > 0 )
        {
            BasicBlock* pIncomingBB = GetIncoming ( 0 );

            if ( pIncomingBB->GetFallthrough () == this )
                pIncomingBB->SetFallthrough ( GetOutputInfo ().m_pNextBB );

            for ( int i = pIncomingBB->NumTargets () - 1; i >= 0; i-- )
            {
                if ( pIncomingBB->GetTarget ( i ) == this )
                    pIncomingBB->SetTarget ( i, GetOutputInfo ().m_pNextBB );
            }
        }

        m_InputInfo.Unlink ();
        m_OutputInfo.Unlink ();
    }

    int BasicBlock::Index () const
    {
        return m_pPool->Index ( this );
    }

    dword BasicBlock::GetLength () const
    {
        if ( !m_bFetched )
            return m_dwLength;
        else
            return m_Buffer.Size ();
    }

    void BasicBlock::SetLength ( dword dwLength )
    {
        if ( !m_bFetched )
            m_dwLength = dwLength;
        else
            m_Buffer.Resize ( dwLength );
    }

    void BasicBlock::SetBranchType ( byte ucBranchType )
    {
        for ( dword i = 0; i < NumTargets (); i++ )
        {
            BasicBlock* pTargetBB = GetTarget ( i );
            IncomingInfo lookup ( this, m_ucBranchType );
            Incomings_t::iterator it = pTargetBB->m_Incoming.find ( lookup );
            if ( it != pTargetBB->m_Incoming.end () )
                it->m_ucBranchType = ucBranchType;
        }
        m_ucBranchType = ucBranchType;
    }

    bool BasicBlock::Contains ( dword dwOffset ) const
    {
        return dwOffset >= GetOffset () && ( GetLength () > 0 ? dwOffset < GetOffset () + GetLength () : dwOffset <= GetOffset () + GetLength () );
    }

    bool BasicBlock::IsContainedIn ( dword dwOffset, dword dwLength ) const
    {
        int iBranchLength = std::max ( m_OutputInfo.m_iBranchLength, 0 );
        return GetOffset () >= dwOffset && GetOffset () + GetLength () + iBranchLength <= dwOffset + dwLength;
    }

    BasicBlock* BasicBlock::GetIncoming ( std::tr1::function < bool ( BasicBlock* ) >&& predicate )
    {
        Incomings_t::iterator it;
        for ( it = m_Incoming.begin (); it != m_Incoming.end (); it++ )
        {
            if ( predicate ( it->m_pBB ) )
                return it->m_pBB;
        }
        return NULL;
    }

    BasicBlock* BasicBlock::GetFallthroughIncoming ()
    {
        Incomings_t::iterator it;
        for ( it = m_Incoming.begin (); it != m_Incoming.end (); it++ )
        {
            if ( it->m_ucBranchType == ILOPCODE_NOP )
                return it->m_pBB;
        }
        return NULL;
    }

    bool BasicBlock::HasTargetIncomings ()
    {
        Incomings_t::iterator it;
        for ( it = m_Incoming.begin (); it != m_Incoming.end (); it++ )
        {
            if ( it->m_ucBranchType != ILOPCODE_NOP )
                return true;
        }
        return false;
    }

    bool BasicBlock::HasIncoming ( BasicBlock* pBlock )
    {
        Incomings_t::iterator it;
        for ( it = m_Incoming.begin (); it != m_Incoming.end (); it++ )
        {
            if ( it->m_pBB == pBlock )
                return true;
        }
        return false;
    }

    bool BasicBlock::HasIncoming ( BasicBlock* pBlock, byte ucBranchType )
    {
        return m_Incoming.contains ( IncomingInfo ( pBlock, ucBranchType ) );
    }

    BasicBlock* BasicBlock::GetTarget ( std::tr1::function < bool ( BasicBlock* ) >&& predicate )
    {
        for ( int i = 0; i < NumTargets (); i++ )
        {
            BasicBlock* pTargetBB = GetTarget ( i );
            if ( predicate ( pTargetBB ) )
                return pTargetBB;
        }
        return NULL;
    }

    bool BasicBlock::HasTarget ( BasicBlock* pBlock )
    {
        for ( int i = 0; i < NumTargets (); i++ )
        {
            if ( GetTarget ( i ) == pBlock )
                return true;
        }
        return false;
    }

    bool BasicBlock::HasTarget ( BasicBlock* pBlock, byte ucBranchType )
    {
        return ucBranchType == m_ucBranchType && HasTarget ( pBlock );
    }

    int BasicBlock::NumOutgoing () const
    {
        return ( m_pFallthroughBB ? 1 : 0 ) + NumTargets () + NumEHs ();
    }

    BasicBlock* BasicBlock::GetOutgoing ( dword i )
    {
		if ( m_pFallthroughBB )
		{
			if ( i == 0 )
				return m_pFallthroughBB;

			i--;
		}

        if ( i < NumTargets () )
			return GetTarget ( i );
		
		i -= NumTargets ();
		return GetEH ( i );
    }

    BasicBlock* BasicBlock::GetOutgoing ( std::tr1::function < bool ( BasicBlock* ) >&& predicate )
    {
        for ( int i = 0; i < NumOutgoing (); i++ )
        {
            BasicBlock* pOutBB = GetOutgoing ( i );
            if ( predicate ( pOutBB ) )
                return pOutBB;
        }
        return NULL;
    }

    bool BasicBlock::HasOutgoing ( BasicBlock* pBlock )
    {
        for ( int i = 0; i < NumOutgoing (); i++ )
        {
            if ( GetOutgoing ( i ) == pBlock )
                return true;
        }
        return false;
    }

    bool BasicBlock::HasOutgoing ( BasicBlock* pBlock, byte ucBranchType )
    {
        if ( ucBranchType == ILOPCODE_NOP )
            return GetFallthrough () == pBlock;
        else if ( ucBranchType == ucBranchType )
            return HasTarget ( pBlock );
        else
            return false;
    }

    void BasicBlock::AddIncoming ( BasicBlock* pBB, byte ucBranchType )
    {
        IncomingInfo info ( pBB, ucBranchType );
        if ( !m_Incoming.contains ( info ) )
        {
            if ( ucBranchType == ILOPCODE_NOP )
                assert ( !GetFallthroughIncoming () );

            m_Incoming.push_back ( info );
        }
    }

    void BasicBlock::RemoveIncoming ( BasicBlock* pBB )
    {
        Incomings_t::iterator it;
        for ( it = m_Incoming.begin (); it != m_Incoming.end (); it++ )
        {
            if ( it->m_pBB == pBB )
            {
                m_Incoming.erase ( it );
                break;
            }
        }
    }

    void BasicBlock::RemoveIncoming ( BasicBlock* pBB, byte ucBranchType )
    {
        IncomingInfo info ( pBB, ucBranchType );
        m_Incoming.remove ( info );
    }

    void BasicBlock::AddTarget ( BasicBlock* pTargetBB )
    {
        SetTarget ( m_Targets.size (), pTargetBB );
    }

    void BasicBlock::SetTarget ( BasicBlock* pTargetBB )
    {
        SetTarget ( 0, pTargetBB );
    }

    void BasicBlock::SetTarget ( int i, BasicBlock* pTargetBB )
    {
        assert ( m_ucBranchType != ILOPCODE_NOP );
        if ( pTargetBB )
        {
            if ( i >= m_Targets.size () )
                m_Targets.resize ( 1 + i );

            if ( m_Targets[i].m_pBB )
                m_Targets[i].m_pBB->RemoveIncoming ( this, m_ucBranchType );

            m_Targets[i].m_pBB = pTargetBB;

            pTargetBB->AddIncoming ( this, m_ucBranchType );
        }
        else
        {
            assert ( i < m_Targets.size () );

            if ( m_Targets[i].m_pBB )
                m_Targets[i].m_pBB->RemoveIncoming ( this, m_ucBranchType );

            m_Targets.erase ( i );
        }
    }

    void BasicBlock::RemoveTarget ( BasicBlock* pTargetBB )
    {
        pTargetBB->RemoveIncoming ( this, m_ucBranchType );
        m_Targets.remove ( pTargetBB );
    }


	void BasicBlock::AddEH ( BasicBlock* pEHBB )
	{
		if ( m_EHs.contains ( pEHBB ) )
			return;

		m_EHs.push_back ( TargetInfo ( pEHBB ) );
		pEHBB->AddIncoming ( this, ILOPCODE_THROW );
	}

	void BasicBlock::ClearEHs ()
	{
		for ( Targets_t::iterator it = m_EHs.begin (); it != m_EHs.end (); it++ )
		{
			it->m_pBB->RemoveIncoming ( this, ILOPCODE_THROW );
		}
		m_EHs.clear ();
	}

    void BasicBlock::SetFallthrough ( BasicBlock* pFallthroughBB )
    {
        if ( m_pFallthroughBB )
            m_pFallthroughBB->RemoveIncoming ( this, ILOPCODE_NOP );

        m_pFallthroughBB = pFallthroughBB;

        if ( m_pFallthroughBB )
            m_pFallthroughBB->AddIncoming ( this, ILOPCODE_NOP );
    }

    bool BasicBlock::IsReachableFrom ( BasicBlock* pBB ) const
    {
        return pBB->Index () < m_ReachableFrom.size () && m_ReachableFrom [ pBB->Index () ];
    }

    bool BasicBlock::AddReachableFrom ( BasicBlock* pBB )
    {
        if ( m_ReachableFrom.empty () )
        {
            m_ReachableFrom.resize ( m_pPool->Size () );
            m_ReachableFrom [ Index () ] = true;
        }

        if ( pBB != this )
        {
            bool bResult = (m_ReachableFrom | pBB->m_ReachableFrom) != m_ReachableFrom;
            if ( bResult )
                m_ReachableFrom |= pBB->m_ReachableFrom;
            return bResult;
        }
        return false;
    }

    void BasicBlock::ResetReachability ()
    {
        m_ReachableFrom.clear ();
    }

	bool BasicBlock::IsInSameLoops ( BasicBlock* pBB )
	{
		if ( m_OutputInfo.m_LoopIDs.size () != pBB->GetOutputInfo ().m_LoopIDs.size () )
			return false;

		for ( int i = 0; i < m_OutputInfo.m_LoopIDs.size (); i++ )
		{
			if ( !pBB->GetOutputInfo ().m_LoopIDs.contains ( m_OutputInfo.m_LoopIDs[i] ) )
				return false;
		}
		return true;
	}

	int BasicBlock::NumLoops () const
	{
		return m_OutputInfo.m_LoopIDs.size ();
	}

    byte* BasicBlock::Code ()
    {
        return GetInstrBuffer ().Data ();
    }

    InstrBuffer& BasicBlock::GetInstrBuffer ()
    {
        assert ( m_bFetched );
        return m_Buffer;
    }

    void BasicBlock::FetchCodeFromMethod ( byte* pMethod )
    {
        if ( m_bFetched )
            return;

        m_Buffer.Assign ( pMethod + GetOffset (), GetLength () );
        m_bFetched = true;
    }
}
