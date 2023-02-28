//-----------------------------------------------------------------------------------
//-   Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: Extended Functor Base Functions
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\IceQA\XFunctor.h
//-          Date: Tue Mar  5 19:47:10 2019
//-      Language: C++
//-
//-   Description: <Class to extend functor with some basic functions usually used in any functor.>
//-
//-----------------------------------------------------------------------------------

#ifndef XFunctor_h
#define XFunctor_h 1

/*!
 * @file doxygen_c.h
 * @author My Self
 * @date 9 Sep 2012
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

/*! \file IceDefs.h
  Testing automatic link generation.
*/
 
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

// Default Image Numbering method
#include "MrServers/MrVista/include/Ice/IceUtils/ImageNumbering.h"

#include<vector>


//-----------------------------------------------------------------------------
// Class definition
//-----------------------------------------------------------------------------

#ifndef ICE_NAMESPACE
    #error ICE_NAMESPACE NOT DEFINED IN MAKEFILE ( ex: CPPFLAGS_LOCAL  += -DICE_NAMESPACE=$(TARGET) )
#endif

namespace ICE_NAMESPACE
{

//! Enumerator for module and phase discrimination in image type
enum ImageType_t { 
                    M,  /**< corresponds to MODULE  */
                    P   /**< corresponds to PHASE  */
                 };
                 
//! storePars class defines all the properties to store an image in PARC or PDS using function ExtendedFunctor::storeImageInPARC()
/*!
  Define several values that are used in storeImageInPARC() to set the way images are stored.
  @note DEFAULT MEMBER VALUES ARE:
    - sPrefix = ""
    - eIceFormat = ICE_FORMAT_FL
    - eExtractMode = IemAmplitude
    - bSaveInPARC = true
    - bSaveInPDS = false
    - eLifeTime = IPDS::Study
    - loopcounter = {0}
  @warning 
*/
class storePars
{
    public:     // public members

        // DATA MEMBERS
        std::string             sPrefix;                    //!< Prefix for data to be stored - see also function setPrefix()
        // vector<std::string>     vsPrefixList;            //!< List of all available prefix
        IceFormat_t             eIceFormat;                 //!< ICE Format for saved data - see also function setIceFormat()
        IceExtractMode          eExtractMode;               //!< Mode to extract data from complex to float | not yet implemented - see also function setExtractMode()
        bool                    bSaveInPARC;                //!< define whether data has to be saved in PARC - see also function saveInPARC()
        bool                    bSaveInPDS;                 //!< define whether data has to be saved in PDS - see also function saveInPDS()
        IPDS::lifetime          eLifeTime;                  //!< in case data is saved in PDS, define the lifetime - see also function setLifeTime()
        IceLoopCounters         m_loopcounter;
        bool                    m_bloopcounterSet;
        
        
        // FUNCTION MEMBERS
        //! set a value to the member storePars::sPrefix
        /**
          Prefix is used to store data with a unique identifier. Ice Loop Counter is added to this prefix
        */
        void setPrefix( std::string str )       { sPrefix.assign(str); };
        
        //! set a value to the member storePars::eIceFormat
        /**
          IceFormat defines the data type for the image to be stored
        */
        void setIceFormat( IceFormat_t eVal )   { eIceFormat = eVal; };
        
        //! set a value to the member storePars::eExtractMode
        /**
          If incoming data is complex and data to be stored is not, ExtractMode defines the way of extracting the complex incoming image
        */
        void setExtractMode( IceExtractMode eVal ) { eExtractMode = eVal; };
        
        //! set a value to the member storePars::bSaveInPARC
        /**
          define whether image is saved in PARC or not
        */
        void saveInPARC( bool b )               { bSaveInPARC = b; };
        
        //! set a value to the member storePars::bSaveInPDS
        /**
          define whether image is saved in PDS or not
        */
        void saveInPDS( bool b )                { bSaveInPDS = b; };
        
        //! set a value to the member storePars::eLifeTime
        /**
          If the image is saved in PDS, defines the lifetime to use
        */
        void setLifeTime( IPDS::lifetime lt )   { eLifeTime = lt; };
        
