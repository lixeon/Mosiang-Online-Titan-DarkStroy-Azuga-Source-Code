
#ifndef	_GetDXVer_h_
#define	_GetDXVer_h_

// ÇöÀç´Â 8.1±îÁö Ã¼Å©°¡ µÈ´Ù.
// DX81SDK\samples\Multimedia\Misc\GetDXVer ¿¡¼­ ¼Ò½º Â÷¿ë.
// ÃßÈÄ 9.0À¸·Î ¹Ù²ð ¶§´Â ¿ª½Ã °°Àº µð·ºÅä¸®ÀÇ °°Àº ¼Ò½º¸¦ ±Ü¾î´Ù ¾²¸é µÉ µí.

// ³»ºÎÀûÀ¸·Î CoInitialize(), CoUninitialize()¸¦ È£ÃâÇÏ¹Ç·Î, comÀ» ¾²´Â °ÍµéÁß °¡Àå ¾Õ¿¡ °¡ ÀÖ¾î¾ß ÇÑ´Ù.
#include <windows.h>

extern DWORD GetDXVersion();

#endif