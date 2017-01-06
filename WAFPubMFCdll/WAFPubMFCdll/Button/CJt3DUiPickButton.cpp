#include "StdAfx.h"
#include "..\resource.h"

IMPLEMENT_DYNAMIC(CJt3DUiPickButton, CButtonST)
CJt3DUiPickButton::CJt3DUiPickButton()
{

}

CJt3DUiPickButton::~CJt3DUiPickButton()
{

}

BOOL CJt3DUiPickButton::AutoLoad()
{
	SetBitmaps(IDB_BITMAP_SELECT, RGB(255, 255, 255), IDB_BITMAP_SELECT, RGB(255, 255, 255));

	CRect rect;
	GetWindowRect(&rect);

	int nSize = 24;

	rect.bottom = rect.top + nSize;
	rect.right = rect.left + nSize;

	this->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOMOVE);

	return TRUE;
}
