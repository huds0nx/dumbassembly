#pragma once

template < class T, int SIZE >
class Pool
{
public:
                            Pool                ()
    {
        memset ( m_bActive, 0, sizeof(m_bActive) );
        m_iLowerPosBound = -1;
        m_iUpperPosBound = -1;
        m_iPos = 0;
        m_iSize = 0;
    }

                            ~Pool               ()
    {
        Clear ();
    }

    T*                      Create              ()
    {
        FindFreePos ();
        T* pObj = new ( &m_Buffer[m_iPos] ) T ();
        m_bActive [ m_iPos ] = true;
        m_iSize++;
        return pObj;
    }

    template < typename P1 >
    T*                      Create ( P1 p1 )
    {
        FindFreePos ();
        T* pObj = new ( &m_Buffer[m_iPos] ) T ( p1 );
        m_bActive [ m_iPos ] = true;
        m_iSize++;
        return pObj;
    }

    template < typename P1, typename P2 >
    T*                      Create ( P1 p1, P2 p2 )
    {
        FindFreePos ();
        T* pObj = new ( &m_Buffer[m_iPos] ) T ( p1, p2 );
        m_bActive [ m_iPos ] = true;
        m_iSize++;
        return pObj;
    }

    template < typename P1, typename P2, typename P3 >
    T*                      Create ( P1 p1, P2 p2, P3 p3 )
    {
        FindFreePos ();
        T* pObj = new ( &m_Buffer[m_iPos] ) T ( p1, p2, p3 );
        m_bActive [ m_iPos ] = true;
        m_iSize++;
        return pObj;
    }

    virtual void            Delete              ( T* pObj )
    {
        int iIndex = Index ( pObj );
        assert ( m_bActive [ iIndex ] );
        pObj->~T ();
        m_bActive [ iIndex ] = false;
        m_iSize--;

        if ( m_iSize == 0 )
        {
            m_iLowerPosBound = -1;
            m_iUpperPosBound = -1;
        }
        else
        {
            while ( !m_bActive[m_iLowerPosBound] )
                m_iLowerPosBound++;

            while ( !m_bActive[m_iUpperPosBound - 1] )
                m_iUpperPosBound--;
        }
    }

    int                     Index               ( const T* pObj ) const
    {
        int iIndex = pObj - reinterpret_cast < const T* > ( m_Buffer );
        assert ( iIndex >= 0 && iIndex < SIZE );
        return iIndex;
    }

    bool                    IsOccupied          ( int index ) const
    {
        assert ( index >= 0 && index < SIZE );
        return m_bActive [ index ];
    }

    T&                      At                  ( int iIndex )
    {
        assert ( m_bActive [ iIndex ] );
        return *reinterpret_cast < T* > ( &m_Buffer[iIndex] );
    }

    const T&                At                  ( int iIndex ) const
    {
        assert ( m_bActive [ iIndex ] );
        return *reinterpret_cast < const T* > ( &m_Buffer[iIndex] );
    }

    T&                      operator[]          ( int iIndex )
    {
        return At ( iIndex );
    }

    const T&                operator[]          ( int iIndex ) const
    {
        return At ( iIndex );
    }

    void                    Clear               ()
    {
        while ( m_iSize > 0 )
        {
            if ( m_bActive [ m_iPos ] )
                Delete ( &At ( m_iPos ) );
            m_iPos = m_iPos > 0 ? m_iPos - 1 : SIZE - 1;
        }
        m_iLowerPosBound = -1;
        m_iUpperPosBound = -1;
        m_iPos = 0;
    }

    int                     Size                () const
    {
        return m_iSize;
    }

    bool                    Empty               () const
    {
        return Size () == 0;
    }

    class iterator
    {
        friend Pool < T, SIZE >;

    public:
                            iterator            ()
    {
        m_pPool = NULL;
        m_iPos = -1;
    }

                            iterator            ( const iterator& other )
    {
        *this = other;
    }

    private:
                            iterator            ( Pool < T, SIZE >& pool )
        {
            m_pPool = &pool;
            m_iPos = m_pPool->Empty () ? -1 : m_pPool->GetLowerPosBound ();
        }

                            iterator            ( Pool < T, SIZE >& pool, int iPos )
        {
            m_pPool = &pool;
            m_iPos = iPos;
        }

