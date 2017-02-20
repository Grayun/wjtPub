#include "ini.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
using std::ifstream;
using std::stringstream;

namespace air
{
	CIni::CIni()
	{
		free();
	}

	CIni::~CIni()
	{
		free();
	}

	bool CIni::load(const char* p_ini)
	{
		ifstream reader;
		reader.open(p_ini);
		if (!reader.is_open())
			return false;

		#define BuffSize	1024
		char buff[BuffSize];
		string section_name;
		IniSection cur_section;
		while (!reader.eof())
		{
			reader.getline(buff, BuffSize);
			string line(buff);

			string::size_type pos_begin = line.find_first_not_of(" \t");
			if (pos_begin >= line.size() || line.at(pos_begin) == '#')		// 跳过空行 或 注释行
				continue;

			if (line.at(pos_begin) == '[')
			{
				if (!section_name.empty())
					m_all_sections[section_name] = cur_section;
				cur_section.clear();

				string::size_type pos_sec_name_end = line.find_first_of(']', pos_begin);
				section_name = line.substr(pos_begin + 1, pos_sec_name_end - pos_begin - 1);
			}
			else
			{
				string::size_type pos1 = line.find_first_of('=', pos_begin);
				string::size_type pos2 = line.find_last_not_of(" \t", pos1 - 1);
				string key = line.substr(pos_begin, pos2 + 1);

				string::size_type pos3 = line.find_first_not_of(" \t", pos1 + 1);
				string::size_type pos4 = line.find_last_not_of(" \t");
				string value = line.substr(pos3, pos4 - pos3 + 1);

				cur_section[key] = value;
			}
		}

		m_all_sections[section_name] = cur_section;

		reader.close();
		return true;
	}

	bool CIni::read_int(const string& section, const string& name, int& value)
	{
		string num;
		if (!read_string(section, name, num))
			return false;

		value = atoi(num.c_str());
		return true;
	}

	bool CIni::read_float(const string& section, const string& name, float& value)
	{
		string num;
		if (!read_string(section, name, num))
			return false;

		value = static_cast<float>(atof(num.c_str()));
		return true;
	}

	bool CIni::read_string(const string& section, const string& name, string& value)
	{
		IniAllSections::iterator it1 = m_all_sections.find(section);
		if (it1 == m_all_sections.end())
			return false;

		IniSection cur_section = it1->second;
		IniSection::iterator it2 = cur_section.find(name);
		if (it2 == cur_section.end())
			return false;

		value = it2->second;
		return true;
	}
	bool CIni::read_3ub(string section, string name, unsigned char& x, unsigned char& y, unsigned char& z)
	{
        string s;
        if (!read_string(section, name, s))
            return false;
        string s1, s2, s3;
        stringstream ss(s);
        ss >> s1 >> s2 >> s3;
        x = static_cast<unsigned char>(atoi(s1.c_str()));
        y = static_cast<unsigned char>(atoi(s2.c_str()));
        z = static_cast<unsigned char>(atoi(s3.c_str()));
        return true;
	}
	void CIni::free()
	{
		m_all_sections.clear();
	}
}
