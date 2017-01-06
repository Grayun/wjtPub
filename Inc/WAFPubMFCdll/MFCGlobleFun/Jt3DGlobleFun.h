#pragma once

namespace HP3D
{
	//调用全局 SendMessage 向CAD发送两个 ESC
	WAFPUBMFCDLL_API bool ClearAcadCmdLine();

	// --------------------------------------------------------------------------------
	// Summary: 
	//   移动窗体到主窗体左上角
	// --------------------------------------------------------------------------------
	WAFPUBMFCDLL_API void MoveWindowToViewTopLeft(CWnd* pWindow);
}
