//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: IceExplorer
//-       Version: IceExplorer
//-        Author: Franck MAUCONDUIT (franck.mauconduit@siemens.com)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\XFL_postproc\ExplorerImageFunctor.h
//-          Date: Fri Nov 18 18:35:18 2016
//-      Language: C++
//-
//-   Description: <ICE functor to explore and dump data in a pipeline.>
//-
//-----------------------------------------------------------------------------------


#ifndef ExplorerImageFunctor_h
#define ExplorerImageFunctor_h 1


// Import/Export DLL macro:
#include "MrServers/MrVista/Ice/IceIdeaFunctors/dllInterface.h"
// Base class:
#include "MrServers/MrVista/include/Ice/IceUtils/IceImageReconFunctors.h"


class ICEIDEAFUNCTORS_API ExplorerImageFunctor : public IceImageReconFunctors
{
    public:
        
        ICE_IMAGE_RECON_FUNCTOR(ExplorerImageFunctor)

		BEGIN_PROPERTY_MAP( ExplorerImageFunctor )
            
			PROPERTY_ENTRY( StopChain )
            PROPERTY_ENTRY( CheckAndDisplay )
            PROPERTY_ENTRY( VisuScaleFactor )
            PROPERTY_ENTRY( DumpCurrentLoopIndex )
			PROPERTY_ENTRY( DumpIceAs )
			PROPERTY_ENTRY( DumpIceObj )
            PROPERTY_ENTRY( DumpMiniHeader )
            PROPERTY_ENTRY( DumpImageControl )
            PROPERTY_ENTRY( DumpParcENV )
            PROPERTY_ENTRY( DumpParcPDS )
			PROPERTY_ENTRY( ccol )
			PROPERTY_ENTRY( ccha )
			PROPERTY_ENTRY( clin )
			PROPERTY_ENTRY( cset )
			PROPERTY_ENTRY( ceco )
			PROPERTY_ENTRY( cphs )
			PROPERTY_ENTRY( cidd )
			PROPERTY_ENTRY( cide )
			PROPERTY_ENTRY( cave )
			PROPERTY_ENTRY( cpar )
			PROPERTY_ENTRY( cslc )
			PROPERTY_ENTRY( cida )
			PROPERTY_ENTRY( cidb )
			PROPERTY_ENTRY( cidc )
			
		END_PROPERTY_MAP()

        DECL_GET_SET_PROPERTY( bool, 		b, 	StopChain )
		DECL_GET_SET_PROPERTY( bool, 		b, 	CheckAndDisplay )
		DECL_GET_SET_PROPERTY( double,      d,  VisuScaleFactor)		
        DECL_GET_SET_PROPERTY( bool, 		b, 	DumpCurrentLoopIndex )
		DECL_GET_SET_PROPERTY( bool,		b,	DumpIceAs)
		DECL_GET_SET_PROPERTY( bool,		b,	DumpIceObj)
        DECL_GET_SET_PROPERTY( bool,		b,	DumpMiniHeader)
        DECL_GET_SET_PROPERTY( bool,		b,	DumpImageControl)
        DECL_GET_SET_PROPERTY( bool,		b,	DumpParcENV)
        DECL_GET_SET_PROPERTY( bool,		b,	DumpParcPDS)
        DECL_GET_SET_PROPERTY( bool,		b,	RemoveMAG)
        DECL_GET_SET_PROPERTY( bool,		b,	RemovePHS)
		DECL_GET_SET_PROPERTY( double, 		d,	ccol )
		DECL_GET_SET_PROPERTY( double, 		d,	ccha )
		DECL_GET_SET_PROPERTY( double, 		d,	clin )
		DECL_GET_SET_PROPERTY( double, 		d,	cset )
		DECL_GET_SET_PROPERTY( double, 		d,	ceco )
		DECL_GET_SET_PROPERTY( double, 		d,	cphs )
		DECL_GET_SET_PROPERTY( double, 		d,	cidd )
		DECL_GET_SET_PROPERTY( double, 		d,	cide )
		DECL_GET_SET_PROPERTY( double, 		d,	cave )
		DECL_GET_SET_PROPERTY( double, 		d,	cpar )
		DECL_GET_SET_PROPERTY( double, 		d,	cslc )
		DECL_GET_SET_PROPERTY( double, 		d,	cida )
		DECL_GET_SET_PROPERTY( double, 		d,	cidb )
		DECL_GET_SET_PROPERTY( double, 		d,	cidc )

        // -----------------------------
        // - constructor / destructor -
        // -----------------------------
        ExplorerImageFunctor();
        virtual ~ExplorerImageFunctor();

        // Application of functor functionality:
        IResult ComputeImage(IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& controlData);

        bool keepChosenData( ImageControl& controlData);

    protected:
    
        // ParcDataStore
		IPDS::Pointer	m_pPDS;
        
		// Configuration of Functor (applied after PROPERTY_MAP is set):
		IResult EndInit( IParcEnvironment* env );
        
         // Finalize (triggered by ACQ_END flag from acquisition):
        virtual IResult endOfJob(IResult reason);

    private:
        
        ExplorerImageFunctor(const ExplorerImageFunctor &right);
        ExplorerImageFunctor & operator=(const ExplorerImageFunctor &right);
};

#endif
