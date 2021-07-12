#include "StdInc.h"

namespace dotnetpe
{

    TypeDef::TypeDef ()
    {
        m_iSize = -1;
        m_iPack = -1;
    }

    TypeDef::~TypeDef ()
    {

    }

    void TypeDef::Init ( StructureLayer* pStructureLayer, dword dwIndex )
    {
        StructureItem < TypeDefMetaTable >::Init ( pStructureLayer, dwIndex );

        m_dwFieldsStartIndex  = m_Record.GetFieldList () - 1;
        m_dwMethodsStartIndex = m_Record.GetMethodList () - 1;

        if ( m_Record.GetIndex () == GetTable ()->GetNumRecords () )
        {
            m_dwFieldsEndIndex  = GetTableLayer ()->GetMetaTable ( MetaTable::METATABLE_FIELD )->GetNumRecords ();
            m_dwMethodsEndIndex = GetTableLayer ()->GetMetaTable ( MetaTable::METATABLE_METHODDEF )->GetNumRecords ();
        }
        else
        {
            m_dwFieldsEndIndex  = GetTable ()->GetRecord ( m_Record.GetIndex () + 1 ).GetFieldList () - 1;
            m_dwMethodsEndIndex = GetTable ()->GetRecord ( m_Record.GetIndex () + 1 ).GetMethodList () - 1;
        }
    }

    int TypeDef::GetSize () const
    {
        FetchClassLayout ();
        return m_iSize;
    }

    int TypeDef::GetPack () const
    {
        FetchClassLayout ();
        return m_iPack;
    }

    Field* TypeDef::GetField ( dword dwIndex ) const
    {
        assert ( dwIndex < NumFields () );
        return &m_pStructureLayer->m_Fields [ m_dwFieldsStartIndex + dwIndex ];
    }

    Field* TypeDef::GetField ( const char* pszName ) const
    {
        for ( dword i = 0; i < NumFields (); i++ )
        {
            Field* pField = GetField ( i );
            if ( strcmp ( pField->GetName (), pszName ) == 0 )
                return pField;
        }
        return NULL;
    }

    MethodDef* TypeDef::GetMethod ( dword dwIndex ) const
    {
        assert ( dwIndex < NumMethods () );
        return &m_pStructureLayer->m_MethodDefs [ m_dwMethodsStartIndex + dwIndex ];
    }

    MethodDef* TypeDef::GetMethod ( const char* pszName ) const
    {
        for ( dword i = 0; i < NumMethods (); i++ )
        {
            MethodDef* pMethod = GetMethod ( i );
            if ( strcmp ( pMethod->GetName (), pszName ) == 0 )
                return pMethod;
        }
        return NULL;
    }

    Type* TypeDef::GetBaseType () const
    {
        MetaTable* pTable = NULL;
        dword dwExtends = m_Record.GetExtends ( pTable );
        if ( dwExtends == 0 )
            return NULL;

        if ( pTable->GetType () == MetaTable::METATABLE_TYPEDEF && dwExtends <= m_pStructureLayer->m_TypeDefs.Size () )
        {
            return &m_pStructureLayer->m_TypeDefs [ dwExtends - 1 ];
        }
        else if ( pTable->GetType () == MetaTable::METATABLE_TYPEREF && dwExtends <= m_pStructureLayer->m_TypeRefs.Size () )
        {
            return &m_pStructureLayer->m_TypeRefs [ dwExtends - 1 ];
        }
        else
        {
            return NULL;
        }
    }

    void TypeDef::FetchClassLayout () const
    {
        if ( m_iSize >= 0 && m_iPack >= 0 )
            return;

        if ( ( m_Record.GetFlags () & tdLayoutMask ) == tdAutoLayout )
        {
            m_iSize = 0;
            m_iPack = 0;
            return;
        }

        ClassLayoutMetaTable* pClassLayoutTable = (ClassLayoutMetaTable *)GetTableLayer ()->GetMetaTable ( MetaTable::METATABLE_CLASSLAYOUT );
        for ( int i = 1; i <= pClassLayoutTable->GetNumRecords (); i++ )
        {
            ClassLayoutMetaTable::Record layoutRec = pClassLayoutTable->GetRecord ( i );
            if ( layoutRec.GetParent () == m_Record.GetIndex () )
            {
                m_iSize = layoutRec.GetClassSize ();
                m_iPack = layoutRec.GetPackingSize ();
                return;
            }
        }
        m_iSize = 0;
        m_iPack = 0;
    }

}
