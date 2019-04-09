#pragma once

#include "../max_common/stdafx.h"
#include "object_property.h"
#include "motion_obj.h"


class Interface;

struct FILE_MOTION_HEADER
{
	DWORD					dwVersion;
	DWORD					dwTicksPerFrame;
	DWORD					dwFirstFrame;
	DWORD					dwLastFrame;
	DWORD					dwFrameSpeed;
	DWORD					dwMotionObjectNum;
	DWORD					dwKeyFrameStep;
	CMotionObject*			pBaseMotionObject;
	TCHAR					uszMotionName[MAX_NAME_LEN];
};

class CMotion 
{
	DWORD					m_dwTicksPerFrame;
	DWORD					m_dwFirstFrame;
	DWORD					m_dwLastFrame;
	DWORD					m_dwFrameSpeed;
	DWORD					m_dwMotionObjectNum;
	DWORD					m_dwKeyFrameStep;
	CMotionObject*			m_pBaseMotionObject;
	TCHAR					m_uszMotionName[MAX_NAME_LEN];
	DWORD					m_dwCurFrame;

	DWORD					m_dwMeshFrameStep;
	
	BOOL					IsKnownController(Control* cont);
	TriObject*				GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt);
	BOOL					TMNegParity(Matrix3 &m);
	BOOL					CheckForAnimation(INode* node, BOOL& bPos, BOOL& bRot, BOOL& bScale,Interface* ip);

	BOOL					ConvertAnimatedMesh(CMotionObject* pObj,INode* pNode,Interface* ip,CMotionFlag flag,TimeValue start,TimeValue end);
	POS_KEY*				ConvertPosSample(CMotionObject* pMotObj,INode* node,Interface* ip,TimeValue start,TimeValue end);
	ROT_KEY*				ConvertRotSample(CMotionObject* pMotObj,INode* node,Interface* ip,TimeValue start,TimeValue end);
	SCALE_KEY*				ConvertScaleSample(CMotionObject* pMotOjb,INode* node,Interface* ip,TimeValue start,TimeValue end);
	
	BOOL					ConvertNodeTM(NODE_TM* pTM,INode* pNode,DWORD dwTime);
	BOOL					DumpMatrix3(NODE_TM* pTM,Matrix3* m, Matrix3* pParentTM);
	CMotionObject*			AllocMotionObject();
public:
	BOOL					SetGlobalInfo(Interface* ip,TCHAR* uszMotionName);
	CMotionObject*			InsertMotionObject(INode* pNode,Interface* ip,CObjectPropertyList* pPropertyList);
	BOOL					WriteFile(TCHAR* uszFileName);
	BOOL					ReadFile(TCHAR* uszFileName);
	void					ReleaseObjects();

	void					SetKeySampleStep(DWORD dwSampleStep) {m_dwKeyFrameStep = dwSampleStep;}
	void					SetMeshSampleStep(DWORD dwSampleStep) {m_dwMeshFrameStep = dwSampleStep;}


	
	CMotion();
	~CMotion();
	
	CMotion*				m_pNextMotion;
	
	
	
};

