#include "StdInc.h"

namespace dotnetpe
{

    TypeRef::TypeRef ()
    {

    }

    TypeRef::~TypeRef ()
    {

    }

    dword TypeRef::NumMethods ()
    {
        FetchMethods ();
        return m_Methods.size ();
    }

    MethodRef* TypeRef::GetMethod ( dword dwIndex )
    {
        FetchMethods ();
        assert ( dwIndex < NumMethods () );
        return m_Methods [ dwIndex ];
    }

    MethodRef* TypeRef::GetMethod ( const char* pszName )
    {
        FetchMethods ();
        for ( Methods_t::iterator it = m_Methods.begin (); it != m_Methods.end (); it++ )
        {
            if ( !strcmp ( (*it)->GetName (), pszName ) )
                return *it;
        }
        return NULL;
    }

    void TypeRef::FetchMethods ()
    {
        if ( !m_Methods.empty () )
            return;

        for ( dword i = 0; i < m_pStructureLayer->m_MethodRefs.Size (); i++ )
        {
            MethodRef* pMethod = &m_pStructureLayer->m_MethodRefs [ i ];
            if ( pMethod->GetType () == this )
                m_Methods.push_back ( pMethod );
        }
    }

}
