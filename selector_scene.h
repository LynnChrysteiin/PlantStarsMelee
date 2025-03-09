#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "animation.h"
#include "camera.h"
#include "player_id.h"
#include "peashooter_player.h"
#include "sunflower_player.h"
#include "scene.h"
#include "scene_manager.h"

extern Player* player_1P;
extern Player* player_2P;

extern IMAGE img_VS;
extern IMAGE img_1P;
extern IMAGE img_2P;
extern IMAGE img_1P_desc;
extern IMAGE img_2P_desc;
extern IMAGE img_gravestone_left;
extern IMAGE img_gravestone_right;
extern IMAGE img_selector_tip;
extern IMAGE img_selector_bg;
extern IMAGE img_1P_selector_btn_idle_left;
extern IMAGE img_1P_selector_btn_idle_right;
extern IMAGE img_1P_selector_btn_down_left;
extern IMAGE img_1P_selector_btn_down_right;
extern IMAGE img_2P_selector_btn_idle_left;
extern IMAGE img_2P_selector_btn_idle_right;
extern IMAGE img_2P_selector_btn_down_left;
extern IMAGE img_2P_selector_btn_down_right;
extern IMAGE img_peashooter_selector_bg_left;
extern IMAGE img_peashooter_selector_bg_right;
extern IMAGE img_sunflower_selector_bg_left;
extern IMAGE img_sunflower_selector_bg_right;

extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_sunflower_idle_right;

extern IMAGE img_avator_peashooter;
extern IMAGE img_avator_sunflower;

extern IMAGE* img_avator_1P;
extern IMAGE* img_avator_2P;

extern SceneManager scene_manager;

class SelectorScene : public Scene
{
private:
	enum class PlayerType
	{
		PEASHOOTER = 0,
		SUNFLOWER,
		INVALID
	};

	POINT pos_img_VS = { 0, 0 };
	POINT pos_img_tip = { 0, 0 };
	POINT pos_img_1P = { 0, 0 };
	POINT pos_img_2P = { 0, 0 };
	POINT pos_img_1P_desc = { 0, 0 };
	POINT pos_img_2P_desc = { 0, 0 };
	POINT pos_txt_1P_name = { 0, 0 };
	POINT pos_txt_2P_name = { 0, 0 };
	POINT pos_anim_1P = { 0, 0 };
	POINT pos_anim_2P = { 0, 0 };
	POINT pos_img_1P_gravestone = { 0, 0 };
	POINT pos_img_2P_gravestone = { 0, 0 };
	POINT pos_img_1P_selector_btn_left = { 0, 0 };
	POINT pos_img_1P_selector_btn_right = { 0, 0 };
	POINT pos_img_2P_selector_btn_left = { 0, 0 };
	POINT pos_img_2P_selector_btn_right = { 0, 0 };

	Animation anim_peashooter;
	Animation anim_sunflower;

	PlayerType player_type_1 = PlayerType::PEASHOOTER;
	PlayerType player_type_2 = PlayerType::SUNFLOWER;

	LPCTSTR str_peashooter_name = _T("Íñ¶ºÉäÊÖ");
	LPCTSTR str_sunflower_name = _T("ÁúÈÕ¿û");

	int selector_bg_scroll_offset_x = 0;

	bool is_1P_selector_btn_left_down = false;
	bool is_1P_selector_btn_right_down = false;
	bool is_2P_selector_btn_left_down = false;
	bool is_2P_selector_btn_right_down = false;

private:
	void outtextxy_shaded(int x, int y, LPCTSTR str)
	{
		settextcolor(RGB(47, 47, 47));
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));
		outtextxy(x, y, str);
	}

