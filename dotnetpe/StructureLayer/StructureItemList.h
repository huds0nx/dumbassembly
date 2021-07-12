#pragma once

namespace dotnetpe
{

    template < class T >
    class StructureItemList
    {
    public:
                                StructureItemList               () {}
                                ~StructureItemList              () {}

        void                    Init                            ( StructureLayer* pStructureLayer )
        {
            m_pStructureLayer = pStructureLayer;

            MetaTable* pTable = pStructureLayer->GetPE ()->GetTableLayer ()->GetMetaTable (
                (MetaTable::eType)T::Table::TYPE );
            if ( !pTable )
                return;

            m_Items.resize ( pTable->GetNumRecords () );
        }

        T&                      operator[]                      ( dword dwIndex )
        {
            if ( !m_Items.active ( dwIndex ) )
            {
                T& item = m_Items[dwIndex];
                item.Init ( m_pStructureLayer, 1 + dwIndex );
                return item;
            }
            else
            {
                return m_Items[dwIndex];
            }
        }

        dword                   Size                            () const
        {
            return m_Items.size ();
        }

    private:
        StructureLayer*         m_pStructureLayer;
        lazyvector < T >        m_Items;
    };

}
