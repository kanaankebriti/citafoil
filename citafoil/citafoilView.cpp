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
#include <algorithm>
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
#include <stdio.h>
#include <iostream>

FILE* g_ic_file_cout_stream; FILE* g_ic_file_cin_stream;

// Success: true , Failure: false
bool InitConsole()
{
	if (!AllocConsole()) { return false; }
	if (freopen_s(&g_ic_file_cout_stream, "CONOUT$", "w", stdout) != 0) { return false; } // For std::cout 
	if (freopen_s(&g_ic_file_cin_stream, "CONIN$", "w+", stdin) != 0) { return false; } // For std::cin
	return true;
}
#endif

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#define M_PI	3.14159265358979323846f
#define M_PI2	9.86960440108935861883f

LPCWSTR get_error_string_d3d9(HRESULT hr)
{
	std::wstring warning_msg_str = std::wstring(DXGetErrorString(hr)) + std::wstring(DXGetErrorDescription(hr));
	LPCWSTR warning_msg = warning_msg_str.c_str();
	return warning_msg;
}

#define ERROR_MSG(X) AfxMessageBox(X, MB_ICONSTOP | MB_OK);
#define HR_CHECK(X) if (FAILED(X)) { ERROR_MSG(get_error_string_d3d9(X)); __debugbreak(); terminate(); }
#define LOAD_AIRFOIL(X) {if (!interpolation_level){boundary_points = X;linter(&X);}else boundary_points = drawcrs(&X, interpolation_level);plist(&boundary_points, &X);}

IMPLEMENT_DYNCREATE(CcitafoilView, CView)

