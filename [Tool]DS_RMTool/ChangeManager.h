#ifndef	_CHANGE_MANAGER_
#define	_CHANGE_MANAGER_

//////////////////////////////////////
//	º¯°æ µ¥ÀÌÅÍ¿¡ ´ëÇÑ °ü¸®¸¦ ÇÑ´Ù. //
//////////////////////////////////////

class CChangeValue;
class CBaseDoc;

class CChangeManager {
protected:
	CYHHashTable<CChangeValue>*			m_pChangeEditInfo;
	CYHHashTable<CChangeValue>*			m_pChangeBtnInfo;
	CYHHashTable<CChangeValue>			m_ChangePosition;

	CBaseDoc*							m_pDocument;

public:
	CChangeManager();
	virtual ~CChangeManager();

	void Init(CBaseDoc* pDoc);
	void ProcessList();

	// ±âÁ¸ Á¤º¸°¡ ÀÖ´ÂÁö °Ë»ç ÇÑ´Ù.
	CChangeValue* ExistChangeInfo(DWORD Idx, BOOL bEdit = TRUE); 

	void Add(CChangeValue* info, DWORD Idx, BOOL bEdit = TRUE); // Ãß°¡
	void Delete(DWORD Idx, BOOL bEdit = TRUE);
	void DeleteAll();

	void AddPositionTable( CChangeValue* pInfo, DWORD dwItemDBIdx );

public: //service
	TABLE_KIND GetTableKind_ByField(INFOFIELD field);
};

#endif //_CHANGE_MANAGER_