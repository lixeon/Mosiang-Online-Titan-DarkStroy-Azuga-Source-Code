//
// Engine Object Cache
//
//			trustpak
//	




#ifndef _ENGINE_OBJECT_CACHE_
#define _ENGINE_OBJECT_CACHE_



struct CACHE_CONTAIN_DATA
{
	BOOL				bUsage;
	DWORD				dwKey;
	GXOBJECT_HANDLE		gxo;
};



class CEngineObjectCache
{
public:
	enum 
	{
		MAX_ENGINE_OBJECT_CACHE_SIZE = 256,
	};

private:
	
	I4DyuchiGXExecutive*			m_pExecutive;
	
	cPtrList						m_lstGXObjects;		// This list is a Queue. It means that this list does FIFO action.
	CYHHashTable<GXOBJECT_HANDLE>	m_hashGXObject;
	CACHE_CONTAIN_DATA*				m_pContainDataPool;

	DWORD							m_dwCacheSize;

	DWORD							m_dwLastDeleteTick;

private:

	DWORD					CreateCacheHashKey(char* pszKeyData, int iKeyDataLength);
	
	void					DeleteOldCacheDatas(DWORD dwDeleteNumber);
	void					DeleteOneOldCacheData(void);


	CACHE_CONTAIN_DATA*		AllocContainData(void);
	VOID					FreeContainData(CACHE_CONTAIN_DATA* pContainData);

public:
	CEngineObjectCache();
	~CEngineObjectCache();

	BOOL Init(I4DyuchiGXExecutive* pExecutive, DWORD dwCacheSize);
	BOOL Release(VOID);	

	VOID ProcessCacheDelete(void);

	GXOBJECT_HANDLE CreateGXObjectThroughCache(char* pszFileName, GXSchedulePROC pProc,void* pData,DWORD dwFlag);		
};

extern CEngineObjectCache* g_pEngineObjectCache; 


#endif