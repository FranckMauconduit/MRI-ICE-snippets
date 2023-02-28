//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: Save Images To File
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\IceSITF\SITF_Configurator.cpp
//-          Date: Thu Jul 18 13:58:25 2019
//-      Language: C++
//-
//-   Description: Insert a functor into pipeline to save all incoming images into a file
//-
//-----------------------------------------------------------------------------------


// SITFConfigurator:
#include "SITF_Configurator.h"

// ICE_OUT, ICE_ERR
#include "MrServers/MrVista/include/Parc/Trace/IceTrace.h"

using namespace MED_MRES_XProtocol; //namespace of everything exported by XProtocol component

IResult SITFConfigurator::Compose( ProtocolComposer::Toolbox& toolbox )
{
	using namespace ProtocolComposer;
	toolbox.Init( "IRIS" );

	// Initialize variables
	std::string	    l_sPipeName1, l_sPipeName2;
	std::stringstream	strICEOUT;
	MrPipeService	l_mpPipeService1, l_mpPipeService2;
    IResult         Res;
    
	ICE_OUT("\nCompilation time was " << __DATE__ << " at " << __TIME__ << "\n" );
	
	// ----------------------------------
	// Retrieve configuration parameters from 
	// ----------------------------------
	// XProtocol& prot = toolbox.Protocol();
    
	// Single Thread required for SaveImagesToFile because of IceAs in class for saving data
	bool SingleThread 	     = true;

    std::string LinkedFunctor = "combineMAG";
    std::string SinkName, EventName;
    bool        bIsSuccessor;
    std::string RemoveFun;
    bool        bextractMAG_mode32 = false;
    
	// -------------------------------
	// Insert XFL post-processing in Functor chain
	// -------------------------------
	
    if ( RemoveFun.size() != 0 )
    {
        // AdaptiveImageFilter functor does not accept ICE_FORMAT_FL type
        Res = toolbox.Remove(RemoveFun.c_str(), true);
        if ( failed(Res) ) {
        ICE_ERR("Failed to remove functor \"" << RemoveFun << "\", aborting ..."); return I_FAIL;}
    }
    
	// The SITF post processing is inserted before or after LinkedFunctor
	MrFunctor fFunctor = toolbox.FindFunctor("imagesend", false);
	l_sPipeName1 = fFunctor.PipeServiceName();			// extract the pipeservice of "imafinish"
	l_mpPipeService1	= toolbox.FindPipeService(l_sPipeName1.c_str());	// locate the pipeservice
    
    
	// Create "l_mfdummy post-processing" to retrieve LinkedFunctor info
	MrFunctor l_mfdummy = l_mpPipeService1.CreateFunctor("dummy", "SaveImagesToFileFunctor@IceSITF");
    LinkedFunctor   = l_mfdummy.GetProperty("LinkedFunctor").ToString();
    SinkName        = l_mfdummy.GetProperty("SinkName").ToString();
    EventName       = l_mfdummy.GetProperty("EventName").ToString();
	toolbox.Remove("dummy", true);
        
	MrFunctor fLink = toolbox.FindFunctor(LinkedFunctor.c_str(), false);
	l_sPipeName2 = fLink.PipeServiceName();			// extract the pipeservice of "imafinish"
        
    // Create "SITF post-processing"
    l_mpPipeService2	= toolbox.FindPipeService( l_sPipeName2.c_str() );
	MrFunctor l_mfSITF = l_mpPipeService2.CreateFunctor("SaveImagesToFile", "SaveImagesToFileFunctor@IceSITF");
    
    // Functor parameters
    bIsSuccessor  = l_mfSITF.GetProperty("Successor").ToBool(); //false; //prot["SITF.Successor"].ToBool();
    bextractMAG_mode32  = l_mfSITF.GetProperty("extractMAG_mode32").ToBool(); //false; //prot["SITF.Successor"].ToBool();
    
    ICE_OUT("\n LinkedFunctor   = "<<LinkedFunctor<<
            "\n SinkName        = "<<SinkName<<
            "\n EventName       = "<<EventName<<
            "\n Successor       = "<<bIsSuccessor<<
            "\n extractMAG mode = "<< (bextractMAG_mode32?"float":"short") <<
            "\n");
        
    if ( bIsSuccessor )
    {
        if ( !toolbox.GetSuccessor(LinkedFunctor.c_str(),EventName.c_str(),false).IsOk() )
        {
            toolbox.Connect(LinkedFunctor.c_str(), EventName.c_str(), "SaveImagesToFile", "ComputeImage");
        }
        else
        {
            toolbox.InsertAfter ( "SaveImagesToFile", "ComputeImage", "ImageReady", LinkedFunctor.c_str(), EventName.c_str() );
        }
    }
    else
    {toolbox.InsertBefore ( "SaveImagesToFile", "ComputeImage", "ImageReady", LinkedFunctor.c_str(), SinkName.c_str() );}
	
    if ( SingleThread == true )
	{
		l_mpPipeService2.PoolThreads(1);
		ICE_OUT("Pipe Service is set to single threaded.\n");		
	}

  #ifdef _VX_VERSION_VB
    // Change functor property to return float data instead of short
    if ( bextractMAG_mode32 )
    {
        MrFunctor fExtractMAG = toolbox.FindFunctor("extractMAG", false);
        fExtractMAG.SetProperty("FunctorMode", 32);
    }
  #endif
  
	ICE_OUT("Functor \"SaveImagesToFile\" connected " << (bIsSuccessor?"after":"before") << " \""<< LinkedFunctor.c_str() <<"\".");
	return I_OK;

}

// Note: For available conversion methods of prot["..."].ToXXX() see file \n4\pkg\MrServers\MrVista\include\Parc\XNode.h
