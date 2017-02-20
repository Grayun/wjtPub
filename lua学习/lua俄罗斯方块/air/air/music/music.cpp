#include "music.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <sstream>
using std::ifstream;
using std::stringstream;

namespace air
{
	CMusic* g_p_music = NULL;
	CMusic::CMusic()
	{
		_check(FMOD_System_Create(&m_p_system));
		unsigned int version;
		_check(FMOD_System_GetVersion(m_p_system, &version));
		if (version < FMOD_VERSION)
			printf("fmod version too old！");
		_check(FMOD_System_Init(m_p_system, 32, FMOD_INIT_NORMAL, NULL));
	}
	CMusic::~CMusic()
	{
		free();
	}
	bool CMusic::load_music(string file, string name)
	{
		if (name.empty() || file.empty() || m_musics.end() != m_musics.find(name))
			return false;
		data obj;
		obj.type = MUSIC;
		_check(FMOD_System_CreateStream(m_p_system, file.c_str(), FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &obj.p_sound));
		FMOD_CHANNEL* p_channel = NULL;
		_check(FMOD_System_PlaySound(m_p_system, FMOD_CHANNEL_FREE, obj.p_sound, true, &p_channel));
		m_channels.insert(make_pair(name, p_channel));
		m_musics.insert(make_pair(name, obj));
		return true;
	}
	bool CMusic::load_sound(string file, string name)
	{
		if (name.empty() || file.empty() || m_musics.end() != m_musics.find(name))
			return false;
		data obj;
		obj.type = SOUND;
		_check(FMOD_System_CreateStream(m_p_system, file.c_str(), FMOD_SOFTWARE | FMOD_LOOP_OFF | FMOD_2D, 0, &obj.p_sound));
		m_musics.insert(make_pair(name, obj));
		return true;
	}
	void CMusic::load_from_config(string config)
	{
		ifstream reader;
		reader.open(config.c_str());
		if (!reader.is_open())
			return;

		char buff[1024];
		string type, path, name;
		while (!reader.eof())
		{
			reader.getline(buff, 1024, '\n');
			buff[1024 - 1] = NULL;
			if (buff[0] == '\n' || buff[0] == '\r' || buff[0] == '#')
				continue;
			stringstream ss(buff);
			ss >> type >> name >> path;
			if (type == "music")
			{
				if (!load_music(path, name))
					printf("load music %s fail...", path.c_str());
			}
			else if (type == "sound")
			{
				if (!load_sound(path, name))
					printf("load sound %s fail...", path.c_str());
			}
		}
	}
	bool CMusic::play(string name)
	{
		MusicMap::iterator it = m_musics.find(name);
		if (it == m_musics.end())
			return false;
		if (it->second.type == MUSIC)
		{
			ChannelMap::iterator it = m_channels.find(name);
			if (it == m_channels.end())
				return false;
			_check(FMOD_Channel_SetPaused(it->second, false));
			return true;
		}
		else if (it->second.type == SOUND)
		{
			FMOD_CHANNEL* p_channel = NULL;
			_check(FMOD_System_PlaySound(m_p_system, FMOD_CHANNEL_FREE, it->second.p_sound, false, &p_channel));
			return true;
		}
		return false;
	}
	void CMusic::pause(string name)
	{
		ChannelMap::iterator it = m_channels.find(name);
		if (it == m_channels.end())
			return;
		_check(FMOD_Channel_SetPaused(it->second, true));
	}
	void CMusic::proceed(string name)
	{
		ChannelMap::iterator it = m_channels.find(name);
		if (it == m_channels.end())
			return;
		_check(FMOD_Channel_SetPaused(it->second, false));
	}
	void CMusic::free()
	{
		for (MusicMap::iterator it = m_musics.begin(); it != m_musics.end(); it++)
			_check(FMOD_Sound_Release(it->second.p_sound));
		_check(FMOD_System_Close(m_p_system));
		_check(FMOD_System_Release(m_p_system));
		m_channels.clear();
		m_musics.clear();
	}
	void CMusic::update()
	{
		_check(FMOD_System_Update(m_p_system));
	}

	// 内部方法 ////////////////////////////////////////////////////////////////////////
	void CMusic::_check(FMOD_RESULT r)
	{
		if (r == FMOD_OK)
			return;
		char buf[128];
        sprintf(buf, "FMOD error！FMOD_RESULT:%s", FMOD_ErrorString(r));
        MessageBoxA(NULL, buf, "err", NULL);
	}
}
