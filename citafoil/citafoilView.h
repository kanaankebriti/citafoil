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
	fa_point* drawcrs(std::vector<D3DXVECTOR3>* _plist, UINT _weight);
	VOID enddraw();
	VOID line(_In_ FLOAT _x1, _In_ FLOAT _y1, _In_ FLOAT _x2, _In_ FLOAT _y2);
	VOID outtextxy(LONG _x, LONG _y, CONST CHAR* txt);
	VOID pset(_In_ FLOAT _x, _In_ FLOAT _y);
	VOID plist(std::vector<D3DXVECTOR3>* _plist);
	VOID render();
	// airfoils
	std::vector<D3DXVECTOR3> NACA0006 =
	{
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00000),0),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00063),0),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(0.00403),0),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(0.00724),0),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(0.01312),0),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(0.01832),0),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(0.02282),0),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(0.02647),0),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(0.02902),0),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(0.03001),0),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(0.02971),0),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(0.02869),0),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(0.02673),0),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(0.02341),0),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(0.02100),0),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(0.01777),0),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(0.01307),0),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(0.00947),0),
		D3DXVECTOR3(0,0,0),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(-0.00947),0),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(-0.01307),0),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(-0.01777),0),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(-0.02100),0),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(-0.02341),0),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(-0.02869),0),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(-0.03001),0),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(-0.02902),0),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(-0.02647),0),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(-0.02282),0),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(-0.01832),0),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(-0.01312),0),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(-0.00724),0),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(-0.00403),0),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(-0.00063),0),
		D3DXVECTOR3(FLOAT(1.0000),0,0)
	};
	std::vector<D3DXVECTOR3> NACA0008 =
	{
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00000),0),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00084),0),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(0.00537),0),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(0.00965),0),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(0.01749),0),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(0.02443),0),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(0.03043),0),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(0.03529),0),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(0.03869),0),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(0.04001),0),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(0.03961),0),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(0.03825),0),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(0.03564),0),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(0.03121),0),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(0.02800),0),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(0.02369),0),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(0.01743),0),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(0.01263),0),
		D3DXVECTOR3(0,0,0),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(-0.01263),0),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(-0.01743),0),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(-0.02369),0),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(-0.02800),0),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(-0.03121),0),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(-0.03564),0),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(-0.03825),0),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(-0.03961),0),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(-0.04001),0),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(-0.03869),0),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(-0.03529),0),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(-0.03043),0),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(-0.02443),0),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(-0.01749),0),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(-0.00965),0),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(-0.00537),0),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(-0.00084),0),
		D3DXVECTOR3(FLOAT(1.0000),0,0)
	};
	std::vector<D3DXVECTOR3> NACA0010 =
	{
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00000),0),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00105),0),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(0.00672),0),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(0.01207),0),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(0.02187),0),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(0.03053),0),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(0.03803),0),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(0.04412),0),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(0.04837),0),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(0.05002),0),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(0.04952),0),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(0.04782),0),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(0.04455),0),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(0.03902),0),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(0.03500),0),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(0.02962),0),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(0.02178),0),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(0.01578),0),
		D3DXVECTOR3(0,0,0),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(-0.01578),0),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(-0.02178),0),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(-0.02962),0),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(-0.03500),0),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(-0.03902),0),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(-0.04455),0),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(-0.04782),0),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(-0.04952),0),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(-0.05002),0),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(-0.04837),0),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(-0.04412),0),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(-0.03803),0),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(-0.03053),0),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(-0.02187),0),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(-0.01207),0),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(-0.00672),0),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(-0.00105),0),
		D3DXVECTOR3(FLOAT(1.0000),0,0)
	};
	std::vector<D3DXVECTOR3> NACA2414 =
	{
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(0.00000),0),
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(0.00147),0),
		D3DXVECTOR3(FLOAT(0.99739),FLOAT(0.00210),0),
		D3DXVECTOR3(FLOAT(0.98929),FLOAT(0.00396),0),
		D3DXVECTOR3(FLOAT(0.97587),FLOAT(0.00700),0),
		D3DXVECTOR3(FLOAT(0.95729),FLOAT(0.01112),0),
		D3DXVECTOR3(FLOAT(0.93372),FLOAT(0.01620),0),
		D3DXVECTOR3(FLOAT(0.90542),FLOAT(0.02207),0),
		D3DXVECTOR3(FLOAT(0.87267),FLOAT(0.02857),0),
		D3DXVECTOR3(FLOAT(0.83582),FLOAT(0.03552),0),
		D3DXVECTOR3(FLOAT(0.79527),FLOAT(0.04274),0),
		D3DXVECTOR3(FLOAT(0.75143),FLOAT(0.05004),0),
		D3DXVECTOR3(FLOAT(0.70480),FLOAT(0.05723),0),
		D3DXVECTOR3(FLOAT(0.65586),FLOAT(0.06412),0),
		D3DXVECTOR3(FLOAT(0.60515),FLOAT(0.07053),0),
		D3DXVECTOR3(FLOAT(0.55324),FLOAT(0.07629),0),
		D3DXVECTOR3(FLOAT(0.50069),FLOAT(0.08120),0),
		D3DXVECTOR3(FLOAT(0.44808),FLOAT(0.08512),0),
		D3DXVECTOR3(FLOAT(0.39598),FLOAT(0.08787),0),
		D3DXVECTOR3(FLOAT(0.34454),FLOAT(0.08913),0),
		D3DXVECTOR3(FLOAT(0.29482),FLOAT(0.08866),0),
		D3DXVECTOR3(FLOAT(0.24740),FLOAT(0.08645),0),
		D3DXVECTOR3(FLOAT(0.20285),FLOAT(0.08255),0),
		D3DXVECTOR3(FLOAT(0.16169),FLOAT(0.07707),0),
		D3DXVECTOR3(FLOAT(0.12440),FLOAT(0.07014),0),
		D3DXVECTOR3(FLOAT(0.09141),FLOAT(0.06198),0),
		D3DXVECTOR3(FLOAT(0.06310),FLOAT(0.05281),0),
		D3DXVECTOR3(FLOAT(0.03977),FLOAT(0.04289),0),
		D3DXVECTOR3(FLOAT(0.02165),FLOAT(0.03245),0),
		D3DXVECTOR3(FLOAT(0.00892),FLOAT(0.02171),0),
		D3DXVECTOR3(FLOAT(0.00169),FLOAT(0.01085),0),
		D3DXVECTOR3(0,0,0),
		D3DXVECTOR3(FLOAT(0.00379),FLOAT(-0.01031),0),
		D3DXVECTOR3(FLOAT(0.01293),FLOAT(-0.01956),0),
		D3DXVECTOR3(FLOAT(0.02730),FLOAT(-0.02770),0),
		D3DXVECTOR3(FLOAT(0.04669),FLOAT(-0.03471),0),
		D3DXVECTOR3(FLOAT(0.07087),FLOAT(-0.04054),0),
		D3DXVECTOR3(FLOAT(0.09957),FLOAT(-0.04516),0),
		D3DXVECTOR3(FLOAT(0.13246),FLOAT(-0.04858),0),
		D3DXVECTOR3(FLOAT(0.16918),FLOAT(-0.05082),0),
		D3DXVECTOR3(FLOAT(0.20937),FLOAT(-0.05195),0),
		D3DXVECTOR3(FLOAT(0.25260),FLOAT(-0.05208),0),
		D3DXVECTOR3(FLOAT(0.29844),FLOAT(-0.05133),0),
		D3DXVECTOR3(FLOAT(0.34644),FLOAT(-0.04987),0),
		D3DXVECTOR3(FLOAT(0.39611),FLOAT(-0.04787),0),
		D3DXVECTOR3(FLOAT(0.44739),FLOAT(-0.04537),0),
		D3DXVECTOR3(FLOAT(0.49931),FLOAT(-0.04232),0),
		D3DXVECTOR3(FLOAT(0.55129),FLOAT(-0.03886),0),
		D3DXVECTOR3(FLOAT(0.60276),FLOAT(-0.03516),0),
		D3DXVECTOR3(FLOAT(0.65316),FLOAT(-0.03132),0),
		D3DXVECTOR3(FLOAT(0.70194),FLOAT(-0.02745),0),
		D3DXVECTOR3(FLOAT(0.74857),FLOAT(-0.02365),0),
		D3DXVECTOR3(FLOAT(0.79252),FLOAT(-0.01998),0),
		D3DXVECTOR3(FLOAT(0.83331),FLOAT(-0.01650),0),
		D3DXVECTOR3(FLOAT(0.87048),FLOAT(-0.01328),0),
		D3DXVECTOR3(FLOAT(0.90360),FLOAT(-0.01035),0),
		D3DXVECTOR3(FLOAT(0.93230),FLOAT(-0.00776),0),
		D3DXVECTOR3(FLOAT(0.95626),FLOAT(-0.00557),0),
		D3DXVECTOR3(FLOAT(0.97518),FLOAT(-0.00381),0),
		D3DXVECTOR3(FLOAT(0.98886),FLOAT(-0.00252),0),
		D3DXVECTOR3(FLOAT(0.99713),FLOAT(-0.00173),0),
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(-0.00147),0),
		D3DXVECTOR3(FLOAT(1.00000),0,0)
	};
};

#ifndef _DEBUG  // debug version in citafoilView.cpp
inline CcitafoilDoc* CcitafoilView::GetDocument() const
{
	return reinterpret_cast<CcitafoilDoc*>(m_pDocument);
}
#endif

