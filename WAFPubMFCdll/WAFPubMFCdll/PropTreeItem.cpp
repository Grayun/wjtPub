// WAFPubMFCdllItem.cpp
//
//  Copyright (C) 1998-2001 Scott Ramsay
//	sramsay@gonavi.com
//	http://www.gonavi.com
//
//  This material is provided "as is", with absolutely no warranty expressed
//  or implied. Any use is at your own risk.
// 
//  Permission to use or copy this software for any purpose is hereby granted 
//  without fee, provided the above notices are retained on all copies.
//  Permission to modify the code and to distribute modified code is granted,
//  provided the above notices are retained, and a notice that the code was
//  modified is included with the above copyright notice.
// 
//	If you use this code, drop me an email.  I'd like to know if you find the code
//	useful.

#include "stdafx.h"
#define WAFPUBMFCDLLITEM_DEFHEIGHT			21			// default heigt of an item
#define WAFPUBMFCDLLITEM_SPACE				5			// default horz spacing
#define WAFPUBMFCDLLITEM_EXPANDBOX			9			// size of the expand box
#define WAFPUBMFCDLLITEM_CHECKBOX			14			// size of the check box
#define WAFPUBMFCDLLITEM_EXPANDCOLUMN		16			// width of the expand column
#define PNINDENT						16			// child level indent

#define WAFPUBMFCDLLITEM_EXPANDBOXHALF		(WAFPUBMFCDLLITEM_EXPANDBOX/2)


/////////////////////////////////////////////////////////////////////////////
// drawing helper functions
//

// draw a dotted horizontal line
static void _DotHLine(HDC hdc, LONG x, LONG y, LONG w)
{
	for (; w>0; w-=2, x+=2)
		SetPixel(hdc, x, y, GetSysColor(COLOR_BTNSHADOW));
}


// draw the plus/minus button
static void _DrawExpand(HDC hdc, LONG x, LONG y, BOOL bExpand, BOOL bFill)
{
	HPEN hPen;
	HPEN oPen;
	HBRUSH oBrush;

	hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
	oPen = (HPEN)SelectObject(hdc, hPen);
	oBrush = (HBRUSH)SelectObject(hdc, GetStockObject(bFill ? WHITE_BRUSH : NULL_BRUSH));

	Rectangle(hdc, x, y, x + WAFPUBMFCDLLITEM_EXPANDBOX, y + WAFPUBMFCDLLITEM_EXPANDBOX);
	SelectObject(hdc, GetStockObject(BLACK_PEN));

	if (!bExpand)
	{
		MoveToEx(hdc, x + WAFPUBMFCDLLITEM_EXPANDBOXHALF, y + 2, NULL);
		LineTo(hdc, x + WAFPUBMFCDLLITEM_EXPANDBOXHALF, y + WAFPUBMFCDLLITEM_EXPANDBOX - 2);
	}

	MoveToEx(hdc, x + 2, y + WAFPUBMFCDLLITEM_EXPANDBOXHALF, NULL);
	LineTo(hdc, x + WAFPUBMFCDLLITEM_EXPANDBOX - 2, y + WAFPUBMFCDLLITEM_EXPANDBOXHALF);

	SelectObject(hdc, oPen);
	SelectObject(hdc, oBrush);
	DeleteObject(hPen);
}

/////////////////////////////////////////////////////////////////////////////
// PropTreelItem
//

CPropTreelItem::CPropTreelItem() :
	m_pProp(NULL),
	m_sLabel(_T("")),
	m_sInfo(_T("")),
	m_loc(0,0),
	m_rc(0,0,0,0),
	m_lParam(0),
	m_nCtrlID(0),
	m_dwState(0),
	m_bActivated(FALSE),
	m_bCommitOnce(FALSE),
	m_rcExpand(0,0,0,0),
	m_rcCheckbox(0,0,0,0),
	m_pParent(NULL),
	m_pSibling(NULL),
	m_pChild(NULL),
	m_pVis(NULL)
{
}


CPropTreelItem::~CPropTreelItem()
{
}


BOOL CPropTreelItem::IsExpanded()
{
	return (m_dwState & TreeItemExpanded) ? TRUE : FALSE;
}


