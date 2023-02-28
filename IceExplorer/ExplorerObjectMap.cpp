//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: IceExplorer
//-       Version: IceExplorer
//-        Author: Franck MAUCONDUIT (franck.mauconduit@siemens.com)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\XFL_postproc\ExplorerObjectMap.cpp
//-          Date: Fri Nov 18 18:35:18 2016
//-      Language: C++
//-
//-   Description: <ICE functor to explore and dump data in a pipeline.>
//-
//-----------------------------------------------------------------------------------

// InverterConfigurator:
#include "MrServers/MrVista/Ice/IceIdeaFunctors/IceExplorer/ExplorerConfigurator.h"
// InverterFunctor:
#include "MrServers/MrVista/Ice/IceIdeaFunctors/IceExplorer/ExplorerImageFunctor.h"
#include "MrServers/MrVista/Ice/IceIdeaFunctors/IceExplorer/ExplorerScanFunctor.h"

// OBJECT_MAP macro definitions
#include "MrServers/MrVista/include/Parc/Reflection/ObjectMap.h"
#include "MrServers/MrVista/include/Parc/Reflection.h"

// Export of classes outside of this Dll (needed once per Dll):
BEGIN_OBJECT_MAP()
    OBJECT_ENTRY(ExplorerImageFunctor)
    OBJECT_ENTRY(ExplorerScanFunctor)
    OBJECT_ENTRY(ExplorerConfigurator)
END_OBJECT_MAP()
