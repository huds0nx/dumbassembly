#include "StdInc.h"

namespace dotnetpe
{

    MetaTable::eType TypeDefOrRefIndexMetaFieldReader::m_Tables[3] = {
        MetaTable::METATABLE_TYPEDEF,
        MetaTable::METATABLE_TYPEREF,
        MetaTable::METATABLE_TYPESPEC
    };

    MetaTable::eType HasConstantIndexMetaFieldReader::m_Tables[3] = {
        MetaTable::METATABLE_FIELD,
        MetaTable::METATABLE_PARAM,
        MetaTable::METATABLE_PROPERTY
    };

    MetaTable::eType HasCustomAttributeIndexMetaFieldReader::m_Tables[19] = {
        MetaTable::METATABLE_METHODDEF,
        MetaTable::METATABLE_FIELD,
        MetaTable::METATABLE_TYPEREF,
        MetaTable::METATABLE_TYPEDEF,
        MetaTable::METATABLE_PARAM,
        MetaTable::METATABLE_INTERFACEIMPL,
        MetaTable::METATABLE_MEMBERREF,
        MetaTable::METATABLE_MODULE,
        MetaTable::METATABLE_DECLSECURITY,
        MetaTable::METATABLE_PROPERTY,
        MetaTable::METATABLE_EVENT,
        MetaTable::METATABLE_STANDALONESIG,
        MetaTable::METATABLE_MODULEREF,
        MetaTable::METATABLE_TYPESPEC,
        MetaTable::METATABLE_ASSEMBLY,
        MetaTable::METATABLE_ASSEMBLYREF,
        MetaTable::METATABLE_FILE,
        MetaTable::METATABLE_EXPORTEDTYPE,
        MetaTable::METATABLE_MANIFESTRESOURCE
    };

    MetaTable::eType HasFieldMarshallIndexMetaFieldReader::m_Tables[2] = {
        MetaTable::METATABLE_FIELD,
        MetaTable::METATABLE_PARAM
    };

    MetaTable::eType HasDeclSecurityIndexMetaFieldReader::m_Tables[3] = {
        MetaTable::METATABLE_TYPEDEF,
        MetaTable::METATABLE_METHODDEF,
        MetaTable::METATABLE_ASSEMBLY
    };

    MetaTable::eType MemberRefParentIndexMetaFieldReader::m_Tables[5] = {
        MetaTable::METATABLE_TYPEDEF,
        MetaTable::METATABLE_TYPEREF,
        MetaTable::METATABLE_MODULEREF,
        MetaTable::METATABLE_METHODDEF,
        MetaTable::METATABLE_TYPESPEC
    };

    MetaTable::eType HasSemanticsIndexMetaFieldReader::m_Tables[2] = {
        MetaTable::METATABLE_EVENT,
        MetaTable::METATABLE_PROPERTY
    };

    MetaTable::eType MethodDefOrRefIndexMetaFieldReader::m_Tables[2] = {
        MetaTable::METATABLE_METHODDEF,
        MetaTable::METATABLE_MEMBERREF
    };

    MetaTable::eType MemberForwardedIndexMetaFieldReader::m_Tables[2] = {
        MetaTable::METATABLE_FIELD,
        MetaTable::METATABLE_METHODDEF
    };

    MetaTable::eType ImplementationIndexMetaFieldReader::m_Tables[3] = {
        MetaTable::METATABLE_FILE,
        MetaTable::METATABLE_ASSEMBLYREF,
        MetaTable::METATABLE_EXPORTEDTYPE
    };

    MetaTable::eType CustomAttributeTypeIndexMetaFieldReader::m_Tables[4] = {
        MetaTable::METATABLE_NONE,
        MetaTable::METATABLE_NONE,
        MetaTable::METATABLE_METHODDEF,
        MetaTable::METATABLE_MEMBERREF
    };

    MetaTable::eType ResolutionScopeIndexMetaFieldReader::m_Tables[4] = {
        MetaTable::METATABLE_MODULE,
        MetaTable::METATABLE_MODULEREF,
        MetaTable::METATABLE_ASSEMBLYREF,
        MetaTable::METATABLE_TYPEREF
    };

    MetaTable::eType TypeOrMethodDefIndexMetaFieldReader::m_Tables[2] = {
        MetaTable::METATABLE_TYPEDEF,
        MetaTable::METATABLE_METHODDEF
    };

}
