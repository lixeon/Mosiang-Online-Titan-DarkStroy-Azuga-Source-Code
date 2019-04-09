#pragma once


#define FORTWARMGR USINGTON(CFortWarManager)

class CFortWarManager
{
protected:
	DWORD m_nRelationFortWarIDX;
	int m_nTaxRate;					// 1/10000

public:
	CFortWarManager(void);
	~CFortWarManager(void);

	void Init();

	void AddSymbolNpc( float x, float z);
	void RemoveSymbolNpc();
	DWORD CalTexRateForSell( DWORD dwMoney );
	DWORD CalTexRateForBuy( DWORD dwMoney );

	void NetworkMsgParse( BYTE Protocol, void* pMsg );
};

EXTERNGLOBALTON(CFortWarManager)
