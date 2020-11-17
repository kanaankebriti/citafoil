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
#include <Windows.h>
#include <vector>
#include <string>
#include <Include/DxErr.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "citafoil.h"
#endif

#include "citafoilDoc.h"
#include "citafoilView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

LPCWSTR get_error_string_d3d9(HRESULT hr)
{
	std::wstring warning_msg_str = std::wstring(DXGetErrorString(hr)) + std::wstring(DXGetErrorDescription(hr));
	LPCWSTR warning_msg = warning_msg_str.c_str();
	return warning_msg;
}

#define ERROR_MSG(X) AfxMessageBox(X, MB_ICONSTOP | MB_OK);
#define HR_CHECK(X) if (FAILED(X)) { ERROR_MSG(get_error_string_d3d9(X)); __debugbreak(); }


IMPLEMENT_DYNCREATE(CcitafoilView, CView)

BEGIN_MESSAGE_MAP(CcitafoilView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_CBN_SELENDOK(IDR_COMBOBOX_AIRFOILS, &CcitafoilView::OnComboChanged)
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
END_MESSAGE_MAP()

CcitafoilView::CcitafoilView() noexcept
{
}

CcitafoilView::~CcitafoilView()
{
	// destroy d3d9 when close document
	//d3ddev->Release();
	//d3d->Release();
}

void CcitafoilView::OnInitialUpdate()
{
	// init d3d9
	d3d = Direct3DCreate9(D3D_SDK_VERSION);		// create the Direct3D interface
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;						// program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// discard old frames
	d3dpp.hDeviceWindow = this->m_hWnd;			// set the window to be used by Direct3D
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;		// enable anti-aliasing
	d3dpp.MultiSampleQuality = 1;							// antialiasing quality
	// create a device class using this information and the info from the d3dpp struct
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

	// visible back side of primitives
	//HR_CHECK(d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
}

void CcitafoilView::OnComboChanged()
{
	// retrieve selected airfoil
	CString selected_airfoil;
	CComboBox* combobox_airfoils = &((((CMainFrame*)AfxGetMainWnd())->m_wndProperties).combobox_airfoils);
	combobox_airfoils->GetLBText(combobox_airfoils->GetCurSel(), selected_airfoil);

	cls();
	begindraw();

	if (selected_airfoil == "NACA-0006")
	{
		drawcrs(&NACA0006, 1);
		palette = D3DCOLOR_XRGB(255, 0, 0);
		plist(&NACA0006);
	}
	else if (selected_airfoil == "NACA-0008")
	{
		drawcrs(&NACA0008, 1);
		palette = D3DCOLOR_XRGB(255, 0, 0);
		plist(&NACA0008);
	}
	else if (selected_airfoil == "NACA-0010")
	{
		drawcrs(&NACA0010, 1);
		palette = D3DCOLOR_XRGB(255, 0, 0);
		plist(&NACA0010);
	}
	else if (selected_airfoil == "NACA-2414")
	{
		drawcrs(&NACA2414, 1);
		palette = D3DCOLOR_XRGB(255, 0, 0);
		plist(&NACA2414);
	}
	palette = D3DCOLOR_XRGB(0, 0, 0);
	enddraw();
	render();
}

BOOL CcitafoilView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// change z position of camera
	if (zDelta > 0)
		// rotating forward
		cam_position.z += WHEEL_ZOOM_SPEED;
	else
		// rotating back
		cam_position.z -= WHEEL_ZOOM_SPEED;

	// the view transform matrix
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&cam_position,						// the camera position. this position (negative z) helps resembling conventional cartesian space.
		&D3DXVECTOR3(0.5f, 0.0f, 0.0f),		// the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	// the up direction
	HR_CHECK(d3ddev->SetTransform(D3DTS_VIEW, &matView));	// set the view transform to matView

	OnComboChanged();
	return TRUE;
}

