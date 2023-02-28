//-----------------------------------------------------------------------------------
//-   Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: TEMPL4TE Post-processing
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\TEMPL4TE_postproc\TEMPL4TE_Configurator.h
//-          Date: Wed Jun  5 15:57:43 2019
//-      Language: C++
//-
//-   Description: 
//-
//-----------------------------------------------------------------------------------


#ifndef TEMPL4TE_Configurator_h
#define TEMPL4TE_Configurator_h 1


#include "MrServers/MrVista/include/Parc/ProtocolComposer.h"

class TEMPL4TEConfigurator : public Parc::Component, public ProtocolComposer::IComposer
{
public:
    DECLARE_PARC_COMPONENT(TEMPL4TEConfigurator);

    virtual IResult Compose( ProtocolComposer::Toolbox& toolbox );
};

#endif
