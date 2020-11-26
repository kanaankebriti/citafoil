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
#define CAMERA_MOVEMENT_STEP 0.05f
#define TRIANGLE_UNIT_LENGTH 0.004f // size of triangle side
#define D3DFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

#include <vector>
#include <Include/d3d9.h>
#include <Include/d3dx9.h>

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
	VOID terminate();
	VOID bisect(std::vector<D3DXVECTOR3>* _plist);
	// GUI
	DECLARE_DYNCREATE(CcitafoilView)
	CString selected_airfoil;
	void On_combobox_airfoils_changed();
	void On_edit_interpolation_level_changed();
	virtual void OnInitialUpdate();
	CcitafoilDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);		// camera movement effect
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	// zoom effect
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
	LPDIRECT3DVERTEXBUFFER9 airfoil_vertex_buffer = NULL;
	LPDIRECT3DVERTEXBUFFER9 boundary_points_vertex_buffer = NULL;
	LPDIRECT3DVERTEXBUFFER9 mean_camber_line_vertex_buffer = NULL;
	D3DCOLOR bpalette = D3DCOLOR_XRGB(255, 255, 255);	// palette color for background
	D3DCOLOR palette = D3DCOLOR_XRGB(0, 0, 0);			// palette color for text, graphics
	D3DXVECTOR3 cam_position = { 0.5f, 0.0f, -1.0f };	// set default camera position
	D3DXVECTOR3 lookat_position = { 0.5f, 0.0f, 0.0f };	// set default look-at position
	D3DPRESENT_PARAMETERS d3dpp;						// create a struct to hold various device information
	VOID begindraw();
	VOID cls();
	std::vector<D3DXVECTOR3> drawcrs(std::vector<D3DXVECTOR3>* _plist, UINT _weight);
	VOID enddraw();
	VOID line(_In_ FLOAT _x1, _In_ FLOAT _y1, _In_ FLOAT _x2, _In_ FLOAT _y2);
	VOID linter(std::vector<D3DXVECTOR3>* _plist);
	VOID outtextxy(LONG _x, LONG _y, CONST CHAR* txt);
	VOID pset(_In_ FLOAT _x, _In_ FLOAT _y);
	VOID plist(std::vector<D3DXVECTOR3>* _boundary_points, std::vector<D3DXVECTOR3>* _original_points);
	VOID redraw_vbuffer();
	VOID render();
	// airfoils
	std::vector<D3DXVECTOR3> NACA0006 =
	{
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00063),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(0.00403),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(0.00724),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(0.01312),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(0.01832),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(0.02282),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(0.02647),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(0.02902),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(0.03001),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(0.02971),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(0.02869),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(0.02673),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(0.02341),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(0.02100),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(0.01777),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(0.01307),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(0.00947),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(-0.00947),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(-0.01307),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(-0.01777),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(-0.02100),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(-0.02341),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(-0.02673),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(-0.02869),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(-0.02971),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(-0.03001),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(-0.02902),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(-0.02647),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(-0.02282),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(-0.01832),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(-0.01312),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(-0.00724),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(-0.00403),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(-0.00063),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.000000),FLOAT(0.00000))
	};
	std::vector<D3DXVECTOR3> NACA0008 =
	{
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00084),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(0.00537),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(0.00965),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(0.01749),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(0.02443),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(0.03043),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(0.03529),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(0.03869),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(0.04001),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(0.03961),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(0.03825),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(0.03564),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(0.03121),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(0.02800),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(0.02369),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(0.01743),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(0.01263),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(-0.01263),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(-0.01743),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(-0.02369),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(-0.02800),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(-0.03121),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(-0.03564),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(-0.03825),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(-0.03961),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(-0.04001),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(-0.03869),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(-0.03529),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(-0.03043),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(-0.02443),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(-0.01749),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(-0.00965),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(-0.00537),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(-0.00084),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.000000),FLOAT(0.00000))
	};
	std::vector<D3DXVECTOR3> NACA0010 =
	{
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.00105),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(0.00672),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(0.01207),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(0.02187),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(0.03053),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(0.03803),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(0.04412),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(0.04837),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(0.05002),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(0.04952),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(0.04782),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(0.04455),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(0.03902),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(0.03500),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(0.02962),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(0.02178),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(0.01578),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0125),FLOAT(-0.01578),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0250),FLOAT(-0.02178),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0500),FLOAT(-0.02962),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.0750),FLOAT(-0.03500),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1000),FLOAT(-0.03902),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.1500),FLOAT(-0.04455),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2000),FLOAT(-0.04782),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.2500),FLOAT(-0.04952),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.3000),FLOAT(-0.05002),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.4000),FLOAT(-0.04837),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.5000),FLOAT(-0.04412),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.6000),FLOAT(-0.03803),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.7000),FLOAT(-0.03053),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.8000),FLOAT(-0.02187),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9000),FLOAT(-0.01207),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.9500),FLOAT(-0.00672),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(-0.00105),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.0000),FLOAT(0.000000),FLOAT(0.00000))
	};
	std::vector<D3DXVECTOR3> NACA4415 =
	{
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.99893),FLOAT(0.00039),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.99572),FLOAT(0.00156),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.99039),FLOAT(0.00349),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.98296),FLOAT(0.00610),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.97347),FLOAT(0.00932),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.96194),FLOAT(0.01303),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.94844),FLOAT(0.01716),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.93301),FLOAT(0.02166),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.91573),FLOAT(0.02652),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.89668),FLOAT(0.03171),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.87592),FLOAT(0.03717),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.85355),FLOAT(0.04283),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.82967),FLOAT(0.04863),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.80438),FLOAT(0.05453),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.77779),FLOAT(0.06048),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.75000),FLOAT(0.06642),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.72114),FLOAT(0.07227),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.69134),FLOAT(0.07795),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.66072),FLOAT(0.08341),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.62941),FLOAT(0.08858),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.59755),FLOAT(0.09341),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.56526),FLOAT(0.09785),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.53270),FLOAT(0.10185),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.50000),FLOAT(0.10538),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.46730),FLOAT(0.10837),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.43474),FLOAT(0.11076),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.40245),FLOAT(0.11248),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.37059),FLOAT(0.11345),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.33928),FLOAT(0.11361),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.30866),FLOAT(0.11294),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.27886),FLOAT(0.11141),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.25000),FLOAT(0.10903),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.22221),FLOAT(0.10584),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.19562),FLOAT(0.10190),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.17033),FLOAT(0.09726),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.14645),FLOAT(0.09195),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.12408),FLOAT(0.08607),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.10332),FLOAT(0.07970),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.08427),FLOAT(0.07283),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.06699),FLOAT(0.06541),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.05156),FLOAT(0.05753),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.03806),FLOAT(0.04937),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.02653),FLOAT(0.04118),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.01704),FLOAT(0.03303),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00961),FLOAT(0.02489),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00428),FLOAT(0.01654),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00107),FLOAT(0.00825),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00000),FLOAT(0.00075),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00107),FLOAT(-0.00566),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00428),FLOAT(-0.01102),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00961),FLOAT(-0.01590),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.01704),FLOAT(-0.02061),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.02653),FLOAT(-0.02502),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.03806),FLOAT(-0.02915),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.05156),FLOAT(-0.03281),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.06699),FLOAT(-0.03582),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.08427),FLOAT(-0.03817),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.10332),FLOAT(-0.03991),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.12408),FLOAT(-0.04106),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.14645),FLOAT(-0.04166),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.17033),FLOAT(-0.04177),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.19562),FLOAT(-0.04147),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.22221),FLOAT(-0.04078),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.25000),FLOAT(-0.03974),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.27886),FLOAT(-0.03845),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.30866),FLOAT(-0.03700),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.33928),FLOAT(-0.03547),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.37059),FLOAT(-0.03390),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.40245),FLOAT(-0.03229),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.43474),FLOAT(-0.03063),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.46730),FLOAT(-0.02891),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.50000),FLOAT(-0.02713),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.53270),FLOAT(-0.02529),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.56526),FLOAT(-0.02340),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.59755),FLOAT(-0.02149),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.62941),FLOAT(-0.01958),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.66072),FLOAT(-0.01772),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.69134),FLOAT(-0.01596),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.72114),FLOAT(-0.01430),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.75000),FLOAT(-0.01277),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.77779),FLOAT(-0.01136),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.80438),FLOAT(-0.01006),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.82967),FLOAT(-0.00886),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.85355),FLOAT(-0.00775),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.87592),FLOAT(-0.00674),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.89668),FLOAT(-0.00583),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.91573),FLOAT(-0.00502),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.93301),FLOAT(-0.00431),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.94844),FLOAT(-0.00364),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.96194),FLOAT(-0.00297),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.97347),FLOAT(-0.00227),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.98296),FLOAT(-0.00156),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.99039),FLOAT(-0.00092),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.99572),FLOAT(-0.00042),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.99893),FLOAT(-0.00011),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(0.00000),FLOAT(0.00000))
	};
	std::vector<D3DXVECTOR3> NACA2414 =
	{
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(0.00147),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.99739),FLOAT(0.00210),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.98929),FLOAT(0.00396),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.97587),FLOAT(0.00700),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.95729),FLOAT(0.01112),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.93372),FLOAT(0.01620),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.90542),FLOAT(0.02207),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.87267),FLOAT(0.02857),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.83582),FLOAT(0.03552),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.79527),FLOAT(0.04274),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.75143),FLOAT(0.05004),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.70480),FLOAT(0.05723),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.65586),FLOAT(0.06412),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.60515),FLOAT(0.07053),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.55324),FLOAT(0.07629),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.50069),FLOAT(0.08120),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.44808),FLOAT(0.08512),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.39598),FLOAT(0.08787),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.34454),FLOAT(0.08913),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.29482),FLOAT(0.08866),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.24740),FLOAT(0.08645),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.20285),FLOAT(0.08255),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.16169),FLOAT(0.07707),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.12440),FLOAT(0.07014),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.09141),FLOAT(0.06198),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.06310),FLOAT(0.05281),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.03977),FLOAT(0.04289),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.02165),FLOAT(0.03245),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00892),FLOAT(0.02171),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00169),FLOAT(0.01085),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.00379),FLOAT(-0.01031),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.01293),FLOAT(-0.01956),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.02730),FLOAT(-0.02770),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.04669),FLOAT(-0.03471),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.07087),FLOAT(-0.04054),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.09957),FLOAT(-0.04516),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.13246),FLOAT(-0.04858),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.16918),FLOAT(-0.05082),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.20937),FLOAT(-0.05195),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.25260),FLOAT(-0.05208),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.29844),FLOAT(-0.05133),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.34644),FLOAT(-0.04987),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.39611),FLOAT(-0.04787),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.44739),FLOAT(-0.04537),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.49931),FLOAT(-0.04232),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.55129),FLOAT(-0.03886),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.60276),FLOAT(-0.03516),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.65316),FLOAT(-0.03132),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.70194),FLOAT(-0.02745),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.74857),FLOAT(-0.02365),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.79252),FLOAT(-0.01998),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.83331),FLOAT(-0.01650),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.87048),FLOAT(-0.01328),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.90360),FLOAT(-0.01035),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.93230),FLOAT(-0.00776),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.95626),FLOAT(-0.00557),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.97518),FLOAT(-0.00381),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.98886),FLOAT(-0.00252),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.99713),FLOAT(-0.00173),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(-0.00147),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(0.000000),FLOAT(0.00000))
	};
	std::vector<D3DXVECTOR3> NACA23012 =
	{
		D3DXVECTOR3(FLOAT(1.00000),FLOAT(0.00000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.997270),FLOAT(0.000458),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.989109),FLOAT(0.001818),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.975606),FLOAT(0.004041),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.956908),FLOAT(0.007065),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.933219),FLOAT(0.010809),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.904797),FLOAT(0.015177),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.871953),FLOAT(0.020064),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.835045),FLOAT(0.025360),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.794476),FLOAT(0.030950),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.750689),FLOAT(0.036718),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.704163),FLOAT(0.042544),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.655407),FLOAT(0.048306),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.604952),FLOAT(0.053875),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.553351),FLOAT(0.059116),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.501167),FLOAT(0.063891),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.448969),FLOAT(0.068055),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.397328),FLOAT(0.071468),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.346806),FLOAT(0.073996),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.297957),FLOAT(0.075524),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.251312),FLOAT(0.075956),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.207382),FLOAT(0.075230),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.166045),FLOAT(0.073193),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.127322),FLOAT(0.069150),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.092309),FLOAT(0.062655),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.062030),FLOAT(0.053824),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.037302),FLOAT(0.043243),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.018649),FLOAT(0.031767),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.006283),FLOAT(0.020296),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.000150),FLOAT(0.009563),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.000000),FLOAT(0.000000),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.005328),FLOAT(-0.007915),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.015570),FLOAT(-0.014008),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.030295),FLOAT(-0.018693),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.049152),FLOAT(-0.022475),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.071944),FLOAT(-0.025852),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.098674),FLOAT(-0.029222),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.129534),FLOAT(-0.032817),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.164824),FLOAT(-0.036603),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.204833),FLOAT(-0.040166),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.248688),FLOAT(-0.042830),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.295307),FLOAT(-0.044457),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.344177),FLOAT(-0.045088),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.394760),FLOAT(-0.044792),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.446502),FLOAT(-0.043662),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.498833),FLOAT(-0.041807),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.551177),FLOAT(-0.039341),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.602959),FLOAT(-0.036383),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.653610),FLOAT(-0.033046),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.702573),FLOAT(-0.029443),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.749311),FLOAT(-0.025676),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.793310),FLOAT(-0.021847),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.834086),FLOAT(-0.018053),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.871192),FLOAT(-0.014392),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.904220),FLOAT(-0.010959),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.932807),FLOAT(-0.007850),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.956638),FLOAT(-0.005156),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.975451),FLOAT(-0.002960),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.989039),FLOAT(-0.001335),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(0.997252),FLOAT(-0.000337),FLOAT(0.00000)),
		D3DXVECTOR3(FLOAT(1.000000),FLOAT(0.000000),FLOAT(0.00000)),
	};
};

#ifndef _DEBUG  // debug version in citafoilView.cpp
inline CcitafoilDoc* CcitafoilView::GetDocument() const
{
	return reinterpret_cast<CcitafoilDoc*>(m_pDocument);
}
#endif

