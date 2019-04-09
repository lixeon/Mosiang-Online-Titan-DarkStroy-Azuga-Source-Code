// ErrorMsg.h: interface for the CErrorMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORMSG_H__3D9898D8_59A3_4AAF_81E1_054B04A547AE__INCLUDED_)
#define AFX_ERRORMSG_H__3D9898D8_59A3_4AAF_81E1_054B04A547AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ERRORMSG	USINGTON(CErrorMsg)
#define LOG(a)		ERRORMSG->PrintError(a);
#define LOGEX(a, flag)	ERRORMSG->PrintErrorEx(a, flag);
#define LOGFILE			ERRORMSG->PrintErrorFile
#define OBJECTLOG(obj)		ERRORMSG->TransObjToString(obj)
#define LOGMSG			ERRORMSG->PrintErrorMessage

enum ERRORCODE
{
	EC_GENERAL_ERROR,
	EC_CONNECT_DISTRIBUTESERVER_FAILED,
	EC_CONNECT_AGENTSERVER_FAILED,
	EC_IMAGELOAD_FAILED,
	EC_MSGSEND_FAILED,
	EC_IDPWAUTH_FAILED,
	EC_MAPSERVER_CLOSED,
	EC_CHARACTERLIST_FAILED,
	EC_CHARACTERSELECT_FAILED,
	EC_CHARNAME_EXIST,
	EC_CHARNAME_SUCCESS,
	EC_DONOTNAMEDUPLCHECK,
	EC_CHARACTERMAKE_FAILED,
	EC_MUGONG_ADDNITEMDELETE_FAILED,
	EC_VERSION_CHECK_FAILED,
	EC_IPADDRESS_INVALID_FAILED,
	EC_UNKNOWN_PROTOCOL,
};

enum PRINTTYPE
{
	PT_FILE,
	PT_MESSAGEBOX,
	PT_INTERFACE,
};

class CObject;
class CErrorMsg  
{
	enum { TRUNC, APPEND };
public:
	////MAKESINGLETON(CErrorMsg);

	CErrorMsg();
	virtual ~CErrorMsg();

	void PrintError(ERRORCODE ecode, PRINTTYPE PrintType = PT_MESSAGEBOX);
	void PrintErrorEx(char * buffer, PRINTTYPE PrintType = PT_MESSAGEBOX);
	void PrintErrorEx(int value, PRINTTYPE PrintType = PT_MESSAGEBOX);
	void PrintErrorFile(char * buffer,...);
	void PrintErrorMessage(char * buffer,...);
	char* TransObjToString(CObject* pObject);
private:
	int OutputErrorFile(char *fn, char * buffer, int type=APPEND);
};
EXTERNGLOBALTON(CErrorMsg)
#endif // !defined(AFX_ERRORMSG_H__3D9898D8_59A3_4AAF_81E1_054B04A547AE__INCLUDED_)
