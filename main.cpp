#include "atlas.h"
#include "bullet.h"
#include "game_scene.h"
#include "menu_scene.h"
#include "platform.h"
#include "player.h"
#include "scene.h"
#include "scene_manager.h"
#include "selector_scene.h"
#include "utils.h"

#include <graphics.h>
#include <vector>
#pragma comment(lib, "Winmm.lib")

bool is_debug = false;					// 是否开启调试模式

// 全局游戏资源
IMAGE img_menu_bg;						// 主菜单背景图片

IMAGE img_VS;							// 对战界面VS图片
IMAGE img_1P;							// 1P 角色图片
IMAGE img_2P;							// 2P 角色图片
IMAGE img_1P_desc;						// 1P 角色描述图片
IMAGE img_2P_desc;						// 2P 角色描述图片
IMAGE img_gravestone_left;				// 朝向左侧墓碑图片
IMAGE img_gravestone_right;				// 朝向右侧墓碑图片
IMAGE img_selector_tip;					// 角色选择提示图片
IMAGE img_selector_bg;					// 角色选择背景图片
IMAGE img_1P_selector_btn_idle_left;	// 1P 向左选择按钮默认状态图片
IMAGE img_1P_selector_btn_idle_right;	// 1P 向右选择按钮默认状态图片
IMAGE img_1P_selector_btn_down_left;	// 1P 向左选择按钮按下状态图片
IMAGE img_1P_selector_btn_down_right;	// 1P 向右选择按钮按下状态图片
IMAGE img_2P_selector_btn_idle_left;	// 2P 向左选择按钮默认状态图片
IMAGE img_2P_selector_btn_idle_right;	// 2P 向右选择按钮默认状态图片
IMAGE img_2P_selector_btn_down_left;	// 2P 向左选择按钮按下状态图片
IMAGE img_2P_selector_btn_down_right;	// 2P 向右选择按钮按下状态图片
IMAGE img_peashooter_selector_bg_left;	// 选角界面朝左的婉逗射手背景图片
IMAGE img_peashooter_selector_bg_right;	// 选角界面朝右的婉逗射手背景图片
IMAGE img_sunflower_selector_bg_left;	// 选角界面朝左的龙日葵背景图片
IMAGE img_sunflower_selector_bg_right;	// 选角界面朝右的龙日葵背景图片

IMAGE img_sky;							// 游戏场景天空图片
IMAGE img_hills;						// 游戏场景山丘图片
IMAGE img_platform_large;				// 游戏场景大平台图片
IMAGE img_platform_small;				// 游戏场景小平台图片

IMAGE img_1P_cursor;					// 1P 游标图片
IMAGE img_2P_cursor;					// 2P 游标图片

Atlas atlas_peashooter_idle_left;		// 婉逗射手朝左默认动画图集
Atlas atlas_peashooter_idle_right;		// 婉逗射手朝右默认动画图集
Atlas atlas_peashooter_run_left;		// 婉逗射手朝左奔跑动画图集
Atlas atlas_peashooter_run_right;		// 婉逗射手朝右奔跑动画图集
Atlas atlas_peashooter_attack_ex_left;	// 婉逗射手朝左特殊攻击动画图集
Atlas atlas_peashooter_attack_ex_right;	// 婉逗射手朝右特殊攻击动画图集
Atlas atlas_peashooter_die_left;		// 婉逗射手朝左死亡动画图集
Atlas atlas_peashooter_die_right;		// 婉逗射手朝右死亡动画图集

Atlas atlas_sunflower_idle_left;		// 龙日葵朝左默认动画图集
Atlas atlas_sunflower_idle_right;		// 龙日葵朝右默认动画图集
Atlas atlas_sunflower_run_left;			// 龙日葵朝左奔跑动画图集
Atlas atlas_sunflower_run_right;		// 龙日葵朝右奔跑动画图集
Atlas atlas_sunflower_attack_ex_left;	// 龙日葵朝左特殊攻击动画图集
Atlas atlas_sunflower_attack_ex_right;	// 龙日葵朝右特殊攻击动画图集
Atlas atlas_sunflower_die_left;			// 龙日葵朝左死亡动画图集
Atlas atlas_sunflower_die_right;		// 龙日葵朝右死亡动画图集

IMAGE img_pea;							// 豌豆图片
Atlas atlas_pea_break;					// 豌豆破碎动画图集
Atlas atlas_sun;						// 阳光图片
Atlas atlas_sun_explode;				// 阳光爆炸动画图集
Atlas atlas_sun_ex;						// 特殊阳光动画图集
Atlas atlas_sun_ex_explode;				// 特殊阳光爆炸动画图集
Atlas atlas_sun_text;					// "日" 文本动画图集

