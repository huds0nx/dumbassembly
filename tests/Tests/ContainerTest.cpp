#include "StdInc.h"

ContainerTest::ContainerTest ()
{

}

ContainerTest::~ContainerTest ()
{

}

void ContainerTest::Run ()
{
    TestLazyVector ();
    TestPreVector ();
    TestPriorityStack ();
    TestPool ();
}

void ContainerTest::TestLazyVector ()
{
    // Primitive type
    lazyvector < int > vec1;
    assert ( vec1.size () == 0 );
    assert ( vec1.empty () );
    
    vec1.resize ( 5 );
    assert ( vec1.size () == 5 );
    for ( int i = 0; i < vec1.size (); i++ )
    {
        assert ( vec1[i] == 0 );
        vec1[i] = i;
        assert ( vec1[i] == i );
    }
    vec1.clear ();
    assert ( vec1.empty () );
    assert ( vec1.size () == 0 );

    // Complex type
    lazyvector < Item > vec2 ( 2 );
    assert ( vec2.size () == 2 );
    for ( int i = 0; i < vec2.size (); i++ )
    {
        assert ( !vec2.active ( i ) );
        assert ( vec2[i].Get () == Item::DEFAULT );
        assert ( vec2.active ( i ) );
    }
    vec2.resize ( 5 );
    for ( int i = 0; i < vec2.size (); i++ )
    {
        assert ( vec2[i].Get () == Item::DEFAULT );
    }

    int iDtorCounter = 0;
    for ( int i = 0; i < vec2.size (); i++ )
    {
        vec2[i].SetDtorCounter ( &iDtorCounter );
    }

    vec2.resize ( 2 );
    assert ( iDtorCounter == 3 );
    vec2.clear ();
    assert ( iDtorCounter == 5 );
}

void ContainerTest::TestPreVector ()
{
    // Simple type
    prevector < int, 3 > vec1;
    assert ( vec1.empty () );
    assert ( vec1.size () == 0 );
    for ( int i = 0; i < 5; i++ )
    {
        vec1.push_back ( 1 + i );
        assert ( vec1.back () == 1 + i );
    }
    assert ( vec1.front () == 1 );
    assert ( vec1.end () - 1 == vec1.begin () + 4 );

    int num = 0;
    prevector < int, 3 >::iterator it;
    for ( it = vec1.begin (); it != vec1.end (); it++ )
    {
        num++;
        assert ( *it == num );
    }
    assert ( num == 5 );

    it = vec1.end ();
    while ( it != vec1.begin () )
    {
        it--;
        assert ( *it == num );
        num--;
    }
    assert ( num == 0 );

    assert ( vec1.begin () < vec1.end () );
    assert ( vec1.begin () < vec1.end () - 1 );

    vec1.pop_back ();
    assert ( vec1.size () == 4 && vec1.back () == 4 );
    vec1.pop_back ();
    assert ( vec1.size () == 3 && vec1.back () == 3 );
    vec1.pop_back ();
    assert ( vec1.size () == 2 && vec1.back () == 2 );

    vec1.push_back ( 3 );
    vec1.insert ( vec1.begin () + 1, 0 );
    assert ( vec1.size () == 4 );
    assert ( vec1[0] == 1 &&
             vec1[1] == 0 &&
             vec1[2] == 2 &&
             vec1[3] == 3 );

    vec1.insert ( vec1.begin () + 3, 4 );
    assert ( vec1.size () == 5 );
    assert ( vec1[0] == 1 &&
             vec1[1] == 0 &&
             vec1[2] == 2 &&
             vec1[3] == 4 &&
             vec1[4] == 3 );

    vec1.erase ( vec1.begin () + 3 );
    vec1.erase ( vec1.begin () + 1 );
    assert ( vec1.size () == 3 );
    for ( int i = 0; i < 3; i++ )
    {
        assert ( vec1[i] == 1 + i );
    }

    vec1.clear ();
    assert ( vec1.size () == 0 );
    assert ( vec1.empty () );

    // Complex type
    prevector < Item, 3 > vec2 ( 5 );
    assert ( vec2.size () == 5 );
    for ( int i = 0; i < vec2.size (); i++ )
    {
        assert ( vec2[i].Get () == Item::DEFAULT );
        vec2[i].Set ( i );
        assert ( vec2[i].Get () == i );
    }

    int iDtorCount = 0;
    for ( int i = 0; i < vec2.size (); i++ )
    {
        vec2[i].SetDtorCounter ( &iDtorCount );
    }
    vec2.resize ( 2 );
    assert ( vec2.size () == 2 );
    assert ( iDtorCount == 3 );
    vec2.erase ( vec2.end () - 1 );
    assert ( vec2.size () == 1 );
    assert ( iDtorCount == 4 );
    vec2.clear ();
    assert ( vec2.size () == 0 );
    assert ( vec2.empty () );
    assert ( iDtorCount == 5 );
}

