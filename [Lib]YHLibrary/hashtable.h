#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "stdafx.h"

typedef void* YHTPOSITION;

template <class T>
class CYHHashTable
{	
	struct BUCKET;
	
public:
	struct LIST
	{
		LIST*		pNext;
		LIST*		pPrv;
		BUCKET*		pBucket;
//		LIST()		{pNext = NULL; pPrv = NULL; pBucket = NULL;}
	};
	
private:
	struct BUCKET
	{
		DWORD		dwKey;
		T*			pVoid;
		LIST*		pList;
		BUCKET*		pNext;
		BUCKET*		pPrv;
		BUCKET(T* pvoid,DWORD key)
		{
			pVoid = pvoid;
			dwKey = key;
			pNext = NULL;
			pPrv = NULL;
		}		
	};
	
	protected:
		DWORD				m_dwDataNum;
		DWORD				m_dwMaxBucketNum;
		BUCKET**			m_ppBucketTable;
		LIST*				m_pListHead;
		LIST*				m_pListTail;
		LIST*				m_pListCur;
		
		BUCKET*				m_pCurBucket;
		BUCKET*				m_pLastBucket;
		DWORD				m_MultiDataKey;
		
	public:
		DWORD	inline		GetDataNum() {return m_dwDataNum;}
		bool				Initialize(DWORD dwMaxBucketNum)
		{
			if( m_ppBucketTable || m_dwMaxBucketNum )
				return false;

			m_dwDataNum = 0;
			m_dwMaxBucketNum = dwMaxBucketNum;

			m_pListHead = NULL;
			m_pListTail = NULL;
			m_pListCur = NULL;

			m_ppBucketTable = new BUCKET*[m_dwMaxBucketNum];
			
			if (!m_ppBucketTable)
				return false;
			
			memset(m_ppBucketTable,0,sizeof(BUCKET*)*m_dwMaxBucketNum);
			
			return true;
			
		}
		
		LIST*				AddList(BUCKET* pBucket)
		{
			LIST* pNewList = new LIST;
			pNewList->pNext = NULL;
			pNewList->pPrv	= NULL;
			pNewList->pBucket = pBucket;
			pBucket->pList = pNewList;

			if( m_pListTail )
			{
				pNewList->pPrv = m_pListTail;
				m_pListTail->pNext = pNewList;
				m_pListTail = pNewList;
			}
			else
			{
				m_pListTail = m_pListHead = pNewList;
			}

			return pNewList;
		}
		void				RemoveList(LIST* pList)
		{
			if( pList->pPrv == NULL )
				m_pListHead = pList->pNext;
			else
				pList->pPrv->pNext = pList->pNext;

			if( pList->pNext )
				pList->pNext->pPrv = pList->pPrv;
			else
				m_pListTail = pList->pPrv;

			if( pList == m_pListCur )
				m_pListCur = pList->pNext;

			delete pList;
		}
		
		bool				Add(T* pVoid,DWORD dwKey)
		{
			DWORD index = dwKey % m_dwMaxBucketNum;
			
			BUCKET* pBucket = new BUCKET(pVoid,dwKey);

			if( !m_ppBucketTable[index] )
			{
				m_ppBucketTable[index] = pBucket;
			}
			else
			{
				pBucket->pNext = m_ppBucketTable[index];
				m_ppBucketTable[index]->pPrv = pBucket;
				m_ppBucketTable[index] = pBucket;				
			}
			
			AddList( pBucket );
			++m_dwDataNum;

			return true;
		}
		
		T*					GetData(DWORD dwKey)
		{
			DWORD index = dwKey%m_dwMaxBucketNum;
			
			BUCKET* pBucket = m_ppBucketTable[index];
			
			while(pBucket)
			{
				if (pBucket->dwKey == dwKey)
				{
					return pBucket->pVoid;
				}
				pBucket = pBucket->pNext;
			}
			return NULL;
			
		}
		
