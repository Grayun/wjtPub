
// SnakeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Snake.h"
#include "SnakeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSnakeDlg 对话框

CSnakeDlg::CSnakeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSnakeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_point.SetPoint(0, 0);
	m_bLButtonDown = FALSE;
	m_rcLastUpdate.SetRectEmpty();
	m_bTracking = FALSE;
	m_bSetCapture = FALSE;
	m_bDrawBackDC = false;
	
	m_nWidth = 320;
	m_nHeight = 512;

 	//m_nWidth = 480;
 	//m_nHeight = 800;

// 	m_nWidth = 480;
// 	m_nHeight = 854;

	//m_nWidth = 800;
	//m_nHeight = 1280;

	ImageFromIDResource(IDB_PNG_IMAGE, TEXT("png"), m_pImage);

	m_nType = 0;

	TCHAR szPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szPath);
	CString strPath = szPath;
	strPath += TEXT("\\SnakeConfig.ini");
	m_nWidth = GetPrivateProfileInt("Config","WindowWidth",320,strPath);
	if(m_nWidth < 16 || m_nWidth > 16000)
	{
		m_nWidth = 320;
	}
	m_nHeight = GetPrivateProfileInt("Config","WindowHeight",512,strPath);
	if(m_nHeight < 8 || m_nHeight > 8000)
	{
		m_nHeight = m_nWidth * 16 / 8;
	}
	m_nStartLevel = GetPrivateProfileInt("Config","StartLevel",1,strPath);
	if(m_nStartLevel < 1 || m_nStartLevel > 9)
	{
		m_nStartLevel = 1;
	}
	m_nSingleTime = GetPrivateProfileInt("Config","SingleTime",75,strPath);
	if(m_nSingleTime < 10 || m_nSingleTime > 10000)
	{
		m_nStartLevel = 75;
	}
}

void CSnakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnakeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_NCHITTEST()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_TIMER()

	ON_MESSAGE(MSG_MENU,OnMessageMenu)
	ON_MESSAGE(MSG_GAME,OnMessageGame)
	ON_MESSAGE(MSG_TOP,OnMessageTop)
	ON_MESSAGE(MSG_SETTING,OnMessageSetting)
	ON_MESSAGE(MSG_ABOUT,OnMessageAbout)

END_MESSAGE_MAP()


// CSnakeDlg 消息处理程序

BOOL CSnakeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_menu.SetInfo(GetSafeHwnd(), m_pImage, m_nWidth, m_nHeight);
	m_game.SetInfo(GetSafeHwnd(), m_pImage, m_nWidth, m_nHeight, m_nStartLevel, m_nSingleTime);
	m_top.SetInfo(GetSafeHwnd(), m_pImage, m_nWidth, m_nHeight);
	m_setting.SetInfo(GetSafeHwnd(), m_pImage, m_nWidth, m_nHeight);
	m_About.SetInfo(GetSafeHwnd(), m_pImage, m_nWidth, m_nHeight);

	SetWindowPos(NULL, 0, 0, m_nWidth, m_nHeight, SWP_NOMOVE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSnakeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSnakeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rcClient, rcUpdate;
		GetClientRect(&rcClient);
		GetUpdateRect(&rcUpdate);

		CPaintDC dc(this);

		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap memBitmap;
		memBitmap.CreateCompatibleBitmap(&dc, m_nWidth, m_nHeight);
		CBitmap *pOldBitmap = MemDC.SelectObject(&memBitmap);

		if(!m_bDrawBackDC)
		{
			m_bDrawBackDC = true;

			m_memBackDC.CreateCompatibleDC(&dc);
			m_memBackBit.CreateCompatibleBitmap(&dc, m_nWidth, m_nHeight);
			CBitmap *pOldBitmap = m_memBackDC.SelectObject(&m_memBackBit);

			Graphics graphics(m_memBackDC);

			//背景
			int nWidth = 200;
			int nHeigth = 200;

			for (int i = 0; i < m_nHeight; i += 200)
			{
				nHeigth = __min(200, m_nHeight - i);
				for (int j = 0; j < m_nWidth; j += 200)
				{
					nWidth = __min(200, m_nWidth - j);

					graphics.DrawImage(m_pImage, Rect(j, i, nWidth, nHeigth),
						820, 870, nWidth, nHeigth, UnitPixel);
				}			
			}
		}
		MemDC.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &m_memBackDC, 0, 0, SRCCOPY);

		if(m_nType == 0)
		{
			m_menu.Draw(MemDC, rcUpdate);
		}
		else if(m_nType == 1)
		{
			m_game.Draw(MemDC, rcUpdate);
		}
		else if(m_nType == 2)
		{
			m_top.Draw(MemDC, rcUpdate);
		}
		else if(m_nType == 3)
		{
			m_setting.Draw(MemDC, rcUpdate);
		}
		else if(m_nType == 4)
		{
			m_About.Draw(MemDC, rcUpdate);
		}

		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(&pOldBitmap);
		MemDC.DeleteDC();

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSnakeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CSnakeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 设置窗口风格
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE)
		| WS_MAXIMIZEBOX | WS_MINIMIZEBOX
		| WS_SYSMENU | WS_SIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	dwStyle &= ~(WS_CAPTION|WS_THICKFRAME);

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	return CDialog::OnCreate(lpCreateStruct);
}

