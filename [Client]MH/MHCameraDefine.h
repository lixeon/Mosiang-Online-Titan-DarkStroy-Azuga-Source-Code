#ifndef _MHCAMERADEFINE_H_
#define _MHCAMERADEFINE_H_

enum eCAMERASHAKEDESC
{
	eCSD_Earthquake =1,
	eCSD_Vibration,

	eCSD_Max,
};

struct SHAKEVECTOR
{
	VECTOR3 ShakeVector;
	DWORD Duration;
};

struct MHCAMERASHAKE
{
	MHCAMERASHAKE(SHAKEVECTOR p[],DWORD TotalShakeVectorSize)
	{
		pShakeVector = p;
		MaxShakeVector = TotalShakeVectorSize / sizeof(SHAKEVECTOR);
	}
	SHAKEVECTOR* pShakeVector;
	DWORD MaxShakeVector;
};

SHAKEVECTOR MHDesc_Earthquake[];

SHAKEVECTOR MHDesc_Vibration[];

#endif