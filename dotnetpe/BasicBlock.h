#pragma once

namespace dotnetpe
{

    class BasicBlockPool;

    class BasicBlock
    {
        friend BasicBlockPool;

    public:
                                BasicBlock              ( BasicBlockPool* pPool );
                                BasicBlock              ( BasicBlockPool* pPool, dword dwOffset, dword dwLength );
                                ~BasicBlock             ();

        int                     Index                   () const;
        BasicBlockPool*         Pool                    () const    { return m_pPool; }

        byte*                   Code                    ();
        InstrBuffer&            GetInstrBuffer          ();
        dword                   GetOffset               () const    { return m_InputInfo.m_iOffset; }
        dword                   GetLength               () const;
        dword                   GetFlags                () const    { return m_dwFlags; }
        byte                    GetBranchType           () const    { return m_ucBranchType; }

        void                    SetLength               ( dword dwLength );
        void                    SetFlags                ( dword dwFlags )       { m_dwFlags = dwFlags; }
        void                    SetBranchType           ( byte ucType );

        bool                    HasFlag                 ( dword dwFlag ) const  { return (m_dwFlags & dwFlag) != 0; }
        dword                   GetFlag                 ( dword dwMask ) const  { return m_dwFlags & dwMask; }
        void                    SetFlag                 ( dword dwFlag )        { m_dwFlags |= dwFlag; }
        void                    SetFlag                 ( dword dwFlag, bool bValue )  { SetFlag ( dwFlag, bValue ? dwFlag : 0 ); }
        void                    SetFlag                 ( dword dwMask, dword dwFlag ) { m_dwFlags = (m_dwFlags & ~dwMask) | dwFlag; }
        void                    ClearFlag               ( dword dwFlag )               { m_dwFlags &= ~dwFlag; }

        bool                    Contains                ( dword dwOffset ) const;
        bool                    IsContainedIn           ( dword dwOffset, dword dwLength ) const;

        struct IncomingInfo
        {
                                IncomingInfo            ()
            {
                m_pBB = NULL;
                m_ucBranchType = ILOPCODE_NOP;
            }

                                IncomingInfo            ( BasicBlock* pBB, byte ucBranchType )
            {
                m_pBB = pBB;
                m_ucBranchType = ucBranchType;
            }

            bool                operator==              ( const IncomingInfo& other ) const
            {
                return m_pBB == other.m_pBB && m_ucBranchType == other.m_ucBranchType;
            }

            bool                operator!=              ( const IncomingInfo& other ) const
            {
                return !(*this == other);
            }

            BasicBlock*         m_pBB;
            byte                m_ucBranchType;
        };
        int                     NumIncoming             () const    { return m_Incoming.size (); }
        IncomingInfo            GetIncomingInfo         ( dword i ) { return m_Incoming[i]; }
        BasicBlock*             GetIncoming             ( dword i ) { return m_Incoming[i].m_pBB; }
        BasicBlock*             GetIncoming             ( std::tr1::function < bool ( BasicBlock* ) >&& predicate );
        BasicBlock*             GetFallthroughIncoming  ();
        bool                    HasTargetIncomings      ();
        bool                    HasIncoming             ( BasicBlock* pBlock );
        bool                    HasIncoming             ( BasicBlock* pBlock, byte ucBranchType );

        struct TargetInfo
        {
                                TargetInfo              () : m_pBB ( NULL ) {}
                                TargetInfo              ( BasicBlock* pBB ) : m_pBB ( pBB ) {}

            bool                operator==              ( const TargetInfo& other ) const
            {
                return m_pBB == other.m_pBB;
            }

            bool                operator!=              ( const TargetInfo& other ) const
            {
                return !(*this == other);
            }

            BasicBlock*         m_pBB;
        };
        int                     NumTargets              () const        { return m_Targets.size (); }
        BasicBlock*             GetTarget               ( dword i = 0 ) { return m_Targets[i].m_pBB; }
        BasicBlock*             GetTarget               ( std::tr1::function < bool ( BasicBlock* ) >&& predicate );
        bool                    HasTarget               ( BasicBlock* pBlock );
        bool                    HasTarget               ( BasicBlock* pBlock, byte ucBranchType );
        BasicBlock*             GetFallthrough          () const        { return m_pFallthroughBB; }
		int						NumEHs					() const		{ return m_EHs.size (); }
		BasicBlock*				GetEH					( dword i )		{ return m_EHs[i].m_pBB; }
        int                     NumOutgoing             () const;
        BasicBlock*             GetOutgoing             ( dword i );
        BasicBlock*             GetOutgoing             ( std::tr1::function < bool ( BasicBlock* ) >&& predicate );
        bool                    HasOutgoing             ( BasicBlock* pBlock );
        bool                    HasOutgoing             ( BasicBlock* pBlock, byte ucBranchType );

        void                    AddTarget               ( BasicBlock* pTargetBB );
        void                    SetTarget               ( BasicBlock* pTargetBB );
        void                    SetTarget               ( int i, BasicBlock* pTargetBB );
        void                    RemoveTarget            ( BasicBlock* pTargetBB );
		void					AddEH					( BasicBlock* pEHBB );
		void					ClearEHs				();
        void                    SetFallthrough          ( BasicBlock* pFallthroughBB );

        bool                    IsReachableFrom         ( BasicBlock* pBB ) const;
        bool                    AddReachableFrom        ( BasicBlock* pBB );
        void                    ResetReachability       ();

