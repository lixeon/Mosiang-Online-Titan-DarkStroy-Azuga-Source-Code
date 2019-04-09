// MIndexManager.cpp: implementation of the MIndexManager class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "MIndexManager.h"
#include <memory.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MIndexManager::MIndexManager()
{
	memset( this, 0, sizeof(MIndexManager));
}

MIndexManager::~MIndexManager()
{
	this->Delete();
}

void MIndexManager::Init(int iMaxIndexCount)
{
#ifdef	_DEBUG
	if( mpBuffer != 0)	// 이미 초기화되어 사용되고 있다면,
	{
		_asm int 3;
		this->Delete();
	}
#endif
	mpBuffer			=	new	MIndexNode[iMaxIndexCount];
	miMaxIndexCount	=	iMaxIndexCount;

	miUsedIndexEntry	=	-2;		// 없단 뜻...

	for( int i = 0; i < miMaxIndexCount; i++)
	{
		mpBuffer[i].iIndex = i;
		mpBuffer[i].iNext = i+1;
		mpBuffer[i].iPrev = i-1;
	}
	// 제일 첫번째 인덱스는 iPrev로 -2를, 제일 마지막 인덱스의 iNext도 -2;
	mpBuffer[0].iPrev	=	-2;
	mpBuffer[miMaxIndexCount-1].iNext = -2;

	miUsedIndexCount	=	0;
	miEmptyIndexEntry	=	0;

	mpIndexArrayBuff	=	new	int[iMaxIndexCount];
}

int MIndexManager::GetIndex()
{
#ifdef	_DEBUG
	if( miUsedIndexCount >= miMaxIndexCount)	// 더이상 발급 가능한 인덱스가 없다.
	{
		_asm int 3;
		// 최대 사용량 초과.
		return -1;
	}
#endif
	// 인덱스 발급.
	if( miUsedIndexCount <= 0)		// 발급된놈이 하나도 없고, 지금게 처음일때.
	{
		// empty에서 하나 가져온다.
		miUsedIndexEntry	=	miEmptyIndexEntry;

		miEmptyIndexEntry	=	mpBuffer[miEmptyIndexEntry].iNext;	// 링크를 끊고, 
		mpBuffer[miEmptyIndexEntry].iPrev	=	-2;

		mpBuffer[miUsedIndexEntry].iNext	=	-2;
		mpBuffer[miUsedIndexEntry].iPrev	=	-2;

		miUsedIndexCount	=	1; 

		return mpBuffer[miUsedIndexEntry].iIndex;
	}

	// 지금 할당해줄 노드.
	int	iTempNode	=	miEmptyIndexEntry;		

	// 엠프티노드의 뒤 노드 링크.
	if( mpBuffer[iTempNode].iNext == -2)		// 마지막 노드였다면...
	{
		miEmptyIndexEntry	=	-2;
	}
	else
	{
		miEmptyIndexEntry	=	mpBuffer[iTempNode].iNext;
		mpBuffer[miEmptyIndexEntry].iPrev	=	-2;		// 필요 없는 코드일지도...
	}

	// 할당할 노드 정리.
	mpBuffer[iTempNode].iNext	=	miUsedIndexEntry;		
	mpBuffer[iTempNode].iPrev	=	-2;
	miUsedIndexEntry		=	iTempNode;

	mpBuffer[mpBuffer[iTempNode].iNext].iPrev		=	iTempNode;

	miUsedIndexCount++;

	return mpBuffer[iTempNode].iIndex;
}

