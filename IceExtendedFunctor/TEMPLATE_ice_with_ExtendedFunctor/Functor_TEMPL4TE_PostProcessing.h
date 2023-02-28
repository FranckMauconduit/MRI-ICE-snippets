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

#ifndef TEMPL4TEFunctor_h
#define TEMPL4TEFunctor_h 1

//-----------------------------------------------------------------------------
// Class libraries
//-----------------------------------------------------------------------------

// Import/Export DLL macro:
#include "MrServers/MrVista/Ice/IceIdeaFunctors/dllInterface.h"

// Base class:
#include "MrServers/MrVista/include/Ice/IceUtils/IceImageReconFunctors.h"

// Extended Functor Class:
#include "XFunctor.h"


//-----------------------------------------------------------------------------
// Class definition
//-----------------------------------------------------------------------------

#ifndef ICE_NAMESPACE
    #error ICE_NAMESPACE NOT DEFINED IN MAKEFILE ( ex: CPPFLAGS_LOCAL  += -DICE_NAMESPACE=$(TARGET) )
#endif

namespace ICE_NAMESPACE
{

class  TEMPL4TEFunctor : public ICE_NAMESPACE::ExtendedFunctor
{

    public:     // public members

        // Macro defining PARC component and member events (e.g. ImageReady)
        ICE_IMAGE_RECON_FUNCTOR_DERIVED(TEMPL4TEFunctor, ICE_NAMESPACE::ExtendedFunctor)
        
        // needed macros to declare class members for IRIS environment:
        // BEGIN_PROPERTY_MAP(TEMPL4TEFunctor)
            // WIP CARD PARAMETERS
          // #ifdef _VX_VERSION_VB
            // PROPERTY_DEFAULT ( WipInt,                   "MEAS.sWiPMemBlock.alFree"  )
            // PROPERTY_DEFAULT ( WipDouble,                "MEAS.sWiPMemBlock.adFree"  )
            // PROPERTY_DEFAULT ( WipString,                "MEAS.sWiPMemBlock.tFree"   )
          // #elif _VX_VERSION_VE
            // PROPERTY_DEFAULT ( WipInt,                   "MEAS.sWipMemBlock.alFree"  )
            // PROPERTY_DEFAULT ( WipDouble,                "MEAS.sWipMemBlock.adFree"  )
            // PROPERTY_DEFAULT ( WipString,                "MEAS.sWipMemBlock.tFree"   )
          // #endif
            // MrProt PARAMETERS
            // PROPERTY_DEFAULT ( NumContrast,              "MEAS.lContrasts")
            // CONFIGURATOR PARAMETERS
            // PROPERTY_ENTRY ( VerboseMode )
        // END_PROPERTY_MAP()

        // macro to declare get/set methods and attribute in Hungarian Notation;
        // (parameter name in this macro must match with name in PROPERTY_MAP macro)
        // DECL_GET_SET_PROPERTY(vector <int> , vi, WipInt)         // --> private: m_lWipInt; public: getWipInt(),setWipInt()
        // DECL_GET_SET_PROPERTY(vector <double>,   vd, WipDouble)  // --> private: m_dWipDouble;public: getWipDouble(),setWipDouble()
        // DECL_GET_SET_PROPERTY(bool,          b,  VerboseMode)
        
        // constructor / destructor
        TEMPL4TEFunctor();
        virtual ~TEMPL4TEFunctor();
            
        // - callbacks -
        // Called once at first call of ComputeImage(...):
        virtual IResult FirstCall(IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& ctrl);
        
        // Functor functionality
        IResult ComputeImage( IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& controlData );
        
    protected:  // protected members
    
        // Configuration of Functor (applied after PROPERTY_MAP is set):
        IResult EndInit( IParcEnvironment* env );

        // Do some "finalize" tasks (e.g. tracing, handle ACQ_END trigger from acquisition)
        virtual IResult endOfJob (IResult reason);
        
        // Method to compute the X map and store it within the PARC environment
        IResult computeMaps( IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& ctrl );
        
        // Define parameters for sending data 2 next functor
        ICE_NAMESPACE::sendPars    cSendAmpli;
        ICE_NAMESPACE::sendPars    cSendPhase;
        ICE_NAMESPACE::sendPars    cSendSum;
        
        // Define parameters for storing data either in PARC or PDS
        ICE_NAMESPACE::storePars   cStoreDATA;
        
        // Define class members
        bool        m_bVerboseMode;
        
    private:    // private members
        
        TEMPL4TEFunctor(const TEMPL4TEFunctor &right);
        TEMPL4TEFunctor & operator=(const TEMPL4TEFunctor &right);


};

} // end of ICE_NAMESPACE

#endif

