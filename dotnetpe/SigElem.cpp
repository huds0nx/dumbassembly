#include "StdInc.h"

namespace dotnetpe
{

    SigElem::SigElem ( StructureLayer* pStructureLayer )
        : m_pStructureLayer ( pStructureLayer ), m_pData ( NULL )
    {

    }

    SigElem::SigElem ( StructureLayer* pStructureLayer, const void* pData )
        : m_pStructureLayer ( pStructureLayer ), m_pData ( (const byte *)pData )
    {

    }

    SigElem::~SigElem ()
    {

    }

}
