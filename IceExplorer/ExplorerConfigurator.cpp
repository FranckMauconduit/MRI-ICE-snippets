//-----------------------------------------------------------------------------------
//-   Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: IceExplorer
//-       Version: IceExplorer
//-        Author: Franck MAUCONDUIT (franck.mauconduit@siemens.com)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\XFL_postproc\ExplorerConfigurator.cpp
//-          Date: Fri Nov 18 18:35:18 2016
//-      Language: C++
//-
//-   Description: <ICE functor to explore and dump data in a pipeline.>
//-
//-----------------------------------------------------------------------------------

#include "MrServers/MrVista/Ice/IceIdeaFunctors/IceExplorer/ExplorerConfigurator.h"

#include "MrServers/MrVista/include/Parc/Trace/IceTrace.h"

using namespace MED_MRES_XProtocol;  //namespace of everything exported by XProtocol component

IResult ExplorerConfigurator::Compose( ProtocolComposer::Toolbox& toolbox )
{

    using namespace ProtocolComposer;
    toolbox.Init( "IRIS" );

    IResult     Res;
    std::string CompilTimeStamp, ScanPipeName, ImagPipeName;
    MrFunctor   ExplorerScanFunctor, ExplorerImagFunctor, ScanLinkedFunctor, ImagLinkedFunctor;
    MrPipeService ScanPipeService, ImagPipeService;
    
    ICE_OUT("\nCompilation time was " << __DATE__ << " at " << __TIME__ << "\n" );

    // ----------------------------------
    // Set properties of inserted Functor
    // ----------------------------------
    XProtocol& prot = toolbox.Protocol();

    // SCAN FUNCTOR
    const bool Scan_IsActivated  = prot["EXPLORER.SCAN_FUNCTOR.Activate"].ToBool();
    const bool Scan_IsSuccessor  = prot["EXPLORER.SCAN_FUNCTOR.Successor"].ToBool();
    std::string Scan_functorStr = prot["EXPLORER.SCAN_FUNCTOR.LinkedFunctor"].ToString();
    
    // IMAGE FUNCTOR
    const bool Imag_IsActivated  = prot["EXPLORER.IMAGE_FUNCTOR.Activate"].ToBool();
    const bool Imag_IsSuccessor  = prot["EXPLORER.IMAGE_FUNCTOR.Successor"].ToBool();
    std::string Imag_functorStr = prot["EXPLORER.IMAGE_FUNCTOR.LinkedFunctor"].ToString();
    std::string Imag_PreMethod  = prot["EXPLORER.IMAGE_FUNCTOR.PredecessorMethod"].ToString();
    std::string Imag_SucEvent   = prot["EXPLORER.IMAGE_FUNCTOR.SuccessorEvent"].ToString();
    
    // -------------------------------
    // Insert Functor in Functor chain
    // -------------------------------
    
    // SCAN FUNCTOR
    if ( Scan_IsActivated ) {
    ScanLinkedFunctor = toolbox.FindFunctor(Scan_functorStr.c_str());
    ScanPipeName = ScanLinkedFunctor.PipeServiceName();
    ScanPipeService = toolbox.FindPipeService(ScanPipeName.c_str());    
    ExplorerScanFunctor = ScanPipeService.CreateFunctor("ScanExplorer",   "ExplorerScanFunctor@IceExplorer" );
    }
    
    // IMAGE FUNCTOR
    if ( Imag_IsActivated ) {
    ImagLinkedFunctor = toolbox.FindFunctor(Imag_functorStr.c_str());
    ImagPipeName = ImagLinkedFunctor.PipeServiceName();
    ImagPipeService = toolbox.FindPipeService(ImagPipeName.c_str());    
    ExplorerImagFunctor = ImagPipeService.CreateFunctor("ImageExplorer",  "ExplorerImageFunctor@IceExplorer" );
    }
    
    const bool StopChain            = prot["EXPLORER.StopChain"].ToBool();
    const bool CheckAndDisplay      = prot["EXPLORER.CheckAndDisplay"].ToBool();
    const double VisuScaleFactor    = prot["EXPLORER.VisuScaleFactor"].ToDouble();
    const bool DumpCurrentLoopIndex = prot["EXPLORER.DumpCurrentLoopIndex"].ToBool();
    const bool SingleThreading      = prot["EXPLORER.SingleThreading"].ToBool();
    const bool DumpIceAs            = prot["EXPLORER.DumpIceAs"].ToBool();
    const bool DumpIceObj           = prot["EXPLORER.DumpIceObj"].ToBool();
    const bool DumpMiniHeader       = prot["EXPLORER.DumpMiniHeader"].ToBool();
    const bool DumpImageControl     = prot["EXPLORER.DumpImageControl"].ToBool();
    const bool DumpParcENV          = prot["EXPLORER.DumpParcENV"].ToBool();
    const bool DumpParcPDS          = prot["EXPLORER.DumpParcPDS"].ToBool();
    std::string RemoveFun1          = prot["EXPLORER.RemoveFunctor_1"].ToString();
    std::string RemoveFun2          = prot["EXPLORER.RemoveFunctor_2"].ToString();
    const bool EM_FloatOuput        = prot["EXPLORER.ExtractMAG_FloatOuput"].ToBool();
    const bool IF_FloatOuput        = prot["EXPLORER.Imafinish_FloatOuput"].ToBool();
    const double ccol               = prot["EXPLORER.ccol"].ToDouble();
    const double ccha               = prot["EXPLORER.ccha"].ToDouble();
    const double clin               = prot["EXPLORER.clin"].ToDouble();
    const double cset               = prot["EXPLORER.cset"].ToDouble();
    const double ceco               = prot["EXPLORER.ceco"].ToDouble();
    const double cphs               = prot["EXPLORER.cphs"].ToDouble();
    const double cidd               = prot["EXPLORER.cidd"].ToDouble();
    const double cide               = prot["EXPLORER.cide"].ToDouble();
    const double cave               = prot["EXPLORER.cave"].ToDouble();
    const double cpar               = prot["EXPLORER.cpar"].ToDouble();
    const double cslc               = prot["EXPLORER.cslc"].ToDouble();
    const double cida               = prot["EXPLORER.cida"].ToDouble();
    const double cidb               = prot["EXPLORER.cidb"].ToDouble();
    const double cidc               = prot["EXPLORER.cidc"].ToDouble();

    int nMultiThreads = prot.At("ICE.CONFIG.THREADING.THREADS_PER_PARALLEL_PIPESERVICE").ToLong();
    int nSingleThreads = prot.At("ICE.CONFIG.THREADING.THREADS_PER_SINGLETHREADED_PIPESERVICE").ToLong();
    
    if ( Scan_IsActivated ) {
        ExplorerScanFunctor.SetProperty( "StopChain", StopChain );
        ExplorerScanFunctor.SetProperty( "CheckAndDisplay", CheckAndDisplay );
        ExplorerScanFunctor.SetProperty( "VisuScaleFactor", VisuScaleFactor );
        ExplorerScanFunctor.SetProperty( "DumpCurrentLoopIndex", DumpCurrentLoopIndex );
        ExplorerScanFunctor.SetProperty( "DumpIceAs", DumpIceAs );
        ExplorerScanFunctor.SetProperty( "DumpIceObj", DumpIceObj );
        ExplorerScanFunctor.SetProperty( "DumpMiniHeader", DumpMiniHeader );
        ExplorerScanFunctor.SetProperty( "DumpImageControl", DumpImageControl );
        ExplorerScanFunctor.SetProperty( "DumpParcENV", DumpParcENV );
        ExplorerScanFunctor.SetProperty( "DumpParcPDS", DumpParcPDS );
        ExplorerScanFunctor.SetProperty( "ccol", ccol );
        ExplorerScanFunctor.SetProperty( "ccha", ccha );
        ExplorerScanFunctor.SetProperty( "clin", clin );
        ExplorerScanFunctor.SetProperty( "cset", cset );
        ExplorerScanFunctor.SetProperty( "ceco", ceco );
        ExplorerScanFunctor.SetProperty( "cphs", cphs );
        ExplorerScanFunctor.SetProperty( "cidd", cidd );
        ExplorerScanFunctor.SetProperty( "cide", cide );
        ExplorerScanFunctor.SetProperty( "cave", cave );
        ExplorerScanFunctor.SetProperty( "cpar", cpar );
        ExplorerScanFunctor.SetProperty( "cslc", cslc );
        ExplorerScanFunctor.SetProperty( "cida", cida );
        ExplorerScanFunctor.SetProperty( "cidb", cidb );
        ExplorerScanFunctor.SetProperty( "cidc", cidc );
        
        ICE_OUT("\nSingle Threads: " << nSingleThreads <<
        ", Multi Threads:  " << nMultiThreads << ", Originally set to "<< ScanPipeService.PoolThreads() <<"\n");

        if ( SingleThreading == true )
        {
            ScanPipeService.PoolThreads(nSingleThreads);
            ICE_OUT("Pipe Service is set to single threaded in \"ExplorerScanFunctor\".\n");        
        }
        
    }
    
    if ( Imag_IsActivated ) {
        ExplorerImagFunctor.SetProperty( "StopChain", StopChain );
        ExplorerImagFunctor.SetProperty( "CheckAndDisplay", CheckAndDisplay );
        ExplorerImagFunctor.SetProperty( "VisuScaleFactor", VisuScaleFactor );
        ExplorerImagFunctor.SetProperty( "DumpCurrentLoopIndex", DumpCurrentLoopIndex );
        ExplorerImagFunctor.SetProperty( "DumpIceAs", DumpIceAs );
        ExplorerImagFunctor.SetProperty( "DumpIceObj", DumpIceObj );
        ExplorerImagFunctor.SetProperty( "DumpMiniHeader", DumpMiniHeader );
        ExplorerImagFunctor.SetProperty( "DumpImageControl", DumpImageControl );
        ExplorerImagFunctor.SetProperty( "DumpParcENV", DumpParcENV );
        ExplorerImagFunctor.SetProperty( "DumpParcPDS", DumpParcPDS );
        ExplorerImagFunctor.SetProperty( "ccol", ccol );
        ExplorerImagFunctor.SetProperty( "ccha", ccha );
        ExplorerImagFunctor.SetProperty( "clin", clin );
        ExplorerImagFunctor.SetProperty( "cset", cset );
        ExplorerImagFunctor.SetProperty( "ceco", ceco );
        ExplorerImagFunctor.SetProperty( "cphs", cphs );
        ExplorerImagFunctor.SetProperty( "cidd", cidd );
        ExplorerImagFunctor.SetProperty( "cide", cide );
        ExplorerImagFunctor.SetProperty( "cave", cave );
        ExplorerImagFunctor.SetProperty( "cpar", cpar );
        ExplorerImagFunctor.SetProperty( "cslc", cslc );
        ExplorerImagFunctor.SetProperty( "cida", cida );
        ExplorerImagFunctor.SetProperty( "cidb", cidb );
        ExplorerImagFunctor.SetProperty( "cidc", cidc );    

        ICE_OUT("\nSingle Threads: " << nSingleThreads <<
        ", Multi Threads:  " << nMultiThreads << ", Originally set to "<< ImagPipeService.PoolThreads() <<"\n");

        if ( SingleThreading == true )
        {
            ImagPipeService.PoolThreads(nSingleThreads);
            ICE_OUT("Pipe Service is set to single threaded in \"ExplorerImagFunctor\".\n");        
        }        
    }

    // Remove functor
    if ( RemoveFun1.size() != 0 )
    {
        Res = toolbox.Remove(RemoveFun1.c_str(), true);
        if ( failed(Res) ) {
            ICE_ERR("Error in ExplorerConfigurator: failed to remove functor \"" << RemoveFun1 << "\"" );
            return I_FAIL; }
    }

    // Remove functor
    if ( RemoveFun2.size() != 0 )
    {
        Res = toolbox.Remove(RemoveFun2.c_str(), true);
        if ( failed(Res) ) {
            ICE_ERR("Error in ExplorerConfigurator: failed to remove functor \"" << RemoveFun2 << "\"" );
            return I_FAIL; }   
    }

    // Insert ExplorerScanFunctor functor either before or after a given functor in chain.
    if ( Scan_IsActivated ) {
        if ( Scan_IsSuccessor )
        {
            if( failed( toolbox.InsertAfter("ScanExplorer", "ComputeScan", "ScanReady", ScanLinkedFunctor.Name(), "ScanReady") ) )
            {
                ICE_ERR("Failed during call of InsertAfter(... "<<ScanLinkedFunctor.Name()<<" ...), aborting...\n");
                return I_FAIL;
            }
        }
        else
        {
            if( failed( toolbox.InsertBefore("ScanExplorer", "ComputeScan", "ScanReady", ScanLinkedFunctor.Name(), "ComputeScan") ) )
            {
                ICE_ERR("Failed during call of InsertBefore(... "<<ScanLinkedFunctor.Name()<<" ...), aborting...\n");
                return I_FAIL;
            }
        }
    }
    
    // Insert ExplorerImagFunctor functor either before or after a given functor in chain.
    if ( Imag_IsActivated ) {
        if ( Imag_IsSuccessor )
        {
            if( failed( toolbox.InsertAfter("ImageExplorer", "ComputeImage", "ImageReady", ImagLinkedFunctor.Name(), (Imag_PreMethod.empty()?"ImageReady":Imag_PreMethod.c_str()) ) ) )
            {
                ICE_ERR("Failed during call of InsertAfter(... "<<ImagLinkedFunctor.Name()<<" ...), aborting...\n");
                return I_FAIL;
            }
        }
        else
        {
            if( failed( toolbox.InsertBefore("ImageExplorer", "ComputeImage", "ImageReady", ImagLinkedFunctor.Name(), (Imag_SucEvent.empty()?"ComputeImage":Imag_SucEvent.c_str()) ) ) )
            {
                ICE_ERR("Failed during call of InsertBefore( ... "<<ImagLinkedFunctor.Name()<<" ...), aborting...\n");
                return I_FAIL;
            }
        }    
    }
    
    if ( EM_FloatOuput )
    {    
        MrFunctor fExtractMAG = toolbox.FindFunctor("extractMAG", false);
        fExtractMAG.SetProperty("FunctorMode", 32);
    }
    
    if ( IF_FloatOuput )
    {
        MrFunctor fFinish = toolbox.FindFunctor("imafinish", false);
        fFinish.SetProperty("FloatExtract", true);
    }
    
    return I_OK;
}

