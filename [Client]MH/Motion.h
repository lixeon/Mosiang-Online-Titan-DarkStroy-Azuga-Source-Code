#ifndef _MOTION_H_
#define _MOTION_H_


enum eMOTION
{
	eMotion_Peace_Standard					 = 1,
	eMotion_Peace_Walk							,
	eMotion_Peace_Run							,
	
	eMotion_Battle_Gum_Standard					,
	eMotion_Battle_Gwun_Standard				,
	eMotion_Battle_Do_Standard					,
	eMotion_Battle_Chang_Standard				,
	eMotion_Battle_Gung_Standard				,
	eMotion_Battle_Amgi_Standard				,

	eMotion_Battle_Gum_Walk						,
	eMotion_Battle_Gwun_Walk					,
	eMotion_Battle_Do_Walk						,
	eMotion_Battle_Chang_Walk					,
	eMotion_Battle_Gung_Walk					,
	eMotion_Battle_Amgi_Walk					,
	
	eMotion_Battle_Gum_Run						,
	eMotion_Battle_Gwun_Run						,
	eMotion_Battle_Do_Run						,
	eMotion_Battle_Chang_Run					,
	eMotion_Battle_Gung_Run						,
	eMotion_Battle_Amgi_Run						,

	eMotion_Damage_Left							,
	eMotion_Damage_Right						,
	eMotion_Damage_Front						,

	eMotion_Die_Fly								,
	eMotion_Die_Normal							,

	eMotion_KyungGong1_Start_NoWeapon			,
	eMotion_KyungGong1_Move_NoWeapon			,
	eMotion_KyungGong1_End_NoWeapon				,

	eMotion_KyungGong2_Start_NoWeapon			,
	eMotion_KyungGong2_Move_NoWeapon			,
	eMotion_KyungGong2_End_NoWeapon				,

	eMotion_KyungGong3_Start_NoWeapon			,
	eMotion_KyungGong3_Move_NoWeapon			,
	eMotion_KyungGong3_End_NoWeapon				,

	eMotion_Ungijosik_Start						,
	eMotion_Ungijosik_Ing						,
	eMotion_Ungijosik_End						,
	
	eMotion_Ungijosik_Damage					,

	eMotion_StreetStall_Start					,
	eMotion_StreetStall_Ing						,
	eMotion_StreetStall_End						,
	
	eMotion_Died_Normal							,
	eMotion_Died_Fly							,

	eMotion_Defence_Gum							,
	eMotion_Defence_Gwon						,
	eMotion_Defence_Do							,
	eMotion_Defence_Chang						,
	eMotion_Defence_Gung						,
	eMotion_Defence_Amgi						,
		
	eMotion_Peace_Standard_Do					,
	eMotion_Peace_Standard_Change				,
	eMotion_Peace_Walk_Do						,
	eMotion_Peace_Walk_Change					,
	eMotion_Peace_Run_Do						,
	eMotion_Peace_Run_Change					,

	// 중간에 공격동작들이 있는데 그건 무공리스트에서 직접 입력하기때문에 필요없다.

//커뮤니티 동작
	eMotion_Society_Bow							= 251,
	eMotion_Society_Happy						,
	eMotion_Society_Sad							,
	eMotion_Society_Yes							,
	eMotion_Society_No							,
	eMotion_Society_Sit_Start					,
	eMotion_Society_Sit_Ing						,
	eMotion_Society_Stand						,
	eMotion_Society_BigBow						,

// 아이템사용 애니
	eMotion_Item_Dyeing_1						= 260,	// 검, 권, 궁, 암기
	eMotion_Item_Dyeing_2						,		// 도
	eMotion_Item_Dyeing_3						,		// 창
	eMotion_Item_ChangeFace_1					,		// 검, 권, 궁, 암기
	eMotion_Item_ChangeFace_2					,		// 도
	eMotion_Item_ChangeFace_3					,		// 창
	eMotion_Item_ChangeHair_1					,		// 검, 권, 궁, 암기
	eMotion_Item_ChangeHair_2					,		// 도
	eMotion_Item_ChangeHair_3					,		// 창
	eMotion_Item_Teleport_1						,		// 검, 권, 궁, 암기
	eMotion_Item_Teleport_2						,		// 도
	eMotion_Item_Teleport_3						,		// 창


// pjs [3/11/2003]	로그인 행동들...
	eMotion_Sel1Before						 = 290, // PJS 애니가 맞는지 시험  [5/15/2003]
	eMotion_sel1After						 ,
	eMotion_Sel1Afterstop					 ,
	eMotion_Sel1Back						 ,

