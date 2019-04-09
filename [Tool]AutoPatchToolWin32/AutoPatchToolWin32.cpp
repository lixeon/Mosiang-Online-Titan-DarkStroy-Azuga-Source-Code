// AutoPatchToolWin32.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "..\[Lib]ZipArchive\ZipArchive.h"
#include "mmsystem.h"

#pragma warning( disable : 4786 )

#include <map>
#include "DirectoryName.h"
#include "StrClass.h"
#include "YHFileFind.h"

using namespace std;

int MAX_PATCH_FILENUM =	10;

map<DWORD,CDirectoryName*> m_DirNameMap;

void CompressAll(CStrClass filename);
void CompressAndAddToBaseFile(CStrClass filename);
void Makepatch();

//CStrClass desFolder = "\\\\203.238.38.160\\mh_share\\";
CStrClass desFolder;
CStrClass Header;

void GetPatchInfo()
{
	char temp[256];

	FILE* fp = fopen("PatchInfo.txt","r");
	if(fp == NULL)
	{
		desFolder = ".\\";
		Header = "TEST";
		return;
	}

	fscanf(fp,"%s",temp);
	desFolder = temp;
	if( temp[strlen(temp)-1] != '\\' &&
		temp[strlen(temp)-1] != '/' )
		desFolder += "\\";

	fscanf(fp,"%s",temp);
	if(strlen(temp) != 4)
	{
		MessageBox(NULL, "Length of Header Must Be 4!!!",0,0);
		exit(0);
	}
	Header = temp;

	fscanf(fp,"%s",temp);
	MAX_PATCH_FILENUM = atoi(temp);
	if(MAX_PATCH_FILENUM == 0)
		MAX_PATCH_FILENUM = 20;
		

	fclose(fp);

}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	DWORD startt = timeGetTime();

	GetPatchInfo();

	Makepatch();

	return 0;
}