BOOL CcitafoilView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CcitafoilView drawing

void CcitafoilView::OnDraw(CDC* /*pDC*/)
{
	CcitafoilDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rectClient;
	this->GetClientRect(rectClient);
	d3dpp.BackBufferWidth = rectClient.right - rectClient.left;
	d3dpp.BackBufferHeight = rectClient.bottom - rectClient.top;
	HR_CHECK(d3ddev->Reset(&d3dpp));

	// the view transform matrix
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&cam_position,						// the camera position. this position (negative z) helps resembling conventional cartesian space.
		&D3DXVECTOR3(0.5f, 0.0f, 0.0f),		// the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	// the up direction
	HR_CHECK(d3ddev->SetTransform(D3DTS_VIEW, &matView));	// set the view transform to matView
	
	// aspect ratio
	RECT lpRect;
	this->GetWindowRect(&lpRect);
	FLOAT wndWidth = (FLOAT)(lpRect.right) - (FLOAT)lpRect.left;
	FLOAT wndHeight = (FLOAT)(lpRect.bottom) - (FLOAT)lpRect.top;
	FLOAT wndAspect = wndWidth / wndHeight;

	// the projection transform matrix
	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),	// the horizontal field of view
		wndAspect,			// aspect ratio
		1,					// the near view-plane
		0);					// the far view-plane
	HR_CHECK(d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection));
	// select which vertex format we are using
	HR_CHECK(d3ddev->SetFVF(D3DFVF));
	// turn off the 3D lighting
	HR_CHECK(d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE));
	// clear output
	OnComboChanged();
}

void CcitafoilView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CcitafoilView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

/// <summary>clears entire viewport rectangle and sets background color</summary>
VOID CcitafoilView::cls()
{
	HR_CHECK(d3ddev->Clear(0, 0, 1, bpalette, 1, 0));
}

/// <summary>begins draw to screen</summary>
VOID CcitafoilView::begindraw()
{
	HR_CHECK(d3ddev->BeginScene());
}

/// <summary>ends draw to screen</summary>
VOID CcitafoilView::enddraw()
{
	HR_CHECK(d3ddev->EndScene());
}

/// <summary>draws a 2d line</summary>
VOID CcitafoilView::line(_In_ FLOAT _x1, _In_ FLOAT _y1, _In_ FLOAT _x2, _In_ FLOAT _y2)
{
	VOID* pVoid;

	fa_VERTEX vertices[2] =
	{
		{D3DXVECTOR3(_x1, _y1, 0), palette },
		{D3DXVECTOR3(_x2, _y2, 0), palette }
	};

	HR_CHECK(d3ddev->CreateVertexBuffer(sizeof(vertices), 0, D3DFVF, D3DPOOL_MANAGED, &vertex_buffer, NULL));
	vertex_buffer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_READONLY);	// lock the vertex buffer
	memcpy(pVoid, vertices, sizeof(vertices));		// copy the vertices to the locked buffer
	vertex_buffer->Unlock();						// unlock the vertex buffer
	HR_CHECK(d3ddev->SetStreamSource(0, vertex_buffer, 0, sizeof(fa_VERTEX)));	// select the vertex buffer to display
	HR_CHECK(d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, 1));						// copy the vertex buffer to the back buffer
}

/// <summary>main rendering function</summary>
VOID CcitafoilView::render()
{
	//bpalette = D3DCOLOR_XRGB(0, 0, 255);
	//cls();
	//CRect rectClient;
	//GetClientRect(rectClient);
	HR_CHECK(d3ddev->Present(NULL, NULL, NULL, NULL));
}

