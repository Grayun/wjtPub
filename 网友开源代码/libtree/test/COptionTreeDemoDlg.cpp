// COptionTree
//
// License
// -------
// This code is provided "as is" with no expressed or implied warranty.
// 
// You may use this code in a commercial product with or without acknowledgement.
// However you may not sell this code or any modification of this code, this includes 
// commercial libraries and anything else for profit.
//
// I would appreciate a notification of any bugs or bug fixes to help the control grow.
//
// History:
// --------
//	See License.txt for full history information.
//
//
// Copyright (c) 1999-2002 
// ComputerSmarts.net 
// mattrmiller@computersmarts.net

#include "stdafx.h"
#include "COptionTreeDemo.h"
#include ".\coptiontreedemodlg.h"
#include <src\precisetimer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_BT_CLICKED (WM_USER+1)


	// Declare variables
	COptionTreeItem *otiBase = NULL;
  COptionTreeItem *otiRoot = NULL;
	COptionTreeItem *otiItem = NULL;
	COptionTreeItemStatic *otiStatic = NULL;
	COptionTreeItemEdit *otiEdit = NULL;
	COptionTreeItemComboBox *otiCombo = NULL;
  COptionTreeItemListBox *otiList = NULL;
	COptionTreeItemCheckBox *otiCheck = NULL;
	COptionTreeItemRadio *otiRadio = NULL;
	COptionTreeItemSpinner *otiSpinner = NULL;
	COptionTreeItemColor *otiColor = NULL;
	COptionTreeItemDate *otiDate = NULL;
	COptionTreeItemImage *otiImage = NULL;
	COptionTreeItemFont *otiFont = NULL;
	COptionTreeItemFile *otiFile = NULL;
	COptionTreeItemIPAddress *otiIPAddress = NULL;
	COptionTreeItemHyperLink *oti_Hyperlink = NULL;
  COptionTreeItemEditButton *oti_editbt = NULL;
	CRect rcClient;
	DWORD dwStyle, dwOptions;
	LOGFONT lfFont, lfDefaultFont;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOptionTreeDemoDlg dialog

CCOptionTreeDemoDlg::CCOptionTreeDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCOptionTreeDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOptionTreeDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_uApplyFontID = 0;
  _pSearchOffset = NULL;
  m_bMindAttribute = FALSE;
  _Text = "";
  _SearchText = "Font Item # 1518";
}

void CCOptionTreeDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_ED_INFO, _Text);
  DDX_Text(pDX, IDC_ED_SEARCH_ITEM, _SearchText);

	//{{AFX_DATA_MAP(CCOptionTreeDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCOptionTreeDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CCOptionTreeDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_MESSAGE(OT_FS_NOTIFY_APPLY, WM_ApplyButton)
	ON_NOTIFY(OT_NOTIFY_ITEMCHANGED, IDC_OPTIONSTREE_OPTIONS, OnTreeItemChanged)
  ON_NOTIFY(OT_NOTIFY_SELCHANGE, IDC_OPTIONSTREE_OPTIONS, OnTreeItemClicked)
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BT_TEST, OnBnClickedBtTest)
  ON_BN_CLICKED(IDC_BT_EXPANDALL, OnBnClickedBtExpandall)
  ON_BN_CLICKED(IDC_BT_CLEAR_INFO, OnBnClickedBtClearInfo)
  ON_BN_CLICKED(IDC_BT_SEARCH_ITEM, OnBnClickedBtSearchItem)
  ON_BN_CLICKED(IDC_BT_RESET_SEARCH, OnBnClickedBtResetSearch)
  ON_BN_CLICKED(IDC_CB_MIND_ATTRIBUTE, OnBnClickedCbMindAttribute)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOptionTreeDemoDlg message handlers

