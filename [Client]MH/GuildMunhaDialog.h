#pragma once
#include "INTERFACE\cDialog.h"


class cListDialog;
class cPtrList;

class CGuildMunhaDialog : public cDialog
{
	cListDialog*	m_pListDlg[2];		// [0]:지원자	[1]:총문하생
	cPtrList		m_MunHaList;		// 지원자
	cButton*		m_pNumHaBtn[3];
	int				m_MunHaIndex;		// 선택한 문하생 인덱스
	BOOL			m_ResetFlag;		// 동일맵에서는 데이터를 한번만 Recv (맵이동하면 다시 Recv)

public:
	CGuildMunhaDialog(void);
	~CGuildMunhaDialog(void);

	virtual void SetActive(BOOL val);
	virtual DWORD ActionEvent(CMouse* mouseInfo);
	void OnActionEvent(LONG lId, void* p, DWORD we);
	void Linking();
	void SetMunHaInfo(char name[]);
	void DeleteMunHaInfo(char name[]);
	void RecvMunHaJoinInfo(MSG_MUNHACATALOG* pTemp);
	void RecvMunHaJoinInfo(MSG_NAME_DWORD2* pTemp);
	void RecvUpdateMunHaJoin(MSG_DWORD2* pTemp);
};
