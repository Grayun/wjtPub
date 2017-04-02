#pragma once

// lua_State 加载基本库 
template<typename T>
class lua_State_BasePointer
{
public:
	lua_State_BasePointer();
	~lua_State_BasePointer();

	// --------------------------------------------------------------------------------
	// Summary: 
	//   创建Lua接口指针 建立lua栈用于和C交互 加载基本库
	// --------------------------------------------------------------------------------
	bool Open();

	const T *  operator->() const;
	T *        operator->();
	operator const T*() const;
	operator T*() const;

	T* Object();

protected:
	void Release();

private:
	T* pTr;
};

template<typename T> inline
lua_State_BasePointer<T>::lua_State_BasePointer()
{
	pTr = NULL;
}

template<typename T> inline
lua_State_BasePointer<T>::~lua_State_BasePointer()
{
	Release(); // 
}

template<typename T> inline
bool lua_State_BasePointer<T>::Open()
{
	Release();
	pTr = lua_open();
	if (NULL == pTr)
	{
		return false;
	}

	// 加载Lua基本库.- print() 
	luaopen_base(pTr);

	return true;
}

template<typename T> inline
void lua_State_BasePointer<T>::Release()
{
	//清除lua
	if (NULL != pTr)
	{
		lua_close(pTr);
		pTr = NULL;
	}
}
template<typename T> inline
T* lua_State_BasePointer<T>::Object()
{
	return pTr;
}

// 
template<typename T> inline
T *  lua_State_BasePointer<T>::operator->()
{
	return pTr;
}

template<typename T> inline
const T *  lua_State_BasePointer<T>::operator->() const
{
	return pTr;
}

template<typename T> inline
lua_State_BasePointer<T>::operator T*() const
{
	return pTr;
}

template<typename T> inline
lua_State_BasePointer<T>::operator const T*() const
{
	return this;
}
//////////////////////////////////////////////////////////////////////////

class CLuaPub
{
public:
	CLuaPub(void);
	~CLuaPub(void);

	static lua_State* LuaPtr();
	static void DoLua(const CString& str);
private:
	// --------------------------------------------------------------------------------
	// Summary: 
	//   加载arx相关类库函数
	// --------------------------------------------------------------------------------
	static void LoadArxLua();
	static void PrintArx(const char*);
	static void PrintArxLn(const char*);

private:
	// --------------------------------------------------------------------------------
	// Summary: 
	//   注册ARX常用类
	// --------------------------------------------------------------------------------
	static void Register_Point();
	static void Register_Vecter();
	static void Register_Mat();
	static void Register_ObjId();

	// --------------------------------------------------------------------------------
	// Summary: 
	//   注册arx简单实体类
	// --------------------------------------------------------------------------------
	static void Register_Ent_Text();
	static void Register_Ent_Line();
	static void Register_Ent_Circle();
	static void Register_Ent_PolyLine();

	// --------------------------------------------------------------------------------
	// Summary: 
	//   注册arx 实体加入数据库相关接口 不处理指针，传入对象，内部copy，lua处理指针麻烦
	// --------------------------------------------------------------------------------
	static void AppEnt(const AcDbEntity* pEntity);

	// --------------------------------------------------------------------------------
	// Summary: 
	//   实体选择相关
	// --------------------------------------------------------------------------------
	static void Registe_EntSelect();

	// 其他辅助函数
	// --------------------------------------------------------------------------------
	// Summary: 
	//    解析utf-8 为ANSI
	// --------------------------------------------------------------------------------
	static const char *utf8_decode (const char *o, int *val);

	// --------------------------------------------------------------------------------
	// Summary: 
	//   ANSI 解析文utf-8 
	// --------------------------------------------------------------------------------
	static const char* ansi_decode(const char* o, int* val);


private:
	lua_State_BasePointer<lua_State> L;
};
