#pragma once

namespace dotnetpe
{

    template < MetaTable::eType tableType >
    class TableIndexMetaFieldReader : public MetaFieldReader
    {
    public:
                                TableIndexMetaFieldReader           () : m_bFat ( false ) {}

        int                     Init                                ( TableLayer* pTableLayer, int iFieldOffset )
        {
            MetaFieldReader::Init ( pTableLayer, iFieldOffset );
            m_bFat = ( pTableLayer->GetMetaTable ( tableType )->GetNumRecords () > 0xFFFF );
            return m_bFat ? 4 : 2;
        }

        dword                   Read                                ( void* pRecord )
        {
            if ( m_bFat )
                return *GetFieldPtr < dword > ( pRecord );
            else
                return *GetFieldPtr < word > ( pRecord );
        }
    
    protected:
        bool                    m_bFat;
    };

    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_MODULE > ModuleIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_TYPEREF > TypeRefIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_TYPEDEF > TypeDefIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_FIELD > FieldIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_METHODDEF > MethodDefIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_PARAM > ParamIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_INTERFACEIMPL > InterfaceImplIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_MEMBERREF > MemberRefIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_CONSTANT > ConstantIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_CUSTOMATTRIBUTE > CustomAttributeIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_FIELDMARSHAL > FieldMarshalIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_DECLSECURITY > DeclSecurityIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_CLASSLAYOUT > ClassLayoutIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_FIELDLAYOUT > FieldLayoutIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_STANDALONESIG > StandAloneSigIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_EVENTMAP > EventMapIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_EVENT > EventIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_PROPERTYMAP > PropertyMapIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_PROPERTY > PropertyIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_METHODSEMANTICS > MethodSemanticsIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_METHODIMPL > MethodImplIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_MODULEREF > ModuleRefIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_TYPESPEC > TypeSpecIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_IMPLMAP > ImplMapIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_FIELDRVA > FieldRVAIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_ASSEMBLY > AssemblyIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_ASSEMBLYPROCESSOR > AssemblyProcessorIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_ASSEMBLYOS > AssemblyOSIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_ASSEMBLYREF > AssemblyRefIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_ASSEMBLYREFPROCESSOR > AssemblyRefProcessorIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_ASSEMBLYREFOS > AssemblyRefOSIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_FILE > FileIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_EXPORTEDTYPE > ExportedTypeIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_MANIFESTRESOURCE > ManifestResourceIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_NESTEDCLASS > NestedClassIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_GENERICPARAM > GenericParamIndexMetaFieldReader;
    typedef TableIndexMetaFieldReader < MetaTable::METATABLE_GENERICPARAMCONSTRAINT > GenericParamConstraintIndexMetaFieldReader;

    // ------------------------------------

    template < int iTagBits, class Derived >
    class TableTaggedIndexMetaFieldReader : public MetaFieldReader
    {
    public:
                                TableTaggedIndexMetaFieldReader     () : m_bFat ( false ) {}

        int                     Init                                ( TableLayer* pTableLayer, int iFieldOffset )
        {
            MetaFieldReader::Init ( pTableLayer, iFieldOffset );

            for ( int i = 0; i < NUMELMS ( ((Derived *)this)->m_Tables ); i++ )
            {
                MetaTable* pTable = pTableLayer->GetMetaTable ( ((Derived *)this)->m_Tables[i] );
                if ( !pTable )
                    continue;

                int iNumRecords = pTable->GetNumRecords ();
                if ( iNumRecords >= (1 << (16 - iTagBits)) )
                {
                    m_bFat = true;
                    break;
                }
            }
            return m_bFat ? 4 : 2;
        }

        dword                   Read                                ( void* pRecord, MetaTable*& pTable )
        {
            if ( m_bFat )
            {
                int iTag = GetFieldPtr < FatIndex > ( pRecord )->m_dwTag;
                pTable = m_pTableLayer->GetMetaTable ( ((Derived *)this)->m_Tables[iTag] );
                return GetFieldPtr < FatIndex > ( pRecord )->m_dwIndex;
            }
            else
            {
                int iTag = GetFieldPtr < TinyIndex > ( pRecord )->m_wTag;
                pTable = m_pTableLayer->GetMetaTable ( ((Derived *)this)->m_Tables[iTag] );
                return GetFieldPtr < TinyIndex > ( pRecord )->m_wIndex;
            }
        }

    protected:
        typedef struct
        {
            word    m_wTag : iTagBits;
            word    m_wIndex : 16 - iTagBits;
        } TinyIndex;

        typedef struct
        {
            dword   m_dwTag : iTagBits;
            dword   m_dwIndex : 32 - iTagBits;
        } FatIndex;

        bool                    m_bFat;
    };

    // ------------------------------------

    class TypeDefOrRefIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 2, TypeDefOrRefIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 2, TypeDefOrRefIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[3];
    };

    // ------------------------------------

    class HasConstantIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 2, HasConstantIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 2, HasConstantIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[3];
    };

    // ------------------------------------

    class HasCustomAttributeIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 5, HasCustomAttributeIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 5, HasCustomAttributeIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[19];
    };

    // ------------------------------------

    class HasFieldMarshallIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 1, HasFieldMarshallIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 1, HasFieldMarshallIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[2];
    };

    // ------------------------------------
    
    class HasDeclSecurityIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 2, HasDeclSecurityIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 2, HasDeclSecurityIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[3];
    };

    // ------------------------------------
    
    class MemberRefParentIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 3, MemberRefParentIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 3, MemberRefParentIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[5];
    };

    // ------------------------------------
    
    class HasSemanticsIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 1, HasSemanticsIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 1, HasSemanticsIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[2];
    };

    // ------------------------------------

    class MethodDefOrRefIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 1, MethodDefOrRefIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 1, MethodDefOrRefIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[2];
    };

    // ------------------------------------

    class MemberForwardedIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 1, MemberForwardedIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 1, MemberForwardedIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[2];
    };

    // ------------------------------------
    
    class ImplementationIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 2, ImplementationIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 2, ImplementationIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[3];
    };

    // ------------------------------------
    
    class CustomAttributeTypeIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 3, CustomAttributeTypeIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 3, CustomAttributeTypeIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[4];
    };

    // ------------------------------------

    class ResolutionScopeIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 2, ResolutionScopeIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 2, ResolutionScopeIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[4];
    };

    // ------------------------------------

    class TypeOrMethodDefIndexMetaFieldReader : public TableTaggedIndexMetaFieldReader < 1, TypeOrMethodDefIndexMetaFieldReader >
    {
        friend TableTaggedIndexMetaFieldReader < 1, TypeOrMethodDefIndexMetaFieldReader >;

    private:
        static MetaTable::eType m_Tables[2];
    };

}
