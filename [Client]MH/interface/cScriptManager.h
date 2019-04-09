// cScriptManager.h: interface for the cScriptManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSCRIPTMANAGER_H__B9C3CDD2_6869_4A8A_801C_BA26B097B799__INCLUDED_)
#define AFX_CSCRIPTMANAGER_H__B9C3CDD2_6869_4A8A_801C_BA26B097B799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "cWindowDef.h"

//2007. 5. 18. Parse Type 관련 헤더파일 추가
#include "ClientGameDefine.h"
#include "ClientGameStruct.h"
//

class cWindow;
class cImage;
class CMHFile;


#define SCRIPTMGR	USINGTON(cScriptManager)
enum ePATH_FILE_TYPE
{
	PFT_HARDPATH = 0,
	PFT_ITEMPATH	,
	PFT_MUGONGPATH	,
	PFT_ABILITYPATH ,
	PFT_BUFFPATH ,
	PFT_MINIMAPPATH ,
	PFT_JACKPOTPATH,
};

struct sIMAGHARDPATH
{
	int	idx;
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
};

class cScriptManager  
{
public:
//	MAKESINGLETON(cScriptManager);
	cScriptManager();
	virtual ~cScriptManager();

	cWindow * GetDlgInfoFromFile(char * filePath, char* mode = "rt");

	cWindow * GetInfoFromFile(cWindow * wnd, CMHFile * fp);
	cWindow * GetCtrlInfo(cWindow * wnd, char * buff, CMHFile * fp);

	void GetImage( int idx, cImage * pImage, cImageRect * rect );
	void GetImage( int hard_idx, cImage * pImage , ePATH_FILE_TYPE type = PFT_HARDPATH );

	void CheckPosForResolution(cPoint* pPos);

	//////////// 2007. 5. 18. Parse 관련 추가 //////////////////
	BOOL InitParseTypeData();		//Parse Type 데이터 초기화 및 로드
	void ReleaseParseTypeData();	//Parse Type 데이터 삭제
	////////////////////////////////////////////////////////////

	void InitScriptManager();

	// 2007. 11. 1. CBH - IMAGEPATH 테이블 메모리 해제 함수
	void cScriptManager::ReleaseImagePathTable();
protected:
	void GetImage( CMHFile * fp, cImage * pImage );
	char * GetMsg( int idx );

	//////////// 2007. 5. 18. Parse 관련 추가 /////////////
	int ChangeHashKey(const char* cpTypeName, int nMaxNum);		//문자열을 해쉬키값으로 바꾸는 함수
	PARSE_TYPE GetParseType(const char* str);		//해당 파싱 문자열에 해당하는 아이디 값 반환
	CYHHashTable<PARSE_TYPE_INFO> m_ParseTypeTable; //파싱된 문자 타입저장 해쉬테이블	
	///////////////////////////////////////////////////////

	CYHHashTable<sIMAGHARDPATH> m_ImageHardPath;
	CYHHashTable<sIMAGHARDPATH>	m_ItemHardPath;
	CYHHashTable<sIMAGHARDPATH> m_MugongHardPath;
	CYHHashTable<sIMAGHARDPATH> m_AbilityHardPath;
	CYHHashTable<sIMAGHARDPATH>	m_BuffHardPath;
	CYHHashTable<sIMAGHARDPATH> m_MiniMapHardPath;
	CYHHashTable<sIMAGHARDPATH> m_JackPotHardPath;
};
EXTERNGLOBALTON(cScriptManager);
#endif // !defined(AFX_CSCRIPTMANAGER_H__B9C3CDD2_6869_4A8A_801C_BA26B097B799__INCLUDED_)