BOOL CPropTreelItem::IsSelected()
{
	return (m_dwState & TreeItemSelected) ? TRUE : FALSE;
}


BOOL CPropTreelItem::IsChecked()
{
	return (m_dwState & TreeItemChecked) ? TRUE : FALSE;
}


BOOL CPropTreelItem::IsReadOnly()
{
	return (m_dwState & TreeItemReadOnly) ? TRUE : FALSE;
}


BOOL CPropTreelItem::IsActivated()
{
	return (m_dwState & TreeItemActivated) ? TRUE : FALSE;
}


void CPropTreelItem::Select(BOOL bSelect)
{
	if (bSelect)
		m_dwState |= TreeItemSelected;
	else
		m_dwState &= ~TreeItemSelected;
}


void CPropTreelItem::Expand(BOOL bExpand)
{
	if (bExpand)
		m_dwState |= TreeItemExpanded;
	else
		m_dwState &= ~TreeItemExpanded;
}


void CPropTreelItem::Check(BOOL bCheck)
{
	if (bCheck)
		m_dwState |= TreeItemChecked;
	else
		m_dwState &= ~TreeItemChecked;
}


void CPropTreelItem::ReadOnly(BOOL bReadOnly)
{
	if (bReadOnly)
		m_dwState |= TreeItemReadOnly;
	else
		m_dwState &= ~TreeItemReadOnly;
}


BOOL CPropTreelItem::IsCheckBox()
{
	return (m_dwState & TreeItemCheckbox) ? TRUE : FALSE;
}


void CPropTreelItem::HasCheckBox(BOOL bCheckbox)
{
	if (bCheckbox)
		m_dwState |= TreeItemCheckbox;
	else
		m_dwState &= ~TreeItemCheckbox;
}


BOOL CPropTreelItem::HitExpand(const POINT& pt)
{
	return m_rcExpand.PtInRect(pt);
}


BOOL CPropTreelItem::HitCheckBox(const POINT& pt)
{
	return m_rcCheckbox.PtInRect(pt);
}


BOOL CPropTreelItem::IsRootLevel()
{
	ASSERT(m_pProp!=NULL);
	return GetParent() == m_pProp->GetRootItem();
}


LONG CPropTreelItem::GetTotalHeight()
{
	CPropTreelItem* pItem;
	LONG nHeight;

	nHeight = GetHeight();

	if (IsExpanded())
	{
		for (pItem = GetChild(); pItem; pItem = pItem->GetSibling())
			nHeight += pItem->GetTotalHeight();
	}

	return nHeight;
}


void CPropTreelItem::SetLabelText(LPCTSTR sLabel)
{
	m_sLabel = sLabel;
}


LPCTSTR CPropTreelItem::GetLabelText()
{
	return m_sLabel;
}


void CPropTreelItem::SetInfoText(LPCTSTR sInfo)
{
	m_sInfo = sInfo;
}


LPCTSTR CPropTreelItem::GetInfoText()
{
	return m_sInfo;
}


void CPropTreelItem::SetCtrlID(UINT nCtrlID)
{
	m_nCtrlID = nCtrlID;
}


UINT CPropTreelItem::GetCtrlID()
{
	return m_nCtrlID;
}


LONG CPropTreelItem::GetHeight()
{
	return WAFPUBMFCDLLITEM_DEFHEIGHT;
}


LPARAM CPropTreelItem::GetItemValue()
{
	// no items are assocatied with this type
	return 0L;
}


void CPropTreelItem::SetItemValue(LPARAM)
{
	// no items are assocatied with this type
}


void CPropTreelItem::OnMove()
{
	// no attributes, do nothing
}


void CPropTreelItem::OnRefresh()
{
	// no attributes, do nothing
}


void CPropTreelItem::OnCommit()
{
	// no attributes, do nothing
}


void CPropTreelItem::Activate()
{
	m_bActivated = TRUE;
	m_bCommitOnce = FALSE;

	OnActivate();
}


void CPropTreelItem::CommitChanges()
{
	m_bActivated = FALSE;

	if (m_bCommitOnce)
		return;

	m_bCommitOnce = TRUE;

	ASSERT(m_pProp!=NULL);

	OnCommit();

	m_pProp->SendNotify(PTN_ITEMCHANGED, this);
	m_pProp->RefreshItems(this);
}


