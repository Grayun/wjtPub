#ifndef single_h_
#define single_h_

namespace air
{
	template<class T>
	class CSingleton
	{
	public:
		CSingleton()	{};
		virtual ~CSingleton()	{};
		static T* instance()
		{
			static T* s_p_instance = NULL;
			if (s_p_instance == NULL)
				s_p_instance = new T();
			return s_p_instance;
		}
		void free()
		{
			delete this;
		}
	private:
		CSingleton(const CSingleton&);
		CSingleton& operator=(const CSingleton&);
	};
	#define SINGLETON(T) friend class CSingleton<T>
	#define NEW(T)  T::instance()
	#define FREE(T) T::free()
}

#endif