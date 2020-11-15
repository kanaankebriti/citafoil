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
	GetClientRect(rectClient);

	// clear background
	this->DoPaint(this->GetDC());

	// set airfoil selection combobox
	combobox_airfoils.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), combobox_airfoils_height, SWP_NOACTIVATE | SWP_NOZORDER);
}

VOID CPropertiesWnd::SetTarget(CWnd* m_cwnd)
{
	message_target = m_cwnd;
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create airfoil selection combobox
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!combobox_airfoils.Create(dwViewStyle, rectDummy, this, IDR_COMBOBOX_AIRFOILS))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;
	}

	combobox_airfoils.AddString(_T("NACA-0006"));
	combobox_airfoils.AddString(_T("NACA-0008"));
	combobox_airfoils.AddString(_T("NACA-0010"));
	combobox_airfoils.AddString(_T("NACA-2414"));
	combobox_airfoils.SetCurSel(0);

	CRect rectCombo;
	combobox_airfoils.GetClientRect(&rectCombo);
	combobox_airfoils_height = rectCombo.Height();

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

	combobox_airfoils.SetFont(&m_fntPropList);
}