void Makepatch()
{
	{
		// Delete All Zip Files
		char curdirectroy[512];
		GetCurrentDirectory(512,curdirectroy);
		SetCurrentDirectory(desFolder);

		CYHFileFind ff;
		ff.FindFile();
		BOOL bHasNext = TRUE;
		while(bHasNext = ff.FindNextFile())
		{
			if(ff.IsDirectory() == TRUE)
				continue;

			CStrClass filename = ff.GetFileName();
			CStrClass filnameHeader = filename.Left(4);
			filnameHeader.MakeUpper();
			if(filnameHeader == Header)
			{
				BOOL rt = DeleteFile(filename);
				if(rt == FALSE)
				{
					char temp[256];
					sprintf(temp,"파일 삭제 실패 (%s)",filename);
					MessageBox(NULL, temp, NULL,NULL);
				}
			}
		}

		SetCurrentDirectory(curdirectroy);
	}
	
	{	// load patch vers
		CYHFileFind ff;
		ff.FindFile();
		BOOL bHasNext = TRUE;
		while(bHasNext = ff.FindNextFile())
		{
			if(ff.IsDirectory() == FALSE)
				continue;
			
			CStrClass dirname = ff.GetFileName();
			DWORD ver = atoi((LPCTSTR)dirname);
			if(ver == 0 && strcmp(dirname,"00000000") != 0)
				continue;
			
			CDirectoryName* pDirName = new CDirectoryName(NULL,NULL);
			
			SetCurrentDirectory(dirname);
			pDirName->LoadChild();
			SetCurrentDirectory("..");
			
			m_DirNameMap.insert(map<DWORD,CDirectoryName*>::value_type(ver,pDirName));
		}
	}


	CStrClass strTempDir = "tempdir";
	{	// process(make patch files..)
		CreateDirectory(strTempDir,NULL);
		map<DWORD,CDirectoryName*>::reverse_iterator iter;
		CStrClass strDirName;
		CStrClass srcName,desName,filename;
		DWORD ver;
		CDirectoryName* pDirName;

		{	// Copy Newest Version and Make "MHVerInfo_New.ver" with Newest Ver
			iter = m_DirNameMap.rbegin();
			ver = (*iter).first;
			pDirName = (*iter).second;
			char temp[1024];
			sprintf(temp,"%08d",ver);
			strDirName = temp;
			
			CStrClass newverFilename = Header+"MHVerInfo_New.ver";
			FILE* fp = fopen(newverFilename,"w");
			fprintf(fp,"%s%s",(LPCTSTR)Header,(LPCTSTR)strDirName);
			fclose(fp);
			CopyFile(newverFilename, strDirName + "\\" + newverFilename, FALSE);
			CopyFile(newverFilename, desFolder + newverFilename, FALSE);

			SetCurrentDirectory(strDirName);
			
			CompressAll("..\\BASE.zip");

			SetCurrentDirectory("..");
			++iter;
		}
		
		{	// Copy Other Version And Make [EACH VER].Zip file
			for(int count = 0;iter != m_DirNameMap.rend();++iter,++count)
			{
				ver = (*iter).first;
				pDirName = (*iter).second;
				char temp[1024];
				sprintf(temp,"%08d",ver);
				strDirName = temp;
				
				CStrClass pathfilename = Header;
				pathfilename += strDirName;

				// 공유폴더로...
				pathfilename = desFolder + pathfilename;

				if( count < MAX_PATCH_FILENUM ||		// 지정된 수까지만 복사를 한다.
					ver == 0)							// ????0000000.zip 은 반드시 복사
				{
					BOOL rt = CopyFile("BASE.zip",pathfilename + ".zip",FALSE);
					if(rt == FALSE)
					{
						MessageBox(NULL,"파일 복사 실패: 대상 폴더와의 연결을 확인해주세요.",NULL,NULL);
						return;
					}
				}

				// make zip file
				SetCurrentDirectory(strDirName);
				CompressAndAddToBaseFile("..\\BASE.zip");
				SetCurrentDirectory("..");				
			}
		}
	}


	{	// free
		map<DWORD,CDirectoryName*>::iterator iter;
		for(iter = m_DirNameMap.begin();iter != m_DirNameMap.end();++iter)
		{
			CDirectoryName* pDirName = (*iter).second;
			delete pDirName;
		}
		m_DirNameMap.clear();

		// 파일 지우기
		SetCurrentDirectory(strTempDir);
		CDirectoryName dir(NULL,NULL);
		dir.LoadChild();
		
		CStrClass filename;
		dir.SetPositionHead();
		while(DWORD kind = dir.GetNextFileName(&filename))
		{
			if(kind == GETNEXTFILENAME_FILE)
			{
				BOOL rt = DeleteFile(filename);
			}
		}
		SetCurrentDirectory("..");

		DeleteFile("BASE.zip");
	}
}

void CompressAll(CStrClass filename)
{
	bool rt;
	DWORD kind;
	CStrClass addedfile;

	CDirectoryName dir(NULL,NULL);
	dir.LoadChild();

	CZipArchive zip;
	// create archive in memory
	zip.Open(filename, CZipArchive::zipCreate);
	
	// add files to zip
	dir.SetPositionHead();
	while(kind = dir.GetNextFileName(&addedfile))
	{
		if( kind == GETNEXTFILENAME_FILE)
		{
			rt = zip.AddNewFile(addedfile,Z_BEST_SPEED,true,CZipArchive::zipsmSafeSmart,65536);
			ASSERT(rt);
		}
	}
	zip.Flush();
	zip.Close();
}

void CompressAndAddToBaseFile(CStrClass filename)
{
	bool rt;
	DWORD kind;
	CStrClass addedfile;

	CDirectoryName dir(NULL,NULL);
	dir.LoadChild();

	CZipArchive zip;
	// create archive in memory
	zip.Open(filename, CZipArchive::zipOpen);
	
	// add files to zip
	dir.SetPositionHead();
	while(kind = dir.GetNextFileName(&addedfile))
	{
		if( kind == GETNEXTFILENAME_FILE)
		{
			CStrClass temp = addedfile.Right(2);
			temp.MakeUpper();
			if( temp == "DB" )
				continue;
			if(zip.FindFile(addedfile) != -1)
				continue;
			SetFileAttributes(addedfile,FILE_ATTRIBUTE_NORMAL);
			rt = zip.AddNewFile(addedfile,Z_BEST_SPEED,true,CZipArchive::zipsmSafeSmart,65536);
			ASSERT(rt);
		}
	}

	zip.Flush();
	zip.Close();
}