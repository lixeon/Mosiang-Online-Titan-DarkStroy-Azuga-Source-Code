#include "UserDefine.h"
#include "../4DyuchiGRX_Common/motion_flag.h"
#include "../4DyuchiGRX_Common/mesh_flag.h"

void CreateMeshTextItem(char* txt,CMeshFlag flag,char* szObjName)
{
	char			szflag[32];
	memset(szflag,0,sizeof(szflag));
	
		
	switch (flag.GetShadeType())
	{
	case SHADE_TYPE_VERTEX_LIGHT_IM:
		szflag[1] = 'I';
		break;
		
	case SHADE_TYPE_VERTEX_LIGHT_RT:
		szflag[1] = 'R';
		break;

	case SHADE_TYPE_LIGHT_MAP:
		szflag[1] = 'L';
		break;
	}

	switch (flag.GetTransformType())
	{
	case TRANSFORM_TYPE_ALIGN_VIEW:
		szflag[2] = 'B';
		break;
	default:
		szflag[2] = ' ';
		break;
	}
	szflag[3] = ' ';
	if (flag.IsDisableZBubfferWrite())
	{
		szflag[4] = 'Z';
		szflag[5] = 'N';
	}
	else
	{
		szflag[4] = 'Z';
		szflag[5] = 'W';
	}
	szflag[6] = ' ';
	
	wsprintf(&szflag[7],"%3d",flag.GetRenderZPriorityValue());
	
	/*
	switch (pItem->meshFlag.GetTransformType())
	{
	case TRANSFORM_TYPE_NOT_TR:
		flag[3] = 'N';
		break;
	case TRANSFORM_TYPE_D3D_TR:
		flag[3] = 'D';
		break;
	case TRANSFORM_TYPE_SOFT_TR:
		flag[3] = 'S';
		break;
	}*/
	szflag[0] = '-';

	DWORD strLen = lstrlen(szObjName);
	lstrcpy(txt,szObjName);
	lstrcpy(txt+strLen,szflag);
}

void BuildMeshFlag(CMeshFlag* pMeshFlag,char* szBuffer,char* szObjName)
{
	CMeshFlag	flag;

	char seps[]   = " ,\t\n";
	char*	token;
	
	token = strtok( szBuffer, seps );
	while( token != NULL )
	{
		if (!lstrcmp(token,BILLBOARD))
		{
			flag.SetShadeType(SHADE_TYPE_VERTEX_LIGHT_IM);
			flag.SetTransformType(TRANSFORM_TYPE_ALIGN_VIEW);
		}
		else if (!lstrcmp(token,VERTEXLIGHT))
		{
			flag.SetShadeType(SHADE_TYPE_VERTEX_LIGHT_IM);

		}
		else if (!lstrcmp(token,LIGHTMAPLIGHT))
		{
			flag.SetShadeType(SHADE_TYPE_LIGHT_MAP);
		}

		token = strtok( NULL, seps );
	}
	flag.SetRenderZPriorityValue(RENDER_ZPRIORITY_DEFAULT);
	*pMeshFlag = flag;

}




void CreateMotionTextItem(char* txt,CMotionFlag flag,char* szObjName)
{
	char			szflag[5];
	szflag[4] = 0;
	
	switch (flag.GetMotionTypeKeyFrame())
	{
	case MOTION_TYPE_KEYFRAME_ENABLE:
		szflag[1] = 'K';
		break;
	case MOTION_TYPE_KEYFRAME_DISABLE:
		szflag[1] = 'X';
	}
	switch (flag.GetMotionTypeVertex())
	{
	case MOTION_TYPE_VERTEX_DISABLE:
		szflag[2] = 'X';
		break;
	case MOTION_TYPE_VERTEX_ENABLE:
		szflag[2] = 'V';
		break;
	}
	
	switch (flag.GetMotionTypeUV())
	{
	case MOTION_TYPE_UV_DISABLE:
		szflag[3] = 'X';
		break;
	case MOTION_TYPE_UV_ENABLE:
		szflag[3] = 'U';
		break;
	}
	szflag[0] = '-';

	DWORD strLen = lstrlen(szObjName);
	lstrcpy(txt,szObjName);
	lstrcpy(txt+strLen,szflag);
}

void BuildMotionFlag(CMotionFlag* pMotionFlag,char* szBuffer,char* szObjName)
{
	CMotionFlag	flag;

	char seps[]   = " ,\t\n";
	char*	token;
	
	token = strtok( szBuffer, seps );
	if (token)
	{
			
		while( token != NULL )
		{
			if (!lstrcmp(token,KEYFRAME))
			{
				flag.SetMotionTypeKeyFrame(MOTION_TYPE_KEYFRAME_ENABLE);
				flag.SetMotionTypeVertex(MOTION_TYPE_VERTEX_DISABLE);
			}
			else if (!lstrcmp(token,VERTEXANI))
			{
				flag.SetMotionTypeVertex(MOTION_TYPE_VERTEX_ENABLE);
				flag.SetMotionTypeKeyFrame(MOTION_TYPE_KEYFRAME_DISABLE);

			}
			else if (!lstrcmp(token,UVANI))
			{
				flag.SetMotionTypeUV(MOTION_TYPE_UV_ENABLE);
			}

			token = strtok( NULL, seps );
		}
		*pMotionFlag = flag;
	}
}
