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

#define D3DFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define WHEEL_ZOOM_SPEED 0.02f
#define CAMERA_MOVEMENT_STEP 0.05f
#define TRIANGLE_UNIT_LENGTH 0.003f		// size of triangle side
#define RESULT_DECIMAL_PRECISION 5		// digits after decimal point
#define SVG_RESULT_DECIMAL_PRECISION 2	// digits after decimal point for SVG output
#define DEFAULT_CAM_POS { 0.5f, 0.0f, -0.75f }
#define DEFAULT_LOOKAT_POS { 0.5f, 0.0f, 0.0f }

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
	FLOAT alpha_lift_zero = 0.0; // in radians
	VOID terminate();
	VOID bisect(std::vector<D3DXVECTOR3>* _plist);
	// GUI
	DECLARE_DYNCREATE(CcitafoilView)
	CString selected_airfoil;
	CString interpolation_level_str;
	void On_combobox_airfoils_changed();
	void On_edit_interpolation_level_changed();
	virtual void OnInitialUpdate();
	CcitafoilDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);		// camera movement effect
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	// zoom effect
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void save_cl_alpha_svg();
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
	LPDIRECT3DVERTEXBUFFER9 airfoil_mesh_vertex_buffer = NULL;
	D3DCOLOR bpalette = D3DCOLOR_XRGB(255, 255, 255);	// palette color for background
	D3DCOLOR palette = D3DCOLOR_XRGB(0, 0, 0);			// palette color for text, graphics
	D3DXVECTOR3 cam_position = DEFAULT_CAM_POS;			// set default camera position
	D3DXVECTOR3 lookat_position = DEFAULT_LOOKAT_POS;	// set default look-at position
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
};

#ifndef _DEBUG  // debug version in citafoilView.cpp
inline CcitafoilDoc* CcitafoilView::GetDocument() const
{
	return reinterpret_cast<CcitafoilDoc*>(m_pDocument);
}
#endif