/// <summary>draws a 2d pixel with color specified in palette</summary>
VOID CcitafoilView::pset(_In_ FLOAT _x, _In_ FLOAT _y)
{
	VOID* pVoid;

	fa_VERTEX point[3] =
	{
		{ D3DXVECTOR3(_x + TRIANGLE_UNIT_LENGTH, _y - TRIANGLE_UNIT_LENGTH, 0.0f), palette},
		{ D3DXVECTOR3(_x - TRIANGLE_UNIT_LENGTH, _y - TRIANGLE_UNIT_LENGTH, 0.0f), palette},
		{ D3DXVECTOR3(_x, _y + TRIANGLE_UNIT_LENGTH, 0.0f), palette}
	};

	HR_CHECK(d3ddev->CreateVertexBuffer(3 * sizeof(fa_VERTEX), 0, D3DFVF, D3DPOOL_MANAGED, &vertex_buffer, NULL));
	vertex_buffer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_READONLY);
	memcpy(pVoid, &point, 3 * sizeof(fa_VERTEX));		// copy the vertices to the locked buffer
	vertex_buffer->Unlock();							// unlock the vertex buffer
	free(point);
	HR_CHECK(d3ddev->SetStreamSource(0, vertex_buffer, 0, sizeof(fa_VERTEX)));	// select the vertex buffer to display
	HR_CHECK(d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1));					// copy the vertex buffer to the back buffer
}

/// <summary>draws catmull-rom spline</summary>
fa_point* CcitafoilView::drawcrs(std::vector<D3DXVECTOR3>* _plist, UINT _weight)
{
	VOID* pVoid;				// the void pointer
	UINT i, j, m;				// counter
	FLOAT k = 1;				// weight counter
	fa_point* boundery_point;	// array of (x,y) locations of vertex will return using this pointer

	UINT number_of_vertices = _plist->size();							// number of control points
	number_of_vertices += (number_of_vertices - 4) * _weight + _weight;	// number of control points + interpolated points

	// memory allocation for vertices and boundary_points
	std::vector<std::pair<D3DXVECTOR3, D3DCOLOR>> vertex(number_of_vertices);
	boundery_point = (fa_point*)malloc(number_of_vertices * sizeof(fa_point));

	// map control point #0 and point #1 to vertex #0 and vertex #1
	for (i = 0; i <= 1; i++)
	{
		vertex.at(i).first.x = _plist->at(i).x;
		vertex.at(i).first.y = _plist->at(i).y;
		vertex.at(i).first.z = 0;
		vertex.at(i).second = palette;

		boundery_point[i].location.x = vertex.at(i).first.x;
		boundery_point[i].location.y = vertex.at(i).first.y;
	}

	// map control point #2 to point #n-1 to vertex
	for (i = _weight + 2, j = 2; i <= number_of_vertices - 1; i += _weight + 1, j++)
	{
		vertex.at(i).first.x = _plist->at(j).x;
		vertex.at(i).first.y = _plist->at(j).y;
		vertex.at(i).first.z = 0;
		vertex.at(i).second = palette;

		boundery_point[i].location.x = vertex.at(i).first.x;
		boundery_point[i].location.y = vertex.at(i).first.y;
	}

	// map control point #n to vertex and boundary_point
	i -= _weight;
	vertex.at(i).first.x = _plist->back().x;
	vertex.at(i).first.y = _plist->back().y;
	vertex.at(i).first.z = 0;
	vertex.at(i).second = palette;

	boundery_point[i].location.x = vertex.at(i).first.x;
	boundery_point[i].location.y = vertex.at(i).first.y;

	// point #1 to point #n-1 catmull-rom interpolation
	for (i = 1, m = 0; i < number_of_vertices - 2; i += _weight + 1, m++)
		for (j = i + 1, k = 1; j <= i + _weight; j++, k++)
		{
			D3DXVec3CatmullRom(&vertex.at(j).first, &_plist->at(m), &_plist->at(m + 1), &_plist->at(m + 2), &_plist->at(m + 3), k / (_weight + 1));
			vertex.at(j).second = palette;

			boundery_point[i].location.x = vertex.at(j).first.x;
			boundery_point[i].location.y = vertex.at(j).first.x;
		}

	HR_CHECK(d3ddev->CreateVertexBuffer(UINT(vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))), 0, D3DFVF, D3DPOOL_MANAGED, &vertex_buffer, NULL));
	vertex_buffer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_READONLY);							// lock the vertex buffer
	memcpy(pVoid, vertex.data(), vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3)));	// copy the vertices to the locked buffer
	vertex_buffer->Unlock();		// unlock the vertex buffer
	HR_CHECK(d3ddev->SetStreamSource(0, vertex_buffer, 0, sizeof(fa_VERTEX)));	// select the vertex buffer to display
	HR_CHECK(d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, number_of_vertices - 1));// copy the vertex buffer to the back buffer

	return boundery_point;
}

