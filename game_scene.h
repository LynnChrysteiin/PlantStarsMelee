#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "camera.h"
#include "platform.h"
#include "player.h"
#include "scene.h"
#include "scene_manager.h"
#include "status_bar.h"
#include "utils.h"

#include <algorithm>
#include <iostream>

extern Player* player_1P;
extern Player* player_2P;

extern IMAGE* img_avator_1P;
extern IMAGE* img_avator_2P;

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern IMAGE img_1P_winner;
extern IMAGE img_2P_winner;
extern IMAGE img_winner_bar;

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

extern Camera main_camera;

extern SceneManager scene_manager;

class GameScene : public Scene
{
private:
	POINT pos_img_sky = { 0, 0 };			// 天空图片位置
	POINT pos_img_hills = { 0, 0 };			// 山丘图片位置

	StatusBar status_bar_1P;				// 玩家 1P 状态栏
	StatusBar status_bar_2P;				// 玩家 2P 状态栏

	bool is_game_over = false;				// 游戏是否结束

	POINT pos_img_winner_bar = { 0, 0 };	// 结算动效背景位置
	POINT pos_img_winner_text = { 0, 0 };	// 结算动效文字位置
	int pos_x_img_winner_bar_dst = 0;		// 结算动效背景移动目标位置
	int pos_x_img_winner_text_dst = 0;		// 结算动效文字移动目标位置
	float speed_winner_bar = 2.0f;			// 结算动效背景移动速度
	float speed_winner_text = 1.5f;			// 结算动效文字移动速度
	Timer timer_winner_slide_in;			// 结算动效滑入计时器
	Timer timer_winner_slide_out;			// 结算动效滑出计时器
	bool is_slide_out_started = false;		// 结算动效滑出是否已经开始

public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter()
	{
		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);

		is_game_over = false;
		is_slide_out_started = false;

		pos_img_winner_bar.x = -img_winner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

		pos_img_winner_text.x = pos_img_winner_bar.x;
		pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_time_waiting(4000);
		timer_winner_slide_in.set_shot_once(true);
		timer_winner_slide_in.set_callback([&]() { is_slide_out_started = true; });

		timer_winner_slide_out.restart();
		timer_winner_slide_out.set_time_waiting(1500);
		timer_winner_slide_out.set_shot_once(true);
		timer_winner_slide_out.set_callback([&]() { scene_manager.switch_to(SceneManager::SceneType::MENU); });

		player_1P->set_position(200, 50);
		player_2P->set_position(975, 50);

		status_bar_1P.set_avator(img_avator_1P);
		status_bar_1P.set_position(235, 625);
		status_bar_2P.set_avator(img_avator_2P);
		status_bar_2P.set_position(675, 625);

		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

		platform_list.resize(4);

		Platform& platform_large = platform_list[0];
		platform_large.img = &img_platform_large;
		platform_large.render_pos.x = 122;
		platform_large.render_pos.y = 455;
		platform_large.shape.left_x = (float)platform_large.render_pos.x + 30;
		platform_large.shape.right_x = (float)platform_large.render_pos.x + img_platform_large.getwidth() - 30;
		platform_large.shape.y = (float)platform_large.render_pos.y + 60;

		Platform& platform_small_1 = platform_list[1];
		platform_small_1.img = &img_platform_small;
		platform_small_1.render_pos.x = 175;
		platform_small_1.render_pos.y = 360;
		platform_small_1.shape.left_x = (float)platform_small_1.render_pos.x + 40;
		platform_small_1.shape.right_x = (float)platform_small_1.render_pos.x + img_platform_small.getwidth() - 40;
		platform_small_1.shape.y = (float)platform_small_1.render_pos.y + img_platform_small.getheight() / 2.0f;

		Platform& platform_small_2 = platform_list[2];
		platform_small_2.img = &img_platform_small;
		platform_small_2.render_pos.x = 855;
		platform_small_2.render_pos.y = 360;
		platform_small_2.shape.left_x = (float)platform_small_2.render_pos.x + 40;
		platform_small_2.shape.right_x = (float)platform_small_2.render_pos.x + img_platform_small.getwidth() - 40;
		platform_small_2.shape.y = (float)platform_small_2.render_pos.y + img_platform_small.getheight() / 2.0f;

