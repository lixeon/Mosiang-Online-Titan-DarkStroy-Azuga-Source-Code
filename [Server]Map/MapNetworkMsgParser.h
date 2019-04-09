
#ifndef __MAPNETWORKMSGPARSER_H__
#define __MAPNETWORKMSGPARSER_H__

void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MAPSERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MOVEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MUGONGMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_CHEATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_QUICKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_PARTYMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_UNGIJOSIKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_PEACEWARMODEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_TACTICMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_PYOGUKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_WANTEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SkillMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_BattleMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_REVIVEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_OPTIONMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MurimnetMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_JOURNALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SURYUNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

//KES EXCHANGE 030922
void MP_EXCHANGEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_HACKCHECKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SOCIETYACTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);


// LBS 03.09.25 ≥Î¡°ªÛ
void MP_STREETSTALLMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_NPCMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_QUESTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_PKMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_HACKCHECKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILDFIELDWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_PARTYWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GTOURNAMENTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_SIEGEWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_GUILUNIONMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_SIEGEWARPROFITMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

// Weather System
void MP_WEATHERMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

void MP_PETMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_SURVIVALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

void MP_BOBUSANGMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

BOOL CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx );

void MP_TITANMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void MP_ITEMLIMITMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_AUTONOTEMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void MP_FORTWARMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

#endif //__MAPNETWORKMSGPARSER_H__
