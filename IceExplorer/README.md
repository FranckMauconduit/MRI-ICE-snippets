# IceExplorer
> Explorer functors

## Short description
This ICE project contains two different functors: an 'IceScanFunctors' and an 'IceImageReconFunctors'. Each one can be used to explorer an ICE chain by positioning the functors before or after a given functor. This ICE project is meant to be used in IDEA for debug purpose, not at the scanner.

## IceExplorer functor parameters

Once compiled, the IceExplorer functors can be configured using the 'IceProgramExplorer.evp/.ipr' files. Available parameters are:

- 'EXPLORER.SCAN_FUNCTOR.Activate' : Activate/Deactivate usage of SCAN functor
- 'EXPLORER.SCAN_FUNCTOR.LinkedFunctor' : Functor name on which to attach the Explorer SCAN functor in the ICE chain.
- 'EXPLORER.SCAN_FUNCTOR.Successor' : Switch to attach SCAN functor before or after 'LinkedFunctor'
- 'EXPLORER.IMAGE_FUNCTOR.Activate' : Activate/Deactivate usage of IMAGE functor
- 'EXPLORER.IMAGE_FUNCTOR.LinkedFunctor' : Functor name on which to attach the Explorer IMAGE functor in the ICE chain.
- 'EXPLORER.IMAGE_FUNCTOR.Successor' : Switch to attach IMAGE functor before or after 'LinkedFunctor'
- 'EXPLORER.StopChain' : Switch to send (or not) data to next functor in chain
- 'EXPLORER.CheckAndDisplay' : Switch to activate 'CheckAndDisplay'. It will show images as they arrive in the functor.
- 'EXPLORER.VisuScaleFactor' : Scale image before visualization with 'CheckAndDisplay'
- 'EXPLORER.DumpCurrentLoopIndex' : Switch to dump current loop counter indexes into logs
- 'EXPLORER.SingleThreading' : Switch to force pipeservice to single-thread instead of multi-thread
- 'EXPLORER.DumpIceAs' : Switch to dump Ice access specifier information into logs
- 'EXPLORER.DumpIceObj' : Switch to dump Ice object information into logs
- 'EXPLORER.DumpMiniHeader' : Switch to dump miniheader information into logs
- 'EXPLORER.DumpImageControl' : Switch to dump image control information into logs
- 'EXPLORER.DumpParcENV' : Switch to dump Parc environment content into logs
- 'EXPLORER.DumpParcPDS' : Switch to dump PDS (Parc Data Store) content into logs


## Information

- Compilation tested on Software versions VB17, VE11C/E, VE12U

