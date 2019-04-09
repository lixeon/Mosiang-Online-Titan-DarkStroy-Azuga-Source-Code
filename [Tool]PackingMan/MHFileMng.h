#ifndef __MHFILEMNG_H__
#define __MHFILEMNG_H__

#include "MHFileEx.h"

class CMHFileMng
{
protected:
	map<int, CMHFileEx*>			m_mapFile;
	map<int, CMHFileEx*>::iterator	mi;
	int								m_nFileNum;
	
public:
	CMHFileMng();
	virtual ~CMHFileMng();

	BOOL	AddFile( const char* fullfilename, char* mode );
	void	DeleteFile( int index );
	void	Clear();

	char*	GetFullFileName( int index );
	char*	GetFileName( int index );
	void	SetExt( const char* ext );
	void	SaveToBin();
	void	SaveAsBin( int index, const char* filename );
	void	SaveToTxt();
	CMHFileEx*	GetFile( int index );
	void	SetFileData( int index, char* str );
	void	New( const char* fullfilename );
	
	void	SaveToDOF();	
};

#endif