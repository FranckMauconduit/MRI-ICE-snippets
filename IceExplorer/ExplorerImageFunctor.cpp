//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: IceExplorer
//-       Version: IceExplorer
//-        Author: Franck MAUCONDUIT (franck.mauconduit@siemens.com)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\XFL_postproc\ExplorerImageFunctor.cpp
//-          Date: Fri Nov 18 18:35:18 2016
//-      Language: C++
//-
//-   Description: <ICE functor to explore and dump data in a pipeline.>
//-
//-----------------------------------------------------------------------------------

#include "MrServers/MrVista/Ice/IceIdeaFunctors/IceExplorer/ExplorerImageFunctor.h"

#include "MrServers/MrVista/include/Ice/IceUtils/IceUtils.h"


//Default Constructor
ExplorerImageFunctor::ExplorerImageFunctor()
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
ExplorerImageFunctor::~ExplorerImageFunctor()
{}

IResult ExplorerImageFunctor::endOfJob(IResult irReason)
{
    ICE_SET_FN("ExplorerImageFunctor::endOfJob(irReason)")

    // irReason :   I_ACQ_END: acquisition finished
    //              I_USER: acquisition cancelled by user
    //              I_FAIL: error

    if (irReason == I_USER)
    {
        ICE_OUT("endOfJob called due to user stop!");
    }

    return I_OK;
}

IResult ExplorerImageFunctor::EndInit(IParcEnvironment* env)
{
	ICE_SET_FN("ExplorerImageFunctor::EndInit(env)")
    
	// Always call base class:
	IResult res = IceImageReconFunctors::EndInit(env);
	if (failed(res))
		ICE_RET_ERR("Error calling IceImageReconFunctors::EndInit(), aborting...", res)

	// Get the PDS service for retrieving B0 images that have been saved through study
	m_pPDS = Parc::cast<IPDS>(m_env->Item("PDS"));
	if (!m_pPDS)
		ICE_RET_ERR("Cannot retrieve ParcDataStore! Pointer is invalid.", I_FAIL);
    
	return I_OK;
}

