#if !defined(AFX_CHSEL_H__DD9E7B83_DBC5_4447_9523_33FA26FF2F27__INCLUDED_)
#define AFX_CHSEL_H__DD9E7B83_DBC5_4447_9523_33FA26FF2F27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib,"winmm.lib")
#pragma auto_inline(on)
/*--------------------------------------------------------------------------------*/
////////////////				HSEL Class Definition				////////////////
////////////////	HSEL===>(HWOARANG SANGWOO ENCRYPT LIBRARY)		////////////////
/*--------------------------------------------------------------------------------*/

#define	HSEL_DES_SINGLE			0x0001
#define	HSEL_DES_TRIPLE			0x0003

#define	HSEL_ENCRYPTTYPE_RAND	0x0000		
#define	HSEL_ENCRYPTTYPE_1		0x0010
#define	HSEL_ENCRYPTTYPE_2		0x0020
#define	HSEL_ENCRYPTTYPE_3		0x0040
#define	HSEL_ENCRYPTTYPE_4		0x0080
		
#define	HSEL_SWAP_FLAG_ON		0x0000
#define	HSEL_SWAP_FLAG_OFF		0x0100

#define	HSEL_KEY_TYPE_DEFAULT	0x0000	
#define	HSEL_KEY_TYPE_CUSTOMIZE	0x1000

typedef struct HSEL_KEY
{
	__int32 iLeftKey;		
	__int32 iRightKey;	
	__int32 iMiddleKey;	
	__int32 iTotalKey;	
	
	__int32 iLeftMultiGab;
	__int32 iRightMultiGab;
	__int32 iMiddleMultiGab;
	__int32 iTotalMultiGab;

	__int32 iLeftPlusGab;	
	__int32 iRightPlusGab;
	__int32 iMiddlePlusGab;
	__int32 iTotalPlusGab;

}HselKey,*lpHselKey;

typedef struct HSEL_INITIAL
{
	__int32			iDesCount	;
	__int32			iEncryptType;
	__int32			iSwapFlag	;
	__int32			iCustomize	;
	HselKey			Keys		;
}HselInit,lpHselInit;

class CHSEL
{
/*------------------------------------------------*/
////////////	public Members			////////////
/*------------------------------------------------*/
	public://Variable
	
	public://Inline Fuctions
		CHSEL(void)		{	iVersion = 0;	iHSELType = 0;	}
		~CHSEL(void)	{					}
		virtual __int32 GetVersion(void)	const{	return iVersion;		}//Return Version
		virtual __int32 GetHSELType(void)	const{	return iHSELType;	}
	public://Normal Fuctions(but use Macro)
/*------------------------------------------------*/
////////////	public Members	End		////////////
/*------------------------------------------------*/


/*------------------------------------------------*/
////////////	protected Members		////////////
/*------------------------------------------------*/
	protected://Variable
		__int32 iVersion;
		__int32 iHSELType;		//Now Types in __int32 
	protected://Inline Fuctions
	protected://Normal Fuctions(but use Macro)
/*------------------------------------------------*/
////////////	protected Members	End	////////////
/*------------------------------------------------*/


/*------------------------------------------------*/
////////////	private Members			////////////
/*------------------------------------------------*/
	private://Variable
	private://Inline Fuctions
	private://Normal Fuctions(but use Macro)
/*------------------------------------------------*/
////////////	private Members	End		////////////
/*------------------------------------------------*/
};
/*--------------------------------------------------------------------------------*/
////////////////			End Of HSEL Class Definition			////////////////
/*--------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------*/
////////////////			HSEL_STREAM Class Definition			////////////////
/*--------------------------------------------------------------------------------*/
class CHSEL_STREAM	:	public CHSEL
{	
/*------------------------------------------------*/
////////////	public Members			////////////
/*------------------------------------------------*/
	public://Variable

	public://Inline Fuctions
		CHSEL_STREAM(void);
		~CHSEL_STREAM(void);
	public://Normal Fuctions(but use Macro)
		__int32		Initial(HSEL_INITIAL hselinit);//initial Library
		
		HSEL_INITIAL	GetHSELCustomizeOption(void)const{	return	Init;		}
		
		HselKey			GetNowHSELKey(void)			const{	return Init.Keys;	}
		//////////////////////////////////////////////////////////////
		bool		Encrypt(char *lpStream,const __int32 iStreamSize = 0);//return crc
		bool		Decrypt(char *lpStream,const __int32 iStreamSize = 0);//return crc
		
		//값을 얻으려면 Encrpyt 또는  Decrpyt 가 실행된 뒤에 호출 할것
		char		GetCRCConvertChar(void)const;	//Encrypt 가 끝난 후에 crc 값을 Char 형으로 리턴
		short		GetCRCConvertShort(void)const;	//Encrypt 가 끝난 후에 crc 값을 short 형으로 리턴
		__int32		GetCRCConvertInt(void)const;	//Encrypt 가 끝난 후에 crc 값을 Int 형으로 리턴
		
		void		SetKeyCustom(HselKey IntoKey);
		
		void		SetNextKey(void);

