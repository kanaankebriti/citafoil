/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
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

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "citafoil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CPropertiesWnd::CPropertiesWnd() noexcept
{
	m_nComboHeight = 0;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	Invalidate();
}

void CPropertiesWnd::OnPaint()
{
	// get CPane rect
	CRect	rectClient;
	GetClientRect(&rectClient);
	
	// set GUI elements' position
	btn_input_groupbox.SetWindowPos(nullptr, GROUPBOX_PADDING_LR, 0, rectClient.Width() - 2 * GROUPBOX_PADDING_LR, (UINT)(1.75 * (combobox_airfoils_height + GROUPBOX_PADDING_TOP)), SWP_SHOWWINDOW);
		lbl_airfoils.SetWindowPos(nullptr, 2 * GROUPBOX_PADDING_LR, GROUPBOX_PADDING_TOP, lbl_airfoils_width, combobox_airfoils_height, SWP_SHOWWINDOW);
		combobox_airfoils.SetWindowPos(nullptr, 2 * GROUPBOX_PADDING_LR + lbl_airfoils_width, GROUPBOX_PADDING_TOP, rectClient.Width() - lbl_airfoils_width - 4 * GROUPBOX_PADDING_LR, combobox_airfoils_height, SWP_SHOWWINDOW);

		lbl_interpolation_level.SetWindowPos(nullptr, 2 * GROUPBOX_PADDING_LR, combobox_airfoils_height + GROUPBOX_PADDING_TOP, lbl_interpolation_level_width, edit_interpolation_level_height, SWP_SHOWWINDOW);
		edit_interpolation_level.SetWindowPos(nullptr, lbl_interpolation_level_width + 2 * GROUPBOX_PADDING_LR, combobox_airfoils_height + GROUPBOX_PADDING_TOP, rectClient.Width() - lbl_interpolation_level_width - 4 * GROUPBOX_PADDING_LR - 30, edit_interpolation_level_height, SWP_SHOWWINDOW);
		spinbtn_interpolation_level.SetWindowPos(nullptr, rectClient.right - 30 - 2 * GROUPBOX_PADDING_LR, combobox_airfoils_height + GROUPBOX_PADDING_TOP, 30, edit_interpolation_level_height, SWP_SHOWWINDOW);

	btn_result_groupbox.SetWindowPos(nullptr, GROUPBOX_PADDING_LR, (UINT)(1.75 * (combobox_airfoils_height + GROUPBOX_PADDING_TOP)), rectClient.Width() - 2 * GROUPBOX_PADDING_LR, (UINT)(1.75 * (combobox_airfoils_height + GROUPBOX_PADDING_TOP)), SWP_SHOWWINDOW);
		lbl_alpha_lift_zero.SetWindowPos(nullptr, 2 * GROUPBOX_PADDING_LR, (UINT)(1.75 * (combobox_airfoils_height + GROUPBOX_PADDING_TOP)) + GROUPBOX_PADDING_TOP, lbl_alpha_lift_zero_width, combobox_airfoils_height, SWP_SHOWWINDOW);
		lbl_alpha_lift_zero_result.SetWindowPos(nullptr, 2 * GROUPBOX_PADDING_LR + lbl_alpha_lift_zero_width, (UINT)(1.75 * (combobox_airfoils_height + GROUPBOX_PADDING_TOP)) + GROUPBOX_PADDING_TOP, rectClient.Width() - lbl_alpha_lift_zero_width - 4 * GROUPBOX_PADDING_LR, combobox_airfoils_height, SWP_SHOWWINDOW);

		btn_save_cl_alpha_svg.SetWindowPos(nullptr, 2 * GROUPBOX_PADDING_LR, (UINT)(1.75 * (combobox_airfoils_height + GROUPBOX_PADDING_TOP)) + GROUPBOX_PADDING_TOP + combobox_airfoils_height, rectClient.Width() - 4 * GROUPBOX_PADDING_LR, combobox_airfoils_height, SWP_SHOWWINDOW);

	lbl_background_canvas.SetWindowPos(nullptr, 0, 0, rectClient.Width(), rectClient.Height(), SWP_SHOWWINDOW);
}

BOOL CPropertiesWnd::OnEraseBkgnd(CDC* pDC)
{
	CWnd::OnEraseBkgnd(pDC);
	CPaintDC dc(this); // i don't know why. but it should be here!
	return TRUE;
}