        void setCol(int i) { m_loopcounter.m_ccol=i; m_bloopcounterSet=true; };
        void setLin(int i) { m_loopcounter.m_clin=i; m_bloopcounterSet=true; };
        void setCha(int i) { m_loopcounter.m_ccha=i; m_bloopcounterSet=true; };
        void setSet(int i) { m_loopcounter.m_cset=i; m_bloopcounterSet=true; };
        void setEco(int i) { m_loopcounter.m_ceco=i; m_bloopcounterSet=true; };
        void setPhs(int i) { m_loopcounter.m_cphs=i; m_bloopcounterSet=true; };
        void setRep(int i) { m_loopcounter.m_crep=i; m_bloopcounterSet=true; };
        void setSeg(int i) { m_loopcounter.m_cseg=i; m_bloopcounterSet=true; };
        void setPar(int i) { m_loopcounter.m_cpar=i; m_bloopcounterSet=true; };
        void setSlc(int i) { m_loopcounter.m_cslc=i; m_bloopcounterSet=true; };
        void setIda(int i) { m_loopcounter.m_cida=i; m_bloopcounterSet=true; };
        void setIdb(int i) { m_loopcounter.m_cidb=i; m_bloopcounterSet=true; };
        void setIdc(int i) { m_loopcounter.m_cidc=i; m_bloopcounterSet=true; };
        void setIdd(int i) { m_loopcounter.m_cidd=i; m_bloopcounterSet=true; };
        void setIde(int i) { m_loopcounter.m_cide=i; m_bloopcounterSet=true; };
        void setAve(int i) { m_loopcounter.m_cave=i; m_bloopcounterSet=true; };
        
        
        //! initialize values of the corresponding variable members
        void init( std::string  sPrefix,
                    IceFormat_t eIceFormat,
                    IceExtractMode eExtractMode,
                    bool        bSaveInPARC,
                    bool        bSaveInPDS,
                    IPDS::lifetime eLifeTime);
        
        //! set loopcounter member
        void setLoopCounter( IceLoopCounters lc ) { m_loopcounter = lc; m_bloopcounterSet=true; };
        
        //! reset loopCounter member
        void resetLoopCounter() { m_loopcounter = IceLoopCounters(); m_bloopcounterSet=false; };
        
        //! copy values of all variable members from those set in srcPars        
        void copy( storePars srcPars );
        
        //! print out Ice Loop values of instance member
        std::string dumpLoop( void );
        
        //! print out values of all variable members
        std::string dump( void );

        
        friend std::ostream&  operator << ( std::ostream&, storePars &s);
        friend std::string dumpIceFormat( IceFormat_t );
        friend std::string dumpImageType( ImageType_t );
        friend std::string dumpExtractMode( IceExtractMode );
        friend std::string dumpLifeTime( IPDS::lifetime );
        
        //! Constructor.
        storePars();
        //! Destructor.
        ~storePars() {};
};

//! sendPars class defines all the properties needed to send an image to next functor with the function ExtendedFunctor::sendData2NextFunctor()
/*!
  Define several values that are used in sendData2NextFunctor() to set the way images are passed to next functor.
  @note DEFAULT MEMBER VALUES ARE:
    - eDicomImaType = M
    - eIceFormat = ICE_FORMAT_FL
    - eExtractMode = IemAmplitude
    - dScale = 1.0
    - iEco = -1
    - bSendUnchanged = true
    - all other members are set to empty string ( sUID, sComment, sProcess, sLUTFile, sSuffix)
  @warning 
*/
class sendPars
{
    public:     // public members
    
