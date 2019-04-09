#pragma once


#include "motion.h"
#include "../max_common/helper.h"
#include "../max_common/stdafx.h"
#include "../4DyuchiGXGFunc/global.h"
#include <crtdbg.h>

DWORD ReCreateVertexList(TFACE* ptFace,FACE* pFace,VECTOR3** ppv3,TVERTEX** pptv,DWORD dwTFacesNum,DWORD dwVertexNum,DWORD** ppdwExtVertexIndex,DWORD* pdwExtVertexNum);
void ReIndexFace(WORD* pFaceIndex,WORD dwTFaceIndex,POINT3D_REF_TVERTEX* pvRef,POINT3D_REF_TVERTEX* pvRefExt,VECTOR3* pv3,TVERTEX* ptv,DWORD* pCount,DWORD* pOriginalIndex);

CMotion::CMotion()
{
	memset(this,0,sizeof(CMotion));
	m_dwKeyFrameStep = 5;
	m_dwMeshFrameStep = 5;

}
BOOL CMotion::SetGlobalInfo(Interface* ip,TCHAR* uszMotionName)
{
	Interval range = ip->GetAnimRange();
	m_dwTicksPerFrame = GetTicksPerFrame();

	m_dwFirstFrame = range.Start() / GetTicksPerFrame();
	m_dwLastFrame = range.End() / GetTicksPerFrame() - 1;
	m_dwFrameSpeed = GetFrameRate();
	
	_tcscpy(m_uszMotionName,uszMotionName);

	return TRUE;
}

CMotionObject* CMotion::AllocMotionObject()
{
	CMotionObject*	pMotObj = new CMotionObject;
	pMotObj->m_pNextMotionObject = m_pBaseMotionObject;
	m_pBaseMotionObject = pMotObj;
	m_dwMotionObjectNum++;

	return pMotObj;
}
CMotionObject* CMotion::InsertMotionObject(INode* pNode,Interface* ip,CObjectPropertyList* pPropertyList)
{
	BOOL	bDoKeys = FALSE;	
	BOOL	bPosAnim;
	BOOL	bRotAnim;
	BOOL	bScaleAnim;
//	OBJECT_PROPERTY_ITEM*	pItem;
//	pItem = pPropertyList->GetPropertyPtr(pNode->GetName());
//	CMotionFlag*	pMotionFlag = (CMotionFlag*)&pItem->dwPropertyFlag;
	CMotionFlag*	pMotionFlag = (CMotionFlag*)pPropertyList->GetPropertyPtr(pNode->GetName());
	
	Control* pC = pNode->GetTMController()->GetPositionController();
	Control* rC = pNode->GetTMController()->GetRotationController();
	Control* sC = pNode->GetTMController()->GetScaleController();

	

	if (pMotionFlag->GetMotionTypeKeyFrame() == MOTION_TYPE_KEYFRAME_ENABLE ||
		pMotionFlag->GetMotionTypeVertex() == MOTION_TYPE_VERTEX_ENABLE ||
		pMotionFlag->GetMotionTypeUV() == MOTION_TYPE_UV_ENABLE ) 
	{
		
		// create motion object
		CMotionObject* pMotObj = AllocMotionObject();
		pMotObj->SetObjectName(pNode->GetName());


		TimeValue start = ip->GetAnimRange().Start();
		TimeValue end = ip->GetAnimRange().End();
		if (pMotionFlag->GetMotionTypeKeyFrame() == MOTION_TYPE_KEYFRAME_ENABLE)
		{
			if (CheckForAnimation(pNode, bPosAnim, bRotAnim, bScaleAnim,ip))
			{


				if (bPosAnim)
					ConvertPosSample(pMotObj,pNode,ip,start,end);
	
				if (bRotAnim) 
					ConvertRotSample(pMotObj,pNode,ip,start,end);

				if (bScaleAnim) 
					ConvertScaleSample(pMotObj,pNode,ip,start,end);
			}
		}
		if (pMotionFlag->GetMotionTypeVertex() == MOTION_TYPE_VERTEX_ENABLE || 
			pMotionFlag->GetMotionTypeUV() == MOTION_TYPE_UV_ENABLE )
		{
			ConvertAnimatedMesh(pMotObj,pNode,ip,*pMotionFlag,start,end);
		}


	
	}
	return NULL;
}
POS_KEY* CMotion::ConvertPosSample(CMotionObject* pMotObj,INode* node,Interface* ip,TimeValue start,TimeValue end)
{	

	TimeValue t;
	int delta = GetTicksPerFrame() * m_dwKeyFrameStep;
	Matrix3 tm;
	AffineParts ap;
	Point3	prevPos;
	POS_KEY	posKey;

	DWORD	dwNum = (end-start)/delta+1+1;

	pMotObj->CreatePosKeyList(dwNum);
	
	for (t=start + GetTicksPerFrame(); t<=end; t+=delta) 
	{
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
		decomp_affine(tm, &ap);

		Point3 pos = ap.t;

		posKey.v3Pos.x = pos.x;
		posKey.v3Pos.y = pos.z;
		posKey.v3Pos.z = pos.y;

		posKey.dwTicks = t;
		posKey.dwFrame = t / GetTicksPerFrame();

	/*	if (posKey.dwFrame != 1)
		{
			if (t!= start && EqualPoint3(pos, prevPos)) 
			{
				// Skip identical keys 
				goto lb_skip;
			}
		}*/
		pMotObj->InsertPosKey(&posKey);

lb_skip:
		prevPos = pos;
	}

	return NULL;
}

