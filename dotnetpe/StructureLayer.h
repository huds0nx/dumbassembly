#pragma once

namespace dotnetpe { class StructureLayer; }

#include "StructureLayer/StructureItem.h"
#include "StructureLayer/StructureItemList.h"

#include "StructureLayer/Type.h"
#include "StructureLayer/Field.h"
#include "StructureLayer/MethodDef.h"
#include "StructureLayer/MethodRef.h"
#include "StructureLayer/TypeDef.h"
#include "StructureLayer/TypeRef.h"
#include "StructureLayer/Module.h"
#include "StructureLayer/Assembly.h"
#include "StructureLayer/Resource.h"

namespace dotnetpe
{

    class StructureLayer
    {
        friend Assembly;
        friend Module;
        friend TypeDef;
        friend TypeRef;
        friend Field;
        friend MethodDef;
        friend MethodRef;

    public:
                                StructureLayer              ( DotNetPE* pPE );
                                ~StructureLayer             ();

        void                    Init                        ();
        DotNetPE*               GetPE                       () const    { return m_pPE; }

        Assembly*               GetAssembly                 ()          { return &m_Assembly; }
        Module*                 GetModule                   ()          { return &m_Module; }

        int                     NumResources                () const;
        Resource*               GetResource                 ( int iResource );
        Resource*               GetResource                 ( const char* pszName );

    private:
        DotNetPE*               m_pPE;

        Assembly                m_Assembly;
        Module                  m_Module;

        StructureItemList < TypeDef >   m_TypeDefs;
        StructureItemList < TypeRef >   m_TypeRefs;
        StructureItemList < Field >     m_Fields;
        StructureItemList < MethodDef > m_MethodDefs;
        StructureItemList < MethodRef > m_MethodRefs;
        StructureItemList < Resource >  m_Resources;
    };

}
