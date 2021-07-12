#pragma once

template < int SIZE = 32 >
class prebitset
{
    typedef prevector < dword, (SIZE / 32) + ((SIZE & 31) ? 1 : 0) > Prevector_t;

public:
                                prebitset               ()
    {
        m_iSize = 0;
    }

                                prebitset               ( const prebitset& other )
    {
        *this = other;
    }

                                prebitset               ( prebitset&& other )
    {
        *this = std::move ( other );
    }

                                ~prebitset              ()
    {
        clear ();
    }

    prebitset&                  operator=               ( const prebitset& other )
    {
        if ( this != &other )
        {
            m_Prevector = other.m_Prevector;
            m_iSize = other.m_iSize;
        }
        return *this;
    }

    prebitset&                  operator=               ( prebitset&& other )
    {
        if ( this != &other )
        {
            m_Prevector = std::move ( other.m_Prevector );
            m_iSize = other.m_iSize;
        }
        return *this;
    }

    bool                        operator==              ( const prebitset& other ) const
    {
        return size () == other.size () && m_Prevector == other.m_Prevector;
    }

    bool                        operator!=              ( const prebitset& other ) const
    {
        return !( *this == other );
    }

    prebitset&                  operator&=              ( const prebitset& other )
    {
        if ( this != &other )
        {
            assert ( size () == other.size () );
            for ( int i = 0; i < m_Prevector.size (); i++ )
            {
                m_Prevector[i] &= other.m_Prevector[i];
            }
        }
        return *this;
    }

    prebitset&                  operator|=              ( const prebitset& other )
    {
        if ( this != &other )
        {
            assert ( size () == other.size () );
            for ( int i = 0; i < m_Prevector.size (); i++ )
            {
                m_Prevector[i] |= other.m_Prevector[i];
            }
        }
        return *this;
    }

    prebitset&                  operator^=              ( const prebitset& other )
    {
        if ( this != &other )
        {
            assert ( size () == other.size () );
            for ( int i = 0; i < m_Prevector.size (); i++ )
            {
                m_Prevector[i] ^= other.m_Prevector[i];
            }
        }
        return *this;
    }

    prebitset                   operator&               ( const prebitset& other ) const
    {
        prebitset result ( *this );
        result &= other;
        return result;
    }

    prebitset                   operator|               ( const prebitset& other ) const
    {
        prebitset result ( *this );
        result |= other;
        return result;
    }

    prebitset                   operator^               ( const prebitset& other ) const
    {
        prebitset result ( *this );
        result &= other;
        return result;
    }

    class const_bitproxy
    {
        friend prebitset;

    public:
                                operator bool           () const
        {
            return ( m_Bitset.m_Prevector[m_iIndex / 32] & (1 << (m_iIndex & 31)) ) != 0;
        }

    protected:
                                const_bitproxy          ( const prebitset& bitset, int iIndex )
            : m_Bitset ( const_cast<prebitset&>(bitset) ), m_iIndex ( iIndex )
        {
            
        }

        prebitset&              m_Bitset;
        int                     m_iIndex;
    };

    class bitproxy : public const_bitproxy
    {
        friend prebitset;

    public:
        bitproxy&               operator=               ( bool b )
        {
            if ( b )
            {
                m_Bitset.m_Prevector[m_iIndex / 32] |= 1 << (m_iIndex & 31);
            }
            else
            {
                m_Bitset.m_Prevector[m_iIndex / 32] &= ~(1 << (m_iIndex & 31));
            }
            return *this;
        }

    private:
                                bitproxy                ( prebitset& bitset, int iIndex )
            : const_bitproxy ( bitset, iIndex )
        {
            
        }
    };
    friend bitproxy;

    bitproxy                    operator[]              ( int iIndex )
    {
        return bitproxy ( *this, iIndex );
    }

    const_bitproxy              operator[]              ( int iIndex ) const
    {
        return const_bitproxy ( *this, iIndex );
    }

    size_t                      size                    () const
    {
        return m_iSize;
    }

    bool                        empty                   () const
    {
        return size () == 0;
    }

    size_t                      capacity                () const
    {
        return 8 * m_Prevector.capacity ();
    }

    void                        resize                  ( size_t iSize )
    {
        m_Prevector.resize ( (iSize / 32) + ((iSize & 31) ? 1 : 0) );
        m_iSize = iSize;
    }

    void                        clear                   ()
    {
        m_Prevector.clear ();
        m_iSize = 0;
    }

private:
    Prevector_t                 m_Prevector;
    size_t                      m_iSize;
};