ROT_KEY* CMotion::ConvertRotSample(CMotionObject* pMotObj,INode* node,Interface* ip,TimeValue start,TimeValue end)
{	
	// 로테이션 샘플은 0번부터 누적이므로 start에 상관없이 0부터 누적 계산한다..
	TimeValue t;
	int delta = GetTicksPerFrame() * m_dwKeyFrameStep;
	Matrix3 tm;
	AffineParts ap;
	Quat prevQ;
	Point3 axis;
	float angle;
	float	temp;

	ROT_KEY rotKey;
	
	prevQ.Identity();

	QUARTERNION		quatA;
	QUARTERNION		quatQ;

	DWORD	dwNum = (end-start)/delta + 1 + 1;//start와 end는 무조건 들어가므로 2개 기본.

	pMotObj->CreateRotKeyList(dwNum);


	tm = node->GetNodeTM(start) * Inverse(node->GetParentTM(start));
	decomp_affine(tm, &ap);

	Quat q = ap.q / prevQ;
	prevQ = ap.q;


	AngAxisFromQ(q, &angle, axis);
	temp = axis.y;
	axis.y = axis.z;
	axis.z = temp;
	angle *= -1.0f;

	QuaternionFromRotation(&quatA,(VECTOR3*)&axis,angle);
	QuaternionMultiply(&quatQ, 
		&quatA,
		&quatQ);

	if (q.IsIdentity()) 
	{
		goto lb_skip;
	}
//	pMotObj->InsertRotKey(&rotKey);

lb_skip:
	for (t=start+GetTicksPerFrame(); t<=end; t+=delta) 
	{
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));

		decomp_affine(tm, &ap);

		// Rotation keys should be relative, so we need to convert these
		// absolute samples to relative values.

		Quat q = ap.q / prevQ;
		prevQ = ap.q;


		AngAxisFromQ(q, &angle, axis);
		temp = axis.y;
		axis.y = axis.z;
		axis.z = temp;
		angle *= -1.0f;

	
		
		QuaternionFromRotation(&quatA,(VECTOR3*)&axis,angle);
		QuaternionMultiply(&quatQ, 
			&quatA,
			&quatQ);

		rotKey.Q = quatQ;
		rotKey.dwTicks = t;
		rotKey.dwFrame = t / GetTicksPerFrame();


		
		/*if (rotKey.dwFrame != 1)
		{
			if (q.IsIdentity()) 
			{
				// No point in exporting null keys...
				continue;
			}
		}*/
		pMotObj->InsertRotKey(&rotKey);
	}
	if (rotKey.dwTicks < end)
	{
		tm = node->GetNodeTM(end) * Inverse(node->GetParentTM(end));

		decomp_affine(tm, &ap);

		// Rotation keys should be relative, so we need to convert these
		// absolute samples to relative values.

		Quat q = ap.q / prevQ;
		prevQ = ap.q;

		AngAxisFromQ(q, &angle, axis);
		temp = axis.y;
		axis.y = axis.z;
		axis.z = temp;
		angle *= -1.0f;
	
		
		QuaternionFromRotation(&quatA,(VECTOR3*)&axis,angle);
		QuaternionMultiply(&quatQ, 
			&quatA,
			&quatQ);

		rotKey.Q = quatQ;
		rotKey.dwTicks = end;
		rotKey.dwFrame = end / GetTicksPerFrame();

		pMotObj->InsertRotKey(&rotKey);
	}
	return NULL;
}/*
ROT_KEY* CMotion::ConvertRotKey(CMotionObject* pMotObj,INode* node,Interface* ip,Control* cont)
{

	IKeyControl *ikc = GetKeyControlInterface(cont);

	if (!ikc)
		__asm int 3

	int numKeys = ikc->GetNumKeys();
	for (i=0; i<numKeys; i++) 
	{
		ILinRotKey key;
		ikc->GetKey(i, &key);
	}
}
*/
SCALE_KEY* CMotion::ConvertScaleSample(CMotionObject* pMotObj,INode* node,Interface* ip,TimeValue start,TimeValue end)
{	
	TimeValue t;
	int delta = GetTicksPerFrame() * m_dwKeyFrameStep;
	Matrix3 tm;
	AffineParts ap;
	Point3	prevFac;
	Point3	axis;
	float	angle;
	
	SCALE_KEY	scaleKey;
	DWORD	dwNum = (end-start)/delta+1;

	pMotObj->CreateScaleKeyList(dwNum);

	for (t=start+GetTicksPerFrame(); t<=end; t+=delta) 
	{
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
		decomp_affine(tm, &ap);

		scaleKey.dwFrame = t / GetTicksPerFrame();
		
	/*	if (scaleKey.dwFrame != 1)
		{

			if (t!= start && EqualPoint3(ap.k, prevFac)) {
				goto lb_skip;
				// Skip identical keys 
				
			}
		}*/

		
		
		AngAxisFromQ(ap.u, &angle, axis);
		
		scaleKey.v3Scale.x = ap.k.x;
		scaleKey.v3Scale.y = ap.k.z;
		scaleKey.v3Scale.z = ap.k.y;
		scaleKey.v3ScaleAxis.x = axis.x;
		scaleKey.v3ScaleAxis.y = axis.z;
		scaleKey.v3ScaleAxis.z = axis.y;
		scaleKey.fScaleAxisAng = angle*(-1.0f);

		scaleKey.dwTicks = t;
		

		pMotObj->InsertScaleKey(&scaleKey);
lb_skip:
		prevFac = ap.k;

	}
	return NULL;
}
BOOL CMotion::ConvertAnimatedMesh(CMotionObject* pObj,INode* pNode,Interface* ip,CMotionFlag flag,TimeValue start,TimeValue end)
{
	ObjectState os = pNode->EvalWorldState(0);
	
	if (!os.obj)
		return FALSE;
	
	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID)
		return FALSE;
	
		
	TriObject* tri;
	BOOL needDel;

	
	// Get animation range
	Interval animRange = ip->GetAnimRange();

	// Get validity of the object
	Interval objRange;
	
	

	Point3			vp;


	NODE_TM			nodeTM;
	DWORD			dwMtlIndex[MAX_MATERIAL_NUM];

	VECTOR3 zero;
	zero.x = 0.0f;
	zero.y = 0.0f;
	zero.z = 0.0f;


	TimeValue t;
	DWORD	dwTicksPerFrame = GetTicksPerFrame();
	int delta = GetTicksPerFrame() * m_dwMeshFrameStep;

	DWORD	dwKeyNum = (end-start)/delta+1+1;
	
	pObj->CreateAnimatedMeshKeyList(dwKeyNum);
	ANIMATED_MESH_KEY	anmKey;

	DWORD	dwNum = 0;
	for (t=start + GetTicksPerFrame(); t<=end; t+=delta) 
	{
		memset(&anmKey,0,sizeof(anmKey));

		// This may seem strange, but the object in the pipeline
		// might not be valid anymore.
		os = pNode->EvalWorldState(t);
		objRange = os.obj->ObjectValidity(t);
	//	t = objRange.Start() < animRange.Start() ? animRange.Start() : objRange.Start();

		if (objRange.End() > animRange.End())
			continue;
			//	break;

		ConvertNodeTM(&nodeTM,pNode,t);			
		
		tri = GetTriObjectFromNode(pNode, t, needDel);
		if (!tri)
		{
			MessageBox(NULL,"Fail to GetTriObject()","Error",MB_OK);
			return FALSE;
			
		}

		Matrix3		tm = pNode->GetObjTMAfterWSM(t);
		BOOL		negScale = TMNegParity(tm);
		ObjectState os = pNode->EvalWorldState(t);
		int vx1, vx2, vx3;
		DWORD		i;

		// Order of the vertices. Get 'em counter clockwise if the objects is
		// negatively scaled.
		if (negScale) {
			vx1 = 2;
			vx2 = 1;
			vx3 = 0;
		}
		else {
			vx1 = 0;
			vx2 = 1;
			vx3 = 2;
		}
		Mesh*		mesh = &tri->GetMesh();
		DWORD		dwVertexNum = mesh->getNumVerts();
		DWORD		dwTexVertexNum = mesh->getNumTVerts();
		DWORD		dwFacesNum = mesh->getNumFaces();
		DWORD		dwTFacesNum = mesh->getNumFaces();
		FACE*		pFace = NULL;
		TFACE*		pTFace = NULL;
		VECTOR3*	pv3;
		TVERTEX*	puv;

		if (dwFacesNum)
		{
			DWORD		dwFaceCount = 0;
			memset(dwMtlIndex,0,sizeof(DWORD)*MAX_MATERIAL_NUM);
			pFace = new FACE[dwFacesNum];

			for (i=0; i<dwFacesNum; i++)
			{
				if (mesh->faces[i].getMatID() >= MAX_MATERIAL_NUM)
				{
					MessageBox(NULL,"Max Materials over","ConvertMesh..",MB_OK);
					return FALSE;
				}
				VECTOR3		vp[3];
				memcpy(&vp[0],tm * mesh->verts[mesh->faces[i].v[vx1]],sizeof(VECTOR3));
				memcpy(&vp[1],tm * mesh->verts[mesh->faces[i].v[vx3]],sizeof(VECTOR3));
				memcpy(&vp[2],tm * mesh->verts[mesh->faces[i].v[vx2]],sizeof(VECTOR3));

				
				if (!IsValidTri(vp+0,vp+1,vp+2))
				{
					__asm nop
				}
				else 
				{
					pFace[i].a = (WORD)mesh->faces[i].v[vx1];
					pFace[i].b = (WORD)mesh->faces[i].v[vx3];
					pFace[i].c = (WORD)mesh->faces[i].v[vx2];
					pFace[i].mtlid = mesh->faces[i].getMatID(); 
			
					dwMtlIndex[pFace[i].mtlid]++;
					dwFaceCount++;
				}
			}
			dwFacesNum = dwFaceCount;
		

			if (dwVertexNum)
			{
				pv3 = new VECTOR3[dwVertexNum];
				
				for (i=0; i<dwVertexNum; i++)
				{
				//	vp = tm * mesh->verts[i];
				//	pv3[i].x = vp.x;
				//	pv3[i].y = vp.z;
				//	pv3[i].z = vp.y;
					vp = tm * mesh->verts[i];
					pv3[i].x = mesh->verts[i].x;
					pv3[i].y = mesh->verts[i].z;
					pv3[i].z = mesh->verts[i].y;
				}
				TransformVector3_VPTR1(pv3,&nodeTM.mat4Inverse,dwVertexNum);
			}
		}
		if (dwTexVertexNum && dwTFacesNum)
		{
			pTFace = new TFACE[dwTFacesNum];

			DWORD		dwFaceCount = 0;
			for (i=0; i<dwTFacesNum; i++)
			{
				VECTOR3		vp[3];
				memcpy(&vp[0],tm * mesh->verts[mesh->faces[i].v[vx1]],sizeof(VECTOR3));
				memcpy(&vp[1],tm * mesh->verts[mesh->faces[i].v[vx3]],sizeof(VECTOR3));
				memcpy(&vp[2],tm * mesh->verts[mesh->faces[i].v[vx2]],sizeof(VECTOR3));

				if (!IsValidTri(vp+0,vp+1,vp+2))
				{
					__asm nop
				}
				else 
				{
					pTFace[i].a = mesh->tvFace[i].t[vx1];
					pTFace[i].b = mesh->tvFace[i].t[vx3];
					pTFace[i].c = mesh->tvFace[i].t[vx2];
					dwFaceCount++;
				}
			}
			dwTFacesNum = dwFaceCount;

			puv = new TVERTEX[dwTexVertexNum];

			if (dwVertexNum > dwTexVertexNum)
				__asm int 3

			
			for (i=0; i<dwTexVertexNum; i++)
			{
				puv[i].u = mesh->tVerts[i].x;
				puv[i].v = mesh->tVerts[i].y;
			}
			DWORD*	pdwExtIndex = NULL;
			DWORD	dwExtVertexNum;
			dwTexVertexNum = dwVertexNum = ReCreateVertexList(pTFace,pFace,&pv3,&puv,dwTFacesNum,dwVertexNum,&pdwExtIndex,&dwExtVertexNum);
			if (pdwExtIndex)
				delete [] pdwExtIndex;

			delete [] pTFace;
			pTFace = NULL;

			delete [] pFace;
			pFace = NULL;

		}


		if (flag.GetMotionTypeVertex() == MOTION_TYPE_VERTEX_ENABLE)
		{
			if (dwVertexNum)
			{
				TransformVector3_VPTR1(pv3,&nodeTM.mat4,dwVertexNum);
				anmKey.dwVertexNum = dwVertexNum;
				anmKey.pv3Pos = pv3;
			}
		}
		else
		{
			if (pv3)
			{
				delete [] pv3;
				pv3 = NULL;
			}
		}
			
		if (flag.GetMotionTypeUV() == MOTION_TYPE_UV_ENABLE)
		{
			if (dwTexVertexNum)
			{
				anmKey.dwTexVertexNum = dwTexVertexNum;
				anmKey.ptUV = puv;
			}
		}
		else
		{
			if (puv)
			{
				delete [] puv;
				puv = NULL;
			}
		}

		anmKey.dwTicks = t;
		anmKey.dwFrame = t / GetTicksPerFrame();
		pObj->InsertAnimatedMeshKey(&anmKey);
		dwNum++;

		////////////////////////////////////
		if (needDel)
			delete tri;

		
	}

	return TRUE;
}
BOOL CMotion::ConvertNodeTM(NODE_TM* pTM,INode* pNode,DWORD dwTime)
{
	BOOL		bResult = FALSE;

	Matrix3 pivot = pNode->GetNodeTM(dwTime);
	Matrix3 parenttm = pNode->GetParentTM(dwTime);

	DumpMatrix3(pTM,&pivot,&parenttm);
	SetInverseMatrix(&pTM->mat4Inverse,&pTM->mat4);
	bResult = TRUE;

lb_return:
	return bResult;

}

