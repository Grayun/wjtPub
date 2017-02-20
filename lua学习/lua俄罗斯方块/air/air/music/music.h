#ifndef music_h_
#define music_h_

#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"
#include <string>
#include <map>
using std::map;
using std::string;

namespace air
{
	class CMusic
	{
	private:
		enum { MUSIC, SOUND };
		struct data
		{
			int type;
			FMOD_SOUND* p_sound;
		};
		typedef map<string, data>           MusicMap;
		typedef map<string, FMOD_CHANNEL*>  ChannelMap;
	private:
		FMOD_SYSTEM*	m_p_system;
		MusicMap		m_musics;
		ChannelMap		m_channels;
	public:
		CMusic();
		~CMusic();
		/*
			载入音乐/音效。音乐可以循环播放，可以暂停；音效只播放一次
			file：		文件路径
			name：		索引名
		*/
		bool load_music(string file, string name);
		bool load_sound(string file, string name);
		/*
			根据配置文件载入音乐资源
			config：		配置文件名
		*/
		void load_from_config(string config);
		/*
			播放指定的音乐
		*/
		bool play(string name);
		/*
			暂停播放
		*/
		void pause(string name);
		/*
			继续播放
		*/
		void proceed(string name);
		/*
			内存释放
		*/
		void free();
		/*
			更新
		*/
		void update();
	private:
		void _check(FMOD_RESULT r);
	private:
		CMusic(const CMusic&);
		CMusic& operator=(const CMusic&);
	};
	extern CMusic* g_p_music;
}

#endif
