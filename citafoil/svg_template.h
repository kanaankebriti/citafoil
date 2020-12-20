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

WCHAR byte_order_mark = 0xFEFF;
CString svg_template_header =
	L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
	L"\n<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"800\" font-family=\"serif\">";
CString svg_template_footer = L"\n</svg>\n";

CString svg_template_body =
	L"\n\t<rect id=\"container-box\" x=\"10\" y=\"10\" width=\"789\" height=\"789\" style=\"stroke-width:0.8; stroke:gray; fill:transparent\"/>"
	L"\n\t<g id=\"grid-lines\" style=\"stroke:gray;stroke-width:0.5\">"
	L"\n\t\t<line id=\"bl_(-9,0)\" x1=\"130\" y1=\"100\" x2=\"130\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(-8,0)\" x1=\"160\" y1=\"100\" x2=\"160\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(-7,0)\" x1=\"190\" y1=\"100\" x2=\"190\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(-6,0)\" x1=\"220\" y1=\"100\" x2=\"220\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(-4,0)\" x1=\"280\" y1=\"100\" x2=\"280\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(-3,0)\" x1=\"310\" y1=\"100\" x2=\"310\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(-2,0)\" x1=\"340\" y1=\"100\" x2=\"340\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(-1,0)\" x1=\"370\" y1=\"100\" x2=\"370\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(1,0)\" x1=\"430\" y1=\"100\" x2=\"430\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(2,0)\" x1=\"460\" y1=\"100\" x2=\"460\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(3,0)\" x1=\"490\" y1=\"100\" x2=\"490\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(4,0)\" x1=\"520\" y1=\"100\" x2=\"520\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(6,0)\" x1=\"580\" y1=\"100\" x2=\"580\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(7,0)\" x1=\"610\" y1=\"100\" x2=\"610\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(8,0)\" x1=\"640\" y1=\"100\" x2=\"640\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(9,0)\" x1=\"670\" y1=\"100\" x2=\"670\" y2=\"700\"/>"
	L"\n\t\t<line id=\"bl_(0,1.8)\" x1=\"100\" y1=\"130\" x2=\"700\" y2=\"130\"/>"
	L"\n\t\t<line id=\"bl_(0,1.6)\" x1=\"100\" y1=\"160\" x2=\"700\" y2=\"160\"/>"
	L"\n\t\t<line id=\"bl_(0,1.4)\" x1=\"100\" y1=\"190\" x2=\"700\" y2=\"190\"/>"
	L"\n\t\t<line id=\"bl_(0,1.2)\" x1=\"100\" y1=\"220\" x2=\"700\" y2=\"220\"/>"
	L"\n\t\t<line id=\"bl_(0,0.8)\" x1=\"100\" y1=\"280\" x2=\"700\" y2=\"280\"/>"
	L"\n\t\t<line id=\"bl_(0,0.6)\" x1=\"100\" y1=\"310\" x2=\"700\" y2=\"310\"/>"
	L"\n\t\t<line id=\"bl_(0,0.4)\" x1=\"100\" y1=\"340\" x2=\"700\" y2=\"340\"/>"
	L"\n\t\t<line id=\"bl_(0,0.2)\" x1=\"100\" y1=\"370\" x2=\"700\" y2=\"370\"/>"
	L"\n\t\t<line id=\"bl_(0,-0.2)\" x1=\"100\" y1=\"430\" x2=\"700\" y2=\"430\"/>"
	L"\n\t\t<line id=\"bl_(0,-0.4)\" x1=\"100\" y1=\"460\" x2=\"700\" y2=\"460\"/>"
	L"\n\t\t<line id=\"bl_(0,-0.6)\" x1=\"100\" y1=\"490\" x2=\"700\" y2=\"490\"/>"
	L"\n\t\t<line id=\"bl_(0,-0.8)\" x1=\"100\" y1=\"520\" x2=\"700\" y2=\"520\"/>"
	L"\n\t\t<line id=\"bl_(0,-1.2)\" x1=\"100\" y1=\"580\" x2=\"700\" y2=\"580\"/>"
	L"\n\t\t<line id=\"bl_(0,-1.4)\" x1=\"100\" y1=\"610\" x2=\"700\" y2=\"610\"/>"
	L"\n\t\t<line id=\"bl_(0,-1.6)\" x1=\"100\" y1=\"640\" x2=\"700\" y2=\"640\"/>"
	L"\n\t\t<line id=\"bl_(0,-1.8)\" x1=\"100\" y1=\"670\" x2=\"700\" y2=\"670\"/>"
	L"\n\t</g>"
	L"\n\t<g id=\"bold-lines\" style=\"stroke:black;stroke-width:1.2\">"
	L"\n\t\t<line id=\"(-10,0)\" x1=\"100\" y1=\"100\" x2=\"100\" y2=\"700\"/>"
	L"\n\t\t<line id=\"(-5,0)\" x1=\"250\" y1=\"100\" x2=\"250\" y2=\"700\"/>"
	L"\n\t\t<line id=\"(5,0)\" x1=\"550\" y1=\"100\" x2=\"550\" y2=\"700\"/>"
	L"\n\t\t<line id=\"(10,0)\" x1=\"700\" y1=\"100\" x2=\"700\" y2=\"700\"/>"
	L"\n\t\t<line id=\"(0,2)\" x1=\"99.5\" y1=\"100\" x2=\"700.5\" y2=\"100\"/>"
	L"\n\t\t<line id=\"(0,1)\" x1=\"99.5\" y1=\"250\" x2=\"700.5\" y2=\"250\"/>"
	L"\n\t\t<line id=\"(0,-1)\" x1=\"99.5\" y1=\"550\" x2=\"700.5\" y2=\"550\"/>"
	L"\n\t\t<line id=\"(0,-2)\" x1=\"99.5\" y1=\"700\" x2=\"700.5\" y2=\"700\"/>"
	L"\n\t</g>"
	L"\n\t<g id=\"coordinate-system\" style=\"stroke:black;stroke-width:2.5\">"
	L"\n\t\t<line id=\"x-axis\" x1=\"10\" y1=\"400\" x2=\"785\" y2=\"400\"/>"
	L"\n\t\t<line id=\"y-axis\" x1=\"400\" y1=\"24\" x2=\"400\" y2=\"799\"/>"
	L"\n\t\t<polygon id=\"x-axis-arrow\" points=\"780,405 795,400 780,395\"/>"
	L"\n\t\t<polygon id=\"y-axis-arrow\" points=\"395,30 400,15 405,30\"/>"
	L"\n\t</g>"
	L"\n\t<g id=\"texts\" style=\"font-size:20px\">"
	L"\n\t\t<text id=\"t_(-10,0)\" x=\"65\" y=\"423\">-10</text>"
	L"\n\t\t<text id=\"t_(-5,0)\" x=\"225\" y=\"423\">-5</text>"
	L"\n\t\t<text id=\"t_(5,0)\" x=\"530\" y=\"423\">5</text>"
	L"\n\t\t<text id=\"t_(10,0)\" x=\"675\" y=\"423\">10</text>"
	L"\n\t\t<text id=\"t_(0,2)\" x=\"380\" y=\"121\">2</text>"
	L"\n\t\t<text id=\"t_(0,1)\" x=\"380\" y=\"272\">1</text>"
	L"\n\t\t<text id=\"t_(0,-1)\" x=\"375\" y=\"572\">-1</text>"
	L"\n\t\t<text id=\"t_(0,-2)\" x=\"375\" y=\"719.5\">-2</text>"
	L"\n\t</g>"
	L"\n\t<g id=\"axis-name\" style=\"font-size:20px\">"
	L"\n\t\t<text id=\"t_(-10,0)\" x=\"373\" y=\"28\">Cₗ</text>"
	L"\n\t\t<text id=\"t_(-10,0)\" x=\"765\" y=\"425\">𝛼(°)</text>"
	L"\n\t</g>";
