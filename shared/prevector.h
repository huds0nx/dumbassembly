#pragma once

template < class T, size_t SIZE = 10 >
class prevector
{
public:
                        prevector               ()
    {
        m_iSize = 0;
    }

                        prevector               ( size_t initSize )
    {
        m_iSize = 0;
        resize ( initSize );
    }

                        prevector               ( const prevector& other )
    {
        m_iSize = 0;
        *this = other;
    }

                        prevector               ( prevector&& other )
    {
        m_iSize = 0;
        *this = std::move ( other );
    }

                        ~prevector              ()
    {
        clear ();
    }

    prevector&          operator=               ( const prevector& other )
    {
        if ( this != &other )
        {
            assign ( other.data (), other.size () );
        }
        return *this;
    }

    prevector&          operator=               ( prevector&& other )
    {
        if ( this != &other )
        {
            clear ();

            if ( other.m_iSize <= SIZE )
                memcpy ( m_Array, other.m_Array, other.m_iSize * sizeof(T) );
            else
                m_Vector = std::move ( other.m_Vector );

            m_iSize = other.m_iSize;
            other.m_iSize = 0;
        }
        return *this;
    }

    prevector&          assign                  ( const T* pT, int iSize )
    {
        clear ();
        m_iSize = iSize;
        if ( m_iSize > SIZE )
            m_Vector.resize ( m_iSize );

        for ( int i = 0; i < iSize; i++ )
            new (&(*this)[i]) T ( pT[i] );
        
        return *this;
    }

    bool                operator==              ( const prevector& other ) const
    {
        if ( size () != other.size () )
            return false;

        for ( int i = 0; i < size (); i++ )
        {
            if ( (*this)[i] != other[i] )
                return false;
        }
        return true;
    }

    bool                operator!=              ( const prevector& other ) const
    {
        return !( *this == other );
    }

    T&                  operator[]              ( size_t i )
    {
        assert ( i >= 0 && i < m_iSize );
        if ( m_iSize <= SIZE )
        {
            return *(T *)&m_Array[i];
        }
        else
        {
            return *(T *)&m_Vector[i];
        }
    }

    const T&            operator[]              ( size_t i ) const
    {
        assert ( i >= 0 && i < m_iSize );
        if ( m_iSize <= SIZE )
        {
            return *(const T *)&m_Array[i];
        }
        else
        {
            return *(const T *)&m_Vector[i];
        }
    }

    T*                  data                    ()
    {
        if ( m_iSize <= SIZE )
            return (T *)m_Array;
        else
            return (T *)m_Vector.data ();
    }

    const T*            data                    () const
    {
        if ( m_iSize <= SIZE )
            return (const T *)m_Array;
        else
            return (const T *)m_Vector.data ();
    }

    void                push_back               ( const T& t )
    {
        if ( m_iSize == SIZE )
        {
            m_Vector.resize ( SIZE );
            memcpy ( m_Vector.data (), m_Array, SIZE * sizeof(T) );
        }

        if ( m_iSize >= SIZE )
            m_Vector.push_back ( Placeholder () );

        m_iSize++;
        new (&(*this)[m_iSize - 1]) T ( t );
    }

    void                pop_back                ()
    {
        assert ( m_iSize > 0 );

        (&(*this)[m_iSize - 1])->~T ();

        if ( m_iSize > SIZE )
            m_Vector.pop_back ();

        m_iSize--;

        if ( m_iSize == SIZE )
        {
            memcpy ( m_Array, m_Vector.data (), SIZE * sizeof(T) );
            m_Vector.clear ();
        }        
    }

    size_t              size                    () const
    {
        return m_iSize;
    }

    bool                empty                   () const
    {
        return m_iSize == 0;
    }

    size_t              capacity                () const
    {
        return std::max ( SIZE, m_Vector.capacity () );
    }

    void                reserve                 ( size_t capacity )
    {
        if ( capacity <= SIZE )
            return;

        m_Vector.reserve ( capacity );
    }

    void                resize                  ( size_t iNewSize )
    {
        for ( int i = iNewSize; i < m_iSize; i++ )
        {
            (&(*this)[i])->~T ();
        }

        size_t iOldSize = m_iSize;

        resize_classless ( iNewSize );

        for ( int i = iOldSize; i < iNewSize; i++ )
        {
            new (&(*this)[i]) T ();
        }
    }

    void                clear                   ()
    {
        for ( size_t i = 0; i < m_iSize; i++ )
        {
            (&(*this)[i])->~T ();
        }
        m_Vector.clear ();
        m_iSize = 0;
    }

    class iterator
    {
        friend prevector < T, SIZE >;

    public:
                        iterator                ()
        {
            m_pVector = NULL;
            m_iPos = -1;
        }

                        iterator                ( const iterator& other )
        {
            *this = other;
        }

    private:
                        iterator                ( prevector& vector )
        {
            m_pVector = &vector;
            m_iPos = m_pVector->empty () ? -1 : 0;
        }

                        iterator                ( prevector& vector, int iPos )
        {
            m_pVector = &vector;
            m_iPos = iPos;
        }

    public:
        iterator&       operator=               ( const iterator& other )
        {
            m_pVector = other.m_pVector;
            m_iPos = other.m_iPos;
            return *this;
        }

        bool            operator==              ( const iterator& other ) const
        {
            if ( !m_pVector || !other.m_pVector || m_pVector != other.m_pVector )
                return false;

            return m_iPos == other.m_iPos;
        }

        bool            operator!=              ( const iterator& other ) const
        {
            return !(*this == other);
        }

