#include "StdInc.h"

namespace dotnetpe
{

    MethodDef::ExtraSection::ExtraSection ( IMAGE_COR_ILMETHOD_SECT_EH* pSection )
        : m_pSection ( pSection )
    {

    }

    MethodDef::ExtraSection::~ExtraSection ()
    {

    }

    bool MethodDef::ExtraSection::HasMoreSections () const
    {
        return ( m_pSection->Small.SectSmall.Kind & CorILMethod_Sect_MoreSects ) != 0;
    }

    dword MethodDef::ExtraSection::Size () const
    {
        if ( IsFat () )
            return m_pSection->Fat.SectFat.DataSize;
        else
            return m_pSection->Small.SectSmall.DataSize;
    }

    dword MethodDef::ExtraSection::NumClauses () const
    {
        if ( IsFat () )
        {
            return ( m_pSection->Fat.SectFat.DataSize - sizeof(m_pSection->Fat.SectFat) ) /
                sizeof(m_pSection->Fat.Clauses[0]);
        }
        else
        {
            return ( m_pSection->Small.SectSmall.DataSize - sizeof(m_pSection->Small.SectSmall) -
                sizeof(m_pSection->Small.Reserved) ) / sizeof(m_pSection->Small.Clauses[0]);
        }
    }

    CorExceptionFlag MethodDef::ExtraSection::GetClauseFlags ( dword dwIndex ) const
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            return m_pSection->Fat.Clauses[dwIndex].Flags;
        else
            return m_pSection->Small.Clauses[dwIndex].Flags;
    }

    dword MethodDef::ExtraSection::GetClauseTryOffset ( dword dwIndex ) const
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            return m_pSection->Fat.Clauses[dwIndex].TryOffset;
        else
            return m_pSection->Small.Clauses[dwIndex].TryOffset;
    }

    dword MethodDef::ExtraSection::GetClauseTryLength ( dword dwIndex ) const
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            return m_pSection->Fat.Clauses[dwIndex].TryLength;
        else
            return m_pSection->Small.Clauses[dwIndex].TryLength;
    }

    dword MethodDef::ExtraSection::GetClauseHandlerOffset ( dword dwIndex ) const
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            return m_pSection->Fat.Clauses[dwIndex].HandlerOffset;
        else
            return m_pSection->Small.Clauses[dwIndex].HandlerOffset;
    }

    dword MethodDef::ExtraSection::GetClauseHandlerLength ( dword dwIndex ) const
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            return m_pSection->Fat.Clauses[dwIndex].HandlerLength;
        else
            return m_pSection->Small.Clauses[dwIndex].HandlerLength;
    }

    dword MethodDef::ExtraSection::GetClauseClassToken ( dword dwIndex ) const
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            return m_pSection->Fat.Clauses[dwIndex].ClassToken;
        else
            return m_pSection->Small.Clauses[dwIndex].ClassToken;
    }

    dword MethodDef::ExtraSection::GetClauseFilterOffset ( dword dwIndex ) const
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            return m_pSection->Fat.Clauses[dwIndex].FilterOffset;
        else
            return m_pSection->Small.Clauses[dwIndex].FilterOffset;
    }

    void MethodDef::ExtraSection::SetClauseFlags ( dword dwIndex, CorExceptionFlag flags )
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            m_pSection->Fat.Clauses[dwIndex].Flags = flags;
        else
            m_pSection->Small.Clauses[dwIndex].Flags = flags;
    }

    void MethodDef::ExtraSection::SetClauseTryOffset ( dword dwIndex, dword dwTryOffset )
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            m_pSection->Fat.Clauses[dwIndex].TryOffset = dwTryOffset;
        else
            m_pSection->Small.Clauses[dwIndex].TryOffset = dwTryOffset;
    }

    void MethodDef::ExtraSection::SetClauseTryLength ( dword dwIndex, dword dwTryLength )
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            m_pSection->Fat.Clauses[dwIndex].TryLength = dwTryLength;
        else
            m_pSection->Small.Clauses[dwIndex].TryLength = dwTryLength;
    }

    void MethodDef::ExtraSection::SetClauseHandlerOffset ( dword dwIndex, dword dwHandlerOffset )
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            m_pSection->Fat.Clauses[dwIndex].HandlerOffset = dwHandlerOffset;
        else
            m_pSection->Small.Clauses[dwIndex].HandlerOffset = dwHandlerOffset;
    }

    void MethodDef::ExtraSection::SetClauseHandlerLength ( dword dwIndex, dword dwHandlerLength )
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            m_pSection->Fat.Clauses[dwIndex].HandlerLength = dwHandlerLength;
        else
            m_pSection->Small.Clauses[dwIndex].HandlerLength = dwHandlerLength;
    }

    void MethodDef::ExtraSection::SetClauseClassToken ( dword dwIndex, dword dwClassToken )
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            m_pSection->Fat.Clauses[dwIndex].ClassToken = dwClassToken;
        else
            m_pSection->Small.Clauses[dwIndex].ClassToken = dwClassToken;
    }

    void MethodDef::ExtraSection::SetClauseFilterOffset ( dword dwIndex, dword dwFilterOffset )
    {
        assert ( dwIndex < NumClauses () );
        if ( IsFat () )
            m_pSection->Fat.Clauses[dwIndex].FilterOffset = dwFilterOffset;
        else
            m_pSection->Small.Clauses[dwIndex].FilterOffset = dwFilterOffset;
    }

    bool MethodDef::ExtraSection::IsFat () const
    {
        return ( m_pSection->Small.SectSmall.Kind & CorILMethod_Sect_FatFormat ) != 0;
    }


    TypeDef* MethodDef::GetType () const
    {
        Module* pModule = m_pStructureLayer->GetModule ();
        TypeDef* pType = NULL;
        TypeDef* pPrevType = NULL;
        for ( int i = 0; i < pModule->NumTypes (); i++ )
        {
            pPrevType = pType;
            pType = pModule->GetType ( i );
            if ( pType->NumMethods () == 0 )
                continue;

            MethodDef* pFirstMethod = pType->GetMethod ( (dword)0 );
            if ( pFirstMethod->m_Record.GetIndex () > m_Record.GetIndex () )
                break;
        }
        return pPrevType;
    }

    byte* MethodDef::Code () const
    {
        if ( !GetHeader () )
            return NULL;

        if ( IsFat () )
            return (byte *)GetHeader () + sizeof(GetHeader ()->Fat);
        else
            return (byte *)GetHeader () + sizeof(GetHeader ()->Tiny);
    }

    dword MethodDef::CodeSize () const
    {
        if ( !GetHeader () )
            return 0;

        if ( IsFat () )
            return GetHeader ()->Fat.CodeSize;
        else
            return GetHeader ()->Tiny.Flags_CodeSize >> 2;
    }

    IMAGE_COR_ILMETHOD* MethodDef::GetHeader () const
    {
        dword dwRVA = m_Record.GetRVA ();
        if ( dwRVA == 0 )
            return NULL;

        return (IMAGE_COR_ILMETHOD *)m_pStructureLayer->GetPE ()->RVAToPtr ( dwRVA );
    }

    dword MethodDef::NumExtraSections () const
    {
        if ( !IsFat () || !(GetHeader ()->Fat.Flags & CorILMethod_MoreSects) )
            return 0;

        IMAGE_COR_ILMETHOD_SECT_EH* pExtraSection = (IMAGE_COR_ILMETHOD_SECT_EH *)(
            (byte *)Code () + CodeSize () );
        if ( (dword)pExtraSection & 3 )
            pExtraSection = (IMAGE_COR_ILMETHOD_SECT_EH *)( ((dword)pExtraSection & ~3) + 4 );

        dword dwNum = 1;
        while ( true )
        {
            ExtraSection section ( pExtraSection );
            if ( !section.HasMoreSections () )
                break;

            pExtraSection = (IMAGE_COR_ILMETHOD_SECT_EH *)( (byte *)pExtraSection + section.Size () );
            dwNum++;
        }

        return dwNum;
    }

	int MethodDef::NumEHs () const
	{
		int iNumEHs = 0;
		for ( int i = 0; i < NumExtraSections (); i++ )
		{
			iNumEHs += GetExtraSection ( i ).NumClauses ();
		}
		return iNumEHs;
	}

    void MethodDef::ForEachEH ( std::tr1::function < void (EHInfo& ehInfo) > f )
    {
		ForEachEH (
			f,
			[] ( const EHInfo& eh1, const EHInfo& eh2 )
			{
				return eh1.m_iTryOffset < eh2.m_iTryOffset || (eh1.m_iTryOffset == eh2.m_iTryOffset && eh1.m_iCatchOffset < eh2.m_iCatchOffset);
			}
		);
	}

	void MethodDef::ForEachEH ( std::tr1::function < void (EHInfo&) > f, std::tr1::function < bool (const EHInfo&, const EHInfo&) > sortComparer )
	{
		byte* pCode = Code ();

		std::vector < EHIndex > clauseOrder;
		for ( int i = 0; i < NumExtraSections (); i++ )
		{
			ExtraSection extraSection = GetExtraSection ( i );
			for ( int j = 0; j < extraSection.NumClauses (); j++ )
			{
				clauseOrder.push_back ( EHIndex ( i, j ) );
			}
		}

		std::sort ( clauseOrder.begin (), clauseOrder.end (),
			[=] ( EHIndex idx1, EHIndex idx2 )
			{
				return sortComparer ( MethodDef::EHInfo ( this, idx1 ), MethodDef::EHInfo ( this, idx2 ) );
			}
		);

		for ( auto it = clauseOrder.begin (); it != clauseOrder.end (); it++ )
		{
			EHInfo eh ( this, *it );
			f ( eh );
			eh.ApplyToMethod ( this, *it );
		}
	}

    MethodDef::ExtraSection MethodDef::GetExtraSection ( dword dwIndex ) const
    {
        assert ( dwIndex < NumExtraSections () );

        IMAGE_COR_ILMETHOD_SECT_EH* pExtraSection = (IMAGE_COR_ILMETHOD_SECT_EH *)(
            (byte *)Code () + CodeSize () );
        if ( (dword)pExtraSection & 3 )
            pExtraSection = (IMAGE_COR_ILMETHOD_SECT_EH *)( ((dword)pExtraSection & ~3) + 4 );

        for ( dword i = 0; i < dwIndex; i++ )
        {
            ExtraSection section ( pExtraSection );
            pExtraSection = (IMAGE_COR_ILMETHOD_SECT_EH *)( (byte *)pExtraSection + section.Size () );
        }
        return ExtraSection ( pExtraSection );
    }

    bool MethodDef::IsFat () const
    {
        return ( GetHeader ()->Tiny.Flags_CodeSize & 3 ) == CorILMethod_FatFormat;
    }

	MethodDef::EHInfo::EHInfo ( dword dwTryOffset, dword dwTryLength, dword dwFilterOffset, dword dwCatchOffset, dword dwCatchLength )
	{
		m_iTryOffset = dwTryOffset;
		m_iTryLength = dwTryLength;
        m_iFilterOffset = dwFilterOffset;
		m_iCatchOffset = dwCatchOffset;
		m_iCatchLength = dwCatchLength;
	}

	MethodDef::EHInfo::EHInfo ( MethodDef* pMethod, EHIndex idx )
	{
		ExtraSection extraSection = pMethod->GetExtraSection ( idx.m_iExtraSectionIndex );
		m_iTryOffset = extraSection.GetClauseTryOffset ( idx.m_iClauseIndex );
        m_iTryLength = extraSection.GetClauseTryLength ( idx.m_iClauseIndex );
        if ( extraSection.GetClauseFlags ( idx.m_iClauseIndex ) & COR_ILEXCEPTION_CLAUSE_FILTER )
            m_iFilterOffset = extraSection.GetClauseFilterOffset ( idx.m_iClauseIndex );
        else
            m_iFilterOffset = -1;
		m_iCatchOffset = extraSection.GetClauseHandlerOffset ( idx.m_iClauseIndex );
		m_iCatchLength = extraSection.GetClauseHandlerLength ( idx.m_iClauseIndex );
	}

	void MethodDef::EHInfo::ApplyToMethod ( MethodDef* pMethod, EHIndex idx ) const
	{
		dword dwCodeSize = pMethod->CodeSize ();

		assert ( m_iTryOffset + m_iTryLength <= dwCodeSize );
		assert ( m_iCatchOffset + m_iCatchLength <= dwCodeSize );

		ExtraSection extraSection = pMethod->GetExtraSection ( idx.m_iExtraSectionIndex );
		extraSection.SetClauseTryOffset ( idx.m_iClauseIndex, m_iTryOffset );
		extraSection.SetClauseTryLength ( idx.m_iClauseIndex, m_iTryLength );
        if ( extraSection.GetClauseFlags ( idx.m_iClauseIndex ) & COR_ILEXCEPTION_CLAUSE_FILTER )
            extraSection.SetClauseFilterOffset ( idx.m_iClauseIndex, m_iFilterOffset );
		extraSection.SetClauseHandlerOffset ( idx.m_iClauseIndex, m_iCatchOffset );
		extraSection.SetClauseHandlerLength ( idx.m_iClauseIndex, m_iCatchLength );
	}

}