BOOL CCOptionTreeDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Get log fonts
	GetFont()->GetLogFont(&lfFont);
	GetFont()->GetLogFont(&lfDefaultFont);
	strcpy(lfFont.lfFaceName, _T("Arial"));

  ///
  /// ======= Tree Creation =======
  ///
	// Get the clients rectangle
	GetClientRect(rcClient);
	rcClient.top += 46;
  rcClient.bottom -= 23;

	// Setup the window style
	dwStyle = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	// Setup the tree options 
	// OT_OPTIONS_SHOWINFOWINDOW
	dwOptions = OT_OPTIONS_SHADEEXPANDCOLUMN | OT_OPTIONS_SHADEROOTITEMS | OT_OPTIONS_SHOWINFOWINDOW;

	// Create tree options
	if (m_otTree.Create(dwStyle, rcClient, this, dwOptions, IDC_OPTIONSTREE_OPTIONS) == FALSE)
	{
		TRACE0("Failed to create options control.\r\n");
		return FALSE;
	}
	// Want to be notified
	m_otTree.SetNotify(TRUE, this,WM_BT_CLICKED,false);
  m_otTree.SetAutoSizeColumn(false);
  m_otTree.SetAttributeOffset(4);
  m_otTree.LockAll();

	// Setup options tree
	// -- Files and Folders -> File Full Path

  // -- Sub Item Test
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Item Test"));
	otiRoot->SetInfoText(_T("This is the info text for this item.\r\nNo one expects the\r\nSpanish\r\nInquisition"));
  otiRoot->AddButton(_D_BT_PLUS|_D_BT_MINUS);
  otiBase = otiRoot;

  // -- Sub Item Test -> Sub Item 1
	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
	otiStatic->SetLabelText(_T("Sub Item (My Subs are dragable)"));
	otiStatic->SetInfoText(_T("Sub item 1."));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}
  otiStatic->SetAllowChildDrag(true);

  COptionTreeItemStatic* pParent = otiStatic;
  // so the subsequent icons of 1. sublevel can be reordered by drag&drop
	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.1"));
	otiStatic->SetInfoText(_T("Sub item 2.1"));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}

	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.2 - Larger Item to demonstrate autosize"));
	otiStatic->SetInfoText(_T("Sub item 2.2"));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}

	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.3"));
	otiStatic->SetInfoText(_T("Sub item 2.3"));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}
  otiStatic->AddButton(_D_BT_DEFAULT);  

	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.4"));
	otiStatic->SetInfoText(_T("Sub item 2.4"));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}
  otiStatic->AddButton(_D_BT_PLUS);  

  otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.5"));
	otiStatic->SetInfoText(_T("Sub item 2.5"));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}
  otiStatic->AddButton(_D_BT_ALL,"User 1");  
  // just test if deletion works
  otiStatic->DeleteButton(_D_BT_PLUS);  
  

  otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.6"));
	otiStatic->SetInfoText(_T("Sub item 2.6"));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}
  otiStatic->AddButton(_D_BT_ALL,"User 2");  

  // -- Edit Items -> Edit Item
	otiEdit = (COptionTreeItemEdit*)m_otTree.InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEdit->SetLabelText(_T("Edit Item"));
	otiEdit->SetInfoText(_T("This is an example of a edit item."));
	if (otiEdit->CreateEditItem(NULL, 0) == TRUE)
	{
		otiEdit->SetWindowText(_T("I am a edit item."));	
	}
	// -- Edit Items -> Multiline
	otiEdit = (COptionTreeItemEdit*) m_otTree.InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEdit->SetLabelText(_T("Multiline"));
	otiEdit->SetInfoText(_T("This is an example of a multiline edit item."));
	if (otiEdit->CreateEditItem(OT_EDIT_MULTILINE, ES_WANTRETURN | ES_AUTOVSCROLL) == TRUE)
	{
		otiEdit->SetWindowText(_T("I am a multiline edit item.\r\nHere is my second line."));
	}
  //otiStatic->AddButton(_D_BT_ALL);


  // -- Sub Item Test -> Sub Item 2
	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStatic);
	otiStatic->SetLabelText(_T("Sub Item 2"));
	otiStatic->SetInfoText(_T("Sub item 2."));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}
  otiStatic->AddButton();


  pParent->SetAllowChildDrag(true);
	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.7"));
	otiStatic->SetInfoText(_T("Sub item 2.7"));
	otiStatic->SetStaticText(_T("I am a sub item."));

	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.8"));
	otiStatic->SetInfoText(_T("Sub item 2.8"));
	otiStatic->SetStaticText(_T("I am a sub item."));

	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.9"));
	otiStatic->SetInfoText(_T("Sub item 2.9"));
	otiStatic->SetStaticText(_T("I am a sub item."));

	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), pParent);
	otiStatic->SetLabelText(_T("Sub Item 2.10"));
	otiStatic->SetInfoText(_T("Sub item 2.10"));
	otiStatic->SetStaticText(_T("I am a sub item."));


	// -- Sub Item Test -> Sub Item 3
	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStatic);
	otiStatic->SetLabelText(_T("Sub Item 3"));
	otiStatic->SetInfoText(_T("Sub item 3."));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}
	// -- Sub Item Test -> Sub Item 4
	otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiStatic);
	otiStatic->SetLabelText(_T("Sub Item 4"));
	otiStatic->SetInfoText(_T("Sub item 4."));
	otiStatic->SetStaticText(_T("I am a sub item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}

  // -- Style Test
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Style Test"));
	otiRoot->SetInfoText(_T("This is an example of numerous styles."));
	otiRoot->SetRootBackgroundColor(RGB(0, 150, 200));
	otiRoot->SetLabelTextColor(RGB(0, 0, 0));
  otiRoot->AddButton();
  
  // -- Style Test-> Label Text Color
	otiStatic = (COptionTreeItemStatic*) m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
	otiStatic->SetLabelText(_T("Label Text Color"));
	otiStatic->SetInfoText(_T("This is an example of setting the label text color."));
	otiStatic->SetStaticText(_T("I am a static item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
		otiStatic->SetLabelTextColor(RGB(0, 0, 255));
	}
	// -- Style Test-> Label Background Color
	otiStatic = (COptionTreeItemStatic*) m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
	otiStatic->SetLabelText(_T("Label Background Color"));
	otiStatic->SetInfoText(_T("This is an example of setting the label background color."));
	otiStatic->SetStaticText(_T("I am a static item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
		otiStatic->SetLabelBackgroundColor(RGB(0, 150, 200));
	}
	// -- Style Test-> Text Color
	otiStatic = (COptionTreeItemStatic*) m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
	otiStatic->SetLabelText(_T("Text Color"));
	otiStatic->SetInfoText(_T("This is an example of setting the text color of an item."));
	otiStatic->SetStaticText(_T("I am a static item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
		otiStatic->SetTextColor(RGB(0, 0, 255));
	}
	// -- Style Test-> Background Color
	otiStatic = (COptionTreeItemStatic*) m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
	otiStatic->SetLabelText(_T("Background Color"));
	otiStatic->SetInfoText(_T("This is an example of setting the background color of an item."));
	otiStatic->SetStaticText(_T("I am a static item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
		otiStatic->SetBackgroundColor(RGB(0, 150, 200));
	}

	// -- Static Items
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Static Items"));
	otiRoot->SetInfoText(_T("These are examples of static items."));
	// -- Static Items -> Static Item
	otiStatic = (COptionTreeItemStatic*) m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
	otiStatic->SetLabelText(_T("Static Item"));
	otiStatic->SetInfoText(_T("This is an example of a static item."));
	otiStatic->SetStaticText(_T("I am a static item."));
	if (otiStatic->CreateStaticItem(0) == TRUE)
	{
	}
	// -- Edit Items
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Edit Items"));
	otiRoot->SetInfoText(_T("These are examples of edit items."));

   
	oti_editbt = (COptionTreeItemEditButton*)m_otTree.InsertItem(new COptionTreeItemEditButton(), otiRoot);
	oti_editbt->SetLabelText(_T("Edit Item"));
	oti_editbt->SetInfoText(_T("This is an example of a edit item."));
	if (oti_editbt->CreateEditItem(NULL, NULL) == TRUE)
	{
		oti_editbt->SetWindowText(_T("edit button"));	
	}
 
  
  // -- Edit Items -> Edit Item
	otiEdit = (COptionTreeItemEdit*)m_otTree.InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEdit->SetLabelText(_T("Edit Item"));
	otiEdit->SetInfoText(_T("This is an example of a edit item."));
	if (otiEdit->CreateEditItem(NULL, 0) == TRUE)
	{
		otiEdit->SetWindowText(_T("I am a edit item."));	
	}
	// -- Edit Items -> Multiline
	otiEdit = (COptionTreeItemEdit*) m_otTree.InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEdit->SetLabelText(_T("Multiline"));
	otiEdit->SetInfoText(_T("This is an example of a multiline edit item."));
	if (otiEdit->CreateEditItem(OT_EDIT_MULTILINE, ES_WANTRETURN | ES_AUTOVSCROLL) == TRUE)
	{
		otiEdit->SetWindowText(_T("I am a multiline edit item.\r\nHere is my second line."));
	}
	// -- Edit Items -> Password
	otiEdit = (COptionTreeItemEdit*) m_otTree.InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEdit->SetLabelText(_T("Password"));
	otiEdit->SetInfoText(_T("This is an example of a password edit item."));
	if (otiEdit->CreateEditItem(OT_EDIT_PASSWORD, NULL) == TRUE)
	{
		otiEdit->SetWindowText(_T("Password"));
	}
	// -- Edit Items -> unsigned int
	otiEdit = (COptionTreeItemEdit*) m_otTree.InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEdit->SetLabelText(_T("Numerical"));
	otiEdit->SetInfoText(_T("This is an example of a unsigned int edit item."));
	if (otiEdit->CreateEditItem(OT_EDIT_NUMERICAL, NULL) == TRUE)
	{
		otiEdit->SetWindowText(_T("100"));
	}
	// -- Edit Items -> unsigned float
	otiEdit = (COptionTreeItemEdit*) m_otTree.InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEdit->SetLabelText(_T("Numerical"));
	otiEdit->SetInfoText(_T("This is an example of a unsigned float edit item."));
	if (otiEdit->CreateEditItem(OT_EDIT_NUMERICAL, NULL,FLOAT_MASK_POSITIVE) == TRUE)
	{
		otiEdit->SetWindowText(_T("3.14"));
	}
	// -- Edit Items -> signed float
	otiEdit = (COptionTreeItemEdit*) m_otTree.InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEdit->SetLabelText(_T("Numerical"));
	otiEdit->SetInfoText(_T("This is an example of a signed float edit item."));
	if (otiEdit->CreateEditItem(OT_EDIT_NUMERICAL, NULL,FLOAT_MASK) == TRUE)
	{
		otiEdit->SetWindowText(_T("-3.14"));
	}

  // -- Edit Items -> Read Only
	otiEdit = (COptionTreeItemEdit*) m_otTree.InsertItem(new COptionTreeItemEdit(), otiRoot);
	otiEdit->SetLabelText(_T("Read Only"));
	otiEdit->SetInfoText(_T("This is an example of a read only edit item."));
	if (otiEdit->CreateEditItem(NULL, NULL) == TRUE)
	{
		otiEdit->SetWindowText(_T("I am a read only edit."));
		otiEdit->ReadOnly(TRUE);
	}
	// -- Combo Box Items
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Combo Box Items"));
	otiRoot->SetInfoText(_T("These are examples of combo box items."));
	// -- Combo Box Items -> Sorted Combo Box
	otiCombo = (COptionTreeItemComboBox*)m_otTree.InsertItem(new COptionTreeItemComboBox(), otiRoot);
	otiCombo->SetLabelText(_T("Sorted"));
	otiCombo->SetInfoText(_T("This is an example of a sorted combo box item."));
	if (otiCombo->CreateComboItem(CBS_SORT) == TRUE)
	{
		otiCombo->AddString("Blue");
		otiCombo->AddString("Red");
		otiCombo->AddString("Green");
		otiCombo->AddString("Yellow");
		otiCombo->AddString("Purple");
		otiCombo->AddString("Orange");

		otiCombo->SelectString(0, "Blue");
	}

	// -- Combo Box Items -> Combo Box
	otiCombo = (COptionTreeItemComboBox*)m_otTree.InsertItem(new COptionTreeItemComboBox(), otiRoot);
	otiCombo->SetLabelText(_T("Combo Box"));
	otiCombo->SetInfoText(_T("This is an example of a combo box item."));
	if (otiCombo->CreateComboItem(NULL) == TRUE)
	{
		otiCombo->AddString("Yes");
		otiCombo->AddString("No");

		otiCombo->SelectString(0, "No");
	}
	// -- Combo Box Items -> Read Only
	otiCombo = (COptionTreeItemComboBox*)m_otTree.InsertItem(new COptionTreeItemComboBox(), otiRoot);
	otiCombo->SetLabelText(_T("Read Only"));
	otiCombo->SetInfoText(_T("This is an example of a read only combo box item."));
	if (otiCombo->CreateComboItem(NULL) == TRUE)
	{
		otiCombo->AddString("I am a read only combo box.");

		otiCombo->SelectString(0, "I am a read only combo box.");

		otiCombo->ReadOnly(TRUE);
	}
  // ce
	// -- List Box Item with user height
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("List Box Item with user height"));
	otiRoot->SetInfoText(_T("These are examples of list box item."));
	otiList = (COptionTreeItemListBox*)m_otTree.InsertItem(new COptionTreeItemListBox(), otiRoot);
	otiList->SetLabelText(_T("Listbox"));
	otiList->SetInfoText(_T("This is an example of a sorted combo box item."));
	if (otiList->CreateListItem(0,200) == TRUE)
	{
		otiList->AddString("Blue");
		otiList->AddString("Red");
		otiList->AddString("Green");
		otiList->AddString("Yellow");
		otiList->AddString("Purple");
		otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("asdasdaaaaaaaaaaaaaaaaaaaaaaaasdasdasddddddddddddddddddddddddddddddddddddddddddddddddddddddasdasd");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");

		otiList->SelectString(0, "Blue");
	}
  
	// -- List Box Item with default height
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("List Box Item with default height"));
	otiRoot->SetInfoText(_T("These are examples of list box item."));
	otiList = (COptionTreeItemListBox*)m_otTree.InsertItem(new COptionTreeItemListBox(), otiRoot);
	otiList->SetLabelText(_T("Listbox"));
	otiList->SetInfoText(_T("This is an example of a sorted combo box item."));
	if (otiList->CreateListItem(0) == TRUE)
	{
		otiList->AddString("Blue");
		otiList->AddString("Red");
		otiList->AddString("Green");
		otiList->AddString("Yellow");
		otiList->AddString("Purple");
		otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");
    otiList->AddString("Orange");

		otiList->SelectString(0, "Blue");
	}

  // -- Check Box Items
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Check Box Items"));
	otiRoot->SetInfoText(_T("These are examples of check box items."));
  otiRoot->AddButton();

	// -- Check Box Items-> Without Text
	otiCheck = (COptionTreeItemCheckBox*)m_otTree.InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiCheck->SetLabelText(_T("Without Text"));
	otiCheck->SetInfoText(_T("This is an example of a check box item without text."));
	if (otiCheck->CreateCheckBoxItem(FALSE, OT_CHECKBOX_SHOWCHECK) == TRUE)
	{
		otiCheck->SetCheckText(_T("Checked"), _T("UnChecked"));
	}
	// -- Check Box Items -> With Text
	otiCheck = (COptionTreeItemCheckBox*)m_otTree.InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiCheck->SetLabelText(_T("With Text and once again a larger label to test autosize feature"));
	otiCheck->SetInfoText(_T("This is an example of a check box item with text."));
	if (otiCheck->CreateCheckBoxItem(FALSE, OT_CHECKBOX_SHOWCHECK | OT_CHECKBOX_SHOWTEXT) == TRUE)
	{
		otiCheck->SetCheckText(_T("Checked"), _T("UnChecked"));
	}
	// -- Check Box Items -> Without Check Box
	otiCheck = (COptionTreeItemCheckBox*)m_otTree.InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiCheck->SetLabelText(_T("Without Check Box"));
	otiCheck->SetInfoText(_T("This is an example of a check box item without check box when not activated."));
	if (otiCheck->CreateCheckBoxItem(FALSE, OT_CHECKBOX_SHOWTEXT) == TRUE)
	{
		otiCheck->SetCheckText(_T("Checked"), _T("UnChecked"));
	}
	// -- Check Box Items -> Read Only
	otiCheck = (COptionTreeItemCheckBox*)m_otTree.InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	otiCheck->SetLabelText(_T("Read Only"));
	otiCheck->SetInfoText(_T("This is an example of a check box item that is read only."));
	if (otiCheck->CreateCheckBoxItem(FALSE, OT_CHECKBOX_SHOWCHECK | OT_CHECKBOX_SHOWTEXT) == TRUE)
	{
		otiCheck->SetCheckText(_T("Checked"), _T("UnChecked"));
		
		otiCheck->ReadOnly(TRUE);
	}
	// -- Radio Items
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Radio Items;radio items"));
	otiRoot->SetInfoText(_T("These are examples of radio items."));
	// -- Radio Items -> Radio Items
	otiRadio = (COptionTreeItemRadio*)m_otTree.InsertItem(new COptionTreeItemRadio(), otiRoot);
	otiRadio->SetLabelText(_T("Radio Items"));
	otiRadio->SetInfoText(_T("This is an example of a radio item."));
	if (otiRadio->CreateRadioItem() == TRUE)
	{
		otiRadio->InsertNewRadio(_T("One"), TRUE);
		otiRadio->InsertNewRadio(_T("Two"), FALSE);
		otiRadio->InsertNewRadio(_T("Three"), FALSE);
	}
  otiRadio->AddButton();

	// -- Radio Items -> Read Only
	otiRadio = (COptionTreeItemRadio*)m_otTree.InsertItem(new COptionTreeItemRadio(), otiRoot);
	otiRadio->SetLabelText(_T("Read Only"));
	otiRadio->SetInfoText(_T("This is an example of a radio item that is read only."));
	if (otiRadio->CreateRadioItem() == TRUE)
	{
		otiRadio->InsertNewRadio(_T("Cat"), FALSE);
		otiRadio->InsertNewRadio(_T("Dog"), TRUE);

		otiRadio->ReadOnly(TRUE);
	}
	// -- Spinner Items
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Spinner Items"));
	otiRoot->SetInfoText(_T("These are examples of spinner items."));
	// -- Spinner Items -> Wrap Around
	otiSpinner = (COptionTreeItemSpinner*)m_otTree.InsertItem(new COptionTreeItemSpinner(), otiRoot);
	otiSpinner->SetLabelText(_T("Wrap Around"));
	otiSpinner->SetInfoText(_T("This is an example of a spinner item that wraps around."));
	if (otiSpinner->CreateSpinnerItem(OT_EDIT_WRAPAROUND | OT_EDIT_USEREDIT, 80, 1, 100) == TRUE)
	{
	}
	// -- Spinner Items -> No User Edit
	otiSpinner = (COptionTreeItemSpinner*)m_otTree.InsertItem(new COptionTreeItemSpinner(), otiRoot);
	otiSpinner->SetLabelText(_T("No User Edit"));
	otiSpinner->SetInfoText(_T("This is an example of spinner item that disallows user edit."));
	if (otiSpinner->CreateSpinnerItem(OT_EDIT_WRAPAROUND, 80, -100, 100) == TRUE)
	{
	}
	// -- Spinner Items -> No Wrap Around
	otiSpinner = (COptionTreeItemSpinner*)m_otTree.InsertItem(new COptionTreeItemSpinner(), otiRoot);
	otiSpinner->SetLabelText(_T("No Wrap Around"));
	otiSpinner->SetInfoText(_T("This is an example of spinner item that does not wrap around."));
	if (otiSpinner->CreateSpinnerItem(OT_EDIT_USEREDIT, 80, 1, 100) == TRUE)
	{
	}
	// -- Color Items
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Color Items"));
	otiRoot->SetInfoText(_T("These are examples of color items."));
	// -- Color Items -> Hex Color
	otiColor = (COptionTreeItemColor*)m_otTree.InsertItem(new COptionTreeItemColor(), otiRoot);
	otiColor->SetLabelText(_T("Hex Color"));
	otiColor->SetInfoText(_T("This is an example of hexadecimal color item."));
	if (otiColor->CreateColorItem(OT_COLOR_SHOWHEX, RGB(0, 0, 255), RGB(0, 0, 0)) == TRUE)
	{
	}
	// -- Color Items -> RGB Color
	otiColor = (COptionTreeItemColor*)m_otTree.InsertItem(new COptionTreeItemColor(), otiRoot);
	otiColor->SetLabelText(_T("RGB Color"));
	otiColor->SetInfoText(_T("This is an example of RGB color item."));
	if (otiColor->CreateColorItem(NULL, RGB(0, 0, 255), RGB(0, 0, 0)) == TRUE)
	{
	}
	// -- Color Items -> Live Update
	otiColor = (COptionTreeItemColor*)m_otTree.InsertItem(new COptionTreeItemColor(), otiRoot);
	otiColor->SetLabelText(_T("Live Update"));
	otiColor->SetInfoText(_T("This is an example of live update color item."));
	if (otiColor->CreateColorItem(OT_COLOR_SHOWHEX | OT_COLOR_LIVEUPDATE, RGB(255, 100, 255), RGB(0, 0, 0)) == TRUE)
	{
	}
	// -- Color Items -> Read Only
	otiColor = (COptionTreeItemColor*)m_otTree.InsertItem(new COptionTreeItemColor(), otiRoot);
	otiColor->SetLabelText(_T("Read Only"));
	otiColor->SetInfoText(_T("This is an example of read only color item."));
	if (otiColor->CreateColorItem(OT_COLOR_SHOWHEX | OT_COLOR_LIVEUPDATE, RGB(130, 100, 255), RGB(0, 0, 0)) == TRUE)
	{
		otiColor->ReadOnly(TRUE);
	}
	// -- Date and Time
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Date and Time"));
	otiRoot->SetInfoText(_T("These are examples of date and time items."));
	// -- Date and Time -> Date
	otiDate = (COptionTreeItemDate*)m_otTree.InsertItem(new COptionTreeItemDate(), otiRoot);
	otiDate->SetLabelText(_T("Date"));
	otiDate->SetInfoText(_T("This is an example of a date item."));
	if (otiDate->CreateDateItem(_T("ddd, MMM d, yyyy"), DTS_LONGDATEFORMAT) == TRUE)
	{
	}
  // ce
  // -- Date and Time -> Date autoformat
	otiDate = (COptionTreeItemDate*)m_otTree.InsertItem(new COptionTreeItemDate(), otiRoot);
	otiDate->SetLabelText(_T("Date"));
	otiDate->SetInfoText(_T("This is an example of a date item with autoformat."));
	if (otiDate->CreateDateItem(_T(""), DTS_LONGDATEFORMAT) == TRUE)
	{
	}

  // -- Date and Time -> Time
	otiDate = (COptionTreeItemDate*)m_otTree.InsertItem(new COptionTreeItemDate(), otiRoot);
	otiDate->SetLabelText(_T("Time"));
	otiDate->SetInfoText(_T("This is an example of a time item."));
	if (otiDate->CreateDateItem(_T("hh:mm tt"), DTS_TIMEFORMAT | DTS_UPDOWN) == TRUE)
	{
	}
	// -- Date and Time -> Read Only
	otiDate = (COptionTreeItemDate*)m_otTree.InsertItem(new COptionTreeItemDate(), otiRoot);
	otiDate->SetLabelText(_T("Read Only"));
	otiDate->SetInfoText(_T("This is an example of a time item that is read only."));
	if (otiDate->CreateDateItem(_T("hh:mm tt"), DTS_TIMEFORMAT | DTS_UPDOWN) == TRUE)
	{
		otiDate->ReadOnly(TRUE);
	}
	// -- Images
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Images"));
	otiRoot->SetInfoText(_T("These are examples of image items."));
	// -- Images -> 16x16
	otiImage = (COptionTreeItemImage*)m_otTree.InsertItem(new COptionTreeItemImage(), otiRoot);
	otiImage->SetLabelText(_T("16x16"));
	otiImage->SetInfoText(_T("This is an example of a 16x16 image item."));
	if (otiImage->CreateImageItem(OT_IMAGE_SHOWTEXT, CSize(16, 16), 2) == TRUE)
	{
		otiImage->AddIcon(IDR_RECYCLE, _T("Recycle"));
		otiImage->AddIcon(IDR_IE, _T("Internet Explorer"));
		otiImage->AddIcon(IDR_INTERNET, _T("Internet"));
		otiImage->AddIcon(IDR_KEY, _T("Key"));
		otiImage->AddIcon(IDR_MEMORY, _T("Memory"));
		otiImage->AddIcon(IDR_NETWORK, _T("Network"));
		otiImage->AddIcon(IDR_GLOBE, _T("Globe"));

		otiImage->SetSelection(0);
	}
	// -- Images -> 32x32
	otiImage = (COptionTreeItemImage*)m_otTree.InsertItem(new COptionTreeItemImage(), otiRoot);
	otiImage->SetLabelText(_T("32x32"));
	otiImage->SetInfoText(_T("This is an example of a 32x32 image item."));
	if (otiImage->CreateImageItem(OT_IMAGE_SHOWTEXT, CSize(32, 32), 3) == TRUE)
	{
		otiImage->AddIcon(IDR_RECYCLE, _T("Recycle"));
		otiImage->AddIcon(IDR_IE, _T("Internet Explorer"));
		otiImage->AddIcon(IDR_INTERNET, _T("Internet"));
		otiImage->AddIcon(IDR_KEY, _T("Key"));
		otiImage->AddIcon(IDR_MEMORY, _T("Memory"));
		otiImage->AddIcon(IDR_NETWORK, _T("Network"));
		otiImage->AddIcon(IDR_GLOBE, _T("Globe"));

		otiImage->SetSelection(0);
	}
	// -- Images -> 48x48
	otiImage = (COptionTreeItemImage*)m_otTree.InsertItem(new COptionTreeItemImage(), otiRoot);
	otiImage->SetLabelText(_T("48x48"));
	otiImage->SetInfoText(_T("This is an example of a 48x48 image item."));
	if (otiImage->CreateImageItem(OT_IMAGE_SHOWTEXT, CSize(48, 48), 3) == TRUE)
	{
		otiImage->AddIcon(IDR_RECYCLE, _T("Recycle"));
		otiImage->AddIcon(IDR_IE, _T("Internet Explorer"));
		otiImage->AddIcon(IDR_INTERNET, _T("Internet"));
		otiImage->AddIcon(IDR_KEY, _T("Key"));
		otiImage->AddIcon(IDR_MEMORY, _T("Memory"));
		otiImage->AddIcon(IDR_NETWORK, _T("Network"));
		otiImage->AddIcon(IDR_GLOBE, _T("Globe"));

		otiImage->SetSelection(0);
	}
	// -- Images -> Read Only
	otiImage = (COptionTreeItemImage*)m_otTree.InsertItem(new COptionTreeItemImage(), otiRoot);
	otiImage->SetLabelText(_T("Read Only"));
	otiImage->SetInfoText(_T("This is an example of a read only image item."));
	if (otiImage->CreateImageItem(OT_IMAGE_SHOWTEXT, CSize(16, 16), 2) == TRUE)
	{
		otiImage->AddIcon(IDR_RECYCLE, _T("Recycle"));
		otiImage->AddIcon(IDR_IE, _T("Internet Explorer"));
		otiImage->AddIcon(IDR_INTERNET, _T("Internet"));
		otiImage->AddIcon(IDR_KEY, _T("Key"));
		otiImage->AddIcon(IDR_MEMORY, _T("Memory"));
		otiImage->AddIcon(IDR_NETWORK, _T("Network"));
		otiImage->AddIcon(IDR_GLOBE, _T("Globe"));

		otiImage->SetSelection(0);

		otiImage->ReadOnly(TRUE);
	}
	// -- Fonts
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Fonts"));
	otiRoot->SetInfoText(_T("These are examples of font items."));
	// -- Fonts -> True Type
	otiFont = (COptionTreeItemFont*)m_otTree.InsertItem(new COptionTreeItemFont(), otiRoot);
	otiFont->SetLabelText(_T("True Type"));
	otiFont->SetInfoText(_T("This is an example of a font item with only true type fonts."));
	if (otiFont->CreateFontItem(NULL, RGB(0, 0, 255), OT_FS_TTONLY) == TRUE)
	{
	}
	// -- Fonts -> Default Button
	otiFont = (COptionTreeItemFont*)m_otTree.InsertItem(new COptionTreeItemFont(), otiRoot);
	otiFont->SetLabelText(_T("Default Button"));
	otiFont->SetInfoText(_T("This is an example of a font item with a default button."));
	if (otiFont->CreateFontItem(&lfFont, RGB(0, 0, 255), OT_FS_USEDEFAULT) == TRUE)
	{
	}
	// -- Fonts -> Limited Styles
	otiFont = (COptionTreeItemFont*)m_otTree.InsertItem(new COptionTreeItemFont(), otiRoot);
	otiFont->SetLabelText(_T("Limited Styles"));
	otiFont->SetInfoText(_T("This is an example of a font item with a limited styles."));
	if (otiFont->CreateFontItem(&lfFont, RGB(0, 0, 0), OT_FS_USEDEFAULT | OT_FS_NOTEXTCOLOR | OT_FS_NOSTYLES | OT_FS_NOEFFECTS) == TRUE)
	{
	}
	// -- Fonts -> Apply Button
	otiFont = (COptionTreeItemFont*)m_otTree.InsertItem(new COptionTreeItemFont(), otiRoot);
	otiFont->SetLabelText(_T("Apply Button"));
	otiFont->SetInfoText(_T("This is an example of a font item with a apply button."));
	if (otiFont->CreateFontItem(&lfFont, RGB(0, 0, 0), OT_FS_USEDEFAULT | OT_FS_USEAPPLY) == TRUE)
	{
		otiFont->SetApplyWindow(this);
		m_uApplyFontID = otiFont->GetCtrlID();
	}	
	// -- Fonts -> Background Color
	otiFont = (COptionTreeItemFont*)m_otTree.InsertItem(new COptionTreeItemFont(), otiRoot);
	otiFont->SetLabelText(_T("Background Color"));
	otiFont->SetInfoText(_T("This is an example of a font item with a different background color."));
	if (otiFont->CreateFontItem(&lfFont, RGB(255, 255, 255), OT_FS_USEDEFAULT) == TRUE)
	{
		otiFont->SetBackgroundColor(RGB(192, 192, 192));
	}
	// -- Fonts -> Read Only
	otiFont = (COptionTreeItemFont*)m_otTree.InsertItem(new COptionTreeItemFont(), otiRoot);
	otiFont->SetLabelText(_T("Read Only"));
	otiFont->SetInfoText(_T("This is an example of a font item that is read only."));
	if (otiFont->CreateFontItem(&lfFont, RGB(255, 255, 255), OT_FS_USEDEFAULT) == TRUE)
	{
		otiFont->ReadOnly(TRUE);
	}
	// -- Files and Folders
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Files and Folders"));
	otiRoot->SetInfoText(_T("These are examples of file and folders items."));
	// -- Files and Folders -> File Full Path
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	otiFile->SetLabelText(_T("File Full Path"));
	otiFile->SetInfoText(_T("This is an example of a file item displaying the full path."));
	if (otiFile->CreateFileItem(_T("C:\\AutoExec.Bat"), _T(""), _T("All files|*.*|"), OT_FILE_OPENDIALOG | OT_FILE_SHOWFULLPATH, OFN_OVERWRITEPROMPT) == TRUE)
	{
	}
	// -- Files and Folders -> File Name
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	otiFile->SetLabelText(_T("File Name"));
	otiFile->SetInfoText(_T("This is an example of a file item displaying the full name."));
	if (otiFile->CreateFileItem(_T("C:\\AutoExec.Bat"), _T(""), _T("All files|*.*|"), OT_FILE_OPENDIALOG | OT_FILE_SHOWFILENAME, OFN_OVERWRITEPROMPT) == TRUE)
	{
	}
	// -- Files and Folders -> File Title
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	otiFile->SetLabelText(_T("File Title"));
	otiFile->SetInfoText(_T("This is an example of a file item displaying the full title."));
	if (otiFile->CreateFileItem(_T("C:\\AutoExec.Bat"), _T(""), _T("All files|*.*|"), OT_FILE_OPENDIALOG | OT_FILE_SHOWFILETITLE, OFN_OVERWRITEPROMPT) == TRUE)
	{
	}
	// -- Files and Folders -> File Extention
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	otiFile->SetLabelText(_T("File Extention"));
	otiFile->SetInfoText(_T("This is an example of a file item displaying the full extention."));
	if (otiFile->CreateFileItem(_T("C:\\AutoExec.Bat"), _T(""), _T("All files|*.*|"), OT_FILE_OPENDIALOG | OT_FILE_SHOWFILEEXT, OFN_OVERWRITEPROMPT) == TRUE)
	{
	}
	// -- Files and Folders -> File Directory
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	otiFile->SetLabelText(_T("File Directory"));
	otiFile->SetInfoText(_T("This is an example of a file item displaying the full directory."));
	if (otiFile->CreateFileItem(_T("C:\\AutoExec.Bat"), _T(""), _T("All files|*.*|"), OT_FILE_OPENDIALOG | OT_FILE_SHOWFILEDIR, OFN_OVERWRITEPROMPT) == TRUE)
	{
	}
	// -- Files and Folders -> File Drive
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	otiFile->SetLabelText(_T("File Drive"));
	otiFile->SetInfoText(_T("This is an example of a file item displaying the full drive."));
	if (otiFile->CreateFileItem(_T("C:\\AutoExec.Bat"), _T(""), _T("All files|*.*|"), OT_FILE_OPENDIALOG | OT_FILE_SHOWFILEDRIVE, OFN_OVERWRITEPROMPT) == TRUE)
	{
	}
	// -- Files and Folders -> Multi Select
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	otiFile->SetLabelText(_T("Multi Select"));
	otiFile->SetInfoText(_T("This is an example of a file item allowing multi file selection."));
	if (otiFile->CreateFileItem(_T("C:\\AutoExec.Bat"), _T(""), _T("All files|*.*|"), OT_FILE_OPENDIALOG | OT_FILE_SHOWFULLPATH, OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT) == TRUE)
	{
	}
	// -- Files and Folders -> Folder Select
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	otiFile->SetLabelText(_T("Folder Select"));
	otiFile->SetInfoText(_T("This is an example of a file item allowing folder selection."));
	if (otiFile->CreateFileItem(_T("C:\\Program Files"), _T(""), _T(""), OT_FILE_SELECTDIALOG | OT_FILE_SHOWFULLPATH, 0x40 | BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS) == TRUE)
	{
	}
	// -- Files and Folders -> Read Only
	otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	otiFile->SetLabelText(_T("Read Only"));
	otiFile->SetInfoText(_T("This is an example of a file item that is read only."));
	if (otiFile->CreateFileItem(_T("C:\\AutoExec.Bat"), _T(""), _T("All files|*.*|"), OT_FILE_OPENDIALOG | OT_FILE_SHOWFULLPATH, OFN_OVERWRITEPROMPT) == TRUE)
	{
		otiFile->ReadOnly(TRUE);
	}
	// -- IP Address
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("IP Address"));
	otiRoot->SetInfoText(_T("These are examples of IP address items."));
	// -- IP Address -> Normal
	otiIPAddress = (COptionTreeItemIPAddress*)m_otTree.InsertItem(new COptionTreeItemIPAddress(), otiRoot);
	otiIPAddress->SetLabelText(_T("Normal"));
	otiIPAddress->SetInfoText(_T("This is an example of a normal IP address."));
	if (otiIPAddress->CreateIPAddressItem(0) == TRUE)
	{
		otiIPAddress->SetAddress(1, 2, 3, 4);
	}
	// -- IP Address -> Read Only
	otiIPAddress = (COptionTreeItemIPAddress*)m_otTree.InsertItem(new COptionTreeItemIPAddress(), otiRoot);
	otiIPAddress->SetLabelText(_T("Read Only"));
	otiIPAddress->SetInfoText(_T("This is an example of a read only IP address."));
	if (otiIPAddress->CreateIPAddressItem(0) == TRUE)
	{
		otiIPAddress->SetAddress(4, 3, 2, 1);
		otiIPAddress->ReadOnly(TRUE);
	}
	// -- Hyperlink
	otiRoot = m_otTree.InsertItem(new COptionTreeItem());
	otiRoot->SetLabelText(_T("Hyperlink"));
	otiRoot->SetInfoText(_T("These are examples of hyperlink items."));
	// -- Hyperlink -> Hover
	oti_Hyperlink = (COptionTreeItemHyperLink*)m_otTree.InsertItem(new COptionTreeItemHyperLink(), otiRoot);
	oti_Hyperlink->SetLabelText(_T("Hover"));
	oti_Hyperlink->SetInfoText(_T("This is an example of a hyperlink with the hover option."));
	if (oti_Hyperlink->CreateHyperlinkItem(OT_HL_HOVER, _T("http://www.computersmarts.net"), RGB(0, 0, 255), RGB(150, 0, 150), RGB(255, 0, 255)) == TRUE)
	{
	}
	// -- Hyperlink -> Visited
	oti_Hyperlink = (COptionTreeItemHyperLink*)m_otTree.InsertItem(new COptionTreeItemHyperLink(), otiRoot);
	oti_Hyperlink->SetLabelText(_T("Visited"));
	oti_Hyperlink->SetInfoText(_T("This is an example of a hyperlink with the visited option."));
	if (oti_Hyperlink->CreateHyperlinkItem(OT_HL_HOVER | OT_HL_VISITED, _T("http://www.computersmarts.net"), RGB(0, 0, 255), RGB(150, 0, 150), RGB(255, 0, 255)) == TRUE)
	{
	}
	// -- Hyperlink -> Underlined Hover
	oti_Hyperlink = (COptionTreeItemHyperLink*)m_otTree.InsertItem(new COptionTreeItemHyperLink(), otiRoot);
	oti_Hyperlink->SetLabelText(_T("Underlined Hover"));
	oti_Hyperlink->SetInfoText(_T("This is an example of a hyperlink with underlined text when hovered."));
	if (oti_Hyperlink->CreateHyperlinkItem(OT_HL_HOVER | OT_HL_UNDERLINEHOVER, _T("http://www.computersmarts.net"), RGB(0, 0, 255), RGB(150, 0, 150), RGB(255, 0, 255)) == TRUE)
	{
	}		
	// -- Hyperlink -> Underlined
	oti_Hyperlink = (COptionTreeItemHyperLink*)m_otTree.InsertItem(new COptionTreeItemHyperLink(), otiRoot);
	oti_Hyperlink->SetLabelText(_T("Underlined"));
	oti_Hyperlink->SetInfoText(_T("This is an example of a hyperlink with underlined text."));
	if (oti_Hyperlink->CreateHyperlinkItem(OT_HL_HOVER | OT_HL_UNDERLINE, _T("http://www.computersmarts.net"), RGB(0, 0, 255), RGB(150, 0, 150), RGB(255, 0, 255)) == TRUE)
	{
	}	
	// -- Hyperlink -> Read Only
	oti_Hyperlink = (COptionTreeItemHyperLink*)m_otTree.InsertItem(new COptionTreeItemHyperLink(), otiRoot);
	oti_Hyperlink->SetLabelText(_T("Read Only"));
	oti_Hyperlink->SetInfoText(_T("This is an example of a hyperlink that is read only."));
	if (oti_Hyperlink->CreateHyperlinkItem(OT_HL_HOVER, _T("http://www.computersmarts.net"), RGB(0, 0, 255), RGB(150, 0, 150), RGB(255, 0, 255)) == TRUE)
	{
		oti_Hyperlink->ReadOnly(TRUE);
	}

  CString sItemCount("");
  sItemCount.Format("COptionTreeDemo; Tree Items loaded: <%d>",m_otTree.GetItemCount());
  this->SetWindowText(sItemCount);
  m_otTree.UnLockAll();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCOptionTreeDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCOptionTreeDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCOptionTreeDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCOptionTreeDemoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// Don't exit when press Enter of Esc
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
  else if(pMsg->message == WM_BT_CLICKED)
  {
    COptionTreeItem* pItem = (COptionTreeItem*) pMsg->lParam;
    if(pItem != NULL)
    {
      CString sClicked("");
      sClicked.Format("Button <%d> clicked of Item with ID <%d>",(long) pMsg->wParam,pItem->GetCtrlID());
      AfxMessageBox(sClicked);
    }
    return TRUE;
  }
	return CDialog::PreTranslateMessage(pMsg);
}

void CCOptionTreeDemoDlg::OnSize(UINT nType, int cx, int cy) 
{
	// Resize the control
	if ((cx > 0) && (cy > 0))
	{
		if (::IsWindow(m_otTree.GetSafeHwnd()))
		{
			m_otTree.MoveWindow(0, 0, cx, cy);
		}
	}
	
	CDialog::OnSize(nType, cx, cy);	
}

LRESULT CCOptionTreeDemoDlg::WM_ApplyButton(WPARAM wParam, LPARAM lParam)
{
	// Declare variables
	UINT uID;

	// Get incoming ID
	uID = (UINT) wParam;

	// See what ID it is
	if (uID == m_uApplyFontID)
	{
		OnApplyFont(lParam);
	}

	return 0;
}

void CCOptionTreeDemoDlg::OnTreeItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	// Declare variables
	LPNMOPTIONTREE pNMOptionTree = (LPNMOPTIONTREE)pNotifyStruct;

	if (pNMOptionTree != NULL && pNMOptionTree->otiItem != NULL)
	{
    ::Beep(1200,30);
    
    _Text.Format("Item of ID <%d> has changed its Value",pNMOptionTree->otiItem->GetCtrlID());
    UpdateData(FALSE);
	}
	//return 0;
}
void CCOptionTreeDemoDlg::OnTreeItemClicked(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	// Declare variables
	LPNMOPTIONTREE pNMOptionTree = (LPNMOPTIONTREE)pNotifyStruct;

	// Validate
	if (pNMOptionTree != NULL && pNMOptionTree->otiItem != NULL)
	{
	}

	//return 0;
}

void CCOptionTreeDemoDlg::OnApplyFont(LPARAM lParam)
{
	// Note depending on if you are usng OT_FS_USECHARFORMAT or not
	// for this item a LOGFONT or CHARFORMAT font is sent.

	// Declare variables
	PLOGFONTA lf;

	// Get font
	lf = (PLOGFONTA) lParam;

	MessageBox("Include apply code to be able to accept font chages live.", "COptionTreeDemo", MB_OK);
}

void CCOptionTreeDemoDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{		
	// Set minimum and maximum dimensions
	lpMMI->ptMinTrackSize.x = 425;
	lpMMI->ptMinTrackSize.y = 450;

	CDialog::OnGetMinMaxInfo(lpMMI);
}
long itc=0;
void CCOptionTreeDemoDlg::OnBnClickedBtTest()
{
  CPreciseTimer pt;
  CWaitCursor WC;

  int i=0;
  m_otTree.LockAll();
  CString Label("");
  while(i++ < 1000)
  {
    /* 5.7 ohne buttons
	  otiStatic = (COptionTreeItemStatic*)m_otTree.InsertItem(new COptionTreeItemStatic(), otiRoot);
	  otiStatic->SetLabelText(_T("Sub Item 1"));
	  otiStatic->SetInfoText(_T("Sub item 1."));
	  otiStatic->SetStaticText(_T("I am a sub item."));
	  if (otiStatic->CreateStaticItem(0) == TRUE)
	  {
	  }
    //otiStatic->AddButton(_D_BT_ALL);
    */
    Label.Format("Edit Item # %d",++itc);
    oti_editbt = (COptionTreeItemEditButton*)m_otTree.InsertItem(new COptionTreeItemEditButton(),otiBase);
    oti_editbt->SetLabelText(Label);
    oti_editbt->SetInfoText(_T("This is an example of a edit item."));

    // Note: dont create the underlying edit here.create it just in time on using first.
    // disable this line and enable following 3 lines to see the performance difference.
    // that is just for demonstration.
    // for COptionTreeItemEditButton derivates, PrepareEditItem(...) is recommended 
    // instead of CreateEditItem(...)
    oti_editbt->PrepareEditItem(_T("edit button"),NULL, NULL);
    /*
    if (oti_editbt->CreateEditItem(NULL, NULL) == TRUE)
    {
      oti_editbt->SetWindowText(_T("edit button"));	
    }
    */
    oti_editbt->AddButton(_D_BT_ALL,"User");

    //otiStatic->AddButton(_D_BT_ALL);
    /* 5.7 ob
	  otiFile = (COptionTreeItemFile*)m_otTree.InsertItem(new COptionTreeItemFile(), otiRoot);
	  otiFile->SetLabelText(_T("File Directory"));
	  otiFile->SetInfoText(_T("This is an example of a file item displaying the full directory."));
	  if (otiFile->CreateFileItem(_T("C:\\AutoExec.Bat"), _T(""), _T("All files|*.*|"), OT_FILE_OPENDIALOG | OT_FILE_SHOWFILEDIR, OFN_OVERWRITEPROMPT) == TRUE)
	  {
	  }
    //otiStatic->AddButton(_D_BT_ALL);

    */
    /* 8.6 ob
	  otiCombo = (COptionTreeItemComboBox*)m_otTree.InsertItem(new COptionTreeItemComboBox(), otiRoot);
	  otiCombo->SetLabelText(_T("Sorted"));
	  otiCombo->SetInfoText(_T("This is an example of a sorted combo box item."));
	  if (otiCombo->CreateComboItem(CBS_SORT) == TRUE)
	  {
		  otiCombo->AddString("Blue");
		  otiCombo->AddString("Red");
		  otiCombo->AddString("Green");
		  otiCombo->AddString("Yellow");
		  otiCombo->AddString("Purple");
		  otiCombo->AddString("Orange");

		  otiCombo->SelectString(0, "Blue");
    }
    //otiStatic->AddButton(_D_BT_ALL);
    */
    /* 6.5 ob
    otiCheck = (COptionTreeItemCheckBox*)m_otTree.InsertItem(new COptionTreeItemCheckBox(), otiRoot);
	  otiCheck->SetLabelText(_T("Without Check Box"));
	  otiCheck->SetInfoText(_T("This is an example of a check box item without check box when not activated."));
	  if (otiCheck->CreateCheckBoxItem(FALSE, OT_CHECKBOX_SHOWTEXT) == TRUE)
	  {
		  otiCheck->SetCheckText(_T("Checked"), _T("UnChecked"));
	  }
    //otiStatic->AddButton(_D_BT_ALL);
    */
    // 6.5 ob
    //
    Label.Format("Color Item # %d",++itc);
	  otiColor = (COptionTreeItemColor*)m_otTree.InsertItem(new COptionTreeItemColor(), otiBase);
	  otiColor->SetLabelText(Label);
	  otiColor->SetInfoText(_T("This is an example of live update color item."));
	  if (otiColor->CreateColorItem(OT_COLOR_SHOWHEX | OT_COLOR_LIVEUPDATE, RGB(255, 100, 255), RGB(0, 0, 0)) == TRUE)
	  {
	  }
    otiColor->AddButton(_D_BT_ALL,"User 2");

    Label.Format("Font Item # %d",++itc);
	  otiFont = (COptionTreeItemFont*)m_otTree.InsertItem(new COptionTreeItemFont(), otiBase);
	  otiFont->SetLabelText(Label);
	  otiFont->SetInfoText(_T("This is an example of a font item with only true type fonts."));
	  if (otiFont->CreateFontItem(NULL, RGB(0, 0, 255), OT_FS_TTONLY) == TRUE)
	  {
	  }
    otiFont->AddButton(_D_BT_ALL,"User 3");
  }
  m_otTree.UnLockAll();

  pt.StopTimer();
  _Text.Format("Elapsed Time (dd:hh:mm:ss:mmm:iii) for Insertion and Creation of 3000 Items: %s",pt.GetDurationString());
  UpdateData(FALSE);

  CString sItemCount("");
  sItemCount.Format("COptionTreeDemo; Tree Items loaded: <%d>",m_otTree.GetItemCount());
  this->SetWindowText(sItemCount);
}

void CCOptionTreeDemoDlg::OnBnClickedBtExpandall()
{
  m_otTree.ExpandAllItems();
}

void CCOptionTreeDemoDlg::OnBnClickedBtClearInfo()
{
  m_otTree.ClearInfoText();
}

void CCOptionTreeDemoDlg::OnBnClickedBtSearchItem()
{
  UpdateData(TRUE);
  CWaitCursor C;
  // unselect previous matches
  m_otTree.SelectItems(NULL, FALSE);

  DWORD searchflags = OT_FI_LABEL;
  if(m_bMindAttribute == TRUE)
    searchflags |= OT_FI_ATTRIBUTE;

  CPreciseTimer pt;
  COptionTreeItem * pNewMatch = m_otTree.FindItem(_SearchText,searchflags,_pSearchOffset);
  pt.StopTimer();
  if(pNewMatch == NULL || pNewMatch == _pSearchOffset)
  {
    _Text.Format("No Item found - Elapsed Time (dd:hh:mm:ss:mmm:iii) for Search: %s",pt.GetDurationString());
    UpdateData(FALSE);
  }
  else
  {
    _pSearchOffset = pNewMatch;
    m_otTree.EnsureVisible(pNewMatch);
    m_otTree.SelectItems(_pSearchOffset,TRUE);
    _Text.Format("Item found - Elapsed Time (dd:hh:mm:ss:mmm:iii) for Search: %s",pt.GetDurationString());
    UpdateData(FALSE);
  }
}

void CCOptionTreeDemoDlg::OnBnClickedBtResetSearch()
{
  _pSearchOffset = NULL;
}

void CCOptionTreeDemoDlg::OnBnClickedCbMindAttribute()
{
  m_bMindAttribute = !m_bMindAttribute;
}
