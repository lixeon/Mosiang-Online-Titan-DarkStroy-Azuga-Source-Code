#ifndef __DIALOGUE_LIST__
#define __DIALOGUE_LIST__

#include "PtrList.h"
#include "../MHFile.h"

#define MAX_DIALOGUE_COUNT 12800

#define STRESS_COLOR	RGB(255,255,  0)
#define NORMAL_COLOR	RGB(255,255,255)

class cDialogueList
{
protected:
	cPtrList	m_Dialogue[MAX_DIALOGUE_COUNT];
	

	DWORD		m_dwDefaultColor;
	DWORD		m_dwStressColor;

	void LoadDialogueList(DWORD dwId, CMHFile* fp);
public:
	cDialogueList();
	virtual ~cDialogueList();

	void LoadDialogueListFile(char* filePath, char* mode = "rt");

	DIALOGUE* GetDialogue( DWORD dwMsgId, WORD wLine );
	void ParsingLine( DWORD dwId, char* buf );
	void AddLine( DWORD dwId, char* str, DWORD color, WORD Line, WORD type);

};

#endif