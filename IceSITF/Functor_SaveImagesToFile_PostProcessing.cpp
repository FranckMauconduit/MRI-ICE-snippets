//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: Save Images To File
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\IceSITF\Functor_SaveImagesToFile_PostProcessing.cpp
//-          Date: Thu Jul 18 13:58:25 2019
//-      Language: C++
//-
//-   Description: Insert a functor into pipeline to save all incoming images into a file
//-
//-----------------------------------------------------------------------------------

// SaveImagesToFileFunctor class
#include "Functor_SaveImagesToFile_PostProcessing.h"

#include <sys/stat.h>   // to use stat

#ifndef BUILD_PLATFORM_LINUX
  #include <direct.h>     // to use mkdir
#endif

//-----------------------------------------------------------------------------
// SaveImagesToFileFunctor()
// Description: constructor method of the "SaveImagesToFileFunctor" class.
//-----------------------------------------------------------------------------

using namespace ICE_NAMESPACE;

SaveImagesToFileFunctor::SaveImagesToFileFunctor()
    // :m_tSendDiskDir( "C:/MedCom/RECO_B0B1/" )
  #ifdef _VX_VERSION_VB
    :m_tSendDiskDir( "/opt/medcom/MriCustomer/seq/SAVED_IMAGES/" )
  #else
    :m_tSendDiskDir( "/opt/medcom/MriCustomer/ice/SAVED_IMAGES/" )
  #endif
    ,m_tSendDiskFile        ( "IMAGES_" )
    ,m_bDateAndTime         ( true )
    ,m_tLinkedFunctor       ( "extractMAG" )
    ,m_tSinkName            ( "ComputeImage" )
    ,m_tEventName           ( "ImageReady" )
    ,m_bSuccessor           ( true )
    ,m_bextractMAG_mode32   ( false )
    ,m_iSliceMin            ( -1 )
    ,m_iSliceMax            ( -1 )
    ,m_bPassOnData          ( true )
    ,m_iNumTXChannels       ( 1 )
{
	// register callback for FirstCall in Functor base class
	// (otherwise e.g. method FirtsCall(...) won't be called):
	addCB(IFirstCall);
}

SaveImagesToFileFunctor::SaveImagesToFileFunctor( const char * c1, const char * c2, bool b3, const char * c4, bool b5, bool b6, int i7, int i8, bool b9 )
    :m_tSendDiskDir         ( c1 )
    ,m_tSendDiskFile        ( c2 )
    ,m_bDateAndTime         ( b3 )
    ,m_tLinkedFunctor       ( c4 )
    ,m_bSuccessor           ( b5 )
    ,m_bextractMAG_mode32   ( b6 )
    ,m_iSliceMin            ( i7 ) // optional, default -1
    ,m_iSliceMax            ( i8 ) // optional, default -1
    ,m_bPassOnData          ( b9 ) // optional, default true
{
	// register callback for FirstCall in Functor base class
	// (otherwise e.g. method FirtsCall(...) won't be called):
	addCB(IFirstCall);
}

//-----------------------------------------------------------------------------
// ~SaveImagesToFileFunctor()
// Description: destructor method of the "SaveImagesToFileFunctor" class.
//-----------------------------------------------------------------------------

SaveImagesToFileFunctor::~SaveImagesToFileFunctor()
{
}


//-----------------------------------------------------------------------------
// EndInit(IParcEnvironment*)
// Description: EndInit method of the "SaveImagesToFileFunctor" class.
//-----------------------------------------------------------------------------

IResult SaveImagesToFileFunctor::EndInit(IParcEnvironment* env)
{
	ICE_SET_FN("SaveImagesToFileFunctor::EndInit(env)")
    
    // Definition for Extended Functor class
    // Either changing values in here or call Base class with definition
    // bDebugModeInX = true;
    // bIsNumberInSeriesActicatedInX = true;

	
	// Always call base class:
	IResult res = IceImageReconFunctors::EndInit(env);
	if (failed(res))
		ICE_RET_ERR("Error calling IceImageReconFunctors::EndInit(), aborting...", res)

	// Get the PDS service for retrieving B0 images that have been saved through study
	// m_pPDS = Parc::cast<IPDS>(m_env->Item("PDS"));
	// if (!m_pPDS)
		// ICE_RET_ERR("Cannot retrieve ParcDataStore! Pointer is invalid.", I_FAIL);

	m_bIsSaved = false;
	m_iImageCounter = 0;
    
    createFolderIfNeeded( m_tSendDiskDir );
    
  #ifdef BUILD_PLATFORM_LINUX
    if ( m_iMeasUID > 2999999 ) // Assuming it is a retro recon
    {
        std::string RecoIP;
      #ifdef _VX_VERSION_VE
        RecoIP.assign("192.168.2.2");
      #else
        RecoIP.assign("192.168.2.3");
      #endif
        
        std::stringstream outputfile;
        outputfile << "temp_measid_" << m_iFileID << ".txt";
        
        std::string strEnvVar("ice_MeasUID");
        std::string sCmd("/opt/med/bin/raidtool");
        std::ostringstream ostr;
        ostr << sCmd << " -d -a "<<RecoIP<<" -p 8010 | ";   // List all data in RAID
        ostr << "tr -s ' ' | ";                             // Replace multiple blanks by one
        ostr << "sed 's/^ //g' | ";                         // Remove leading blank
        ostr << "cut -d' ' -f1-2 | ";                       // Retrieve first and second column (FileID MeasID)
        ostr << "grep "<< m_iFileID <<" | ";                // Get line with FileID of the ongoing reconstruction dataset
        ostr << "cut -d' ' -f2";                            // Retrieve corresponding MeasID
        ostr << " > "<< outputfile.str();
        
        ICE_OUT("Ready to execute the following command:\n"<<ostr.str());
        int status = system(ostr.str().c_str());
        if ( status != 0 )
        {
            ICE_ERR( "Calling command failed : " << ostr.str());
        }
        else
        {
            std::ifstream ifmeasid( outputfile.str().c_str() );
            std::stringstream ss;
            ss << ifmeasid.rdbuf();
            ss >> m_iMeasUID;
            ICE_OUT( "RetroRecon, MeadUID retrieved from RAID : MeasUID=" << m_iMeasUID);
            
            // For safety reason, hard coded filename
            // Nevertheless, that should be the same filename as above
            system( "rm -f temp_measid_*" );
            
        }
    }
  #endif

	return I_OK;
}


