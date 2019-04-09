#ifndef _CLASS_CHANGE_VALUE_
#define _CLASS_CHANGE_VALUE_

struct CHANGEINFO
{
	OPER_KIND		OperKind;
	TABLE_KIND		TableKind;
};

class CChangeValue {
// Attribute
protected:
	CHANGEINFO		m_sInfo;
	WORD			m_wPos;				// DataÀÇ ¹è¿­ÀÎµ¦½º

public:
	CChangeValue();
	virtual ~CChangeValue();

	virtual void Init(OPER_KIND OperKind, TABLE_KIND TableKind, WORD wPos);
	virtual void ApplyChangeValue(CBaseDoc* pDoc) = 0;

	OPER_KIND	GetOperKind()	{ return m_sInfo.OperKind; }
	TABLE_KIND	GetTableKind()	{ return m_sInfo.TableKind; }
	WORD		GetDataPos()	{ return m_wPos; }

// Service Func
public:
	CHANGEINFO* GetChangeBaseInfo() { return &m_sInfo; }
};

class CChangeData : public CChangeValue {
protected:
	BYTE		m_bType;
	CString		m_strBeforeData;
	CString		m_strAfterData;

public:
	CChangeData();
	virtual ~CChangeData();

	void SetData(char* strBefore, char* strAfter, BYTE bType);
	BOOL IsTextTypeData() { return (!m_bType)? TRUE:FALSE; }

	virtual void ApplyChangeValue(CBaseDoc* pDoc);
};

class CChangeItem : public CChangeValue {
protected:
	ITEMBASE			m_BeforeItem;
	ITEMBASE			m_AfterItem;

	ITEM_OPTION_INFO	m_BeforeOption;
	ITEM_OPTION_INFO	m_AfterOption;

public:
	CChangeItem();
	virtual ~CChangeItem();

	void SetData( ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption );
	virtual void ApplyChangeValue(CBaseDoc* pDoc);
	DWORD GetTargetIdx(CBaseDoc* pDoc);
	char* GetTargetName( CBaseDoc* pDoc );
	BYTE GetTableIdx() { return (m_sInfo.TableKind == eTK_InventoryItem)? 0:((m_sInfo.TableKind == eTK_PyogukItem)? 1:2); }
};

class CChangeMugong : public CChangeValue {
protected:
	MUGONGBASE		m_BeforeMugong;
	MUGONGBASE		m_AfterMugong;

public:
	CChangeMugong();
	virtual ~CChangeMugong();

	void SetData( MUGONGBASE* pBMugong, MUGONGBASE* pAMugong );
	virtual void ApplyChangeValue(CBaseDoc* pDoc);
	DWORD GetTargetIdx(CBaseDoc* pDoc);
	char* GetTargetName(CBaseDoc* pDoc);
};


class CChangePosition : public CChangeValue
{
protected:
	WORD		m_wKind;		// 1 = Item, 2 = Mugong
	DWORD		m_dwOwnerIdx;
	DWORD		m_dwDBIdx;
	POSTYPE		m_Position;
	
public:
	CChangePosition();
	virtual ~CChangePosition();

	virtual void	ApplyChangeValue( CBaseDoc* pDoc );
	void			SetData( WORD wType, DWORD dwOwnerIdx, DWORD dwDBIdx, POSTYPE position );
};

#endif //_CLASS_CHANGE_VALUE_