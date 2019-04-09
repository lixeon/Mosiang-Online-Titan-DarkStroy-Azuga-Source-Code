; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPackingToolDlg
LastTemplate=CRichEditCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PackingTool.h"

ClassCount=7
Class1=CPackingToolApp
Class2=CPackingToolDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_DIALOG_EXT
Resource2=IDR_MAINFRAME
Resource3=IDD_PACKINGTOOL_DIALOG
Class4=CExtDlg
Resource4=IDD_DIALOG_NEW
Class5=CNewDlg
Class6=CMyEdit
Class7=CMyRichEdit
Resource5=IDD_ABOUTBOX
Resource6=IDR_ACCELERATOR1

[CLS:CPackingToolApp]
Type=0
HeaderFile=PackingTool.h
ImplementationFile=PackingTool.cpp
Filter=N
LastObject=CPackingToolApp

[CLS:CPackingToolDlg]
Type=0
HeaderFile=PackingToolDlg.h
ImplementationFile=PackingToolDlg.cpp
Filter=W
LastObject=ID_SEARCH
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=PackingToolDlg.h
ImplementationFile=PackingToolDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PACKINGTOOL_DIALOG]
Type=1
Class=CPackingToolDlg
ControlCount=18
Control1=IDC_EDIT,edit,1353715908
Control2=IDC_BUTTON_OPEN,button,1342177280
Control3=IDC_BUTTON_OPEN_BIN,button,1342177280
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BUTTON_SAVE_TO_BIN,button,1342177280
Control6=IDC_BUTTON_NEW,button,1342177280
Control7=IDC_BUTTON_SAVE_TO_TXT,button,1342177280
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC_CURFILE,static,1342308352
Control10=IDC_LIST_FILE,listbox,1353712897
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC_FILENUM,static,1342308354
Control13=IDC_BUTTON_DELALL,button,1342177280
Control14=IDC_STATIC,button,1342177287
Control15=IDCANCEL,button,1342177280
Control16=IDC_BUTTON_SAVE_AS_BIN,button,1342242816
Control17=IDC_STATIC,button,1342177287
Control18=IDC_BUTTON_SAVE_BIN,button,1342242816

[DLG:IDD_DIALOG_EXT]
Type=1
Class=CExtDlg
ControlCount=5
Control1=IDC_EDIT_EXT,edit,1350631552
Control2=IDC_STATIC,static,1342308353
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342242816
Control5=IDCANCEL,button,1342242816

[CLS:CExtDlg]
Type=0
HeaderFile=ExtDlg.h
ImplementationFile=ExtDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CExtDlg

[DLG:IDD_DIALOG_NEW]
Type=1
Class=CNewDlg
ControlCount=4
Control1=ID_SEARCH,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308353
Control4=IDC_EDIT_FILE,edit,1350635648

[CLS:CNewDlg]
Type=0
HeaderFile=NewDlg.h
ImplementationFile=NewDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CNewDlg
VirtualFilter=dWC

[CLS:CMyEdit]
Type=0
HeaderFile=MyEdit.h
ImplementationFile=MyEdit.cpp
BaseClass=CEdit
Filter=W
LastObject=ID_FILE_SEARCH
VirtualFilter=WC

[CLS:CMyRichEdit]
Type=0
HeaderFile=MyRichEdit.h
ImplementationFile=MyRichEdit.cpp
BaseClass=CRichEditCtrl
Filter=W
LastObject=CMyRichEdit

[ACL:IDR_ACCELERATOR1]
Type=1
Class=CPackingToolDlg
Command1=ID_SEARCH
CommandCount=1

