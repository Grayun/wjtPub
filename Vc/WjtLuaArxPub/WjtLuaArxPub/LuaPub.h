#pragma once

// lua_State ���ػ����� 
template<typename T>
class lua_State_BasePointer
{
public:
	lua_State_BasePointer();
	~lua_State_BasePointer();

	// --------------------------------------------------------------------------------
	// Summary: 
	//   ����Lua�ӿ�ָ�� ����luaջ���ں�C���� ���ػ�����
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

	// ����Lua������.- print() 
	luaopen_base(pTr);

	return true;
}

template<typename T> inline
void lua_State_BasePointer<T>::Release()
{
	//���lua
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
	//   ����arx�����⺯��
	// --------------------------------------------------------------------------------
	static void LoadArxLua();
	static void PrintArx(const char*);
	static void PrintArxLn(const char*);

private:
	// --------------------------------------------------------------------------------
	// Summary: 
	//   ע��ARX������
	// --------------------------------------------------------------------------------
	static void Register_Point();
	static void Register_Vecter();
	static void Register_Mat();
	static void Register_ObjId();

	// --------------------------------------------------------------------------------
	// Summary: 
	//   ע��arx��ʵ����
	// --------------------------------------------------------------------------------
	static void Register_Ent_Text();
	static void Register_Ent_Line();
	static void Register_Ent_Circle();
	static void Register_Ent_PolyLine();

	// --------------------------------------------------------------------------------
	// Summary: 
	//   ע��arx ʵ��������ݿ���ؽӿ� ������ָ�룬��������ڲ�copy��lua����ָ���鷳
	// --------------------------------------------------------------------------------
	static void AppEnt(const AcDbEntity* pEntity);

	// --------------------------------------------------------------------------------
	// Summary: 
	//   ʵ��ѡ�����
	// --------------------------------------------------------------------------------
	static void Registe_EntSelect();

	// ������������
	// --------------------------------------------------------------------------------
	// Summary: 
	//    ����utf-8 ΪANSI
	// --------------------------------------------------------------------------------
	static const char *utf8_decode (const char *o, int *val);

	// --------------------------------------------------------------------------------
	// Summary: 
	//   ANSI ������utf-8 
	// --------------------------------------------------------------------------------
	static const char* ansi_decode(const char* o, int* val);


private:
	lua_State_BasePointer<lua_State> L;
};
