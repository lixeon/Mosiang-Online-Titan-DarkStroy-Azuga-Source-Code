// MPMissionDialog.h: interface for the CMPMissionDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPMISSIONDIALOG_H__A644A86F_7F3B_4324_8D3F_1FA13F9ED0BB__INCLUDED_)
#define AFX_MPMISSIONDIALOG_H__A644A86F_7F3B_4324_8D3F_1FA13F9ED0BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#define MAX_MISSIONMSG_NUM  5
class cTextArea;
class CMPMissionDialog : public cDialog  
{
	cTextArea* m_pMission;
	cTextArea* m_pCaution;
	char* m_pMissionMsg[MAX_MISSIONMSG_NUM];
	char* m_pCautionMsg[MAX_MISSIONMSG_NUM];

	DWORD m_dwStartTime;

public:
	CMPMissionDialog();
	virtual ~CMPMissionDialog();

	void Linking();
	void SetMissionInfo(int msgnum);
	virtual void SetActive(BOOL val);
	void LoadMissionMsg();

 	virtual DWORD ActionEvent(CMouse * mouseInfo);
};

#endif // !defined(AFX_MPMISSIONDIALOG_H__A644A86F_7F3B_4324_8D3F_1FA13F9ED0BB__INCLUDED_)
