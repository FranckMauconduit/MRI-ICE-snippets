//-----------------------------------------------------------------------------------
//-   Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: TEMPL4TE Post-processing
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\TEMPL4TE_postproc\TEMPL4TE_Configurator.cpp
//-          Date: Wed Jun  5 15:57:43 2019
//-      Language: C++
//-
//-   Description: 
//-
//-----------------------------------------------------------------------------------


// TEMPL4TEConfigurator class
#include "TEMPL4TE_Configurator.h"

// ICE_OUT, ICE_ERR
#include "MrServers/MrVista/include/Parc/Trace/IceTrace.h"

using namespace MED_MRES_XProtocol; //namespace of everything exported by XProtocol component

IResult TEMPL4TEConfigurator::Compose( ProtocolComposer::Toolbox& toolbox )
{
    using namespace ProtocolComposer;
    toolbox.Init( "IRIS" );

    // Initialize variables
    std::string     l_sPipeName, sConnectFunctor, sSuccessorFunctor;
    MrPipeService   l_mpPipeService;
    MrFunctor       fImafinish, fTEMPL4TE, fconnect;

    // Compilation time stamp
    ICE_OUT("\nCompilation was done " << __DATE__ << " at " << __TIME__ << "\n" );

    // ----------------------------------
    // Retrieve configuration parameters from MEAS
    // ----------------------------------
    // XProtocol& prot = toolbox.Protocol();
    
    // long    lCoilMode  = prot["MEAS.ucCoilCombineMode"].ToLong();
    // int     lWipParam0 = prot["MEAS.sWiPMemBlock.alFree"].ToLong(0);
    // int     lWipParam4 = prot["MEAS.sWiPMemBlock.alFree"].ToLong(4);
    // double  lWipParam1 = prot["MEAS.sWiPMemBlock.adFree"].ToDouble(3);
    
    // -------------------------------
    // Create TEMPL4TE post-processing Functor
    // -------------------------------
    fImafinish = toolbox.FindFunctor("imafinish", false);
    l_sPipeName = fImafinish.PipeServiceName();         // extract the pipeservice of "imTEMPL4TEnish"
    l_mpPipeService = toolbox.FindPipeService(l_sPipeName.c_str()); // locate the pipeservice

    // Create TEMPL4TE post-processing functor
    fTEMPL4TE = l_mpPipeService.CreateFunctor("TEMPL4TE_PostProcessing", "TEMPL4TEFunctor@IceTEMPL4TE");

    // bActivateFun    = l_mfTEMPL4TE.GetProperty( "ActivateFunctor" ).ToBool();
    // bSingleThread   = l_mfTEMPL4TE.GetProperty( "SingleThreading" ).ToBool();
    
    // ICE_OUT("\ActivateFunctor =  " << (bActivateFun?"true":"false") << ", SingleThread = " << (bSingleThread?"true":"false") );
    
    // ----------------------------------
    // Desactivation of TEMPL4TE Functor integration in several cases
    // ----------------------------------
    // if ( ! bActivateFun )
    // {
        // ICE_OUT("\n#####################################################"
            // "\n TEMPL4TE Functor desactivated : configuration file"
            // "\n See IceTEMPL4TE.evp configurator file for activation"
            // "\n#####################################################\n");
        // TEMPL4TE functor desactivated from evp parameters
        // Stops here to make sure that original functor chain works properly
        // return I_OK;
    // }

    
    // -------------------------------
    // Insert TEMPL4TE post-processing in Functor chain
    // -------------------------------
    // if ( bSingleThread )
    // {
        // l_mpPipeService.PoolThreads(1);
        // ICE_OUT("Pipe Service is set to single threaded.\n");
    // }

    // -------------------------------
    // Find XXX Functor to connect TEMPL4TE_PostProcessing
    // sConnectFunctor.assign("XXX");
    // fconnect = toolbox.FindFunctor(sConnectFunctor.c_str(), false);
    // if ( fconnect.IsBad() )
    // {
        // ICE_OUT("\n#####################################################"
            // "\n XXX functor not found in functor chain"
            // "\n Trying to connect to extractMag instead"
            // "\n#####################################################\n");

        // sConnectFunctor.assign("extractMAG");
        // fconnect = toolbox.FindFunctor(sConnectFunctor.c_str(), false);
        // if ( fconnect.IsBad() )
        // {
            // ICE_OUT("\n#####################################################"
                // "\n TEMPL4TE Functor desactivated : Functor connection"
                // "\n extractMag functor not found in functor chain"
                // "\n#####################################################\n");
        
            // return I_OK;
        // }

        // ICE_OUT("\n#####################################################"
            // "\n extractMag functor connection successfull"
            // "\n#####################################################\n");
            
    // }
    
    // sSuccessorFunctor = toolbox.GetSuccessor(sConnectFunctor.c_str(),"ImageReady").SinkFunctor();    
    // toolbox.Connect ( sConnectFunctor.c_str(), "ImageReady", "TEMPL4TE_PostProcessing", "ComputeImage" );
    // toolbox.Connect ( "TEMPL4TE_PostProcessing", "ImageReady", sSuccessorFunctor.c_str(), "ComputeImage" );
    // toolbox.Disconnect ( sConnectFunctor.c_str(), "ImageReady", sSuccessorFunctor.c_str(), "ComputeImage");


    // ICE_OUT("\n#####################################################"
            // "\n TEMPL4TE Functor added succesfully to funtor chain"
            // "\n#####################################################\n");

    
    // ----------------------------------
    // End of the chain configuration
    // ----------------------------------

    return I_OK;
}

// Note: For available conversion methods of prot["..."].ToXXX() see file \n4\pkg\MrServers\MrVista\include\Parc\XNode.h

// Note: Code example for throwing an exception 'Functor myFunctor not found' in the Configurator if searched Functor doesn't exist:
//           toolbox.FindFunctor("myFunctor").SetProperty("myProperty", myValue);
//       Code example for catching a non-existent Functor with a specific error message:
//           MrFunctor  predecessor = toolbox.FindFunctor("myFunctor", false);
//           if(predecessor.IsBad())
//           {
//               ICE_ERROR("xxx");
//           }
//               else
//           {
//               predecessor.SetProperty("myProperty", myValue); 
//           }
