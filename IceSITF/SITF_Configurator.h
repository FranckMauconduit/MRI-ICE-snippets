//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: Save Images To File
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\IceSITF\SITF_Configurator.h
//-          Date: Thu Jul 18 13:58:25 2019
//-      Language: C++
//-
//-   Description: Insert a functor into pipeline to save all incoming images into a file
//-
//-----------------------------------------------------------------------------------

#ifndef XFL_Configurator_h
#define XFL_Configurator_h 1


#include "MrServers/MrVista/include/Parc/ProtocolComposer.h"

class SITFConfigurator : public Parc::Component, public ProtocolComposer::IComposer
{
public:
    DECLARE_PARC_COMPONENT(SITFConfigurator);

    virtual IResult Compose( ProtocolComposer::Toolbox& toolbox );
};

#endif
