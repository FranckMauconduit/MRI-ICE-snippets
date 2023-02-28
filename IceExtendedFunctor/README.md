# IceExtendedFunctor
> an 'IceImageReconFunctors' class to be used as a parent class

## Short description
This functor is a helper class that brings functionalities to save and retrieve images for post-processing in the ICE environment. It must be used as a parent class (inheritance).

## Content description

- The 'ExtendedFunctor' folder contains the .cpp/.h file to be copied in your ICE project

- The 'TEMPLATE_ice_with_ExtendedFunctor' folder contains a basic functor example using the 'ExtendedFunctor' class

## More details

Once your functor inherits from 'ExtendedFunctor' class (instead of the standard 'IceImageReconFunctors' class), the following functionalities become available.


Methods to save/retrieve data in PARC or PDS (Parc data store) and to send data to next functor:
- storeImageInPARC      ( IceAs& srcAs, ImageControl& ctrl, storePars cPars ); // will store srcAs using parameters defined in storePars
- retrieveImageFromPARC ( IceAs& dstAs, IceLoopCounters& lcIm, storePars cPars ); // will retrieve data using parameters defined in storePars
- sendData2NextFunctor  ( IceAs& srcAs, MiniHeader::Pointer& dataHeader, ImageControl& ctrl, sendPars cPars); // will send data to next functor using parameters defined in sendPars


The class 'sendPars' is used in conjunction with sendData2NextFunctor() method. It will basically send the IceAs data to next functor. Several parameters can be easily set using the following methods of sendPars class:
- change the ICE format type                    setIceFormat(...)
- add a comment in Miniheader (DICOM comment)   setComment(...)
- change the lookup table                       setLUTFile(...)
- scale the data before sending                 setScale(...)
- change dicom type to module or phase          setDicomImaType(...)
- append a string to the series name            setSuffix(...)


The class 'storePars' is used in conjunction with storeImageInPARC() / retrieveImageFromPARC(). It can save/retrieve IceAs objects as well as Miniheader objects. The 'storePars' class helps to configure the save/retrieve actions using the following methods:
- use a prefix to build the "key"           setPrefix(...)
- change the data type before saving        setIceFormat(...)
- extract mod or phase from complex data    setExtractMode(...)
- save data in PARC (m_env->Add())          saveInPARC(...)
- save data in PDS (m_pPDS->Add())          saveInPDS(...)
- set the lifetime for sharing between scans for instance setLifeTime(...)
