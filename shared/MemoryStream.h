#pragma once

class MemoryStream : public Stream
{
public:
                    MemoryStream            ();
                    MemoryStream            ( const void* pData, int iSize, bool bReadOnly = false );
    virtual         ~MemoryStream           ();

    void*           Data                    () const    { return m_pBuffer; }

    virtual int     GetPosition             () const    { return m_iPosition; }
    virtual void    SetPosition             ( int iPosition );
    virtual void    Seek                    ( int iOffset );

    virtual int     Size                    () const    { return m_iSize; }
    void            Resize                  ( int iSize );

    int             Capacity                () const    { return m_iCapacity; }
    void            Reserve                 ( int iCapacity );

    virtual void    ReadBytes               ( void* pBuffer, int iSize );
    virtual dword   ReadBits                ( int count );
    virtual byte    PeekByte                () const;

    virtual void    ReadSz                  ( std::string& str );

    virtual void    SkipBits                ( int count );
    virtual void    AlignByte               ();

    virtual void    WriteBytes              ( const void* pData, int iSize );

protected:
    byte*           m_pBuffer;
    int             m_iSize;
    int             m_iCapacity;
    int             m_iPosition;
    int             m_iBitOffset;
};
