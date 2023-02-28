//-----------------------------------------------------------------------------------
//-	  Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: IceExplorer
//-       Version: IceExplorer
//-        Author: Franck MAUCONDUIT (franck.mauconduit@siemens.com)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\XFL_postproc\ExplorerScanFunctor.h
//-          Date: Fri Nov 18 18:35:18 2016
//-      Language: C++
//-
//-   Description: <ICE functor to explore and dump data in a pipeline.>
//-
//-----------------------------------------------------------------------------------
#ifndef ExplorerScanFunctor_h
#define ExplorerScanFunctor_h 1


// Base class:
#include "MrServers/MrVista/include/Ice/IceUtils/IceScanFunctors.h"

class ExplorerScanFunctor : public IceScanFunctors
{
    public:
        
        ICE_SCAN_FUNCTOR(ExplorerScanFunctor)

		BEGIN_PROPERTY_MAP( ExplorerScanFunctor )
            
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
        ExplorerScanFunctor();
        virtual ~ExplorerScanFunctor();

        
        // Application of functor functionality:
        virtual IResult ComputeScan(IceAs& srcAs, MdhProxy& aMdh, ScanControl& ctrl);
        virtual IResult endOfJob(IResult reason);
		virtual IResult EndInit( IParcEnvironment* env );
        
        bool keepChosenData( MdhProxy& );

    protected:
    
        // ParcDataStore
		IPDS::Pointer	m_pPDS;
        
    private:
        
        ExplorerScanFunctor(const ExplorerScanFunctor &right);
        ExplorerScanFunctor & operator=(const ExplorerScanFunctor &right);
};

#endif