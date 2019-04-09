#ifndef _SURYUNDEFINE_H_
#define _SURYUNDEFINE_H_

#define SURYUNMAP		44

#define MAX_MUGONG_PRACTICE  3
#define MAX_MONSTER_PERLEVEL  5
#define MAX_MONIDX_PERMONSTERINFO  3

#define SURYUNENTER_POSITION_X	10650
#define SURYUNENTER_POSITION_Z	12500

#define	COMPLETELY_ALL_RECOVER 147
#define	NORMAL_LIFE_RECOVER 148
#define	NORMAL_SHIELD_RECOVER 149
#define	NORMAL_NAERYUK_RECOVER 150

struct SURYUNINFO
{
	BYTE AimSung;
	DWORD LimitTime;
	int MonKind;
	int MonNum;
	MONEYTYPE Fee;
	int RegenNum;
	int AdjustLevel;
};

struct SURYUNLEVELCHANGE
{
	int ChangeLevel[3];
};

struct SURYUNMONSTER
{
	int count;
	WORD MonsterIdx[3];
};
struct SURYUNMONSTERINFO
{
	LEVELTYPE Level;
	SURYUNMONSTER MonsterIdxArray[MAX_MONSTER_PERLEVEL];

	void GetMonsterArray(WORD* pMonsterArray);
};

struct SURYUNMISSIONINFO
{
	WORD MugongIdx;	
	SURYUNINFO MissionInfo[MAX_MUGONG_PRACTICE];

	SURYUNINFO* GetSuryunInfo(BYTE currentsung) 
	{
		for(int n=0;n<MAX_MUGONG_PRACTICE;++n)
		{
			if(MissionInfo[n].AimSung-1 == currentsung)
				return &MissionInfo[n];
		}
		return NULL;
	}

};


#endif