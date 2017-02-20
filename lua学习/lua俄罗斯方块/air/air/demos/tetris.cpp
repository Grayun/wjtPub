#include "../air.h"
#include <stdio.h>
#include <cstdlib>
#include <math.h>
using namespace air;
CImageFont g_img_font1;
CImageFont g_img_font2;
void init()
{
	//srand(GetTickCount());
	g_p_tex_mgr->load_from_config("res/tetris/texture.txt");
	g_p_pen_mgr->create_from_config("res/tetris/pen.txt");
	g_p_music->load_from_config("res/tetris/music.txt");
	g_img_font1.create("font1");
	g_img_font2.create("font2");
	g_p_script->load("res/tetris/logic.lua");
	g_p_script->call("main_init");
}
void frame()
{
	g_p_script->call("main_update");
}
void draw()
{
	//string s = "x:" + tostr(g_p_input->get_mouse_x()) + " y:" + tostr(g_p_input->get_mouse_y());
    //g_p_pen_mgr->print("3", 100.0f, 400.0f, color(0,255,0), s);
	g_p_script->call("main_draw");
	g_img_font2.print(200, 410, "QQDY");
	g_img_font1.print(55, 435, "TETRIS");
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	g_p_system = new CSystem(320, 480, "AirGE by qq_d_y --- tetris");
	g_p_system->set_callback(CSystem::INIT, init);
	g_p_system->set_callback(CSystem::DRAW, draw);
	g_p_system->set_callback(CSystem::FRAME, frame);
	g_p_system->run();
	return 0;
}