//-----------------------------------------------------------------------------
// FirstCall(IceAs&, MiniHeader&, ImageControl&)
// Description: FirstCall method
//-----------------------------------------------------------------------------

IResult SaveImagesToFileFunctor::FirstCall(IceAs& srcAs, MiniHeader::Pointer& , ImageControl& ctrl)
{
	ICE_SET_FN("SaveImagesToFileFunctor::FirstCall(...)")

	// Initialize local variables
	
	// Dump information
	// ICE_OUT("\n	FIRSTCALL"
			// "\n	Ech number = " << ctrl.m_lc.m_ceco <<
			// "\n	Par number = " << ctrl.m_lc.m_cpar << 
			// "\n	Slc number = " << ctrl.m_lc.m_cslc << 
			// "\n	Set number = " << ctrl.m_lc.m_cset << "\n" );
    
    m_bIs2D = (ctrl.m_imaDimBoundaries.m_cslc>ctrl.m_imaDimBoundaries.m_cpar);
    m_sAnatSliceNumber.assign( (m_bIs2D?"AnatomicalSliceNo":"AnatomicalPartitionNo") );
    
    m_iImaColLen = (int)srcAs.getObj()->getLen(COL);
    m_iImaChaLen = (int)srcAs.getObj()->getLen(CHA); 
    m_iImaLinLen = (int)srcAs.getObj()->getLen(LIN); 
    m_iImaSlcLen = (m_bIs2D?ctrl.m_imaDimBoundaries.m_cslc:ctrl.m_imaDimBoundaries.m_cpar);
	m_iImaSetLen = ctrl.m_imaDimBoundaries.m_cset;
	m_iImaEcoLen = ctrl.m_imaDimBoundaries.m_ceco;
	m_iImaRepLen = ctrl.m_imaDimBoundaries.m_crep;
	m_iImaIdaLen = ctrl.m_imaDimBoundaries.m_cida;
    
    if ( m_iSliceMax!=-1 && m_iSliceMin!=-1  && m_iSliceMin<=m_iSliceMax )
    {
        m_iImaSlcLenKeep = m_iSliceMax-m_iSliceMin+1;
        m_iImaSlcLenKeep = (m_iImaSlcLenKeep>m_iImaSlcLen?m_iImaSlcLen:m_iImaSlcLenKeep);
    }
    else
    {m_iImaSlcLenKeep = m_iImaSlcLen;}
    
    m_iTotalSlc  = m_iImaSlcLenKeep*m_iImaSetLen*m_iImaEcoLen*m_iImaRepLen*m_iImaIdaLen;
    
    // m_iImaChaLen might be equal to 0, which is a problem
    if ( m_iImaChaLen==0 ) {m_iImaChaLen=1;}
    
    m_tFormat = srcAs.getIceObjFormat();

    ICE_OUT("\n  Functor parameters :"    <<
            "\n     m_tSendDiskDir    = " << m_tSendDiskDir     << 
            "\n     m_tSendDiskFile   = " << m_tSendDiskFile    << 
            "\n     m_bDateAndTime    = " << m_bDateAndTime     << 
            "\n     m_tLinkedFunctor  = " << m_tLinkedFunctor   << 
            "\n     m_bSuccessor      = " << m_bSuccessor       << 
            "\n  Image caracteristics :"             <<
            "\n     m_tFormat    = " << m_tFormat    << 
            "\n     m_iImaColLen = " << m_iImaColLen << 
            "\n     m_iImaChaLen = " << m_iImaChaLen <<
            "\n     m_iImaLinLen = " << m_iImaLinLen <<
            "\n     m_iImaSlcLen = " << m_iImaSlcLen <<
            "\n     m_iImaSlcLenKeep = " << m_iImaSlcLenKeep <<
            "\n     m_iImaSetLen = " << m_iImaSetLen <<
            "\n     m_iImaEcoLen = " << m_iImaEcoLen <<
            "\n     m_iImaRepLen = " << m_iImaRepLen <<
            "\n     m_iImaIdaLen = " << m_iImaIdaLen <<
            "\n     m_iTotalSlc  = " << m_iTotalSlc  << 
            "\n" );
    
	IceObj::Pointer ImageObj = Parc::HeapObject<IceObj>::Create();
	if( ! ImageObj->create( m_tFormat, COL, m_iImaColLen*m_iImaChaLen, LIN, m_iImaLinLen, CHA, m_iTotalSlc ) )
		{ICE_RET_ERR("Failed during ImageObj->create( ), aborting...", I_FAIL);}
	
	m_ImageAs = ( *ImageObj )();
	
	//IceUtils::CheckAndDisplay( b1As, IceDisplayData::DISPLAY, " - b1As" );
	
	// ICE_OUT("\n#############\n"<<b1As<<"\n#############\n"<<ctrl);
	
    
	return I_OK;
}



