//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: IceExplorer
//-       Version: IceExplorer
//-        Author: Franck MAUCONDUIT (franck.mauconduit@siemens.com)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\XFL_postproc\ExplorerScanFunctor.cpp
//-          Date: Fri Nov 18 18:35:18 2016
//-      Language: C++
//-
//-   Description: <ICE functor to explore and dump data in a pipeline.>
//-
//-----------------------------------------------------------------------------------

#include "MrServers/MrVista/Ice/IceIdeaFunctors/IceExplorer/ExplorerScanFunctor.h"
#include "MrServers/MrVista/include/Ice/IceBasic/IceAs.h"
#include "MrServers/MrVista/include/Ice/IceUtils/IceUtils.h"

#include "MrServers/MrVista/Ice/IrcNlsMessageId.h"
#include "MrServers/MrVista/Ice/IrcNlsMessageId.h"
#include <fstream>

//Default Constructor
ExplorerScanFunctor::ExplorerScanFunctor()
    :m_bStopChain( false )
    ,m_bCheckAndDisplay( false )
    ,m_dVisuScaleFactor( 1.0 )
    ,m_bDumpCurrentLoopIndex( false )
    ,m_bDumpIceAs( false )
    ,m_bDumpIceObj( false )
    ,m_bDumpMiniHeader( false )
    ,m_bDumpImageControl( false )
    ,m_bDumpParcENV( false )
    ,m_bDumpParcPDS( false )
    ,m_dccol( -1 )
    ,m_dccha( -1 )
    ,m_dclin( -1 )
    ,m_dcset( -1 )
    ,m_dceco( -1 )
    ,m_dcphs( -1 )
    ,m_dcidd( -1 )
    ,m_dcide( -1 )
    ,m_dcave( -1 )
    ,m_dcpar( -1 )
    ,m_dcslc( -1 )
    ,m_dcida( -1 )
    ,m_dcidb( -1 )
    ,m_dcidc( -1 )
{}

//Default Destructor
ExplorerScanFunctor::~ExplorerScanFunctor()
{}

IResult ExplorerScanFunctor::endOfJob(IResult irReason)
{
    ICE_SET_FN("ExplorerScanFunctor::endOfJob(irReason)")

    // irReason :   I_ACQ_END: acquisition finished
    //              I_USER: acquisition cancelled by user
    //              I_FAIL: error

    if (irReason == I_USER)
    {
        ICE_OUT("endOfJob called due to user stop!");
    }

    return I_OK;
}

IResult ExplorerScanFunctor::EndInit(IParcEnvironment* env)
{
	ICE_SET_FN("ExplorerScanFunctor::EndInit(env)")
    
	// Always call base class:
	IResult res = IceScanFunctors::EndInit(env);
	if (failed(res))
		ICE_RET_ERR("Error calling IceScanFunctors::EndInit(), aborting...", res)

	// Get the PDS service for retrieving B0 images that have been saved through study
	m_pPDS = Parc::cast<IPDS>(m_env->Item("PDS"));
	if (!m_pPDS)
		ICE_RET_ERR("Cannot retrieve ParcDataStore! Pointer is invalid.", I_FAIL);

	return I_OK;
}

