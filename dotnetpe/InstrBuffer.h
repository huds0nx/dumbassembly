#pragma once

namespace dotnetpe
{

    class InstrBuffer
    {
    public:
                            InstrBuffer                 ();
                            InstrBuffer                 ( byte* pInstr, dword dwLength );
                            ~InstrBuffer                ();

        byte*               Data                        ()          { return m_Buffer.data (); }
        const byte*         Data                        () const    { return m_Buffer.data (); }
        dword               Size                        () const    { return m_Buffer.size (); }

        bool                Contains                    ( byte* pInstr );

        void                Assign                      ( byte* pInstr, dword dwLength );

        class iterator
        {
            friend InstrBuffer;

        public:
                                iterator                    ()
            {
                m_pBuffer = NULL;
                m_iOffset = -1;
            }

                                iterator                    ( const iterator& other )
            {
                *this = other;
            }

            iterator&           operator=                   ( const iterator& other )
            {
                if ( this != &other )
                {
                    m_pBuffer = other.m_pBuffer;
                    m_iOffset = other.m_iOffset;
                }
                return *this;
            }

            byte*               operator*                   () const
            {
                assert ( isvalid () );
                return m_pBuffer->Data () + m_iOffset;
            }

            dword               Length                      () const
            {
                return ILInstr::GetLength ( **this );
            }

            iterator&           operator++                  ()
            {
                assert ( isvalid () );
                m_iOffset += Length ();
                if ( m_iOffset >= m_pBuffer->Size () )
                {
                    m_pBuffer = NULL;
                    m_iOffset = -1;
                }
                return *this;
            }

            iterator            operator++                  ( int )
            {
                iterator old ( *this );
                ++*this;
                return old;
            }

            bool                operator==                  ( const iterator& other ) const
            {
                return m_pBuffer == other.m_pBuffer && m_iOffset == other.m_iOffset;
            }

            bool                operator!=                  ( const iterator& other ) const
            {
                return !(*this == other);
            }

            bool                operator<                   ( const iterator& other ) const
            {
                if ( !m_pBuffer || m_pBuffer != other.m_pBuffer )
                    return false;

                return m_iOffset < other.m_iOffset;
            }

            bool                operator<=                  ( const iterator& other ) const
            {
                if ( !m_pBuffer || m_pBuffer != other.m_pBuffer )
                    return false;

                return m_iOffset <= other.m_iOffset;
            }

            bool                operator>=                  ( const iterator& other ) const
            {
                if ( !m_pBuffer || m_pBuffer != other.m_pBuffer )
                    return false;

                return m_iOffset >= other.m_iOffset;
            }

            bool                operator>                   ( const iterator& other ) const
            {
                if ( !m_pBuffer || m_pBuffer != other.m_pBuffer )
                    return false;

                return m_iOffset > other.m_iOffset;
            }

        private:
                                iterator                    ( InstrBuffer* pBuffer )
            {
                m_pBuffer = pBuffer;
                m_iOffset = 0;
            }

            bool                isvalid                     () const
            {
                return m_pBuffer && m_iOffset >= 0 && m_iOffset < m_pBuffer->Size ();
            }

            InstrBuffer*        m_pBuffer;
            int                 m_iOffset;
        };

        iterator            Begin                       ();
        iterator            End                         ();

        void                AppendInstrBuffer           ( const InstrBuffer& buffer );

        void                AppendInstr                 ( byte* pInstr );

        void                InsertInstr                 ( byte* pAt, byte* pInstr );
        void                InsertInstr                 ( iterator at, byte* pInstr );

        void                EraseInstr                  ( byte* pAt );
        iterator            EraseInstr                  ( iterator at );

        void                Resize                      ( int iSize );

        void                Clear                       ();

    private:
        typedef prevector < byte, 0x20 > Buffer_t;
        Buffer_t            m_Buffer;
    };

}
