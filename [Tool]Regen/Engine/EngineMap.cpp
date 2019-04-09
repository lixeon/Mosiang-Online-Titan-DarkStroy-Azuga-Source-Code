// EngineMap.cpp: implementation of the EngineMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineMap.h"
//#include "../MainGame.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngineMap::CEngineMap()
{
	m_bMapBuild = FALSE;
}

CEngineMap::~CEngineMap()
{
	//Release();
}

BOOL CEngineMap::Init(char* szMapFile)
{
	//return g_pExecutive->LoadMapScript(szMapFile,MapLoadCallback,0);
	return g_pExecutive->LoadMapScript(szMapFile,NULL,0);
	m_pExecutive = g_pExecutive;
	
	BOOL	bBuildTree	=	FALSE;	

	char	buf[128];

	FILE*	fp;
	fp = fopen(szMapFile,"rt");

	while (fscanf(fp,"%s",buf)!=EOF)
	{
		if( !lstrcmp( buf, PID_GX_METADATA))
		{
			while( lstrcmp( buf, "}"))
			{
				fscanf( fp, "%s", buf);
				if( !lstrcmp( buf, PID_BOUNDINGBOX_MAX))
				{
					fscanf( fp, "%f %f %f", &m_WorldBox.Max.x, &m_WorldBox.Max.y, &m_WorldBox.Max.z);
					
				}
				else if( !lstrcmp( buf, PID_BOUNDINGBOX_MIN))
				{
					fscanf( fp, "%f %f %f", &m_WorldBox.Min.x, &m_WorldBox.Min.y, &m_WorldBox.Min.z);
				}

			}
		}
		if (!lstrcmp(buf,PID_GX_MAP))
		{
			while (lstrcmp(buf,"}"))
			{
				fscanf(fp,"%s",buf);
				if (!lstrcmp(buf,PID_STATIC_MODEL))
				{
					fscanf(fp,"%s",buf);
					ImportStaticModel(buf);
					bBuildTree	=	TRUE;
				}
				if (!lstrcmp(buf,PID_HEIGHT_FIELD))
				{
					fscanf(fp,"%s",buf);
					ImportHeightField(buf,0);
					bBuildTree	=	TRUE;
				}


			}
			if (bBuildTree)
				BuildMap(&m_WorldBox);

			
		}

		if (!lstrcmp(buf,PID_GX_OBJECT))
		{
			DWORD	dwGXObjectsNum;
			fscanf(fp,"%u",&dwGXObjectsNum);

			fscanf(fp,"%s",buf);		// {
			for (DWORD i=0; i<dwGXObjectsNum; i++)
			{
				DWORD		dwFlag;
				DWORD		dwID;
				VECTOR3		v3Scale,v3Pos,v3Axis;
				float		fRad;

				// | 파일명 | 고유ID | 스케일 | 위치 | 축벡터 | 방향각 | 플래그 
				fscanf(fp,"%s",buf);		// 파일명
				fscanf(fp,"%u",&dwID);		// 고유ID
				fscanf(fp,"%f %f %f",&v3Scale.x,&v3Scale.y,&v3Scale.z);
				fscanf(fp,"%f %f %f",&v3Pos.x,&v3Pos.y,&v3Pos.z);
				fscanf(fp,"%f %f %f %f",&v3Axis.x,&v3Axis.y,&v3Axis.z,&fRad);
				fscanf(fp,"%x",&dwFlag);
				
				GXOBJECT_HANDLE gxo = m_pExecutive->CreateGXObject(buf,NULL,NULL,dwFlag);

				//////////////////////////////////////////////////////////////////////////
				// yh edit
				DWORD motionNum = m_pExecutive->GXOGetMotionNum(gxo,0);
				if(motionNum > 0)
				{
					m_pExecutive->DeleteGXObject(gxo);
					gxo = m_pExecutive->CreateGXObject(buf,NULL,NULL,dwFlag|GXOBJECT_CREATE_TYPE_DEFAULT_PROC);
				}
				//////////////////////////////////////////////////////////////////////////

				if (gxo)
				{
					m_pExecutive->SetID(gxo,dwID);
									
					m_pExecutive->GXOSetScale(gxo,&v3Scale);
					m_pExecutive->GXOSetPosition(gxo,&v3Pos, FALSE);
					m_pExecutive->GXOSetDirection(gxo,&v3Axis,fRad);

					/*
					char*	pFileName;
					pFileName = (char*)m_pExecutive->GetData(gxo);
					pFileName = new char[lstrlen(buf)+1];
					lstrcpy(pFileName,buf);
					m_pExecutive->SetData(gxo,pFileName);
					*/


					m_GXObjectList.AddTail(gxo);
				}
			}
			fscanf(fp,"%s",buf);		// }
		}
		if (!lstrcmp(buf,PID_GX_LIGHT))
		{
			DWORD	dwGXLightsNum;
			fscanf(fp,"%u",&dwGXLightsNum);

		
			fscanf(fp,"%s",buf);		// {
			for (DWORD i=0; i<dwGXLightsNum; i++)
			{
				
				LIGHT_DESC	lightDesc;
				memset(&lightDesc,0,sizeof(LIGHT_DESC));
				DWORD		dwFlag;

				// | RGBA | 위치 |범위(CM) | 플래그 
				fscanf(fp,"%x",&lightDesc.dwDiffuse);
				fscanf(fp,"%f %f %f %f",&lightDesc.v3Point.x,&lightDesc.v3Point.y,&lightDesc.v3Point.z,&lightDesc.fRs);
				fscanf(fp,"%x",&dwFlag);
				dwFlag |= GXLIGHT_TYPE_STATIC;

				GXLIGHT_HANDLE	gxl = m_pExecutive->CreateGXLight(&lightDesc,NULL,NULL,0xffffffff,NULL,dwFlag);


				if(gxl)
				{
					m_GXLightList.AddTail(gxl);
				}
			}
			fscanf(fp,"%s",buf);		// }
		}
		if (!lstrcmp(buf,PID_GX_TRIGGER))
		{
			DWORD	dwGXTriggerNum;
			fscanf(fp,"%u",&dwGXTriggerNum);

		
			fscanf(fp,"%s",buf);		// {
			for (DWORD i=0; i<dwGXTriggerNum; i++)
			{
				EVENT_TRIGGER_DESC	triggerDesc;
				memset(&triggerDesc,0,sizeof(triggerDesc));
				DWORD		dwFlag;
				DWORD		dwID;

					
				// 고유아이디 | RGBA | 위치 | 스케일 | 회전 | 플래그 
				fscanf(fp,"%u",&dwID);
				fscanf(fp,"%x",&triggerDesc.dwColor);
				fscanf(fp,"%f %f %f",&triggerDesc.v3Pos.x,&triggerDesc.v3Pos.y,&triggerDesc.v3Pos.z);
				fscanf(fp,"%f %f %f",&triggerDesc.v3Scale.x,&triggerDesc.v3Scale.y,&triggerDesc.v3Scale.z);
				fscanf(fp,"%f %f %f",&triggerDesc.v3Rot.x,&triggerDesc.v3Rot.y,&triggerDesc.v3Rot.z);
				fscanf(fp,"%x",&dwFlag);

				GXTRIGGER_HANDLE	gxt = m_pExecutive->CreateGXEventTrigger(NULL,NULL,dwFlag);
				m_pExecutive->SetID(gxt,dwID);
				m_pExecutive->GXTSetPosition(gxt,&triggerDesc.v3Pos);
				m_pExecutive->GXTSetScale(gxt,&triggerDesc.v3Scale);
				m_pExecutive->GXTSetRotation(gxt,&triggerDesc.v3Rot);
			}
			fscanf(fp,"%s",buf);		// }
				
		}
		
	} 


	fclose(fp);

	return m_bMapBuild;
}

