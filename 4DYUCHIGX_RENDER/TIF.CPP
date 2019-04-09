#include "tif.h"
#include "global_variable.h"
#include "../4DyuchiGRX_Common/IFileStorage.h"

CTiff::CTiff()
{
	IFDCount = 0;
	memset(&TIFHeader,0,sizeof(TIFHEADER));

	pIFDIndex = NULL;
	pImage8 = NULL;
	pImage = NULL;
	RowsPerStrip = 0;
	BytesPerStrip = 0;
	bHasAlpha = false;
}

BOOL CTiff::LoadTIFF(char *filename)
{
	
	void*	pFP = NULL;

	unsigned int i,j;
	unsigned char BitsPerSample;

	unsigned int MemSize;

	unsigned int StripByteCounts = 0;
	unsigned int StripOffsets    = 0;
    unsigned int StripInfoCount  = 0;

	unsigned char* pImageData;	            
	
	STRIPINFO *pStripInfo = NULL;		

	pFP = g_pFileStorage->FSOpenFile(filename,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
		return FALSE;

//	if (!(fp = fopen(filename,"rb")))
//		return false;

//	fread(&TIFHeader,sizeof(TIFHEADER),1,fp);
	g_pFileStorage->FSRead(pFP,&TIFHeader,sizeof(TIFHEADER));

	if(TIFHeader.Endian != 0x4949 && TIFHeader.Endian != 0x4D4D)
	{
		g_pFileStorage->FSCloseFile(pFP);
		return false;
	}

//	fseek(fp,TIFHeader.FirstIFD,SEEK_SET);
//	fread(&IFDCount,2,1,fp);

	g_pFileStorage->FSSeek(pFP,TIFHeader.FirstIFD,FSFILE_SEEK_SET);
	g_pFileStorage->FSRead(pFP,&IFDCount,2);
	
	pIFDIndex = new IFDHEADER[IFDCount]; 

	for(i=0;i<IFDCount;i++)
	{
//		fread(&pIFDIndex[i],sizeof(IFDHEADER),1,fp);
		g_pFileStorage->FSRead(pFP,&pIFDIndex[i],sizeof(IFDHEADER));


		if(pIFDIndex[i].Tag == tiffTag_StripByteCounts)
		{
			StripByteCounts = pIFDIndex[i].Count;
		}
		if(pIFDIndex[i].Tag == tiffTag_StripOffsets)
		{
			StripOffsets = pIFDIndex[i].Count;
		}
	}
	
/*
=====================================================================
	Offset Set
=====================================================================
*/
	
	if(StripByteCounts == StripOffsets)
	{
		StripInfoCount = StripByteCounts;
		
		pStripInfo = new STRIPINFO[StripInfoCount];
		for(i=0;i<IFDCount;i++)
		{
			// Strip Byte Counts
			if(pIFDIndex[i].Tag == tiffTag_StripByteCounts)
			{
				if(pIFDIndex[i].Count == 1)
				{
					pStripInfo[0].StripByteCounts = pIFDIndex[i].Value;
				} else 
		       	// Scan the file for the list of Strip Bytes Counts
				{
					//fseek(fp,pIFDIndex[i].Value,SEEK_SET);
					g_pFileStorage->FSSeek(pFP,pIFDIndex[i].Value,FSFILE_SEEK_SET);
					for(j=0;j<StripInfoCount;j++)
					{
				//		fread(&pStripInfo[j].StripByteCounts,4,1,fp);
						g_pFileStorage->FSRead(pFP,&pStripInfo[j].StripByteCounts,4);
						
					}
				}
			}
			// StripOffsets
			if (pIFDIndex[i].Tag == tiffTag_StripOffsets)
			{
				if(pIFDIndex[i].Count == 1)
				{
					pStripInfo[0].StripOffsets = pIFDIndex[i].Value;
				} else 
				{
	//				fseek(fp,pIFDIndex[i].Value,SEEK_SET);
					g_pFileStorage->FSSeek(pFP,pIFDIndex[i].Value,FSFILE_SEEK_SET);
					for(j=0;j<StripInfoCount;j++)
					{
		//				fread(&pStripInfo[j].StripOffsets,4,1,fp);
						g_pFileStorage->FSRead(pFP,&pStripInfo[j].StripOffsets,4);
					}
				}
			}
		}
	}
	
	MemSize = 0;
	for(i=0;i<StripInfoCount;i++)
	{
		MemSize += pStripInfo[i].StripByteCounts;
	}
	pImageData = new unsigned char[MemSize];
	unsigned char *DumpPoint = pImageData;			// Same Pointer

	for(i=0;i<StripInfoCount;i++)
	{
//	  fseek(fp,pStripInfo[i].StripOffsets,SEEK_SET);
//	  fread((unsigned char*)DumpPoint,pStripInfo[i].StripByteCounts,1,fp);
		g_pFileStorage->FSSeek(pFP,pStripInfo[i].StripOffsets,FSFILE_SEEK_SET);
		g_pFileStorage->FSRead(pFP,DumpPoint,pStripInfo[i].StripByteCounts);
	  DumpPoint += pStripInfo[i].StripByteCounts;	// Point Add
	}
/*
=====================================================================
	Image Size , Bit Set
=====================================================================
*/
	unsigned int ChannelNum = 0;	

	if (GetIFDWithIndex(338))
	{
		ChannelNum = 4;
		bHasAlpha = 4;
	} 
	else 
	{
		char txt[_MAX_PATH];
		wsprintf(txt,"TIF file is not 32Bit, %s",filename);
		MessageBox(NULL,txt,"ERROR",MB_OK);
		__asm int 3

		ChannelNum = 3;
		bHasAlpha = 3;
	}

	
	if(GetIFDWithIndex(tiffTag_ImageWidth)) 
		Width = IFDAddr.Value;
	else 
	{
		MessageBox(GetActiveWindow(),"Error reading tag Image Width","TIF Error",MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	
	if(GetIFDWithIndex(tiffTag_ImageLength)) 
		Height = IFDAddr.Value;
	else 
	{
		MessageBox(GetActiveWindow(),"Error reading tag Image Height","TIF Error",MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	if(GetIFDWithIndex(tiffTag_BitsPerSample)) 
		BitsPerSample = IFDAddr.Value;
	else 
	{
		MessageBox(GetActiveWindow(),"Error reading tag BitsPerSample","TIF Error",MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	if(GetIFDWithIndex(tiffTag_RowsPerStrip)) 
		RowsPerStrip = IFDAddr.Value;
	else 
	{
//		MessageBox(GetActiveWindow(),"Error reading tag Image Width","TIF Error",MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	if(GetIFDWithIndex(tiffTag_StripByteCounts)) 
		BytesPerStrip = IFDAddr.Value;
	else 
	{
//		MessageBox(GetActiveWindow(),"Error reading tag Image Width","TIF Error",MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	if(GetIFDWithIndex(tiffTag_PhotometricInterpretation))
	{
		if(pImage8 != NULL)
			delete pImage8;
		
		pImage8 = new RGB8[Width * Height];
		pImage = new BYTE[Width*Height*4];
		//memset(pImage,0,Width*Height*4);
		
		switch(IFDAddr.Value)
		{
			case 2:		// RGB color     
				short  BitsPerSampleRed;
				short  BitsPerSampleGreen;
				short  BitsPerSampleBlue;
				if(GetIFDWithIndex(tiffTag_BitsPerSample))
				{
					if(IFDAddr.Count == 1)
					{
						BitsPerSampleRed   = IFDAddr.Value;
						BitsPerSampleGreen = IFDAddr.Value;
						BitsPerSampleBlue  = IFDAddr.Value;
					} else {

				//		fseek(fp,IFDAddr.Value,SEEK_SET);
				//		fread(&BitsPerSampleRed,2,1,fp);
				//		fread(&BitsPerSampleGreen,2,1,fp);
				//		fread(&BitsPerSampleBlue,2,1,fp);
						g_pFileStorage->FSSeek(pFP,IFDAddr.Value,FSFILE_SEEK_SET);
						g_pFileStorage->FSRead(pFP,&BitsPerSampleRed,2);
						g_pFileStorage->FSRead(pFP,&BitsPerSampleGreen,2);
						g_pFileStorage->FSRead(pFP,&BitsPerSampleBlue,2);

					}

					if (BitsPerSampleRed != 8 && BitsPerSampleGreen != 8 && BitsPerSampleBlue != 8)
					{
						MessageBox(GetActiveWindow(),"Error: Bits Per Sample (RGB) not 8","TIF Error",MB_OK | MB_ICONSTOP);
						return false;
					}

					for(i=0;i<Width * Height;i++)
					{
						pImage[i*4+2] = pImageData[ChannelNum*i+0];
						pImage[i*4+1] = pImageData[ChannelNum*i+1];
						pImage[i*4+0] = pImageData[ChannelNum*i+2];
						if (ChannelNum==4)
							pImage[i*4+3] = pImageData[ChannelNum*i+3];

						pImage8[i].R = pImageData[ChannelNum*i];
						pImage8[i].G = pImageData[ChannelNum*i+1];
						pImage8[i].B = pImageData[ChannelNum*i+2];

					}
				} else {
					MessageBox(GetActiveWindow(),"Error reading tag Bits Per Sample","TIF Error",MB_OK | MB_ICONSTOP);
					return false;
				}
				break;
			default:
				MessageBox(GetActiveWindow(),"Not Supported Format","ERROR",MB_OK);
		}
	} else {
		MessageBox(GetActiveWindow(),"Error reading tag PhotometricInterpretation","TIF Error",MB_OK | MB_ICONSTOP);
		return false;
	}

	// Image Data Load (first image only!)
	g_pFileStorage->FSCloseFile(pFP);

//	Report();
	
	if (pImageData)
	{
		delete pImageData;
		pImageData = NULL;
	}
	if (pStripInfo)
	{
		delete pStripInfo;
		pStripInfo = NULL;
	}
	if (pIFDIndex)
	{
		delete pIFDIndex;
		pIFDIndex = NULL;
	}

	return true;
}
BOOL CTiff::GetIFDWithIndex(int Index)
{
	for(unsigned int i = 0; i < IFDCount; i++)
	{
		if(pIFDIndex[i].Tag == Index)
		{
			IFDAddr = pIFDIndex[i];
			return true;
		}
	}
	return false;
}

CTiff::~CTiff()
{
	if (pImage)
	{
		delete pImage;
		pImage = NULL;
	}
	if (pImage8)
	{
		delete pImage8;
		pImage8 = NULL;
	}
}