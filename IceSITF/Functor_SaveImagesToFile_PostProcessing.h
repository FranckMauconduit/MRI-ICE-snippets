//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: Save Images To File
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\IceSITF\Functor_SaveImagesToFile_PostProcessing.h
//-          Date: Thu Jul 18 13:58:25 2019
//-      Language: C++
//-
//-   Description: Insert a functor into pipeline to save all incoming images into a file
//-
//-----------------------------------------------------------------------------------

#ifndef SaveImagesToFileFunctor_h
#define SaveImagesToFileFunctor_h 1

//-----------------------------------------------------------------------------
// Class libraries
//-----------------------------------------------------------------------------

// Base class:
#include "MrServers/MrVista/include/Ice/IceUtils/IceImageReconFunctors.h"

// Import/Export DLL macro:
#include "MrServers/MrVista/Ice/IceIdeaFunctors/dllInterface.h"

// createDicomInstanceUid
#include "MrServers/MrVista/include/Parc/Utils/DicomUtils.h"

// Functor to display data
#include "MrServers/MrVista/include/Ice/IceUtils/IceUtils.h"


#include <fstream>
#include <iostream>


//-----------------------------------------------------------------------------
// Class definition
//-----------------------------------------------------------------------------

namespace ICE_NAMESPACE
{

class ICEIDEAFUNCTORS_API SaveImagesToFileFunctor : public IceImageReconFunctors
{

	public:		// public members

		// Macro defining PARC component and member events (e.g. ImageReady)
		ICE_IMAGE_RECON_FUNCTOR(SaveImagesToFileFunctor)
        
		// needed macros to declare class members for IRIS environment:
		BEGIN_PROPERTY_MAP(SaveImagesToFileFunctor)
			// WIP CARD PARAMETERS

		    // MrProt PARAMETERS
            PROPERTY_DEFAULT ( MeasUID, "HEADER.MeasUID")
            PROPERTY_DEFAULT ( FileID,  "HEADER.FileID")
            
		    // CONFIGURATOR PARAMETERS
          #ifdef N4_VB17A_LATEST_20121129 // FOR PTX ONLY
            PROPERTY_DEFAULT ( NumTXChannels,   "YAPS.lTXANumberOfChannels")
          #elif _VX_VERSION_VE
            PROPERTY_DEFAULT ( NumTXChannels,   "YAPS.lNumberOfTXCalibDateTime")
          #endif
          
          #ifdef _VX_VERSION_VB
            PROPERTY_DEFAULT ( WipInt,                  "MEAS.sWiPMemBlock.alFree")
            PROPERTY_DEFAULT ( WipDouble,               "MEAS.sWiPMemBlock.adFree")
          #elif _VX_VERSION_VE
            PROPERTY_DEFAULT ( WipInt,                  "MEAS.sWipMemBlock.alFree"  )
            PROPERTY_DEFAULT ( WipDouble,               "MEAS.sWipMemBlock.adFree"  )
            PROPERTY_DEFAULT ( TxScaleReal,             "MEAS.sTXSPEC.aTxScaleFactor.*.dRe")
            PROPERTY_DEFAULT ( TxScaleImag,             "MEAS.sTXSPEC.aTxScaleFactor.*.dIm")
          #endif
            
            PROPERTY_DEFAULT ( RFPulseName,             "MEAS.sTXSPEC.aRFPULSE.*.tName")
            PROPERTY_DEFAULT ( RFPulseVoltage,          "MEAS.sTXSPEC.aRFPULSE.*.flAmplitude")
            
            PROPERTY_DEFAULT ( ImOrientSag,    "MEAS.sSliceArray.asSlice.*.sNormal.dSag" )
            PROPERTY_DEFAULT ( ImOrientCor,    "MEAS.sSliceArray.asSlice.*.sNormal.dCor" )
            PROPERTY_DEFAULT ( ImOrientTra,    "MEAS.sSliceArray.asSlice.*.sNormal.dTra" )
            PROPERTY_DEFAULT ( ImPosSag,       "MEAS.sSliceArray.asSlice.*.sPosition.dSag" )
            PROPERTY_DEFAULT ( ImPosCor,       "MEAS.sSliceArray.asSlice.*.sPosition.dCor" )
            PROPERTY_DEFAULT ( ImPosTra,       "MEAS.sSliceArray.asSlice.*.sPosition.dTra" )
            PROPERTY_DEFAULT ( ImInPlaneRot,   "MEAS.sSliceArray.asSlice.*.dInPlaneRot" )
            PROPERTY_DEFAULT ( SliceArraySize, "MEAS.sSliceArray.lSize" )
            PROPERTY_DEFAULT ( GroupArraySize, "MEAS.sGroupArray.lSize" )
            
		    PROPERTY_ENTRY ( SendDiskDir )
            PROPERTY_ENTRY ( SendDiskFile )
            PROPERTY_ENTRY ( DateAndTime )
            PROPERTY_ENTRY ( LinkedFunctor )
            PROPERTY_ENTRY ( SinkName )
            PROPERTY_ENTRY ( EventName )
            PROPERTY_ENTRY ( Successor )
            PROPERTY_ENTRY ( extractMAG_mode32 )
            PROPERTY_ENTRY ( SliceMin )
            PROPERTY_ENTRY ( SliceMax )
            PROPERTY_ENTRY ( PassOnData )
            
		END_PROPERTY_MAP()

