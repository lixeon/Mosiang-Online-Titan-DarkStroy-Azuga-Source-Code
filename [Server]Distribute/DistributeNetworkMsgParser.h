
#ifndef __DISTRIBUTENETWORKMSGPARSER_H__
#define __DISTRIBUTENETWORKMSGPARSER_H__
void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_DISTRIBUTESERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_DISTRIBUTEAUTOPATCHMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

//void MP_MORNITORTOOLMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

#endif //__DISTRIBUTENETWORKMSGPARSER_H__