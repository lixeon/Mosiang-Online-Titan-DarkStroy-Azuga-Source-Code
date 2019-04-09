


#include <wininet.h>


static const int MAX_BUDDYAUTH_NUM = 10;
static const int COMMAND_DISTANCE = 4;
static const int LOGINRESULT_DISTANCE = 8;
static const int AUTHCODE_LENGTH = 256;
static const int IP_LENGTH = 16;




enum LoginResult
{
	eBuddyLoginRes_Success = 0,
	eBuddyLoginRes_Fail = 1,
	eBuddyLoginRes_PacketErr = 10,
	eBuddyLoginRes_AppNameErr = 11,
	eBuddyLoginRes_AccountIdErr = 12,
	eBuddyLoginRes_AccountPwdErr = 13,
	eBuddyLoginRes_OverlappedLogin = 20,
	eBuddyLoginRes_NotExistID = 21,
	eBuddyLoginRes_WrongPwd = 22,
	eBuddyLoginRes_NotRegistIP = 23,
};



struct stBUDDYAUTHINFO
{
	DWORD		dwConnectIdx;
	DWORD		dwAuthKey;
	char		szID[MAX_NAME_LENGTH+1];
	char		szIP[IP_LENGTH];
	char		szAuthCode[AUTHCODE_LENGTH];

	SOCKET		conn_socket;
};




class CBuddyAuth
{

	stBUDDYAUTHINFO			m_BuddyAuthInfoTable[MAX_BUDDYAUTH_NUM];
	int						m_ConnectTryNum;



private:
	BOOL	ResetBuddyAuthInfo( stBUDDYAUTHINFO* pAuthInfo, BOOL bOnlySocket=FALSE );

	stBUDDYAUTHINFO*	GetBlankAuthInfo();
	void	SendIDandAuthCode( stBUDDYAUTHINFO* pAuthInfo, char* szData );
	void	SendAuthResulttoUser( stBUDDYAUTHINFO* pAuthInfo, char* szData );

	void	AttendSendMsg( char* szMsg, void* pData, int nLength, int& nSize, int& nBodySize );
	void	ConvertIPtoUINT( char* szIP, unsigned int& nRV );



public:
	CBuddyAuth();
	~CBuddyAuth();


	BOOL	ConnectToBuddyAuthServerAll();
	void	RevMsgProcess();
	BOOL	SendAuthInfotoBuddy( DWORD dwConnectIdx, DWORD dwAuthKey, char* szID, char* szPassword, char* szIP, char* szAuthCode );

};



template<class _tDSIZE>
_tDSIZE GetResultCommand( char* szData, int nDistance, _tDSIZE tSize )
{
	_tDSIZE cmd = 0;
	memcpy( &cmd, &szData[nDistance], tSize );
	return cmd;
}

extern CBuddyAuth g_BuddyAuth;
