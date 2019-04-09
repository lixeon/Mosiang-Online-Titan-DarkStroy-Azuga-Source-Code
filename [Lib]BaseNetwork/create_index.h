#pragma once


#include "stdafx.h"

struct INDEX_DESC
{
	DWORD			m_dwIndex;
	INDEX_DESC*		m_pNext;
};

#define		INDEX_DESC_SIZE		8

class CIndexCreator
{

	INDEX_DESC*			m_pIndexList;
	INDEX_DESC*			m_pBaseDesc;
	INDEX_DESC**		m_ppIndexDescTable;
	DWORD				m_dwIndexNum;
	DWORD				m_dwMaxIndexNum;

public:
	BOOL				Initialize(DWORD dwMaxNum);
	DWORD				GetIndexNum()	{return m_dwIndexNum;}


	CIndexCreator();
	~CIndexCreator();

};
DWORD	ICAllocIndex(CIndexCreator* pIC);
void 	ICFreeIndex(CIndexCreator* pIC,DWORD dwIndex);

