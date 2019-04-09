

#ifndef	_MouseLib_h_
#define	_MouseLib_h_

#include "../4DyuchiGRX_common/IGeometry.h"

// ½ºÅ©¸°»óÀÇ x,yÁÂÇ¥¸¦ ½ÇÁ¦ ÁÂÇ¥ÀÇ x,0,z·Î ¹Ù²Û´Ù. y´Â ¾ðÁ¦³ª 0
//	ÁÖÀÇÇÒÁ¡Àº, Ä«¸Þ¶ó fromÀÇ yÁÂÇ¥°¡ 0 ÀÌ»óÀÏ¶§ ¼öÆò¼±º¸´Ù À§¸¦ º¸°í ÂïÀ¸¸é, ½ÃÁ¡º¸´Ù µÚÂÊ¿¡¼­ ¸¸³ª´Â ÁÂÇ¥°¡ ³ª¿Â´Ù.
VECTOR3	GetXYZFromScreenXY( I4DyuchiGXGeometry* pGeometry, DWORD dwX, DWORD dwY, DWORD dwScreenX, DWORD dwScreenY);
VECTOR3	GetXYZFromScreenXY2( I4DyuchiGXGeometry* pGeometry, DWORD dwX, DWORD dwY, DWORD dwScreenX, DWORD dwScreenY, float height);

// °ø°£»óÀÇ VECTOR3°¡ È­¸é»óÀÇ ¾îµðÀÎÁö ¾Ë¾Æ³»´Â ÇÔ¼ö.
// ´Ü¼øÈ÷ pWhereÀ» Æ®·£½ºÆûÀ» °ÅÄ£ ÈÄ °á°ú¸¦ pOutScreen¿¡ Ãâ·ÂÇÑ´Ù.
// ³ª¿À´Â pOutScreen.x¿Í .y´Â ¿ÞÂÊ, À§°¡ °¢°¢ 0.0f, ¿À¸¥ÂÊ ¾Æ·¡°¡ °¢°¢ 1.0f ÀÌ¹Ç·Î, È­¸é ÇØ»óµµ¸¦ °öÇØ¾ß È­¸é»óÀÇ ÁÂÇ¥¸¦ ±¸ÇÒ ¼ö
// ÀÖ´Ù. ¹üÀ§°¡ 0.0f¿Í 1.0f»çÀÌ¸¦ ¹þ¾î³ª´Â °æ¿ì È­¸é¿¡ ¾È³ª¿Â´Ù°í º¸¸é µÈ´Ù.
void	GetScreenXYFromXYZ( I4DyuchiGXGeometry* pGeometry, DWORD dwViewportIndex, VECTOR3* pInWhere, VECTOR3* pOutScreen);

#endif