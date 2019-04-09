// Encryptor.cpp: implementation of the CEncryptor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Encryptor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEncryptor::CEncryptor()
{
	m_dwEncryptionKey = 0;
	m_EncryptionKey = 0;
}

CEncryptor::~CEncryptor()
{
	Release();
}

void CEncryptor::Init(char EncryptionKey)
{
	m_EncryptionKey = EncryptionKey;
	char* temp = (char*)&m_dwEncryptionKey;
	for(int n=0;n<4;++n)
	{
		temp[n] = m_EncryptionKey;
	}
}
void CEncryptor::Release()
{
}

void CEncryptor::Encrypt(void* pMsg,int DataLen)
{
	DWORD* pdwTemp = (DWORD*)pMsg;
	char* pTemp = (char*)pMsg;
	int len_div4 = DataLen / 4;
	int n;
	for(n=0;n<len_div4;++n)
	{
		pdwTemp[n] ^= m_dwEncryptionKey;
	}
	for(n=n*4;n<DataLen;++n)
	{
		pTemp[n] ^= m_EncryptionKey;
	}
}
void CEncryptor::DeEncrypt(void* pMsg,int DataLen)
{
	DWORD* pdwTemp = (DWORD*)pMsg;
	char* pTemp = (char*)pMsg;
	int len_div4 = DataLen / 4;
	int n;
	for(n=0;n<len_div4;++n)
	{
		pdwTemp[n] ^= m_dwEncryptionKey;
	}
	for(n=n*4;n<DataLen;++n)
	{
		pTemp[n] ^= m_EncryptionKey;
	}
}
