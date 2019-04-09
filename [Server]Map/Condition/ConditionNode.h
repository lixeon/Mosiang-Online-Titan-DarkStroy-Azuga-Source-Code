// ConditionNode.h: interface for the CConditionNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONDITIONNODE_H__6B34E43F_1F99_49EC_9AA9_80880F7602A5__INCLUDED_)
#define AFX_CONDITIONNODE_H__6B34E43F_1F99_49EC_9AA9_80880F7602A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum CRETURN
{
	RETURN_FALSE	= 0 ,
	RETURN_TRUE			,
	MAX_RETURN			,
	RETURN_NOTUSE		,
	RETURN_STOP			,
};

enum NODESEARCHTYPE
{
	SEARCH_STOP			= 0 ,			// implicit FALSE, Æ®¸®°Å°°Àº Á÷Á¢¼¼ÆÃ¹æ¹ý
	SEARCH_BASIC			,
	SEARCH_JUMP_TRUE		,
	SEARCH_JUMP_FALSE		,
};

#define MAX_DEPEND_OBJECT			10

class SITUATION;
class RESOURCE;
class DATA;
class CACTION;


class CConditionNode  
{
public:
	CConditionNode();
	virtual ~CConditionNode();

	CRETURN					Execute();

	// --- ¹Ýµå½Ã Execute¸¦ ÇÏ°í ³ª¼­ ½ÇÇàÇÏ´Â ÇÔ¼ö
	void					Action(CObject * pObj);
	CConditionNode *		NextNode();// CBOOL type );
	// --- 
	void					SetEnd(BOOL bval){ bEndProcess = bval;	}
	BOOL					IsEnd()						{ return bEndProcess; }
	void					SetExpireTime(DWORD t)		{ 
															m_dwExpireTime	= t; 
															m_dwStartTime	= gCurTime; 
															m_bExpireTime	= FALSE; 
														}
	void					SetSearchType( WORD type )	{	m_wNodeSearchType = type;	}

	void					SetSubNode( CConditionNode* pNode, CRETURN bCondition );
	void					SetAction( CACTION* pAction, CRETURN bCondition );
	CACTION *				GetAction(CRETURN bCondition) { return pACTION[bCondition]; }
	

	void					SetDataObject( DATA * data ) { pDATA = data; }
	void					SetSituationObject( SITUATION * situation) { pSITUATION = situation; }
	void					SetResourceObject( RESOURCE * resource ) { pRESOURCE = resource; }

	DATA *					GetDataObject() { return pDATA; }
	SITUATION *				GetSituationObject() { return pSITUATION; }
	RESOURCE *				GetResourceObject() { return pRESOURCE; }
	DWORD					GetID() { return m_dwConditionID;	}
	void					SetID(DWORD id) { m_dwConditionID = id;	}
protected:
	WORD					m_dwID[ MAX_DEPEND_OBJECT ];
	DWORD					m_dwConditionID;
	//WORD					m_Volatile;						// MGR

	// --- TIME
	BOOL					m_bExpireTime;
	DWORD					m_dwStartTime;
	DWORD					m_dwExpireTime;					// MGR
	// --- TIME

	WORD					m_wNodeSearchType;

	CRETURN					m_bCondition;


	// expand condition check
	DATA *					pDATA;
	SITUATION *				pSITUATION;
	RESOURCE *				pRESOURCE;
	
	// action Tree Root notify
	CACTION *				pACTION[ MAX_RETURN ];


	CConditionNode *		pSubNode[ MAX_RETURN ];
	BOOL					bEndProcess;
};

#endif // !defined(AFX_CONDITIONNODE_H__6B34E43F_1F99_49EC_9AA9_80880F7602A5__INCLUDED_)
