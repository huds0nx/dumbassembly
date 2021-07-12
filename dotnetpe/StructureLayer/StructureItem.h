#pragma once

namespace dotnetpe
{

    template < class TableT >
    class StructureItem
    {
    public:
        typedef TableT Table;

                                StructureItem           ()
        {
            m_pStructureLayer = NULL;
        }

        virtual                 ~StructureItem          () {}

        virtual void            Init                    ( StructureLayer* pStructureLayer, dword dwIndex )
        {
            m_pStructureLayer = pStructureLayer;

            TableT* pTable = (TableT *)pStructureLayer->GetPE ()->GetTableLayer ()->GetMetaTable (
                (MetaTable::eType)Table::TYPE );
            m_Record = pTable->GetRecord ( dwIndex );
        }

        dword                   Token                   () const
        {
            return ( GetTable ()->GetType () << 24 ) | m_Record.GetIndex ();
        }

        bool                    operator==              ( const StructureItem < TableT >& other ) const
        {
            return m_Record.GetIndex () == other.m_Record.GetIndex ();
        }

        bool                    operator!=              ( const StructureItem < TableT >& other ) const
        {
            return !(*this == other );
        }

    protected:
        DotNetPE*               GetPE                   () const    { return m_pStructureLayer->GetPE (); }
        TableLayer*             GetTableLayer           () const    { return m_pStructureLayer->GetPE ()->GetTableLayer (); }
        TableT*                 GetTable                () const    { return m_Record.GetTable (); }

        StructureLayer*         m_pStructureLayer;
        typename TableT::Record m_Record;
    };

}
