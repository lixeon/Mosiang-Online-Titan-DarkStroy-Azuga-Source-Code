#ifndef _COMBOBOX_EX_
#define _COMBOBOX_EX_

#include "./interface/cComboBox.h"


/* ÄÞº¸¹Ú½ºÀÇ ¼±ÅÃ¿¡ µû¸¥ º¯¼öÀÇ Á¦¾î¸¦ À§ÇØ¼­ ¸¸µé¾îÁü.. LBS 03.10.29 */
class cComboBoxEx : public cComboBox
{
	int			m_nOptionValue;

public:
	cComboBoxEx();
	virtual ~cComboBoxEx();

	virtual void ListMouseCheck( CMouse * mouseInfo );

	void SetOptionValue( int nValue ) { m_nOptionValue = nValue;}
	int GetOptionValue() { return m_nOptionValue; }
};

#endif //_COMBOBOX_EX_