//-----------------------------------------------------------------------------
// endOfJob(IResult reason)
// Description: endOfJob method
//-----------------------------------------------------------------------------

IResult SaveImagesToFileFunctor::endOfJob(IResult reason)
{
	ICE_SET_FN("SaveImagesToFileFunctor::endOfJob(...)");

	std::stringstream	stream;

	if(reason == I_ACQ_END)
	{   stream << "ACQ_END";}
	else
	{
		if (reason==I_STOP)
			stream << "STOP";
		else if (reason==I_FAIL)
			stream << "FAIL";
		else if (reason==I_USER)
			stream << "USER";
		else
			stream << "status unknow";
        
        ICE_OUT( "EndofJob() called with reason " << stream.str() );
        
	}

	return I_OK;

}


//-----------------------------------------------------------------------------
// ComputeImage(IceAs&, MiniHeader&, ImageControl&)
// Description: SaveImagesToFileFunctor method
//-----------------------------------------------------------------------------

IResult SaveImagesToFileFunctor::ComputeImage(
		IceAs& srcAs, 
		MiniHeader::Pointer& dataHeader, 
		ImageControl& ctrl		)
{
	ICE_SET_FN("SaveImagesToFileFunctor::ComputeImage")

	// Initialize local variables
	IResult		Res;
	int		    k, AnatSlNo=0, iImaIndex;
    
    // retrieve information for scaling data
    // int AnatSl, ChronSl;
    // dataHeader->get("ChronSliceNo", ChronSl, true);
    
    // ICE_OUT("\n\n"
            // "   IceAs :" << srcAs <<
            // "    slc=" << (m_bIs2D?ctrl.m_lc.m_cslc:ctrl.m_lc.m_cpar) << ";\n"
            // "    set=" << ctrl.m_lc.m_cset << ";\n"
            // "    eco=" << ctrl.m_lc.m_ceco << ";\n"
            // "    rep=" << ctrl.m_lc.m_crep << ";\n"
            // "    ida=" << ctrl.m_lc.m_cida << ";\n"
            // "");
    
	// ------------------------------
	// Check all registered callbacks
	// ------------------------------
	// (SaveImagesToFileFunctor's FirstCall(...) is called here):
	Res = ExecuteCallbacks(srcAs, dataHeader, ctrl);
	if(failed(Res))
	{ICE_RET_ERR("ExecuteCallbacks failed, aborting...", Res)}

	// Return ERROR on wrong format type
	// IceFormat_t srcFormat = srcAs.getIceObjFormat();
	// if ( srcFormat != ICE_FORMAT_FL )
	// {ICE_RET_ERR("Format type of srcAs is "<<srcFormat<<", waiting ICE_FORMAT_FL, aborting ...", I_FAIL)}
	if ( m_tFormat != ICE_FORMAT_FL && m_tFormat != ICE_FORMAT_CXFL && m_tFormat != ICE_FORMAT_SH )
	{ICE_RET_ERR("Format type of srcAs is "<<m_tFormat<<", waiting ICE_FORMAT_FL or ICE_FORMAT_CXFL, aborting ...", I_FAIL)}

	// Return ERROR on wrong number of slc*set
	// if ( ctrl.m_lc.m_cset + ctrl.m_lc.m_cslc*ImaSetLen > ImaSlcLen * ImaSetLen - 1 )
	// {ICE_RET_ERR("Incoming data : slc="<<ctrl.m_lc.m_cslc<<", set="<<ctrl.m_lc.m_cset<<" superior to "<<ImaSlcLen * ImaSetLen - 1<<", aborting ...",I_FAIL);}
    
    // Using AnatSlNo for ordering purpose
    dataHeader->get(m_sAnatSliceNumber.c_str(), AnatSlNo, true);
	
    if (AnatSlNo >= (int)m_iImaSlcLen)
    {ICE_RET_ERR("Incoming data : AnatSlNo = "<<AnatSlNo<<" superior or equal to ImaSlcLen = "<<m_iImaSlcLen<<", aborting ...",I_FAIL);}
	// Return ERROR on wrong number of AnatSlNo*set
    // if (( ctrl.m_lc.m_cset + AnatSlNo*ImaSetLen > ImaSlcLen * ImaSetLen - 1 ))
	// {ICE_RET_ERR("Incoming data : slc(not used)="<<ctrl.m_lc.m_cslc<<", AnatSlNo(used)"<<AnatSlNo<<", set(used)="<<ctrl.m_lc.m_cset<<" superior to "<<ImaSlcLen * ImaSetLen - 1<<", aborting ...",I_FAIL);}
    
    if ( m_iImaSlcLenKeep != m_iImaSlcLen )
    {
        if ( AnatSlNo < m_iSliceMin || AnatSlNo > m_iSliceMax ) {return I_OK;}
        AnatSlNo -= m_iSliceMin;
    }
    
    if (failed( currentImageIndex( ctrl, AnatSlNo, iImaIndex ) ))
    {return I_FAIL;}
    
	m_ImageAs.modify(CHA, iImaIndex, 1,1 );

    float *pSrcfl, *pImafl;
    short *pSrcsh, *pImash;
    
    if ( m_tFormat == ICE_FORMAT_FL || m_tFormat == ICE_FORMAT_CXFL )
    {
        pSrcfl = (float*) srcAs.calcSplObjStartAddr();
        pImafl = (float*) m_ImageAs.calcSplObjStartAddr();
    }
    else
    {
        pSrcsh = (short*) srcAs.calcSplObjStartAddr();
        pImash = (short*) m_ImageAs.calcSplObjStartAddr();
    }
    
    int iLen = 1;
    
    switch ( m_tFormat )
    {
        case ICE_FORMAT_CXFL :
            iLen = 2;
            break;
        case ICE_FORMAT_FL :
        case ICE_FORMAT_SH :
        default :
            iLen = 1;
            break;
    }
    
    if ( m_tFormat == ICE_FORMAT_FL || m_tFormat == ICE_FORMAT_CXFL )
    {
        for ( k=0; k<(int)(m_iImaColLen*m_iImaLinLen*m_iImaChaLen*iLen); k++, pSrcfl++, pImafl++ )
        {*pImafl=*pSrcfl;}
    }
    else
    {
        for ( k=0; k<(int)(m_iImaColLen*m_iImaLinLen*m_iImaChaLen*iLen); k++, pSrcsh++, pImash++ )
        {*pImash=*pSrcsh;}
    }
    
	m_iImageCounter++;
	
	if ( m_iImageCounter == (int)(m_iTotalSlc) )
	{
		if (failed(SaveImages2File(dataHeader, ctrl)))
        {ICE_RET_ERR("Failed to save images to file",I_FAIL);}
		//IceUtils::CheckAndDisplay( m_ImageAs, IceDisplayData::DISPLAY, " - b1As" );
		m_bIsSaved = true;
	}
	
    if ( m_bPassOnData )
    {    
        // Call next Functor in chain
        Res = ImageReady( srcAs, dataHeader, ctrl );
        if( failed(Res) )
        {ICE_RET_ERR("ERROR calling remaining functor chain, aborting...", Res);}
    }
    
	return I_OK;
}

