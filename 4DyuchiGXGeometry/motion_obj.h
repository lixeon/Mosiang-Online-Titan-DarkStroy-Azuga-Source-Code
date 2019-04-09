#pragma once

#include "../4DyuchiGRX_common/typedef.h"
#include "../4DyuchiGRX_common/motion_flag.h"
#include "bintree.h"

enum KEY_TYPE {KEY_TYPE_POS = 0x000000ff, KEY_TYPE_ROT = 0x0000ff00, KEY_TYPE_SCALE = 0x00ff0000};

struct ANIMATED_MESH_KEY
{
	DWORD			dwTicks;
	DWORD			dwFrame;
	DWORD			dwVertexNum;
	DWORD			dwTexVertexNum;
	VECTOR3*		pv3Pos;
	TVERTEX*		ptUV;

};
struct KEY
{
	DWORD			dwTicks;
	DWORD			dwFrame;
};

struct ROT_KEY
{
	DWORD			dwTicks;
	DWORD			dwFrame;
	QUARTERNION		Q;	
};

struct SCALE_KEY
{
	DWORD			dwTicks;
	DWORD			dwFrame;
	VECTOR3			v3Scale;
	VECTOR3			v3ScaleAxis;
	float			fScaleAxisAng;
};

struct POS_KEY 
{
	DWORD			dwTicks;
	DWORD			dwFrame;
	VECTOR3			v3Pos;
};
struct FILE_MOTION_OBJECT_HEADER
{
	DWORD					dwIndex;
	DWORD					dwRotKeyNum;
	DWORD					dwPosKeyNum;
	DWORD					dwScaleKeyNum;
	DWORD					dwAnimatedMeshKeyNum;
	char					szObjectName[MAX_NAME_LEN];
	CMotionFlag				motionFlag;

};

class CMotionObject
{	
	DWORD					m_dwIndex;
	DWORD					m_dwRotKeyNum;
	DWORD					m_dwPosKeyNum;
	DWORD					m_dwScaleKeyNum;
	DWORD					m_dwAnimatedMeshKeyNum;
	char					m_szObjectName[MAX_NAME_LEN];
	CMotionFlag				m_motionFlag;

	ROT_KEY*				m_pRotKey;
	POS_KEY*				m_pPosKey;
	SCALE_KEY*				m_pScaleKey;
	ANIMATED_MESH_KEY*		m_pAnimatedMeshKey;
	DWORD					m_dwKeyFrameStep;
	CBinTree				m_RotKeyTree;
	CBinTree				m_PosKeyTree;
	CBinTree				m_ScaleKeyTree;
	CBinTree				m_AnimatedMeshKeyTree;
	
	DWORD					m_dwNameLen;

	void					Initialize();

public:
	DWORD					GetNameLen()	{return m_dwNameLen;}

	void					SetKeyFrameStep(DWORD dwStep) {m_dwKeyFrameStep = dwStep;}
	void					SetObjectName(char* szObjName) {lstrcpy(m_szObjectName,szObjName);}
	char*					GetObjectName() {return m_szObjectName;}
	
	void					GetKeyEntry(SCALE_KEY* pScale,ROT_KEY* pRot,POS_KEY* pPos);
	BOOL					SetRotMatrix(MATRIX4* pMat,NODE_TM* pBaseTM,DWORD dwFrame);
	BOOL					SetPosMatrix(MATRIX4* pMat,NODE_TM* pBaseTM,DWORD dwFrame);
	BOOL					SetScaleMatrix(MATRIX4* pMat,NODE_TM* pBaseTM,DWORD dwFrame);
	BOOL					SetMorphFrame(VECTOR3** pv3Local,DWORD dwVertexNum,TVERTEX** ptvUV,DWORD dwTexVertexNum,DWORD dwFrame);

	BOOL					CreateRotKeyList(DWORD dwNum);
	BOOL					CreatePosKeyList(DWORD dwNum);
	BOOL					CreateScaleKeyList(DWORD dwNum);


	BOOL					InsertRotKey(ROT_KEY* pRotKey);
	BOOL					InsertPosKey(POS_KEY* pPosKey);
	BOOL					InsertScaleKey(SCALE_KEY* pScaleKey);
	
	void					ReleasePosKey();
	void					ReleaseRotKey();
	void					ReleaseScaleKey();

	DWORD					WriteFile(FILE* fp);
	DWORD					ReadFile(void* pFP);
	CMotionObject();
	~CMotionObject();

	CMotionObject*			m_pNextMotionObject;
};
