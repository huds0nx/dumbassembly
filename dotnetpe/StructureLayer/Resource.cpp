#include "StdInc.h"

namespace dotnetpe
{

    Resource::Resource ()
    {

    }

    Resource::~Resource ()
    {

    }

    const char* Resource::GetName () const
    {
        return m_Record.GetName ();
    }

    void* Resource::Data () const
    {
        DotNetPE* pPE = m_pStructureLayer->GetPE ();
        return pPE->RVAToPtr ( pPE->GetCor20Header ()->Resources.VirtualAddress + m_Record.GetOffset () + 4 );
    }

    dword Resource::Size () const
    {
        return *(dword *)( (byte *)Data () - 4 );
    }

}
