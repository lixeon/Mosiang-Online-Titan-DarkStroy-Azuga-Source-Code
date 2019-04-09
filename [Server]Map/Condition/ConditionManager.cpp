// ConditionManager.cpp: implementation of the CConditionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConditionManager.h"

#include "ConditionFactory.h"
#include "SITUATION.h"
#include "RESOURCE.h"
#include "DATA.h"
#include "ACTION.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConditionManager::CConditionManager()
{
	m_addID = 0;
	m_removeID = 0;
}

CConditionManager::~CConditionManager()
{

}
void CConditionManager::Initial()
{
	m_ConditionList.Initialize(100);
	m_ConditionPoolList.Initialize(100);
	CONDITIONFACTORY->Init();
}
void CConditionManager::Release()
{
	CConditionNode* pRoot;
	m_ConditionList.SetPositionHead();
	while(pRoot = m_ConditionList.GetData())
	{
		ReleaseSubNode(pRoot);
	}
	m_ConditionList.RemoveAll();

	m_ConditionPoolList.SetPositionHead();
	while(pRoot = m_ConditionPoolList.GetData())
	{
		ReleaseSubNode(pRoot);
	}
	m_ConditionPoolList.RemoveAll();
	CONDITIONFACTORY->Release();
}

void CConditionManager::ReleaseSubNode( CConditionNode * RootNode )
{
	CConditionNode * pOldRoot;
	while(RootNode)
	{
		if(RootNode->GetDataObject())
		{
			CONDITIONFACTORY->FreeDATA(RootNode->GetDataObject());
		}
		if(RootNode->GetSituationObject())
		{
			CONDITIONFACTORY->FreeSITUATION(RootNode->GetSituationObject());
		}
		if(RootNode->GetResourceObject())
		{
			CONDITIONFACTORY->FreeRESOURCE(RootNode->GetResourceObject());
		}

		ReleaseSubActionNode(RootNode->GetAction(RETURN_FALSE));
		ReleaseSubActionNode(RootNode->GetAction(RETURN_TRUE));

		pOldRoot = RootNode;
		RootNode = RootNode->NextNode();
		CONDITIONFACTORY->FreeConditionNode(pOldRoot);
	}
}

void CConditionManager::ReleaseSubActionNode( CACTION * RootNode )
{
	if(!RootNode) return;
	CONDITIONFACTORY->FreeACTION(RootNode);
	/*
	while(RootNode)
		{
			RootNode->
		}*/
	
}

void CConditionManager::AddRootNodeInList( CConditionNode * RootNode, DWORD id )
{
	m_ConditionPoolList.Add( RootNode, id );
}

void CConditionManager::AddListFromPool( DWORD id )
{
	CConditionNode * pRootNode = m_ConditionPoolList.GetData(id);
	ASSERT(pRootNode);
	m_ConditionPoolList.Remove(id);
	m_ConditionList.Add(pRootNode, id);
}

void CConditionManager::RemoveListToPool( DWORD id )
{
	CConditionNode * pRootNode = m_ConditionList.GetData(id);
	pRootNode->SetEnd(TRUE);
}

void CConditionManager::GarbageProcess()
{
	CConditionNode* pRoot;
	m_ConditionList.SetPositionHead();
	while(pRoot = m_ConditionList.GetData())
	{
		if(pRoot->IsEnd())
		{
			RemoveListToPool(pRoot->GetID());
		}
	}
}

void CConditionManager::Process()
{	
	CConditionNode* pRoot;
	m_ConditionList.SetPositionHead();
	while(pRoot = m_ConditionList.GetData())
	{
		if( pRoot )
		{
			CRETURN bBool = pRoot->Execute();
			if( bBool == RETURN_STOP) continue;
			pRoot->Action(NULL);
			CConditionNode * pNode = pRoot->NextNode();
			while( pNode )
			{
				bBool = pNode->Execute();
				if( bBool != RETURN_STOP )
				{
					pNode->Action(NULL);
					pNode = pNode->NextNode();
				}
				else
					pNode = NULL;
			}
		}
	}
	GarbageProcess();
}

// 임시로 테스트 LBS
void CConditionManager::LoadConditionList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( "./Resource/Condition_List.bin", "rb" ) )
		return;
#else
	if( !file.Init( CONDITIONLIST_PATH, "r" ) )
		return;
