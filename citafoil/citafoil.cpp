﻿/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
░ This file is part of citafoil.										░
░																		░
░ citafoil is free software: you can redistribute it and/or modify		░
░ it under the terms of the GNU General Public License as published by	░
░ the Free Software Foundation, either version 3 of the License, or		░
░ (at your option) any later version.									░
░																		░
░ citafoil is distributed in the hope that it will be useful,			░
░ but WITHOUT ANY WARRANTY; without even the implied warranty of		░
░ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the			░
░ GNU General Public License for more details.							░
░																		░
░ You should have received a copy of the GNU General Public License		░
░ along with citafoil.  If not, see <https://www.gnu.org/licenses/>.	░
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/
#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "citafoil.h"
#include "MainFrm.h"

#include "citafoilDoc.h"
#include "citafoilView.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define new DEBUG_NEW
#endif


// CcitafoilApp

BEGIN_MESSAGE_MAP(CcitafoilApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CcitafoilApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()

CcitafoilApp::CcitafoilApp() noexcept
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("citafoil.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CcitafoilApp object

CcitafoilApp theApp;


// CcitafoilApp initialization

BOOL CcitafoilApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	CMainFrame main_frame;
	CcitafoilView main_view;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CcitafoilDoc),
		main_frame.GetRuntimeClass(),       // main SDI frame window
		main_view.GetRuntimeClass());
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// for redirecting messages from (current document)->(m_wndProperties child) to directx output
	main_frame.m_wndProperties.SetTarget(main_view.GetWindow(GW_CHILD));
	
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	afx_msg void OnEnSetfocusLiecence();
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_EN_SETFOCUS(IDC_LIECENCE, &CAboutDlg::OnEnSetfocusLiecence)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// App command to run the dialog
void CcitafoilApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CAboutDlg::OnEnSetfocusLiecence()
{
	SetDlgItemText(IDC_LIECENCE,
		L"citafoil is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version."
		L"\r\n\r\n"
		L"citafoil is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.citafoil is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.citafoil is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details."
		L"\r\n\r\n"
		L"You should have received a copy of the GNU General Public License along with citafoil.If not, see <https://www.gnu.org/licenses/>.");
}
