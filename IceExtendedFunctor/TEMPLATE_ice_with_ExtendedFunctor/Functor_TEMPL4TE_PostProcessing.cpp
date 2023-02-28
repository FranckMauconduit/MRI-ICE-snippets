//-----------------------------------------------------------------------------------
//-   Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: TEMPL4TE Post-processing
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\TEMPL4TE\Functor_TEMPL4TE_PostProcessing.cpp
//-          Date: Wed Jun  5 15:57:43 2019
//-      Language: C++
//-
//-   Description: 
//-
//-----------------------------------------------------------------------------------


// TEMPL4TEFunctor class
#include "Functor_TEMPL4TE_PostProcessing.h"

// Functor to display data
#include "MrServers/MrVista/include/Ice/IceUtils/IceUtils.h"

// createDicomInstanceUid
#include "MrServers/MrVista/include/Parc/Utils/DicomUtils.h"

// Read external file
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

using namespace ICE_NAMESPACE;

//-----------------------------------------------------------------------------
// TEMPL4TEFunctor()
// Description: constructor method of the "TEMPL4TEFunctor" class.
//-----------------------------------------------------------------------------

TEMPL4TEFunctor::TEMPL4TEFunctor()
    :m_bVerboseMode( true )
{
    // register callback for FirstCall in Functor base class
    // (otherwise e.g. method FirtsCall(...) won't be called):
    addCB(IFirstCall);
}


//-----------------------------------------------------------------------------
// ~TEMPL4TEFunctor()
// Description: destructor method of the "TEMPL4TEFunctor" class.
//-----------------------------------------------------------------------------

TEMPL4TEFunctor::~TEMPL4TEFunctor()
{
}


//-----------------------------------------------------------------------------
// EndInit(IParcEnvironment*)
// Description: EndInit method of the "TEMPL4TEFunctor" class.
//-----------------------------------------------------------------------------

IResult TEMPL4TEFunctor::EndInit(IParcEnvironment* env)
{
    ICE_SET_FN("TEMPL4TEFunctor::EndInit(env)")
    
    // Definition for Extended Functor class
    // Either changing values in here or call Base class with definition
    // bDebugModeInX = true;
    // bIsNumberInSeriesActicatedInX = true;

    // Always call base class:
    IResult Res = ExtendedFunctor::EndInit(env);
    if (failed(Res))
        ICE_RET_ERR("Error calling ExtendedFunctor::EndInit(), aborting...", Res)

    // Get the PDS service for saving images through study
    m_pPDS = Parc::cast<IPDS>(m_env->Item("PDS"));
    if (!m_pPDS)
        ICE_RET_ERR("Cannot retrieve ParcDataStore! Pointer is invalid.", I_FAIL);
    
    // Set parameters for storing images in during ComputeImage
    cStoreDATA.setIceFormat(ICE_FORMAT_CXFL);   // Images will be saved as complex float, could be ICE_FORMAT_FL or ICE_FORMAT_SH as well
    cStoreDATA.saveInPARC(false);               // Images will not be saved in PARC
    cStoreDATA.saveInPDS(true);                 // Images will be saved in PDS
    cStoreDATA.setPrefix("DATA");               // will be set when calling function storeImageInPARC
    
    Res = IceUtils::GetSeriesInstanceUID(m_env, this, "IncomingData", cSendAmpli.sUID);
    if(failed(Res))
    ICE_RET_ERR("Failed calling IceUtils::GetSeriesInstanceUID 'IncomingData', aborting...", Res)
    
    Res = IceUtils::GetSeriesInstanceUID(m_env, this, "SumData", cSendSum.sUID);
    if(failed(Res))
    ICE_RET_ERR("Failed calling IceUtils::GetSeriesInstanceUID 'SumData', aborting...", Res)
    
    // --------------------------------------
    // Define properties to send each image to next functor (serie UID, comment, scaling, ice_format, etc)
    cSendAmpli.setScale(1.0);
    cSendAmpli.setIceFormat(ICE_FORMAT_FL);
    cSendAmpli.setExtractMode(IemAmplitude);
    
    cSendSum.setScale(0.5);
    cSendSum.setIceFormat(ICE_FORMAT_FL);
    cSendSum.setExtractMode(IemAmplitude);
    
    // cSendPhase.setScale(PhaseScale);
    // cSendPhase.setIceFormat(ICE_FORMAT_FL);  // useless because of default definition 
    // cSendPhase.setExtractMode(IemPhase);
    // cSendPhase.setDicomImaType(P);
    
    // Define suffix for all series
    // cSendAmpli.setSuffix("_Amplitude");
    // cSendPhase.setSuffix("_Phase");
    cSendSum.setSuffix("_sum");

    // Define Image comments
    // cSendAmpli.setComment("Reference scan - module (arb. unit)");
    // cSendPhase.setComment("Reference scan - phase  (unit: 0.1 degree)");
    cSendSum.setComment("Sum of echo 1 and 2");
    
    return I_OK;
}


//-----------------------------------------------------------------------------
// FirstCall(IceAs&, MiniHeader&, ImageControl&)
// Description: FirstCall method
//-----------------------------------------------------------------------------