BEGIN_MESSAGE_MAP(CcitafoilView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_CBN_SELENDOK(IDR_COMBOBOX_AIRFOILS, &CcitafoilView::On_combobox_airfoils_changed)
	ON_EN_CHANGE(IDR_EDIT_INTERPOLATION_LEVEL, &CcitafoilView::On_edit_interpolation_level_changed)
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()

CcitafoilView::CcitafoilView() noexcept {}

CcitafoilView::~CcitafoilView()
{
	// destroy d3d9 when close document
	// without if, it will fail on startup
	// twice instantiation! why? i don't know!
	if (d3ddev != NULL)
	{
		d3ddev->Release();
		d3d->Release();
	}
}

VOID CcitafoilView::terminate()
{
	ASSERT(AfxGetMainWnd() != NULL);
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

void CcitafoilView::OnInitialUpdate()
{
	// init d3d9
	d3d = Direct3DCreate9(D3D_SDK_VERSION);		// create the Direct3D interface
	if (!d3d)
	{
		ERROR_MSG(L"Failed to initialize Direct3D 9 - the application was built against the correct header files.");
		terminate();
	}
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Windowed = TRUE;						// program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// discard old frames
	d3dpp.hDeviceWindow = this->m_hWnd;			// set the window to be used by Direct3D
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;		// enable anti-aliasing
	//d3dpp.MultiSampleQuality = 1;							// antialiasing quality
	// create a device class using this information and the info from the d3dpp struct
	HR_CHECK(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev));

	if (!d3ddev)
	{
		ERROR_MSG(L"Failed to initialize Direct3D 9 Device.");
		terminate();
	}

	// the view transform matrix
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&cam_position,						// the camera position. this position (negative z) helps resembling conventional cartesian space.
		&lookat_position,					// the look-at position
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
	// visible back side of primitives
	//HR_CHECK(d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

	On_combobox_airfoils_changed();
}

void CcitafoilView::On_combobox_airfoils_changed()
{
	// retrieve selected airfoil
	CComboBox* combobox_airfoils = &((((CMainFrame*)AfxGetMainWnd())->m_wndProperties).combobox_airfoils);
	combobox_airfoils->GetLBText(combobox_airfoils->GetCurSel(), selected_airfoil);

	On_edit_interpolation_level_changed();
}

bool compareByX(D3DXVECTOR3& p1, D3DXVECTOR3& p2)
{
	return p1.x < p2.x;
}

/// <summary>perform bisection for each quadrilateral in boundary points</summary>
VOID CcitafoilView::bisect(std::vector<D3DXVECTOR3>* _plist)
{
	VOID* pVoid; // the void pointer
	std::vector<D3DXVECTOR3> upper_surface, lower_surface;
	std::vector<std::pair<D3DXVECTOR3, D3DCOLOR>> mean_camber_line;
	std::vector<std::pair<D3DXVECTOR3, D3DCOLOR>> airfoil_mesh;

	float x1, x2, x3, x4, y1, y2, y3, y4;	// all points of each mesh	
	float x5 = 0, x6 = 0, y5 = 0, y6 = 0;	// mean camber line points. two points hence a line.

	float theta1;			// lower bound of integral for each section. transformed from x to theta.
	float theta2;			// upper bound of integral for each section. transformed from x to theta.
	float m;				// slope of each section
	float partial_term;		// contribution of each mesh to alpha_lift_zero
	float alpha_lift_zero = 0;

	USHORT i = 1;			// counter. from 1 in order to exclude (1,0)


	while (_plist->at(i).y > 0)
	{
		upper_surface.push_back(_plist->at(i));
		i++;
	}

	// exclude (0,0)
	if ((_plist->at(i).x == 0) && (_plist->at(i).y == 0))
		i++;

	while (_plist->at(i).x < 1)
	{
		lower_surface.push_back(_plist->at(i));
		i++;
	}

	// rearrange upper_surface from (0,0) to (1,0)
	std::reverse(upper_surface.begin(), upper_surface.end());

	// mean camber line color
	palette = D3DCOLOR_XRGB(255, 0, 0);

	// connect mean camber line to the leading edge
	mean_camber_line.push_back(std::pair(D3DXVECTOR3(0, 0, 0), palette));

	for (USHORT i = 0; i < max(upper_surface.size(), lower_surface.size()) - 2; i++)
	{
		// rearenge point of each quadrilateral such that
		// upper left	(x1,y1) ┐
		// upper right	(x3,y3) ┤
		//						└─> from upper surface
		//
		// lower left	(x2,y2) ┐
		// lower right	(x4,y4) ┤
		//						└─> from lower surface

		if (i < upper_surface.size() - 1)
		{
			x1 = upper_surface.at(i).x;
			y1 = upper_surface.at(i).y;
			x3 = upper_surface.at(i + 1).x;
			y3 = upper_surface.at(i + 1).y;
		}
		// in case of upper_surface out of bound
		// consider only last two control points
		else
		{
			x1 = upper_surface.at(upper_surface.size() - 2).x;
			y1 = upper_surface.at(upper_surface.size() - 2).y;
			x3 = upper_surface.at(upper_surface.size() - 1).x;
			y3 = upper_surface.at(upper_surface.size() - 1).y;
		}

		if (i < lower_surface.size() - 1)
		{
			x2 = lower_surface.at(i).x;
			y2 = lower_surface.at(i).y;
			x4 = lower_surface.at(i + 1).x;
			y4 = lower_surface.at(i + 1).y;
		}
		// in case of lower_surface out of bound
		// consider only last two control points
		else
		{
			x2 = lower_surface.at(lower_surface.size() - 2).x;
			y2 = lower_surface.at(lower_surface.size() - 2).y;
			x4 = lower_surface.at(lower_surface.size() - 1).x;
			y4 = lower_surface.at(lower_surface.size() - 1).y;
		}

		float A1 = y1 - y3;
		float A2 = y2 - y4;
		float B1 = x3 - x1;
		float B2 = x4 - x2;
		float C1 = -A1 * x1 - B1 * y1;
		float C2 = -A2 * x2 - B1 * y2;
		float R = sqrtf((powf(A2, 2) + powf(B2, 2)) / (powf(A1, 2) + powf(B1, 2)));

		float A3 = y1 - y2;
		float A4 = y3 - y4;
		float B3 = x2 - x1;
		float B4 = x4 - x3;
		float C3 = A3 * x1 + B3 * y1;
		float C4 = A4 * x3 + B4 * y3;

		if ((A1 * A2 + B1 * B2) > 0)
		{
			x5 = (C3 * (R * B1 + B2) + B3 * (C2 + R * C1)) / (A3 * (R * B1 + B2) - B3 * (R * A1 + A2));
			y5 = (-A3 * (C2 + R * C1) - C3 * (R * A1 + A2)) / (A3 * (R * B1 + B2) - B3 * (R * A1 + A2));
			x6 = (C4 * (R * B1 + B2) + B4 * (C2 + R * C1)) / (A4 * (R * B1 + B2) - B4 * (R * A1 + A2));
			y6 = (-A4 * (C2 + R * C1) - C4 * (R * A1 + A2)) / (A4 * (R * B1 + B2) - B4 * (R * A1 + A2));
		}
		else
		{
			// this is my best effort. just ignore the problem!
			continue;
			// these are incorrect!
			//x5 = (C3 * (R * B1 - B2) - B3 * (C2 - R * C1)) / (A3 * (R * B1 - B2) - B3 * (R * A1 - A2));
			//y5 = (A3 * (C2 - R * C1) - C3 * (R * A1 - A2)) / (A3 * (R * B1 - B2) - B3 * (R * A1 - A2));
			//x6 = (C4 * (R * B1 - B2) - B4 * (C2 - R * C1)) / (A4 * (R * B1 - B2) - B4 * (R * A1 - A2));
			//y6 = (A4 * (C2 - R * C1) - C4 * (R * A1 - A2)) / (A4 * (R * B1 - B2) - B4 * (R * A1 - A2));
		}

		mean_camber_line.push_back(std::pair(D3DXVECTOR3(x5, y5, 0), palette));

		theta1 = acosf(1 - 2 * x5);
		theta2 = acosf(1 - 2 * x6);
		m = (y6 - y5) / (x6 - x5);
		partial_term = m * (sinf(theta2) - sinf(theta1) - theta2 + theta1);

		alpha_lift_zero += partial_term;

		// add mesh lines
		airfoil_mesh.push_back(std::pair(D3DXVECTOR3(x1, y1, 0), D3DCOLOR_XRGB(0, 255, 0)));
		airfoil_mesh.push_back(std::pair(D3DXVECTOR3(x2, y2, 0), D3DCOLOR_XRGB(0, 255, 0)));
		airfoil_mesh.push_back(std::pair(D3DXVECTOR3(x3, y3, 0), D3DCOLOR_XRGB(0, 255, 0)));
		airfoil_mesh.push_back(std::pair(D3DXVECTOR3(x4, y4, 0), D3DCOLOR_XRGB(0, 255, 0)));
	}

	mean_camber_line.push_back(std::pair(D3DXVECTOR3(x6, y6, 0), palette));

	// there are here ONLY because i can't
	// figure out a way to find TRUE leading edge!
	theta2 = acosf(1 - 2 * mean_camber_line.at(1).first.x);
	m = mean_camber_line.at(1).first.y / mean_camber_line.at(1).first.x;
	partial_term = m * (sinf(theta2) - theta2);
	alpha_lift_zero += partial_term;

	theta1 = acosf(1 - 2 * mean_camber_line.back().first.x);
	m = -mean_camber_line.back().first.y / (1 - mean_camber_line.back().first.x);
	partial_term = m * (-sinf(theta1) - M_PI + theta1);
	alpha_lift_zero += partial_term;

	mean_camber_line.push_back(std::pair(D3DXVECTOR3(1, 0, 0), palette));

	alpha_lift_zero *= -180.000 / M_PI2;

	// retrieve lbl_alpha_lift_zero_result and put result in it
	CStatic* lbl_alpha_lift_zero_result = &((((CMainFrame*)AfxGetMainWnd())->m_wndProperties).lbl_alpha_lift_zero_result);
	CString alpha_lift_zero_str;
	alpha_lift_zero_str.Format(L"%.*f°", RESULT_DECIMAL_PRECISION, alpha_lift_zero);
	lbl_alpha_lift_zero_result->SetWindowTextW(alpha_lift_zero_str);

	// FALSE chord line color
	palette = D3DCOLOR_XRGB(0, 255, 255);

	// draw FALSE chord line
	// chord line is considered part of mesh list for convenience
	airfoil_mesh.push_back(std::pair(D3DXVECTOR3(1, 0, 0), palette));
	airfoil_mesh.push_back(std::pair(D3DXVECTOR3(0, 0, 0), palette));

	// assign mean_camber_line to mean_camber_line_vertex_buffer
	HR_CHECK(d3ddev->CreateVertexBuffer(UINT(mean_camber_line.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))), 0, D3DFVF, D3DPOOL_MANAGED, &mean_camber_line_vertex_buffer, NULL));
	mean_camber_line_vertex_buffer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_READONLY);	// lock the vertex buffer
	memcpy(pVoid, mean_camber_line.data(), mean_camber_line.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3)));	// copy the vertices to the locked buffer
	mean_camber_line_vertex_buffer->Unlock();	// unlock the vertex buffer

	// assign airfoil_mesh to airfoil_mesh_vertex_buffer
	HR_CHECK(d3ddev->CreateVertexBuffer(UINT(airfoil_mesh.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))), 0, D3DFVF, D3DPOOL_MANAGED, &airfoil_mesh_vertex_buffer, NULL));
	airfoil_mesh_vertex_buffer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_READONLY);	// lock the vertex buffer
	memcpy(pVoid, airfoil_mesh.data(), airfoil_mesh.size()* (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3)));	// copy the vertices to the locked buffer
	airfoil_mesh_vertex_buffer->Unlock();	// unlock the vertex buffer

	// restore palette color
	palette = D3DCOLOR_XRGB(0, 0, 0);
	redraw_vbuffer();
}