BOOL CMotion::DumpMatrix3(NODE_TM* pTM,Matrix3* m, Matrix3* pParentTM)
{
	Point3 row;

	row = m->GetRow(0);

	pTM->mat4._11 = row.x;
	pTM->mat4._12 = row.z;
	pTM->mat4._13 = row.y;
	pTM->mat4._14 = 0.0f;
	
	row = m->GetRow(2);

	pTM->mat4._21 = row.x;
	pTM->mat4._22 = row.z;
	pTM->mat4._23 = row.y;
	pTM->mat4._24 = 0.0f;

	row = m->GetRow(1);
	pTM->mat4._31 = row.x;
	pTM->mat4._32 = row.z;
	pTM->mat4._33 = row.y;
	pTM->mat4._34 = 0.0f;

	row = m->GetRow(3);
	pTM->mat4._41 = row.x;
	pTM->mat4._42 = row.z;
	pTM->mat4._43 = row.y;
	pTM->mat4._44 = 1.0f;

	// Decompose the matrix and dump the contents
	AffineParts ap;
	float rotAngle;
	Point3 rotAxis;
	float scaleAxAngle;
	Point3 scaleAxis;

	Matrix3 tm;
	if (pParentTM)
	{
		tm = (*m) * Inverse(*pParentTM);
		decomp_affine(tm, &ap);
	}
	else 
	{
		decomp_affine(*m,&ap);
	}
	
	// Quaternions are dumped as angle axis.
	AngAxisFromQ(ap.q, &rotAngle, rotAxis);
	AngAxisFromQ(ap.u, &scaleAxAngle, scaleAxis);
	
	pTM->fPosX = ap.t.x;
	pTM->fPosY = ap.t.z;
	pTM->fPosZ = ap.t.y;

	pTM->fRotAxisX = rotAxis.x;
	pTM->fRotAxisY = rotAxis.z;
	pTM->fRotAxisZ = rotAxis.y;

	pTM->fRotAng = rotAngle;
	
	pTM->fScaleAxisX = scaleAxis.x;
	pTM->fScaleAxisY = scaleAxis.z;
	pTM->fScaleAxisZ = scaleAxis.y;

	pTM->fScaleX = ap.k.x;
	pTM->fScaleY = ap.k.z;
	pTM->fScaleZ = ap.k.y;

	pTM->fScaleAngle = scaleAxAngle;

	return TRUE;
}

