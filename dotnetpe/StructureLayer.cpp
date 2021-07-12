#include "StdInc.h"

namespace dotnetpe
{

    StructureLayer::StructureLayer ( DotNetPE* pPE )
        : m_pPE ( pPE )
    {

    }

    StructureLayer::~StructureLayer ()
    {

    }

    void StructureLayer::Init ()
    {
        m_Module.Init ( this, 1 );
        m_Assembly.Init ( this, 1 );

        m_TypeDefs.Init ( this );
        m_TypeRefs.Init ( this );
        m_Fields.Init ( this );
        m_MethodDefs.Init ( this );
        m_MethodRefs.Init ( this );
        m_Resources.Init ( this );
    }

    int StructureLayer::NumResources () const
    {
        return m_Resources.Size ();
    }

    Resource* StructureLayer::GetResource ( int iResource )
    {
        assert ( iResource >= 0 && iResource < m_Resources.Size () );
        return &m_Resources [ iResource ];
    }

    Resource* StructureLayer::GetResource ( const char* pszName )
    {
        for ( int i = 0; i < NumResources (); i++ )
        {
            Resource* pResource = GetResource ( i );
            if ( strcmp ( pResource->GetName (), pszName ) == 0 )
                return pResource;
        }
        return NULL;
    }

}
