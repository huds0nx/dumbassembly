#pragma once

template < class T, int PREITEMSPERPRIORITY = 10 >
class prioritystack
{
    typedef prevector < T, PREITEMSPERPRIORITY > PriorityItem_t;
    typedef std::map < int, PriorityItem_t > Stack_t;

public:
                                prioritystack           ()
    {

    }

                                prioritystack           ( const prioritystack& other )
    {
        *this = other;
    }

                                prioritystack           ( prioritystack&& other )
    {
        *this = std::move ( other );
    }

    prioritystack&              operator=               ( const prioritystack& other )
    {
        if ( this != &other )
        {
            m_Stack = other.m_Stack;
        }
        return *this;
    }

    prioritystack&              operator=               ( prioritystack&& other )
    {
        if ( this != &other )
        {
            m_Stack = std::move ( other );
        }
        return *this;
    }

    void                        push_back               ( int priority, const T& t )
    {
        m_Stack[priority].push_back ( t );
    }

    void                        pop_back                ( int priority )
    {
        if ( m_Stack.count ( priority ) == 0 )
            return;

        m_Stack[priority].pop_back ( priority );
        if ( m_Stack [ priority ].empty () )
            m_Stack.erase ( priority );
    }

    void                        pop_back                ()
    {
        assert ( !m_Stack.empty () );

        Stack_t::iterator it = m_Stack.end ();
        it--;
        it->second.pop_back ();
        if ( it->second.empty () )
            m_Stack.erase ( it );
    }

    void                        clear                   ()
    {
        m_Stack.clear ();
    }

    int                         size                    () const
    {
        int result = 0;
        for ( Stack_t::const_iterator it = m_Stack.begin (); it != m_Stack.end (); it++ )
        {
            result += it->second.size ();
        }
        return result;
    }

    bool                        empty                   () const
    {
        return m_Stack.empty ();
    }

    class iterator
    {
        friend prioritystack < T, PREITEMSPERPRIORITY >;

    public:
                                iterator                ()
        {
            m_pStack = NULL;
        }

                                iterator                ( const iterator& other )
            : m_pStack ( other.m_pStack ),
              m_PrioritiesIt ( other.m_PrioritiesIt ),
              m_PriorityIt ( other.m_PriorityIt )
        {

        }

    private:
                                iterator                ( prioritystack < T, PREITEMSPERPRIORITY >& stack )
        {
            m_pStack = &stack;

            if ( m_pStack->empty () )
            {
                m_PrioritiesIt = m_pStack->m_Stack.end ();
            }
            else
            {
                m_PrioritiesIt = m_pStack->m_Stack.begin ();
                m_PriorityIt = m_PrioritiesIt->second.begin ();
            }
        }

    public:
        iterator&               operator=               ( const iterator& other )
        {
            m_pStack = other.m_pStack;
            m_PrioritiesIt = other.m_PrioritiesIt;
            m_PriorityIt = other.m_PriorityIt;
            return *this;
        }

        bool                    operator==              ( const iterator& other ) const
        {
            if ( !m_pStack || !other.m_pStack || m_pStack != other.m_pStack )
                return false;

            if ( m_PrioritiesIt == m_pStack->m_Stack.end () &&
                 other.m_PrioritiesIt == other.m_pStack->m_Stack.end () )
                return true;

            return m_PrioritiesIt == other.m_PrioritiesIt &&
                   m_PriorityIt == other.m_PriorityIt;
        }

        bool                    operator!=              ( const iterator& other ) const
        {
            return !(*this == other);
        }

        bool                    operator<               ( const iterator& other ) const
        {
            if ( !m_pStack || !other.m_pStack || m_pStack != other.m_pStack )
                return false;

            return m_PrioritiesIt < other.m_PrioritiesIt ||
                   ( m_PrioritiesIt == other.m_PrioritiesIt && m_PriorityIt < other.m_PriorityIt );
        }

        bool                    operator<=              ( const iterator& other ) const
        {
            return *this < other || *this == other;
        }

        bool                    operator>               ( const iterator& other ) const
        {
            if ( !m_pStack || !other.m_pStack || m_pStack != other.m_pStack )
                return false;

            return m_PrioritiesIt > other.m_PrioritiesIt ||
                   ( m_PrioritiesIt == other.m_PrioritiesIt && m_PriorityIt > other.m_PriorityIt );
        }

        bool                    operator>=              ( const iterator& other ) const
        {
            return *this > other || *this == other;
        }

        iterator&               operator++              ()
        {
            assert ( m_pStack );
            assert ( m_PrioritiesIt != m_pStack->m_Stack.end () );

            m_PriorityIt++;
            if ( m_PriorityIt == m_PrioritiesIt->second.end () )
            {
                m_PrioritiesIt++;
                if ( m_PrioritiesIt == m_pStack->m_Stack.end () )
                    return *this;

                m_PriorityIt = m_PrioritiesIt->second.begin ();
            }
            return *this;
        }