TriObject* CMotion::GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) { 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, 
			Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else {
		return NULL;
	}
}
BOOL CMotion::TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}

BOOL CMotion::IsKnownController(Control* cont)
{
	ulong partA, partB;

	if (!cont)
		return FALSE;

	partA = cont->ClassID().PartA();
	partB = cont->ClassID().PartB();

	if (partB != 0x00)
		return FALSE;

	switch (partA) {
		case TCBINTERP_POSITION_CLASS_ID:
		case TCBINTERP_ROTATION_CLASS_ID:
		case TCBINTERP_SCALE_CLASS_ID:
		case HYBRIDINTERP_POSITION_CLASS_ID:
		case HYBRIDINTERP_ROTATION_CLASS_ID:
		case HYBRIDINTERP_SCALE_CLASS_ID:
		case LININTERP_POSITION_CLASS_ID:
		case LININTERP_ROTATION_CLASS_ID:
		case LININTERP_SCALE_CLASS_ID:
			return TRUE;
	}

	return FALSE;
}
BOOL CMotion::CheckForAnimation(INode* node, BOOL& bPos, BOOL& bRot, BOOL& bScale,Interface* ip)
{

	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame();
	Matrix3 tm;
	AffineParts ap;
	Point3 firstPos;
	float rotAngle, firstRotAngle;
	Point3 rotAxis, firstRotAxis;
	Point3 firstScaleFactor;

	bPos = bRot = bScale = FALSE;

	for (t=start; t<=end; t+=delta) {
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));

		decomp_affine(tm, &ap);

		AngAxisFromQ(ap.q, &rotAngle, rotAxis);

		if (t != start) {
			if (!bPos) {
				if (!EqualPoint3(ap.t, firstPos)) {
					bPos = TRUE;
					}
				}
			// MAX 2.x:
			// We examine the rotation angle to see if the rotation component
			// has changed.
			// Although not entierly true, it should work.
			// It is rare that the rotation axis is animated without
			// the rotation angle being somewhat affected.
			// MAX 3.x:
			// The above did not work, I have a repro scene that doesn't export a rotation track
			// because of this. I fixed it to also compare the axis.
			if (!bRot) {
				if (fabs(rotAngle - firstRotAngle) > ALMOST_ZERO) {
					bRot = TRUE;
					}
				else if (!EqualPoint3(rotAxis, firstRotAxis)) {
					bRot = TRUE;
					}
				}

			if (!bScale) {
				if (!EqualPoint3(ap.k, firstScaleFactor)) {
					bScale = TRUE;
					}
				}
			}
		else {
			firstPos = ap.t;
			firstRotAngle = rotAngle;
			firstRotAxis = rotAxis;
			firstScaleFactor = ap.k;
			}

		// No need to continue looping if all components are animated
		if (bPos && bRot && bScale)
			break;
		}

	return bPos || bRot || bScale;
}
BOOL CMotion::ReadFile(TCHAR* uszFileName)
{
	DWORD		i;
	BOOL		bResult = FALSE;
	DWORD		size;
	DWORD		type;
	CMotionObject*	pMotObj;

	FILE*	fp = _tfopen(uszFileName,"rb");

	if (!fp)
		goto lb_return;

	FILE_MOTION_HEADER	header;
	
	fread(&header,sizeof(FILE_MOTION_HEADER),1,fp);
	for (i=0; i<header.dwMotionObjectNum; i++)
	{
		fread(&type,sizeof(DWORD),1,fp);
		pMotObj = AllocMotionObject();

		fread(&size,sizeof(DWORD),1,fp);
		pMotObj->ReadFile(fp);
	}

	fclose(fp);


lb_return:
	return bResult;

}

