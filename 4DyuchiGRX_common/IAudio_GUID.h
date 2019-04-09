#pragma once

#include "stdafx.h"

// {49FA7378-0BD9-4a4a-958B-B3F141E19002}
DEFINE_GUID(CLSID_4DyuchiAudio_DEFAULT, 
0x49fa7378, 0xbd9, 0x4a4a, 0x95, 0x8b, 0xb3, 0xf1, 0x41, 0xe1, 0x90, 0x2);

// {DD373350-D183-4b24-9909-13145EAEBD04}
DEFINE_GUID(CLSID_4DyuchiAudio_FOR_CORUM, 
0xdd373350, 0xd183, 0x4b24, 0x99, 0x9, 0x13, 0x14, 0x5e, 0xae, 0xbd, 0x4);

// {EA5E25FD-4010-47c9-AADF-C06B77299A6A}
DEFINE_GUID(CLSID_4DyuchiAudio_FOR_MUKHYANG, 
0xea5e25fd, 0x4010, 0x47c9, 0xaa, 0xdf, 0xc0, 0x6b, 0x77, 0x29, 0x9a, 0x6a);

// {E025FC53-0292-4ff0-A07D-240CB7D24066}
DEFINE_GUID(IID_4DyuchiAudio, 
0xe025fc53, 0x292, 0x4ff0, 0xa0, 0x7d, 0x24, 0xc, 0xb7, 0xd2, 0x40, 0x66);



#if defined(_FOR_CORUM)
	#define	CLSID_4DyuchiAudio	CLSID_4DyuchiAudio_FOR_CORUM
#elif defined(_FOR_MUKHYANG)
	#define	CLSID_4DyuchiAudio	CLSID_4DyuchiAudio_FOR_MUKHYANG
#else
	#define CLSID_4DyuchiAudio	CLSID_4DyuchiAudio_DEFAULT
#endif