		// macro to declare get/set methods and attribute in Hungarian Notation;
		// (parameter name in this macro must match with name in PROPERTY_MAP macro)
        DECL_GET_SET_PROPERTY(int, 		    i,	MeasUID)
        DECL_GET_SET_PROPERTY(int,          i,  FileID)
		DECL_GET_SET_PROPERTY(const char *,	t,	SendDiskDir)
        DECL_GET_SET_PROPERTY(const char *,	t,	SendDiskFile)
		DECL_GET_SET_PROPERTY(bool,			b,	DateAndTime)
        DECL_GET_SET_PROPERTY(const char *,	t,	LinkedFunctor)
        DECL_GET_SET_PROPERTY(const char *,	t,	SinkName)
        DECL_GET_SET_PROPERTY(const char *,	t,	EventName)
		DECL_GET_SET_PROPERTY(bool,			b,	Successor)
		DECL_GET_SET_PROPERTY(bool,			b,	extractMAG_mode32)
        DECL_GET_SET_PROPERTY(int, 		    i,	SliceMin)
        DECL_GET_SET_PROPERTY(int, 		    i,	SliceMax)
		DECL_GET_SET_PROPERTY(bool,			b,	PassOnData)
        
      #if !defined( N4_VB17A_LATEST_20090307 ) // FOR PTX ONLY
        DECL_GET_SET_PROPERTY(int,          i,  NumTXChannels)
      #endif
        
        DECL_GET_SET_PROPERTY(vector <std::string>, vs, RFPulseName     )
        DECL_GET_SET_PROPERTY(vector <double>,      vd, RFPulseVoltage  )
        DECL_GET_SET_PROPERTY(vector <int> ,    vi, WipInt) // --> private: m_lWipInt; public: getWipInt(),setWipInt()
        DECL_GET_SET_PROPERTY(vector <double>,  vd, WipDouble)// --> private: m_dWipDouble;public: getWipDouble(),setWipDouble()
        
      #ifdef _VX_VERSION_VE
        DECL_GET_SET_PROPERTY(vector <double>,   vd,	TxScaleReal)
        DECL_GET_SET_PROPERTY(vector <double>,   vd,	TxScaleImag)
      #endif
        
        DECL_GET_SET_PROPERTY(vector <double>,   vd,	ImOrientSag)
        DECL_GET_SET_PROPERTY(vector <double>,   vd,	ImOrientCor)
        DECL_GET_SET_PROPERTY(vector <double>,   vd,	ImOrientTra)
        DECL_GET_SET_PROPERTY(vector <double>,   vd,	ImPosSag)
        DECL_GET_SET_PROPERTY(vector <double>,   vd,	ImPosCor)
        DECL_GET_SET_PROPERTY(vector <double>,   vd,	ImPosTra)
        DECL_GET_SET_PROPERTY(vector <double>,   vd,	ImInPlaneRot)
        DECL_GET_SET_PROPERTY(int, 		          i,	SliceArraySize)
        DECL_GET_SET_PROPERTY(int, 		          i,	GroupArraySize)
                
        // constructor / destructor
        SaveImagesToFileFunctor();
		virtual ~SaveImagesToFileFunctor();
        
        // Constructor used in derived class to setup different default values of PROPERTY parameters 
        SaveImagesToFileFunctor( const char *, const char *, bool, const char *, bool, bool, int i7 = -1, int i8 = -1, bool b9 = true );
        
		// - callbacks -
		// Called once at first call of ComputeImage(...):
		virtual IResult FirstCall(IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& ctrl);
		
		// Functor functionality
		IResult ComputeImage( IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& controlData );
        
	protected:	// protected members
	
        //==========
        // METHODS
        //==========
        
		// Configuration of Functor (applied after PROPERTY_MAP is set):
		virtual IResult EndInit( IParcEnvironment* env );

		// Do some "finalize" tasks (e.g. tracing, handle ACQ_END trigger from acquisition)
		virtual IResult endOfJob (IResult reason);

        // Method to retrieve images in PDS and send them to disk
        IResult SaveImages2File( MiniHeader::Pointer& dataHeader, ImageControl& controlData );
        
        // Method to uniquely identify incoming image
        IResult currentImageIndex( ImageControl& ctrl, int currentSlc, int& currentIndex );
        
        // Method to get a time for filename to be written to disk
        std::string currentDateTime( void );
        
        void createFolderIfNeeded( std::string );
        
        // Method to save protocol file
        void SaveProtocolToFile( std::string destFile );
        
        // Method to reorient data in matlab script
        std::string addReorientedMatlabData( const char * varin, const char * varout );
        
        //============
        // PARAMETERS
        //============
        
		IceAs	    m_ImageAs;
		int		    m_iImageCounter, m_iTotalSlc;

        int         m_iImaColLen, m_iImaLinLen, m_iImaChaLen, m_iImaSlcLen, m_iImaSetLen, m_iImaEcoLen, m_iImaRepLen, m_iImaIdaLen;
        int         m_iImaSlcLenKeep;
        
        bool        m_bIsSaved, m_bIs2D;
        
        IceFormat_t m_tFormat;
        
        std::string m_sAnatSliceNumber;
        
      #ifdef N4_VB17A_LATEST_20090307 // FOR PTX ONLY
        int m_iNumTXChannels;
      #endif
        
	private:	// private members
        
		SaveImagesToFileFunctor(const SaveImagesToFileFunctor &right);
		SaveImagesToFileFunctor & operator=(const SaveImagesToFileFunctor &right);

};

} // END OF ICE_NAMESPACE

#endif

