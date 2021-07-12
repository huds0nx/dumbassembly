#pragma once

namespace dotnetpe
{

    class BasicBlockPool : public Pool < BasicBlock, 0x1000 >
    {
    public:
                                BasicBlockPool              ();
                                BasicBlockPool              ( MethodDef* pMethod );
                                BasicBlockPool              ( byte* pCode, dword dwSize );
                                ~BasicBlockPool             ();

        void                    FromMethod                  ( MethodDef* pMethod );
        void                    FromCode                    ( byte* pCode, dword dwSize );

        BasicBlock*             GetFirstBB                  () const { return m_pFirstBB; }
        BasicBlock*             GetLastBB                   () const { return m_pLastBB; }
        BasicBlock*             GetBBContaining             ( dword dwOffset ) const;
        BasicBlock*             GetFirstBBContainedIn       ( dword dwOffset, dword dwLength ) const;
        BasicBlock*             GetBBContainingOrBefore     ( dword dwOffset ) const;
        BasicBlock*             GetBBContainingOrAfter      ( dword dwOffset ) const;

        BasicBlock*             ReorderBlocks               ( MethodDef* pMethod );
        BasicBlock*             ReorderBlocks               ();

        void                    RemoveDeadBlocks            ( MethodDef* pMethod );
        bool                    MergeBlocks                 ( MethodDef* pMethod );

        void                    ToMethod                    ( MethodDef* pMethod );
        dword                   ToCode                      ( byte* pCode, dword dwSize );

        void                    Delete                      ( BasicBlock* pBB );
        void                    Clear                       ();

        template < class Functor >
        void                    DFS                         ( Functor&& f, BasicBlock* pStartBB )
        {
            ClearDFSState ();
            DFSNoClear ( f, pStartBB );
        }

    private:
		BasicBlock				m_EntryBB;
        BasicBlock*             m_pFirstBB;
        BasicBlock*             m_pLastBB;

        enum eBBFlag
        {
            HANDLED_DFS               = 1 << 0,

            IN_CATCH                  = 1 << 1,
            IN_LOOP                   = 1 << 2,
            IS_HEADER                 = 1 << 3,

            HANDLED_REACHABILITY      = 1 << 4,
			HANDLED_EH				  = 1 << 5
        };

        void                    FromCode                    ( byte* pCode, dword dwSize, bool bClear );

        template < class Functor >
        void                    DFSNoClear                  ( Functor&& f, BasicBlock* pStartBB )
        {
            if ( pStartBB->HasFlag ( HANDLED_DFS ) )
                return;

            prevector < BasicBlock*, 0x40 > bbQueue;
            bbQueue.push_back ( pStartBB );
            while ( !bbQueue.empty () )
            {
                BasicBlock* pBB = bbQueue.back ();
                bbQueue.pop_back ();

				if ( pBB->HasFlag ( HANDLED_DFS ) )
					continue;

                pBB->SetFlag ( HANDLED_DFS );
                if ( !f ( pBB ) )
                    continue;

                for ( int i = pBB->NumOutgoing () - 1; i >= 0; i-- )
                {
                    BasicBlock* pOutBB = pBB->GetOutgoing ( i );
                    if ( !pOutBB->HasFlag ( HANDLED_DFS ) )
                        bbQueue.push_back ( pOutBB );
                }
            }
        }

        void                    ClearDFSState               ()
        {
            for ( iterator it = Begin (); it != End (); it++ )
                it->ClearFlag ( HANDLED_DFS );
        }

        BasicBlock*             SplitBB                     ( dword dwOffset, int iSkip, bool bLink );
        BasicBlock*             SplitBB                     ( BasicBlock* pBB, dword dwOffset, int iSkip, bool bLink );
        void                    MergeBB                     ( BasicBlock* pBB );

        void                    AssignTryCatchBBPriorities  ( MethodDef* pMethod );
        bool                    AssignLoopBBPriorities      ( BasicBlock* pHeaderBB );
		void					SetCatchOutgoings			( MethodDef* pMethod );
		void					ClearCatchOutgoings			();
        void                    BuildFullReachabilities     ( BasicBlock* pStartBB );
		void					BuildFullReachabilities     ( MethodDef* pMethod );
        void                    BuildPartialReachabilities  ( BasicBlock* pStartBB, bool bConnectLoopEndStart = true );
        void                    BuildPartialReachabilities  ( MethodDef* pMethod );
		void					ConnectPartialReachabilityLoopEndStart	();
        void                    ResetReachabilities         ();
        void                    RemoveDeadBlocksInternal    ();

        BasicBlock*             OrderBlocks                 ( MethodDef* pMethod, BasicBlock* pStartBB );

        void                    CalcBranchLengths           ();
        int                     WriteBlocks                 ( byte* pCode, int iSize );
        void                    FixEH                       ( MethodDef* pMethod );

		bool					IsReachable					( BasicBlock* pFromBB, BasicBlock* pToBB, std::tr1::function < bool (BasicBlock*) > predicate );
		bool					IsReachableWithFlag			( BasicBlock* pFromBB, BasicBlock* pToBB, dword dwFlag );
		bool                    IsReachableWithoutFlag      ( BasicBlock* pFromBB, BasicBlock* pToBB, dword dwFlag );
        dword                   GetOutputLength             ( dword dwInputOffset, dword dwInputLength );

        class BBTargetOutAddrProvider
        {
        public:
                                    BBTargetOutAddrProvider     ();
            void                    Setup                       ( BasicBlock* pBB, byte* pOutputCode );

            byte*                   GetTargetAddr               ( dword i );

        private:
            BasicBlock*             m_pBB;
            byte*                   m_pOutputCode;
        };
        friend BBTargetOutAddrProvider;
        BBTargetOutAddrProvider m_BBTargetOutAddrProvider;

        typedef std::map < dword, BasicBlock* > InputOffsetToBB_t;
        InputOffsetToBB_t       m_InputOffsetToBB;

        int                     m_iNextLoopID;
    };

}