        // DATA MEMEBERS
        std::string     sUID;               //!< Uniquely identifies the series - see also function setUID()
        std::string     sComment;           //!< On viewer, define a string that appears at bottom of the image - see also function setComment()
        std::string     sProcess;           //!< On viewer, add string specifying a process done during ICE reco - see also function setProcess()
        std::string     sLUTFile;           //!< Define a file for LUT color coded image - see also function setLUTFile()
        ImageType_t     eDicomImaType;      //!< Define image data type: "M" or "P" - set param ImageTypeValue3 - see also function setDicomImaType()
        std::string     sSuffix;            //!< Suffix added at the end of the default serie name - see also function setSuffix()
        IceFormat_t     eIceFormat;         //!< Define the ICE_FORMAT for outgoing data (sendData2NextFunctor) - see also function setIceFormat()
        IceExtractMode  eExtractMode;       //!< Define extraction type when incoming data is complex and eIceFormat is set to float or short - see also function setExtractMode()
        double          dScale;             //!< Scaling factor for out going data - see also function setScale()
        int             iEco;               //!< Define Eco number - see also function setEco()
        bool            bSendUnchanged;     //!< Flag to identify passed data without any change
        
        // FUNCTION MEMBERS
        //! set a value to the member sendPars::sComment
        /**
          previous string is removed
        */
        void setComment( std::string comment )      { sComment.assign(comment); bSendUnchanged=false; };

        //! set a value to the member sendPars::sComment using two strings as argument to be concatenated
        /**
          previous string is removed
        */
        void setComment( std::string comment1, std::string comment2 )      { sComment.assign(comment1); sComment.append(comment2); bSendUnchanged=false; };
        
        //! append a value to the member sendPars::sComment
        /**
          if a previous string has been added, comment is appended to the member value sComment
        */
        void appendComment( std::string comment )   { sComment.append(comment); bSendUnchanged=false; };
        
        //! set a value to the member sendPars::sUID with string to define the serie to use
        void setUID( std::string UID )          { sUID.assign(UID); bSendUnchanged=false; };
        
        //! set a value to the member sendPars::Process
        /**
          sProcess is used as a dicom text for tag IceImageType4.
          It appears as a list on image viewer.
          (could be improved by enabling to add a list of process)
        */
        void setProcess( std::string process )      { sProcess.assign(process); bSendUnchanged=false; };
        
        //! set a value to the member sendPars::sLUTFile - the corresponding dicom tag is set
        /**
          The value must correspond to an existing file with a color map to be used on the image
          If set to empty, no color map is used
        */
        void setLUTFile( std::string filename )      { sLUTFile.assign(filename); bSendUnchanged=false; };
        
        //! set a value to the member sendPars::eDicomImaType - the dicom tag "ImageTypeValue3" is set to this value
        /** 
          eDicomImaType has an effect on the image Distortion correction when it is active in the sequence
          ImageTypeValue3 also appears on the viewer as data information ( M or P )
        */
        void setDicomImaType(  ImageType_t eVal )  { eDicomImaType = eVal; };
        
        //! set a value to the member sendPars::sSuffix
        /** 
          the suffix is appended to the serie name
        */
        void setSuffix( std::string suffix )       { sSuffix.assign(suffix); bSendUnchanged=false; };
        
        //! set a value to the member sendPars::eExtractMode defining the method for extracting complex data to float data
        /** 
          Extraction mode can be one of the following:
            - IemAmplitude
            - IemPhase
            - ...
        */
        void setExtractMode( IceExtractMode eVal ) { eExtractMode = eVal; };
        
        //! set a value to the member sendPars::dScale
        /** 
          scaling factor is used to scale the image before sending to next functor
          (to be verified if done on any case)
        */
        void setScale( double scale )               { dScale = scale; bSendUnchanged=false; };
        
        //! set a value to the member sendPars::eIceFormat defining the outgoing image format
        /** 
          Ice format is one of the following:
            - ICE_FORMAT_SH
            - ICE_FORMAT_FL
            - ICE_FORMAT_CXFL
            - ...
        */
        void setIceFormat( IceFormat_t eVal )   { eIceFormat = eVal; };
        
        //! set a value to the member sendPars::iEco (don't know why it's defined in this class)
        void setEco( int i )                    { iEco = i; bSendUnchanged=false; };
        
        //! print out values of all variable members
        std::string dump( void );
        
