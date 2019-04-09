#pragma once

#include "../4DyuchiGRX_common/stdafx.h"

#define	tiffTag_ImageWidth				   0x0100
#define	tiffTag_ImageLength                0x0101
#define	tiffTag_BitsPerSample              0x0102
#define	tiffTag_Compression                0x0103
#define	tiffTag_PhotometricInterpretation  0x0106
#define	tiffTag_StripOffsets               0x0111
#define	tiffTag_SamplesPerPixels           0x0115
#define	tiffTag_RowsPerStrip               0x0116
#define	tiffTag_StripByteCounts            0x0117
#define	tiffTag_ColorMapOffset             0x0140

#pragma pack(push,1)

typedef struct
{
	unsigned short B;
	unsigned short G;
	unsigned short R;
} RGBc;

typedef struct
{
	unsigned char B;
	unsigned char G;
	unsigned char R;
} RGB8;            

struct STRIPINFO
{
    unsigned int StripOffsets;
	unsigned int StripByteCounts;
};
struct IFDHEADER
{
	unsigned short Tag;
	unsigned short VarType;
	unsigned int Count;
	unsigned int Value;
};


struct TIFHEADER
{
    unsigned short Endian;
	unsigned short Confirm;
	unsigned int FirstIFD;
};

#pragma pack(pop)

class CTiff
{
	unsigned short		IFDCount;
	TIFHEADER			TIFHeader;
	IFDHEADER*			pIFDIndex;
	IFDHEADER			IFDAddr;			/* index에서 찾은 IFD를 저장하는곳 */
	RGB8*				pImage8;
	RGBc				PAL[256];		/* For 8Bit */
	unsigned char*				pImage;

	unsigned int		Width;
	unsigned int		Height;
	unsigned int		RowsPerStrip;
	unsigned int		BytesPerStrip;
	BOOL		GetIFDWithIndex(int Index);
	BOOL		bHasAlpha;

public:
	BOOL		LoadTIFF(char *filename);
	unsigned char*		GetImage() {return pImage;}
	unsigned int		GetWidth() {return Width;}
	unsigned int		GetHeight() {return Height;}
	BOOL		GetHasAlpha() {return bHasAlpha;}

	CTiff();
	~CTiff();

};
