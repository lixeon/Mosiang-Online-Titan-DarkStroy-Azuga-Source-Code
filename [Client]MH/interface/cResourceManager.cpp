// cResourceManager.cpp: implementation of the cResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cResourceManager.h"
#include "../Engine/GraphicEngine.h"
#include "PtrList.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(cResourceManager);
cResourceManager::cResourceManager()
{
	m_pImageList = NULL;
	m_pImageInfoArray = NULL;
	m_nMaxImageInfoNum = 0;
}

cResourceManager::~cResourceManager()
{
	Release();
}
void cResourceManager::Init(char * szImagePath, char * szMsgPath)
{
	if(!m_pImageList)
		m_pImageList = new cPtrList;

	strcpy( m_szImageListPath,	szImagePath );
	strcpy( m_szMsgListPath,	szMsgPath );


	// ImageInfo List
	CMHFile fp;
#ifdef _FILE_BIN_
	fp.Init(m_szImageListPath, "rb");
#else
	fp.Init(m_szImageListPath, "rt");	
#endif
	m_nMaxImageInfoNum = fp.GetInt();
	m_pImageInfoArray = new IMAGE_NODE[ m_nMaxImageInfoNum ];
	for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
	{
		fp.GetInt();
		strcpy( m_pImageInfoArray[i].szFileName, fp.GetString() );
		m_pImageInfoArray[i].size.x			= fp.GetFloat();
		m_pImageInfoArray[i].size.y			= fp.GetFloat();
		m_pImageInfoArray[i].layer			= fp.GetInt();
		m_pImageInfoArray[i].pSpriteObject	= NULL;
	}
	fp.Release();

/*
#ifdef _FILE_BIN_
	char temp[255];
	fp.Init(m_szImageListPath, "rb");
	m_nMaxImageInfoNum = fp.GetInt();
//	fp.GetLine( temp, 255 );
	m_pImageInfoArray = new IMAGE_NODE[ m_nMaxImageInfoNum ];
	for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
	{
		fp.GetInt();
		strcpy( m_pImageInfoArray[i].szFileName, fp.GetString() );
		m_pImageInfoArray[i].size.x			= fp.GetFloat();
		m_pImageInfoArray[i].size.y			= fp.GetFloat();
		m_pImageInfoArray[i].layer			= fp.GetInt();
		m_pImageInfoArray[i].pSpriteObject	= NULL;
//		fp.GetLine( temp, 255 );
	}
	fp.Release();
#else
	fp.Init(m_szImageListPath, "rt");
	m_nMaxImageInfoNum = fp.GetInt();
	m_pImageInfoArray = new IMAGE_NODE[ m_nMaxImageInfoNum ];
	for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
	{
		fp.GetInt();
		strcpy( m_pImageInfoArray[i].szFileName, fp.GetString() );
		m_pImageInfoArray[i].size.x			= fp.GetFloat();
		m_pImageInfoArray[i].size.y			= fp.GetFloat();
		m_pImageInfoArray[i].layer			= fp.GetInt();
		m_pImageInfoArray[i].pSpriteObject	= NULL;
	}
	fp.Release();
#endif
	*/
	// ImageInfo List
	/*
	fp.Init(m_szMsgListPath, "rt");
	for( i = 0 ; i < idx-1 ; ++i )
	{
		fp.GetLine(msg, size);
	}
	fp.GetInt();
	fp.Seek(1);
	fp.GetLine(msg, size);
	fp.Release();
	*/
}
IMAGE_NODE * cResourceManager::GetInfo( int idx )
{	
	if( idx < 0 )
		return NULL;
	if( idx >= m_nMaxImageInfoNum ) 
		return NULL;
	return &m_pImageInfoArray[idx];
}
IDISpriteObject * cResourceManager::GetImageInfo( int idx )
{
	if( idx >= m_nMaxImageInfoNum ) 
		return NULL;

	if( !m_pImageInfoArray[idx].pSpriteObject )
	{
		m_pImageInfoArray[idx].pSpriteObject = g_pExecutive->GetRenderer()->CreateSpriteObject(m_pImageInfoArray[idx].szFileName,0);
	}

	return ( m_pImageInfoArray[idx].pSpriteObject );
}
IDISpriteObject * cResourceManager::GetImageInfo( char * fileName, cImageSize size, int layer )
{

	for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
	{
		if( m_pImageInfoArray[i].pSpriteObject && 0 == strcmp( m_pImageInfoArray[i].szFileName, fileName ) )
		{
			return m_pImageInfoArray[i].pSpriteObject;
		}
	}

	PTRLISTPOS pos = m_pImageList->GetHeadPosition();
	while(pos)
	{
		IMAGE_NODE * node = (IMAGE_NODE *)m_pImageList->GetNext(pos);
		if(0 == strcmp( node->szFileName, fileName ) )
		{	
			return node->pSpriteObject;
		}
	}

	IMAGE_NODE * pNewNode	= new IMAGE_NODE;
	strcpy( pNewNode->szFileName, fileName );
	pNewNode->pSpriteObject = g_pExecutive->GetRenderer()->CreateSpriteObject(fileName,0);
	pNewNode->layer			= layer;
	pNewNode->size			= size;
	
	if( !pNewNode->pSpriteObject )
	{
		char buffer[255];
		GetCurrentDirectory(255, buffer);
		strcat(buffer, fileName);
		ASSERTMSG(0, buffer);
		if( pNewNode ) delete pNewNode;
	}

	m_pImageList->AddTail( pNewNode );
	return pNewNode->pSpriteObject;
}


