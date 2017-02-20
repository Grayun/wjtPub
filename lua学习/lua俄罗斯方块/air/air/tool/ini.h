/*
	ini 文件解析类
*/
#ifndef ini_h_
#define ini_h_

#include <map>
#include <string>
using std::map;
using std::string;

namespace air
{
	class CIni
	{
	private:
		typedef map<string, string>		IniSection;
		typedef map<string, IniSection>	IniAllSections;
		IniAllSections m_all_sections;
	public:
		CIni();
		~CIni();
		/*
			载入并解析配置文件
			p_ini：		配置文件路径
			返回值：	成功 / 失败
		*/
		bool load(const char* p_ini);
		/*
			从配置文件中读取信息
			section：	块名
			name：		属性名
			value：		读取到的值
			返回值：	成功 / 失败
		*/
		bool read_int(const string& section, const string& name, int& value);
		bool read_float(const string& section, const string& name, float& value);
		bool read_string(const string& section, const string& name, string& value);
		bool read_3ub(string section, string name, unsigned char& x, unsigned char& y, unsigned char& z);
		/*
			释放内存
		*/
		void free();

	private:
		CIni(const CIni&);
		CIni& operator = (const CIni&);
	};
}

#endif
