#ifndef _HELPDICTIONARYMANAGER_
#define _HELPDICTIONARYMANAGER_

#include "PtrList.h"
#include "../MHFile.h"


#define HELPDICMGR USINGTON(cHelpDicManager)

#ifdef _FILE_BIN_
//#define HELP_SCRIPT_PATH		"./Npc_Script/Help_Script.bin"
//#define HELP_DIALOGUE_PATH		"./Npc_Script/Help_Msg.bin"
//#define HELP_HYPERTEXT_PATH		"./Npc_Script/Help_HyperText.bin"
#define HELP_SCRIPT_PATH		"./Image/InterfaceScript/Help_Script.bin"
#define HELP_DIALOGUE_PATH		"./Image/Help_Msg.bin"
#define HELP_HYPERTEXT_PATH		"./Image/Help_HyperText.bin"
#else
#define HELP_SCRIPT_PATH		"./Npc_Script/Help_Script.txt"
#define HELP_DIALOGUE_PATH		"./Npc_Script/Help_Msg.txt"
#define HELP_HYPERTEXT_PATH		"./Npc_Script/Help_HyperText.txt"
#endif

class cPage;
class cDialogueList;
class cHyperTextList;

class cHelpDicManager 
{
	cPtrList			m_HelpDicList;

	cDialogueList*		m_pDialogue;
	cHyperTextList*		m_pHyperText;
	

	int nTest;


public:
//	//MAKESINGLETON(cHelpDicManager);

	cHelpDicManager();
	virtual ~cHelpDicManager();

	void Init();

	// Help µ¥ÀÌÅÍ ·Îµù
	virtual void LoadHelpDicInfo();
	virtual void LoadPageInfo(cPage* pPage, CMHFile* fp);

	virtual void AddPage( cPage* pPage );
	void DeleteAllPageInfo();


public:
	cPage* GetMainPage();					// ½ÃÀÛ ÆäÀÌÁö¸¦ ¾ò´Â´Ù.
	cPage* GetPage( DWORD dwPageId );		// ¸µÅ©µÈ ÆäÀÌÁö¸¦ ¾ò´Â´Ù.	

	cDialogueList* GetDialogueList() { return m_pDialogue; }
	cHyperTextList* GetHyperTextList() { return m_pHyperText; }
};
EXTERNGLOBALTON(cHelpDicManager)

#endif // _HELPDICTIONARYMANAGER_