	eMotion_Sel2Before						 ,
	eMotion_Sel2After						 ,
	eMotion_Sel2Afterstop					 ,
	eMotion_Sel2Back						 ,

	eMotion_Sel3Before						 ,
	eMotion_Sel3After						 ,
	eMotion_Sel3Afterstop					 ,
	eMotion_Sel3Back						 ,

	eMotion_Sel4Before						 ,
	eMotion_Sel4After						 ,
	eMotion_Sel4Afterstop					 ,
	eMotion_Sel4Back						 ,

	eMotion_Sel5Before						 = 286,
	eMotion_Sel5After						 = 287,
	eMotion_Sel5Afterstop					 = 288,
	eMotion_Sel5Back						 = 289,

	// magi82 - Motion(070920) 캐릭터 선택창에서 캐릭터 선택시 모션 관련 업데이트
	eMotion_Gum_Before						= 282,
	eMotion_Gum_After						,
	eMotion_Gum_AfterStop					,
	eMotion_Gum_Back						,

	eMotion_Kwon_Before						,
	eMotion_Kwon_After						,
	eMotion_Kwon_AfterStop					,
	eMotion_Kwon_Back						,

	eMotion_Do_Before						,
	eMotion_Do_After						,
	eMotion_Do_AfterStop					,
	eMotion_Do_Back							,

	eMotion_Chang_Before					,
	eMotion_Chang_After						,
	eMotion_Chang_AfterStop					,
	eMotion_Chang_Back						,

	eMotion_Gung_Before						,
	eMotion_Gung_After						,
	eMotion_Gung_AfterStop					,
	eMotion_Gung_Back						,

	eMotion_Amki_Before						,
	eMotion_Amki_After						,
	eMotion_Amki_AfterStop					,
	eMotion_Amki_Back						,

	//////////////////////////////////////////////////////////////////////////
	
	//2007. 10. 25. CBH - 각인 모션 번호 정의
	eMotion_Engrave_Start						= 326,
	eMotion_Engrave_End							= 327,
	eMotion_Engrave_Ing							= 328,	
	//

	eMotion_Tactic_Start						= 330,
	eMotion_Tactic_Ing							= 331,
	eMotion_Tactic_Excute						= 332,
/*
	eMotion_Emoticon_Bow = 150,//인사
	eMotion_Emoticon_Thanks = 151,//감사
	eMotion_Emoticon_Yes = 152,//긍정
	eMotion_Emoticon_No = 153,//부정
	eMotion_Emoticon_Pleasure = 154,//기쁨
	eMotion_Emoticon_Sadness = 155,//슬픔
	eMotion_Emoticon_Love = 156,//사랑
	eMotion_Emoticon_Hostility = 157,//적대
	eMotion_Emoticon_Disappoint = 158,//실망
	eMotion_Emoticon_Angry = 159,//분노
	eMotion_Emoticon_Suprised = 160,//놀람
	eMotion_Emoticon_Evasion = 161,//회피
	eMotion_Emoticon_Declare = 162,//선언
*/	
	// 특수무공 동작들도 공격 동작과 마찬가지로 처리

};

enum eMONSTERMOTION
{
	eMonsterMotion_Standard = 1,
	eMonsterMotion_Walk = 2,
	eMonsterMotion_Attack1 = 3,
	eMonsterMotion_Attack2 = 4,
	eMonsterMotion_DamageFront = 5,
	eMonsterMotion_DamageLeft = 6,
	eMonsterMotion_DamageRight = 7,
	eMonsterMotion_FlyDie = 8,			// 날라가서 죽기
	eMonsterMotion_Die = 9,				// 제자리에서 죽기
	eMonsterMotion_Died = 10,			// 죽어서 멈춰있기
	eMonsterMotion_FlyDied = 11,		// 날라가서 죽어서 멈춰있기
};

enum eBOSSMONSTERMOTION
{
	eBossMonsterMotion_Stand		= 1,
	eBossMonsterMotion_Walk			= 2,
	
	eBossMonsterMotion_Attack1		= 3,		// 일반 공격
	eBossMonsterMotion_Attack2		= 4,		// 소환
	eBossMonsterMotion_Attack3		= 5,		// 찍기

	eBossMonsterMotion_Attack4		= 6,		// NULL
	eBossMonsterMotion_Attack5		= 7,		// NULL

	eBossMonsterMotion_Die			= 8,
	eBossMonsterMotion_Died			= 9,
	
	eBossMonsterMotion_RestDown		= 10,
	eBossMonsterMotion_RestOn		= 11,
	eBossMonsterMotion_RestUp		= 12,
};


#endif
