#pragma once

namespace dotnetpe
{

    class MethodDef : public StructureItem < MethodDefMetaTable >
    {
    public:
        class ExtraSection
        {
        public:
                                ExtraSection            ( IMAGE_COR_ILMETHOD_SECT_EH* pSection );
                                ~ExtraSection           ();

            bool                HasMoreSections         () const;
            dword               Size                    () const;

            dword               NumClauses              () const;

            CorExceptionFlag    GetClauseFlags          ( dword dwIndex ) const;
            dword               GetClauseTryOffset      ( dword dwIndex ) const;
            dword               GetClauseTryLength      ( dword dwIndex ) const;
            dword               GetClauseHandlerOffset  ( dword dwIndex ) const;
            dword               GetClauseHandlerLength  ( dword dwIndex ) const;
            dword               GetClauseClassToken     ( dword dwIndex ) const;
            dword               GetClauseFilterOffset   ( dword dwIndex ) const;

            void                SetClauseFlags          ( dword dwIndex, CorExceptionFlag flags );
            void                SetClauseTryOffset      ( dword dwIndex, dword dwTryOffset );
            void                SetClauseTryLength      ( dword dwIndex, dword dwTryLength );
            void                SetClauseHandlerOffset  ( dword dwIndex, dword dwHandlerOffset );
            void                SetClauseHandlerLength  ( dword dwIndex, dword dwHandlerLength );
            void                SetClauseClassToken     ( dword dwIndex, dword dwClassToken );
            void                SetClauseFilterOffset   ( dword dwIndex, dword dwFilterOffset );

        private:
            bool                IsFat                   () const;

            IMAGE_COR_ILMETHOD_SECT_EH* m_pSection;
        };

		struct EHIndex
		{
		public:
								EHIndex					( int iExtraSectionIndex, int iClauseIndex )
			{
				m_iExtraSectionIndex = iExtraSectionIndex;
				m_iClauseIndex = iClauseIndex;
			}

			int					m_iExtraSectionIndex;
			int					m_iClauseIndex;
		};

		struct EHInfo
		{
		public:
								EHInfo                  ( dword dwTryOffset, dword dwTryLength, dword dwFilterOffset, dword dwCatchOffset, dword dwCatchLength );
								EHInfo                  ( MethodDef* pMethod, EHIndex idx );

			void                ApplyToMethod           ( MethodDef* pMethod, EHIndex idx ) const;

			int                 m_iTryOffset;
			int                 m_iTryLength;
            int                 m_iFilterOffset;
			int                 m_iCatchOffset;
			int                 m_iCatchLength;
		};

                                MethodDef           () {}
        virtual                 ~MethodDef          () {}

        class TypeDef*          GetType             () const;

        const char*             GetName             () const    { return m_Record.GetName (); }

        byte*                   Code                () const;
        dword                   CodeSize            () const;

        dword                   NumExtraSections    () const;
        ExtraSection            GetExtraSection     ( dword dwIndex ) const;

		int						NumEHs				() const;
        void                    ForEachEH           ( std::tr1::function < void (EHInfo&) > f );
		void					ForEachEH			( std::tr1::function < void (EHInfo&) > f, std::tr1::function < bool (const EHInfo&, const EHInfo&) > sortComparer );

    private:
        IMAGE_COR_ILMETHOD*     GetHeader           () const;
        bool                    IsFat               () const;
    };

}
