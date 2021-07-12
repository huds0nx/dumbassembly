#pragma once

template < class T >
class lazyvector
{
public:
                                lazyvector          () {}
                                lazyvector          ( size_t len ) : m_Data ( len ), m_Active ( len ) {}

                                ~lazyvector         ()
    {
        clear ();
    }

    T&                          operator[]          ( size_t index )
    {
        assert ( index >= 0 && index < m_Data.size () );
        if ( !m_Active[index] )
        {
            new (&m_Data[index]) T();
            m_Active[index] = true;
        }
        return *reinterpret_cast < T* > ( &m_Data[index] );
    }

    size_t                      size                () const
    {
        return m_Data.size ();
    }

    bool                        empty               () const
    {
        return size () == 0;
    }

    void                        resize              ( size_t newsize )
    {
        assert ( newsize >= 0 );
        if ( newsize < size () )
        {
            for ( size_t i = newsize; i < size (); i++ )
            {
                if ( m_Active[i] )
                    (*this)[i].~T ();
            }
        }
        m_Data.resize ( newsize );
        m_Active.resize ( newsize );
    }

    void                        reserve             ( size_t capacity )
    {
        assert ( capacity >= size () );
        m_Data.reserve ( capacity );
        m_Active.reserve ( capacity );
    }

    void                        clear               ()
    {
        resize ( 0 );
    }

    bool                        active              ( size_t index )
    {
        return m_Active[index];
    }

private:
    struct Placeholder
    {
        byte pad[sizeof(T)];
    };

    std::vector < Placeholder > m_Data;
    std::vector < bool >        m_Active;
};