BOOL MIndexManager::ReleaseIndex(int iIndex)
{
	int		i = 0;
	int		iTempNode	=	-2;
#ifdef	_DEBUG
	if( iIndex < 0 || iIndex >= miMaxIndexCount)	// init때 넣은 인자보다 큰수가 들어왔다.
	{
		_asm int 3;
		return FALSE;
	}
	if( miUsedIndexCount <= 0)	// 인덱스가 사용되지 않고 있는데 호출됬다.
	{
		_asm int 3;
		return	FALSE;
	}
	if( miUsedIndexCount > 0 && miUsedIndexEntry == -2)	// 내부 인자 오류.
	{
		_asm int 3;
		return	FALSE;
	}
#endif
	// 인덱스가 단 하나 남았을때
	if( miUsedIndexCount == 1 || mpBuffer[miUsedIndexEntry].iNext == -2)
	{
#ifdef	_DEBUG
		if( mpBuffer[miUsedIndexEntry].iIndex != iIndex)		// 혹시나...
		{
			_asm int 3;		// 설마... 
			return FALSE;
		}
#endif
		// 노드를 iEmptyIndexEntry에 붙이고,
		iTempNode	=	miUsedIndexEntry;
		if( miEmptyIndexEntry != -2)
		{
			mpBuffer[miEmptyIndexEntry].iPrev	=	iTempNode;
		}
		mpBuffer[iTempNode].iNext	=	miEmptyIndexEntry;
		mpBuffer[iTempNode].iPrev	=	-2;
		miEmptyIndexEntry		=	iTempNode;

		miUsedIndexEntry		=	-2;	
		miUsedIndexCount		=	0;
		return	TRUE;
	}

	// 인덱스가 두개 이상일때.
	iTempNode	=	miUsedIndexEntry;
	for( i = 0; i < miUsedIndexCount; iTempNode = mpBuffer[iTempNode].iNext, i++)
	{
		if( mpBuffer[iTempNode].iIndex == iIndex)	// 찾던 노드면,
		{
			if( mpBuffer[iTempNode].iPrev == -2)		// 지금 노드가 제일 앞 노드라면,
			{
				mpBuffer[mpBuffer[iTempNode].iNext].iPrev	=	-2;				// iUsedIndexEntry에서 링크를 끊고,
				miUsedIndexEntry	=	mpBuffer[iTempNode].iNext;

			goto	ReturnIndex;
			}
			if( mpBuffer[iTempNode].iNext == -2)		// 지금 노드가 제일 뒷 노드라면,
			{
				// iUsed에서 연결 끊고,
				mpBuffer[mpBuffer[iTempNode].iPrev].iNext	=	-2;

				// iEmpty에 붙이고,
				goto	ReturnIndex;
			}

			// 노드의 앞뒤로 노드가 있을때. 
			// iUsed에서 노드를 끊어낸다.
			mpBuffer[mpBuffer[iTempNode].iPrev].iNext	=	mpBuffer[iTempNode].iNext;
			mpBuffer[mpBuffer[iTempNode].iNext].iPrev	=	mpBuffer[iTempNode].iPrev;
		
			// iEmpty에 끼워넣기.
			goto	ReturnIndex;
		}
	}

#ifdef	_DEBUG
	// 여기까지 새어 나오면 곤란하다.
	_asm int 3;		// iIndex란놈이 없다. 뭔가 잘못된 경우.
	return FALSE;
#endif

ReturnIndex:
			if( miEmptyIndexEntry != -2)	// 엠프티에 노드가 있으면,
			{
				mpBuffer[miEmptyIndexEntry].iPrev	=	iTempNode;
			}
			mpBuffer[iTempNode].iNext			=	miEmptyIndexEntry;
			mpBuffer[iTempNode].iPrev			=	-2;
			miEmptyIndexEntry				=	iTempNode;

			miUsedIndexCount--;


			return	TRUE;
}
// 가장 오래전에 생성된 인덱스 한놈을 Release 한다..
DWORD MIndexManager::ReleaseLastCreated(void)
{
	// 대충, 노드의 끝까지 가서, (그놈이 가장 오래전에 만들어진 놈일테니)
	// 그놈을 Release 하고 리턴.
	if( miUsedIndexCount <= 0)	// 인덱스가 사용되지 않고 있는데 호출됬다.
	{
		_asm int 3;			// 어쨌든 목적은 달성 했으니 주석 처리 해도 상관없지 않을까...
		return	;
	}

	int		iCurrentNode	=	miUsedIndexEntry;
	for( ; mpBuffer[iCurrentNode].iNext != -2; iCurrentNode = mpBuffer[iCurrentNode].iNext)	;

	// 지금의 인덱스가 마지막 인덱스겠지.
	BOOL	bResult	=	ReleaseIndex( iCurrentNode);
	if( bResult == FALSE)
		_asm int 3;

	return	iCurrentNode;
}

void MIndexManager::Delete()
{
	if( miUsedIndexCount != 0)
	{
		// 아직 트리의 다이나믹 오브젝트가 지워지지 않았다.
//		_asm int 3;
	}
	if( mpBuffer)	delete[]	mpBuffer;
	mpBuffer			=	0;
	miMaxIndexCount	=	0;
	miUsedIndexCount	=	0;

	delete[]	mpIndexArrayBuff;
	mpIndexArrayBuff	=	0;

}

// 할당된 모든 인덱스를 릴리즈한다.
void MIndexManager::ReleaseAllIndex(void)
{
	// 그냥 Delete() 한번, Init 한번. -_-;
	int		iMaxIndexCount	=	miMaxIndexCount;
	Delete();
	Init(iMaxIndexCount);
}

// 현재 얼록되어있는 인덱스 버퍼를 제공한다. 리턴값은 갯수.
DWORD MIndexManager::GetCurrentIndex( int** ppIndexArray)
{
	if( miUsedIndexCount == 0)
		return	0;
	// used 체인을 돌면서 인덱스를 하나하나 찍어간다.
	DWORD	dwAllocatedCount	=	0;
	DWORD	dwCurrentNode	=	miUsedIndexEntry;
	for(   ; mpBuffer[dwCurrentNode].iNext != -2; dwCurrentNode = mpBuffer[dwCurrentNode].iNext)
	{
		mpIndexArrayBuff[dwAllocatedCount]	=	mpBuffer[dwCurrentNode].iIndex;
		dwAllocatedCount++;
	}
	mpIndexArrayBuff[dwAllocatedCount]	=	mpBuffer[dwCurrentNode].iIndex;			// 제일 마지막 노드. -_-;	  조건이 맞을 경우 한번더 실행해야하는데..
	dwAllocatedCount++;

	*ppIndexArray	=	mpIndexArrayBuff;

	if( dwAllocatedCount != miUsedIndexCount)
		_asm int 3;

	return	dwAllocatedCount;

	
}