void CPropTreelItem::OnActivate()
{
	// no attributes, do nothing
}


void CPropTreelItem::SetPropOwner(CPropTree* pProp)
{
	m_pProp = pProp;
}


const POINT& CPropTreelItem::GetLocation()
{
	return m_loc;
}


CPropTreelItem* CPropTreelItem::GetParent()
{
	return m_pParent;
}


CPropTreelItem* CPropTreelItem::GetSibling()
{
	return m_pSibling;
}


CPropTreelItem* CPropTreelItem::GetChild()
{
	return m_pChild;
}


CPropTreelItem* CPropTreelItem::GetNextVisible()
{
	return m_pVis;
}


void CPropTreelItem::SetParent(CPropTreelItem* pParent)
{
	m_pParent = pParent;
}


void CPropTreelItem::SetSibling(CPropTreelItem* pSibling)
{
	m_pSibling = pSibling;
}


void CPropTreelItem::SetChild(CPropTreelItem* pChild)
{
	m_pChild = pChild;
}


void CPropTreelItem::SetNextVisible(CPropTreelItem* pVis)
{
	m_pVis = pVis;
}


LONG CPropTreelItem::DrawItem(CDC* pDC, const RECT& rc, LONG x, LONG y)
{
	CPoint pt;
	LONG nTotal, nCol, ey;
	CRect drc, ir;

	ASSERT(m_pProp!=NULL);

	// Add TreeItem the list of visble items
	m_pProp->AddToVisibleList(this);

	// store the item's location
	m_loc = CPoint(x, y);

	// store the items rectangle position
	m_rc.SetRect(m_pProp->GetOrigin().x + WAFPUBMFCDLLITEM_SPACE, m_loc.y, rc.right, m_loc.y + GetHeight()-1);
	m_rc.OffsetRect(0, -m_pProp->GetOrigin().y);

	// init temp drawing variables
	nTotal = GetHeight();
	ey = (nTotal >> 1) - (WAFPUBMFCDLLITEM_EXPANDBOX >> 1) - 2;

	bool bCheck = false;

	// convert item coordinates to screen coordinates
	pt = m_loc;
	pt.y -= m_pProp->GetOrigin().y;
	nCol = m_pProp->GetOrigin().x;

	if (IsRootLevel())
		drc.SetRect(pt.x + WAFPUBMFCDLLITEM_EXPANDCOLUMN, pt.y, rc.right, pt.y + nTotal);
	else
		drc.SetRect(pt.x + WAFPUBMFCDLLITEM_EXPANDCOLUMN, pt.y, nCol, pt.y + nTotal);

	// root level items are shaded
	if (IsRootLevel())
	{
		HGDIOBJ hOld = pDC->SelectObject(GetSysColorBrush(COLOR_BTNFACE));
		pDC->PatBlt(rc.left, drc.top, rc.right - rc.left + 1, drc.Height(), PATCOPY);
		pDC->SelectObject(hOld);
	}

	// calc/draw expand box position
	if (GetChild())
	{
		m_rcExpand.left = WAFPUBMFCDLLITEM_EXPANDCOLUMN/2 - WAFPUBMFCDLLITEM_EXPANDBOXHALF;
		m_rcExpand.top = m_loc.y + ey;
		m_rcExpand.right = m_rcExpand.left + WAFPUBMFCDLLITEM_EXPANDBOX - 1;
		m_rcExpand.bottom = m_rcExpand.top + WAFPUBMFCDLLITEM_EXPANDBOX - 1;

		ir = m_rcExpand;
		ir.OffsetRect(0, -m_pProp->GetOrigin().y);
		_DrawExpand(pDC->m_hDC, ir.left, ir.top, IsExpanded(), !IsRootLevel());
	}
	else
		m_rcExpand.SetRectEmpty();

	// calc/draw check box position
	if (IsCheckBox())
	{
		bCheck = true;

		ir.left = drc.left + WAFPUBMFCDLLITEM_SPACE;
		ir.top = m_loc.y + ey;

		ir.right = ir.left + WAFPUBMFCDLLITEM_CHECKBOX;
		ir.bottom = ir.top + WAFPUBMFCDLLITEM_CHECKBOX;

		m_rcCheckbox = ir;
	}
	else
		m_rcCheckbox.SetRectEmpty();

	HRGN hRgn = NULL;

	// create a clipping region for the label
	if (!IsRootLevel())
	{
		hRgn = CreateRectRgn(drc.left, drc.top, drc.right, drc.bottom);
		SelectClipRgn(pDC->m_hDC, hRgn);
	}

	// calc label position
	ir = drc;
	ir.left += WAFPUBMFCDLLITEM_SPACE;

	// offset the label text if item has a check box
	if (bCheck)
		OffsetRect(&ir, WAFPUBMFCDLLITEM_CHECKBOX + WAFPUBMFCDLLITEM_SPACE * 2, 0);

	// draw label
	if (!m_sLabel.IsEmpty())
	{
		if (IsRootLevel())
			pDC->SelectObject(CPropTree::GetBoldFont());
		else
			pDC->SelectObject(CPropTree::GetNormalFont());

		pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_sLabel, &ir, DT_SINGLELINE|DT_VCENTER|DT_CALCRECT);

		// draw the text highlighted if selected
		if (IsSelected())
		{
			HGDIOBJ oPen = pDC->SelectObject(GetStockObject(NULL_PEN));
			HGDIOBJ oBrush = pDC->SelectObject(GetSysColorBrush(COLOR_HIGHLIGHT));
			
			CRect dr;
			dr = drc;
			dr.left = WAFPUBMFCDLLITEM_EXPANDCOLUMN;
			
			pDC->Rectangle(&dr);
			
			pDC->SelectObject(oPen);
			pDC->SelectObject(oBrush);

			pDC->SetTextColor(GetSysColor(COLOR_BTNHIGHLIGHT));
		}

		// check if we need to draw the text as disabled
		if (!m_pProp->IsWindowEnabled())
			pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));

		pDC->DrawText(m_sLabel, &ir, DT_SINGLELINE|DT_VCENTER);
	}

	// draw check box frame
	if (IsCheckBox())
	{
		ir = m_rcCheckbox;
		ir.OffsetRect(0, -m_pProp->GetOrigin().y);
		pDC->DrawFrameControl(&ir, DFC_BUTTON, DFCS_BUTTONCHECK | (IsChecked() ? DFCS_CHECKED : 0));
	}

	// remove clip region
	if (hRgn)
	{
		SelectClipRgn(pDC->m_hDC, NULL);
		DeleteObject(hRgn);
	}

	// draw horzontal sep
	_DotHLine(pDC->m_hDC, WAFPUBMFCDLLITEM_EXPANDCOLUMN, pt.y + nTotal - 1, rc.right - WAFPUBMFCDLLITEM_EXPANDCOLUMN + 1);

	// draw separators
	if (!IsRootLevel())
	{
		// column sep
		CPen pn1(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
		CPen* pOld;

		pOld = pDC->SelectObject(&pn1);
		pDC->MoveTo(nCol, drc.top);
		pDC->LineTo(nCol, drc.bottom);

		CPen pn2(PS_SOLID, 1, GetSysColor(COLOR_BTNHIGHLIGHT));
		pDC->SelectObject(&pn2);
		pDC->MoveTo(nCol + 1, drc.top);
		pDC->LineTo(nCol + 1, drc.bottom);

		pDC->SelectObject(pOld);
	}

	// draw attribute
	if (!IsRootLevel())
	{
		// create clip region
		hRgn = CreateRectRgn(m_rc.left, m_rc.top, m_rc.right, m_rc.bottom);
		SelectClipRgn(pDC->m_hDC, hRgn);
		
		DrawAttribute(pDC, m_rc);

		SelectClipRgn(pDC->m_hDC, NULL);
		DeleteObject(hRgn);
	}

	// draw children
	if (GetChild() && IsExpanded())
	{
		y += nTotal;

		CPropTreelItem* pNext;

		for (pNext = GetChild(); pNext; pNext = pNext->GetSibling())
		{
			LONG nHeight = pNext->DrawItem(pDC, rc, x + (IsRootLevel() ? 0 : PNINDENT), y);
			nTotal += nHeight;
			y += nHeight;
		}
	}

	return nTotal;
}


void CPropTreelItem::DrawAttribute(CDC*, const RECT&)
{
	// no attributes are assocatied with this type
}