//-----------------------------------------------------------------------------
// END OF COMPUTE IMAGE
//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
// SaveImages2File(MiniHeader::Pointer&, ImageControl&, bool, std::string)
// Description: check if all B1 images are registered in PDS
//-----------------------------------------------------------------------------

IResult SaveImagesToFileFunctor::SaveImages2File( MiniHeader::Pointer& /*dataHeader*/, ImageControl& /*ctrl*/ )
{
    ICE_SET_FN("SaveImagesToFileFunctor::SaveImages2File")

    //ICE_OUT("\n\n  ########### RETRIEVING SAVED IMAGES !!! ########### \n\n");
    
    // Initialize local variables
    // IResult Res;
    //IceLoopCounters mlc = ctrl.m_lc;
    //int MAXSLC = ctrl.m_imaDimBoundaries.m_cslc;
    int lI;
    std::stringstream   streamDATE, streamIMA, streamIMAfile, streamHEADER, streamHEADERfile, streamMID, streamProt;
    std::stringstream   strWipLong, strWipDouble;
    std::ofstream ofs;
    
    m_ImageAs = (*m_ImageAs.getObj())();
	
    streamDATE << (m_bDateAndTime ? currentDateTime() : "" );
    streamMID << "MID" << m_iMeasUID << "_";
    
    // Write image to file
	streamIMAfile << streamMID.str() << m_tSendDiskFile << streamDATE.str() << ".dat";
    streamIMA << m_tSendDiskDir << streamIMAfile.str();

    ofs.open(streamIMA.str().c_str(), std::ios::binary);
    if ( ! ofs.is_open())
    {ICE_RET_ERR("Could not create file : "<<streamIMA.str(), I_FAIL);}
    
    //                 |-> being complex values (2 float)
    float VolumeSize = (m_tFormat==ICE_FORMAT_CXFL?2.f:1.f)*m_iImaColLen*m_iImaChaLen*m_iImaLinLen*m_iTotalSlc;
    ofs.write((char *)&VolumeSize,sizeof(float));
    if ( m_tFormat != ICE_FORMAT_SH )
    {
        float *pIma = (float*) m_ImageAs.calcSplObjStartAddr();
        for (long k=0; k<(long)(VolumeSize); k++, pIma++ )
        {ofs.write((char *)pIma,sizeof(float));}
    }
    else
    {
        short *pIma = (short*) m_ImageAs.calcSplObjStartAddr();
        for (long k=0; k<(long)(VolumeSize); k++, pIma++ )
        {ofs.write((char *)pIma,sizeof(short));}
    }
    ofs.close();
    
    // if( !Ice.WriteToFile(m_ImageAs,streamIMA.str().c_str()) )
	// {ICE_RET_ERR("Could not create file " << streamIMA.str() << ". Folder might be missing, aborting", I_FAIL)}
    
    // Write matlab script to open data file
	streamHEADERfile << streamMID.str() << m_tSendDiskFile << streamDATE.str() << ".m";
    streamHEADER << m_tSendDiskDir << streamHEADERfile.str();
    ofs.open(streamHEADER.str().c_str());
	
	if ( ! ofs.is_open())
	{ICE_RET_ERR("Could not open file : "<<streamHEADER.str(), I_FAIL);}
	
    for ( lI=0; lI<m_viWipInt.size(); lI++ )
    {strWipLong << m_viWipInt[lI] << " ";}
    
    for ( lI=0; lI<m_vdWipDouble.size(); lI++ )
    {strWipDouble << m_vdWipDouble[lI] << " ";}
    
    ofs << "%% MATLAB SCRIPT : " << streamHEADERfile.str() << " \n";
    ofs << "% OPEN .dat file created at the same location and containing all images\n";
    ofs << "% File to open : " << streamIMAfile.str() << " \n \n";
    ofs << "% WipMemBlock long   : "<< strWipLong.str() << "\n";
    ofs << "% WipMemBlock double : "<< strWipDouble.str() << "\n";
    ofs << "% ";    
    ofs << "% Data size \n";
    ofs << "    col=" << m_iImaColLen << ";\n";
    ofs << "    cha=" << m_iImaChaLen << ";\n";
    ofs << "    lin=" << m_iImaLinLen << ";\n";
    ofs << "    slc=" << m_iImaSlcLenKeep << ";\n";
    ofs << "    set=" << m_iImaSetLen << ";\n";
    ofs << "    eco=" << m_iImaEcoLen << ";\n";
    ofs << "    rep=" << m_iImaRepLen << ";\n";
    ofs << "    ida=" << m_iImaIdaLen << ";\n";
    ofs << "% Open data as a vector \n";
    ofs << "    fid=fopen('" << streamIMAfile.str() << "','r');\n";
    ofs << "    num_elmt=fread(fid,1,'float');\n";
    switch ( m_tFormat )
    {
        case ICE_FORMAT_SH :
            ofs << "    vect_fid=fread(fid,num_elmt,'int16');\n";
            break;
        case ICE_FORMAT_CXFL :
        case ICE_FORMAT_FL :
        default :
            ofs << "    vect_fid=fread(fid,num_elmt,'float');\n";
            break;
    }
    ofs << "    fclose(fid);\n\n";
    ofs << "% Reshape data into a matlab matrix containing images\n";
    switch ( m_tFormat )
    {
        case ICE_FORMAT_CXFL :
            ofs << "% first dimension contains real / imaginary values\n";
            ofs << "    vect_cplx = vect_fid(1:2:end) + 1i * vect_fid(2:2:end);\n";
            ofs << "    ICE_DATA_orig = reshape(vect_cplx,[col cha lin slc set eco rep ida]);\n";
            ofs << "    clear vect_cplx vect_fid;\n";
            break;
        case ICE_FORMAT_FL :
        case ICE_FORMAT_SH :
        default :
            ofs << "    ICE_DATA_orig = reshape(vect_fid,[col cha lin slc set eco rep ida]);\n";
            ofs << "    clear vect_fid;\n";
            break;
    }
    // addReorientedMatlabData() assumes orig volume comes with with the following order: lin,col,slc 
    ofs << "    ICE_DATA_orig = permute(ICE_DATA_orig, [3 1 4 2 5 6 7 8]);\n\n";
    ofs << addReorientedMatlabData("ICE_DATA_orig", "ICE_DATA");
    ofs << "\n";
    ofs << "% Number of transmit channels \n";
    ofs << "    NumOfTXChannels = " << m_iNumTXChannels << ";\n\n";
  #ifdef _VX_VERSION_VE
    ofs << "% Complex RF shim coefficients used during acquisition \n";
    ofs << "    RFShim = [ ... \n";
    for ( lI=0; lI<m_iNumTXChannels; lI++ )
    {
        ofs << "               "<<m_vdTxScaleReal[lI]<<"+1i*"<<m_vdTxScaleImag[lI];
        if ( lI!=m_iNumTXChannels-1 )
        {ofs << ", ...\n";}
        else
        {ofs << "]; \n";}
    }
  #endif
    ofs << "% RF pulse information :\n";
    for ( lI=0; lI<m_vsRFPulseName.size(); lI++ )
    {
    if ( m_vsRFPulseName[lI].empty() ) {break;}
    ofs << "    RFPulse("<<lI+1<<").name = '"<< m_vsRFPulseName[lI] <<"';\n";
    ofs << "    RFPulse("<<lI+1<<").voltage = "<< m_vdRFPulseVoltage[lI] <<";\n";
    }
    ofs << "% EOF";
    ofs.close();
    
    ICE_OUT("Images dumped into file : '" << streamIMA.str() << "'");
    
    // streamProt << m_tSendDiskDir << streamMID.str() << m_tSendDiskFile << streamDATE.str() << "_protocol.evp";
    SaveProtocolToFile( streamIMA.str() );
    
    return I_OK;
}

