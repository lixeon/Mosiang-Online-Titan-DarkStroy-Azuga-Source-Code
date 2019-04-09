#include "net_global.h"

ICode* g_pICode = NULL;
void ErrorMsg(char* pMsg)
{
	MessageBox(NULL,pMsg,"Error",MB_OK);
}