IResult ExplorerImageFunctor::ComputeImage(IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& ctrl)
{
	ICE_SET_FN("ExplorerImageFunctor::ComputeImage(srcAs, dataHeader, ctrl)")
    
	std::string DataInfo;
	std::stringstream StreamStr;
	
    IceObj::Pointer visuObj( Parc::HeapObject<IceObj>::Create() );
	if( ! visuObj->create( srcAs ) )
        ICE_RET_ERR("Failed during sendObj->create(srcAs), aborting...", I_FAIL)
	IceAs visuAs = (*visuObj)();
    
    Ice.Mul(visuAs, srcAs, m_dVisuScaleFactor);
    
	if ( ! keepChosenData(ctrl) )
    {
        ICE_ERR("keepChosenData() is filtering out some incoming data.");
        return I_OK;
    }

    if( m_bDumpCurrentLoopIndex )
    {
        ICE_OUT("\n	LOOP INFORMATION, current loop index is :"
            "\n	slc=" << ctrl.m_lc.m_cslc << 
            "  set=" << ctrl.m_lc.m_cset <<
            "  eco=" << ctrl.m_lc.m_ceco << //*
            "  par=" << ctrl.m_lc.m_cpar << //*
            "  seg=" << ctrl.m_lc.m_cseg <<
            "  phs=" << ctrl.m_lc.m_cphs <<
            "  rep=" << ctrl.m_lc.m_crep <<
            "  ida=" << ctrl.m_lc.m_cida <<
            "  idb=" << ctrl.m_lc.m_cidb <<
            "  idc=" << ctrl.m_lc.m_cidc <<
            "  idd=" << ctrl.m_lc.m_cidd <<
            "  ide=" << ctrl.m_lc.m_cide << "\n");
	}
    
	if( m_bCheckAndDisplay )
	{
		StreamStr << " | Data: slc " << ctrl.m_lc.m_cslc << 
				" set " << ctrl.m_lc.m_cset <<
				" eco " << ctrl.m_lc.m_ceco << 
				" par " << ctrl.m_lc.m_cpar << 
				" seg " << ctrl.m_lc.m_cseg <<
				" phs " << ctrl.m_lc.m_cphs <<
				" rep " << ctrl.m_lc.m_crep <<
				" ida " << ctrl.m_lc.m_cida <<
				" idb " << ctrl.m_lc.m_cidb <<
				" idc " << ctrl.m_lc.m_cidc <<
				" idd " << ctrl.m_lc.m_cidd <<
				" ide " << ctrl.m_lc.m_cide ;
				
		DataInfo = StreamStr.str();
		IceUtils::CheckAndDisplay(visuAs, IceDisplayData::DISPLAY,   DataInfo.c_str()  );
	}

	if( m_bDumpIceAs )
	{
		float	minVal, maxVal;
		Ice.CalcStatistics(srcAs,IsmMinMax);
		Ice.GetStatisticsMaxVal(maxVal);
		Ice.GetStatisticsMinVal(minVal);
		ICE_OUT("\n         ---------------------------------------------\n"
                <<srcAs<<"Minimum Value:"<<minVal<<"\nMaximum Value:"<<maxVal<<"\n"<<"m_IceExtractMode:"<<ctrl.m_IceExtractMode<<"\n");
		//Ice.Dump(srcAs);//Dump values of object
	}
    
    if (m_bDumpIceObj )
    {
        ICE_OUT("\n         ---------------------------------------------\n"
                "Dump IceObject:\n" << srcAs.getObj()->dump() );                
    }

    if (m_bDumpIceObj ) // Should create a m_bDumpIceSoda
    {
		
		// Retrieve correct slice position for B0Map
		IceSoda* sodaPtr = const_cast<IceSoda*>(srcAs.calcCurrentSodaPtr());
		
		size_t srcCOL = srcAs.getObj()->getLen(COL);
		size_t srcLIN = srcAs.getObj()->getLen(LIN);
		
        ICE_OUT("\n         ---------------------------------------------\n"
                "Dump IceSoda:\n" << *sodaPtr );  		
		ICE_OUT("\n   SODA INFORMATION :\n"
            "\n   ColumnVec         : " << sodaPtr->getColumnVec() <<
            "\n   PhaseEncVec       : " << sodaPtr->getPhaseEncVec() <<
            "\n   PixelSpacingCol   : " << sodaPtr->getPixelSpacingCol() <<
            "\n   PixelColNumber    : " << srcCOL <<
            "\n   PixelSpacingRow   : " << sodaPtr->getPixelSpacingRow() <<
            "\n   PixelRowNumber    : " << srcLIN <<
            "\n   RowVec            : " << sodaPtr->getRowVec() <<
            "\n   SliceNormVec      : " << sodaPtr->getSliceNormVec() <<
            "\n   SlicePosVec       : " << sodaPtr->getSlicePosVec() <<
            "\n   SlicePosVecSBCS   : " << sodaPtr->getSlicePosVecSBCS() <<
            "\n   getTablePositionX : " << sodaPtr->getTablePositionX() <<
            "\n   getTablePositionY : " << sodaPtr->getTablePositionY() <<
            "\n   getTablePositionZ : " << sodaPtr->getTablePositionZ() <<
            "\n   getThickness      : " << sodaPtr->getThickness() << 
            "\n"
            );
				
    }
    
    if ( m_bDumpMiniHeader ) // m_bDumpMiniHeader
    {
        ICE_OUT("\n         ---------------------------------------------\n"
                "Dump MiniHeader:\n" << dataHeader->SaveToStreamCompact() );
        //dataHeader->SaveToFile("/opt/MiniHeader.txt"); // Dump dataHeader into a file
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
        IResult res = ImageReady(srcAs, dataHeader, ctrl);
        
        if( failed(res) )
            ICE_RET_ERR("ERROR calling remaining functor chain, aborting...", res)
    }
    
	return I_OK;
}


bool ExplorerImageFunctor::keepChosenData( ImageControl& ctrl )
{
	if ( ( m_dccol != -1 ) & ( ctrl.m_lc.m_ccol != m_dccol) )
			return false;

	if ( ( m_dccha != -1 ) & ( ctrl.m_lc.m_ccha != m_dccha) )
			return false;

	if ( ( m_dclin != -1 ) & ( ctrl.m_lc.m_clin != m_dclin) )
			return false;

	if ( ( m_dcset != -1 ) & ( ctrl.m_lc.m_cset != m_dcset) )
			return false;

	if ( ( m_dceco != -1 ) & ( ctrl.m_lc.m_ceco != m_dceco) )
			return false;

	if ( ( m_dcphs != -1 ) & ( ctrl.m_lc.m_cphs != m_dcphs) )
			return false;

	if ( ( m_dcidd != -1 ) & ( ctrl.m_lc.m_cidd != m_dcidd) )
			return false;

	if ( ( m_dcide != -1 ) & ( ctrl.m_lc.m_cide != m_dcide) )
			return false;

	if ( ( m_dcave != -1 ) & ( ctrl.m_lc.m_cave != m_dcave) )
			return false;

	if ( ( m_dcpar != -1 ) & ( ctrl.m_lc.m_cpar != m_dcpar) )
			return false;

	if ( ( m_dcslc != -1 ) & ( ctrl.m_lc.m_cslc != m_dcslc) )
			return false;

	if ( ( m_dcida != -1 ) & ( ctrl.m_lc.m_cida != m_dcida) )
			return false;

	if ( ( m_dcidb != -1 ) & ( ctrl.m_lc.m_cidb != m_dcidb) )
			return false;

	if ( ( m_dcidc != -1 ) & ( ctrl.m_lc.m_cidc != m_dcidc) )
			return false;

	return true;
}