//-----------------------------------------------------------------------------
// SaveProtocolToFile()
// Description : create a file containing protocol
//-----------------------------------------------------------------------------

void SaveImagesToFileFunctor::SaveProtocolToFile( std::string destFile )
{
    ICE_SET_FN("SaveImagesToFileFunctor::SaveProtocolToFile")
    
    std::stringstream protFile;
    
    protFile << "/opt/medcom/log/PAS_SC_" << m_iMeasUID << ".log";    
    
    // Check for existence of protocol file
    struct stat st1;
    if (stat(protFile.str().c_str(), &st1) != 0) /* returns 0 on success */
    {
        ICE_ERR("Can't find file: '" << protFile.str().c_str() << "'");
        return;
    }
    
    // Open target file
    std::ofstream ofdest ( destFile.c_str(), std::ios::out );
    
    // Dump protocol file
    std::ifstream ifprot ( protFile.str().c_str() );
    ofdest << ifprot.rdbuf();
    ifprot.close();
    
    ofdest.close();
        
    ICE_OUT("Protocol information dumped in file : '" << destFile.c_str() << "'");
}

//-----------------------------------------------------------------------------
// currentImageIndex()
// Description: determine current image index based on actual loopcounter
//-----------------------------------------------------------------------------

IResult SaveImagesToFileFunctor::currentImageIndex( ImageControl& ctrl, int currentSlc, int& currentIndex ) 
{
    ICE_SET_FN("SaveImagesToFileFunctor::currentImageIndex")
    
    currentIndex    = currentSlc
                    + ctrl.m_lc.m_cset * m_iImaSlcLenKeep
                    + ctrl.m_lc.m_ceco * m_iImaSlcLenKeep * m_iImaSetLen
                    + ctrl.m_lc.m_crep * m_iImaSlcLenKeep * m_iImaSetLen * m_iImaEcoLen
                    + ctrl.m_lc.m_cida * m_iImaSlcLenKeep * m_iImaSetLen * m_iImaEcoLen * m_iImaRepLen;
    
    if ( currentIndex >= m_iTotalSlc )
    {ICE_RET_ERR("Index exceed total number of slices!",I_FAIL);}

    return I_OK;
}

