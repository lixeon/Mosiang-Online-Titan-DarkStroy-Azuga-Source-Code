// OPDATA.h: interface for the COPDATA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPDATA_H__92EE46CF_D042_4B3A_8B13_BB4B1CFF5E4D__INCLUDED_)
#define AFX_OPDATA_H__92EE46CF_D042_4B3A_8B13_BB4B1CFF5E4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum OPERATERTYPE
{
	PLUS_TYPE,							// +
	MINUS_TYPE,							// -
	MULTIPLE_TYPE,						// *
	DIVIDE_TYPE,						// /
	MODULER_TYPE,						// %
	LESSTHAN_TYPE,						// <
	MORETHAN_TYPE,						// >
	LTEQUAL_TYPE,						// <=
	MTEQUAL_TYPE,						// >=
	EQUAL_TYPE,							// ==
};

enum OPTYPE
{
	OP_DW,			//	DWORD
	OP_IN,			//	int
	OP_LG,			//	LONG
};

class OPBASE
{
public:
	DWORD	GetOPCode()	{ return opCode;	}
	OPTYPE	GetOPType()	{ return opType;	}
	OPTYPE		opType;
	DWORD		opCode;
};
template<class Type>
class COPDATA  : public OPBASE
{
public:
	COPDATA(){}
	virtual ~COPDATA(){}

	void	SetOPData(Type & data) { opData = data;	}
	Type &	GetOPData() { return opData;	}
private:

	Type	&	opData;

};

#endif // !defined(AFX_OPDATA_H__92EE46CF_D042_4B3A_8B13_BB4B1CFF5E4D__INCLUDED_)