void ContainerTest::TestPriorityStack ()
{
    prioritystack < int > ps1;
    assert ( ps1.empty () );
    for ( int i = 0; i < 10; i++ )
    {
        ps1.push_back ( i % 2, i );
    }
    assert ( ps1.size () == 10 );
    assert ( ps1.front () == 0 );

    int num = 0;
    prioritystack < int >::iterator it;
    for ( it = ps1.begin (); it != ps1.end (); it++ )
    {
        if ( num < 5 )
        {
            assert ( it.priority () == 0 );
            assert ( *it == 2*num );
        }
        else
        {
            assert ( it.priority () == 1 );
            assert ( *it == 1 + 2*(num - 5) );
        }
        num++;
    }
    assert ( num == 10 );

    it = ps1.end ();
    num = 9;
    while ( it != ps1.begin () )
    {
        it--;
        if ( num < 5 )
        {
            assert ( it.priority () == 0 );
            assert ( *it == 2*num );
        }
        else
        {
            assert ( it.priority () == 1 );
            assert ( *it == 1 + 2*(num - 5) );
        }
        num--;
    }
    assert ( num == -1 );

    for ( int i = 9; i >= 0; i-- )
    {
        if ( i < 5 )
        {
            assert ( ps1.back () == 2*i );
        }
        else
        {
            assert ( ps1.back () == 1 + 2*(i - 5) );
        }
        ps1.pop_back ();
        assert ( ps1.size () == i );
    }

    assert ( ps1.empty () );
    assert ( ps1.size () == 0 );
}

void ContainerTest::TestPool ()
{
    Pool < Item, 5 > pool;
    assert ( pool.Empty () );
    assert ( pool.Size () == 0 );

    Item* pItem0 = pool.Create ();
    assert ( pItem0->Get () == Item::DEFAULT );

    Item* pItem1 = pool.Create ( 5 );
    assert ( pItem1->Get () == 5 );

    assert ( pool.Index ( pItem0 ) == 0 );
    assert ( pool.Index ( pItem1 ) == 1 );

    assert ( pool.Size () == 2 );

    Pool < Item, 5 >::iterator it;
    int num = 0;
    for ( it = pool.Begin (); it != pool.End (); it++ )
    {
        if ( num == 0 )
            assert ( it->Get () == Item::DEFAULT );
        else if ( num == 1 )
            assert ( it->Get () == 5 );

        num++;
    }
    assert ( num == 2 );

    num = 1;
    it = pool.End ();
    while ( it != pool.Begin () )
    {
        it--;

        if ( num == 0 )
            assert ( it->Get () == Item::DEFAULT );
        else if ( num == 1 )
            assert ( it->Get () == 5 );

        num--;
    }
    assert ( num == -1 );

    int iDtorCount = 0;
    for ( Pool < Item, 5 >::iterator it = pool.Begin (); it != pool.End (); it++ )
    {
        it->SetDtorCounter ( &iDtorCount );
    }

    pool.Delete ( pItem0 );
    assert ( iDtorCount == 1 );
    pool.Clear ();
    assert ( iDtorCount == 2 );
}
