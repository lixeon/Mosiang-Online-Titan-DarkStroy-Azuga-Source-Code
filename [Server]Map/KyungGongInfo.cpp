// KyungGongInfo.cpp: implementation of the CKyungGongInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KyungGongInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKyungGongInfo::CKyungGongInfo()
{

}

CKyungGongInfo::~CKyungGongInfo()
{

}

void CKyungGongInfo::Init(CMHFile* pFile)
{	
	m_Info.KyungGongIdx = pFile->GetWord();		//무공 index
	//pFile->GetString(m_Info.KyungGongName);		//경공이름
	SafeStrCpy( m_Info.KyungGongName, pFile->GetString(), MAX_NAME_LENGTH+1 );
	m_Info.NeedNaeRyuk = pFile->GetWord();		//소모 내력
	m_Info.MoveType = pFile->GetWord();			//이동 타입
	m_Info.Speed = pFile->GetFloat();			//이동 속도
	m_Info.ChangeTime = pFile->GetWord();		//경공 모드 전환 시간
	m_Info.StartEffect = pFile->GetWord();		//경공 사용 연출
	m_Info.IngEffect = pFile->GetWord();			//경공 연출 파일
	m_Info.EndEffect = pFile->GetWord();			//경공 끝 연출
}