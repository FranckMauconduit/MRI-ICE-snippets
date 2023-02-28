//-----------------------------------------------------------------------------------
//-   Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: TEMPL4TE Post-processing
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\TEMPL4TE_postproc\TEMPL4TE_ObjectMap.cpp
//-          Date: Wed Jun  5 15:57:43 2019
//-      Language: C++
//-
//-   Description: 
//-
//-----------------------------------------------------------------------------------


// OBJECT_MAP macro definitions
#include "MrServers/MrVista/include/Parc/Reflection/ObjectMap.h"

// TEMPL4TE Configurator:
#include "TEMPL4TE_Configurator.h"

// TEMPL4TEFunctor: Post processing for TEMPL4TE functor
#include "Functor_TEMPL4TE_PostProcessing.h"

using namespace ICE_NAMESPACE;

// Export of classes outside of this Dll (needed once per Dll):
BEGIN_OBJECT_MAP()
    OBJECT_ENTRY(TEMPL4TEConfigurator)  // configurator of the ICE program
    OBJECT_ENTRY(TEMPL4TEFunctor)       // TEMPL4TE post processing
END_OBJECT_MAP()