void CcitafoilView::On_edit_interpolation_level_changed()
{
	// retrieve selected interpolation level
	CEdit* edit_interpolation_level = &((((CMainFrame*)AfxGetMainWnd())->m_wndProperties).edit_interpolation_level);
	CString interpolation_level_str;
	edit_interpolation_level->GetWindowTextW(interpolation_level_str);
	int interpolation_level = _ttoi(interpolation_level_str);

	// calculate interpolated airfoil and draw it
	std::vector<D3DXVECTOR3> boundary_points;

	if (selected_airfoil == "NACA 0006")
		LOAD_AIRFOIL(NACA0006)
	else if (selected_airfoil == "NACA 0008")
		LOAD_AIRFOIL(NACA0008)
	else if (selected_airfoil == "NACA 0010")
		LOAD_AIRFOIL(NACA0010)
	else if (selected_airfoil == "NACA 2412")
		LOAD_AIRFOIL(NACA2412)
	else if (selected_airfoil == "NACA 2414")
		LOAD_AIRFOIL(NACA2414)
	else if (selected_airfoil == "NACA 4412")
		LOAD_AIRFOIL(NACA4412)
	else if (selected_airfoil == "NACA 4415")
		LOAD_AIRFOIL(NACA4415)
	else if (selected_airfoil == "NACA 6409")
		LOAD_AIRFOIL(NACA6409)
	else if (selected_airfoil == "NACA 23012")
		LOAD_AIRFOIL(NACA23012)
	else if (selected_airfoil == "NACA 23112")
		LOAD_AIRFOIL(NACA23112)
	else if (selected_airfoil == "NACA 25112")
		LOAD_AIRFOIL(NACA25112)
	else if (selected_airfoil == "NACA 63(2)-615")
		LOAD_AIRFOIL(NACA632615)
	else if (selected_airfoil == "NACA 65(2)-215")
		LOAD_AIRFOIL(NACA652215)
	else if (selected_airfoil == "NACA M6")
		LOAD_AIRFOIL(NACAM6)
	//else if (selected_airfoil == "Selig S1223")
		//LOAD_AIRFOIL(S1223)
	else if (selected_airfoil == "Selig S3024")
		LOAD_AIRFOIL(S3024)
	else if (selected_airfoil == "Selig S9000")
		LOAD_AIRFOIL(S9000)

	// restore palette color
	palette = D3DCOLOR_XRGB(0, 0, 0);

	bisect(&boundary_points);
}

