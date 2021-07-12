#include "StdInc.h"

namespace dotnetpe
{

    dword Module::NumTypes () const
    {
        return m_pStructureLayer->m_TypeDefs.Size ();
    }

    TypeDef* Module::GetType ( dword dwIndex ) const
    {
        assert ( dwIndex < NumTypes () );
        return &m_pStructureLayer->m_TypeDefs [ dwIndex ];
    }

    TypeDef* Module::GetType ( const char* pszFullName ) const
    {
        const char* pszNamespace;
        const char* pszName;
        const char* pszLastDot = strrchr ( pszFullName, '.' );
        if ( pszLastDot )
        {
            pszNamespace = pszFullName;
            pszName = pszLastDot + 1;
        }
        else
        {
            pszNamespace = pszFullName;
            pszName = pszFullName;
            pszLastDot = pszFullName;
        }

        for ( dword i = 0; i < NumTypes (); i++ )
        {
            TypeDef* pType = GetType ( i );
            const char* pszTypeNamespace = pType->GetNamespace ();
            const char* pszTypeName = pType->GetName ();
            if ( pszTypeNamespace && pszTypeName &&
                 strlen ( pszTypeNamespace ) == pszLastDot - pszNamespace &&
                 memcmp ( pszTypeNamespace, pszNamespace, pszLastDot - pszNamespace ) == 0 &&
                 strcmp ( pszTypeName, pszName ) == 0 )
            {
                return pType;
            }
        }
        return NULL;
    }

    dword Module::NumReferencedTypes () const
    {
        return m_pStructureLayer->m_TypeRefs.Size ();
    }

    TypeRef* Module::GetReferencedType ( dword dwIndex ) const
    {
        return &m_pStructureLayer->m_TypeRefs [ dwIndex ];
    }

    TypeRef* Module::GetReferencedType ( const char* pszFullName ) const
    {
        const char* pszNamespace;
        const char* pszName;
        const char* pszLastDot = strrchr ( pszFullName, '.' );
        if ( pszLastDot )
        {
            pszNamespace = pszFullName;
            pszName = pszLastDot + 1;
        }
        else
        {
            pszNamespace = pszFullName;
            pszName = pszFullName;
            pszLastDot = pszFullName;
        }

        for ( dword i = 0; i < NumReferencedTypes (); i++ )
        {
            TypeRef* pType = GetReferencedType ( i );
            const char* pszTypeNamespace = pType->GetNamespace ();
            const char* pszTypeName = pType->GetName ();
            if ( pszTypeNamespace && pszTypeName &&
                 strlen ( pszTypeNamespace ) == pszLastDot - pszNamespace &&
                 memcmp ( pszTypeNamespace, pszNamespace, pszLastDot - pszNamespace ) == 0 &&
                 strcmp ( pszTypeName, pszName ) == 0 )
            {
                return pType;
            }
        }
        return NULL;
    }

    dword Module::NumMethods () const
    {
        return m_pStructureLayer->m_MethodDefs.Size ();
    }

    Module::FindInfo::FindInfo ()
    {
        m_pModule = NULL;
        m_dwTypeIndex = 0;
        m_dwMethodIndex = -1;
        m_pInstr = NULL;
        m_pMethodEnd = NULL;
    }

    TypeDef* Module::FindInfo::Type () const
    {
        assert ( m_pModule );
        return m_pModule->GetType ( m_dwTypeIndex );
    }

    MethodDef* Module::FindInfo::Method () const
    {
        return Type () ? Type ()->GetMethod ( m_dwMethodIndex ) : NULL;
    }

    byte* Module::FindInfo::Instruction () const
    {
        return m_pInstr;
    }

    bool Module::FindNextInstr ( FindInfo& info, std::tr1::function < bool (FindInfo&) > handler )
    {
        if ( !info.m_pModule )
            info.m_pModule = this;

        assert ( info.m_pModule == this );

        if ( info.m_dwTypeIndex >= NumTypes () )
            return false;

        while ( true )
        {
            while ( info.m_pInstr >= info.m_pMethodEnd )
            {
                info.m_dwMethodIndex++;
                while ( info.m_dwMethodIndex >= info.Type ()->NumMethods () )
                {
                    info.m_dwMethodIndex = 0;
                    info.m_dwTypeIndex++;
                    if ( info.m_dwTypeIndex >= NumTypes () )
                        return false;
                }
                info.m_pInstr = info.Method ()->Code ();
                info.m_pMethodEnd = info.m_pInstr + info.Method ()->CodeSize ();
            }

            bool bResult = handler ( info );
            info.m_pInstr += ILInstr::GetLength ( info.m_pInstr );
            if ( bResult )
                return true;
        }
        return false;
    }

}
