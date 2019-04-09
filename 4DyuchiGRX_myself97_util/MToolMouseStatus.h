// MToolMouseStatus.h: interface for the CToolMouseStatus class.
//
//////////////////////////////////////////////////////////////////////
/*
	µ¶¸³ÀûÀ¸·Î µ¹¾Æ°¥ ¼ö ÀÖ´Â Å¬·¡½º´Â ¾Æ´Ï´Ù.
*/
#if !defined(AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_)
#define AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGRX_common/IGeometry.h"



class MToolMouseStatus  
{
protected:		// º¯¼ö±º.
	int		miOldMouseX;
	int		miOldMouseY;
	int		miMouseX;
	int		miMouseY;
	int		miMouseZ;		// ÈÙ.

	float	mfMoveHorizonStep;
	float	mfMoveVerticalStep;
	float	mfRotateStep;	// Shift¿¡ ÀÇÇØ »óÅÂ°¡ ´Þ¶óÁö´Â È¸Àü, ÀÌµ¿ ¼Óµµ°ª.

	bool	mbLButtonDown;
	bool	mbRButtonDown;
	bool	mbMButtonDown;
	bool	mbCtrlDown;
	bool	mbShiftDown;		// ½¬ÇÁÆ® ¹öÆ°Àº °í¼Ó ÀÌµ¿.

	DWORD	mdwMouseMode;

	I4DyuchiGXGeometry*			mpGeometry;

	BOOL	mbDoNotRotate;
public:

	// ÃÊ±âÈ­ ÇÒ¶§ ²À ³Ö¾îÁà¾ßÇÑ´Ù.
	BOOL	Initialize(I4DyuchiGXGeometry* pGeometry);

	// ·ÎÅ×ÀÌÆ® ¸ðµå°¡ WithPivotÀÏ¶§ ±âÁØÃà.
	void SetRotationPivot( VECTOR3* pPivot);
	// WM_MOUSEMOVE ¸Þ½ÃÁö¸¦ ¹Þ¾ÒÀ»¶§ È£ÃâÇÒ ÇÔ¼ö. dwFlagÀº ¹öÆ° »óÅÂ.
	bool MoveMouse( DWORD dwFlag, int iMouseX, int iMouseY);

	void	DoNotRotate(void);
	void	AllowRotate(void);

	void	PushCameraAngleAndPos(DWORD	dwStackNum);
	void	PopCameraAngleAndPos(DWORD	dwStackNum);
	DWORD	GetMouseMode() {return mdwMouseMode;}
	MToolMouseStatus();
	virtual ~MToolMouseStatus();



	VECTOR3		mpCameraStackPos[10];
	float		mpfCameraStackFromTo[10];
	VECTOR3		mpCameraStackAngle[10];

	VECTOR3		mRotationPivot;

protected:		// ÇÔ¼ö±º.
	bool MouseNone(void);
	bool MouseRotate(void);
	bool MouseMoveVertical(void);
	bool MouseMoveHorizon(void);
	bool MouseRotateWithPivot(void);


};

#endif // !defined(AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_)