		T*			 		GetData()
		{
			if( !m_pListCur ) return NULL;

			T*  pVoid = m_pListCur->pBucket->pVoid;
			m_pListCur = m_pListCur->pNext;
			return pVoid;
		}
		
		T*			 		GetDataPos(YHTPOSITION* ppList)
		{
			if ( *ppList == NULL ) return NULL;

			T*	pVoid = (*((LIST**)ppList))->pBucket->pVoid;
			*ppList = (*((LIST**)ppList))->pNext;
			return pVoid;
		}

		void				StartGetMultiData(DWORD dwKey)
		{
			DWORD index = dwKey%m_dwMaxBucketNum;			
			m_pCurBucket = m_ppBucketTable[index];
			m_pLastBucket = NULL;
			m_MultiDataKey = dwKey;
		}
		T*					GetMultiData()
		{
			while(m_pCurBucket)
			{
				if (m_pCurBucket->dwKey == m_MultiDataKey)
				{
					m_pLastBucket = m_pCurBucket;					
					m_pCurBucket = m_pCurBucket->pNext;
					return m_pLastBucket->pVoid;
				}
				m_pCurBucket = m_pCurBucket->pNext;
			}
			return NULL;
		}
		void				RemoveCurMultiData()
		{
			if( m_pLastBucket == NULL )
				return;


			BUCKET*	pBucket = m_pLastBucket;

			if( pBucket->pPrv == NULL )
			{
				DWORD dwIndex = m_MultiDataKey%m_dwMaxBucketNum;
				m_ppBucketTable[dwIndex] = pBucket->pNext;
			}
			else
			{
				pBucket->pPrv->pNext = pBucket->pNext;
			}

			if( pBucket->pNext )
				pBucket->pNext->pPrv = pBucket->pPrv;

			m_pLastBucket = NULL;
			
			RemoveList(pBucket->pList);

			delete pBucket;

			--m_dwDataNum;
		}

		
		inline 		void	SetPositionHead() {m_pListCur = m_pListHead;}
		inline 		YHTPOSITION	GetPositionHead() {return m_pListHead;}

		void				Remove(DWORD dwKey)
		{
			DWORD dwIndex = dwKey%m_dwMaxBucketNum;
			
			BUCKET*	pBucket = m_ppBucketTable[dwIndex];
			
			while (pBucket)
			{
				if (pBucket->dwKey == dwKey)
				{
					if( pBucket->pPrv == NULL )
						m_ppBucketTable[dwIndex] = pBucket->pNext;
					else
						pBucket->pPrv->pNext = pBucket->pNext;

					if( pBucket->pNext )
						pBucket->pNext->pPrv = pBucket->pPrv;

					RemoveList(pBucket->pList);

					delete pBucket;

					--m_dwDataNum;

					return;
				}

				pBucket = pBucket->pNext;
			}
		}
		
		void				RemoveAll()
		{
			LIST*	cur = m_pListHead;
			LIST*	next = NULL;
			
			while (cur)
			{
				next = cur->pNext;
				delete cur->pBucket;
				delete cur;
				cur = next;
			}

			m_dwDataNum = 0;
			m_pListHead = NULL;
			m_pListTail = NULL;
			m_pListCur = NULL;
			
			if(m_ppBucketTable)
			{
				memset(m_ppBucketTable,0,sizeof(BUCKET*)*m_dwMaxBucketNum);
			}
		}
		
		CYHHashTable()
		{
			m_dwDataNum = 0;
			m_dwMaxBucketNum = 0;
			m_ppBucketTable = NULL;
			m_pListHead = NULL;
			m_pListTail = NULL;
			m_pListCur = NULL;
		}
		
		~CYHHashTable()
		{
			RemoveAll();
			if (m_ppBucketTable)
			{
				delete [] m_ppBucketTable;
				m_ppBucketTable = NULL;
			}
		}
};

#endif
