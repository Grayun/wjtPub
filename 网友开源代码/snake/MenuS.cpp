#include "StdAfx.h"
#include "MenuS.h"

CMenuS::CMenuS()
{	
	m_bDrawBackDC = false;	
	m_hWnd = NULL;

	m_nItemHover = -1;
	m_nItemDown = -1;
	m_nState = 0;

	m_nItemHeigt = 170;
}

CMenuS::~CMenuS(void)
{
}

void CMenuS::SetInfo(HWND hWnd, Image *pImage, int nWidth, int nHeight)
{
	m_hWnd = hWnd;
	m_pImage = pImage;

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_rc.SetRect(nWidth * 9 / 80, nHeight * 14 / 64, nWidth - nWidth * 9 / 80, nHeight - nHeight * 1 / 64);

	m_nItemHeigt = m_rc.Height() / 6;
}

void CMenuS::Draw(CDC &dc, CRect rcUpdate)
{
	if(!m_bDrawBackDC)
	{
		m_bDrawBackDC = true;

		m_memBackDC.CreateCompatibleDC(&dc);
		m_memBackBit.CreateCompatibleBitmap(&dc, m_nWidth * 3, m_nHeight);
		CBitmap *pOldBitmap = m_memBackDC.SelectObject(&m_memBackBit);

		m_memBackDC.BitBlt(0, 0, m_nWidth, m_nHeight, &dc, 0, 0, SRCCOPY);
		m_memBackDC.BitBlt(m_nWidth, 0, m_nWidth, m_nHeight, &dc, 0, 0, SRCCOPY);
		m_memBackDC.BitBlt(m_nWidth * 2, 0, m_nWidth, m_nHeight, &dc, 0, 0, SRCCOPY);


		Graphics graphics(m_memBackDC);
		
		graphics.DrawImage(m_pImage, Rect(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height()), 0, 305, 420, 1020, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_nWidth / 10, m_rc.Height() / 16, m_nWidth * 8 / 10, m_rc.Height() * 3 / 16), 0, 1732, 700, 240, UnitPixel);

		graphics.DrawImage(m_pImage, Rect(m_rc.left + m_nWidth, m_rc.top, m_rc.Width(), m_rc.Height()), 0, 305, 420, 1020, UnitPixel);
		for(int i = 0; i < 6; i++)
		{
			graphics.DrawImage(m_pImage, Rect(m_rc.left + m_nWidth, m_rc.top + m_nItemHeigt * i, m_rc.Width(), m_nItemHeigt), 0, 1562, 420, 170, UnitPixel);
		}

		graphics.DrawImage(m_pImage, Rect(m_rc.left + m_nWidth * 2, m_rc.top, m_rc.Width(), m_rc.Height()), 0, 305, 420, 1020, UnitPixel);
		for(int i = 0; i < 6; i++)
		{
			graphics.DrawImage(m_pImage, Rect(m_rc.left + m_nWidth * 2, m_rc.top + m_nItemHeigt * i, m_rc.Width(), m_nItemHeigt), 420, 1562, 420, 170, UnitPixel);
		}

	}
	dc.BitBlt(0, 0, m_nWidth, m_nHeight, &m_memBackDC, 0, 0, SRCCOPY);

	if(m_nItemHover >= 0 && m_nItemHover <= 5)
	{
		dc.BitBlt(m_rc.left, m_rc.top + m_nItemHeigt * m_nItemHover, m_rc.Width(), m_nItemHeigt, 
			&m_memBackDC, m_rc.left + m_nWidth * m_nState, m_rc.top + m_nItemHeigt * m_nItemHover, SRCCOPY);
	}

}

bool CMenuS::OnMouseMove(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rc.PtInRect(point))
	{
		m_nItemHover = (point.y - m_rc.top) / m_nItemHeigt;
		m_nState = 1;
	}
	else
	{
		m_nItemHover = -1;
		m_nState = 0;
	}

	if(nOldItemHover != m_nItemHover)
	{
		::InvalidateRect(m_hWnd, NULL, true);
	}
	return false;
}

bool CMenuS::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rc.PtInRect(point))
	{
		m_nItemHover = (point.y - m_rc.top) / m_nItemHeigt;
		m_nState = 2;
		m_nItemDown = m_nItemHover;
	}
	else
	{
		m_nItemHover = -1;
		m_nState = 0;
	}

	if(nOldItemHover != m_nItemHover)
	{
		::InvalidateRect(m_hWnd, NULL, true);
	}

	return m_nItemHover != -1;
}

bool CMenuS::OnLButtonUp(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rc.PtInRect(point))
	{
		m_nItemHover = (point.y - m_rc.top) / m_nItemHeigt;
		m_nState = 1;
	}
	else
	{
		m_nItemHover = -1;
		m_nState = 0;
	}

	if(m_nItemHover != -1 && m_nItemDown == m_nItemHover)
	{
		::SendMessage(m_hWnd, MSG_MENU, m_nItemDown, 0);
	}

	if(nOldItemHover != m_nItemHover)
	{
		::InvalidateRect(m_hWnd, NULL, true);
	}

	m_nItemDown = -1;

	return false;
}

bool CMenuS::OnTimer(UINT_PTR nIDEvent)
{
	return false;
}
