// MonitoringServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "MSSystem.h"
#include "stdio.h"

//SLT
#include "../[CC]ServerModule/ServerListManager.h"

#define MAX_LOADSTRING 100

HWND gMainHwnd = NULL;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

BOOL g_bConsole = FALSE;
HANDLE g_hOutConsole;
void ReleaseConsole();
void InitConsole();
void LogConsole(char * buff,...);
// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
CMSSystem * g_pMSSystem = NULL;
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
//	/*
	// 복구툴 서버는 TYPE_RMS = 3
	// 복구툴 서버를 위해 번호 4번으로...
	if(atoi(lpCmdLine) == 4)
	{			
		{
		//Adult 1
		CServerListManager mgr;
		mgr.SetServerSetNum(1);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"192.168.242.36",20001,"222.234.222.36",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"192.168.242.36",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"192.168.242.12",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"192.168.242.13",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"192.168.242.14",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"192.168.242.1",19005,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,5,"192.168.242.11",19006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,6,"192.168.242.15",19007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,7,"192.168.242.37",19008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,8,"192.168.242.38",19009,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,9,"192.168.242.39",19010,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"192.168.242.36",16001,"222.234.222.36",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"192.168.242.12",17001,"222.234.222.12",14600));
		mgr.Add(&SERVERINFO(AGENT_SERVER,1,"192.168.242.13",17002,"222.234.222.13",14601));
		mgr.Add(&SERVERINFO(AGENT_SERVER,2,"192.168.242.14",17003,"222.234.222.14",14602));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"192.168.242.1",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"192.168.242.1",18008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"192.168.242.1",18035,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,65,"192.168.242.11",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"192.168.242.11",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"192.168.242.11",18002,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,20,"192.168.242.15",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"192.168.242.15",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,19,"192.168.242.37",18019,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"192.168.242.37",18060,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"192.168.242.38",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"192.168.242.38",18015,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,1,"192.168.242.38",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"192.168.242.38",18059,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"192.168.242.39",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"192.168.242.39",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"192.168.242.39",18013,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,11,"192.168.242.15",18011,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,66,"192.168.242.37",18066,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,67,"192.168.242.38",18067,"0.0.0.0",0));
		

		mgr.Save("serverlist_DEVS.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		}
		{
		//Adult 1
		CServerListManager mgr;
		mgr.SetServerSetNum(2);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"192.168.242.65",20001,"222.234.222.65",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"192.168.242.65",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"192.168.242.66",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"192.168.242.67",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"192.168.242.68",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"192.168.242.69",19005,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,5,"192.168.242.70",19006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,6,"192.168.242.71",19007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,7,"192.168.242.72",19008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,8,"192.168.242.73",19009,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,9,"192.168.242.74",19010,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"192.168.242.65",16001,"222.234.222.65",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"192.168.242.66",17001,"222.234.222.66",14600));
		mgr.Add(&SERVERINFO(AGENT_SERVER,1,"192.168.242.67",17002,"222.234.222.67",14601));
		mgr.Add(&SERVERINFO(AGENT_SERVER,2,"192.168.242.68",17003,"222.234.222.68",14602));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"192.168.242.69",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"192.168.242.69",18008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"192.168.242.69",18035,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,65,"192.168.242.70",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"192.168.242.70",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"192.168.242.70",18002,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,20,"192.168.242.71",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"192.168.242.71",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,19,"192.168.242.72",18019,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"192.168.242.72",18060,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"192.168.242.73",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"192.168.242.73",18015,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,1,"192.168.242.73",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"192.168.242.73",18059,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"192.168.242.74",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"192.168.242.74",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"192.168.242.74",18013,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,11,"192.168.242.71",18011,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,66,"192.168.242.72",18066,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,67,"192.168.242.73",18067,"0.0.0.0",0));
		

		mgr.Save("serverlist_02.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		}

		
		//Adult02
		{
		CServerListManager mgr;
		mgr.SetServerSetNum(3);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"192.168.242.25",20001,"222.234.222.25",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"192.168.242.25",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"192.168.242.26",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"192.168.242.49",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"192.168.242.50",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"192.168.242.51",19005,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,5,"192.168.242.52",19006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,6,"192.168.242.53",19007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,7,"192.168.242.54",19008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,8,"192.168.242.55",19009,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,9,"192.168.242.56",19010,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"192.168.242.25",16001,"222.234.222.25",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"192.168.242.26",17001,"222.234.222.26",14600));
		mgr.Add(&SERVERINFO(AGENT_SERVER,1,"192.168.242.49",17002,"222.234.222.49",14601));
		mgr.Add(&SERVERINFO(AGENT_SERVER,2,"192.168.242.50",17003,"222.234.222.50",14602));
		
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"192.168.242.51",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"192.168.242.51",18008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"192.168.242.51",18035,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,65,"192.168.242.52",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"192.168.242.52",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"192.168.242.52",18002,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,20,"192.168.242.53",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"192.168.242.53",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,19,"192.168.242.54",18019,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"192.168.242.54",18060,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"192.168.242.55",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"192.168.242.55",18015,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,1,"192.168.242.55",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"192.168.242.55",18059,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"192.168.242.56",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"192.168.242.56",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"192.168.242.56",18013,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,11,"192.168.242.53",18011,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,66,"192.168.242.54",18066,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,67,"192.168.242.55",18067,"0.0.0.0",0));


		mgr.Save("serverlist_03.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		}


		//Teen01
		{
		CServerListManager mgr;
		mgr.SetServerSetNum(4);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"192.168.242.5",20001,"222.234.222.5",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"192.168.242.5",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"192.168.242.19",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"192.168.242.21",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"192.168.242.22",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"192.168.242.16",19005,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,5,"192.168.242.17",19006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,6,"192.168.242.45",19007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,7,"192.168.242.46",19008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,8,"192.168.242.62",19009,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,9,"192.168.242.64",19010,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"192.168.242.5",16001,"222.234.222.5",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"192.168.242.19",17001,"222.234.222.19",14600));
		mgr.Add(&SERVERINFO(AGENT_SERVER,1,"192.168.242.21",17002,"222.234.222.21",14601));
		mgr.Add(&SERVERINFO(AGENT_SERVER,2,"192.168.242.22",17003,"222.234.222.22",14602));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"192.168.242.16",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"192.168.242.16",18008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"192.168.242.16",18035,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,65,"192.168.242.17",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"192.168.242.17",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"192.168.242.17",18002,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,20,"192.168.242.45",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"192.168.242.45",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,19,"192.168.242.46",18019,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"192.168.242.46",18060,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"192.168.242.62",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"192.168.242.62",18015,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,1,"192.168.242.62",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"192.168.242.62",18059,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"192.168.242.64",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"192.168.242.64",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"192.168.242.64",18013,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,11,"192.168.242.45",18011,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,66,"192.168.242.46",18066,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,67,"192.168.242.62",18067,"0.0.0.0",0));
		

		mgr.Save("serverlist_04.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		}


		//  CSC1
		CServerListManager mgr;
		mgr.SetServerSetNum(1);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"61.152.93.30",20001,"61.152.93.30",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"61.152.93.30",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"172.20.2.11",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"172.20.2.12",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"172.20.2.13",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"172.20.2.14",19005,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"61.152.93.30",16001,"61.152.93.30",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"61.152.93.30",17001,"61.152.93.30",14600));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"172.20.2.11",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,65,"172.20.2.11",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"172.20.2.11",18060,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.11",18006,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"172.20.2.12",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"172.20.2.12",18059,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,20,"172.20.2.12",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,19,"172.20.2.12",18019,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,1,"172.20.2.13",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"172.20.2.13",18015,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"172.20.2.13",18035,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"172.20.2.13",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.14",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"172.20.2.14",18002,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.14",18013,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"172.20.2.14",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"172.20.2.14",18008,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,11,"172.20.2.12",18011,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,66,"172.20.2.13",18066,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,67,"172.20.2.14",18067,"0.0.0.0",0));

		mgr.Save("serverlist_csc1.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		
		
		//CSC2
		{
		CServerListManager mgr;
		mgr.SetServerSetNum(2);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"61.152.93.18",20001,"61.152.93.18",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"61.152.93.18",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"61.152.93.19",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"61.152.93.20",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"61.152.93.21",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"172.20.2.21",19005,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,5,"172.20.2.22",19006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,6,"172.20.2.23",19007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,7,"172.20.2.24",19008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,8,"172.20.2.25",19009,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,9,"172.20.2.26",19010,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"61.152.93.18",16001,"61.152.93.18",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"61.152.93.19",17001,"61.152.93.19",14600));
		mgr.Add(&SERVERINFO(AGENT_SERVER,1,"61.152.93.20",17002,"61.152.93.20",14601));
		mgr.Add(&SERVERINFO(AGENT_SERVER,2,"61.152.93.21",17003,"61.152.93.21",14602));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"172.20.2.21",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"172.20.2.21",18008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"172.20.2.21",18035,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,65,"172.20.2.22",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"172.20.2.22",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"172.20.2.22",18002,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,20,"172.20.2.23",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"172.20.2.23",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,19,"172.20.2.24",18019,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"172.20.2.24",18060,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"172.20.2.25",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"172.20.2.25",18015,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,1,"172.20.2.25",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"172.20.2.25",18059,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.26",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.26",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.26",18013,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.26",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.26",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.26",18013,"0.0.0.0",0));
		

		mgr.Save("serverlist_csc2.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		}
		

		//CSC3
		{
		CServerListManager mgr;
		mgr.SetServerSetNum(3);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"172.20.11.20",20001,"221.208.172.66",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"172.20.11.20",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"172.20.11.21",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"172.20.11.22",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"172.20.11.11",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"172.20.11.12",19005,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,5,"172.20.11.13",19006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,6,"172.20.11.14",19007,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"172.20.11.20",16001,"221.208.172.66",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"172.20.11.21",17001,"221.208.172.67",14600));
		mgr.Add(&SERVERINFO(AGENT_SERVER,1,"172.20.11.22",17002,"221.208.172.68",14601));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"172.20.2.11",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,65,"172.20.2.11",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"172.20.2.11",18060,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.11",18006,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"172.20.2.12",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"172.20.2.12",18059,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,20,"172.20.2.12",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,19,"172.20.2.12",18019,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,1,"172.20.2.13",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"172.20.2.13",18015,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"172.20.2.13",18035,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"172.20.2.13",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.14",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"172.20.2.14",18002,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.14",18013,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"172.20.2.14",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"172.20.2.14",18008,"0.0.0.0",0));


		mgr.Save("serverlist_csc3.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		}

		//CSC4
		{
		CServerListManager mgr;
		mgr.SetServerSetNum(4);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"61.145.125.146",20001,"61.145.125.146",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"61.145.125.146",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"61.145.125.147",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"61.145.125.148",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"61.145.125.149",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"172.20.2.11",19005,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,5,"172.20.2.12",19006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,6,"172.20.2.13",19007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,7,"172.20.2.14",19008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,8,"172.20.2.15",19009,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,9,"172.20.2.16",19010,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"61.145.125.146",16001,"61.145.125.146",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"61.145.125.147",17001,"61.145.125.147",14600));
		mgr.Add(&SERVERINFO(AGENT_SERVER,1,"61.145.125.148",17002,"61.145.125.148",14601));
		mgr.Add(&SERVERINFO(AGENT_SERVER,2,"61.145.125.149",17003,"61.145.125.149",14602));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"172.20.2.11",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"172.20.2.11",18008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"172.20.2.11",18035,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,65,"172.20.2.12",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"172.20.2.12",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"172.20.2.12",18002,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,20,"172.20.2.13",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"172.20.2.13",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,19,"172.20.2.14",18019,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"172.20.2.14",18060,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"172.20.2.15",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"172.20.2.15",18015,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,1,"172.20.2.15",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"172.20.2.15",18059,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.16",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.16",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.16",18013,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.16",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.16",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.16",18013,"0.0.0.0",0));
		

		mgr.Save("serverlist_csc4.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		}

		//CSC5
		{
		CServerListManager mgr;
		mgr.SetServerSetNum(5);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"61.152.114.2",20001,"61.152.114.2",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"61.152.114.2",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"61.152.114.3",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"61.152.114.4",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"61.152.114.5",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"172.20.2.11",19005,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,5,"172.20.2.12",19006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,6,"172.20.2.13",19007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,7,"172.20.2.14",19008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,8,"172.20.2.15",19009,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,9,"172.20.2.16",19010,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"61.152.114.2",16001,"61.152.114.2",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"61.152.114.3",17001,"61.152.114.3",14600));
		mgr.Add(&SERVERINFO(AGENT_SERVER,1,"61.152.114.4",17002,"61.152.114.4",14601));
		mgr.Add(&SERVERINFO(AGENT_SERVER,2,"61.152.114.5",17003,"61.152.114.5",14602));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"172.20.2.11",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"172.20.2.11",18008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"172.20.2.11",18035,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,65,"172.20.2.12",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"172.20.2.12",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"172.20.2.12",18002,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,20,"172.20.2.13",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"172.20.2.13",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,19,"172.20.2.14",18019,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"172.20.2.14",18060,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"172.20.2.15",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"172.20.2.15",18015,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,1,"172.20.2.15",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"172.20.2.15",18059,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.16",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.16",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.16",18013,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.16",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.16",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.16",18013,"0.0.0.0",0));
		

		mgr.Save("serverlist_csc5.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		}

		//CSC6
		{
		CServerListManager mgr;
		mgr.SetServerSetNum(6);

		mgr.Add(&SERVERINFO(MONITOR_AGENT_SERVER,0,"218.6.171.2",20001,"218.6.171.2",20034));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,0,"218.6.171.2",19001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,1,"218.6.171.3",19002,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,2,"218.6.171.4",19003,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,3,"218.6.171.5",19004,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,4,"172.20.2.11",19005,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,5,"172.20.2.12",19006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,6,"172.20.2.13",19007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,7,"172.20.2.14",19008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,8,"172.20.2.15",19009,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MONITOR_SERVER,9,"172.20.2.16",19010,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(DISTRIBUTE_SERVER,0,"218.6.171.2",16001,"218.6.171.2",14400));
		mgr.Add(&SERVERINFO(AGENT_SERVER,0,"218.6.171.3",17001,"218.6.171.3",14600));
		mgr.Add(&SERVERINFO(AGENT_SERVER,1,"218.6.171.4",17002,"218.6.171.4",14601));
		mgr.Add(&SERVERINFO(AGENT_SERVER,2,"218.6.171.5",17003,"218.6.171.5",14602));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,18,"172.20.2.11",18018,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,8,"172.20.2.11",18008,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,35,"172.20.2.11",18035,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,65,"172.20.2.12",18065,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,12,"172.20.2.12",18012,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,2,"172.20.2.12",18002,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,20,"172.20.2.13",18020,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,40,"172.20.2.13",18040,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,19,"172.20.2.14",18019,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,60,"172.20.2.14",18060,"0.0.0.0",0));

		mgr.Add(&SERVERINFO(MAP_SERVER,17,"172.20.2.15",18017,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,15,"172.20.2.15",18015,"0.0.0.0",0));		
		mgr.Add(&SERVERINFO(MAP_SERVER,1,"172.20.2.15",18001,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,59,"172.20.2.15",18059,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.16",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.16",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.16",18013,"0.0.0.0",0));
		
		mgr.Add(&SERVERINFO(MAP_SERVER,6,"172.20.2.16",18006,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,7,"172.20.2.16",18007,"0.0.0.0",0));
		mgr.Add(&SERVERINFO(MAP_SERVER,13,"172.20.2.16",18013,"0.0.0.0",0));
		

		mgr.Save("serverlist_csc6.msl");
		MessageBox(NULL,"ServerList Successfully Saved",NULL,NULL);
		}
		
		return 0;
	}