public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void on_enter()
	{
		mciSendString(_T("play bgm_menu"), NULL, 0, NULL);
		anim_peashooter.set_atlas(&atlas_peashooter_idle_right);
		anim_sunflower.set_atlas(&atlas_sunflower_idle_right);
		anim_peashooter.set_interval(100);
		anim_sunflower.set_interval(100);
		anim_peashooter.set_loop(true);
		anim_sunflower.set_loop(true);

		static const int OFFSET_X = 50;

		pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;
		pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;

		pos_img_tip.x = (getwidth() - img_selector_tip.getwidth()) / 2;
		pos_img_tip.y = getheight() - 125;

		pos_img_1P.x = (getwidth() / 2 - img_1P.getwidth()) / 2 - OFFSET_X;
		pos_img_1P.y = 35;
		pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P.getwidth()) / 2 + OFFSET_X;
		pos_img_2P.y = pos_img_1P.y;

		pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc.getwidth()) / 2 - OFFSET_X;
		pos_img_1P_desc.y = getheight() - 150;
		pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;
		pos_img_2P_desc.y = pos_img_1P_desc.y;

		pos_img_1P_gravestone.x = (getwidth() / 2 - img_gravestone_right.getwidth()) / 2 - OFFSET_X;
		pos_img_1P_gravestone.y = pos_img_1P.y + img_1P.getheight() + 35;
		pos_img_2P_gravestone.x = getwidth() / 2 + (getwidth() / 2 - img_gravestone_left.getwidth()) / 2 + OFFSET_X;
		pos_img_2P_gravestone.y = pos_img_1P_gravestone.y;

		pos_anim_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 - OFFSET_X;
		pos_anim_1P.y = pos_img_1P_gravestone.y + 80;
		pos_anim_2P.x = getwidth() / 2 + (getwidth() / 2 - atlas_sunflower_idle_right.get_image(0)->getwidth()) / 2 + OFFSET_X;
		pos_anim_2P.y = pos_anim_1P.y;

		pos_txt_1P_name.y = pos_anim_1P.y + 135;
		pos_txt_2P_name.y = pos_txt_1P_name.y;

		pos_img_1P_selector_btn_left.x = pos_img_1P_gravestone.x - img_1P_selector_btn_idle_left.getwidth();
		pos_img_1P_selector_btn_left.y = pos_img_1P_gravestone.y + (img_gravestone_right.getheight() - img_1P_selector_btn_idle_left.getheight()) / 2;
		pos_img_1P_selector_btn_right.x = pos_img_1P_gravestone.x + img_gravestone_right.getwidth();
		pos_img_1P_selector_btn_right.y = pos_img_1P_selector_btn_left.y;
		pos_img_2P_selector_btn_left.x = pos_img_2P_gravestone.x - img_2P_selector_btn_idle_left.getwidth();
		pos_img_2P_selector_btn_left.y = pos_img_1P_selector_btn_left.y;
		pos_img_2P_selector_btn_right.x = pos_img_2P_gravestone.x + img_gravestone_left.getwidth();
		pos_img_2P_selector_btn_right.y = pos_img_1P_selector_btn_left.y;
	}
	void on_update(int dtime)
	{
		anim_peashooter.on_update(dtime);
		anim_sunflower.on_update(dtime);

		selector_bg_scroll_offset_x += 5;
		if (selector_bg_scroll_offset_x >= img_peashooter_selector_bg_left.getwidth())
			selector_bg_scroll_offset_x = 0;
	}
	void on_draw(const Camera& camera)
	{
		IMAGE* img_1P_selector_bg = nullptr;
		IMAGE* img_2P_selector_bg = nullptr;

		switch (player_type_2)
		{
		case PlayerType::PEASHOOTER:
			img_1P_selector_bg = &img_peashooter_selector_bg_right;
			break;
		case PlayerType::SUNFLOWER:
			img_1P_selector_bg = &img_sunflower_selector_bg_right;
			break;
		default:
			img_1P_selector_bg = &img_peashooter_selector_bg_right;
			break;
		}

		switch (player_type_1)
		{
		case PlayerType::PEASHOOTER:
			img_2P_selector_bg = &img_peashooter_selector_bg_left;
			break;
		case PlayerType::SUNFLOWER:
			img_2P_selector_bg = &img_sunflower_selector_bg_left;
			break;
		default:
			img_2P_selector_bg = &img_peashooter_selector_bg_left;
			break;
		}

		putimage(0, 0, &img_selector_bg);

		putimage_with_alpha(selector_bg_scroll_offset_x - img_1P_selector_bg->getwidth(), 0, img_1P_selector_bg);
		putimage_with_alpha(
			selector_bg_scroll_offset_x, 0,
			img_1P_selector_bg->getwidth() - selector_bg_scroll_offset_x, 0,
			img_1P_selector_bg,
			0, 0
		);
		putimage_with_alpha(
			getwidth() - img_2P_selector_bg->getwidth(), 0,
			img_2P_selector_bg->getwidth() - selector_bg_scroll_offset_x, 0,
			img_2P_selector_bg,
			selector_bg_scroll_offset_x, 0
		);
		putimage_with_alpha(getwidth() - selector_bg_scroll_offset_x, 0, img_2P_selector_bg);

		putimage_with_alpha(pos_img_VS.x, pos_img_VS.y, &img_VS);

		putimage_with_alpha(pos_img_1P.x, pos_img_1P.y, &img_1P);
		putimage_with_alpha(pos_img_2P.x, pos_img_2P.y, &img_2P);

		putimage_with_alpha(pos_img_1P_gravestone.x, pos_img_1P_gravestone.y, &img_gravestone_right);
		putimage_with_alpha(pos_img_2P_gravestone.x, pos_img_2P_gravestone.y, &img_gravestone_left);

		switch (player_type_1)
		{
		case PlayerType::PEASHOOTER:
			anim_peashooter.on_draw(camera, pos_anim_1P.x, pos_anim_1P.y);
			pos_txt_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_txt_1P_name.x, pos_txt_1P_name.y, str_peashooter_name);
			break;
		case PlayerType::SUNFLOWER:
			anim_sunflower.on_draw(camera, pos_anim_1P.x, pos_anim_1P.y);
			pos_txt_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_txt_1P_name.x, pos_txt_1P_name.y, str_sunflower_name);
			break;
		}

		switch (player_type_2)
		{
		case PlayerType::PEASHOOTER:
			anim_peashooter.on_draw(camera, pos_anim_2P.x, pos_anim_2P.y);
			pos_txt_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_txt_2P_name.x, pos_txt_2P_name.y, str_peashooter_name);
			break;
		case PlayerType::SUNFLOWER:
			anim_sunflower.on_draw(camera, pos_anim_2P.x, pos_anim_2P.y);
			pos_txt_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_txt_2P_name.x, pos_txt_2P_name.y, str_sunflower_name);
			break;
		}
		putimage_with_alpha(pos_img_1P_selector_btn_left.x, pos_img_1P_selector_btn_left.y,
			is_1P_selector_btn_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
		putimage_with_alpha(pos_img_1P_selector_btn_right.x, pos_img_1P_selector_btn_right.y,
			is_1P_selector_btn_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
		putimage_with_alpha(pos_img_2P_selector_btn_left.x, pos_img_2P_selector_btn_left.y,
			is_2P_selector_btn_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
		putimage_with_alpha(pos_img_2P_selector_btn_right.x, pos_img_2P_selector_btn_right.y,
			is_2P_selector_btn_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

		putimage_with_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
		putimage_with_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);

		putimage_with_alpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);
	}
	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case 0x41:
				is_1P_selector_btn_left_down = true;
				break;
			case 0x44:
				is_1P_selector_btn_right_down = true;
				break;
			case VK_LEFT:
				is_2P_selector_btn_left_down = true;
				break;
			case VK_RIGHT:
				is_2P_selector_btn_right_down = true;
				break;
			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
			case 0x41:
				is_1P_selector_btn_left_down = false;
				player_type_1 = (PlayerType)(((int)PlayerType::INVALID + (int)player_type_1 - 1) % (int)PlayerType::INVALID);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
			case 0x44:
				is_1P_selector_btn_right_down = false;
				player_type_1 = (PlayerType)(((int)player_type_1 + 1) % (int)PlayerType::INVALID);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
			case VK_LEFT:
				is_2P_selector_btn_left_down = false;
				player_type_2 = (PlayerType)(((int)PlayerType::INVALID + (int)player_type_2 - 1) % (int)PlayerType::INVALID);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
			case VK_RIGHT:
				is_2P_selector_btn_right_down = false;
				player_type_2 = (PlayerType)(((int)player_type_2 + 1) % (int)PlayerType::INVALID);
				mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
				break;
			case VK_RETURN:
				scene_manager.switch_to(SceneManager::SceneType::GAME);
				mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	void on_exit()
	{
		switch (player_type_1)
		{
		case PlayerType::PEASHOOTER:
			player_1P = new PeashooterPlayer();
			img_avator_1P = &img_avator_peashooter;
			break;
		case PlayerType::SUNFLOWER:
			player_1P = new SunflowerPlayer();
			img_avator_1P = &img_avator_sunflower;
			break;
		default:
			break;
		}
		player_1P->set_id(PlayerID::P1);

		switch (player_type_2)
		{
		case PlayerType::PEASHOOTER:
			player_2P = new PeashooterPlayer(false);
			img_avator_2P = &img_avator_peashooter;
			break;
		case PlayerType::SUNFLOWER:
			player_2P = new SunflowerPlayer(false);
			img_avator_2P = &img_avator_sunflower;
			break;
		default:
			break;
		}
		player_2P->set_id(PlayerID::P2);

		mciSendString(_T("stop bgm_menu"), NULL, 0, NULL);
	}
};

#endif // !_SELECTOR_SCENE_H_