//-----------------------------------------------------------------------------
// currentDateTime()
// Description: Get current date/time, format is YYYYMMDD_HHmmss
//-----------------------------------------------------------------------------

std::string SaveImagesToFileFunctor::currentDateTime() 
{
    ICE_SET_FN("SaveImagesToFileFunctor::currentDateTime")

    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);

    return buf;
}

//-----------------------------------------------------------------------------
// createFolderIfNeeded(std::string)
// Description: create a folder if does not exist
//-----------------------------------------------------------------------------

void SaveImagesToFileFunctor::createFolderIfNeeded( std::string strFolderName )
{
    ICE_SET_FN("SaveImagesToFileFunctor::createFolderIfNeeded")
    
    // Initialize local variables
    struct stat st;

    // Check if folder name finish with a slash and remove it
    if ( *strFolderName.rbegin() == '/' )
    {strFolderName.erase( strFolderName.size()-1 );}
    
    // Create "CoilName" folder if needed
    if ( stat(strFolderName.c_str(), &st) != 0 ) /* returns 0 on success */
    {
        ICE_OUT("Folder " << strFolderName << " does not exist, try to create it !");
        #ifdef BUILD_PLATFORM_LINUX
            mkdir(strFolderName.c_str(), S_IRWXU | S_IRWXG | S_IRWXO); // Any rights to anyone
        #else
            mkdir(strFolderName.c_str());
        #endif
    }
    
}

//-----------------------------------------------------------------------------
// addReorientedMatlabData()
// Description: 
//-----------------------------------------------------------------------------