		bool					IsInSameLoops			( BasicBlock* pBB );
		int						NumLoops				() const;

        template < class Derived >
        struct LinkNode
        {
                                    LinkNode                ( BasicBlock* pBB )
            {
                m_pBB = pBB;
                m_pPrevBB = NULL;
                m_pNextBB = NULL;
            }

            void                    InsertBefore            ( BasicBlock* pInsertBB )
            {
                LinkNode < Derived >& insertLinkInfo = Derived::GetDerivedLinkNode ( pInsertBB );
                insertLinkInfo.Unlink ();
                insertLinkInfo.LinkPrevious ( m_pPrevBB );
                insertLinkInfo.LinkNext ( m_pBB );
            }

            void                    InsertAfter             ( BasicBlock* pInsertBB )
            {
                LinkNode < Derived >& insertLinkInfo = Derived::GetDerivedLinkNode ( pInsertBB );
                insertLinkInfo.Unlink ();
                insertLinkInfo.LinkNext ( m_pNextBB );
                insertLinkInfo.LinkPrevious ( m_pBB );
            }

            BasicBlock*             LinkPrevious            ( BasicBlock* pPrevBB )
            {
                BasicBlock* pOldPrevBB = m_pPrevBB;

                if ( m_pPrevBB )
                    Derived::GetDerivedLinkNode ( m_pPrevBB ).m_pNextBB = NULL;

                m_pPrevBB = pPrevBB;

                if ( m_pPrevBB )
                    Derived::GetDerivedLinkNode ( m_pPrevBB ).m_pNextBB = m_pBB;

                return pOldPrevBB;
            }

            BasicBlock*             LinkNext                ( BasicBlock* pNextBB )
            {
                BasicBlock* pOldNextBB = m_pNextBB;

                if ( m_pNextBB )
                    Derived::GetDerivedLinkNode ( m_pNextBB ).m_pPrevBB = NULL;

                m_pNextBB = pNextBB;

                if ( m_pNextBB )
                    Derived::GetDerivedLinkNode ( m_pNextBB ).m_pPrevBB = m_pBB;

                return pOldNextBB;
            }

            void                    Unlink                  ()
            {
                if ( m_pPrevBB )
                    Derived::GetDerivedLinkNode ( m_pPrevBB ).m_pNextBB = m_pNextBB;
                if ( m_pNextBB )
                    Derived::GetDerivedLinkNode ( m_pNextBB ).m_pPrevBB = m_pPrevBB;

                m_pPrevBB = NULL;
                m_pNextBB = NULL;
            }

            BasicBlock*             m_pBB;
            BasicBlock*             m_pPrevBB;
            BasicBlock*             m_pNextBB;
        };

        struct InputInfo : public LinkNode < InputInfo >
        {
                                    InputInfo               ( BasicBlock* pBB )
                : LinkNode < InputInfo > ( pBB )
            {
                m_iIndex = -1;
				m_iOffset = -1;
            }

            static InputInfo&       GetDerivedLinkNode      ( BasicBlock* pBB )
            {
                return pBB->GetInputInfo ();
            }
            
            int                     m_iIndex;
			int						m_iOffset;
        };
        InputInfo&              GetInputInfo            ()  { return m_InputInfo; }

        struct OutputInfo : public LinkNode < OutputInfo >
        {
                                    OutputInfo              ( BasicBlock* pBB )
                : LinkNode < OutputInfo > ( pBB )
            {
                Reset ();
            }

            void                    Reset                   ()
            {
                m_iIndex = -1;
                m_iPriority = 0;
                m_LoopIDs.clear ();
                m_iBranchLength = -1;
                m_iOffset = -1;
				m_pPrevBB = NULL;
				m_pNextBB = NULL;
            }

            bool                    IsOrdered               () const
            {
                return m_iIndex >= 0;
            }

            static OutputInfo&      GetDerivedLinkNode      ( BasicBlock* pBB )
            {
                return pBB->GetOutputInfo ();
            }

            int                     m_iIndex;
            int                     m_iPriority;
			prevector < int >		m_LoopIDs;
            int                     m_iBranchLength;
            int                     m_iOffset;
        };
        OutputInfo&             GetOutputInfo           ()  { return m_OutputInfo; }

    private:
        void                    SetOffset               ( dword dwOffset )  { m_InputInfo.m_iOffset = dwOffset; }

        void                    FetchCodeFromMethod     ( byte* pMethod );

        void                    AddIncoming             ( BasicBlock* pBB, byte ucBranchType );
        void                    RemoveIncoming          ( BasicBlock* pBB );
        void                    RemoveIncoming          ( BasicBlock* pBB, byte ucBranchType );

        BasicBlockPool*         m_pPool;

        dword                   m_dwOffset;
        dword                   m_dwLength;
        dword                   m_dwFlags;
        
        bool                    m_bFetched;
        InstrBuffer             m_Buffer;

        byte                    m_ucBranchType;
        BasicBlock*             m_pFallthroughBB;

        typedef prevector < IncomingInfo, 4 > Incomings_t;
        Incomings_t             m_Incoming;

        typedef prevector < TargetInfo, 1 > Targets_t;
        Targets_t               m_Targets;
		Targets_t				m_EHs;

        InputInfo               m_InputInfo;
        OutputInfo              m_OutputInfo;

        prebitset < 128 >       m_ReachableFrom;
    };

}
