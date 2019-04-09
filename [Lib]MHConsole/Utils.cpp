#include <objbase.h>
#include "Utils.h"

void StringFromCLSIDCustom(const CLSID& clsid, char * szCLSID, int length)
{
	LPOLESTR wszCLSID = NULL;

	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
	wcstombs(szCLSID, wszCLSID, length);

	CoTaskMemFree(wszCLSID);
}

BOOL SetRegistryKeyCustom(const char * szKey, const char * szSubKey, const char * szValue)
{
	HKEY hKey;
	char szkeyPath[1024];

	strcpy(szkeyPath, szKey);
	if(szSubKey != NULL)
	{
		strcat(szkeyPath, "\\");
		strcat(szkeyPath, szSubKey);
	}

	long lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT, szkeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if(lResult != ERROR_SUCCESS) return FALSE;

	lResult = RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE *)szValue, strlen(szValue)+1);
	if(lResult != ERROR_SUCCESS) return FALSE;
	
	lResult = RegCloseKey(hKey);
	if(lResult != ERROR_SUCCESS) return FALSE;

	return TRUE;
}

BOOL DeleteRegistryKeyCustom(HKEY hKey, const char * pKeyName)
{
	HKEY hRKey;
	DWORD dwSubkeyLength;
	DWORD dwRtn;
	TCHAR szSubKey[256];

	if(pKeyName && lstrlen(pKeyName))
	{
		if((dwRtn = RegOpenKeyEx(hKey, pKeyName, 0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hRKey)) == ERROR_SUCCESS)
		{
			while(dwRtn == ERROR_SUCCESS)
			{
				dwSubkeyLength = 256;
				dwRtn = RegEnumKeyEx(hRKey, 0, szSubKey, &dwSubkeyLength, NULL, NULL, NULL, NULL);
				if(dwRtn == ERROR_NO_MORE_ITEMS)
				{
					dwRtn = RegDeleteKey(hKey, pKeyName);
					break;
				}
				else if(dwRtn == ERROR_SUCCESS)
				{
					dwRtn = DeleteRegistryKeyCustom(hRKey, szSubKey);
				}
			}
			RegCloseKey(hRKey);
		}
	}
	else
		dwRtn = ERROR_BADKEY;
	
	return dwRtn;
}