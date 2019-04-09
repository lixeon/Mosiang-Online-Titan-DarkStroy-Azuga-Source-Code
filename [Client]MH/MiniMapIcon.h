// MiniMapIcon.h: interface for the CMiniMapIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIMAPICON_H__FC0A3BA0_83BA_4F40_A70F_137803C0AA71__INCLUDED_)
#define AFX_MINIMAPICON_H__FC0A3BA0_83BA_4F40_A70F_137803C0AA71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum eQUEST_MARK
{
	eQM_NONE,
	eQM_EXCLAMATION,
	eQM_QUESTION,
	eQM_MAX,
};


class cImage;

class CMiniMapIconImage
{
public:
	cImage* m_pImage;
	VECTOR2 m_ImageWH;

public:
	CMiniMapIconImage()
	{
		m_pImage = NULL;
	}
	~CMiniMapIconImage();
	void InitIconImage(cImage* pImage,DWORD w,DWORD h)
	{
		m_pImage = pImage;
		m_ImageWH.x = (float)w;
		m_ImageWH.y = (float)h;
	}
	
};

class CMiniMapIcon
{
protected:
	CMiniMapIconImage* m_pIconImage;

	CMiniMapIconImage* m_pMarkExclamation[3];
	CMiniMapIconImage* m_pMarkQuestion[3];

	int		m_ShowQuestMark;
	
	CObject* m_pObject;
	VECTOR3 m_ObjectPos;

	VECTOR2 m_ScreenPos;

	int		m_Kind;
	VECTOR3 m_TargetPos;
	int		m_Interpolation;
	char	m_MemberName[MAX_NAME_LENGTH+1];
	
	DWORD	m_dwColor;
	BOOL	m_bAlwaysTooltip;
	
public:
	CMiniMapIcon();
	virtual ~CMiniMapIcon();

	void Init(CMiniMapIconImage* pIconImage, int Kind );
	void Init(CMiniMapIconImage* pIconImage,CObject* pObject, int Kind);
	void Init(CMiniMapIconImage* pIconImage,VECTOR3* pObjectPos, int Kind);
	void Init(CMiniMapIconImage* pIconImage,PARTY_MEMBER* pMember, int Kind);
	virtual void Update(DWORD MinimapImageWidth,DWORD MinimapImageHeight);
	virtual void Render(VECTOR2* pMapStartPixel,RECT* pCullRect);

	void RenderQuestMark(VECTOR2* pMapStartPixel,RECT* pCullRect);
	
	int GetToolTip(int x,int y,VECTOR2* pMapStartPixel,char* pOutText, float* distOut = NULL );

	VECTOR3 GetPosition();
	VECTOR2* GetScreenPos()			{	 return &m_ScreenPos;	}

	void SetRGBColor( DWORD dwColor ) { m_dwColor = dwColor; }
	void SetAlwaysTooltip( BOOL bAlways ) { m_bAlwaysTooltip = bAlways;	}

	void SetObjectPos(DWORD PosX, DWORD PosZ)		{ m_ObjectPos.x = (float)PosX;	m_ObjectPos.z = (float)PosZ;	}
	void SetTargetPos(DWORD PosX, DWORD PosZ)		{ m_TargetPos.x = (float)PosX;	m_TargetPos.z = (float)PosZ;	}

	void SetMarkImage(CMiniMapIconImage* pMarkExclamation, CMiniMapIconImage* pMarkQuestion);
	void ShowQuestMark( int nKind ) { m_ShowQuestMark = nKind; }

};
#endif // !defined(AFX_MINIMAPICON_H__FC0A3BA0_83BA_4F40_A70F_137803C0AA71__INCLUDED_)
