#pragma once

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
	void AdjustLayout();
	VOID SetTarget(CWnd* m_cwnd);
	// GUI elements access from directx window
	CComboBox combobox_airfoils;
private:
	UINT	combobox_airfoils_height = 0;
	CWnd*	message_target = NULL;
	UINT	m_nComboHeight;
	CFont	m_fntPropList;
	CPropertiesToolBar m_wndToolBar;
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	VOID SetupFont();
};
