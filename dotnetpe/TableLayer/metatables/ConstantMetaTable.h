#pragma once

namespace dotnetpe
{

    class ConstantMetaTable;

    class _ConstantMetaRecord : public MetaRecordBase < ConstantMetaTable >
    {
    public:
                                _ConstantMetaRecord         () {}
                                _ConstantMetaRecord         ( ConstantMetaTable* pTable, void* pRecord );

        byte                    GetType                     () const;
        dword                   GetParent                   ( MetaTable*& pTable ) const;
        const void*             GetValue                    () const;
    };

    class ConstantMetaTable : public MetaTableBase < ConstantMetaTable, _ConstantMetaRecord >
    {
        friend _ConstantMetaRecord;

    public:
                                ConstantMetaTable           ();
        virtual                 ~ConstantMetaTable          ();

        enum { TYPE = METATABLE_CONSTANT };
        virtual eType           GetType                     () const    { return (eType)TYPE; }

        virtual void            Init                        ( TableLayer* pTableLayer );

    private:
        ByteMetaFieldReader             m_Type;
        ByteMetaFieldReader             m_Pad0;
        HasConstantIndexMetaFieldReader m_Parent;
        BlobIndexMetaFieldReader        m_Value;
    };

}