    public:
        iterator&           operator=           ( const iterator& other )
        {
            m_pPool = other.m_pPool;
            m_iPos = other.m_iPos;
            return *this;
        }

        bool                operator==          ( const iterator& other ) const
        {
            if ( !m_pPool || !other.m_pPool || m_pPool != other.m_pPool )
                return false;

            return m_iPos == other.m_iPos;
        }

        bool                operator!=          ( const iterator& other ) const
        {
            return !(*this == other);
        }

        bool                operator<           ( const iterator& other ) const
        {
            if ( !m_pPool || !other.m_pPool || m_pPool != other.m_pPool )
                return false;

            int iPos1 = m_iPos < 0 ? SIZE : m_iPos;
            int iPos2 = other.m_iPos < 0 ? SIZE : other.m_iPos;
            return iPos1 < iPos2;
        }

        bool                operator<=          ( const iterator& other ) const
        {
            return *this < other || *this == other;
        }

        bool                operator>           ( const iterator& other ) const
        {
            if ( !m_pPool || !other.m_pPool || m_pPool != other.m_pPool )
                return false;

            int iPos1 = m_iPos < 0 ? SIZE : m_iPos;
            int iPos2 = other.m_iPos < 0 ? SIZE : other.m_iPos;
            return iPos1 > iPos2;
        }

        bool                operator>=          ( const iterator& other ) const
        {
            return *this > other || *this == other;
        }

        T&                  operator*           () const
        {
            assert ( m_pPool && m_iPos >= 0 );
            return (*m_pPool) [ m_iPos ];
        }

        T*                  operator->          () const
        {
            return &(**this);
        }

        iterator&           operator++          ()
        {
            assert ( m_pPool && m_iPos >= 0 );
            m_iPos++;
            for ( ; m_iPos < m_pPool->GetUpperPosBound (); m_iPos++ )
            {
                if ( m_pPool->m_bActive [ m_iPos ] )
                    break;
            }
            if ( m_iPos >= m_pPool->GetUpperPosBound () )
                m_iPos = -1;

            return *this;
        }

        iterator&           operator--          ()
        {
            assert ( m_pPool );
            if ( m_iPos < 0 )
            {
                assert ( !m_pPool->Empty () );
                m_iPos = m_pPool->GetUpperPosBound ();
            }
            else
            {
                assert ( m_iPos > m_pPool->GetLowerPosBound () );
            }

            m_iPos--;
            for ( ; m_iPos >= m_pPool->GetLowerPosBound (); m_iPos-- )
            {
                if ( m_pPool->m_bActive [ m_iPos ] )
                    break;
            }

            return *this;
        }

        iterator            operator++          ( int )
        {
            iterator result ( *this );
            ++*this;
            return result;
        }

        iterator            operator--          ( int )
        {
            iterator result ( *this );
            --*this;
            return result;
        }

    private:
        

        Pool < T, SIZE >*   m_pPool;
        int                 m_iPos;
    };
    friend iterator;

    iterator                Begin               ()
    {
        return iterator ( *this );
    }

    iterator                End                 ()
    {
        return iterator ( *this, -1 );
    }

protected:
    void                    FindFreePos         ()
    {
        assert ( m_iSize < SIZE );
        while ( m_bActive [ m_iPos ] )
            m_iPos = ( m_iPos + 1 ) % SIZE;

        if ( m_iLowerPosBound < 0 || m_iPos < m_iLowerPosBound )
            m_iLowerPosBound = m_iPos;

        if ( m_iUpperPosBound < 0 || m_iPos >= m_iUpperPosBound )
            m_iUpperPosBound = m_iPos + 1;
    }

    int                     GetLowerPosBound    () const
    {
        return m_iLowerPosBound < 0 ? 0 : m_iLowerPosBound;
    }

    int                     GetUpperPosBound    () const
    {
        return m_iUpperPosBound < 0 ? SIZE : m_iUpperPosBound;
    }

    struct Placeholder
    {
        byte ucPad[sizeof(T)];
    };
    Placeholder             m_Buffer[SIZE];
    bool                    m_bActive[SIZE];
    int                     m_iLowerPosBound;
    int                     m_iUpperPosBound;
    int                     m_iPos;
    int                     m_iSize;
};
