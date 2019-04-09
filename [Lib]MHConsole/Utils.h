
#ifndef __UTILS_H__
#define __UTILS_H__

void StringFromCLSIDCustom(const CLSID& clsid, char * szCLSID, int length);
BOOL SetRegistryKeyCustom(const char * szKey, const char * szSubKey, const char * szValue);
BOOL DeleteRegistryKeyCustom(HKEY hKey, const char * pKeyName);

#endif // __UTILS_H__