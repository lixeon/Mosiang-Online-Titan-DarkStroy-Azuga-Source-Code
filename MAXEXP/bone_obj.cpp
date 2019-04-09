#include "bone_obj.h"


CBoneObject::CBoneObject()
{
}
DWORD CBoneObject::WriteFile(FILE* fp)
{
	DWORD	oldPos = ftell(fp);



	CBaseObject::WriteFile(fp);
	fwrite(&m_colMeshDesc,sizeof(m_colMeshDesc),1,fp);

	return (ftell(fp)-oldPos);
	
}

CBoneObject::~CBoneObject()
{

	CBaseObject::~CBaseObject();
	
	
}