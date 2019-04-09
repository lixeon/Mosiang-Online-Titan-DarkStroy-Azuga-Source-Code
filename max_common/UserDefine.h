#pragma once

#define	BILLBOARD		"BILLBOARD"
#define VERTEXLIGHT		"VERTEXLIGHT"
#define LIGHTMAPLIGHT	"LIGHTMAPLIGHT" 

class CMeshFlag;
void CreateMeshTextItem(char* txt,CMeshFlag flag,char* szObjName);
void BuildMeshFlag(CMeshFlag* pMeshFlag,char* szBuffer,char* szObjName);

#define	KEYFRAME		"KEYFRAME"
#define VERTEXANI		"VERTEXANI"
#define UVANI			"UVANI" 

class CMotionFlag;

void CreateMotionTextItem(char* txt,CMotionFlag flag,char* szObjName);
void BuildMotionFlag(CMotionFlag* pMotionFlag,char* szBuffer,char* szObjName);

