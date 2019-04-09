#pragma once

#include "AbilityUse.h"


class CAbilityUse_Pet :	public CAbilityUse
{
public:
	CAbilityUse_Pet(void);
	virtual ~CAbilityUse_Pet(void);

	virtual void Use(BYTE Level,CAbilityInfo* pAbilityInfo);
};