		Platform& platform_small_3 = platform_list[3];
		platform_small_3.img = &img_platform_small;
		platform_small_3.render_pos.x = 515;
		platform_small_3.render_pos.y = 225;
		platform_small_3.shape.left_x = (float)platform_small_3.render_pos.x + 40;
		platform_small_3.shape.right_x = (float)platform_small_3.render_pos.x + img_platform_small.getwidth() - 40;
		platform_small_3.shape.y = (float)platform_small_3.render_pos.y + img_platform_small.getheight() / 2.0f;
	}
	void on_update(int dtime)
	{
		player_1P->on_update(dtime);
		player_2P->on_update(dtime);

		main_camera.on_update(dtime);

		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet)
			{
				bool deletable = bullet->is_can_remove();
				if (deletable) delete bullet;
				return deletable;
			}),
			bullet_list.end()
		);

		// 删除子弹的逻辑等价于下面的代码：
		// 
		//	for (int i = 0; i < bullet_list.size(); i++)
		//	{
		//		if (bullet_list[i]->is_can_remove())
		//		{
		//			std::swap(bullet_list[i], bullet_list.back());
		//			delete bullet_list.back();
		//			bullet_list.pop_back();
		//		}
		//	}

		for (auto bullet : bullet_list)
			bullet->on_update(dtime);

		const Vector2& position_1P = player_1P->get_position();
		const Vector2& position_2P = player_2P->get_position();
		if (position_1P.y >= getheight())
			player_1P->set_hp(0);
		if (position_2P.y >= getheight())
			player_2P->set_hp(0);
		if (player_1P->get_hp() <= 0 || player_2P->get_hp() <= 0)
		{
			if (!is_game_over)
			{
				mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
				mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
			}
			is_game_over = true;
		}

		if (status_bar_1P.get_hp() != player_1P->get_hp())
			status_bar_1P.add_delta_hp(player_1P->get_hp() - status_bar_1P.get_hp());
		if (status_bar_1P.get_mp() != player_1P->get_mp())
			status_bar_1P.add_delta_mp(player_1P->get_mp() - status_bar_1P.get_mp());
		if (status_bar_2P.get_hp() != player_2P->get_hp())
			status_bar_2P.add_delta_hp(player_2P->get_hp() - status_bar_2P.get_hp());
		if (status_bar_2P.get_mp() != player_2P->get_mp())
			status_bar_2P.add_delta_mp(player_2P->get_mp() - status_bar_2P.get_mp());

		status_bar_1P.on_update(dtime);
		status_bar_2P.on_update(dtime);

		status_bar_1P.set_hp(player_1P->get_hp());
		status_bar_1P.set_mp(player_1P->get_mp());
		status_bar_2P.set_hp(player_2P->get_hp());
		status_bar_2P.set_mp(player_2P->get_mp());

		if (is_game_over)
		{
			pos_img_winner_bar.x += (int)(speed_winner_bar * dtime);
			pos_img_winner_text.x += (int)(speed_winner_text * dtime);

			if (!is_slide_out_started)
			{
				timer_winner_slide_in.on_update(dtime);
				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst)
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst)
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
				std::cout << pos_img_winner_bar.x << ", " << pos_img_winner_text.x << std::endl;
			}
			else
				timer_winner_slide_out.on_update(dtime);
		}
	}
	void on_draw(const Camera& camera)
	{
		putimage_with_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_with_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

		for (const auto& platform : platform_list)
			platform.on_draw(camera);

		if (is_debug)
		{
			settextcolor(RGB(127, 127, 127));
			outtextxy(16, 16, _T("已开启调试模式，按 F1 关闭"));
			settextcolor(RGB(0, 255, 0));
			outtextxy(15, 15, _T("已开启调试模式，按 F1 关闭"));
		}

		player_1P->on_draw(camera);
		player_2P->on_draw(camera);

		for (const Bullet* bullet : bullet_list)
			bullet->on_draw(camera);

		if (is_game_over)
		{
			putimage_with_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			putimage_with_alpha(pos_img_winner_text.x, pos_img_winner_text.y, 
				player_1P->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}
		else
		{
			status_bar_1P.on_draw();
			status_bar_2P.on_draw();
		}
	}
	void on_input(const ExMessage& msg)
	{
		player_1P->on_input(msg);
		player_2P->on_input(msg);

		switch (msg.message)
		{
		case WM_KEYUP:
			// 'F1'
			if (msg.vkcode == VK_F1)
				is_debug = !is_debug;
			break;
		default:
			break;
		}
	}
	void on_exit()
	{
		delete player_1P;
		delete player_2P;
		player_1P = nullptr;
		player_2P = nullptr;

		is_debug = false;

		bullet_list.clear();
		main_camera.reset();
	}
};

#endif // !_GAME_SCENE_H_