BOOL CSnakeDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

LRESULT CSnakeDlg::OnNcHitTest(CPoint point)
{
	return	HTCLIENT;
}

void CSnakeDlg::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnNcPaint()
}

BOOL CSnakeDlg::OnNcActivate(BOOL bActive)
{
	if (!bActive)
	{
		if (m_bSetCapture)
		{
			ReleaseCapture();
			m_bSetCapture = false;
		}
		m_bLButtonDown = FALSE;
		m_point.SetPoint(0, 0);
		if(!m_rcLastUpdate.IsRectEmpty())
		{
			InvalidateRect(&m_rcLastUpdate);
			m_rcLastUpdate.SetRectEmpty();
		}
	}

	return TRUE;
}

// 加载图片
bool CSnakeDlg::ImageFromIDResource(UINT nID, CString strType, Image * & pImg)  
{  
	HINSTANCE hInst = AfxGetResourceHandle();  
	HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),strType);
	if (!hRsrc)  
	{
		return FALSE; 
	}

	DWORD len = SizeofResource(hInst, hRsrc);  
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);  
	if (!lpRsrc)  
	{
		return FALSE;
	}
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);  
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);  
	memcpy(pmem,lpRsrc,len);  
	IStream* pstm;  
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);  
	pImg=Gdiplus::Image::FromStream(pstm);  
	GlobalUnlock(m_hMem);  
	GlobalFree(m_hMem);
	pstm->Release();  
	FreeResource(lpRsrc);  
	return TRUE;  
} 



BOOL CSnakeDlg::UpdateControl()
{
	CRect rcLastUpdate = m_rcLastUpdate;


	if(!rcLastUpdate.IsRectEmpty())
	{
		InvalidateRect(rcLastUpdate);
	}
	return !m_rcLastUpdate.IsRectEmpty();
}

void CSnakeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;//HOVER_DEFAULT;
		m_bTracking = ::_TrackMouseEvent(&tme);
	}
	
	m_point = point;

	if(!m_bLButtonDown)
	{		
		if(m_nType == 0)
		{
			m_menu.OnMouseMove(nFlags, point);
		}
		else if(m_nType == 1)
		{
			m_game.OnMouseMove(nFlags, point);
		}
		else if(m_nType == 2)
		{
			m_top.OnMouseMove(nFlags, point);
		}
		else if(m_nType == 3)
		{
			m_setting.OnMouseMove(nFlags, point);
		}
		else if(m_nType == 4)
		{
			m_About.OnMouseMove(nFlags, point);
		}
	}
}

void CSnakeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_point = point;
	m_bLButtonDown = TRUE;

	bool bSetCapture = false;

	if(m_nType == 0)
	{
		bSetCapture = m_menu.OnLButtonDown(nFlags, point);
	}
	else if(m_nType == 1)
	{
		bSetCapture = m_game.OnLButtonDown(nFlags, point);
	}
	else if(m_nType == 2)
	{
		bSetCapture = m_top.OnLButtonDown(nFlags, point);
	}
	else if(m_nType == 3)
	{
		bSetCapture = m_setting.OnLButtonDown(nFlags, point);
	}
	else if(m_nType == 4)
	{
		bSetCapture = m_About.OnLButtonDown(nFlags, point);
	}

	if(bSetCapture)
	{
		if (!m_bSetCapture)
		{
			SetCapture();
			m_bSetCapture = TRUE;		
		}	
	}
	else
	{		
		m_bLButtonDown = FALSE;
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	}	

	InvalidateRect(NULL);
}

void CSnakeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bSetCapture)
	{
		ReleaseCapture();
		m_bSetCapture = false;
	}

	m_point = point;
	m_bLButtonDown = FALSE;

	if(m_nType == 0)
	{
		m_menu.OnLButtonUp(nFlags, point);
	}
	else if(m_nType == 1)
	{
		m_game.OnLButtonUp(nFlags, point);
	}
	else if(m_nType == 2)
	{
		m_top.OnLButtonUp(nFlags, point);
	}
	else if(m_nType == 3)
	{
		m_setting.OnLButtonUp(nFlags, point);
	}
	else if(m_nType == 4)
	{
		m_About.OnLButtonUp(nFlags, point);
	}


	InvalidateRect(NULL);
}

void CSnakeDlg::OnMouseLeave()
{
	m_bTracking = FALSE;
	m_point.SetPoint(-1, -1);
	m_bLButtonDown = FALSE;
	m_rcLastUpdate.SetRectEmpty();

	bool bUpdate = false;

	if(m_nType == 0)
	{
		bUpdate = m_menu.OnMouseMove(0, m_point);
	}
	else if(m_nType == 1)
	{
		bUpdate = m_game.OnMouseMove(0, m_point);
	}
	else if(m_nType == 2)
	{
		bUpdate = m_top.OnMouseMove(0, m_point);
	}
	else if(m_nType == 3)
	{
		bUpdate = m_setting.OnMouseMove(0, m_point);
	}
	else if(m_nType == 4)
	{
		bUpdate = m_About.OnMouseMove(0, m_point);
	}

	if(bUpdate)
	{
		InvalidateRect(NULL);
	}

	CDialog::OnMouseLeave();
}

void CSnakeDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnMouseHover(nFlags, point);
}

void CSnakeDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(m_nType == 1)
	{
		m_game.OnTimer(nIDEvent);
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CSnakeDlg::OnMessageMenu(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	if(0 == wParam) //新游戏
	{
		TCHAR szPath[MAX_PATH] = {0};
		GetCurrentDirectory(MAX_PATH, szPath);
		CString strPath = szPath;
		strPath += TEXT("\\SnakeConfig.ini");
		m_nStartLevel = GetPrivateProfileInt("Config","StartLevel",1,strPath);
		if(m_nStartLevel < 1 || m_nStartLevel > 9)
		{
			m_nStartLevel = 1;
		}
		m_nSingleTime = GetPrivateProfileInt("Config","SingleTime",75,strPath);
		if(m_nSingleTime < 10 || m_nSingleTime > 10000)
		{
			m_nSingleTime = 75;
		}

		m_game.SetInfo(GetSafeHwnd(), m_pImage, m_nWidth, m_nHeight, m_nStartLevel, m_nSingleTime);
		SetTimer(1, m_nSingleTime, NULL);
		m_nType = 1;
		InvalidateRect(NULL);
	}
	else if(1 == wParam)  //继续
	{
		SetTimer(1, m_nSingleTime, NULL);
		m_nType = 1;
		InvalidateRect(NULL);
	}
	else if(2 == wParam)  //排行榜
	{
		m_nType = 2;
		InvalidateRect(NULL);
	}
	else if(3 == wParam)  //设置
	{
		m_nType = 3;
		InvalidateRect(NULL);
	}
	else if(4 == wParam)  //关于
	{
		m_nType = 4;
		InvalidateRect(NULL);
	}
	else if(5 == wParam)  //退出
	{
		OnOK();
	}
	return 0;
}

LRESULT CSnakeDlg::OnMessageGame(WPARAM wParam, LPARAM lParam)
{
	//暂停
	if(0 == wParam)
	{
		KillTimer(1);
		m_nType = 0;
		InvalidateRect(NULL);
	}
	else
	{
		m_game.OnKeyDown(wParam);
	}

	return 0;
}

LRESULT CSnakeDlg::OnMessageTop(WPARAM wParam, LPARAM lParam)
{
	//返回
	if(0 == wParam)
	{
		m_nType = 0;
		InvalidateRect(NULL);
	}

	return 0;
}

LRESULT CSnakeDlg::OnMessageSetting(WPARAM wParam, LPARAM lParam)
{
	//返回
	if(0 == wParam)
	{
		m_nType = 0;
		InvalidateRect(NULL);
	}

	return 0;
}

LRESULT CSnakeDlg::OnMessageAbout(WPARAM wParam, LPARAM lParam)
{
	//返回
	if(0 == wParam)
	{
		m_nType = 0;
		InvalidateRect(NULL);
	}

	return 0;
}

BOOL CSnakeDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(m_nType == 1)
		{
			m_game.OnKeyDown(pMsg->wParam);
		}
	}
	else if(pMsg->message==WM_KEYUP)
	{
		if(m_nType == 1)
		{
			m_game.OnKeyUp(pMsg->wParam);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
