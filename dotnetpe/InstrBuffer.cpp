#include "StdInc.h"

namespace dotnetpe
{

    InstrBuffer::InstrBuffer ()
    {

    }

    InstrBuffer::InstrBuffer ( byte* pInstr, dword dwLength )
    {
        Assign ( pInstr, dwLength );
    }

    InstrBuffer::~InstrBuffer ()
    {

    }

    bool InstrBuffer::Contains ( byte* pInstr )
    {
        return pInstr >= Data () && pInstr < Data () + Size ();
    }

    void InstrBuffer::Assign ( byte* pInstr, dword dwLength )
    {
        m_Buffer.assign ( pInstr, dwLength );
    }

    InstrBuffer::iterator InstrBuffer::Begin ()
    {
        if ( Size () == 0 )
            return End ();

        return iterator ( this );
    }

    InstrBuffer::iterator InstrBuffer::End ()
    {
        return iterator ();
    }

    void InstrBuffer::AppendInstrBuffer ( const InstrBuffer& buffer )
    {
        int iOrigSize = Size ();
        Resize ( iOrigSize + buffer.Size () );
        memcpy ( Data () + iOrigSize, buffer.Data (), buffer.Size () );
    }

    void InstrBuffer::AppendInstr ( byte* pInstr )
    {
        InsertInstr ( Data () + Size (), pInstr );
    }

    void InstrBuffer::InsertInstr ( byte* pAt, byte* pInstr )
    {
        assert ( pAt >= Data () && pAt <= Data () + Size () );

        int iLength = ILInstr::GetLength ( pInstr );
        int iPos = pAt - Data ();
        m_Buffer.resize ( Size () + iLength );
        pAt = Data () + iPos;
        memmove ( pAt + iLength, pAt, Size () - iLength - iPos );
        memcpy ( pAt, pInstr, iLength );
    }

    void InstrBuffer::InsertInstr ( iterator at, byte* pInstr )
    {
        InsertInstr ( *at, pInstr );
    }

    void InstrBuffer::EraseInstr ( byte* pAt )
    {
        assert ( Contains ( pAt ) );

        int iLength = ILInstr::GetLength ( pAt );
        int iPos = pAt - Data ();
        if ( iPos + iLength >= Size () )
        {
            m_Buffer.resize ( iPos );
            return;
        }

        memmove ( pAt, pAt + iLength, Size () - iPos - iLength );
        m_Buffer.resize ( Size () - iLength );
    }

    InstrBuffer::iterator InstrBuffer::EraseInstr ( iterator at )
    {
        EraseInstr ( *at );
        if ( at.isvalid () )
            return at;
        else
            return End ();
    }

    void InstrBuffer::Resize ( int iSize )
    {
        m_Buffer.resize ( iSize );
    }

    void InstrBuffer::Clear ()
    {
        m_Buffer.clear ();
    }

}