void cResourceManager::Release()
{
	ReleaseResourceAll();
	if(m_pImageInfoArray)
	{
		delete [] m_pImageInfoArray;
		m_pImageInfoArray = NULL;
	}
	if(m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

void cResourceManager::ReleaseResourceAll()
{
	if( m_pImageList )
	{	
		for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
		{
			if( m_pImageInfoArray[i].pSpriteObject )
			{
				m_pImageInfoArray[i].pSpriteObject->Release();
				m_pImageInfoArray[i].pSpriteObject = NULL;
			}
		}

		PTRLISTPOS pos = m_pImageList->GetHeadPosition();
		while(pos)
		{
			IMAGE_NODE * node = (IMAGE_NODE *)m_pImageList->GetNext(pos);
			node->pSpriteObject->Release();
			node->pSpriteObject = NULL;
			SAFE_DELETE(node);
		}
		m_pImageList->RemoveAll();
	}
}

char * cResourceManager::GetMsg(int idx)
{
	static char msg[255];
	CMHFile fp;
#ifdef _FILE_BIN_
	fp.Init(m_szMsgListPath, "rb");
	for(int i = 0 ; i < idx-1 ; i++)
	{
		fp.GetLine(msg, 255);
	}
	fp.GetInt();
//	fp.Seek(1);
	fp.GetLine(msg, 255);
	fp.Release();
#else
	fp.Init(m_szMsgListPath, "rt");
	for(int i = 0 ; i < idx-1 ; i++)
	{
		fp.GetLine(msg, 255);
	}
	fp.GetInt();
	fp.Seek(1);
	fp.GetLine(msg, 255);
	fp.Release();
#endif

	return msg;
}


void cResourceManager::ReleaseResource(int layer)
{
	for( int i = 0 ; i < m_nMaxImageInfoNum ; ++i )
	{
		if( m_pImageInfoArray[i].pSpriteObject && m_pImageInfoArray[i].layer == layer)
		{
			m_pImageInfoArray[i].pSpriteObject->Release();
			m_pImageInfoArray[i].pSpriteObject = NULL;
			return;
		}
	}

	PTRLISTPOS pos = m_pImageList->GetHeadPosition();
	while(pos)
	{
		IMAGE_NODE * node = (IMAGE_NODE *)m_pImageList->GetAt(pos);
		if(node->layer == layer)
		{
			node->pSpriteObject->Release();
			node->pSpriteObject = NULL;
			SAFE_DELETE(node);
			m_pImageList->RemoveAt(pos);
			break;
		}
		m_pImageList->GetNext(pos);
	}
}
