// NoteManager.h: interface for the CNoteManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTEMANAGER_H__72F3A198_8C23_4651_8C14_A089477ABE67__INCLUDED_)
#define AFX_NOTEMANAGER_H__72F3A198_8C23_4651_8C14_A089477ABE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NOTEMGR USINGTON(CNoteManager)
class CNoteManager  
{
public:
	CNoteManager();
	virtual ~CNoteManager();
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	void WriteNote(char* Note, char* ToName);
	void WriteNoteID(char* Note, DWORD TargetID);
	void DelNoteSyn(DWORD NoteID, BOOL bLast);
	void DelAllNoteSyn();
	void NoteListSyn(WORD SelectedPage, WORD Mode=eMode_NormalNote);
	void ReadNote(DWORD NoteID, BOOL IsFront); //0 : front, 1: cur,  2:back
};
EXTERNGLOBALTON(CNoteManager)

#endif // !defined(AFX_NOTEMANAGER_H__72F3A198_8C23_4651_8C14_A089477ABE67__INCLUDED_)