        iterator&               operator--              ()
        {
            assert ( m_pStack );

            if ( m_PrioritiesIt == m_pStack->m_Stack.end () )
            {
                assert ( !m_pStack->m_Stack.empty () );
                m_PrioritiesIt--;
                m_PriorityIt = m_PrioritiesIt->second.end ();
            }

            assert ( !(m_PrioritiesIt == m_pStack->m_Stack.begin () &&
                       m_PriorityIt == m_PrioritiesIt->second.begin () ) );

            if ( m_PriorityIt == m_PrioritiesIt->second.begin () )
            {
                m_PrioritiesIt--;
                m_PriorityIt = m_PrioritiesIt->second.end ();
            }
            m_PriorityIt--;
            return *this;
        }

        iterator                operator++              ( int )
        {
            iterator result ( *this );
            ++*this;
            return result;
        }

        iterator                operator--              ( int )
        {
            iterator result ( *this );
            --*this;
            return result;
        }

        T&                      operator*               () const
        {
            assert ( m_pStack );
            assert ( m_PrioritiesIt != m_pStack->m_Stack.end () &&
                     m_PriorityIt != m_PrioritiesIt->second.end () );

            return *m_PriorityIt;
        }

        T*                      operator->              () const
        {
            return &(**this);
        }

		iterator				operator+				( int iOffset ) const
		{
			if ( iOffset < 0 )
				return *this - (-iOffset);
			
			iterator result ( *this );
			for ( int i = 0; i < iOffset; i++ )
			{
				result++;
			}
			return result;
		}

		iterator				operator-				( int iOffset ) const
		{
			if ( iOffset < 0 )
				return *this + (-iOffset);
			
			iterator result ( *this );
			for ( int i = 0; i < iOffset; i++ )
			{
				result--;
			}
			return result;
		}
			
        int                     priority                () const
        {
            assert ( m_pStack );
            assert ( m_PrioritiesIt != m_pStack->m_Stack.end () &&
                     m_PriorityIt != m_PrioritiesIt->second.end () );

            return m_PrioritiesIt->first;
        }

    private:
        prioritystack < T, PREITEMSPERPRIORITY >*                                   m_pStack;
        typename prioritystack < T, PREITEMSPERPRIORITY >::Stack_t::iterator        m_PrioritiesIt;
        typename prioritystack < T, PREITEMSPERPRIORITY >::PriorityItem_t::iterator m_PriorityIt;
    };
    friend iterator;

    iterator                    begin                   ()
    {
        return iterator ( *this );
    }

    iterator                    end                     ()
    {
        iterator it ( *this );
        it.m_PrioritiesIt = m_Stack.end ();
        return it;
    }

    T&                          front                   ()
    {
        assert ( !empty () );
        return *begin ();
    }

    T&                          back                    ()
    {
        assert ( !empty () );
        iterator it = end ();
        it--;
        return *it;
    }

    bool                        contains                ( const T& t )
    {
        return find ( t ) != end ();
    }

    bool                        contains                ( int iPriority, const T& t )
    {
        return find ( iPriority, t ) != end ();
    }

    iterator                    find                    ( const T& t )
    {
        typename Stack_t::iterator stackIt;
        for ( stackIt = m_Stack.begin (); stackIt != m_Stack.end (); stackIt++ )
        {
            typename PriorityItem_t::iterator priorityIt = stackIt->second.find ( t );
            if ( priorityIt != stackIt->second.end () )
            {
                iterator it ( *this );
                it.m_PrioritiesIt = stackIt;
                it.m_PriorityIt = priorityIt;
                return it;
            }
        }
        return end ();
    }

    iterator                    find                    ( int iPriority, const T& t )
    {
        typename Stack_t::iterator stackIt = m_Stack.find ( iPriority );
        if ( stackIt == m_Stack.end () )
            return end ();

        typename PriorityItem_t::iterator priorityIt = stackIt->second.find ( t );
        if ( priorityIt == stackIt->second.end () )
            return end ();

        iterator it ( *this );
        it.m_PrioritiesIt = stackIt;
        it.m_PriorityIt = priorityIt;
        return it;
    }

    iterator                    erase                   ( iterator it )
    {
        assert ( it.m_pStack == this );

        it.m_PriorityIt = it.m_PrioritiesIt->second.erase ( it.m_PriorityIt );
        if ( it.m_PrioritiesIt->second.empty () )
            it.m_PrioritiesIt = m_Stack.erase ( it.m_PrioritiesIt );

        return it;
    }

    bool                        remove                  ( const T& t )
    {
        iterator it = find ( t );
        if ( it == end () )
            return false;

        erase ( it );
        return true;
    }

private:
    Stack_t                     m_Stack;
};
