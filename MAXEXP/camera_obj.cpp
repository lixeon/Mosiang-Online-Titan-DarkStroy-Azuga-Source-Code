#include "camera_obj.h"

CCameraObject::CCameraObject()
{
	memset((char*)this+4,0,sizeof(CCameraObject)-4);
}

DWORD CCameraObject::WriteFile(FILE* fp)
{
	FILE_CAMERA_HEADER* pHeader = (FILE_CAMERA_HEADER*)&(this->m_dwTargetIndex);

	DWORD	oldPos = ftell(fp);

	CBaseObject::WriteFile(fp);
	fwrite(pHeader,sizeof(FILE_CAMERA_HEADER),1,fp);
	
	return (ftell(fp)-oldPos);

}

CCameraObject::~CCameraObject()
{
}
