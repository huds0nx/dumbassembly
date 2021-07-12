#pragma once

class Stream
{
public:
                        Stream                  () : m_bReadOnly ( false ) {}
    virtual             ~Stream                 () {}

    virtual int         GetPosition             () const = 0;
    virtual void        SetPosition             ( int iPosition ) = 0;
    virtual void        Seek                    ( int iOffset ) = 0;

    virtual int         Size                    () const = 0;

    bool                IsReadOnly              () const    { return m_bReadOnly; }

    virtual void        ReadBytes               ( void* pBuffer, int iSize ) = 0;
    virtual dword       ReadBits                ( int count ) = 0;
    virtual byte        PeekByte                () const = 0;

    virtual void        WriteBytes              ( const void* pData, int iSize ) = 0;

    virtual void        SkipBits                ( int count ) = 0;
    virtual void        AlignByte               () = 0;

    template < typename T >
    void                Read                    ( T& data )
    {
        ReadBytes ( &data, sizeof(data) );
    }

    char                ReadChar                ();
    wchar_t             ReadWChar               ();
    byte                ReadByte                ();
    short               ReadInt16               ();
    ushort              ReadUInt16              ();
    int                 ReadInt24               ();
    uint                ReadUInt24              ();
    int                 ReadInt32               ();
    uint                ReadUInt32              ();
    __int64             ReadInt64               ();
    unsigned __int64    ReadUInt64              ();
    int                 ReadVarInt32            ();
    uint                ReadVarUInt32           ();
    float               ReadSingle              ();
    double              ReadDouble              ();
    double              ReadScrambledDouble     ();

    std::wstring        ReadUtf8StringZ         ();
    std::wstring        ReadUtf8String          ( int iLength );
    std::wstring        ReadVarUtf8String       ();

    std::wstring        ReadUtf16StringZ        ();
    std::wstring        ReadUtf16String         ( int iLength );
    std::wstring        ReadVarUtf16String      ();

    template < typename T >
    void                Write                   ( T data )
    {
        WriteBytes ( &data, sizeof(data) );
    }

    template < typename T >
    void                WriteByRef              ( const T& data )
    {
        WriteBytes ( &data, sizeof(data) );
    }

    void                WriteVarInt32           ( int iNum );

    void                WriteUtf8StringZ        ( std::wstring wstr );
    void                WriteVarUtf8String      ( std::wstring wstr );

    void                WriteUtf16StringZ       ( std::wstring wstr );
    void                WriteVarUtf16String     ( std::wstring wstr );

protected:
    std::wstring        Utf8StringToWide        ( std::string str );
    std::string         WideStringToUtf8        ( std::wstring wstr );

    bool                m_bReadOnly;
};
