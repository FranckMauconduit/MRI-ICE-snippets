//-----------------------------------------------------------------------------------
//-   Copyright (C) Siemens AG 2005  All Rights Reserved.
//-----------------------------------------------------------------------------------
//-
//-       Project: Extended Functor Base Functions
//-       Version: -
//-        Author: Franck MAUCONDUIT (franck.mauconduit@cea.fr)
//-          Site: Neurospin, CEA, Saclay, France
//-
//-          File: \n4\pkg\MrServers\MrVista\Ice\IceIdeaFunctors\IceQA\XFunctor.cpp
//-          Date: Tue Mar  5 19:47:10 2019
//-      Language: C++
//-
//-   Description: <Class to extend functor with some basic functions usually used in any functor.>
//-
//-----------------------------------------------------------------------------------

// ExtendedFunctor class
#include "XFunctor.h"

using namespace std;

namespace ICE_NAMESPACE
{

ExtendedFunctor::ExtendedFunctor(bool b1, bool b2, bool b3)
    : bDebugModeInX(b1)
    , bShowMapInColorInX(b2)
    , bIsNumberInSeriesActicatedInX(b3)
{
}

ExtendedFunctor::ExtendedFunctor()
    : bDebugModeInX(false)
    , bShowMapInColorInX(true)
    , bIsNumberInSeriesActicatedInX(false)
{
    ICE_SET_FN("ExtendedFunctor::ExtendedFunctor()")
    // ICE_OUT("Extented Functor in use, constructor called.");
}


//-----------------------------------------------------------------------------
// buildImageKey(char*, size_t, IceLoopCounters&, storePars&)
// Description: method to build a unique label to identify the incoming image
//      inside the PARC environment.
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::buildImageKey(
        char*               pszBuffer,
        size_t              uBufferLen, 
        IceLoopCounters&    loopCounter, 
        storePars&          cStore      // Storage data parameters
        )
{
    ICE_SET_FN("ExtendedFunctor::buildImageKey")
        
    // Initialize the final name
    pszBuffer[0] = '\0';
    strncpy(pszBuffer,cStore.sPrefix.c_str(),uBufferLen);
    pszBuffer[uBufferLen-1] = '\0';

    // Appends IceDimString to the label:
    if( loopCounter.returnIceDimsAsString((uBufferLen - 1),pszBuffer) == -1 )
    {ICE_RET_ERR("IceLoopCounters::returnIceDimsAsString(...) failed, aborting ...", I_FAIL)}
    
    return I_OK;
}

//-----------------------------------------------------------------------------
// buildImageKey(char*, size_t, IceLoopCounters&, const string)
// Description: method to build a unique label to identify the incoming image
//      inside the PARC environment.
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::buildImageKey(
        char* pszBuffer,
        size_t uBufferLen, 
        IceLoopCounters& loopCounter, 
        const string prefix            )
{
    ICE_SET_FN("ExtendedFunctor::buildImageKey(char*, size_t, IceLoopCounters&)")

    // Initialize the final name
    pszBuffer[0] = '\0';
    strncpy(pszBuffer,prefix.c_str(),uBufferLen);
    pszBuffer[uBufferLen-1] = '\0';

    // Appends IceDimString to the label:
    if( loopCounter.returnIceDimsAsString((uBufferLen - 1),pszBuffer) == -1 )
    {ICE_RET_ERR("IceLoopCounters::returnIceDimsAsString(...) failed, aborting ...", I_FAIL)}

    return I_OK;
}

//-----------------------------------------------------------------------------
// buildImageKey( IceLoopCounters&, const string )
// Description: method to build a unique label to identify the incoming image
//      inside the PARC environment.
//-----------------------------------------------------------------------------

string ExtendedFunctor::buildImageKey(
        IceLoopCounters& loopCounter, 
        const string prefix            )
{
    ICE_SET_FN("ExtendedFunctor::buildImageKey(IceLoopCounters, string)")

    // Initialize the final name
    string     outString;
    char            pszBuffer[ICE_ID_NAME_LENGTH];
    
    // Construct Buffer
    strncpy( pszBuffer, prefix.c_str(), ICE_ID_NAME_LENGTH );
    pszBuffer[ICE_ID_NAME_LENGTH-1] = '\0';

    // Appends IceDimString to the label:
    if( loopCounter.returnIceDimsAsString((ICE_ID_NAME_LENGTH - 1),pszBuffer) == -1 )
    {ICE_ERR("IceLoopCounters::returnIceDimsAsString(...) failed, continue anyway ...");}
    
    outString.assign(pszBuffer);
    return outString;
}

//-----------------------------------------------------------------------------
// buildImageKey(char*, size_t, IceLoopCounters&, storePars&)
// Description: method to build a unique label to identify the incoming image
//      inside the PARC environment.
//-----------------------------------------------------------------------------


string ExtendedFunctor::buildImageKey(
        IceLoopCounters&    loopCounter, 
        storePars&          cStore      // Storage data parameters
        )
{
    ICE_SET_FN("ExtendedFunctor::buildImageKey(IceLoopCounters, storePars)")
    
    // Initialize the final name
    string     outString;
    char            pszBuffer[ICE_ID_NAME_LENGTH];
    
    // Construct Buffer
    strncpy(pszBuffer, cStore.sPrefix.c_str(), ICE_ID_NAME_LENGTH );
    pszBuffer[ICE_ID_NAME_LENGTH-1] = '\0';

    // Appends IceDimString to the label:
    if( loopCounter.returnIceDimsAsString((ICE_ID_NAME_LENGTH - 1),pszBuffer) == -1 )
    {ICE_ERR("IceLoopCounters::returnIceDimsAsString(...) failed, continue anyway ...");}
    
    outString.assign(pszBuffer);
    return outString;

}


//-----------------------------------------------------------------------------
// storeDataInPARC(...)
// Arguments :  IceAs&
//              ImageControl& or IceLoopCounters& or NONE
//              storePars&
// Description: Call storeImageInPARC
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::storeDataInPARC( IceAs& srcAs, ImageControl& ctrl, storePars& cStore )
{return storeImageInPARC( srcAs, ctrl.m_lc, cStore );}

IResult ExtendedFunctor::storeDataInPARC( IceAs& srcAs, IceLoopCounters& lc, storePars& cStore )
{return storeImageInPARC( srcAs, lc, cStore );}

IResult ExtendedFunctor::storeDataInPARC( IceAs& srcAs, storePars& cStore )
{return storeImageInPARC( srcAs, cStore );}


//-----------------------------------------------------------------------------
// storeDataInPARC(...)
// Arguments :  MiniHeader::Pointer&
//              ImageControl& or IceLoopCounters& or NONE
//              storePars&
// Description: Call storeHeaderInPARC
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::storeDataInPARC( MiniHeader::Pointer& dataHeader, ImageControl& ctrl, storePars& cStore )
{return storeHeaderInPARC( dataHeader, ctrl.m_lc, cStore );}

IResult ExtendedFunctor::storeDataInPARC( MiniHeader::Pointer& dataHeader, IceLoopCounters& lc, storePars& cStore )
{return storeHeaderInPARC( dataHeader, lc, cStore );}

IResult ExtendedFunctor::storeDataInPARC( MiniHeader::Pointer& dataHeader, storePars& cStore )
{return storeHeaderInPARC( dataHeader, cStore );}


//-----------------------------------------------------------------------------
// storeHeaderInPARC(...)
// Arguments :  MiniHeader::Pointer&
//              ImageControl& or IceLoopCounters& or NONE
//              storePars&
// Description:  method to store the passed argument MiniHeader inside the PARC
//          environment. A source object can be passed since a copy
//          is created before sending within the PARC.
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::storeHeaderInPARC( MiniHeader::Pointer& dataHeader, ImageControl& ctrl, storePars& cStore )
{return storeHeaderInPARC( dataHeader, ctrl.m_lc, cStore );}

IResult ExtendedFunctor::storeHeaderInPARC( MiniHeader::Pointer& dataHeader, storePars& cStore )
{
    ICE_SET_FN("ExtendedFunctor::storeHeaderInPARC(MiniHeader::Pointer,storePars)")
    
    if ( !cStore.m_bloopcounterSet )
    {ICE_RET_ERR("None of the loopcounter value in storePars was set, aborting...",I_FAIL);}
    
    return storeHeaderInPARC( dataHeader, cStore.m_loopcounter, cStore );
}

IResult ExtendedFunctor::storeHeaderInPARC(
        MiniHeader::Pointer&    dataHeader, // MiniHeader
        IceLoopCounters&        lc,         // Image loopcounter
        storePars&              cStore      // Storage data parameters
        )
{
    ICE_SET_FN("ExtendedFunctor::storeHeaderInPARC")
    
    // Reset loopcounter usage
    cStore.m_bloopcounterSet = false;
    
    // Initialize local variables
    char    headerID[ICE_ID_NAME_LENGTH] = {'\0'};
    IResult Res;
    
    // Build image key for miniheader to be stored
    Res = buildImageKey( headerID, ICE_ID_NAME_LENGTH, lc, cStore );
    if ( failed( Res ) )
        ICE_RET_ERR("Error calling buildImageKey(...)\n", Res )    
       
    if ( bDebugModeInX )
    {ICE_OUT("\nMiniHeader Key has been built with ID : " << headerID << ".\n");}

    // Create MiniHeader for storing header in environment, since we have smart pointer
    // Avoid multi-threading bug over pipe services by creating clone object
    MiniHeader::Pointer copyHeader = dataHeader->deepCopy();
        
    // ----------------------
    // save miniheader in PDS
    
    // Check if header already exists in PDS
    bool bContains = m_pPDS->Contains(headerID, cStore.eLifeTime);
    
    if ( bDebugModeInX )
    {ICE_OUT("PARC DATA STORE: MiniHeader with name " << headerID << " already present in PDS ? " << (bContains?"true":"false") << "\n"
        "Whether exist or not, trying to add image to PDS !" );}
        
    if ( bContains )
    {
        // Remove image in PDS before saving again
        Res = m_pPDS->Remove(headerID, cStore.eLifeTime);
        if (failed(Res))
        {ICE_RET_ERR("Failed to remove MiniHeader in PDS, aborting...\n", Res);}
    }
    
    // Save image in PDS with a lifetime of the current study
    Res = m_pPDS->Add(headerID, copyHeader, cStore.eLifeTime);
    if (failed(Res))
    {
        // In case of error dump all entrees in PDS to log file to ease trouble shooting
        // ICE_OUT(m_pPDS->print());
        ICE_RET_ERR("Failed to store MiniHeader in PDS, aborting...\n", Res);
    }
    
    if ( bDebugModeInX )
    {ICE_OUT("\nMiniHeader ImageKey to be saved in PDS: " << headerID);}
    
    vsStorageList.push_back(storedElement(headerID,"MiniHeader",cStore.eLifeTime));
    
    return I_OK;
}


//-----------------------------------------------------------------------------
// storeImageInPARC(...)
// Arguments :  IceAs&
//              ImageControl& or IceLoopCounters& or NONE
//              storePars&
// Description: method to store the passed argument image inside the PARC
//          environment. A source object can be passed since a copy
//          is created before sending within the PARC.
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::storeImageInPARC( IceAs& srcAs, ImageControl& ctrl, storePars& cStore )
{return storeImageInPARC( srcAs, ctrl.m_lc, cStore );}

IResult ExtendedFunctor::storeImageInPARC( IceAs& srcAs, storePars& cStore )
{
    ICE_SET_FN("ExtendedFunctor::storeImageInPARC(IceAs,storePars)")
    
    if ( !cStore.m_bloopcounterSet )
    {ICE_RET_ERR("None of the loopcounter value in storePars was set, aborting...",I_FAIL);}
    
    return storeImageInPARC( srcAs, cStore.m_loopcounter, cStore );
}

IResult ExtendedFunctor::storeImageInPARC(
        IceAs&              srcAs,      // Data Access Specifier
        IceLoopCounters&    lc,         // Data loop counter
        storePars&          cStore      // Storage data parameters
        )
{
    ICE_SET_FN("ExtendedFunctor::storeImageInPARC(IceAs,IceLoopCounters,storePars)")
    
    cStore.m_bloopcounterSet = false;
    
    // Initialize local variables
    char    imageTag[ICE_ID_NAME_LENGTH] = {'\0'};
    IResult Res;
    bool    bCopyBeforeStorage;
    
    // Build image key for data to be stored
    Res = buildImageKey( imageTag, ICE_ID_NAME_LENGTH, lc, cStore );
    if ( failed( Res ) )
    {ICE_RET_ERR("Error calling buildImageKey(...)\n", Res )}
    
    if ( bDebugModeInX )
    {ICE_OUT("\nImage Key has been built with ID : " << imageTag << ".\n");}
    
    // Create IceObj for storing image in environment, since we have smart pointer
    // Avoid multi-threading bug over pipe services by creating clone object
    IceObj::Pointer pStoredImage(Parc::HeapObject<IceObj>::Create());
    
    // Make sure to create a dataset of size as object pointed by srcAs
    IceAs StoredImageAs;

    // A new object will be used if the object type to be saved is different from src object
    //  or if data is saved in PARC (seen issues in this case)
    bCopyBeforeStorage  = (srcAs.getIceObjFormat() != cStore.eIceFormat) || cStore.bSaveInPARC;
    
    if ( bCopyBeforeStorage )
    {
        // Create an object matching srcAs
        if ( !pStoredImage->create( cStore.eIceFormat, srcAs) )
            ICE_RET_ERR("Error in ExtendedFunctor::storeImageInPARC\n" <<
                        "Failed in creating access specifier for pStoredImage with following ID and format type: \n" << 
                        imageTag << "\n" << dumpIceFormat(cStore.eIceFormat) << "\n", I_FAIL)
        StoredImageAs = (*pStoredImage)();
        Ice.SDC.calcSoda(StoredImageAs, srcAs);
    }
    else
    {
        pStoredImage  = srcAs.getObj();
        StoredImageAs = (*srcAs.getObj())();
    }
    
    // ---------------------------------------------------------- //
    // Copy source data to be saved in different IceFormat cases  //

    // switch case on incoming data type
    switch(srcAs.getIceObjFormat())
    {
        // incoming data is short
        case ICE_FORMAT_SH :
        
                // switch case on IceFormat image to be saved
                switch(cStore.eIceFormat)
                {
                    case ICE_FORMAT_SH :
                        // using original src object or a copy
                        if ( bCopyBeforeStorage )
                        {Ice.Copy(StoredImageAs,srcAs);}
                        break;
                    
                    case ICE_FORMAT_FL :
                        if ( !Ice.Copy(StoredImageAs, srcAs) )
                        {ICE_RET_ERR("Failed during Copy of image with following ID: "  << imageTag << "\n", I_FAIL)}
                        break;
                        
                    case ICE_FORMAT_CXFL :
                        if ( !ice_format_sh2cxfl(StoredImageAs, srcAs) )
                        {ICE_RET_ERR("Failed during ice_format_sh2cxfl of image with following ID: "  << imageTag << "\n", I_FAIL)}
                        break;
                        
                    default :
                        ICE_RET_ERR("\nError in ExtendedFunctor::storeImageInPARC\n"
                                    "Incoming data IceFormat is: " << dumpIceFormat(srcAs.getIceObjFormat()) << "\n"
                                    "To be saved data IceFormat is: " << dumpIceFormat(cStore.eIceFormat) << "\n"
                                    "This case has not been implemented, data not saved, function Abort\n", I_FAIL);
                }
                
            break;
        
        // incoming data is float
        case ICE_FORMAT_FL :
        
                // switch case on IceFormat image to be saved
                switch(cStore.eIceFormat)
                {
                    case ICE_FORMAT_SH :
                        if ( !Ice.Copy(StoredImageAs, srcAs) )
                        {ICE_RET_ERR("Failed during Copy of image with following ID: "  << imageTag << "\n", I_FAIL)}
                        break;
                        
                    case ICE_FORMAT_FL :
                        // using original src object or a copy
                        if ( bCopyBeforeStorage )
                        {Ice.Copy(StoredImageAs,srcAs);}
                        break;
                        
                    case ICE_FORMAT_CXFL :
                        if ( !ice_format_fl2cxfl(StoredImageAs, srcAs) )
                        {ICE_RET_ERR("Failed during ice_format_fl2cxfl of image with following ID: "  << imageTag << "\n", I_FAIL)}
                        break;
                        
                    default :
                        ICE_RET_ERR("\nError in ExtendedFunctor::storeImageInPARC\n"
                                    "Incoming data IceFormat is: " << dumpIceFormat(srcAs.getIceObjFormat()) << "\n"
                                    "To be saved data IceFormat is: " << dumpIceFormat(cStore.eIceFormat) << "\n"
                                    "This case has not been implemented, data not saved, function Abort\n", I_FAIL);
                }
                
            break;

        // incoming data is complex float
        case ICE_FORMAT_CXFL :
        
                // switch case on IceFormat image to be saved
                switch(cStore.eIceFormat)
                {
                    // to be saved data is short | should not be used
                    // case ICE_FORMAT_SH :
                        // if ( !Ice.Copy(StoredImageAs, srcAs) )
                        // {ICE_RET_ERR("Failed during Copy of image with following ID: "  << imageTag << "\n", I_FAIL)}
                        // break;
                        
                    // to be saved data is float
                    case ICE_FORMAT_FL :
                        // Extract module from incoming Access Specifier
                        // Should use eExtractMode instead of IemAmplitude
                        if ( ! Ice.ExtractComplex( StoredImageAs, srcAs, IemAmplitude, 1.0 ) )
                        ICE_RET_ERR("Ice.ExtractComplex(StoredImageAs, srcAs, IemAmplitude) failed, aborting...", I_FAIL)
                        break;
                        
                    case ICE_FORMAT_CXFL :
                        // using original src object or a copy
                        if ( bCopyBeforeStorage )
                        {Ice.Copy(StoredImageAs,srcAs);}
                        break;
                        
                    default :
                        ICE_RET_ERR("\nError in ExtendedFunctor::storeImageInPARC\n"
                                    "Incoming data IceFormat is: " << dumpIceFormat(srcAs.getIceObjFormat()) << "\n"
                                    "To be saved data IceFormat is: " << dumpIceFormat(cStore.eIceFormat) << "\n"
                                    "This case has not been implemented, data not saved, function Abort\n", I_FAIL);
                }

            break;
                
        // incoming data is something else
        default :
            ICE_RET_ERR("\nError in ExtendedFunctor::storeImageInPARC\n"
                    "Incoming data IceFormat is: " << dumpIceFormat(srcAs.getIceObjFormat()) << "\n"
                    "Format not yet handled.", I_FAIL);
    }
    
    
    
    // ----------------
    // save data in PDS
    
    if ( cStore.bSaveInPDS )
    {
        // Check if image already exists in PDS
        // bool bContains = false;
        // ICE_OUT( cStore.dump() );
        bool bContains = m_pPDS->Contains(imageTag, cStore.eLifeTime);
        
        if ( bDebugModeInX )
        {ICE_OUT("PARC DATA STORE: Image with name " << imageTag << " already present in PDS ? " << (bContains?"true":"false") << "\n"
            "Whether exist or not, trying to add image to PDS !" );}
            
        if ( bContains )
        {
            // Remove image in PDS before saving again
            Res = m_pPDS->Remove(imageTag, cStore.eLifeTime);
            if (failed(Res))
            {ICE_RET_ERR("Failed to remove IceObj in PDS, aborting...\n", Res);}
        }
        
        // Save image in PDS with a lifetime of the current study
        Res = m_pPDS->Add(imageTag, pStoredImage, cStore.eLifeTime);
        if (failed(Res))
        {
            // In case of error dump all entrees in PDS to log file to ease trouble shooting
            // ICE_OUT(m_pPDS->print());
            ICE_RET_ERR("Failed to store IceObj in PDS, aborting...\n", Res);
        }
        
        if ( bDebugModeInX )
        {ICE_OUT("\nImage ImageKey to be saved in PDS: " << imageTag);}
    }

    // -----------------
    // save data in PARC
    
    if  ( cStore.bSaveInPARC )
    {
        // Register object within the Parc environment for later use:
        Res = m_env->Add(imageTag, pStoredImage);
        if ( failed(Res) )
        {
            ICE_OUT("Image tag is: "<< imageTag);
            ICE_OUT( cStore.dump() );
            // In case of error dump all entrees in Parc environment to log file to ease trouble shooting
            m_env->ListContent();
            ICE_RET_ERR("Error in ExtendedFunctor::storeImageInPARC\n" <<
                        "Failed adding pStoredImage to Parc environment.", Res)
        }
    }

    if ( bDebugModeInX )
    {ICE_OUT("\nImage saved in PARC environment. ID: " << imageTag << ".\n");}
    
    vsStorageList.push_back(storedElement(imageTag,"IceObj",cStore.eLifeTime,cStore.bSaveInPARC,cStore.bSaveInPDS));
    
    return I_OK;
}


//-----------------------------------------------------------------------------
// retrieveImageFromPARC(IceAs&, storePars&)
// retrieveImageFromPARC(IceAs&, LoopCounter&, storePars&)
// Description: retrieve image 
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::retrieveImageFromPARC(
        IceAs&                  dstAs,
        storePars&              cStore
        )
{
    ICE_SET_FN("ExtendedFunctor::retrieveImageFromPARC")
        
    if ( !cStore.m_bloopcounterSet )
    {ICE_RET_ERR("None of the loopcounter value in storePars was set, aborting...",I_FAIL);}
    
    return retrieveImageFromPARC(dstAs, cStore.m_loopcounter, cStore);
}


IResult ExtendedFunctor::retrieveImageFromPARC(
        IceAs&                  dstAs,
        IceLoopCounters&        lc,
        storePars&              cStore
        )
{
    ICE_SET_FN("ExtendedFunctor::retrieveImageFromPARC")
    
    cStore.m_bloopcounterSet = false;
    
    //Initialize local variables
    IResult         Res;
    char            imageID[ICE_ID_NAME_LENGTH] = {'\0'};
    
    // Build ID name from lcIm and prefix information
    Res = buildImageKey( imageID, ICE_ID_NAME_LENGTH, lc, cStore.sPrefix.c_str() );
    if ( failed( Res ) )
    {ICE_RET_ERR("Failed while creating image KEY:" << imageID << "\n", Res );}
    
    IceObj::Pointer ImObj = Parc::HeapObject<IceObj>::Create();
    
    
    if ( cStore.bSaveInPDS )
    {
        // Retrieve image from PDS
        m_pPDS->Item( imageID, ImObj, cStore.eLifeTime );

        // Send an error if not object
        if( ImObj==NULL || !ImObj->isValid() )
        {
            ICE_OUT(m_pPDS->print());
            ICE_RET_ERR("No image object could be retrieved from PDS while trying to retrieve image with ID: "
                    << imageID << ", aborting... ", I_FAIL);
        }
    }
    else if ( cStore.bSaveInPARC )
    {
        
        // Retrieve image from PARC environment
        ImObj = Parc::cast<IceObj>( m_env->Item(imageID) );
        
        // Send an error if not object
        if( ImObj==NULL || !ImObj->isValid() )
        {
            m_env->ListContent();
            ICE_RET_ERR("No image object could be retrieved from PARC environment while trying to retrieve image with ID: "
                    << imageID << ", aborting... ", I_FAIL);
        }
    }
    else
    {
        ICE_RET_ERR("storePars is set to save neither in PDS nor in PARC, aborting... ",I_FAIL);
    }

    dstAs = ( *ImObj )();
    
    return I_OK;
}


//-----------------------------------------------------------------------------
// retrieveHeaderFromPARC(MiniHeader::Pointer&, storePars&)
// retrieveHeaderFromPARC(MiniHeader::Pointer&, IceLoopCounter&, storePars&)
// Description: retrieve header 
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::retrieveHeaderFromPARC(
        MiniHeader::Pointer&    dataHeader,
        storePars&              cStore
        )
{
    ICE_SET_FN("ExtendedFunctor::retrieveHeaderFromPARC")
    
    if ( !cStore.m_bloopcounterSet )
    {ICE_RET_ERR("None of the loopcounter value in storePars was set, aborting...",I_FAIL);}
    
    return retrieveHeaderFromPARC(dataHeader, cStore.m_loopcounter, cStore);
}


IResult ExtendedFunctor::retrieveHeaderFromPARC(
        MiniHeader::Pointer&    dataHeader,
        IceLoopCounters&        lc,
        storePars&              cStore
        )
{
    ICE_SET_FN("ExtendedFunctor::retrieveHeaderFromPARC")
    
    cStore.m_bloopcounterSet = false;
    
    //Initialize local variables
    IResult         Res;
    char            headerID[ICE_ID_NAME_LENGTH] = {'\0'};
    
    if ( dataHeader == NULL )
    {ICE_RET_ERR("Incoming MiniHeader should not be a null pointer, aborting...",I_FAIL);}
    // Comment : in the function calling retrieveHeaderFromPARC()
    // either make a deepCopy to have a new MiniHeader instance : MiniHeader::Pointer dataHeader = srcHeader->deepCopy();
    // or copy the pointer itself that will be overwritten      : MiniHeader::Pointer dataHeader(srcHeader);
    
    // Build ID name from lcIm and prefix information
    Res = buildImageKey( headerID, ICE_ID_NAME_LENGTH, lc, cStore.sPrefix.c_str() );
    if ( failed( Res ) )
    {ICE_RET_ERR("Failed while creating header KEY:" << headerID << "\n", Res );}
    
    // Retrieve header from PDS : dataHeader 
    Res = m_pPDS->Item( headerID, dataHeader, cStore.eLifeTime );
    if ( failed( Res ) )
    {ICE_RET_ERR("No header object could be retrieved from PDS with ID: " << headerID << ", aborting... ", I_FAIL);}
    
    return I_OK;
}


//-----------------------------------------------------------------------------
// castIceObject(IceAs&, sendPars&)
// Description: cast ice object using parameter infos.
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::castIceObject(
        IceAs&                  castAs,     // Access Specifier to be cast
        IceAs&                  srcAs,      // Original Access Specifier
        sendPars&               cSend       // Send parameters
        )
{
    ICE_SET_FN("ExtendedFunctor::castIceObject")
    
    // Duplicate srcAs before call of next functor (to avoid overriding because of multi-threading)
    IceObj::Pointer sendObj( Parc::HeapObject<IceObj>::Create() );
    if( ! sendObj->create( cSend.eIceFormat, srcAs ) )
        ICE_RET_ERR("Failed during sendObj->create(srcAs), aborting...", I_FAIL)
    castAs = (*sendObj)();
    Ice.SDC.calcSoda(castAs, srcAs);
    
    IceAs copyAs;
    
    // ---------------------------------------------------------- //
    // Copy source data to send data in different IceFormat cases //
    
    // switch case on incoming data type
    switch(srcAs.getIceObjFormat())
    {
        // incoming data is short
        case ICE_FORMAT_SH :
        
                // switch case on outgoing data type
                switch( cSend.eIceFormat )
                {

                    case ICE_FORMAT_SH :
                        if ( ! Ice.Mul( castAs, srcAs, cSend.dScale ) )
                        ICE_RET_ERR("Ice.Mul(castAs, srcAs, scale) failed, aborting...", I_FAIL)        
                        break;
                    case ICE_FORMAT_FL :
                        // Ice.Mul( srcAs, srcAs, cSend.dScale )
                        // ICE_RET_ERR("Scale not equal to 1 and IceFormat different for incoming and outgoing data. This case is not yet implemented", I_FAIL)
                        if ( ! Ice.Copy( castAs, srcAs) )
                        ICE_RET_ERR("Ice.Copy(castAs, srcAs) failed, aborting...", I_FAIL)
                        if ( cSend.dScale != 1.0 )
                        {
                            if ( ! Ice.Mul( castAs, castAs, cSend.dScale ) )
                            ICE_RET_ERR("Ice.Mul(castAs, castAs, scale) failed, aborting...", I_FAIL)
                        }
                        break;
                    case ICE_FORMAT_CXFL :
                        // if ( ! Ice.ExtractComplex( castAs, srcAs, IemInvalid, 1.0 ) ) // does not work to construct CXFL
                        // ICE_RET_ERR("Ice.ExtractComplex(castAs, srcAs, IemAmplitude) failed, aborting...", I_FAIL)
                        if ( ! ice_format_sh2cxfl( castAs, srcAs, cSend.dScale ) )
                        ICE_RET_ERR("ice_format_sh2cxfl(castAs, srcAs, scale) failed, aborting...", I_FAIL)
                        break;                        
                    default :
                        ICE_RET_ERR("\nExtendedFunctor::castIceObject\n"
                                "Incoming data IceFormat is: " << dumpIceFormat(srcAs.getIceObjFormat()) << "\n"
                                "Outgoing data IceFormat is: " << dumpIceFormat(cSend.eIceFormat) << "\n"
                                "This case has not been implemented, data not sent to next functor, function Abort\n", I_FAIL);
                }
        
            break;
        
        // incoming data is float
        case ICE_FORMAT_FL :
        
                // switch case on outgoing data type
                switch( cSend.eIceFormat )
                {
                    case ICE_FORMAT_SH :
                    
                        if ( cSend.dScale != 1.0 )
                        {
                            // Duplicate srcAs to be able to scale
                            IceObj::Pointer copyObj( Parc::HeapObject<IceObj>::Create() );
                            if( ! copyObj->create( srcAs ) )
                                ICE_RET_ERR("Failed during sendObj->create(srcAs), aborting...", I_FAIL)
                            copyAs = (*copyObj)();
                            Ice.SDC.calcSoda(copyAs, srcAs);
                    
                            if ( ! Ice.Mul( copyAs, srcAs, cSend.dScale ) )
                            ICE_RET_ERR("Ice.Mul(copyAs, srcAs, scale) failed, aborting...", I_FAIL)
                        }
                        
                        if ( ! Ice.Copy( castAs, (cSend.dScale==1.0?srcAs:copyAs) ) )
                        ICE_RET_ERR("Ice.Copy(castAs, srcAs) failed, aborting...", I_FAIL)
                        break;
                    case ICE_FORMAT_FL :
                        if ( ! Ice.Mul( castAs, srcAs, cSend.dScale ) )
                        ICE_RET_ERR("Ice.Mul(castAs, srcAs, scale) failed, aborting...", I_FAIL)        
                        break;
                    case ICE_FORMAT_CXFL :
                        // if ( ! Ice.ExtractComplex( castAs, srcAs, IemInvalid, 1.0 ) ) // does not work to construct CXFL
                        // ICE_RET_ERR("Ice.ExtractComplex(castAs, srcAs, IemAmplitude) failed, aborting...", I_FAIL)
                        if ( ! ice_format_fl2cxfl( castAs, srcAs, cSend.dScale ) )
                        ICE_RET_ERR("ice_format_fl2cxfl(castAs, srcAs, scale) failed, aborting...", I_FAIL)
                        break;
                    default :
                        ICE_RET_ERR("\nExtendedFunctor::castIceObject\n"
                                "Incoming data IceFormat is: " << dumpIceFormat(srcAs.getIceObjFormat()) << "\n"
                                "Outgoing data IceFormat is: " << dumpIceFormat(cSend.eIceFormat) << "\n"
                                "This case has not been implemented, data not sent to next functor, function Abort\n", I_FAIL);
                }
                
            break;
        
        // incoming data is complex
        case ICE_FORMAT_CXFL :

                // switch case on outgoing data type
                switch( cSend.eIceFormat )
                {
                    case ICE_FORMAT_SH :
                    case ICE_FORMAT_FL :
                    
                            // switch case on complex extraction type (M or P)
                            switch( cSend.eExtractMode )
                            {
                                case IemAmplitude :
                                // case IemPhase : 
                                    //ICE_OUT("Ice.ExtractComplex(castAs, srcAs, "<<cSend.eExtractMode<<", "<<cSend.dScale);
                                    if ( ! Ice.ExtractComplex( castAs, srcAs, cSend.eExtractMode, cSend.dScale ) )
                                    ICE_RET_ERR("Ice.ExtractComplex(castAs, srcAs, "<<cSend.eExtractMode<<"(IemAmplitude), "<<cSend.dScale<<") failed, aborting...\n"
                                                "Dump of sendPars instance: " << cSend.dump() << "\n"
                                                "srcAs:"<<srcAs<<
                                                "castAs:"<<castAs
                                                , I_FAIL)
                                    //IceUtils::CheckAndDisplay( srcAs, IceDisplayData::DISPLAY, " - Incoming" );
                                    //IceUtils::CheckAndDisplay( castAs, IceDisplayData::DISPLAY, " - to be sent" );
                                    break;
                                 
                                case IemPhase :
                                    if ( ! Ice.ExtractComplex( castAs, srcAs, cSend.eExtractMode, cSend.dScale ) )
                                    ICE_RET_ERR("Ice.ExtractComplex(castAs, srcAs, "<<cSend.eExtractMode<<"(IemPhase), "<<cSend.dScale<<") failed, aborting...\n"
                                                "Dump of sendPars instance: " << cSend.dump() << "\n"
                                                "srcAs:"<<srcAs<<
                                                "castAs:"<<castAs
                                                , I_FAIL)
                                    // Ice.ExtractComplex( castAs, srcAs, cSend.eExtractMode, cSend.dScale );
                                    // Ice.Add(castAs, castAs, -3.1415926);
                                    // Ice.Mul(castAs, castAs, cSend.dScale);
                                    // IceUtils::CheckAndDisplay( castAs, IceDisplayData::DISPLAY, "extractPhase" );
                                    // extractPhase( castAs, srcAs, cSend.dScale );
                                    // ICE_RET_ERR("extractPhase(castAs, srcAs, scale) failed, aborting...\nDump of sendPars instance: " << cSend.dump() << "\n", I_FAIL)   
                                    break;
                                
                                default :
                                    ICE_RET_ERR("\nCase not implemented\nIce.ExtractComplex(castAs, srcAs, "<<cSend.eExtractMode<<", "<<cSend.dScale<<") failed, aborting...", I_FAIL)
                                    // ICE_RET_ERR("\nExtendedFunctor::sendData2NextFunctor\n"
                                            // "Incoming data IceFormat is: " << dumpIceFormat(srcAs.getIceObjFormat()) << "\n"
                                            // "Outgoing data IceFormat is: " << dumpIceFormat(cSend.eIceFormat) << "\n"
                                            // "Dump of sendPars instance: " << cSend.dump() << "\n"
                                            // "This case has not been implemented, data not sent to next functor, function Abort\n", I_FAIL);
                            }
                    
                        break;
                        
                    case ICE_FORMAT_CXFL :
                        if ( cSend.dScale != 1.0 ) {                        
                            if ( ! Ice.cMul( castAs, srcAs, cSend.dScale, 0.0 ) )
                            ICE_RET_ERR("Ice.cMul(castAs, srcAs, real, 0.0) failed, aborting...", I_FAIL)
                        }
                        else {
                            if ( ! Ice.Copy( castAs, srcAs) )
                            ICE_RET_ERR("Ice.Copy(castAs, srcAs) failed, aborting...", I_FAIL)
                        }
                        break;

                    default :
                        ICE_RET_ERR("\nExtendedFunctor::castIceObject\n"
                                "Incoming data IceFormat is: " << dumpIceFormat(srcAs.getIceObjFormat()) << "\n"
                                "Outgoing data IceFormat is: " << dumpIceFormat(cSend.eIceFormat) << "\n"
                                "This case has not been implemented, data not sent to next functor, function Abort\n", I_FAIL);
                }
        
            break;
        
        default :
            ICE_RET_ERR("\nExtendedFunctor::castIceObject\n"
                    "Incoming data IceFormat is: " << dumpIceFormat(srcAs.getIceObjFormat()) << "\n"
                    "Format not yet handled.", I_FAIL);
    }

    castAs = (*sendObj)();
    
    return I_OK;
}




//-----------------------------------------------------------------------------
// setMiniHeader(IceAs&, ImageControl&, sendPars&)
// Description: copy MiniHeader and change header info according to sendPars
//-----------------------------------------------------------------------------

MiniHeader::Pointer ExtendedFunctor::setMiniHeader(
        MiniHeader::Pointer&    srcHeader, // Data MiniHeader
        ImageControl&           ctrl,       // Data image Control
        sendPars&               cSend      // Outgoing data parameters
        )
{
    ICE_SET_FN("ExtendedFunctor::setMiniHeader")
    
    // Initialize local variables
    string strSerieName;

    // ---------------------------------------------------------------- //
    // Modify MiniHeader according to information set in sendPars class //

    // Copy incoming MiniHeader
    MiniHeader::Pointer sendHeader = srcHeader->deepCopy();
    
    // Check whether outgoing data miniheader must be modified
    if ( cSend.bSendUnchanged != 1 )
    {
        // Miniheader - Define an Image Comment
        if ( ! cSend.sComment.empty() )
        sendHeader->set("ImageComment", cSend.sComment );
        
        // Miniheader - Append a suffix to the serie name
        if ( ! cSend.sSuffix.empty() )
        {
            sendHeader->get("SequenceDescription", strSerieName);
            strSerieName.append(cSend.sSuffix);
            sendHeader->set("SequenceDescription", strSerieName);
        }
        
        // Miniheader - Define a Process
        if ( ! cSend.sProcess.empty() )
        {
            // On viewer, add string specifying a process done during ICE reco
            sendHeader->appendArrayValue("ImageTypeValue4", cSend.sProcess);
        }
        
        // Miniheader - Define a Type ("M" or "P" - important for DistortionCorrection process)
        sendHeader->set("ImageTypeValue3", dumpImageType(cSend.eDicomImaType) );
        
        // Possible futur implementation
        //sendHeader->set("LutLowerBound", -1);
        //sendHeader->set("WindowCenter", -1);
        //sendHeader->set("WindowWidth", -1);
        //sendHeader->set("LargestImagePixelValue", -1);
        
        // Miniheader - Specify a serie UID
        if( ! sendHeader->set( "SeriesInstanceUID", cSend.sUID, MHD_CTRL_SECTION) ) 
        ICE_ERR( "sendHeader->setSeriesInstanceUID failed! Continue anyway ..." );
        
        // Miniheader - Specify a file for color coded images
        // ICE_OUT("\n         bShowMapInColorInX="<<bShowMapInColorInX<<", bDebugModeInX="<<bDebugModeInX<<", LUTFile="<<cSend.sLUTFile.c_str()<<", Serie="<<cSend.sSuffix);
        if ( bShowMapInColorInX && (!cSend.sLUTFile.empty()) )
        {
            sendHeader->set("LUTFileName", cSend.sLUTFile);
            sendHeader->set("PaletteColorLUTUid", createDicomInstanceUid());
        }
        
        // Miniheader - IMAGE NUMBERING
        if ( bIsNumberInSeriesActicatedInX )
        {
            // User defined numbering
            // iNumberInSeries = retrieveNumberInSeries( ctrl, cSend.sSuffix );
            // sendHeader->set("NumberInSeries", iNumberInSeries );
            
            // Default ICE numbering
            m_ImageNumbering->computeSeriesImageNumber(sendHeader, ctrl);
        }
        
        if ( cSend.iEco != -1 )
        sendHeader->set("EchoNumber", cSend.iEco);
    
        // BIsSeriesEnd=true caused pTx system to crash while sending uncombined data
        sendHeader->set( "BIsSeriesEnd", false, MHD_CTRL_SECTION);
        
    }
    
    
    return sendHeader;
}

//-----------------------------------------------------------------------------
// sendData2NextFunctor(IceAs&, MiniHeader&, ImageControl&, sendPars&)
// Description: Send data to the next functor. This method duplicates
//          data before sending to the next functor so that original
//          data can be passed in argument.
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::sendData2NextFunctor(
        IceAs&                  srcAs,      // Data Access Specifier
        MiniHeader::Pointer&    dataHeader, // Data MiniHeader
        ImageControl&           ctrl,       // Data image Control
        sendPars&               cSend      // Outgoing data parameters
        )
{
    ICE_SET_FN("ExtendedFunctor::sendData2NextFunctor")

    // Initialize local variables
    IResult     Res;
    IceAs       sendAs;
    
    if ( bDebugModeInX )
    {ICE_OUT("Incoming IceFormat : "<< dumpIceFormat(srcAs.getIceObjFormat()) << "\n" << cSend.dump() );}
    
    // Cast IceObject and IceAs according to cSend parameters
    if ( failed( castIceObject( sendAs, srcAs, cSend ) ) )
    {ICE_RET_ERR("Could not successfully call castIceObject(...)", I_FAIL);}
    
    // Copy and modify MiniHeader according to cSend parameters
    MiniHeader::Pointer sendHeader = setMiniHeader( dataHeader, ctrl, cSend );
    
    // Copy ImageControl
    ImageControl copyCtrl(ctrl);
    
    // Call next Functor in chain
    Res = ImageReady(sendAs, sendHeader, copyCtrl);
    if( failed(Res) )
    {ICE_RET_ERR("ERROR calling ImageReady(...), aborting...", I_FAIL);}

    return I_OK;
}


//-----------------------------------------------------------------------------
// sendData2Host(IceAs&, MiniHeader&, ImageControl&, sendPars&)
// Description: Send data to the Host. This method duplicates
//          data before sending to the Host so that original
//          data can be passed in argument.
//          Remaining functor in chain are skipped with this method
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::sendData2Host(
        IceAs&                  srcAs,      // Data Access Specifier
        MiniHeader::Pointer&    dataHeader, // Data MiniHeader
        ImageControl&           ctrl,       // Data image Control
        sendPars&               cSend      // Outgoing data parameters
        )
{
    ICE_SET_FN("ExtendedFunctor::sendData2Host")

    // Initialize local variables
    IResult     Res;
    IceAs       sendAs;
    
    if ( bDebugModeInX )
    {ICE_OUT("Incoming IceFormat : "<< dumpIceFormat(srcAs.getIceObjFormat()) << "\n" << cSend.dump() );}
    
    // Cast IceObject and IceAs according to cSend parameters
    if ( failed( castIceObject( sendAs, srcAs, cSend ) ) )
    {ICE_RET_ERR("Could not successfully call castIceObject(...)", I_FAIL);}
    
    // Copy and modify MiniHeader according to cSend parameters
    MiniHeader::Pointer sendHeader = setMiniHeader( dataHeader, ctrl, cSend );
    
    // Copy ImageControl
    ImageControl copyCtrl(ctrl);
    
    // Send data to dicom database
    Res = IceUtils::send(sendAs, sendHeader, ctrl, m_env);
    if (failed(Res))
    {ICE_RET_ERR("Failed during IceUtils::send(sendAs), Abort ...\n", I_FAIL);}
    
    return I_OK;
}



//-----------------------------------------------------------------------------
// ice_format_fl2cxfl(IceAs&, IceAs&)
// Description: Construct a CXFL As from a FL As. Real part is filled with
//              input As value, imaginary part is set to zero.
//-----------------------------------------------------------------------------

bool ExtendedFunctor::ice_format_fl2cxfl(
        IceAs&                  dstAs,      // Data Access Specifier
        IceAs&                  srcAs,      // Data Access Specifier
        double                  scale /* = 1.0 */)
{
    ICE_SET_FN("ExtendedFunctor::ice_format_fl2cxfl")

    // Initialize local variables
    int         lI;
    long        iceBlockLen;
    float       *pFl;
    CMPLX       *pCx;
    
    iceBlockLen = srcAs.getObj()->getSizeOfIceBloc();
    pFl = (float*) srcAs.calcSplObjStartAddr();
    pCx = (CMPLX*) dstAs.calcSplObjStartAddr();
    
    for ( lI=0; lI<iceBlockLen; lI++, pFl++, pCx++ )
    {
        //mod = *pComb / pow( pow(pPha->re,2) + pow(pPha->im,2) , 0.5);
        pCx->re = *pFl * (float)scale;
        pCx->im = 0;
    }
    
    return true;
    
}


//-----------------------------------------------------------------------------
// ice_format_sh2cxfl(IceAs&, IceAs&)
// Description: Construct a CXFL As from a SH As. Real part is filled with
//              input As value, imaginary part is set to zero.
//-----------------------------------------------------------------------------

bool ExtendedFunctor::ice_format_sh2cxfl(
        IceAs&                  dstAs,      // Data Access Specifier
        IceAs&                  srcAs,      // Data Access Specifier
        double                  scale /* = 1.0 */)
{
    ICE_SET_FN("ExtendedFunctor::ice_format_sh2cxfl")

    // Initialize local variables

    
    // Duplicate srcAs before call of next functor
    IceObj::Pointer flObj( Parc::HeapObject<IceObj>::Create() );
    if( ! flObj->create( ICE_FORMAT_FL, srcAs ) )
        ICE_RET_ERR("Failed during flObj->create(srcAs), aborting...", I_FAIL)
    IceAs flAs( flObj );
    flAs = (*flObj)();
    Ice.SDC.calcSoda(flAs, srcAs);
    
    if ( ! Ice.Copy( flAs, srcAs) )
    ICE_RET_ERR("Ice.Copy(flAs, srcAs) failed, aborting...", I_FAIL)
    if ( scale != 1.0 )
    {
        if ( ! Ice.Mul( flAs, srcAs, scale ) )
        ICE_RET_ERR("Ice.Mul(flAs, sendAs, scale) failed, aborting...", I_FAIL)
    }
    
    return ice_format_fl2cxfl(dstAs, flAs, scale);
    
}


//-----------------------------------------------------------------------------
// removeDataInPARC()
// Description: remove all data saved within PDS
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::removeDataInPARC( void )
{
    ICE_SET_FN("ExtendedFunctor::removeDataInPARC")
        
    // Initialize local variables
    int     lI;
    bool    bContains, bDumpPDS=false;
    IResult Res;
    
    for ( lI=0; lI<vsStorageList.size(); lI++ )
    {
        if (    vsStorageList[lI].eLifeTime == IPDS::Measurement
                || !vsStorageList[lI].bSavedPDS 
                || vsStorageList[lI].bSuppressed ) 
        {continue;}
        
        // Check if header already exists in PDS
        bContains = m_pPDS->Contains(vsStorageList[lI].strTag.c_str(), vsStorageList[lI].eLifeTime);
        
        if ( bContains )
        {
            // Remove data in PDS
            Res = m_pPDS->Remove(vsStorageList[lI].strTag.c_str(), vsStorageList[lI].eLifeTime);
            
            if (failed(Res))
            {ICE_ERR("Failed to remove data from PDS :\n"<<dumpStoredElement(vsStorageList[lI]) );}
            else
            {vsStorageList[lI].bSuppressed = true;}
        }
        else
        {
            bDumpPDS=true;
            ICE_ERR("Object not found in PDS :\n"<<dumpStoredElement(vsStorageList[lI]) );
        }
        
    }
    
    if ( bDumpPDS )
    {ICE_OUT(m_pPDS->print());}
    
    return I_OK;
}


//-----------------------------------------------------------------------------
// dumpStoredList()
// Description: Print all objects saved within ExtendedFunctor
//-----------------------------------------------------------------------------

string ExtendedFunctor::dumpStoredList( void )
{
    stringstream stream;
    stream << "Number of stored objects : "<<vsStorageList.size()<<endl;

    for ( int lI=0; lI<vsStorageList.size(); lI++ )
    {stream << dumpStoredElement( vsStorageList[lI] );}

    return stream.str();
}

string ExtendedFunctor::dumpStoredElement( storedElement& element )
{
    stringstream stream;

    stream<<"  "<<element.strTag;
    stream<<" "<<element.strType;
    stream<<(element.bSavedPARC?" PARC":"");
    stream<<(element.bSavedPDS?" PDS":"");
    if ( element.bSavedPDS )
    {stream<<" ("<<dumpLifeTime(element.eLifeTime)<<")";}
    if ( element.bSuppressed )
    {stream<<" -> suppressed";}
    stream<<endl;
 
    return stream.str();
}

//-----------------------------------------------------------------------------
// EndInit(IParcEnvironment*)
// Description: EndInit method of the "XFLFunctor" class.
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::EndInit(IParcEnvironment* env)
{
    ICE_SET_FN("ExtendedFunctor::EndInit(env)")

    // Always call base class:
    IResult res = IceImageReconFunctors::EndInit(env);
    if (failed(res))
        ICE_RET_ERR("Error calling IceImageReconFunctors::EndInit(), aborting...", res)
    
    // Default ICE image numbering method
    m_ImageNumbering = Parc::cast<ImageNumbering>(m_env->Item("DefaultImageNumbering"));
    if(!m_ImageNumbering)
    {
        ICE_RET_ERR("Failed retrieving 'DefaultImageNumbering' from environment, creating local instance.",I_FAIL);
    }
    
    // Get the PDS service for retrieving B0 images that have been saved through study
    // m_pPDS = Parc::cast<IPDS>(m_env->Item("PDS"));
    // if (!m_pPDS)
        // ICE_RET_ERR("Cannot retrieve ParcDataStore! Pointer is invalid.", I_FAIL);

    // ICE_OUT(getDebugMode());
        
    return I_OK;
}

//-----------------------------------------------------------------------------
// ComputeImage(IceAs&, MiniHeader&, ImageControl&)
// Description: ExtendedFunctor method
//-----------------------------------------------------------------------------

IResult ExtendedFunctor::ComputeImage(
        IceAs& /*srcAs*/, 
        MiniHeader::Pointer& /*dataHeader*/, 
        ImageControl& /*ctrl*/      )
{
    ICE_SET_FN("ExtendedFunctor::ComputeImage")
    return I_OK;
}

// -------------------------------------------- //
// -------------------------------------------- //
// DEFINE FUNCTIONS RELATED TO STOREPARS CLASS  //
// -------------------------------------------- //
// -------------------------------------------- //

storePars::storePars()
    : sPrefix           ("")
    , eIceFormat        (ICE_FORMAT_FL)
    , eExtractMode      (IemAmplitude)
    , bSaveInPARC       (true)
    , bSaveInPDS        (false)
    , eLifeTime         (IPDS::Study)
    , m_loopcounter     ()
    , m_bloopcounterSet (false)
{
}


void storePars::init(   string s1,
                        IceFormat_t e2,
                        IceExtractMode e3,
                        bool        b4,
                        bool        b5,
                        IPDS::lifetime e6)
{
    setPrefix       ( s1 );
    setIceFormat    ( e2 );
    setExtractMode  ( e3 );
    saveInPARC      ( b4 );
    saveInPDS       ( b5 );
    setLifeTime     ( e6 );
}


void storePars::copy( storePars cPars )
{
    init(cPars.sPrefix, cPars.eIceFormat, cPars.eExtractMode, cPars.bSaveInPARC, cPars.bSaveInPDS, cPars.eLifeTime);
    m_loopcounter       = cPars.m_loopcounter;
    m_bloopcounterSet   = cPars.m_bloopcounterSet;
}


string storePars::dump( void )
{
    stringstream stream;
    stream << "\n        --- class storePars dump ---\n";
    stream << "           sPrefix:     " << sPrefix << "\n";
    stream << "           Ice Format:  " << dumpIceFormat(eIceFormat) << "\n";
    stream << "           Extract Mode:" << dumpExtractMode(eExtractMode) << "\n";
    stream << "           SaveInPARC:  " << (bSaveInPARC? "True":"False") << "\n";
    stream << "           SaveInPDS:   " << (bSaveInPDS?  "True":"False") << "\n";
    stream << "           Life Time:   " << dumpLifeTime(eLifeTime) << "\n";
    stream << "           loopcounter: " << (m_bloopcounterSet? "True":"False") <<"\n";
    stream << "           "<< dumpLoop() <<"\n";
    stream << "        --- END ---\n";    
    return stream.str();
}


string storePars::dumpLoop( void )
{
    stringstream stream;
    stream << m_loopcounter << endl;
    return stream.str();
}


// -------------------------------------------- //
// -------------------------------------------- //
// DEFINE FUNCTIONS RELATED TO SENDPARS CLASS   //
// -------------------------------------------- //
// -------------------------------------------- //

sendPars::sendPars()
    : sUID              ("")
    , sComment          ("")
    , sProcess          ("")
    , sLUTFile          ("")
    , eDicomImaType     (M)
    , sSuffix           ("")
    , eIceFormat        (ICE_FORMAT_FL)
    , eExtractMode      (IemAmplitude)
    , dScale            (1.0)
    , iEco              (-1)
    , bSendUnchanged    (true)
{}


void sendPars::init(    string s1,
                        string s2,
                        string s3,
                        string s4,
                        ImageType_t   e5, 
                        string s6, 
                        IceExtractMode e7, 
                        double      d8,
                        IceFormat_t e9,
                        int         i10,
                        bool        b11)
{
    setComment      (s1);
    setUID          (s2);
    setProcess      (s3);
    setLUTFile      (s4);
    setDicomImaType (e5);
    setSuffix       (s6);
    setExtractMode  (e7);
    setScale        (d8);
    setIceFormat    (e9);
    setEco          (i10);
    bSendUnchanged = b11;
}


void sendPars::copy( sendPars srcPars )
{

    setComment      ( srcPars.sComment );
    // setUID          ( srcPars.sUID ); // do not want to copy UID, as to be done separately
    setProcess      ( srcPars.sProcess );
    setLUTFile      ( srcPars.sLUTFile );
    setDicomImaType ( srcPars.eDicomImaType );
    setSuffix       ( srcPars.sSuffix );
    setExtractMode  ( srcPars.eExtractMode );
    setScale        ( srcPars.dScale );
    setIceFormat    ( srcPars.eIceFormat );
    setEco          ( srcPars.iEco );
    bSendUnchanged = srcPars.bSendUnchanged;

}


string sendPars::dump( void )
{

    stringstream stream;
    stream << "\n        --- class sendPars dump ---\n";
    stream << "           UID:        " << sUID << "\n";
    stream << "           Comment:    " << sComment << "\n";
    stream << "           Process:    " << sProcess << "\n";
    stream << "           LUT file:   " << sLUTFile << "\n";
    // stream << "           Enum Dicom type: " << eDicomImaType << "\n";
    // stream << "           Strg Dicom type: " << dumpImageType(eDicomImaType) << "\n";
    stream << "           Dicom type: " << dumpImageType(eDicomImaType) << "\n";
    stream << "           Suffix:     " << sSuffix << "\n";
    // stream << "           Enum Extract CX: " << eExtractMode << "\n";
    // stream << "           Strg Extract CX: " << dumpExtractMode(eExtractMode) << "\n";
    stream << "           Extract CX: " << dumpExtractMode(eExtractMode) << "\n";
    stream << "           Scale:      " << dScale << "\n";
    // stream << "           Enum Ice Format: " << eIceFormat << "\n";
    // stream << "           Strg Ice Format: " << dumpIceFormat(eIceFormat) << "\n";
    stream << "           Ice Format: " << dumpIceFormat(eIceFormat) << "\n";
    stream << "           Eco:        " << iEco << "\n";
    stream << "           Unchanged:  " << (bSendUnchanged? "True":"False") << "\n";
    stream << "        --- END ---\n";

    return stream.str();

}



// -------------------------------------------- //
// -------------------------------------------- //
//  FRIEND FUNCTIONS FOR storePars & sendPars   //
// -------------------------------------------- //
// -------------------------------------------- //


ostream&  operator << ( ostream& os, storePars &sPars)
{
    os << sPars.dump();
    return os;
}

ostream&  operator << ( ostream& os, sendPars &sPars)
{
    os << sPars.dump();
    return os;
}

string dumpIceFormat( IceFormat_t eIceFormat )
{
    stringstream stream;
    // ICE_FORMAT_VOID -> -1
    // ICE_FORMAT_SH   -> 0
    // ICE_FORMAT_LO   -> 1
    // ICE_FORMAT_FL   -> 2
    // ICE_FORMAT_CXSH -> 3
    // ICE_FORMAT_CXLO -> 4
    // ICE_FORMAT_CXFL -> 5
    // ICE_FORMAT_CHAR -> 6
    switch(eIceFormat)
    {
        case ICE_FORMAT_VOID:
            stream << "ICE_FORMAT_VOID"; break;
        case ICE_FORMAT_SH:
            stream << "ICE_FORMAT_SH"; break;
        case ICE_FORMAT_LO:
            stream << "ICE_FORMAT_LO"; break;
        case ICE_FORMAT_FL:
            stream << "ICE_FORMAT_FL"; break;
        case ICE_FORMAT_CXSH:
            stream << "ICE_FORMAT_CXSH"; break;
        case ICE_FORMAT_CXLO:
            stream << "ICE_FORMAT_CXLO"; break;
        case ICE_FORMAT_CXFL:
            stream << "ICE_FORMAT_CXFL"; break;    
        case ICE_FORMAT_CHAR:
            stream << "ICE_FORMAT_CHAR"; break;
        default :
            stream << "UNKNOWN"; break;
    }
    return stream.str();
}


string dumpExtractMode( IceExtractMode eExtractMode )
{
    stringstream stream;
    // IemInvalid              = -1,       //***DINO: alle Initialisierungen mÃ¼ssen so bleiben wie es jetzt ist,
    // IemAmplitude            = 0,        //          sonst werden im MTF die Werte falsch berechnet!!!
    // IemPhase                = 1,
    // IemRealPart             = 2,
    // IemImaginaryPart        = 3,
    // IemRealPartAbs          = 4,
    // IemImaginaryPartAbs     = 5,
    // IemRealPartOffset       = 6,
    // IemImaginaryPartOffset  = 7
    switch(eExtractMode)
    {
        case IemInvalid:
            stream << "IemInvalid"; break;
        case IemAmplitude:
            stream << "IemAmplitude"; break;
        case IemPhase:
            stream << "IemPhase"; break;
        case IemRealPart:
            stream << "IemRealPart"; break;
        case IemImaginaryPart:
            stream << "IemImaginaryPart"; break;
        case IemRealPartAbs:
            stream << "IemRealPartAbs"; break;
        case IemImaginaryPartAbs:
            stream << "IemImaginaryPartAbs"; break;    
        case IemRealPartOffset:
            stream << "IemRealPartOffset"; break;
        case IemImaginaryPartOffset:
            stream << "IemImaginaryPartOffset"; break;
        default :
            stream << "UNKNOWN"; break;
    }
    return stream.str();
}


string dumpImageType( ImageType_t eImageType )
{
    stringstream stream;
    switch(eImageType)
    {
        case M:
            stream << "M"; break;
        case P:
            stream << "P"; break;
        default :
            stream << "UNKNOWN"; break;
    }
    return stream.str();
}

string dumpLifeTime( IPDS::lifetime eLifeTime )
{
    stringstream stream;
    switch(eLifeTime)
    {
        // case IPDS::Patient:
            // stream << "IPDS::Patient" << "\n"; break;
        case IPDS::Study: // same as IPDS::Patient
            stream << "IPDS::Study"; break;
        case IPDS::Multistep:
            stream << "IPDS::Multistep"; break;
        case IPDS::Measurement:
            stream << "IPDS::Measurement"; break;
        default :
            stream << "UNKNOWN"; break;
    }
    return stream.str();
}


IResult extractPhase(IceAs& dstAs, IceAs& srcAs, double dScale /* = 1.0 */)
{
    
    // un-used fonction
    
    long            iceBlockLen/*, iceBlockLenDST*/;
    float           *psrcPix, *pdstPix, pi=(float)3.1415926;
    float           rSRC, iSRC, fScale;

    fScale = (float)dScale;
    
    IceAs copyAs = srcAs;
    copyAs.rearrange(COL, LIN);
    ICE_OUT(copyAs);
    ICE_OUT(dstAs);
    
    iceBlockLen = dstAs.getObj()->getSizeOfIceBloc();
    psrcPix = (float*) srcAs.calcSplObjStartAddr();
    pdstPix = (float*) dstAs.calcSplObjStartAddr();
    
    // iceBlockLenDST = dstAs.getObj()->getSizeOfIceBloc();
    // ICE_OUT("\nSizeOfIceBloc srcAs = "<<iceBlockLen<<"\nSizeOfIceBloc dstAs = "<<iceBlockLenDST);
    
    // IceUtils::CheckAndDisplay( dstAs, IceDisplayData::DISPLAY, "extractPhase" );
    
    for ( int k=0; k < iceBlockLen; k++, psrcPix++, pdstPix++ )
    {

        // Retrieve real, imaginary and phase from source Image
        rSRC = *psrcPix;
        psrcPix++;
        iSRC = *psrcPix;
        
        if ( rSRC == 0.0 && iSRC == 0.0 ) {
            *pdstPix = 0; }
        else if ( rSRC == 0.0 && iSRC < 0.0 ) {
            *pdstPix = -pi/2 * fScale; }
        else if ( rSRC == 0.0 && iSRC > 0.0 ) {
            *pdstPix = pi/2 * fScale; }
        else if ( rSRC < 0.0 && iSRC < 0.0 ) {
            *pdstPix = (atanf(iSRC/rSRC)-pi) * fScale; }
        else if ( rSRC < 0.0 && iSRC >= 0.0 ) {
            *pdstPix = (atanf(iSRC/rSRC)+pi) * fScale; }
        else if ( rSRC > 0.0 ) {
            *pdstPix = atanf(iSRC/rSRC) * fScale; }
        else {
            ICE_OUT("What are values of rSRC and iSRC ?");
            return I_FAIL; }

        *pdstPix = iSRC;    
            
    }
    IceUtils::CheckAndDisplay( srcAs, IceDisplayData::DISPLAY, "extractPhase" );
    IceUtils::CheckAndDisplay( dstAs, IceDisplayData::DISPLAY, "extractPhase" );
    return I_OK;
}

} // END OF NAMESPACE ICE_NAMESPACE

//EOF