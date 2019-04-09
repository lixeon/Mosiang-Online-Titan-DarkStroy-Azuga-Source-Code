// cObject.h: interface for the cObject class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __cOBJECT_H__
#define __cOBJECT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cObject  
{
public:
	cObject();
	virtual ~cObject();


	virtual void OnCreate(BOOL val)
	{
		m_bActive = val;
	}
	virtual void SetActive(BOOL val) 
	{ 
		if( !m_bDisable ) m_bActive=val; 
	}
	BOOL IsActive() const{ return m_bActive; }

	virtual void SetDisable(BOOL val) 
	{ 
		m_bDisable=val; 
	}
	BOOL IsDisable() const{ return m_bDisable; }

	void SetParent(cObject * pParent) 
	{ 
		m_pParent = pParent; 
	}
	cObject * const GetParent() const { return m_pParent; }

	void SetID(LONG id) { m_ID=id; }
	LONG GetID() const { return m_ID; }

	void SetType(WORD type) { m_type=type; }
	WORD GetType() const { return m_type; }

protected:
	cObject * m_pParent;
	LONG	m_ID;
	WORD	m_type;
	BOOL	m_bActive;
	BOOL	m_bDisable;
};

#endif // __cOBJECT_H__
