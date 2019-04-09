
#ifndef _cWINDOWSYSTEMFUNC_H_
#define _cWINDOWSYSTEMFUNC_H_

class cIcon;
class cWindow;

void cbDragDropProcess(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY);
void SendDeleteIconMsg(cIcon * pIcon);

void CreateMainTitle_m();
void NewCreateCharSelect_m();
void CreateCharMakeDlg();
void CharMessageDlg_m();
cWindow * CreateGameLoading_m();
void CreateInitLogo_m();
void CreateMoneyDlg();

void CreateMonsterGuageDlg();
void CreateChannelDlg();

#endif // _cWINDOWSYSTEMFUNC_H_
