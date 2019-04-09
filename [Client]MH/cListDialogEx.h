#ifndef __CLISTDIALOG_EX__
#define __CLISTDIALOG_EX__

#include "./interface/cListDialog.h"

class cListDialogEx : public cListDialog
{
public:
	cListDialogEx();
	~cListDialogEx();

	virtual void ListMouseCheck( LONG x, LONG y, DWORD we );
	virtual void Render();
};

#endif //__CLISTDIALOG_EX__