        bool            operator<               ( const iterator& other ) const
        {
            if ( !m_pVector || !other.m_pVector || m_pVector != other.m_pVector )
                return false;

            int iPos1 = m_iPos < 0 ? m_pVector->size () : m_iPos;
            int iPos2 = other.m_iPos < 0 ? other.m_pVector->size () : other.m_iPos;
            return iPos1 < iPos2;
        }

        bool            operator<=              ( const iterator& other ) const
        {
            return *this < other || *this == other;
        }

        bool            operator>               ( const iterator& other ) const
        {
            if ( !m_pVector || !other.m_pVector || m_pVector != other.m_pVector )
                return false;

            int iPos1 = m_iPos < 0 ? m_pVector->size () : m_iPos;
            int iPos2 = other.m_iPos < 0 ? other.m_pVector->size () : other.m_iPos;
            return iPos1 > iPos2;
        }

        bool            operator>=              ( const iterator& other ) const
        {
            return *this > other || *this == other;
        }

        iterator&       operator++              ()
        {
            assert ( m_pVector && m_iPos >= 0 );
            
            m_iPos++;
            if ( m_iPos >= m_pVector->size () )
                m_iPos = -1;

            return *this;
        }

        iterator&       operator--              ()
        {
            assert ( m_pVector );

            if ( m_iPos == -1 )
            {
                assert ( !m_pVector->empty () );
                m_iPos = m_pVector->size ();
            }

            assert ( m_iPos > 0 );
            m_iPos--;
            return *this;
        }

        iterator        operator++              ( int )
        {
            iterator result ( *this );
            ++*this;
            return result;
        }

        iterator        operator--              ( int )
        {
            iterator result ( *this );
            --*this;
            return result;
        }

        iterator&       operator+=              ( int iOffset )
        {
            assert ( m_pVector );

            int iPos = m_iPos;
            if ( iPos < 0 )
                iPos = m_pVector->size ();

            assert ( iPos + iOffset >= 0 && iPos + iOffset <= m_pVector->size () );
            if ( iPos + iOffset == m_pVector->size () )
            {
                m_iPos = -1;
            }
            else
            {
                m_iPos = iPos + iOffset;
            }
            return *this;
        }

        iterator&       operator-=              ( int iOffset )
        {
            assert ( m_pVector );

            return *this += -iOffset;
        }

        iterator        operator+               ( int iOffset ) const
        {
            assert ( m_pVector );

            iterator it ( *this );
            it += iOffset;
            return it;
        }

        iterator        operator-               ( int offset ) const
        {
            assert ( m_pVector );

            return operator+ ( -offset );
        }

        T&              operator*               () const
        {
            assert ( m_pVector && m_iPos >= 0 );

            return m_pVector->operator[] ( m_iPos );
        }

        T*              operator->              () const
        {
            return &(**this);
        }

    private:
        prevector*      m_pVector;
        int             m_iPos;
    };

    iterator            begin                   ()
    {
        return iterator ( *this );
    }

    iterator            end                     ()
    {
        return iterator ( *this, -1 );
    }

    T&                  front                   ()
    {
        assert ( m_iSize > 0 );
        return (*this)[0];
    }

    T&                  back                    ()
    {
        assert ( m_iSize > 0 );
        return (*this)[m_iSize - 1];
    }

    void                insert                  ( int iIndex, const T& t )
    {
        assert ( iIndex >= 0 && iIndex <= m_iSize );

        resize_classless ( m_iSize + 1 );

        for ( int i = m_iSize - 1; i > iIndex; i-- )
        {
            *(Placeholder *)&(*this)[i] = *(Placeholder *)&(*this)[i - 1];
        }
        new (&(*this)[iIndex]) T ( t );
    }

    void                insert                  ( iterator& it, const T& t )
    {
        insert ( it.m_iPos < 0 ? size () : it.m_iPos, t );
    }

    void                erase                   ( int iIndex )
    {
        assert ( iIndex >= 0 && iIndex < m_iSize );

        (&(*this)[iIndex])->~T ();
        for ( int i = iIndex; i < m_iSize - 1; i++ )
        {
            *(Placeholder *)&(*this)[i] = *(Placeholder *)&(*this)[i + 1];
        }
        resize_classless ( m_iSize - 1 );
    }

    iterator            erase                   ( iterator& it )
    {
        erase ( it.m_iPos );
        return it.m_iPos < m_iSize ? it : end ();
    }

    bool                remove                  ( const T& t )
    {
        for ( iterator it = begin (); it != end (); it++ )
        {
            if ( *it == t )
            {
                erase ( it );
                return true;
            }
        }
        return false;
    }

    bool                contains                ( const T& t )
    {
        return find ( t ) != end ();
    }

    iterator            find                    ( const T& t )
    {
        for ( iterator it = begin (); it != end (); it++ )
        {
            if ( *it == t )
                return it;
        }
        return end ();
    }

private:
    void                resize_classless        ( size_t iNewSize )
    {
        if ( m_iSize <= SIZE && iNewSize > SIZE )
        {
            m_Vector.resize ( iNewSize );
            memcpy ( m_Vector.data (), m_Array, m_iSize * sizeof(T) );
        }
        else if ( m_iSize > SIZE && iNewSize <= SIZE )
        {
            memcpy ( m_Array, m_Vector.data (), iNewSize * sizeof(T) );
            m_Vector.clear ();
        }
        else if ( m_iSize > SIZE && iNewSize > SIZE )
        {
            m_Vector.resize ( iNewSize );
        }

        m_iSize = iNewSize;
    }

    struct Placeholder
    {
        byte            m_pad[sizeof(T)];
    };

    Placeholder         m_Array[SIZE];
    std::vector < Placeholder > m_Vector;
    size_t              m_iSize;
};