//	*/	

	DeleteFile( "MS_Debug.txt" );
	
	if(atoi(lpCmdLine) == 0)
	{
		LOG( "lpCmdLine == 0" );
		return 0;
	}

	char filename[256];
	FILE* fp = fopen("serverset.txt","r");
	if(fp == NULL)
	{
		LOG( "serverset.txt is not exist" );
		MessageBox(NULL,"serverset.txt is not exist. this server will be closed",0,0);
		exit(0);
		return 0;
	}
	int serversetnum;
	fscanf(fp,"%d",&serversetnum);
	fclose(fp);

	LOG( "Load serverset/%d/serverlist.msl", serversetnum );

	sprintf(filename,"serverset/%d/serverlist.msl",serversetnum);
	SERVERLISTMGR->Load(filename);

	LOG( "End Load serverset/%d/serverlist.msl", serversetnum );

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MONITORINGSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MONITORINGSERVER);
	
	g_pMSSystem = new CMSSystem;
	g_pMSSystem->Start(atoi(lpCmdLine));
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	g_pMSSystem->End();
	delete g_pMSSystem;
	ReleaseConsole();

	return msg.wParam;
}

void InitConsole()
{
	g_bConsole = AllocConsole();
	if(g_bConsole)
	{
		g_hOutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	
}
void ReleaseConsole()
{
	if(g_bConsole)
		FreeConsole();

}
void LogConsole(char * buff,...)
{
	if(g_bConsole)
	{
		static char msg2[255];
		va_list argList;
		va_start(argList, buff);
		vsprintf(msg2, buff, argList);
		va_end(argList);

		DWORD dw;
		WriteFile(g_hOutConsole, msg2,strlen(msg2),&dw,NULL);
		WriteFile(g_hOutConsole, "\n",strlen("\n"),&dw,NULL);
	}
	
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MONITORINGSERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_MONITORINGSERVER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   gMainHwnd = hWnd;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			if( g_pMSSystem )
			{
				if( g_pMSSystem->m_wServerType == 1 )
				{
					sprintf( szHello, "Monitoring Agent Server" );
				}
				else if( g_pMSSystem->m_wServerType == 2 )
				{
					sprintf( szHello, "Monitoring Server" );
				}
				else if( g_pMSSystem->m_wServerType == 3 )
				{
					sprintf( szHello, "RMTool Server" );
				}				
			}	
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			else if(LOWORD(wParam) == IDC_BUTTON2)
			{
				InitConsole();
				return TRUE;
			}
			break;
	}
    return FALSE;
}
