#include "light_obj.h"


DWORD CLightObject::WriteFile(FILE* fp)
{
	DWORD	oldPos = ftell(fp);
	
	CBaseObject::WriteFile(fp);

	FILE_LIGHT_HEADER*	pHeader = (FILE_LIGHT_HEADER*)&this->m_dwColor;
	fwrite(pHeader,sizeof(FILE_LIGHT_HEADER),1,fp);

	
	return (ftell(fp)-oldPos);
}
