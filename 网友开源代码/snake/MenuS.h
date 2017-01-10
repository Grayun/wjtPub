#pragma once

#define MSG_MENU	(WM_USER + 1)

class CMenuS
{
	HWND m_hWnd;
	int m_nItemHover;
	int m_nItemDown;
	int m_nState;
	CRect m_rc;
	int m_nWidth;
	int m_nHeight;
	int m_nItemHeigt;
	Image *m_pImage;
	CDC m_memBackDC;
	CBitmap m_memBackBit;
	bool m_bDrawBackDC;

public:
	CMenuS();
	~CMenuS(void);

	void SetInfo(HWND hWnd, Image *pImage, int nWidth, int nHeight);
	void Draw(CDC &dc, CRect rcUpdate);

	bool OnMouseMove(UINT nFlags, CPoint point);
	bool OnLButtonDown(UINT nFlags, CPoint point);
	bool OnLButtonUp(UINT nFlags, CPoint point);
	bool OnTimer(UINT_PTR nIDEvent);
};
