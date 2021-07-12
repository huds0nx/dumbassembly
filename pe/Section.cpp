#include "StdInc.h"

namespace pe
{

    Section::Section ( void* pFile, IMAGE_SECTION_HEADER* pHeader )
        : m_pFile ( pFile ), m_pHeader ( pHeader )
    {
        
    }

}