VOID CcitafoilView::redraw_vbuffer()
{
	UINT airfoil_vertex_buffer_size = 0;
	UINT boundary_points_vertex_buffer_size = 0;
	UINT camber_line_vertex_buffer_size = 0;
	UINT airfoil_mesh_vertex_buffer_size = 0;
	D3DVERTEXBUFFER_DESC vbuffer_data;

	// retrive vertex buffers size
	HR_CHECK(airfoil_vertex_buffer->GetDesc(&vbuffer_data));
	airfoil_vertex_buffer_size = vbuffer_data.Size;
	airfoil_vertex_buffer_size /= sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3);
	
	HR_CHECK(boundary_points_vertex_buffer->GetDesc(&vbuffer_data));
	boundary_points_vertex_buffer_size = vbuffer_data.Size;
	boundary_points_vertex_buffer_size /= sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3);
	
	HR_CHECK(mean_camber_line_vertex_buffer->GetDesc(&vbuffer_data));
	camber_line_vertex_buffer_size = vbuffer_data.Size;
	camber_line_vertex_buffer_size /= sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3);

	HR_CHECK(airfoil_mesh_vertex_buffer->GetDesc(&vbuffer_data));
	airfoil_mesh_vertex_buffer_size = vbuffer_data.Size;
	airfoil_mesh_vertex_buffer_size /= sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3);

	// redraw vertex buffers
	cls();
	render();
	begindraw();
	// redraw boundary points
	HR_CHECK(d3ddev->SetStreamSource(0, boundary_points_vertex_buffer, 0, sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))); // select the vertex buffer to display
	HR_CHECK(d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, boundary_points_vertex_buffer_size / 3)); // copy the vertex buffer to the back buffer
	// redraw airfoil mesh
	HR_CHECK(d3ddev->SetStreamSource(0, airfoil_mesh_vertex_buffer, 0, sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))); // select the vertex buffer to display
	HR_CHECK(d3ddev->DrawPrimitive(D3DPT_LINELIST, 0, airfoil_mesh_vertex_buffer_size)); // copy the vertex buffer to the back buffer
	// redraw airfoil spline
	HR_CHECK(d3ddev->SetStreamSource(0, airfoil_vertex_buffer, 0, sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))); // select the vertex buffer to display
	HR_CHECK(d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, airfoil_vertex_buffer_size - 1)); // copy the vertex buffer to the back buffer
	// redraw camberline spline
	HR_CHECK(d3ddev->SetStreamSource(0, mean_camber_line_vertex_buffer, 0, sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))); // select the vertex buffer to display
	HR_CHECK(d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, camber_line_vertex_buffer_size - 1)); // copy the vertex buffer to the back buffer
	enddraw();
	render();
}