std::string SaveImagesToFileFunctor::addReorientedMatlabData( const char * varin, const char * varout )
{
    // addReorientedMatlabData() assumes varin volume comes with the following order: lin,col,slc 
    
    std::stringstream ss, ssinfo;
    bool bReorientedData = true;
    int  lI;
    double dPosTra, dPosCor, dPosSag, dAveragePos;
    
    if ( m_iGroupArraySize > 1 )
    {
        // Multiple orientation defined
        // Just sending all slice data information
        // responsability of matlab user to process this information
        for ( lI=0; lI<m_iSliceArraySize; lI++ )
        {
            ssinfo << "    SliceInfo("<<lI+1<<").dInPlaneRot     = "<<m_vdImInPlaneRot[lI]<<";\n";
            ssinfo << "    SliceInfo("<<lI+1<<").sNormal.dTra    = "<<m_vdImOrientTra[lI]<<";\n";
            ssinfo << "    SliceInfo("<<lI+1<<").sNormal.dCor    = "<<m_vdImOrientCor[lI]<<";\n";
            ssinfo << "    SliceInfo("<<lI+1<<").sNormal.dSag    = "<<m_vdImOrientSag[lI]<<";\n";
            ssinfo << "    SliceInfo("<<lI+1<<").sPosition.dTra  = "<<m_vdImPosTra[lI]<<";       % Positive=Head,        Negative=Foot \n";
            ssinfo << "    SliceInfo("<<lI+1<<").sPosition.dCor  = "<<m_vdImPosCor[lI]<<";       % Positive=Posterior,   Negative=Anterior \n";
            ssinfo << "    SliceInfo("<<lI+1<<").sPosition.dSag  = "<<m_vdImPosSag[lI]<<";       % Positive=Left,        Negative=Right \n";
        }
        
        ss << "% Data acquired with multiple slice groups\n";
        ss << "% No re-orientation is performed\n";
        ss << "    "<<varout<<" = "<<varin<<";\n\n";
        ss << ssinfo.str();
        return ss.str();
    }
    else
    {
    // Group Array Size is 1
    // It means all slices will have the same orientation
    ssinfo << "    SliceInfo.dInPlaneRot     = "<<m_vdImInPlaneRot[0]<<";\n";
    ssinfo << "    SliceInfo.sNormal.dTra    = "<<m_vdImOrientTra[0]<<";\n";
    ssinfo << "    SliceInfo.sNormal.dCor    = "<<m_vdImOrientCor[0]<<";\n";
    ssinfo << "    SliceInfo.sNormal.dSag    = "<<m_vdImOrientSag[0]<<";\n";
    
    if ( m_iSliceArraySize > 0 )
    {
        // SliceArraySize = 1 means either 1 slice acquired in 2D or a volume acquired in 3D
        // SliceArraySize > 1 means 2D multi slice but same orientation assumed and equally spaced
        
        dPosTra=0; dPosCor=0; dPosSag=0; 
        for ( lI=0; lI<m_iSliceArraySize; lI++ )
        {
            dPosTra += m_vdImPosTra[lI];
            dPosCor += m_vdImPosCor[lI];
            dPosSag += m_vdImPosSag[lI];
        }
        ssinfo << "    SliceInfo.sPosition.dTra  = "<<dPosTra/m_iSliceArraySize<<";       % Positive=Head,        Negative=Foot \n";
        ssinfo << "    SliceInfo.sPosition.dCor  = "<<dPosCor/m_iSliceArraySize<<";       % Positive=Posterior,   Negative=Anterior \n";
        ssinfo << "    SliceInfo.sPosition.dSag  = "<<dPosSag/m_iSliceArraySize<<";       % Positive=Left,        Negative=Right \n";
    
        if ( m_vdImOrientTra[0] != 1.0 && m_vdImOrientCor[0] != 1.0 && m_vdImOrientSag[0] != 1.0 )
        {
            for ( lI=0; lI<m_iSliceArraySize; lI++ )
            {
                ssinfo << "    SliceInfo.sPositionSliceIndex("<<lI+1<<").dTra  = "<<m_vdImPosTra[lI]<<"; ";
                ssinfo << "    SliceInfo.sPositionSliceIndex("<<lI+1<<").dCor  = "<<m_vdImPosCor[lI]<<"; ";
                ssinfo << "    SliceInfo.sPositionSliceIndex("<<lI+1<<").dSag  = "<<m_vdImPosSag[lI]<<";\n";
            }
        }
        
        /*
        if ( m_vdImOrientTra[0] == 1.0 )
        {
            dMinPos=m_vdImPosTra[0]; dMaxPos=m_vdImPosTra[0];
            for ( lI=1; lI<m_iSliceArraySize; lI++ )
            {
                dMinPos=(m_vdImPosTra[lI]<dMinPos?m_vdImPosTra[lI]:dMinPos);
                dMaxPos=(m_vdImPosTra[lI]>dMaxPos?m_vdImPosTra[lI]:dMaxPos);
                dAveragePos=(dMaxPos+dMinPos)/2.;
            }
            ssinfo << "    SliceInfo.sPosition.dTra  = "<<dAveragePos    <<";       % Positive=Head,        Negative=Foot \n";
            ssinfo << "    SliceInfo.sPosition.dCor  = "<<m_vdImPosCor[0]<<";       % Positive=Posterior,   Negative=Anterior \n";
            ssinfo << "    SliceInfo.sPosition.dSag  = "<<m_vdImPosSag[0]<<";       % Positive=Left,        Negative=Right \n";
        }
        else if ( m_vdImOrientCor[0] == 1.0 )
        {
            dMinPos=m_vdImPosCor[0]; dMaxPos=m_vdImPosCor[0];
            for ( lI=1; lI<m_iSliceArraySize; lI++ )
            {
                dMinPos=(m_vdImPosCor[lI]<dMinPos?m_vdImPosCor[lI]:dMinPos);
                dMaxPos=(m_vdImPosCor[lI]>dMaxPos?m_vdImPosCor[lI]:dMaxPos);
                dAveragePos=(dMaxPos+dMinPos)/2.;
            }
            ssinfo << "    SliceInfo.sPosition.dTra  = "<<m_vdImPosTra[0]<<";       % Positive=Head,        Negative=Foot \n";
            ssinfo << "    SliceInfo.sPosition.dCor  = "<<dAveragePos    <<";       % Positive=Posterior,   Negative=Anterior \n";
            ssinfo << "    SliceInfo.sPosition.dSag  = "<<m_vdImPosSag[0]<<";       % Positive=Left,        Negative=Right \n";
        }
        else if ( m_vdImOrientSag[0] == 1.0 )
        {
            dMinPos=m_vdImPosSag[0]; dMaxPos=m_vdImPosSag[0];
            for ( lI=1; lI<m_iSliceArraySize; lI++ )
            {
                dMinPos=(m_vdImPosSag[lI]<dMinPos?m_vdImPosSag[lI]:dMinPos);
                dMaxPos=(m_vdImPosSag[lI]>dMaxPos?m_vdImPosSag[lI]:dMaxPos);
                dAveragePos=(dMaxPos+dMinPos)/2.;
            }
            ssinfo << "    SliceInfo.sPosition.dTra  = "<<m_vdImPosTra[0]<<";       % Positive=Head,        Negative=Foot \n";
            ssinfo << "    SliceInfo.sPosition.dCor  = "<<m_vdImPosCor[0]<<";       % Positive=Posterior,   Negative=Anterior \n";
            ssinfo << "    SliceInfo.sPosition.dSag  = "<<dAveragePos    <<";       % Positive=Left,        Negative=Right \n";
        }
        else
        {
            for ( lI=0; lI<m_iSliceArraySize; lI++ )
            {
                ssinfo << "    SliceInfo.sPosition("<<lI+1<<").dTra  = "<<m_vdImPosTra[lI]<<";       % Positive=Head,        Negative=Foot \n";
                ssinfo << "    SliceInfo.sPosition("<<lI+1<<").dCor  = "<<m_vdImPosCor[lI]<<";       % Positive=Posterior,   Negative=Anterior \n";
                ssinfo << "    SliceInfo.sPosition("<<lI+1<<").dSag  = "<<m_vdImPosSag[lI]<<";       % Positive=Left,        Negative=Right \n";
            }
        }
        */
    }
    
    
    } // GroupArraySize = 1
    
    
    // Tranverse
    if ( m_vdImOrientTra[0] == 1.0 )
    {
        if ( fabs(m_vdImInPlaneRot[0]) < 1e-4 )
        {
            // Transverse A->P
            ss << "% Data acquired in transverse A->P \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [2 1 3 4 5 6 7 8]);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 1);\n";
        }
        else if ( fabs(m_vdImInPlaneRot[0]-M_PI/2) < 1e-4 )
        {
            // Transverse R->L
            // Nothing to do for reorientation
            ss << "% Data acquired in transverse R->L \n";
            ss << "% No reorientation required \n";
            ss << "    "<<varout<<" = "<<varin<<";\n";
        }
        else if ( fabs(m_vdImInPlaneRot[0]-M_PI) < 1e-4 )
        {
            // Transverse P->A
            ss << "% Data acquired in transverse P->A \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [2 1 3 4 5 6 7 8]);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 2);\n";            
        }
        else if ( fabs(m_vdImInPlaneRot[0]+M_PI/2) < 1e-4 )
        {
            // Transverse L->R
            ss << "% Data acquired in transverse L->R \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = flipdim("<<varin<<"  , 1);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 2);\n";
        }
        else
        {
            bReorientedData = false;
        }
    }
    // Sagittal
    else if ( m_vdImOrientSag[0] == 1.0 )
    {
        // Sagittal A->P
        if ( fabs(m_vdImInPlaneRot[0]) < 1e-4 )
        {
            ss << "% Data acquired in sagittal A->P \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [3 1 2 4 5 6 7 8]);\n";
        }
        // Sagittal H->F
        else if ( fabs(m_vdImInPlaneRot[0]-M_PI/2) < 1e-4 )
        {
            ss << "% Data acquired in sagittal H->F \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [3 2 1 4 5 6 7 8]);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 3);\n";
        }
        // Sagittal P->A
        else if ( fabs(m_vdImInPlaneRot[0]-M_PI) < 1e-4 )
        {
            ss << "% Data acquired in sagittal P->A \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [3 1 2 4 5 6 7 8]);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 2);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 3);\n";
        }
        // Sagittal F->H
        else if ( fabs(m_vdImInPlaneRot[0]+M_PI/2) < 1e-4 )
        {
            ss << "% Data acquired in sagittal F->H \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [3 2 1 4 5 6 7 8]);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 2);\n";
        }
        else
        {
            bReorientedData = false;
        }
    }
    // Coronal
    else if ( m_vdImOrientCor[0] == 1.0 )
    {
        // Coronal R->L
        if ( fabs(m_vdImInPlaneRot[0]) < 1e-4 )
        {
            ss << "% Data acquired in coronal R->L \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [1 3 2 4 5 6 7 8]);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 3);\n";
        }
        // Coronal F->H
        else if ( fabs(m_vdImInPlaneRot[0]-M_PI/2) < 1e-4 )
        {
            ss << "% Data acquired in coronal F->H \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [2 3 1 4 5 6 7 8]);\n";
        }
        // Coronal L->R
        else if ( fabs(m_vdImInPlaneRot[0]-M_PI) < 1e-4 )
        {
            ss << "% Data acquired in coronal L->R \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [1 3 2 4 5 6 7 8]);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 1);\n";
        }
        // Coronal H->F
        else if ( fabs(m_vdImInPlaneRot[0]+M_PI/2) < 1e-4 )
        {
            ss << "% Data acquired in coronal H->F \n";
            ss << "% Reorienting data in transverse R->L \n";
            ss << "    "<<varout<<" = permute("<<varin<<"  , [2 3 1 4 5 6 7 8]);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 1);\n";
            ss << "    "<<varout<<" = flipdim("<<varout<<"       , 3);\n";
        }
        else
        {
            bReorientedData = false;
        }
    }
    else
    {
        bReorientedData = false;
    }
    
    if ( bReorientedData == false )
    {
        ss << "% Data acquired in oblique orientation \n";
        ss << "% Cannot reorient data to transverse R->L \n";
        ss << "    "<<varout<<" = "<<varin<<";\n";
    }

    ss << "\n";
    ss << ssinfo.str();
    
    return ss.str();
}

//-----------------------------------------------------------------------------
// EOF
//-----------------------------------------------------------------------------
