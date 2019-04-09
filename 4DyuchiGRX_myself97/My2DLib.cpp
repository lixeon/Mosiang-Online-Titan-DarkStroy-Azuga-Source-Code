#include "My2DLib.h"
//#include "../4dyuchigrx_common/typedef.h"

//	스케일 다운.
void ScaleDownTexture565( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale)
{
	DWORD	dwImageWidth	=	dwWidth/dwScale;
	DWORD	dwImageHeight	=	dwHeight/dwScale;
//	DWORD	dwImageBPP		=	16;		// 지금은 무조껀 16비트.
	
	// 작아진 이미지의 한 도트는 원본 이미지의 dwScale*dwScale만큼의 도트를 평균 낸것이다.

	DWORD	x = 0, y = 0;
	for( y = 0; y < dwImageHeight; y++)	// 다운 스케일된 이미지를 기준으로 루프를 돈다.
	{
		for( x = 0; x < dwImageWidth; x++)
		{
			// dwScale이 2일 경우 결과좌표 x,y의 점은 원본좌표 x,y; x+1,y; x,y+1; x+1,y+1의 각 요소의 평균값이다.
			// 원본 텍스쳐에서 dwScale*dwScale만큼 샘플링.
			DWORD	ComR	=	0;
			DWORD	ComG	=	0;
			DWORD	ComB	=	0;
			DWORD	u = 0, v = 0;
			for(v = 0; v < dwScale; v++)
			{
				for( u = 0; u < dwScale; u++)
				{
//					WORD	Component = pTex[(x*dwScale) + u + ((y+v)*dwScale*(dwWidth))];
//					WORD	Component = pTex[(x*dwScale) + u + ((y*dwScale+v)*(dwWidth))];
					WORD	Component = pTex[   (y*dwScale+v)*dwWidth    + (x*dwScale)+u];
					ComR = ComR + ((Component&0xf800) >> 11);		// 비트 배열이 바뀌면 여길 수정.
					ComG = ComG + ((Component&0x07e0) >> 5);
					ComB = ComB + ((Component&0x001f) );
				}
			}
			ComR = (ComR /(dwScale*dwScale)) << 11;		// dwScale*dwScale로 나누고 11비트 왼쪽으로.
			ComG = (ComG /(dwScale*dwScale)) << 5;		// dwScale*dwScale로 나누고 5비트 왼쪽으로.
			ComB = ComB /(dwScale*dwScale);				// dwScale*dwScale로 나누기만..
			pDest[x+y*dwImageWidth] = unsigned short (ComR + ComG + ComB);
		}
	}

}

void ScaleDownTexture4444( WORD* pTex, WORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale)
{
	DWORD	dwImageWidth	=	dwWidth/dwScale;
	DWORD	dwImageHeight	=	dwHeight/dwScale;
//	DWORD	dwImageBPP		=	16;		// 지금은 무조껀 16비트.
	
	// 작아진 이미지의 한 도트는 원본 이미지의 dwScale*dwScale만큼의 도트를 평균 낸것이다.

	DWORD	x = 0, y = 0;
	for( y = 0; y < dwImageHeight; y++)	// 다운 스케일된 이미지를 기준으로 루프를 돈다.
	{
		for( x = 0; x < dwImageWidth; x++)
		{
			// dwScale이 2일 경우 결과좌표 x,y의 점은 원본좌표 x,y; x+1,y; x,y+1; x+1,y+1의 각 요소의 평균값이다.
			// 원본 텍스쳐에서 dwScale*dwScale만큼 샘플링.
			DWORD	ComR	=	0;
			DWORD	ComG	=	0;
			DWORD	ComB	=	0;
			DWORD	ComA	=	0;
			DWORD	u = 0, v = 0;
			for(v = 0; v < dwScale; v++)
			{
				for( u = 0; u < dwScale; u++)
				{
					WORD	Component = pTex[   (y*dwScale+v)*dwWidth    + (x*dwScale)+u];
					ComR	=	ComR	+	((Component&0xf000) >> 12);
					ComG	=	ComG	+	((Component&0x0f00) >> 8);
					ComB	=	ComB	+	((Component&0x00f0) >> 4);
					ComA	=	ComA	+	((Component&0x000f));
				}
			}
			ComR	=	(ComR / (dwScale*dwScale)) << 12;
			ComG	=	(ComG / (dwScale*dwScale)) << 8;
			ComB	=	(ComB / (dwScale*dwScale)) << 4;
			ComA	=	(ComA / (dwScale*dwScale));
			pDest[x+y*dwImageWidth]	=	unsigned short (ComR + ComG + ComB + ComA);
		}
	}

}

void ScaleDownTexture8888(DWORD* pTex, DWORD* pDest, DWORD dwWidth, DWORD dwHeight, DWORD dwScale)
{
	DWORD	dwImageWidth	=	dwWidth/dwScale;
	DWORD	dwImageHeight	=	dwHeight/dwScale;
//	DWORD	dwImageBPP		=	16;		// 지금은 무조껀 16비트.
	
	// 작아진 이미지의 한 도트는 원본 이미지의 dwScale*dwScale만큼의 도트를 평균 낸것이다.

	DWORD	x = 0, y = 0;
	for( y = 0; y < dwImageHeight; y++)	// 다운 스케일된 이미지를 기준으로 루프를 돈다.
	{
		for( x = 0; x < dwImageWidth; x++)
		{
			// dwScale이 2일 경우 결과좌표 x,y의 점은 원본좌표 x,y; x+1,y; x,y+1; x+1,y+1의 각 요소의 평균값이다.
			// 원본 텍스쳐에서 dwScale*dwScale만큼 샘플링.
			DWORD	ComR	=	0;
			DWORD	ComG	=	0;
			DWORD	ComB	=	0;
			DWORD	ComA	=	0;
			DWORD	u = 0, v = 0;
			for(v = 0; v < dwScale; v++)
			{
				for( u = 0; u < dwScale; u++)
				{
					DWORD	Component = pTex[   (y*dwScale+v)*dwWidth    + (x*dwScale)+u];
					ComR	=	ComR	+	((Component & 0xff000000)>>24);
					ComG	=	ComG	+	((Component & 0x00ff0000)>>16);
					ComB	=	ComB	+	((Component & 0x0000ff00)>>8);
					ComA	=	ComA	+	(Component & 0x000000ff);
				}
			}
			ComR	=	(ComR / (dwScale*dwScale)) << 24;
			ComG	=	(ComG / (dwScale*dwScale)) << 16;
			ComB	=	(ComB / (dwScale*dwScale)) << 8;
			ComA	=	(ComA / (dwScale*dwScale));
			pDest[x+y*dwImageWidth]	=	(ComR | ComG | ComB | ComA);
		}
	}

}