void CcitafoilView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
		cam_position.x -= CAMERA_MOVEMENT_STEP;
		lookat_position.x -= CAMERA_MOVEMENT_STEP;
		break;
	case VK_RIGHT:
		cam_position.x += CAMERA_MOVEMENT_STEP;
		lookat_position.x += CAMERA_MOVEMENT_STEP;
		break;
	case VK_UP:
		cam_position.y += CAMERA_MOVEMENT_STEP;
		lookat_position.y += CAMERA_MOVEMENT_STEP;
		break;
	case VK_DOWN:
		cam_position.y -= CAMERA_MOVEMENT_STEP;
		lookat_position.y -= CAMERA_MOVEMENT_STEP;
		break;
	case VK_HOME:
		// restore default position
		cam_position = DEFAULT_CAM_POS;
		lookat_position = DEFAULT_LOOKAT_POS;
		break;
	case VK_PRIOR:
		// zoom-in
		cam_position.z += WHEEL_ZOOM_SPEED;
		break;
	case VK_NEXT:
		// zoom-out
		cam_position.z -= WHEEL_ZOOM_SPEED;
		break;
	};

	// the view transform matrix
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&cam_position,						// the camera position. this position (negative z) helps resembling conventional cartesian space.
		&lookat_position,					// the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	// the up direction
	HR_CHECK(d3ddev->SetTransform(D3DTS_VIEW, &matView));	// set the view transform to matView

	redraw_vbuffer();
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
		&lookat_position,					// the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));	// the up direction
	HR_CHECK(d3ddev->SetTransform(D3DTS_VIEW, &matView));	// set the view transform to matView

	redraw_vbuffer();

	return TRUE;
}

