// Encryptor.h: interface for the CEncryptor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENCRYPTOR_H__CE1CBE02_DA20_4B2E_9EC2_801BD3F51130__INCLUDED_)
#define AFX_ENCRYPTOR_H__CE1CBE02_DA20_4B2E_9EC2_801BD3F51130__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEncryptor  
{
	DWORD m_dwEncryptionKey;
	char m_EncryptionKey;
public:
	CEncryptor();
	virtual ~CEncryptor();

	void Init(char EncryptionKey);
	void Release();

	void Encrypt(void* pMsg,int DataLen);
	void DeEncrypt(void* pMsg,int DataLen);	

};

#endif // !defined(AFX_ENCRYPTOR_H__CE1CBE02_DA20_4B2E_9EC2_801BD3F51130__INCLUDED_)
