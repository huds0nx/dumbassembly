#include "StdInc.h"

char Stream::ReadChar ()
{
    char c;
    Read ( c );
    return c;
}

wchar_t Stream::ReadWChar ()
{
    wchar_t c;
    Read ( c );
    return c;
}

byte Stream::ReadByte ()
{
    byte uc;
    Read ( uc );
    return uc;
}

short Stream::ReadInt16 ()
{
    short s;
    Read ( s );
    return s;
}

ushort Stream::ReadUInt16 ()
{
    ushort us;
    Read ( us );
    return us;
}

int Stream::ReadInt24 ()
{
    int i = 0;
    ReadBytes ( &i, 3 );
    i <<= 8;
    i >>= 8;
    return i;
}

uint Stream::ReadUInt24 ()
{
    uint ui = 0;
    ReadBytes ( &ui, 3 );
    return ui;
}

int Stream::ReadInt32 ()
{
    int i;
    Read ( i );
    return i;
}

uint Stream::ReadUInt32 ()
{
    uint ui;
    Read ( ui );
    return ui;
}

__int64 Stream::ReadInt64 ()
{
    __int64 i;
    Read ( i );
    return i;
}

unsigned __int64 Stream::ReadUInt64 ()
{
    unsigned __int64 ui;
    Read ( ui );
    return ui;
}

int Stream::ReadVarInt32 ()
{
    int i = ReadByte ();
    if ( i & 0x80 )
    {
        i = ((i & 0x7F) << 8) | ReadByte ();
        if ( i & 0x4000 )
        {
            byte uc1 = ReadByte ();
            byte uc0 = ReadByte ();
            i = ((i & 0x3FFF) << 16) | (uc1 << 8) | uc0;
        }
        return i;
    }
    else
    {
        return i;
    }
}

uint Stream::ReadVarUInt32 ()
{
    uint ui = ReadByte ();
    if ( ui & 0x80 )
    {
        ui = ((ui & 0x7F) << 8) | ReadByte ();
        if ( ui & 0x4000 )
        {
            byte uc1 = ReadByte ();
            byte uc0 = ReadByte ();
            ui = ((ui & 0x3FFF) << 16) | (uc1 << 8) | uc0;
        }
        return ui;
    }
    else
    {
        return ui;
    }
}

float Stream::ReadSingle ()
{
    float f;
    Read ( f );
    return f;
}

double Stream::ReadDouble ()
{
    double d;
    Read ( d );
    return d;
}

double Stream::ReadScrambledDouble ()
{
    byte ucBuff [ 8 ];
    ReadBytes ( &ucBuff[4], 4 );
    ReadBytes ( &ucBuff[0], 4 );
    return *(double *)ucBuff;
}

std::wstring Stream::ReadUtf8StringZ ()
{
    std::string str;
    char c;
    while ( ( c = ReadChar () ) != '\0' )
    {
        str += c;
    }
    
    return Utf8StringToWide ( str );
}

std::wstring Stream::ReadUtf8String ( int iLength )
{
    std::string str;
    str.resize ( iLength );
    ReadBytes ( const_cast < char* > ( str.data () ), iLength );
    return Utf8StringToWide ( str );
}

std::wstring Stream::ReadVarUtf8String ()
{
    return ReadUtf8String ( ReadVarInt32 () );
}

std::wstring Stream::ReadUtf16StringZ ()
{
    std::wstring wstr;
    wchar_t c;
    while ( ( c = ReadWChar () ) != L'\0' )
    {
        wstr += c;
    }
    return wstr;
}

std::wstring Stream::ReadUtf16String ( int iLength )
{
    std::wstring wstr;
    wstr.resize ( iLength );
    ReadBytes ( const_cast < wchar_t* > ( wstr.data () ), iLength * sizeof(wchar_t) );
    return wstr;
}

std::wstring Stream::ReadVarUtf16String ()
{
    return ReadUtf16String ( ReadVarInt32 () );
}

void Stream::WriteVarInt32 ( int iNum )
{
    if ( iNum <= 0x3F )
    {
        Write ( (byte)iNum );
    }
    else if ( iNum <= 0x3FFF )
    {
        Write ( (byte)(0x80 | (iNum >> 8)) );
        Write ( (byte)iNum );
    }
    else
    {
        Write ( (byte)(0xC0 | (iNum >> 0x18)) );
        Write ( (byte)(iNum >> 0x10) );
        Write ( (byte)(iNum >> 8) );
        Write ( (byte)iNum );
    }
}

void Stream::WriteUtf8StringZ ( std::wstring wstr )
{
    std::string str = WideStringToUtf8 ( wstr );
    WriteBytes ( str.data (), str.size () );
    Write ( '\0' );
}

void Stream::WriteVarUtf8String ( std::wstring wstr )
{
    std::string str = WideStringToUtf8 ( wstr );
    WriteVarInt32 ( str.size () );
    WriteBytes ( str.data (), str.size () );
}

void Stream::WriteUtf16StringZ ( std::wstring wstr )
{
    WriteBytes ( wstr.data (), wstr.size () * sizeof(wchar_t) );
    Write ( L'\0' );
}

void Stream::WriteVarUtf16String ( std::wstring wstr )
{
    WriteVarInt32 ( wstr.size () );
    WriteBytes ( wstr.data (), wstr.size () * sizeof(wchar_t) );
}

std::wstring Stream::Utf8StringToWide ( std::string str )
{
    std::wstring wstr;
    wstr.resize ( MultiByteToWideChar ( CP_UTF8, 0, str.c_str (), str.size (), NULL, 0 ) );
    MultiByteToWideChar ( CP_UTF8, 0, str.c_str (), str.size (), const_cast < wchar_t* > ( wstr.data () ), wstr.size () );
    return wstr;
}

std::string Stream::WideStringToUtf8 ( std::wstring wstr )
{
    std::string str;
    str.resize ( WideCharToMultiByte ( CP_UTF8, 0, wstr.c_str (), wstr.size (), NULL, 0, NULL, NULL ) );
    WideCharToMultiByte ( CP_UTF8, 0, wstr.c_str (), wstr.size (), const_cast < char* > ( str.data () ), str.size (), NULL, NULL );
    return str;
}