BOOL CcitafoilView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
#ifdef _DEBUG
	InitConsole();
#endif
	return CView::PreCreateWindow(cs);
}

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
		&lookat_position,		// the look-at position
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
	
	redraw_vbuffer();
}

void CcitafoilView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
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
	HR_CHECK(d3ddev->SetStreamSource(0, vertex_buffer, 0, 3 * sizeof(fa_VERTEX)));	// select the vertex buffer to display
	HR_CHECK(d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1));					// copy the vertex buffer to the back buffer
}

/// <summary>draws catmull-rom spline</summary>
std::vector<D3DXVECTOR3> CcitafoilView::drawcrs(std::vector<D3DXVECTOR3>* _plist, UINT _weight)
{
	VOID* pVoid;				// the void pointer
	UINT i, j, m;				// counter
	FLOAT k = 1;				// weight counter

	UINT number_of_vertices = _plist->size();							// number of control points
	number_of_vertices += (number_of_vertices - 4) * _weight + _weight;	// number of control points + interpolated points

	// memory allocation for vertices and boundary points (return)
	std::vector<std::pair<D3DXVECTOR3, D3DCOLOR>> vertex(number_of_vertices);
	std::vector<D3DXVECTOR3> boundary_points;

	// map control point #0 and point #1 to vertex #0 and vertex #1
	for (i = 0; i <= 1; i++)
	{
		vertex.at(i).first.x = _plist->at(i).x;
		vertex.at(i).first.y = _plist->at(i).y;
		vertex.at(i).first.z = 0;
		vertex.at(i).second = palette;
	}

	// map control point #2 to point #n-1 to vertex
	for (i = _weight + 2, j = 2; i <= number_of_vertices - 1; i += _weight + 1, j++)
	{
		vertex.at(i).first.x = _plist->at(j).x;
		vertex.at(i).first.y = _plist->at(j).y;
		vertex.at(i).first.z = 0;
		vertex.at(i).second = palette;
	}

	// map control point #n to vertex
	i -= _weight;
	vertex.at(i).first.x = _plist->back().x;
	vertex.at(i).first.y = _plist->back().y;
	vertex.at(i).first.z = 0;
	vertex.at(i).second = palette;

	// point #1 to point #n-1 catmull-rom interpolation
	for (i = 1, m = 0; i < number_of_vertices - 2; i += _weight + 1, m++)
		for (j = i + 1, k = 1; j <= i + _weight; j++, k++)
		{
			D3DXVec3CatmullRom(&vertex.at(j).first, &_plist->at(m), &_plist->at(m + 1), &_plist->at(m + 2), &_plist->at(m + 3), k / (_weight + 1));
			vertex.at(j).second = palette;
		}

	HR_CHECK(d3ddev->CreateVertexBuffer(UINT(vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))), 0, D3DFVF, D3DPOOL_MANAGED, &airfoil_vertex_buffer, NULL));
	airfoil_vertex_buffer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_READONLY);							// lock the vertex buffer
	memcpy(pVoid, vertex.data(), vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3)));	// copy the vertices to the locked buffer
	airfoil_vertex_buffer->Unlock();		// unlock the vertex buffer

	// vertex.first contains (x,y,z) and that is what we want as return
	for (i = 0; i < vertex.size(); i++)
		boundary_points.push_back(vertex.at(i).first);

	return boundary_points;
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