IResult TEMPL4TEFunctor::FirstCall(IceAs&, MiniHeader::Pointer& dataHeader, ImageControl& ctrl)
{
    ICE_SET_FN("TEMPL4TEFunctor::FirstCall(...)")

    // Initialize local variables
    
    return I_OK;
}



//-----------------------------------------------------------------------------
// endOfJob(IResult reason)
// Description: endOfJob method
//-----------------------------------------------------------------------------

IResult TEMPL4TEFunctor::endOfJob(IResult reason)
{
    ICE_SET_FN("TEMPL4TEFunctor::endOfJob(...)");
    
    // Initialize local variables
    std::stringstream   stream;

    if(reason == I_ACQ_END)
    {
        stream << "ACQ_END";
    }
    else
    {
        if (reason==I_STOP)
        {stream << "STOP";}
        else if (reason==I_FAIL)
        {stream << "FAIL";}
        else if (reason==I_USER)
        {stream << "USER";}
        else
        {stream << "status unknow";}

        ICE_OUT( "EndofJob() called with reason " << stream.str() );
    }
    return I_OK;
}


//-----------------------------------------------------------------------------
// ComputeImage(IceAs&, MiniHeader&, ImageControl&)
// Description: TEMPL4TEFunctor method
//-----------------------------------------------------------------------------

IResult TEMPL4TEFunctor::ComputeImage(
        IceAs& srcAs, 
        MiniHeader::Pointer& dataHeader, 
        ImageControl& ctrl      )
{
    ICE_SET_FN("TEMPL4TEFunctor::ComputeImage(...)")
    
    // Initialize local variables
    IResult     Res;
    
    // ------------------------------
    // Check all registered callbacks
    // ------------------------------
    // (TEMPL4TEFunctor's FirstCall(...) is called here):
    Res = ExecuteCallbacks(srcAs, dataHeader, ctrl);
    if(failed(Res))
    {ICE_RET_ERR("ExecuteCallbacks failed, aborting...", Res)}
    
    if ( ICE_DEBUG_ON( TRACE_1 ))
    {IceUtils::CheckAndDisplay( srcAs, IceDisplayData::DISPLAY, " - Incoming image" );}
    
    // Store incoming image
    Res = storeImageInPARC( srcAs, ctrl, cStoreDATA );
    if ( failed(Res) ) {ICE_RET_ERR("Error when calling storeImageInPARC for a reference image\n", Res )}

    if ( ctrl.m_lc.m_ceco == 1 )
    {
        // Assuming eco 0 and 1 arrived, do some job on eco 0 and 1 in computeMaps
        computeMaps(srcAs, dataHeader, ctrl);
    }
    
    // Call next Functor in chain
    // Res = ImageReady(srcAs, dataHeader, ctrl);
    // instead of ImageReady, send incoming image to next functor using
    sendData2NextFunctor( srcAs, dataHeader, ctrl, cSendAmpli );
    
    if ( ICE_DEBUG_ON( TRACE_2 ))
    {IceUtils::CheckAndDisplay( srcAs, IceDisplayData::DISPLAY, " - Outgoing image" );}

    return I_OK;
}

//-----------------------------------------------------------------------------
// END OF COMPUTE IMAGE
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// computeMaps(IceAs&, ImageControl&)
// Description: method to compute B1 maps
//-----------------------------------------------------------------------------

IResult TEMPL4TEFunctor::computeMaps(
        IceAs& imageAs,
        ImageControl& ctrl      )
{
    ICE_SET_FN("TEMPL4TEFunctor::computeB1Maps(...)")

    // Initialize local variables
    IResult         Res;
    IceLoopCounters lc;
    IceAs           Eco0As, Eco1As;
    ImageControl    SumCtrl(ctrl);
    
    // Retrieve reference image ID corresponding to the incoming image
    lc = ctrl.m_lc;

    lc.m_ceco = 0;
    Res = retrieveImageFromPARC( Eco0As, mscCtrl.m_lc, cStoreDATA );
    if ( failed(Res) ) {ICE_RET_ERR("Error when calling retrieveImageFromPARC, aborting",I_FAIL);}
    
    lc.m_ceco = 1;
    Res = retrieveImageFromPARC( Eco1As, mscCtrl.m_lc, cStoreDATA );
    if ( failed(Res) ) {ICE_RET_ERR("Error when calling retrieveImageFromPARC, aborting",I_FAIL);}
    
    
    // Ready to do some job on Echo 0 and 1 like adding them for exemple
    Ice.Add(Eco1As, Eco1As, Eco0As);
    
    // And directly send it to next functor as a different series
    SumCtrl.m_imaDimBoundaries.m_ceco=1;
    SumCtrl.m_lc.m_ceco=0;
    sendData2NextFunctor( Eco1As, dataHeader, SumCtrl, cSendSum );
    
    // if (ICE_DEBUG_ON(TRACE_3))
    // {IceUtils::CheckAndDisplay( Echo1As, IceDisplayData::DISPLAY, " - added map" );}

    return I_OK;
}

//-----------------------------------------------------------------------------
// EOF
//-----------------------------------------------------------------------------
