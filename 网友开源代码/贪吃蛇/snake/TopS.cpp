#include "StdAfx.h"
#include "TopS.h"

CTopS::CTopS()
{	
	m_bDrawBackDC = false;	
	m_hWnd = NULL;

	m_nItemHover = -1;
	m_nItemDown = -1;
	m_nState = 0;
}

CTopS::~CTopS(void)
{
}

void CTopS::SetInfo(HWND hWnd, Image *pImage, int nWidth, int nHeight)
{
	m_hWnd = hWnd;
	m_pImage = pImage;

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_nButtomSize = m_nWidth * 3 / 20;
	int nButtomFrame = m_nButtomSize / 2;

	m_rcLineTop.SetRect(0, m_nButtomSize, m_nWidth, m_nButtomSize + m_nWidth * 30 / 800);
	m_rcTitle.SetRect(0, 0, m_nWidth / 2, 0 + m_nHeight * 170 / 1280);
}

void CTopS::Draw(CDC &dc, CRect rcUpdate)
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

		//上线
		graphics.DrawImage(m_pImage, Rect(m_rcLineTop.left, m_rcLineTop.top, m_rcLineTop.Width(), m_rcLineTop.Height()), 0, 275, 800, 30, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcLineTop.left + m_nWidth, m_rcLineTop.top, m_rcLineTop.Width(), m_rcLineTop.Height()), 0, 275, 800, 30, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcLineTop.left + m_nWidth * 2, m_rcLineTop.top, m_rcLineTop.Width(), m_rcLineTop.Height()), 0, 275, 800, 30, UnitPixel);

		//标题
		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 820, 240, 400, 170, UnitPixel);

		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left + m_nWidth, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 820, 240, 400, 170, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left + m_nWidth, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 0, 1562, 420, 170, UnitPixel);
		
		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left + m_nWidth * 2, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 820, 240, 400, 170, UnitPixel);
		graphics.DrawImage(m_pImage, Rect(m_rcTitle.left + m_nWidth * 2, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height()), 420, 1562, 420, 170, UnitPixel);
	}

	dc.BitBlt(0, 0, m_nWidth, m_nHeight, &m_memBackDC, 0, 0, SRCCOPY);

	if(m_nItemHover == 0)
	{
		dc.BitBlt(m_rcTitle.left, m_rcTitle.top, m_rcTitle.Width(), m_rcTitle.Height(), 
			&m_memBackDC, m_rcTitle.left + m_nWidth * m_nState, m_rcTitle.top, SRCCOPY);
	}

}

bool CTopS::OnMouseMove(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rcTitle.PtInRect(point))
	{
		m_nItemHover = 0;
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

bool CTopS::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rcTitle.PtInRect(point))
	{
		m_nItemHover = 0;
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

bool CTopS::OnLButtonUp(UINT nFlags, CPoint point)
{
	int nOldItemHover = m_nItemHover;
	if(m_rcTitle.PtInRect(point))
	{
		m_nItemHover = 0;
		m_nState = 1;
	}
	else
	{
		m_nItemHover = -1;
		m_nState = 0;
	}

	if(m_nItemHover != -1 && m_nItemDown == m_nItemHover)
	{
		::SendMessage(m_hWnd, MSG_TOP, m_nItemDown, 0);
	}

	if(nOldItemHover != m_nItemHover)
	{
		::InvalidateRect(m_hWnd, NULL, true);
	}

	m_nItemDown = -1;

	return false;
}

bool CTopS::OnTimer(UINT_PTR nIDEvent)
{
	return false;
}