VOID CcitafoilView::linter(std::vector<D3DXVECTOR3>* _plist)
{
	UINT i;		// counter
	VOID* pVoid;// the void pointer
	std::vector<std::pair<D3DXVECTOR3, D3DCOLOR>> vertex;

	// map control point #0 to point #n to vertex
	for (i = 1; i < _plist->size(); i++)
	{
		vertex.push_back(std::pair(D3DXVECTOR3(FLOAT(_plist->at(i - 1).x), FLOAT(_plist->at(i - 1).y), FLOAT(0)), palette));
		vertex.push_back(std::pair(D3DXVECTOR3(FLOAT(_plist->at(i).x), FLOAT(_plist->at(i).y), FLOAT(0)), palette));
	}

	HR_CHECK(d3ddev->CreateVertexBuffer(UINT(vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))), 0, D3DFVF, D3DPOOL_MANAGED, &airfoil_vertex_buffer, NULL));
	airfoil_vertex_buffer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_READONLY);	// lock the vertex buffer
	memcpy(pVoid, vertex.data(), vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3)));	// copy the vertices to the locked buffer
	airfoil_vertex_buffer->Unlock();	// unlock the vertex buffer
}

/// <summary>draws list of points</summary>
VOID CcitafoilView::plist(std::vector<D3DXVECTOR3>* _boundary_points, std::vector<D3DXVECTOR3>* _original_points)
{
	UINT i;		// counter
	VOID* pVoid;// the void pointer
	std::vector<std::pair<D3DXVECTOR3, D3DCOLOR>> vertex;

	// color of _boundary_points
	palette = D3DCOLOR_XRGB(255, 255, 0);

	// map control point #0 to point #n to vertex
	// plist->size() = number of control points
	for (i = 0; i < _boundary_points->size(); i++)
	{
		// 1.155 = sqrt(16/3) / 2
		vertex.push_back(
			std::pair(
				D3DXVECTOR3(
					FLOAT(_boundary_points->at(i).x + 1.155f * TRIANGLE_UNIT_LENGTH)
					, FLOAT(_boundary_points->at(i).y - TRIANGLE_UNIT_LENGTH)
					, FLOAT(0))
				, palette
			)
		);

		vertex.push_back(
			std::pair(
				D3DXVECTOR3(
					FLOAT(_boundary_points->at(i).x - 1.155f * TRIANGLE_UNIT_LENGTH)
					, FLOAT(_boundary_points->at(i).y - TRIANGLE_UNIT_LENGTH)
					, FLOAT(0))
				, palette
			)
		);

		vertex.push_back(
			std::pair(
				D3DXVECTOR3(
					FLOAT(_boundary_points->at(i).x)
					, FLOAT(_boundary_points->at(i).y + TRIANGLE_UNIT_LENGTH)
					, FLOAT(0))
				, palette
			)
		);
	}

	// color of _original_points
	palette = D3DCOLOR_XRGB(255, 0, 0);

	// load _original_points to plist_vertex_buffer
	for (i = 0; i < _original_points->size(); i++)
	{
		// 1.155 = sqrt(16/3) / 2
		vertex.push_back(
			std::pair(
				D3DXVECTOR3(
					FLOAT(_original_points->at(i).x + 1.155f * TRIANGLE_UNIT_LENGTH)
					, FLOAT(_original_points->at(i).y - TRIANGLE_UNIT_LENGTH)
					, FLOAT(0))
				, palette
			)
		);

		vertex.push_back(
			std::pair(
				D3DXVECTOR3(
					FLOAT(_original_points->at(i).x - 1.155f * TRIANGLE_UNIT_LENGTH)
					, FLOAT(_original_points->at(i).y - TRIANGLE_UNIT_LENGTH)
					, FLOAT(0))
				, palette
			)
		);

		vertex.push_back(
			std::pair(
				D3DXVECTOR3(
					FLOAT(_original_points->at(i).x)
					, FLOAT(_original_points->at(i).y + TRIANGLE_UNIT_LENGTH)
					, FLOAT(0))
				, palette
			)
		);
	}

	HR_CHECK(d3ddev->CreateVertexBuffer(UINT(vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3))), 0, D3DFVF, D3DPOOL_MANAGED, &boundary_points_vertex_buffer, NULL));
	boundary_points_vertex_buffer->Lock(0, 0, (VOID**)&pVoid, D3DLOCK_READONLY);	// lock the vertex buffer
	memcpy(pVoid, vertex.data(), vertex.size() * (sizeof(D3DCOLOR) + sizeof(D3DXVECTOR3)));	// copy the vertices to the locked buffer
	boundary_points_vertex_buffer->Unlock();	// unlock the vertex buffer
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
