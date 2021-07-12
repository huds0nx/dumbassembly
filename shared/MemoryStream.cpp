#include "StdInc.h"

MemoryStream::MemoryStream ()
    : m_pBuffer ( NULL ), m_iSize ( 0 ), m_iCapacity ( 0 ), m_iPosition ( 0 ), m_iBitOffset ( 0 )
{
    
}

MemoryStream::MemoryStream ( const void* pData, int iSize, bool bReadOnly )
    : m_pBuffer ( NULL ), m_iSize ( 0 ), m_iCapacity ( 0 ), m_iPosition ( 0 ), m_iBitOffset ( 0 )
{
    assert ( iSize >= 0 );
    if ( iSize == 0 )
        return;

    m_bReadOnly = bReadOnly;
    if ( bReadOnly )
    {
        m_pBuffer = (byte *)pData;
        m_iSize = iSize;
    }
    else
    {
        Resize ( iSize );
        memcpy ( m_pBuffer, pData, iSize );
    }
}

MemoryStream::~MemoryStream ()
{
    if ( m_pBuffer && !m_bReadOnly )
        delete[] m_pBuffer;
}

void MemoryStream::SetPosition ( int iPosition )
{
    assert ( iPosition >= 0 && iPosition <= m_iSize );
    m_iPosition = iPosition;
    m_iBitOffset = 0;
}

void MemoryStream::Seek ( int iOffset )
{
    AlignByte ();
    assert ( m_iPosition + iOffset >= 0 && m_iPosition + iOffset <= m_iSize );
    m_iPosition += iOffset;
}

void MemoryStream::Resize ( int iSize )
{
    assert ( iSize >= 0 );
    Reserve ( iSize );
    m_iSize = iSize;
}

void MemoryStream::Reserve ( int iCapacity )
{
    assert ( iCapacity >= 0 );
    if ( iCapacity <= m_iCapacity )
        return;

    if ( !m_iCapacity )
        m_iCapacity = 1;

    while ( m_iCapacity < iCapacity )
        m_iCapacity *= 2;

    byte* pNewBuffer = new byte[m_iCapacity];
    if ( m_pBuffer )
    {
        memcpy ( pNewBuffer, m_pBuffer, m_iSize );
        delete[] m_pBuffer;
    }
    m_pBuffer = pNewBuffer;
}

dword MemoryStream::ReadBits ( int count )
{
    assert ( m_iPosition < m_iSize );
    byte ucCurByte = PeekByte () & ( (1 << (8 - m_iBitOffset)) - 1 );
    if ( count < 8 - m_iBitOffset )
    {
        ucCurByte >>= (8 - m_iBitOffset) - count;
        m_iBitOffset += count;
        return ucCurByte;
    }
    dword dwResult = ucCurByte;
    count -= 8 - m_iBitOffset;
    m_iBitOffset = 0;
    Seek ( 1 );
    while ( count >= 8 )
    {
        dwResult = (dwResult << 8) | ReadByte ();
        count -= 8;
    }
    if ( count )
    {
        assert ( m_iPosition < m_iSize );
        dwResult = (dwResult << count) | (PeekByte () >> (8 - count));
    }
    m_iBitOffset = count;
    return dwResult;
}

byte MemoryStream::PeekByte () const
{
    assert ( m_iPosition < m_iSize );
    return m_pBuffer [ m_iPosition ];
}

void MemoryStream::SkipBits ( int count )
{
    assert ( count >= 0 );
    if ( count < 8 - m_iBitOffset )
    {
        m_iBitOffset += count;
        return;
    }
    count -= 8 - m_iBitOffset;
    Seek ( count/8 );
    m_iBitOffset = count % 8;
}

void MemoryStream::AlignByte ()
{
    if ( m_iBitOffset )
    {
        m_iBitOffset = 0;
        Seek ( 1 );
    }
}

void MemoryStream::ReadBytes ( void* pBuffer, int iSize )
{
    AlignByte ();
    assert ( m_iPosition + iSize >= 0 && m_iPosition + iSize <= m_iSize );
    memcpy ( pBuffer, (byte *)m_pBuffer + m_iPosition, iSize );
    m_iPosition += iSize;
}

void MemoryStream::ReadSz ( std::string& str )
{
    AlignByte ();
    int iLength = 0;
    while ( m_iPosition + iLength < m_iSize && m_pBuffer[m_iPosition + iLength] != '\0' )
    {
        iLength++;
    }
    str.assign ( (char *)&m_pBuffer[m_iPosition], iLength );
    m_iPosition += iLength;
    if ( m_iPosition < m_iSize )
        m_iPosition++;
}

void MemoryStream::WriteBytes ( const void* pData, int iSize )
{
    assert ( !IsReadOnly () );

    Resize ( std::max ( m_iPosition + iSize, m_iSize ) );
    memcpy ( &m_pBuffer[m_iPosition], pData, iSize );
    m_iPosition += iSize;
}
