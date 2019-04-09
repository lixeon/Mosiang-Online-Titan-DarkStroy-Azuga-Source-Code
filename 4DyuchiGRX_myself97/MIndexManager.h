/*
	MIndexManager
	
	시간 날때 Release 한방에 하도록 고쳐야 할듯. 
*/
#if !defined(AFX_MINDEXMANAGER_H__E9D7E92E_CD14_44FB_8C09_7EAF6B0A2F5E__INCLUDED_)
#define AFX_MINDEXMANAGER_H__E9D7E92E_CD14_44FB_8C09_7EAF6B0A2F5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchigrx_common/stdafx.h"

struct	MIndexNode		// 더블 링크드리스트로...
{
//	int				iPrev;		// 인덱스 링크. -_-;
//	int				iNext;
	short				iPrev;		// 인덱스 링크. -_-;
	short				iNext;
	int iIndex;
};

class MIndexManager  
{
public:
	MIndexManager();
	~MIndexManager();

	void Init(int iMaxIndexCount);		// 초기화, iMaxIndexCount는 최대 인덱스의 갯수이다.

	int GetIndex(void);					// 비어있는 인덱스 하나를 얻는다. 최대한 앞으로 몰아서,
	BOOL ReleaseIndex(int iIndex);		// 인덱스 하나를 반환한다.

	DWORD ReleaseLastCreated(void);			// 가장 오래전에 생성된 인덱스 한놈을 Release 하고 그놈의 인덱스를 리턴.

	DWORD	GetCurrentIndex( int** ppIndexArray);		// 현재 얼록되어있는 인덱스 버퍼를 제공한다. 리턴값은 갯수. get 할때마다 연산한다.

	void Delete(void);

	void	ReleaseAllIndex(void);			// 할당된 모든 인덱스를 릴리즈한다.
protected:
	int				miEmptyIndexEntry;		// 비어있는 인덱스의 체인 인덱스.
	int				miUsedIndexEntry;			// 사용되는 인덱스의 체인 인덱스.

	MIndexNode*		mpBuffer;			// 버퍼. 지울때 한방이면 된다.

	int				miMaxIndexCount;		// 최대 인덱스 갯수. 0부터 이거-1까지.
	int				miUsedIndexCount;	// 사용되고 있는 인덱스 갯수.

	int*			mpIndexArrayBuff;		// GetCurrentIndex 에서 사용될 외부 출력용 버퍼.

};

#endif // !defined(AFX_MINDEXMANAGER_H__E9D7E92E_CD14_44FB_8C09_7EAF6B0A2F5E__INCLUDED_)
