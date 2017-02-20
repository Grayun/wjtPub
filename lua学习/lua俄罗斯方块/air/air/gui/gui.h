#ifndef gui_static_h_
#define gui_static_h_
#include "../graphics/openGL.h"
#include <string>
#include <list>
using std::string;
using std::list;
namespace air
{
     ////////////////////////////////////////////////////////////////////////
	// gui控件基类
	class CGuiObject
	{
	protected:
		unsigned int m_id;			// 唯一标识
		float	 m_x, m_y;			        // 控件左下角坐标
		float	 m_width, m_height;	// 控件尺寸
		//bool	 m_visible;			        // 是否可见，目前没这个需求
		bool m_focus;                   // 是否拥有焦点
    public:
        // gui消息
        enum { NIL, KEY_DOWN, KEY_UP, MOUSE_MOVE, MOUSE_L_DOWN, MOUSE_L_UP };
        enum { SHIFT = 1 };
        struct msg
        {
            int type;     // 消息类型
            int key;      // 虚拟键值
            int flags;    // 键盘状态
            float x, y;   // 鼠标位置
        };
	public:
        // 构造控件
        // id：     唯一标识
        // x，y：  位置
        // w，h： 尺寸
		CGuiObject(unsigned int id, float x, float y, float w, float h);
		virtual ~CGuiObject();
		// 是否获得焦点
		// 返回值：true -- 有焦点 / false -- 无焦点
		bool is_focus() const;
		// 失去焦点
		void lost();
		// 返回控件ID
        // 返回值：	控件ID
		unsigned int get_id() const;
		// 纯虚接口 ////////////////////////////////////////////////////////////////////////
		// 绘制控件
		virtual void draw() = 0;
		// 事件响应
		// e：消息
		// 返回值：事件是否被处理
		virtual bool on_msg(const msg& e) = 0;
		// 判断坐标是否在控件范围内
        // x，y：	某点的坐标
        // 返回值： true -- 点在控件内 / false -- 点在控件外
		virtual bool is_over(float x, float y) = 0;
	private:
		CGuiObject();
		CGuiObject(const CGuiObject&);
		CGuiObject& operator =(const CGuiObject&);
	};
	////////////////////////////////////////////////////////////////////////
	// gui管理器
	class CGuiSystem
	{
	private:
		typedef list<CGuiObject*>	GuiList;
		GuiList m_gui_object_list;
	public:
		CGuiSystem();
		~CGuiSystem();
        // 添加控件
        // p_obj： 控件对象
		void add(CGuiObject* p_obj);
		// 从配置初始化GUI
        // p_config：	配置文件
		bool load_from_config(const char* p_config);
		//	绘制所有控件
		void draw();
		// 输入消息处理
		// 返回值： 控件ID
		enum { NO_MSG = 0xffff };
		unsigned int process_msg();
		 // 取消所有控件的焦点
		void all_lost();
		// 返回一个控件
		CGuiObject* get(unsigned int id);
	private:
		CGuiSystem(const CGuiSystem&);
		CGuiSystem& operator=(const CGuiSystem&);
	};
	////////////////////////////////////////////////////////////////////////
	// 文本标签
	class CLabel : public CGuiObject
	{
	private:
		string m_text;				    // 文本内容
		string m_pen;				// 字体索引名
		color m_text_color;		    // 文本颜色
	public:
		CLabel(unsigned int id, float x, float y, string text, string pen, color c);
		virtual void draw();
		virtual bool on_msg(const msg& e);
		virtual bool is_over(float x, float y);
	private:
		CLabel();
		CLabel(const CLabel&);
		CLabel& operator=(const CLabel&);
	};
	////////////////////////////////////////////////////////////////////////
	// 按钮
	class CButton : public CGuiObject
	{
	private:
		string	m_up_tex;			// 弹起纹理名
		string	m_down_tex;		// 按下纹理名
		string	m_over_tex;		// 滑过纹理名
		enum ButtonState	{ B_UP, B_DOWN, B_OVER };
		ButtonState	m_state;
	public:
		CButton(unsigned int id, float x, float y, float width, float height, string up_tex, string down_tex, string over_tex);
		virtual void draw();
		virtual bool on_msg(const msg& e);
		virtual bool is_over(float x, float y);
	private:
		CButton();
		CButton(const CButton&);
		CButton& operator=(const CButton&);
	};
	////////////////////////////////////////////////////////////////////////
	// 编辑框
	class CEdit : public CGuiObject
	{
    private:
        string m_pen;   // 画笔名称
        string m_result;   // 编辑框文本
    public:
        CEdit(unsigned int id, float x, float y, float width, float height, string pen);
        ~CEdit();
        virtual void draw();
		virtual bool on_msg(const msg& e);
		virtual bool is_over(float x, float y);
		// 取得编辑框输入的内容
		string content() const;
    private:
        bool is_full() const;   // 编辑框是否已满
        CEdit();
        CEdit(const CEdit&);
        CEdit& operator=(const CEdit&);
	};
}
#endif
