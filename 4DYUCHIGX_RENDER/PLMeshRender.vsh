//----------------------------------------------------------------------------
//      reg c0-c3	= WorldViewProj matrix
//      reg c4      = constant color
//	    reg v0		= input register
//----------------------------------------------------------------------------

vs.1.1                             //Shader version 1.1		
m4x4 oPos, v0, c0
mov     oD0  , c4

