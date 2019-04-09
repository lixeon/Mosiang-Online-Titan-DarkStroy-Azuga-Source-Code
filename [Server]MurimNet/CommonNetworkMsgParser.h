
#ifndef __COMMONNETWORKMSGPARSER_H__
#define __COMMONNETWORKMSGPARSER_H__




//---------------------------------------------------------------------------------------------
//don't changed -------------------------------------------------------------------------------
typedef void (*MSGPARSER)(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
extern MSGPARSER g_pServerMsgParser[];
extern MSGPARSER g_pUserMsgParser[];
//---------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------
// 추가되는 것 
void ErrorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//void MP_SERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
//void MP_PACKEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);














#endif //__NETWORKMSGPARSER_H__
