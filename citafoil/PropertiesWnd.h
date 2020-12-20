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

class CPropertiesWnd : public CDockablePane
{
public:
	CPropertiesWnd() noexcept;
	virtual ~CPropertiesWnd();
	VOID SetTarget(CWnd* m_cwnd);
	// GUI elements access from directx window
	CComboBox combobox_airfoils;
	CEdit edit_interpolation_level;
	CStatic lbl_alpha_lift_zero_result;
private:
	// Labels
	CStatic lbl_background_canvas;
	CStatic lbl_airfoils;				UINT lbl_airfoils_width = 0;
	CStatic lbl_interpolation_level;	UINT lbl_interpolation_level_width = 0;
	CStatic lbl_alpha_lift_zero;		UINT lbl_alpha_lift_zero_width = 0;

	// Group Boxes
	CButton btn_input_groupbox;
	CButton btn_result_groupbox;

	// Buttons
	CMFCButton btn_save_cl_alpha_svg;

	UINT	combobox_airfoils_height = 0;
	UINT	edit_interpolation_level_height = 0;
	CWnd*	message_target = NULL;
	UINT	m_nComboHeight;
	CFont	m_fntPropList;
	CSpinButtonCtrl spinbtn_interpolation_level;
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	VOID SetupFont();
};