Atlas atlas_run_effect;					// 奔跑特效动画图集
Atlas atlas_jump_effect;				// 跳跃特效动画图集
Atlas atlas_land_effect;				// 落地特效动画图集

IMAGE img_1P_winner;					// 1P 胜利图片
IMAGE img_2P_winner;					// 2P 胜利图片
IMAGE img_winner_bar;					// 获胜玩家文本背景图片

IMAGE img_avator_peashooter;			// 婉逗射手头像图片
IMAGE img_avator_sunflower;				// 龙日葵头像图片

Scene* menu_scene = nullptr;			// 主菜单场景
Scene* selector_scene = nullptr;		// 角色选择场景
Scene* game_scene = nullptr;			// 游戏场景

SceneManager scene_manager;				// 场景管理器

Camera main_camera;						// 主摄像机
Player* player_1P = nullptr;			// 1P 玩家
Player* player_2P = nullptr;			// 2P 玩家

IMAGE* img_avator_1P = nullptr;			// 1P 玩家头像
IMAGE* img_avator_2P = nullptr;			// 2P 玩家头像

std::vector<Platform> platform_list;	// 平台集合
std::vector<Bullet*> bullet_list;		// 子弹集合

static void flip_atlas(Atlas& src, Atlas& dst)
{
	dst.clear();
	for (int i = 0; i < src.get_size(); ++i)
	{
		IMAGE img_flipped;
		flip_image(src.get_image(i), &img_flipped);
		dst.add_image(img_flipped);
	}
}

