// ErrorMsg.cpp: implementation of the CErrorMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorMsg.h"
#include "Object.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CErrorMsg)
CErrorMsg::CErrorMsg()
{

}

CErrorMsg::~CErrorMsg()
{

}

void CErrorMsg::PrintError(ERRORCODE ecode,PRINTTYPE PrintType)
{
#define ERRSTART switch(ecode)	{
#define ERRCODE(code,str)	case code:	sprintf(tmp,str);	break
#define ERREND	}

	char tmp[256];
	
	ERRSTART
	ERRCODE(EC_GENERAL_ERROR, "General Error");
	ERRCODE(EC_CONNECT_DISTRIBUTESERVER_FAILED, "TIMEOUT : Connecting To distribute is Failed");
	ERRCODE(EC_CONNECT_AGENTSERVER_FAILED,"Connecting To Agent is Failed");
	ERRCODE(EC_UNKNOWN_PROTOCOL,"Unknow(Unnecessary) Protocol is Received");
	ERRCODE(EC_MSGSEND_FAILED,"Sending Msg is Failed");
	ERRCODE(EC_CHARACTERLIST_FAILED,"Receiving CHARACTERLIST is FAILED");
	ERRCODE(EC_IDPWAUTH_FAILED,"Checking ID/PW is FAILED");
	ERRCODE(EC_CHARACTERSELECT_FAILED,"Selected Character is Invalid");
//	ERRCODE(EC_CHARNAME_EXIST, "character name exist");
//	ERRCODE(EC_CHARNAME_SUCCESS, "character name doesn't exist");
//	ERRCODE(EC_DONOTNAMEDUPLCHECK, "you doesn't check duplicate character name");
	ERRCODE(EC_IMAGELOAD_FAILED, "Cann't load Image file from deginated position");
//	ERRCODE(EC_CHARACTERMAKE_FAILED, "Character make is failed");
	ERRCODE(EC_MUGONG_ADDNITEMDELETE_FAILED, "Mugong add & item delete failed");
	ERRCODE(EC_VERSION_CHECK_FAILED, "Client Version is not updated");
	ERRCODE(EC_IPADDRESS_INVALID_FAILED, "Your IP Address is not allowed");
	ERRCODE(EC_MAPSERVER_CLOSED, "MapServer Closed. Try Again Later.");
	
	ERREND
	
	PrintErrorEx(tmp, PrintType);
}
void CErrorMsg::PrintErrorEx(char * buffer, PRINTTYPE PrintType)
{
	switch(PrintType)
	{
	case PT_MESSAGEBOX:
		MessageBox(NULL,buffer,NULL,NULL);
		break;
	case PT_FILE:
		OutputErrorFile("./Log/debug.txt", buffer, APPEND);
		break;
	case PT_INTERFACE:
		{
			
		}
		break;
	}	
}
void CErrorMsg::PrintErrorEx(int value, PRINTTYPE PrintType)
{
	char tmp[256];
	wsprintf(tmp, "%d", value);
	PrintErrorEx(tmp, PrintType);
}
void CErrorMsg::PrintErrorFile(char * buffer,...)
{
	char msg2[255];
	char msg3[255];
	va_list argList;

	va_start(argList, buffer);
	vsprintf(msg2,buffer,argList);
	va_end(argList);
	
	sprintf(msg3,"%s   [%s]",msg2,GetCurTimeToString());

	PrintErrorEx(msg2,PT_FILE);
}
void CErrorMsg::PrintErrorMessage(char * buffer,...)
{
	char msg2[255];
	va_list argList;

	va_start(argList, buffer);
	vsprintf(msg2,buffer,argList);
	va_end(argList);

	PrintErrorEx(msg2,PT_MESSAGEBOX);
}
char* CErrorMsg::TransObjToString(CObject* pObject)
{
	static char buf[256];
	VECTOR3 pos;
	pObject->GetPosition(&pos);
	sprintf(buf,"%u\t%s\t%4.1f\t%4.1f\t%d",
		pObject->GetID(),
		pObject->GetObjectName(),
		pos.x,
		pos.z,
		pObject->GetObjectKind()
		);
	return buf;
}

int CErrorMsg::OutputErrorFile(char *fn, char * buffer, int type)
{
	FILE	*fp = NULL;
	if(type == TRUNC)
	{
		fp = fopen(fn, "wt");
		if( fp )
		{
            fwrite(buffer, strlen(buffer), 1, fp);
            fclose(fp);
		}
	}
	else if(type == APPEND)
	{
		fp = fopen(fn,"a+t");
		if( fp )
		{
			fprintf(fp,"\n%s", buffer);
            fclose(fp);
		}
	}

	return 0;
}