VOID CPropertiesWnd::SetTarget(CWnd* m_cwnd)
{
	message_target = m_cwnd;
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString retrieved_str;			// retrieved literal string
	BOOL	retrieved_str_valid;	// retrieved literal string validity check
	CRect	test_rect, rectDummy;	// retrieve GUI elements dimensions

	rectDummy.SetRectEmpty();

	DWORD dwViewStyle; // Specifies the static control's window style

	// background canvas
	dwViewStyle = WS_CHILD | WS_VISIBLE;
	if (!lbl_background_canvas.Create(NULL, dwViewStyle, rectDummy, this, IDR_LBL_BACKGROUND_CANVAS))
	{
		TRACE0("Failed to create master container\n");
		return -1;
	}

	// input group box
	retrieved_str_valid = retrieved_str.LoadString(IDS_BTN_INPUT_GROUPBOX_CAPTION);
	ASSERT(retrieved_str_valid);

	dwViewStyle = WS_CHILD | WS_VISIBLE | BS_LEFT | BS_GROUPBOX;
	if (!btn_input_groupbox.Create(retrieved_str, dwViewStyle, rectDummy, this, IDR_BTN_INPUT_GROUPBOX))
	{
		TRACE0("Failed to create input group box\n");
		return -1;
	}

	// airfoil label
	retrieved_str_valid = retrieved_str.LoadString(IDS_LBL_AIRFOILS);
	ASSERT(retrieved_str_valid);

	CPaintDC dc(this);
	dwViewStyle = WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE;
	if (!lbl_airfoils.Create(retrieved_str, dwViewStyle, rectDummy, this, IDR_LBL_AIRFOILS))
	{
		TRACE0("Failed to create airfoil Label\n");
		return -1;
	}
	lbl_airfoils_width = (dc.GetTextExtent(L"airfoil :")).cx;

	// airfoil selection combobox
	dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST;

	if (!combobox_airfoils.Create(dwViewStyle, rectDummy, this, IDR_COMBOBOX_AIRFOILS))
	{
		TRACE0("Failed to create Properties Combo\n");
		return -1;
	}

	combobox_airfoils.AddString(_T("NACA 0006"));
	combobox_airfoils.AddString(_T("NACA 0008"));
	combobox_airfoils.AddString(_T("NACA 0010"));
	combobox_airfoils.AddString(_T("NACA 2412"));
	combobox_airfoils.AddString(_T("NACA 2414"));
	combobox_airfoils.AddString(_T("NACA 4412"));
	combobox_airfoils.AddString(_T("NACA 4415"));
	combobox_airfoils.AddString(_T("NACA 6409"));
	combobox_airfoils.AddString(_T("NACA 23012"));
	combobox_airfoils.AddString(_T("NACA 23112"));
	combobox_airfoils.AddString(_T("NACA 25112"));
	combobox_airfoils.AddString(_T("NACA 63(2)-615"));
	combobox_airfoils.AddString(_T("NACA 65(2)-215"));
	combobox_airfoils.AddString(_T("NACA M6"));
	//combobox_airfoils.AddString(_T("Selig S1223"));
	combobox_airfoils.AddString(_T("Selig S3024"));
	combobox_airfoils.AddString(_T("Selig S9000"));
	combobox_airfoils.SetCurSel(0);

	combobox_airfoils.GetClientRect(&test_rect);
	combobox_airfoils_height = test_rect.Height();

	// interpolation label
	retrieved_str_valid = retrieved_str.LoadString(IDS_LBL_INTERPOLATION_LEVEL);
	ASSERT(retrieved_str_valid);

	dwViewStyle = WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE;
	if (!lbl_interpolation_level.Create(retrieved_str, dwViewStyle, rectDummy, this, IDR_LBL_INTERPOLATION_LEVEL))
	{
		TRACE0("Failed to create interpolation level Label\n");
		return -1;
	}
	lbl_interpolation_level_width = (dc.GetTextExtent(retrieved_str)).cx;

	// interpolation level edit/buddy
	dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY;
	rectDummy.SetRectEmpty();

	if (!edit_interpolation_level.Create(dwViewStyle, rectDummy, this, IDR_EDIT_INTERPOLATION_LEVEL))
	{
		TRACE0("Failed to create Interpolation Level Spinbutton\n");
		return -1;
	}

	edit_interpolation_level.GetRect(&test_rect);
	edit_interpolation_level_height = test_rect.Height();
	edit_interpolation_level_height += 4; // for showing selected text properly

	// interpolation level spin button
	dwViewStyle = WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_SETBUDDYINT;
	rectDummy.SetRectEmpty();

	if (!spinbtn_interpolation_level.Create(dwViewStyle, rectDummy, this, IDR_SPINBTN_INTERPOLATION_LEVEL))
	{
		TRACE0("Failed to create Interpolation Level Spinbutton\n");
		return -1;
	}
	
	spinbtn_interpolation_level.SetBuddy(&edit_interpolation_level); // attach spin to edit box
	spinbtn_interpolation_level.SetRange(MIN_INTERPOLATION_LEVEL, MAX_INTERPOLATION_LEVEL);
	spinbtn_interpolation_level.SetPos(0); // default interpolation level


	// result group box
	retrieved_str_valid = retrieved_str.LoadString(IDS_BTN_RESULT_GROUPBOX_CAPTION);
	ASSERT(retrieved_str_valid);

	dwViewStyle = WS_CHILD | WS_VISIBLE | BS_LEFT | BS_GROUPBOX;
	if (!btn_result_groupbox.Create(retrieved_str, dwViewStyle, rectDummy, this, IDR_BTN_INPUT_GROUPBOX))
	{
		TRACE0("Failed to create input group box\n");
		return -1;
	}

	// alpha lift zero label
	retrieved_str_valid = retrieved_str.LoadString(IDS_LBL_ALPHA_LIFT_ZERO);
	ASSERT(retrieved_str_valid);
	
	dwViewStyle = WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE;
	if (!lbl_alpha_lift_zero.Create(retrieved_str, dwViewStyle, rectDummy, this, IDR_LBL_INTERPOLATION_LEVEL))
	{
		TRACE0("Failed to create interpolation alpha lift zero Label\n");
		return -1;
	}
	lbl_alpha_lift_zero_width = (dc.GetTextExtent(retrieved_str)).cx;

	// alpha lift zero result label
	dwViewStyle = WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE;
	if (!lbl_alpha_lift_zero_result.Create(NULL, dwViewStyle, rectDummy, this, IDR_LBL_INTERPOLATION_LEVEL))
	{
		TRACE0("Failed to create interpolation alpha lift zero result Label\n");
		return -1;
	}

	// save cl-alpha to svg button
	retrieved_str_valid = retrieved_str.LoadString(IDS_BTN_SAVE_CL_ALPHA_SVG_CAPTION);
	ASSERT(retrieved_str_valid);
	
	dwViewStyle = WS_CHILD | WS_VISIBLE | BS_CENTER;
	if (!btn_save_cl_alpha_svg.Create(retrieved_str, dwViewStyle, rectDummy, this, IDR_BTN_SAVE_CL_ALPHA_SVG))
	{
		TRACE0("Failed to create save cl-alpha to svg button\n");
		return -1;
	}

	GetDlgItem(IDR_BTN_SAVE_CL_ALPHA_SVG)->EnableWindow(TRUE);

	SetupFont();
	return 0;
}

BOOL CPropertiesWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// redirect all messages to directx window
	if (message_target)
	{
		message_target->SendMessage(WM_COMMAND, wParam, lParam);
		return TRUE;
	}
	else
		return FALSE;
}

VOID CPropertiesWnd::SetupFont()
{
	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);
	_tcscpy_s(lf.lfFaceName, _T("Arial"));
	::DeleteObject(m_fntPropList.Detach());

	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	btn_input_groupbox.SetFont(&m_fntPropList);
	lbl_airfoils.SetFont(&m_fntPropList);
	lbl_interpolation_level.SetFont(&m_fntPropList);
	combobox_airfoils.SetFont(&m_fntPropList);
	edit_interpolation_level.SetFont(&m_fntPropList);
	btn_result_groupbox.SetFont(&m_fntPropList);
	lbl_alpha_lift_zero.SetFont(&m_fntPropList);
	lbl_alpha_lift_zero_result.SetFont(&m_fntPropList);
	btn_save_cl_alpha_svg.SetFont(&m_fntPropList);
}
