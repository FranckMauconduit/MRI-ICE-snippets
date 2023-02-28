//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: IceExplorer
//-       Version: IceExplorer
//-        Author: Franck MAUCONDUIT (franck.mauconduit@siemens.com)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\XFL_postproc\ExplorerConfigurator.h
//-          Date: Fri Nov 18 18:35:18 2016
//-      Language: C++
//-
//-   Description: <ICE functor to explore and dump data in a pipeline.>
//-
//-----------------------------------------------------------------------------------

#ifndef ExplorerConfigurator_h
#define ExplorerConfigurator_h 1


#include "MrServers/MrVista/include/Parc/ProtocolComposer.h"

class ExplorerConfigurator : public Parc::Component, public ProtocolComposer::IComposer
{
public:
    DECLARE_PARC_COMPONENT(ExplorerConfigurator);

    virtual IResult Compose( ProtocolComposer::Toolbox& toolbox );
};

#endif
