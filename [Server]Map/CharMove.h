// CharMove.h: interface for the CCharMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARMOVE_H__50E4C329_4F72_4B84_8EC6_9BE4326B42B7__INCLUDED_)
#define AFX_CHARMOVE_H__50E4C329_4F72_4B84_8EC6_9BE4326B42B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObject;
class CPlayer;

class CCharMove  
{
	// ÇöÀç À§Ä¡¸¦ °è»êÇØ¼­ Move_Position¿¡ ¼ÂÆÃÇÏ°í ³Ñ°ÜÁØ´Ù.
	// °è¼ÓÇØ¼­ °è»êÇÏÁö ¾Ê°í ÇÊ¿äÇÒ¶§¸¸ °è»êÇÑ´Ù.
	// Move_bMovingÀÌ FALSEÀÌ¸é ¹Ù·ç ¸®ÅÏÇÑ´Ù.
	// ½Ã°£ º¯¼ö¿¡ ´ëÇÑ OverFlow¸¦ °í·ÁÇØ¾ß ÇÑ´Ù.
	//static VECTOR3* CalcPosition(CObject* pObject,DWORD CurTime);
	static VECTOR3* CalcPositionEx(CObject* pObject,DWORD CurTime);
public:
	CCharMove();
	virtual ~CCharMove();

	static void InitMove(CObject* pObject,VECTOR3* pPos);
	static void ReleaseMove(CObject* pObject);

	static VECTOR3* GetPosition(CObject* pObject);
	static void SetPosition(CObject* pObject,VECTOR3* pPos);	// Move_bMovingÀÌ TRUE ÀÌ ¾Æ´Ò¶§¿¡¸¸ °¡´É.. ASSERT(Move_bMoving == FALSE); °É¾îµÎÀÚ

	// ÇöÀç À§Ä¡¸¦ °è»êÇØ¼­ EndPos¿Í Â÷ÀÌ°¡ ¸¹ÀÌ ³ª¸é Á¶Ä¡-_-;¸¦ ÃëÇÑ´Ù.
	// ÀÌµ¿ÀÌ ³¡³µÀ½À» Ç¥½ÃÇÑ´Ù.
	static void EndMove(CObject* pObject,DWORD CurTime,VECTOR3* EndPos);
	
	static void StartMoveEx(CObject* pObject,DWORD CurTime,VECTOR3* StartPos,VECTOR3* pTargetPos);
	
	// ÀÌµ¿ÁßÀÎ »óÅÂ·Î Ç¥½ÃÇÏ°í ½ÃÀÛÁöÁ¡°ú ³¡ÁöÁ¡À» ±â¾ïÇØ³õ´Â´Ù.
	// ÀÌµ¿½ÃÀÛ½Ã°£µµ ±â¾ïÇØ³õ´Â´Ù.
	// ÇöÀçÀ§Ä¡¸¦ °è»êÇØºÁ¼­ StartPos ¶û Â÷ÀÌ°¡ ¸¹ÀÌ ³ª¸é Á¶Ä¡-_-;¸¦ ÃëÇÑ´Ù.
	//static void StartMove(CObject* pObject,DWORD CurTime,VECTOR3* StartPos,VECTOR3* TargetPos);

	static void MoveProcess(CObject* pObject);
	static BOOL IsMoving(CObject* pObject);
	
	static void SetWalkMode(CObject* pObject);
	static void SetRunMode(CObject* pObject);
	static BOOL SetKyungGong(CObject* pObject,WORD KGIdx);

	static BOOL IsValidKyungGong(CObject* pObject,WORD KGIdx);
		
	static void CorrectPlayerPosToServer(CObject* pObject);
	
	static DWORD GetMoveEstimateTime(CObject* pObject);
	
//	static BOOL CheckVaildStop(CPlayer* pPlayer,DWORD CurTime,VECTOR3* EndPos);
	static BOOL CheckVaildStop(CObject* pObject,DWORD CurTime,VECTOR3* EndPos);
	
	static void ForceStopKyungGong(CPlayer* pPlayer);
	
	static void Warp(CObject* pObject, VECTOR3* vPos );
};

#endif // !defined(AFX_CHARMOVE_H__50E4C329_4F72_4B84_8EC6_9BE4326B42B7__INCLUDED_)
