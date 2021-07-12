#pragma once

#define COR20_METAHEADER_SIGNATURE 0x424A5342

namespace dotnetpe
{

    class DotNetPE : public pe::PE
    {
    public:
                                        DotNetPE            ( const wchar_t* pwszFileName );
                                        DotNetPE            ( byte* pData, dword dwSize );
                                        ~DotNetPE           ();

        IMAGE_COR20_HEADER*             GetCor20Header      () const;

        StreamLayer*                    GetStreamLayer      ()              { return &m_StreamLayer; }
        TableLayer*                     GetTableLayer       ()              { return &m_TableLayer; }
        StructureLayer*                 GetStructureLayer   ()              { return &m_StructureLayer; }

        int                             NumMetaStreams      () const        { return m_StreamLayer.NumMetaStreams (); }
        MetaStream*                     GetMetaStream       ( int iIndex )  { return m_StreamLayer.GetMetaStream ( iIndex ); }
        MetaStream*                     GetMetaStream       ( const char* pszName ) { return m_StreamLayer.GetMetaStream ( pszName ); }

        MetaStream*                     GetTableMetaStream  () const    { return m_StreamLayer.GetTableMetaStream (); }
        MetaStream*                     GetStringsMetaStream() const    { return m_StreamLayer.GetStringsMetaStream (); }
        MetaStream*                     GetUSMetaStream     () const    { return m_StreamLayer.GetUSMetaStream (); }
        MetaStream*                     GetGUIDMetaStream   () const    { return m_StreamLayer.GetGUIDMetaStream (); }
        MetaStream*                     GetBlobMetaStream   () const    { return m_StreamLayer.GetBlobMetaStream (); }

        MetaTable*                      GetMetaTable        ( MetaTable::eType type )   { return m_TableLayer.GetMetaTable ( type ); }

        Assembly*                       GetAssembly         ()          { return m_StructureLayer.GetAssembly (); }
        Module*                         GetModule           ()          { return m_StructureLayer.GetModule (); }

        std::wstring                    GetUserString       ( dword dwOffset ) const;

        int                             NumResources        () const                        { return m_StructureLayer.NumResources (); }
        Resource*                       GetResource         ( int iResource )               { return m_StructureLayer.GetResource ( iResource ); }
        Resource*                       GetResource         ( const char* pszName )         { return m_StructureLayer.GetResource ( pszName ); }

    private:
        void                            Init                ();

        StreamLayer                     m_StreamLayer;
        TableLayer                      m_TableLayer;
        StructureLayer                  m_StructureLayer;
    };

}