void CEngineMap::Release()
{
	if(g_pExecutive)
	{
		//g_pExecutive->DeleteAllGXMapObjectsWitLoadMapScript();
		GXOBJECT_HANDLE gxo;
		GXLIGHT_HANDLE gxl;
		while(gxo = m_GXObjectList.RemoveTail())
		{
			g_pExecutive->DeleteGXObject(gxo);
		}
		while(gxl = m_GXLightList.RemoveTail())
		{
			g_pExecutive->DeleteGXLight(gxl);
		}

		CleanHeightField();
		CleanStaticModel();
		g_pExecutive->DeleteGXMap(m_ghMap);		
	}
}

void CEngineMap::SetAmbientColor(DWORD Color)
{
	g_pExecutive->GetGeometry()->SetAmbientColor(0,Color);
}



BOOL CEngineMap::ImportHeightField(char* szFileName,DWORD dwIndexBufferNum)
{
	
	TEXTURE_TABLE*	pTexTable = NULL;
	BOOL			bResult = FALSE;
	
	CleanHeightField();

	bResult = m_pExecutive->GetGeometry()->CreateHeightField(&m_pHField,0);

	if (!m_pHField->ReadFile(szFileName,dwIndexBufferNum,NULL,0))
	{
		m_pHField->Release();
		m_pHField = NULL;
		memset(m_szHFieldName,0,sizeof(m_szHFieldName));
		return FALSE;
	}
	lstrcpy(m_szHFieldName,szFileName);


	m_pHField->GetHFieldDesc(&m_hfDesc);
	
//	for (DWORD i=0; i<4; i++)
//		ResetDefaultCamera(i);

	m_pExecutive->GetRenderer()->SetLightMapFlag(0);

	return bResult;

}

