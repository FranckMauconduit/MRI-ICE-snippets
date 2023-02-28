# IceSITF
> 'Save Images To File' functor

## Short description
This functor is an 'IceImageReconFunctors' that can be positioned into the ICE chain. All images that pass through this functor are saved in a binary file. In addition, a Matlab script is generated to easily load the binary data into Maltab variables.

## IceSITF functor parameters

Once compiled, the IceSITF functor can be configured using the 'IceSITF.evp' file. Available parameters are:

- <ParamString."LinkedFunctor"> : Functor name on which to attach IceSITF in the ICE chain.

- <ParamBool."Successor"> : Switch to attach IceSITF before or after 'LinkedFunctor'

- <ParamBool."PassOnData"> : Switch to (not) send data from IceSITF to next functor


## Information

- Compilation tested on Software versions VB17, VE11C/E, VE12U

