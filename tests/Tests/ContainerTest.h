#pragma once

class ContainerTest : public Test
{
public:
                            ContainerTest               ();
    virtual                 ~ContainerTest              ();

    virtual void            Run                         ();

private:
    class Item
    {
    public:
        enum { DEFAULT = 9001 };

                            Item                        () : m_iA ( DEFAULT ), m_pDtorCounter ( NULL ) {}
                            Item                        ( int iA ) : m_iA ( iA ), m_pDtorCounter ( NULL ) {}
                            ~Item                       ()
        {
            if ( m_pDtorCounter )
                ++*m_pDtorCounter;
        }

        int                 Get                         () const    { return m_iA; }
        void                Set                         ( int iA )  { m_iA = iA; }

        void                SetDtorCounter              ( int* pDtorCounter ) { m_pDtorCounter = pDtorCounter; }

    private:
        int                 m_iA;
        int*                m_pDtorCounter;
    };

    void                    TestLazyVector              ();
    void                    TestPreVector               ();
    void                    TestPriorityStack           ();
    void                    TestPool                    ();
};
