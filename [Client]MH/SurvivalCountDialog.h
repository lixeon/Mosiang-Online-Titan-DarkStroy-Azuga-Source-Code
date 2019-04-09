#pragma once
#include "interface\cdialog.h"

#define NUMBER_MAX 10
enum{FrontNum,BackNum,MaxNum = 2};

class CSurvivalCountDialog :	public cDialog
{
	//cImage	m_NumImage[NUMBER_MAX];		//0, 1, 2, ...9
	//cStatic* m_pCounterNum[MaxNum];
	cStatic* m_pCounterNum;
	cStatic* m_pWinnerName;
public:
	CSurvivalCountDialog(void);
	virtual ~CSurvivalCountDialog(void);

	void	Linking();
	void	InitSurvivalCountDlg(MAPTYPE MapNum);
	void	LoadNumImage();
	void	SetCounterNumber(DWORD num);
	void	SetWinnerName(char* pName);
};