        //! initialize values of the corresponding variable members
        void init(   std::string Comment, 
                     std::string UID, 
                     std::string Process, 
                     std::string LUTFilename, 
                     ImageType_t   DicomImaType, 
                     std::string Suffix, 
                     IceExtractMode ExtractMode, 
                     double      Scale,
                     IceFormat_t IceFormat,
                     int         Eco,
                     bool        SendUnchanged );

        //! copy values of all variable members (except sUID) from those set in srcPars
        void copy( sendPars srcPars );

        friend std::ostream&  operator << ( std::ostream&, sendPars &s);
        friend std::string dumpIceFormat( IceFormat_t );
        friend std::string dumpImageType( ImageType_t );
        friend std::string dumpExtractMode( IceExtractMode );
        friend std::string dumpLifeTime( IPDS::lifetime );
        
        //! Constructor.
        sendPars();
        //! Destructor.
        ~sendPars() {};
};

//! ExtendedFunctor class can be used as a parent class to define and use sendData2NextFunctor() and storeImageInPARC() functions
/*!
  The ExtendedFunctor class defines two usefull functions that often need to be implemented in ICE reconstruction:
    - sendData2NextFunctor()
    - storeImageInPARC()
  
  The aim is to save some implementation time for when creating a new ICE Functor
  
  Additionnaly, it might be necessary to redefine the value for NumberInSeries of each image. In this case, a function called retrieveNumberInSeries must be implemented in the child class
  
  @note CLASS DEFINITION
        <BR>To define the new class that inherits from the ExtendedFunctor class, do as follow:
            - USE:          class  MyNewFunctor : public ExtendedFunctor
            - INSTEAD OF:   class ICEIDEAFUNCTORS_API MyNewFunctor : public IceImageReconFunctors
  @warning MACRO DEFINITION
        <BR>One must replace the macro definition as follow:
            - USE:          ICE_IMAGE_RECON_FUNCTOR_DERIVED(MyNewFunctor, ExtendedFunctor)
            - INSTEAD OF:   ICE_IMAGE_RECON_FUNCTOR(MyNewFunctor)
        
*/
class ICEIDEAFUNCTORS_API ExtendedFunctor : public IceImageReconFunctors
{

    public:     // public members

        ICE_IMAGE_RECON_FUNCTOR(ExtendedFunctor)

        // Functor functionality
        int     iNumberInSeries; // not used anymore
        
        // Define how is computed NumberInSeries for data sent to next functors
        // void    setComputeNumberInSeries(bool bValue) { bIsNumberInSeriesActicatedInX = bValue; };
        // bool    getComputeNumberInSeries() { return bIsNumberInSeriesActicatedInX; };
        
        //! return a value to be used for the NumberInSeries dicom tag
        /** 
          The NumberInSeries dicom tag is usefull to set a special order of the image stack in a given serie
          If the call of this function has been activated with setComputeNumberInSeries()
          retrieveNumberInSeries() is called for each image inside the function sendData2NextFunctor() and the return value overwrites the previous NumberInSeries value
          see also setComputeNumberInSeries(), getComputeNumberInSeries()
        */        
        virtual int retrieveNumberInSeries( ImageControl& , std::string ) { return 0; };
        
        // De/activate color coded images inside dicom information
        // void    setDebugMode(bool bValue) { bDebugModeInX = bValue; };
        // bool    getDebugMode();
        
        // De/activate color coded images inside dicom information
        // void    setShowMapInColor(bool bValue = false) { bShowMapInColorInX = bValue; };
        // bool    getShowMapInColor() { return bShowMapInColorInX; };
        
