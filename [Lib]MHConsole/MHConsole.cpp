
#include <objbase.h>
#include "Utils.h"
#include "TConsole_Guids.h"
#include "TConsoleFactory.h"

HMODULE g_hModule = NULL;
const int CLSID_STRING_SIZE = 39;

const char * szFriendlyName = "MHTConsole Components by taiyo";
const char * szProgID = "MHTConsole.Component";
const char * szVerIndProgID = "MHTConsole.Component.1";


BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
	break;
	}

	return TRUE;
}


STDAPI DllRegisterServer()
{
	char szModule[512];
	char szCLSID[CLSID_STRING_SIZE];
	char szKey[64];

	DWORD dwResult = ::GetModuleFileName(g_hModule, szModule, sizeof(szModule)/sizeof(char));
	if(!dwResult) return E_FAIL;

	StringFromCLSIDCustom(CLSID_ULTRA_TCONSOLE, szCLSID, sizeof(szCLSID));
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	SetRegistryKeyCustom(szKey, NULL, szFriendlyName);
	SetRegistryKeyCustom(szKey, "InprocServer32", szModule);
	SetRegistryKeyCustom(szKey, "ProgID", szProgID);
	SetRegistryKeyCustom(szKey, "VersionIndependentProgID", szVerIndProgID);


	SetRegistryKeyCustom(szVerIndProgID, NULL, szFriendlyName);
	SetRegistryKeyCustom(szVerIndProgID, "CLSID", szCLSID);
	SetRegistryKeyCustom(szVerIndProgID, "CurVer", szProgID);

	SetRegistryKeyCustom(szProgID, NULL, szFriendlyName);
	SetRegistryKeyCustom(szProgID, "CLSID", szCLSID);
	
	return S_OK;
}

STDAPI DllUnregisterServer()
{
	char szCLSID[CLSID_STRING_SIZE];
	char szKey[64];

	StringFromCLSIDCustom(CLSID_ULTRA_TCONSOLE, szCLSID, sizeof(szCLSID));
	strcpy(szKey, "CLSID\\");
	strcpy(szKey, szCLSID);
	
	long lResult = DeleteRegistryKeyCustom(HKEY_CLASSES_ROOT, szKey);
	lResult = DeleteRegistryKeyCustom(HKEY_CLASSES_ROOT, szVerIndProgID);
	lResult = DeleteRegistryKeyCustom(HKEY_CLASSES_ROOT, szProgID);

	return S_OK;
}


STDAPI DllGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID* ppv )
{
	HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
   
	if(CLSID_ULTRA_TCONSOLE != rclsid) return hr;
	
	IUnknown* pIFactory = new CTConsoleFactory;
	if (!pIFactory) return E_OUTOFMEMORY;

	hr = pIFactory->QueryInterface(riid, ppv);
	pIFactory->Release();

	return hr;
}

STDAPI DllCanUnloadNow()
{
//	if( (g_cComponents == 0) )
	{
		return S_OK;
	}
//	else
	{
		return S_FALSE;
	}

}
