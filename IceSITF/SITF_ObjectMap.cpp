//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: Save Images To File
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\IceSITF\SITF_ObjectMap.cpp
//-          Date: Thu Jul 18 13:58:25 2019
//-      Language: C++
//-
//-   Description: Insert a functor into pipeline to save all incoming images into a file
//-
//-----------------------------------------------------------------------------------

// OBJECT_MAP macro definitions
#include "MrServers/MrVista/include/Parc/Reflection/ObjectMap.h"

// SITF Configurator:
#include "SITF_Configurator.h"

// SITFFunctor: Post processing for SITF sequence
#include "Functor_SaveImagesToFile_PostProcessing.h"

using namespace ICE_NAMESPACE;

// Export of classes outside of this Dll (needed once per Dll):
BEGIN_OBJECT_MAP()
    OBJECT_ENTRY(SITFConfigurator)	// configurator of the ICE program
    OBJECT_ENTRY(SaveImagesToFileFunctor)		// SITF post processing
END_OBJECT_MAP()