		void		GenerateKeys(HselKey &IntoKey);
		//////////////////////////////////////////////////////////////

		
/*------------------------------------------------*/
////////////	public Members	End		////////////
/*------------------------------------------------*/

/*------------------------------------------------*/
////////////	protected Members		////////////
/*------------------------------------------------*/	
	protected://Variable
	protected://Inline Fuctions
	protected://Normal Fuctions(but use Macro)

/*------------------------------------------------*/
////////////	protected Members	End	////////////
/*------------------------------------------------*/	

/*------------------------------------------------*/
////////////	private Members			////////////
/*------------------------------------------------*/
	private://Variable
		
		__int32	iLPos0;
		__int32	iLPos1;
		__int32	iLPos2;
		__int32	iLPos3;
		
		__int32	iRPos0;
		__int32	iRPos1;
		__int32	iRPos2;
		__int32	iRPos3;
		
		__int32	iMPos0;
		__int32	iMPos1;
		__int32	iMPos2;
		__int32	iMPos3;

		__int32	iTempLeftKey;
		__int32	iTempRightKey;
		__int32	iTempMiddleKey;

		__int32	iBlockCount;	
		__int32	iRemainCount;

		__int32	iPos;
		
		__int32	iCRCValue;
		/*	Encryption Keys Total 32*4 Bit	*/
		HSEL_INITIAL	Init;//When Make Same Class Initial Option

	private://Inline Fuctions
	
	private://Normal Fuctions(but use Macro)

		//////////////////////////////////
		//		Function Po__int32ers		//
		//////////////////////////////////////////////////////////////
		void (CHSEL_STREAM::*lpDesEncryptType)(char *, const __int32);
		void (CHSEL_STREAM::*lpDesDecryptType)(char *, const __int32);
		//////////////////////////////////////////////////////////////
		void (CHSEL_STREAM::*lpDesLeftEncrypt)(char *, const __int32);
		void (CHSEL_STREAM::*lpDesLeftDecrypt)(char *, const __int32);
		
		void (CHSEL_STREAM::*lpDesRightEncrypt)(char *, const __int32);
		void (CHSEL_STREAM::*lpDesRightDecrypt)(char *, const __int32);
		
		void (CHSEL_STREAM::*lpDesMiddleEncrypt)(char *, const __int32);
		void (CHSEL_STREAM::*lpDesMiddleDecrypt)(char *, const __int32);
		//////////////////////////////////////////////////////////////
		void (CHSEL_STREAM::*lpSwapEncrypt)(char *, const __int32);
		void (CHSEL_STREAM::*lpSwapDecrypt)(char *, const __int32);
		//////////////////////////////////////////////////////////////
	
		bool ChackFaultStreamSize(const __int32 iStreamSize = 0);

		void GetCRC(char *lpStream = NULL, const __int32 iStreamSize = 0);
		
		//////		DES		/////
		void DESSingleEncode(char *lpStream = NULL, const __int32 iStreamSize = 0);
		void DESTripleEncode(char *lpStream = NULL, const __int32 iStreamSize = 0);

		void DESSingleDecode(char *lpStream = NULL, const __int32 iStreamSize = 0);
		void DESTripleDecode(char *lpStream = NULL, const __int32 iStreamSize = 0);

		void DESLeftEncode_Type_1(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESLeftDecode_Type_1(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESRightEncode_Type_1(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESRightDecode_Type_1(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESMiddleEncode_Type_1(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESMiddleDecode_Type_1(char *lpStream = NULL,	const __int32 iStreamSize = 0);

		void DESLeftEncode_Type_2(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESLeftDecode_Type_2(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESRightEncode_Type_2(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESRightDecode_Type_2(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESMiddleEncode_Type_2(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESMiddleDecode_Type_2(char *lpStream = NULL,	const __int32 iStreamSize = 0);

		void DESLeftEncode_Type_3(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESLeftDecode_Type_3(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESRightEncode_Type_3(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESRightDecode_Type_3(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESMiddleEncode_Type_3(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESMiddleDecode_Type_3(char *lpStream = NULL,	const __int32 iStreamSize = 0);

		void DESLeftEncode_Type_4(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESLeftDecode_Type_4(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESRightEncode_Type_4(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESRightDecode_Type_4(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESMiddleEncode_Type_4(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void DESMiddleDecode_Type_4(char *lpStream = NULL,	const __int32 iStreamSize = 0);

		//////		DES		/////
		void SwapEncrypt	(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void SwapDecrypt	(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void NoSwapEncrypt	(char *lpStream = NULL,	const __int32 iStreamSize = 0);
		void NoSwapDecrypt	(char *lpStream = NULL,	const __int32 iStreamSize = 0);
/*------------------------------------------------*/
////////////	private Members	End		////////////
/*------------------------------------------------*/
};
/*--------------------------------------------------------------------------------*/
////////////////		End Of HSEL_STREAM Class Definition			////////////////
/*--------------------------------------------------------------------------------*/
#endif // !defined(AFX_CONTROLMOUSE_H__DD9E7B83_DBC5_4447_9523_33FA26FF2F27__INCLUDED_)
