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

#define WHEEL_ZOOM_SPEED 0.02f
#define TRIANGLE_UNIT_LENGTH 0.004f // size of triangle side
#define D3DFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

#include <d3d9.h>
#include "airfoils.h"

class CcitafoilView : public CView
{
public:
	CcitafoilView() noexcept;
	virtual ~CcitafoilView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
private:
	// GUI
	DECLARE_DYNCREATE(CcitafoilView)
	void OnComboChanged();
	virtual void OnInitialUpdate();
	CcitafoilDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

	// DirectX 9
	struct fa_VERTEX
	{
		D3DXVECTOR3 location;	// (x,y,z)
		DWORD color;			// from the D3DFVF_DIFFUSE flag
	};
	LPDIRECT3D9 d3d = NULL;								// the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev = NULL;					// the pointer to the device class
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer;
	D3DCOLOR bpalette = D3DCOLOR_XRGB(255, 255, 255);	// palette color for background
	D3DCOLOR palette = D3DCOLOR_XRGB(0, 0, 0);			// palette color for text, graphics
	D3DXVECTOR3 cam_position = { 0.5f, 0.0f, -1.0f };	// set default camera position
	D3DPRESENT_PARAMETERS d3dpp;						// create a struct to hold various device information
	VOID begindraw();
	VOID cls();
	fa_point* drawcrs(fa_point* point, UINT _size, UINT _weight);
	VOID enddraw();
	VOID line(_In_ FLOAT _x1, _In_ FLOAT _y1, _In_ FLOAT _x2, _In_ FLOAT _y2);
	VOID outtextxy(LONG _x, LONG _y, CONST CHAR* txt);
	VOID pset(_In_ FLOAT _x, _In_ FLOAT _y);
	VOID plist(fa_point* point, UINT _size);
	VOID render();
};

#ifndef _DEBUG  // debug version in citafoilView.cpp
inline CcitafoilDoc* CcitafoilView::GetDocument() const
{
	return reinterpret_cast<CcitafoilDoc*>(m_pDocument);
}
#endif