BOOL CMotion::WriteFile(TCHAR* uszFileName)
{
	BOOL		bResult = FALSE;

	CMotionObject*	pCurObj = m_pBaseMotionObject;

	OBJECT_TYPE type;
	DWORD		size;
	long		entry;

	FILE_MOTION_HEADER header;
	header.dwVersion = 0x00000001;
	header.dwTicksPerFrame = m_dwTicksPerFrame;
	header.dwFirstFrame = m_dwFirstFrame;
	header.dwLastFrame = m_dwLastFrame;
	header.dwFrameSpeed = m_dwFrameSpeed;
	header.dwMotionObjectNum = m_dwMotionObjectNum;
	header.dwKeyFrameStep = m_dwKeyFrameStep;
	_tcscpy(header.uszMotionName,m_uszMotionName);

	FILE* fp = _tfopen(uszFileName,"wb");
	if (!fp)
		goto lb_return;

	fwrite(&header,sizeof(FILE_MOTION_HEADER),1,fp);

	// 매터리얼 write
	type = OBJECT_TYPE_MOTION;
	while (pCurObj)
	{
		fwrite(&type,sizeof(DWORD),1,fp);
		entry = ftell(fp);
		fseek(fp,4,SEEK_CUR);

		size = pCurObj->WriteFile(fp);

		fseek(fp,entry,SEEK_SET);
		fwrite(&size,sizeof(DWORD),1,fp);
		fseek(fp,size,SEEK_CUR);

		pCurObj = pCurObj->m_pNextMotionObject;
	}
lb_return:
	fclose(fp);

	return bResult;
}
	