        //! build a char* Key to be used to retrieve an image that has been previously stored either in PARC or PDS
        /**
          This function is used in storeImageInPARC to set a key using storePars::sPrefix and IceLoopCounters.
          To retrieve an image from PARC or PDS:
            -# Define an instance of storePars with the correct properties
            -# build the Key using buildImageKey( char* Key, ICE_ID_NAME_LENGTH, IceLoopCounter, storePars )
            -# Call m_env as follow:
          
          \code
          IceObj::Pointer retievedObject ( Parc::HeapObject<IceObj>::Create() );
          retievedObject = Parc::cast<IceObj>( m_env->Item(ImRefID) );
          \endcode
        */
        IResult buildImageKey( char* Key, size_t uBufferLen, IceLoopCounters& loopCounter, storePars& cPars );
        IResult buildImageKey( char* Key, size_t uBufferLen, IceLoopCounters& loopCounter, const std::string );
        std::string buildImageKey( IceLoopCounters& loopCounter, const std::string );
        std::string buildImageKey( IceLoopCounters& loopCounter, storePars& cPars );
        
        // Method to store the incoming image with a label
        //! store an image using properties defined in an instance of storePars class. see storePars class for more details about properties
        IResult storeImageInPARC( IceAs& srcAs, ImageControl& ctrl, storePars& cPars );
        IResult storeImageInPARC( IceAs& srcAs, IceLoopCounters& lc, storePars& cPars );
        IResult storeImageInPARC( IceAs& srcAs, storePars& cPars );

        // Method to store the incoming header with a label
        //! store a miniheader using properties defined in an instance of storePars class. A MiniHeader is always saved in PDS and not PARC, whatever the storePars parameter setting. see storePars class for more details about properties
        IResult storeHeaderInPARC( MiniHeader::Pointer& dataHeader, ImageControl& ctrl, storePars& cPars );
        IResult storeHeaderInPARC( MiniHeader::Pointer& dataHeader, IceLoopCounters& lc, storePars& cPars );
        IResult storeHeaderInPARC( MiniHeader::Pointer& dataHeader, storePars& cPars );

        // Method to store the incoming image with a label
        //! store an image using properties defined in an instance of storePars class. see storePars class for more details about properties
        IResult storeDataInPARC( IceAs& srcAs, ImageControl& ctrl, storePars& cPars );
        IResult storeDataInPARC( IceAs& srcAs, IceLoopCounters& lc, storePars& cPars );
        IResult storeDataInPARC( IceAs& srcAs, storePars& cPars );

        // Method to store the incoming image with a label
        //! store a miniheader using properties defined in an instance of storePars class. A MiniHeader is always saved in PDS and not PARC, whatever the storePars parameter setting. see storePars class for more details about properties
        IResult storeDataInPARC( MiniHeader::Pointer& dataHeader, ImageControl& ctrl, storePars& cPars );
        IResult storeDataInPARC( MiniHeader::Pointer& dataHeader, IceLoopCounters& lc, storePars& cPars );
        IResult storeDataInPARC( MiniHeader::Pointer& dataHeader, storePars& cPars );
        
        // Method to retrieve an image with a label
        //! retrieve an image using properties defined in an instance of StorePars class. see storePars class for more details about properties
        IResult retrieveImageFromPARC( IceAs& dstAs, IceLoopCounters& lcIm, storePars& cPars );
        IResult retrieveImageFromPARC( IceAs& dstAs, storePars& cPars );

        // Method to retrieve a header with a label
        //! retrieve a header using properties defined in an instance of StorePars class. see storePars class for more details about properties
        IResult retrieveHeaderFromPARC( MiniHeader::Pointer& dataHeader, IceLoopCounters& lcIm, storePars& cPars );
        IResult retrieveHeaderFromPARC( MiniHeader::Pointer& dataHeader, storePars& cPars );
                
        // Method to cast a data object
        //! copy IceObject and cast it using properties defined in an instance of sendPars class. see sendPars class for more details about properties
        IResult castIceObject( IceAs& castAs, IceAs& srcAs, sendPars& cSend );
        
        // Method to modify MiniHeader
        //! copy MiniHeader and fill it using properties defined in an instance of sendPars class. see sendPars class for more details about properties
        MiniHeader::Pointer setMiniHeader( MiniHeader::Pointer& dataHeader, ImageControl& ctrl, sendPars& cPars);
        
        // Method to send data to the next functors
        //! send an image to next functor using properties defined in an instance of sendPars class. see sendPars class for more details about properties
        IResult sendData2NextFunctor( IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& ctrl, sendPars& cPars);
        //IResult sendData2NextFunctor( IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& ctrl, std::string, std::string, std::string, std::string, std::string, std::string, double scale );
        
