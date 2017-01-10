#pragma once

#define MSG_SETTING	(WM_USER + 4)

class CSettingS
{
	HWND m_hWnd;
	int m_nItemHover;
	int m_nItemDown;
	int m_nState;

	CRect m_rcTitle;
	CRect m_rcLineTop;

	int m_nWidth;
	int m_nHeight;
	int m_nButtomSize;


	Image *m_pImage;
	CDC m_memBackDC;
	CBitmap m_memBackBit;
	bool m_bDrawBackDC;

public:
	CSettingS();
	~CSettingS(void);

	void SetInfo(HWND hWnd, Image *pImage, int nWidth, int nHeight);
	void Draw(CDC &dc, CRect rcUpdate);

	bool OnMouseMove(UINT nFlags, CPoint point);
	bool OnLButtonDown(UINT nFlags, CPoint point);
	bool OnLButtonUp(UINT nFlags, CPoint point);
	bool OnTimer(UINT_PTR nIDEvent);
};