IResult ExplorerScanFunctor::ComputeScan(IceAs& srcAs, MdhProxy& aMdh, ScanControl& ctrl)
{
	ICE_SET_FN("ExplorerScanFunctor::ComputeImage(srcAs, dataHeader, ctrl)")
    
    
	std::string DataInfo;
	std::stringstream StreamStr;
	std::stringstream StreamMdh;
    
    IceObj::Pointer visuObj( Parc::HeapObject<IceObj>::Create() );
	if( ! visuObj->create( srcAs ) )
        ICE_RET_ERR("Failed during sendObj->create(srcAs), aborting...", I_FAIL)
	IceAs visuAs = (*visuObj)();
    
    Ice.Mul(visuAs, srcAs, m_dVisuScaleFactor);
    
	if ( ! keepChosenData(aMdh) )
		return I_OK;

    if( m_bDumpCurrentLoopIndex )
    {
        ICE_OUT("\n	LOOP INFORMATION, current loop index is :"
            "\n	acq=" << aMdh.getCacq() << 
            "  eco=" << aMdh.getCeco() << //*
            // "  chID=" << aMdh.getChannelID() <<
            "  set=" << aMdh.getCset() <<
            "  ida=" << aMdh.getCida() <<
            "  idb=" << aMdh.getCidb() <<
            "  idc=" << aMdh.getCidc() <<
            "  idd=" << aMdh.getCidd() <<
            "  ide=" << aMdh.getCide() <<
            "  lin=" << aMdh.getClin() << //*
            "  none=" << aMdh.getCnone() << //*
            "  par=" << aMdh.getCpar() << //*
            "  phs=" << aMdh.getCphs() <<
            "  rep=" << aMdh.getCrep() <<
            "  seg=" << aMdh.getCseg() <<
            "  slc=" << aMdh.getCslc() << "\n");
	}
    
	if( m_bCheckAndDisplay )
	{
		StreamStr << "  " <<
			"  acq=" << aMdh.getCacq() << 
            "  eco=" << aMdh.getCeco() << //*
            // "  chID=" << aMdh.getChannelID() <<
            "  set=" << aMdh.getCset() <<
            "  ida=" << aMdh.getCida() <<
            "  idb=" << aMdh.getCidb() <<
            "  idc=" << aMdh.getCidc() <<
            "  idd=" << aMdh.getCidd() <<
            "  ide=" << aMdh.getCide() <<
            "  lin=" << aMdh.getClin() << //*
            "  none=" << aMdh.getCnone() << //*
            "  par=" << aMdh.getCpar() << //*
            "  phs=" << aMdh.getCphs() <<
            "  rep=" << aMdh.getCrep() <<
            "  seg=" << aMdh.getCseg() <<
            "  set=" << aMdh.getCset() <<
            "  slc=" << aMdh.getCslc() ;
            
        ICE_OUT("\nDATA INFO : " << StreamStr.str() << "\nDATA VALUES : ");		
		// Ice.Dump( visuAs );				
		DataInfo = StreamStr.str();
		IceUtils::CheckAndDisplay(visuAs, IceDisplayData::DISPLAY,   DataInfo.c_str()  );
	}

	if( m_bDumpIceAs )
	{
		_complex	minVal, maxVal;
		// Ice.CalcStatistics(srcAs,IsmMinMax);
		// Ice.GetStatisticsMaxVal(maxVal);
		// Ice.GetStatisticsMinVal(minVal);
		// ICE_OUT("\n         ---------------------------------------------"
                // "\n"<<srcAs<<
                // "\nMinimum Value:"<<pow(pow(minVal.x,2.0)+pow(minVal.y,2.0), 0.5)<<
                // "\nMaximum Value:"<<pow(pow(maxVal.x,2.0)+pow(maxVal.y,2.0), 0.5)<<"\n");
		// ICE_OUT("\n         ---------------------------------------------\n"
                // <<srcAs<<"Minimum Value:"<<minVal<<"\nMaximum Value:"<<maxVal<<"\n");
		//Ice.Dump(srcAs);//Dump values of object
	}
	
    if (m_bDumpIceObj )
    {
        ICE_OUT("srcAs.getObj()->getSizeOfIceObj()="<<srcAs.getObj()->getSizeOfIceObj()<<"\n");
        // ICE_OUT("srcAs.getSizeOfIceBloc()="<<srcAs.getSizeOfIceBloc()<<"\n");
        ICE_OUT("srcAs.getObj()->getSizeOfIceObj ()="<<srcAs.getObj()->getSizeOfIceObj ()<<"\n");
        // ICE_OUT("srcAs.getSizeOfIceObj ()="<<srcAs.getSizeOfIceObj ()<<"\n");
        ICE_OUT("srcAs.getObj()->getNoOfIceBlocs ()="<<srcAs.getObj()->getNoOfIceBlocs ()<<"\n");
        ICE_OUT("srcAs.getNoOfIceBlocs ()="<<srcAs.getNoOfIceBlocs ()<<"\n");
        ICE_OUT("srcAs.isDataCoherent()="<<srcAs.isDataCoherent()<<"\n");
        ICE_OUT("visuAs.isDataCoherent()="<<visuAs.isDataCoherent()<<"\n");
        ICE_OUT("\n         ---------------------------------------------\n"
                "Dump IceObject:\n" << srcAs.getObj()->dump() );                
    }
    
    if ( m_bDumpMiniHeader ) // m_bDumpMdh in case of Scan Functor
    {
        
        aMdh.dump(StreamMdh);
        ICE_OUT("\n         ---------------------------------------------\n"
                "Dump MDH:\n" << StreamMdh.str() );                
    }
    
    if ( m_bDumpImageControl )
    {
        ICE_OUT("\n         ---------------------------------------------\n"
                "ImageControl:\n" << ctrl );
    }
    
    if( m_bDumpParcENV )
	{
        ICE_OUT("\n         ------------------ ENV ----------------------\n");
        m_env->ListContent();
    }
    
    if ( m_bDumpParcPDS )
    {
        ICE_OUT("\n         ------------------ PDS ----------------------\n");
        ICE_OUT(m_pPDS->print());
    }
    
    if ( ! m_bStopChain )
    {
        //Pass on image to next functor
        IResult res = ScanReady(srcAs, aMdh, ctrl);
        if( failed(res) )
            ICE_RET_ERR("ERROR calling remaining functor chain, aborting...", res)
    }
    
	return I_OK;
}


bool ExplorerScanFunctor::keepChosenData( MdhProxy& aMdh )
{
	// if ( ( m_dccol != -1 ) & ( ctrl.m_lc.m_ccol != m_dccol) )
			// return false;

	// if ( ( m_dccha != -1 ) & ( ctrl.m_lc.m_ccha != m_dccha) )
			// return false;

	if ( ( m_dclin != -1 ) & ( aMdh.getClin() != m_dclin) )
			return false;

	if ( ( m_dcset != -1 ) & ( aMdh.getCset() != m_dcset) )
			return false;

	if ( ( m_dceco != -1 ) & ( aMdh.getCeco() != m_dceco) )
			return false;

	if ( ( m_dcphs != -1 ) & ( aMdh.getCphs() != m_dcphs) )
			return false;

	if ( ( m_dcidd != -1 ) & ( aMdh.getCidd() != m_dcidd) )
			return false;

	if ( ( m_dcide != -1 ) & ( aMdh.getCide() != m_dcide) )
			return false;

	// if ( ( m_dcave != -1 ) & ( aMdh.getCave() != m_dcave) )
			// return false;

	if ( ( m_dcpar != -1 ) & ( aMdh.getCpar() != m_dcpar) )
			return false;

	if ( ( m_dcslc != -1 ) & ( aMdh.getCslc() != m_dcslc) )
			return false;

	if ( ( m_dcida != -1 ) & ( aMdh.getCida() != m_dcida) )
			return false;

	if ( ( m_dcidb != -1 ) & ( aMdh.getCidb() != m_dcidb) )
			return false;

	if ( ( m_dcidc != -1 ) & ( aMdh.getCidc() != m_dcidc) )
			return false;

	return true;
}

