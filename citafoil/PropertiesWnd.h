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
#pragma once

#define MIN_INTERPOLATION_LEVEL 0
#define MAX_INTERPOLATION_LEVEL 5
#define GROUPBOX_PADDING_LR		5
#define GROUPBOX_PADDING_TOP	22

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
public:
	CPropertiesWnd() noexcept;
	virtual ~CPropertiesWnd();
	VOID SetTarget(CWnd* m_cwnd);
	// GUI elements access from directx window
	CComboBox combobox_airfoils;
	CEdit edit_interpolation_level;
private:
	UINT	combobox_airfoils_height = 0;
	UINT	edit_interpolation_level_height = 0;
	UINT	lbl_airfoils_width = 0;
	UINT	lbl_interpolation_level_width = 0;
	CWnd*	message_target = NULL;
	UINT	m_nComboHeight;
	CFont	m_fntPropList;
	CPropertiesToolBar m_wndToolBar;
	CSpinButtonCtrl spinbtn_interpolation_level;
	CStatic lbl_airfoils, lbl_interpolation_level, lbl_background_canvas;
	CButton btn_input_groupbox;
	DECLARE_MESSAGE_MAP()
	void AdjustLayout();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	VOID SetupFont();
};
