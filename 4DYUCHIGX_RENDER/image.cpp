#include "../4DyuchiGXGFunc/global.h"
#include "image.h"
#include "tif.h"
#include "global_variable.h"
#include "../4DyuchiGRX_Common/IFileStorage.h"
#include <crtdbg.h>

CImage::CImage()
{
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_pRawImage = 0;
	m_dwColorDepth = 0;

}

BOOL CImage::LoadTga(char* szFileName,DWORD dwBPS)
{
	BOOL		bResult = FALSE;
//	FILE *fp = fopen(szFileName,"rb");
	void*	pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
		goto lb_return;

	bResult = LoadTga(pFP,dwBPS);

	g_pFileStorage->FSCloseFile(pFP);
	bResult = TRUE;
	
lb_return:
	return bResult;
}
BOOL CImage::LoadTga(void* pFP,DWORD dwBPS)
{
	BOOL	bResult = FALSE;


	Destroy();
	
	TGA_HEADER	header;
	memset(&header,0,sizeof(TGA_HEADER));
	

	g_pFileStorage->FSRead(pFP,&header,sizeof(header));
//	fread(&header,sizeof(header),1,fp);	
/*
	// 이미지 타잎이 이 중 하나가 아니라면 이 파일은 로드할 수 없다.
	if (ImageType!=0x01 &&
		ImageType!=0x02 &&
		ImageType!=0x03 &&
		ImageType!=0x09 &&
		ImageType!=0x0a &&
		ImageType!=0x0b)
	{
		fclose(fp);
		return FALSE;
	}*/

	unsigned char* temp = new unsigned char[header.width*header.height*3];
	memset(temp,0,header.width * header.height*3);
//	fread(temp,sizeof(BYTE),3* header.width * header.height,fp);
	g_pFileStorage->FSRead(pFP,temp,3* header.width * header.height);

	
	m_dwColorDepth = dwBPS;

	DWORD y,x,count =0;
	DWORD line = header.height;

	m_pRawImage = new char[header.width * header.height*2]; // 이미지를 위한 메모리할당

	if (header.Descriptor)
	{
		for (y=0; y<header.height; y++)
		{
			line--;
			for (x=0; x<header.width; x++)
			{
				*(WORD*)(m_pRawImage+line*header.width*2+x*2) = 
					temp[y*header.width*3+x*3+0]>>3 | 
					temp[y*header.width*3+x*3+1]>>2<<5 | 
					temp[y*header.width*3+x*3+2]>>3<<11;
			}
		}
	}
	else 
	{
		for (y=0; y<header.height; y++)
		{
			for (x=0; x<header.width; x++)
			{
				*(WORD*)(m_pRawImage+y*header.width*2+x*2) = 
					temp[y*header.width*3+x*3+0]>>3 | 
					temp[y*header.width*3+x*3+1]>>2<<5 | 
					temp[y*header.width*3+x*3+2]>>3<<11;
			
			}
		}
	}
	delete [] temp;
	temp = NULL;

	m_dwWidth = header.width;
	m_dwHeight = header.height;

	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CImage::LoadTIF(char* filename,DWORD dwBPS)
{
	BOOL	bResult = FALSE;
	BYTE*	temp;
	DWORD	line;
	DWORD	y,x,count =0;
	

	Destroy();

	CTiff	tiff;
	if (!tiff.LoadTIFF(filename))
		goto lb_return;

	m_dwColorDepth = dwBPS;
	m_dwWidth = tiff.GetWidth();
	m_dwHeight = tiff.GetHeight();

	temp = tiff.GetImage();
	line = tiff.GetHeight();

	m_pRawImage = new char[m_dwWidth*m_dwHeight*dwBPS]; // 이미지를 위한 메모리할당

	if (dwBPS == 2)
	{	
		for (y=0; y<m_dwHeight; y++)
		{
			line--;
			for (x=0; x<m_dwWidth; x++)
			{
				*(WORD*)(m_pRawImage+line*m_dwWidth*2+x*2) = 
					temp[y*m_dwWidth*4+x*4+0]>>4 | 
					temp[y*m_dwWidth*4+x*4+1]>>4<<4 | 
					temp[y*m_dwWidth*4+x*4+2]>>4<<8 |
					temp[y*m_dwWidth*4+x*4+3]>>4<<12;

			}
		} 
	}
	else if (dwBPS == 4)
	{
		for (y=0; y<m_dwHeight; y++)
		{
			line--;
			memcpy(m_pRawImage+line*m_dwWidth*4,temp+y*m_dwWidth*4,4*m_dwWidth);
//			memcpy(m_pRawImage+y*m_dwWidth*4,temp+y*m_dwWidth*4,4*m_dwWidth);
		}
	}

	
	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CImage::Resize(DWORD dwWidth,DWORD dwHeight,DWORD dwBPS)
{
//	WORD				m_wWidth;
//	WORD				m_wHeight;
//	BYTE*				m_pRawImage;
//	BYTE				m_bColorDepth;
	char* pTemp;
	
	if ((dwWidth == m_dwWidth) && (dwHeight == m_dwHeight) )
		goto lb_return;

	pTemp = new char[dwWidth*dwHeight*m_dwColorDepth];
	ResizeImage(pTemp,dwWidth,dwHeight,m_pRawImage,m_dwWidth,m_dwHeight,m_dwColorDepth);

	delete [] m_pRawImage;
	m_pRawImage = pTemp;
	m_dwHeight = dwHeight;
	m_dwWidth = dwWidth;

lb_return:
	return TRUE;

}


void CImage::Destroy()
{
	if (m_pRawImage)
	{
		delete [] m_pRawImage;
		m_pRawImage = NULL;
	}
}
CImage::~CImage()
{
	Destroy();
}