void CMotion::ReleaseObjects()
{

	CMotionObject* pCur = m_pBaseMotionObject;
	CMotionObject* pNext;
	
	while (pCur)
	{
		pNext = pCur->m_pNextMotionObject;
		delete pCur;
	
		pCur = pNext;
	}
	m_pBaseMotionObject = NULL;
	m_dwMotionObjectNum = 0;
	
	
}

CMotion::~CMotion()
{
	ReleaseObjects();
}

DWORD ReCreateVertexList(TFACE* ptFace,FACE* pFace,VECTOR3** ppv3,TVERTEX** pptv,DWORD dwTFacesNum,DWORD dwVertexNum,DWORD** ppdwExtVertexIndex,DWORD* pdwExtVertexNum)
{
	DWORD		dwTotalVertexNum = 0;

	VECTOR3*	pv3 = *ppv3;
	TVERTEX*	ptv = *pptv;

	DWORD		i;
	DWORD		dwExtVertexCount = 0;

	POINT3D_REF_TVERTEX*	pvRef = new POINT3D_REF_TVERTEX[dwTFacesNum*3];

	POINT3D_REF_TVERTEX*	pvRefExt = pvRef+dwVertexNum;

	DWORD*		pOriginalIndex = new DWORD[dwTFacesNum*3];
	
	memset(pvRef,0,sizeof(POINT3D_REF_TVERTEX)*dwTFacesNum*3);
	for (i=0; i<dwTFacesNum*3; i++)
		pvRef[i].wIndex = (WORD)i;
	
	for (i=0; i<dwTFacesNum; i++)
	{
		ReIndexFace(&pFace[i].a,ptFace[i].a,pvRef,pvRefExt,pv3,ptv,&dwExtVertexCount,pOriginalIndex);
		ReIndexFace(&pFace[i].b,ptFace[i].b,pvRef,pvRefExt,pv3,ptv,&dwExtVertexCount,pOriginalIndex);
		ReIndexFace(&pFace[i].c,ptFace[i].c,pvRef,pvRefExt,pv3,ptv,&dwExtVertexCount,pOriginalIndex);
	}

	dwTotalVertexNum = dwVertexNum+dwExtVertexCount;
	*pdwExtVertexNum = dwExtVertexCount;

	VECTOR3*	pvPoint = new VECTOR3[dwTotalVertexNum];

	TVERTEX*	ptUVW = new TVERTEX[dwTotalVertexNum];

	DWORD		noncount1 = 0;
	DWORD		noncount2 = 0;
	for (i=0; i<dwTotalVertexNum; i++)
	{
		if (!pvRef[i].pv3Point)
			noncount1++;
		else 
			pvPoint[i] = *pvRef[i].pv3Point;
		
		if (!pvRef[i].ptv)
			noncount1++;
		else 
			ptUVW[i] = *pvRef[i].ptv;
	}

	delete [] pv3;
	delete [] ptv;
	delete [] pvRef;

	if (dwExtVertexCount)
	{
		*ppdwExtVertexIndex = new DWORD[dwExtVertexCount];
		memcpy(*ppdwExtVertexIndex,pOriginalIndex,sizeof(DWORD)*dwExtVertexCount);
	}

	delete [] pOriginalIndex;

	*ppv3 = pvPoint;
	*pptv = ptUVW;

	return dwTotalVertexNum;
}
void ReIndexFace(WORD* pFaceIndex,WORD dwTFaceIndex,POINT3D_REF_TVERTEX* pvRef,POINT3D_REF_TVERTEX* pvRefExt,VECTOR3* pv3,TVERTEX* ptv,DWORD* pCount,DWORD* pOriginalIndex)
{
	POINT3D_REF_TVERTEX* pvRefCur;
	POINT3D_REF_TVERTEX* pvRefPrv;


	if (!pvRef[*pFaceIndex].ptv)
	{
		pvRef[*pFaceIndex].ptv = ptv + dwTFaceIndex;
		pvRef[*pFaceIndex].pv3Point = pv3+*pFaceIndex;
		if (!pvRef[*pFaceIndex].pv3Point)
			__asm int 3

		goto lb_return;
	}
	pvRefCur = &pvRef[*pFaceIndex];
	
	while(pvRefCur)
	{
		pvRefPrv = pvRefCur;
		if ((pvRefCur->ptv == ptv + dwTFaceIndex) && (pvRefCur->pv3Point == pv3 + *pFaceIndex))
		{
			*pFaceIndex = pvRefCur->wIndex;
			goto lb_return;
		}
		pvRefCur = pvRefCur->pNext;
	}
	pvRefPrv->pNext = pvRefExt + *pCount;
	
	pvRefExt[*pCount].ptv = ptv + dwTFaceIndex;
	pvRefExt[*pCount].pv3Point = pv3 + *pFaceIndex;
	if (!pvRef[*pFaceIndex].pv3Point)
			__asm int 3
	
	pOriginalIndex[*pCount] = (DWORD)(*pFaceIndex);
	
	*pFaceIndex = pvRefExt[*pCount].wIndex; 
	(*pCount)++;

lb_return:
	
	return;
}
