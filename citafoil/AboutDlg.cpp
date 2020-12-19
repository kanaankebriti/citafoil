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

#include "resource.h"
#include "AboutDlg.h"

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_EN_SETFOCUS(IDC_LIECENCE, &CAboutDlg::OnEnSetfocusLiecence)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CAboutDlg::OnEnSetfocusLiecence()
{
	CString retrieved_str;			// retrieved literal string
	BOOL	retrieved_str_valid;	// retrieved literal string validity check

	retrieved_str_valid = retrieved_str.LoadString(IDS_LIECENCE_INFO);
	ASSERT(retrieved_str_valid);

	SetDlgItemText(IDC_LIECENCE, retrieved_str);
}
