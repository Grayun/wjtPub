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

private:
	lua_State_BasePointer<lua_State> L;
};
