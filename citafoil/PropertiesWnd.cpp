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
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	this->GetClientRect(rectClient);

	// clear background
	this->DoPaint(this->GetDC());

	// set GUI elements' position
	lbl_airfoils.SetWindowPos				(nullptr, 0,								0,												lbl_airfoils_width,										combobox_airfoils_height,			SWP_NOACTIVATE | SWP_NOZORDER);
	combobox_airfoils.SetWindowPos			(nullptr, lbl_airfoils_width,				0,												rectClient.Width() - lbl_airfoils_width,				combobox_airfoils_height,			SWP_NOACTIVATE | SWP_NOZORDER);
	lbl_interpolation_level.SetWindowPos	(nullptr, 0,								combobox_airfoils_height + FREE_SPACE_HEIGHT,	lbl_interpolation_level_width,							edit_interpolation_level_height,	SWP_NOACTIVATE | SWP_NOZORDER);
	edit_interpolation_level.SetWindowPos	(nullptr, lbl_interpolation_level_width,	combobox_airfoils_height + FREE_SPACE_HEIGHT,	rectClient.Width() - lbl_interpolation_level_width - 30,edit_interpolation_level_height,	SWP_NOACTIVATE | SWP_NOZORDER);
	spinbtn_interpolation_level.SetWindowPos(nullptr, rectClient.right - 30,			combobox_airfoils_height + FREE_SPACE_HEIGHT,	30,														edit_interpolation_level_height,	SWP_NOACTIVATE | SWP_NOZORDER);
}

VOID CPropertiesWnd::SetTarget(CWnd* m_cwnd)
{
	message_target = m_cwnd;
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect test_rect, rectDummy;
	rectDummy.SetRectEmpty();

	DWORD dwViewStyle; // Specifies the static control's window style

	// airfoil label
	dwViewStyle = WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE;
	if (!lbl_airfoils.Create(L"airfoil:", dwViewStyle, rectDummy, this, IDR_AIRFOILS_LBL))
	{
		TRACE0("Failed to create airfoil Label \n");
		return -1;
	}
	lbl_airfoils_width = (lbl_airfoils.GetDC()->GetTextExtent(L"airfoil:")).cx;

	// airfoil selection combobox
	dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBS_DROPDOWNLIST;

	if (!combobox_airfoils.Create(dwViewStyle, rectDummy, this, IDR_COMBOBOX_AIRFOILS))
	{
		TRACE0("Failed to create Properties Combo \n");
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
	dwViewStyle = WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE;
	if (!lbl_interpolation_level.Create(L"interpolation level:", dwViewStyle, rectDummy, this, IDR_AIRFOILS_LBL))
	{
		TRACE0("Failed to create interpolation level Label \n");
		return -1;
	}
	lbl_interpolation_level_width = (lbl_airfoils.GetDC()->GetTextExtent(L"interpolation level:")).cx;

	// interpolation level edit/buddy
	dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_READONLY;
	rectDummy.SetRectEmpty();

	if (!edit_interpolation_level.Create(dwViewStyle, rectDummy, this, IDR_EDIT_INTERPOLATION_LEVEL))
	{
		TRACE0("Failed to create Interpolation Level Spinbutton \n");
		return -1;
	}

	edit_interpolation_level.GetRect(&test_rect);
	edit_interpolation_level_height = test_rect.Height();
	edit_interpolation_level_height += 4;	// for showing selected text properly

	// interpolation level spin button
	dwViewStyle = WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_SETBUDDYINT;
	rectDummy.SetRectEmpty();

	if (!spinbtn_interpolation_level.Create(dwViewStyle, rectDummy, this, IDR_SPINBTN_INTERPOLATION_LEVEL))
	{
		TRACE0("Failed to create Interpolation Level Spinbutton \n");
		return -1;
	}
	
	spinbtn_interpolation_level.SetBuddy(&edit_interpolation_level);	// attach spin to edit box
	spinbtn_interpolation_level.SetRange(MIN_INTERPOLATION_LEVEL, MAX_INTERPOLATION_LEVEL);
	spinbtn_interpolation_level.SetPos(0);	// default interpolation level

	SetupFont();
	AdjustLayout();
	return 0;
}

BOOL CPropertiesWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// redirect messages to directx output
	if (message_target)
	{
		message_target->SendMessage(WM_COMMAND, wParam, lParam);
		return TRUE;
	}
	else
		return FALSE;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
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

	lbl_airfoils.SetFont(&m_fntPropList);
	lbl_interpolation_level.SetFont(&m_fntPropList);
	combobox_airfoils.SetFont(&m_fntPropList);
	edit_interpolation_level.SetFont(&m_fntPropList);
}