static void load_game_resources()
{
	AddFontResourceEx(_T("resources/IPix.ttf"), FR_PRIVATE, NULL);

	loadimage(&img_menu_bg, _T("resources/menu_background.png"));

	loadimage(&img_VS, _T("resources/VS.png"));
	loadimage(&img_1P, _T("resources/1P.png"));
	loadimage(&img_2P, _T("resources/2P.png"));
	loadimage(&img_1P_desc, _T("resources/1P_desc.png"));
	loadimage(&img_2P_desc, _T("resources/2P_desc.png"));
	loadimage(&img_gravestone_right, _T("resources/gravestone.png"));
	flip_image(&img_gravestone_right, &img_gravestone_left);
	loadimage(&img_selector_tip, _T("resources/selector_tip.png"));
	loadimage(&img_selector_bg, _T("resources/selector_background.png"));
	loadimage(&img_1P_selector_btn_idle_right, _T("resources/1P_selector_btn_idle.png"));
	flip_image(&img_1P_selector_btn_idle_right, &img_1P_selector_btn_idle_left);
	loadimage(&img_1P_selector_btn_down_right, _T("resources/1P_selector_btn_down.png"));
	flip_image(&img_1P_selector_btn_down_right, &img_1P_selector_btn_down_left);
	loadimage(&img_2P_selector_btn_idle_right, _T("resources/2P_selector_btn_idle.png"));
	flip_image(&img_2P_selector_btn_idle_right, &img_2P_selector_btn_idle_left);
	loadimage(&img_2P_selector_btn_down_right, _T("resources/2P_selector_btn_down.png"));
	flip_image(&img_2P_selector_btn_down_right, &img_2P_selector_btn_down_left);
	loadimage(&img_peashooter_selector_bg_right, _T("resources/peashooter_selector_background.png"));
	flip_image(&img_peashooter_selector_bg_right, &img_peashooter_selector_bg_left);
	loadimage(&img_sunflower_selector_bg_right, _T("resources/sunflower_selector_background.png"));
	flip_image(&img_sunflower_selector_bg_right, &img_sunflower_selector_bg_left);

	loadimage(&img_sky, _T("resources/sky.png"));
	loadimage(&img_hills, _T("resources/hills.png"));
	loadimage(&img_platform_large, _T("resources/platform_large.png"));
	loadimage(&img_platform_small, _T("resources/platform_small.png"));

	loadimage(&img_1P_cursor, _T("resources/1P_cursor.png"));
	loadimage(&img_2P_cursor, _T("resources/2P_cursor.png"));

	atlas_peashooter_idle_right.load_from_file(_T("resources/peashooter_idle_%d.png"), 9);
	flip_atlas(atlas_peashooter_idle_right, atlas_peashooter_idle_left);
	atlas_peashooter_run_right.load_from_file(_T("resources/peashooter_run_%d.png"), 5);
	flip_atlas(atlas_peashooter_run_right, atlas_peashooter_run_left);
	atlas_peashooter_attack_ex_right.load_from_file(_T("resources/peashooter_attack_ex_%d.png"), 3);
	flip_atlas(atlas_peashooter_attack_ex_right, atlas_peashooter_attack_ex_left);
	atlas_peashooter_die_right.load_from_file(_T("resources/peashooter_die_%d.png"), 4);
	flip_atlas(atlas_peashooter_die_right, atlas_peashooter_die_left);

	atlas_sunflower_idle_right.load_from_file(_T("resources/sunflower_idle_%d.png"), 8);
	flip_atlas(atlas_sunflower_idle_right, atlas_sunflower_idle_left);
	atlas_sunflower_run_right.load_from_file(_T("resources/sunflower_run_%d.png"), 5);
	flip_atlas(atlas_sunflower_run_right, atlas_sunflower_run_left);
	atlas_sunflower_attack_ex_right.load_from_file(_T("resources/sunflower_attack_ex_%d.png"), 9);
	flip_atlas(atlas_sunflower_attack_ex_right, atlas_sunflower_attack_ex_left);
	atlas_sunflower_die_right.load_from_file(_T("resources/sunflower_die_%d.png"), 2);
	flip_atlas(atlas_sunflower_die_right, atlas_sunflower_die_left);

	loadimage(&img_pea, _T("resources/pea.png"));
	atlas_pea_break.load_from_file(_T("resources/pea_break_%d.png"), 3);
	atlas_sun.load_from_file(_T("resources/sun_%d.png"), 5);
	atlas_sun_explode.load_from_file(_T("resources/sun_explode_%d.png"), 5);
	atlas_sun_ex.load_from_file(_T("resources/sun_ex_%d.png"), 5);
	atlas_sun_ex_explode.load_from_file(_T("resources/sun_ex_explode_%d.png"), 5);
	atlas_sun_text.load_from_file(_T("resources/sun_text_%d.png"), 6);

	atlas_run_effect.load_from_file(_T("resources/run_effect_%d.png"), 4);
	atlas_jump_effect.load_from_file(_T("resources/jump_effect_%d.png"), 5);
	atlas_land_effect.load_from_file(_T("resources/land_effect_%d.png"), 2);

	loadimage(&img_1P_winner, _T("resources/1P_winner.png"));
	loadimage(&img_2P_winner, _T("resources/2P_winner.png"));
	loadimage(&img_winner_bar, _T("resources/winner_bar.png"));

	loadimage(&img_avator_peashooter, _T("resources/avatar_peashooter.png"));
	loadimage(&img_avator_sunflower, _T("resources/avatar_sunflower.png"));

	mciSendString(_T("open resources/bgm_game.mp3 alias bgm_game"), NULL, 0, NULL);
	mciSendString(_T("open resources/bgm_menu.mp3 alias bgm_menu"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_1.mp3 alias pea_break_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_2.mp3 alias pea_break_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_3.mp3 alias pea_break_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_shoot_1.mp3 alias pea_shoot_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_shoot_2.mp3 alias pea_shoot_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_shoot_ex.mp3 alias pea_shoot_ex"), NULL, 0, NULL);
	mciSendString(_T("open resources/sun_explode.mp3 alias sun_explode"), NULL, 0, NULL);
	mciSendString(_T("open resources/sun_ex_explode.mp3 alias sun_ex_explode"), NULL, 0, NULL);
	mciSendString(_T("open resources/sun_text.mp3 alias sun_text"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_confirm.wav alias ui_confirm"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_switch.wav alias ui_switch"), NULL, 0, NULL);
	mciSendString(_T("open resources/ui_win.wav alias ui_win"), NULL, 0, NULL);
}

int main(int argc, char** argv)
{
	ExMessage msg;
	const int FPS = 60;

	initgraph(1280, 720, EW_SHOWCONSOLE);
	load_game_resources();

	settextstyle(28, 0, _T("IPix"));
	setbkmode(TRANSPARENT);

	BeginBatchDraw();

	menu_scene = new MenuScene();
	selector_scene = new SelectorScene();
	game_scene = new GameScene();
	scene_manager.set_current_scene(menu_scene);

	while (true)
	{
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			scene_manager.on_input(msg);
		}

		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick = current_tick_time - last_tick_time;
		scene_manager.on_update(delta_tick);
		last_tick_time = current_tick_time;

		cleardevice();
		scene_manager.on_draw(main_camera);
		FlushBatchDraw();

		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / FPS)
			Sleep(1000 / FPS - frame_delta_time);
	}

	EndBatchDraw();
	return 0;
}