        // Method to send data to Host
        //! send an image to Host using properties defined in an instance of sendPars class. see sendPars class for more details about properties
        IResult sendData2Host( IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& ctrl, sendPars& cPars);
        
        // Method to construct a cxfl from a fl As
        //! construct a complex object from a real float object. Imaginary part is set to 0. 
        bool ice_format_fl2cxfl( IceAs& dstAs, IceAs& srcAs, double scale = 1.0 );

        // Method to construct a cxfl from a sh As
        //! construct a complex object from a short object. Imaginary part is set to 0.
        bool ice_format_sh2cxfl( IceAs& dstAs, IceAs& srcAs, double scale = 1.0 );
        
        IResult ComputeImage( IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& controlData );
        
        // ParcDataStore
        IPDS::Pointer   m_pPDS;
        
        // Configuration of Functor (applied after PROPERTY_MAP is set):
        virtual IResult EndInit( IParcEnvironment* env );       
        // IResult EndInit( IParcEnvironment* env );
        
        // ICE numbering object
        ImageNumbering::Pointer m_ImageNumbering;

        bool    bDebugModeInX;
        bool    bShowMapInColorInX;     
        bool    bIsNumberInSeriesActicatedInX;
        
    protected:  // protected members
        
                
        struct storedElement
        {
            std::string     strTag;
            std::string     strType;
            IPDS::lifetime  eLifeTime;
            bool            bSavedPARC;
            bool            bSavedPDS;
            bool            bSuppressed;
            
            storedElement(std::string s1, std::string s2, IPDS::lifetime e3)
                :strTag(s1), strType(s2), eLifeTime(e3), bSavedPARC(false), bSavedPDS(true), bSuppressed(false) {};
            
            storedElement(std::string s1, std::string s2, IPDS::lifetime e3, bool b4, bool b5)
                :strTag(s1), strType(s2), eLifeTime(e3), bSavedPARC(b4), bSavedPDS(b5), bSuppressed(false) {};
        };
        
        std::vector<storedElement>  vsStorageList;
        
        //! print out list of saved objects
        std::string dumpStoredList( void );
        
        //! print a storedElement information
        std::string dumpStoredElement( storedElement& );
        
        IResult removeDataInPARC( void );
        
        // constructor / destructor
        ExtendedFunctor();
        ExtendedFunctor(bool b1, bool b2, bool b3);// Accessible to derived classes
        // Change "protected" to "public" to allow others to instantiate A.
        virtual ~ExtendedFunctor() {};

    private:    // private members

};

//! print out value of an enumarator object IceFormat_t
/**
  related functions: 
    - sendPars::setIceFormat( IceFormat_t )
    - storePars::setIceFormat( IceFormat_t )

  @note see also dumpImageType(), dumpExtractMode(), dumpLifeTime()
*/
std::string dumpIceFormat( IceFormat_t );
//! print out value of an enumarator object ImageType_t
/**
  related function: sendPars::setDicomImaType( ImageType_t )

  @note see also dumpIceFormat(), dumpExtractMode(), dumpLifeTime()
*/
std::string dumpImageType( ImageType_t );
//! print out value of an enumarator object IceExtractMode
/**
  related function: storePars::setExtractMode( IceExtractMode )

  @note see also dumpIceFormat(), dumpImageType(), dumpLifeTime()
*/
std::string dumpExtractMode( IceExtractMode );
//! print out value of an enumarator object IPDS::lifetime
/**
  related function: storePars::setLifeTime( IPDS::lifetime )

  @note see also dumpIceFormat(), dumpImageType(), dumpExtractMode()
*/
std::string dumpLifeTime( IPDS::lifetime );
//! extract phase image from complex source image and write it to destination image.
/**
  Apply a scaling factor to destination image.
*/
IResult extractPhase(IceAs&, IceAs&, double d = 1.0);


} // END OF NAMESPACE ICE_NAMESPACE

#endif