/// <summary>draws txt to screen at location (x,y)</summary>
VOID CcitafoilView::outtextxy(LONG _x, LONG _y, CONST CHAR* txt)
{
	// set font up
	LPD3DXFONT font;
	D3DXCreateFont(d3ddev, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &font);

	// set rectangle
	RECT FontRect;

	// get text width
	UINT string_length = strlen(txt);

	// set rectangle up
	FontRect.left = _x;
	FontRect.top = _y;
	FontRect.bottom = FontRect.top + 16;
	FontRect.right = FontRect.left + (16 * string_length);

	// draw final text
	font->DrawTextA(NULL, LPCSTR(txt), -1, &FontRect, DT_CENTER, palette);
}

/// <summary>draws list of points</summary>
VOID CcitafoilView::plist(std::vector<D3DXVECTOR3>* _plist)
{
	UINT i;		// counter
	VOID* pVoid;// the void pointer
	std::vector<std::pair<D3DXVECTOR3, D3DCOLOR>> vertex;

	// map control point #0 to point #n to vertex
	// plist->size() = number of control points
	for (i = 0; i < _plist->size(); i++)
	{
		// 1.155 = sqrt(16/3) / 2
		vertex.push_back(
			std::pair(
				D3DXVECTOR3(
					FLOAT(_plist->at(i).x + 1.155f * TRIANGLE_UNIT_LENGTH)
					, FLOAT(_plist->at(i).y - TRIANGLE_UNIT_LENGTH)
					, FLOAT(0))
				, palette
			)
		);

		vertex.push_back(
			std::pair(
				D3DXVECTOR3(
					FLOAT(_plist->at(i).x - 1.155f * TRIANGLE_UNIT_LENGTH)
					, FLOAT(_plist->at(i).y - TRIANGLE_UNIT_LENGTH)
					, FLOAT(0))
				, palette
			)
		);

		vertex.push_back(
			std::pair(
				D3DXVECTOR3(
					FLOAT(_plist->at(i).x)
					, FLOAT(_plist->at(i).y + TRIANGLE_UNIT_LENGTH)
					, FLOAT(0))
				, palette
			)
		);
	}

	HR_CHECK(d3ddev->CreateVertexBuffer(UINT(vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))), 0, D3DFVF, D3DPOOL_MANAGED, &vertex_buffer, NULL));
	vertex_buffer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_READONLY);	// lock the vertex buffer
	memcpy(pVoid, vertex.data(), vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3)));	// copy the vertices to the locked buffer
	vertex_buffer->Unlock();	// unlock the vertex buffer
	HR_CHECK(d3ddev->SetStreamSource(0, vertex_buffer, 0, (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))));	// select the vertex buffer to display
	HR_CHECK(d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, UINT(vertex.size())));	// copy the vertex buffer to the back buffer
}

// CcitafoilView diagnostics

#ifdef _DEBUG
void CcitafoilView::AssertValid() const
{
	CView::AssertValid();
}

void CcitafoilView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CcitafoilDoc* CcitafoilView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CcitafoilDoc)));
	return (CcitafoilDoc*)m_pDocument;
}
#endif //_DEBUG