#endif

	char buff[256];
	DWORD dwConditionIdx;

	while(1)
	{
		file.GetString(buff);
		if(file.IsEOF())
		{
			break;
		}
		if(buff[0] == '@') // 주석
		{
			file.GetLineX(buff, 256);
			continue;
		}
		strcpy(buff, _strupr(buff));
		CMD_ST(buff)
		CMD_CS("#CONDITION")
			// CONDITIONNODE를 생성
			dwConditionIdx = file.GetDword(); // 어디에 사용??
			if((file.GetString())[0] == '{')
			{
				CConditionNode* pRootNode =  CONDITIONFACTORY->NewConditionNode();
				pRootNode->SetID(dwConditionIdx);
				LoadRootNodeInfo( &file, pRootNode ); // 안의 내용을 읽는다.
				AddRootNodeInList( pRootNode, dwConditionIdx ); // 내용을 리스트에 첨가한다.
			}
			else
				__asm int 3;
		CMD_EN
	}
	
	file.Release();
}

void CConditionManager::LoadRootNodeInfo( CMHFile* fp, CConditionNode* pRootNode  )
{
	char buff[256];

	while(1)
	{
		if(fp->IsEOF())
		{
			break;
		}
		fp->GetString(buff);
		if( buff[0] == '}' )
		{
			return; // 
		}
		if(buff[0] == '@') // 주석
		{
			fp->GetLineX(buff, 256);
			continue;
		}
		CMD_ST(buff)
		CMD_CS("_if") // TRUE
			LoadConditionObject( fp, pRootNode );

			if((fp->GetString())[0] == '{')
			{
				LoadSubNodeInfo( fp, pRootNode, RETURN_TRUE );
			}
			else
				__asm int 3;
		CMD_CS("_else") // FALSE
			if((fp->GetString())[0] == '{')
			{
				LoadSubNodeInfo( fp, pRootNode, RETURN_FALSE );
			}
			else
				__asm int 3;
		CMD_EN
	}	
}

void CConditionManager::LoadSubNodeInfo( CMHFile* fp, CConditionNode* pNode, CRETURN bCondition  )
{
	char				buff[256] = {0,};
	char				temp[256] = {0,};

	CConditionNode*		pNewNode = NULL;
	WORD				wRepeat = 0;
	WORD				wSearchType = 0;
	DWORD				dwExpireTime = 0;
	DWORD				dwActionIdx = 0;
	
	while(1)
	{
		fp->GetString(buff);

		if(fp->IsEOF())
			return;

		if(buff[0] == '}')
		{
			return;
		}
		if(buff[0] == '@') // 주석
		{
			fp->GetLineX(buff, 256);
			continue;
		}
		CMD_ST(buff)
		CMD_CS("_searchType")
			fp->GetString(temp);
			if( temp[0] != '(') break;

			wSearchType = fp->GetWord();

			fp->GetString(temp);
			if( temp[0] != ')') break;

			pNode->SetSearchType( wSearchType );
		CMD_CS("_expireTime")
			fp->GetString(temp);
			if( temp[0] != '(') break;

			dwExpireTime = fp->GetDword();

			fp->GetString(temp);
			if( temp[0] != ')') break;

			pNode->SetExpireTime( dwExpireTime );

		CMD_CS("#ACTION")
			dwActionIdx = fp->GetDword();
			CACTION* action = CONDITIONFACTORY->NewACTION();
			action->SetFUNC( globalACTION[dwActionIdx] );
			pNode->SetAction( action, bCondition );
		CMD_CS("_if") // TRUE
			LoadConditionObject( fp, pNode );
			pNewNode = CONDITIONFACTORY->NewConditionNode();
			pNode->SetSubNode( pNewNode, bCondition );

			if((fp->GetString())[0] == '{')
			{
				LoadSubNodeInfo( fp, pNewNode, RETURN_TRUE );
			}
			else
				__asm int 3;
		CMD_CS("_else") // FALSE
			if((fp->GetString())[0] == '{')
			{
				if( pNewNode )
					LoadSubNodeInfo( fp, pNewNode, RETURN_FALSE );
				else
					__asm int 3;
			}
			else
				__asm int 3;
		CMD_EN
	}	
}

void CConditionManager::LoadConditionObject( CMHFile* fp, CConditionNode* pNode )
{
	char	temp[256];
	DWORD	dwSituationIdx;
	

	fp->GetString(temp);
	if( temp[0] != '(') return;

	fp->GetString(temp);
	CMD_ST(temp)
	CMD_CS("#SITUATIONIDX")
		dwSituationIdx = fp->GetDword();
		SITUATION* situation = CONDITIONFACTORY->NewSITUATION();
		situation->SetFUNC( globalSITUATION[dwSituationIdx] );
		pNode->SetSituationObject( situation );
	CMD_EN

	fp->GetString(temp);
	if( temp[0] != '(') return;
}
