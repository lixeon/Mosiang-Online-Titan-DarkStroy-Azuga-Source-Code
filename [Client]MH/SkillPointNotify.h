// SkillPointNotify.h: interface for the CSkillPointNotify class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLPOINTNOTIFY_H__8B6FCF04_3EBE_4307_90A6_21D5A2896E3A__INCLUDED_)
#define AFX_SKILLPOINTNOTIFY_H__8B6FCF04_3EBE_4307_90A6_21D5A2896E3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"


class cButton;
class cTextArea;



class CSkillPointNotify : public cDialog
{
	cButton*		m_RedistBtn;
	cTextArea*		m_Notifyta1;
	cTextArea*		m_Notifyta2;
	
public:
	CSkillPointNotify();
	virtual ~CSkillPointNotify();

	void Linking();
	void InitTextArea();
};

#endif // !defined(AFX_SKILLPOINTNOTIFY_H__8B6FCF04_3EBE_4307_90A6_21D5A2896E3A__INCLUDED_)