BOOL CEngineMap::ImportStaticModel(char* szFileName)
{
	I3DStaticModel*	pStaticModel = NULL;
	
	if (0xffffffff == m_pExecutive->GetGeometry()->CreateStaticModel(&pStaticModel,4000,500,0))
		return FALSE;


	if (!pStaticModel->ReadFile(szFileName,NULL,STATIC_MODEL_LOAD_ENABLE_SHADE))
	{
		pStaticModel->Release();
		return FALSE;
	}

	if (m_ghMap)
	{
		m_pExecutive->DeleteGXMap(m_ghMap);
		m_ghMap = NULL;
	}

	CleanStaticModel();

	m_pStaticModel = pStaticModel;
	m_pStaticModel->BeginCreateMesh(0);

	return TRUE;
}

void CEngineMap::CleanHeightField()
{	
	if (m_pHField)
	{
		m_pHField->Release();
		m_pHField = NULL;
	}

	memset(&m_hfDesc,0,sizeof(m_hfDesc));
	
	m_bMapBuild = FALSE;
}
void CEngineMap::CleanStaticModel()
{
	if (m_pStaticModel)
	{
		m_pStaticModel->Release();
		m_pStaticModel = NULL;
	}
	m_bMapBuild = FALSE;
}

BOOL CEngineMap::BuildMap(MAABB* pWorldBox)
{
	if (m_bMapBuild)
	{
		m_pExecutive->RebuildMap(pWorldBox->Max.y, pWorldBox->Min.y);
		m_WorldBox	=	*pWorldBox;
		return TRUE;

	}
	if( (pWorldBox->Max.y - pWorldBox->Min.y) <= 0.0f)
	{
		pWorldBox->Max.y	=	pWorldBox->Min.y	+	100.0f;
	}
	m_WorldBox.Max.y	=	pWorldBox->Max.y;
	m_WorldBox.Min.y	=	pWorldBox->Min.y;

	m_ghMap = m_pExecutive->CreateGXMap(NULL,0,NULL);

	m_pExecutive->BeginBuildMap(0);

	if (m_pStaticModel)
		m_pExecutive->InsertStaticModelTOGXMap(m_pStaticModel);
	
	if (m_pHField)
		m_pExecutive->InsertHFieldToGXMap(m_pHField);

	m_pExecutive->EndBuildMap(pWorldBox->Max.y,pWorldBox->Min.z);

	m_pExecutive->GetWorldBoundingBox( &m_WorldBox);

	m_bMapBuild = TRUE;
	return TRUE;
}