#ifndef _HELPER_MANAGER_
#define _HELPER_MANAGER_

#include "PtrList.h"
#include "../MHFile.h"


#define HELPERMGR USINGTON(cHelperManager)

class cPageBase;
class cDialogueList;
class cHelper;
class cHelperSpeechDlg;

class cHelperManager 
{
protected:
	cPtrList				m_HelperSpeechList;		// µµ¿ì¹Ì Page ¸®½ºÆ®
	cDialogueList*			m_pDialogue;			// µµ¿ì¹Ì ´ë»ç ¸®½ºÆ®

	cHelper*				m_pHelper;				// µµ¿ì¹Ì ¾Ö´Ï ÀÌ¹ÌÁö
	cHelperSpeechDlg*		m_pSpeechDlg;

	BOOL					m_bInit;

public:
//	//MAKESINGLETON(cHelperManager);

	cHelperManager();
	virtual ~cHelperManager();

	void Init();
	void Release();
	void SetActive(BOOL bActive);

	void LoadSpeechList();
	void LoadPageInfo(cPageBase* pPage, CMHFile* fp);
	
	void LoadHelper();
	void LoadHelperInfo( CMHFile* fp );
	void LoadFrameInfo( CMHFile* fp );

	void AddPage(cPageBase* pPage);
	void DeleteAllPageInfo();

	cDialogueList* GetDialogueList() { return m_pDialogue; }

	cPageBase* GetPage( DWORD dwPageId );

	cHelper* GetHelper() { return m_pHelper; }
	cHelperSpeechDlg* GetHelperDlg() { return m_pSpeechDlg; }

public: // Helper¿Í ´ë»ç Á¦¾î ÇÔ¼öµé
	void	ShowRespect( DWORD dwPageId, BOOL bComp = TRUE );
	void	ChangeSpeechDlg( DWORD dwPageId, BOOL bComp = TRUE );
	
	BOOL	IsCompleteInit() { return m_bInit; }
};

EXTERNGLOBALTON(cHelperManager)

#endif // _HELPER_MANAGER_