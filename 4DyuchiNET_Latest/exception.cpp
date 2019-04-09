#include "exception.h"

BOOL DumpException(LPEXCEPTION_POINTERS lpExcep,char* szOutMsg)
{
	if (!lpExcep)
		return FALSE;

	BOOL		result = TRUE;
	SYSTEMTIME time;
	FILE*		fp = NULL;

	DWORD		dwExceptionAddress = (DWORD)lpExcep->ExceptionRecord->ExceptionAddress;
	DWORD		dwExceptionCode = lpExcep->ExceptionRecord->ExceptionCode;
	DWORD		dwExceptionFlags = lpExcep->ExceptionRecord->ExceptionFlags;
	DWORD		dwNumberParameters = lpExcep->ExceptionRecord->NumberParameters;
	DWORD		dwDr0 = lpExcep->ContextRecord->Dr0;
	DWORD		dwDr1 = lpExcep->ContextRecord->Dr1;
	DWORD		dwDr2 = lpExcep->ContextRecord->Dr2;
	DWORD		dwDr3 = lpExcep->ContextRecord->Dr3;
	DWORD		dwDr6 = lpExcep->ContextRecord->Dr6;
	DWORD		dwDr7 = lpExcep->ContextRecord->Dr7;
	DWORD		dwSegGs = lpExcep->ContextRecord->SegGs;
	DWORD		dwSegFs = lpExcep->ContextRecord->SegFs;
	DWORD		dwSegEs = lpExcep->ContextRecord->SegEs;
	DWORD		dwEFlags = lpExcep->ContextRecord->EFlags;

	DWORD		dwEsi = lpExcep->ContextRecord->Esi;
	DWORD		dwEdi = lpExcep->ContextRecord->Edi;
	DWORD		dwEbp = lpExcep->ContextRecord->Ebp;
	DWORD		dwEsp = lpExcep->ContextRecord->Esp;
	DWORD		dwEip = lpExcep->ContextRecord->Eip;

	DWORD		dwEax = lpExcep->ContextRecord->Eax;
	DWORD		dwEbx = lpExcep->ContextRecord->Ebx;
	DWORD		dwEcx = lpExcep->ContextRecord->Ecx;
	DWORD		dwEdx = lpExcep->ContextRecord->Edx;
	

	GetLocalTime(&time);
	fp = fopen("exception.dump","at");

	fprintf(fp,"%d year, %d month, %d day,%d hour, %d minute, %d second\n",
		time.wYear,time.wMonth,time.wDay,time.wMinute,time.wSecond);
	
	fprintf(fp,"%s\n",szOutMsg);
	fprintf(fp,"Crashed address %xh \n",dwExceptionAddress);
	fprintf(fp,"Exception Code %u \n",dwExceptionCode);
	fprintf(fp,"Exception Flag %xh \n",dwExceptionFlags);
	fprintf(fp,"NumberParameters %d \n",dwNumberParameters);
	fprintf(fp,"Dr0 %xh \n",dwDr0);
	fprintf(fp,"Dr1 %xh \n",dwDr1);
	fprintf(fp,"Dr2 %xh \n",dwDr2);
	fprintf(fp,"Dr3 %xh \n",dwDr3);
	fprintf(fp,"Dr6 %xh \n",dwDr6);
	fprintf(fp,"Dr7 %xh \n",dwDr7);
	
	fprintf(fp,"SegGs %xh \n",dwSegGs);
	fprintf(fp,"SegFs %xh \n",dwSegFs);
	fprintf(fp,"SegEs %xh \n",dwSegEs);
	fprintf(fp,"EFlags %xh \n",dwEFlags);
	
	fprintf(fp,"Esi %xh \n",dwEsi);
	fprintf(fp,"Edi %xh \n",dwEdi);
	fprintf(fp,"Ebp %xh \n",dwEbp);
	fprintf(fp,"Esp %xh \n",dwEsp);
	fprintf(fp,"Eip %xh \n",dwEip);
	

	fprintf(fp,"Eax %xh \n",dwEax);
	fprintf(fp,"Ebx %xh \n",dwEbx);
	fprintf(fp,"Ecx %xh \n",dwEcx);
	fprintf(fp,"Edx %xh \n",dwEdx);

	fprintf(fp,"\n\n");

	fclose(fp);
	fp = NULL;

	MessageBox(NULL,"Exception Occured.Reference exception.dump file","Exception",MB_OK);

	return result;

}