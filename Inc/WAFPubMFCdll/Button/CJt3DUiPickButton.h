#pragma once

// --------------------------------------------------------------------------------
// Summary: 
//   按钮显示一个 【拾取】图片，按钮大小为 24 x 24(AutoLoad之后大小)
//	 用列：CJt3DUiPickButton m_btnPick;窗体初始化时，m_btnPick.AutoLoad()即可
// --------------------------------------------------------------------------------
class WAFPUBMFCDLL_API CJt3DUiPickButton : public CButtonST
{
public:
	DECLARE_DYNAMIC(CJt3DUiPickButton)

	CJt3DUiPickButton();
	virtual ~CJt3DUiPickButton();

public:
	// --------------------------------------------------------------------------------
	// Summary: 
	//   加载选择图片，调整大小和选择图片一样，位置不变
	// --------------------------------------------------------------------------------
	BOOL AutoLoad();
};
