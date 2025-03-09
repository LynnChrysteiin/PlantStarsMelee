#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include "utils.h"

#include <graphics.h>

class StatusBar
{
private:
	int mp, hp;				// mp和hp的实际值
	int delta_mp, delta_hp;	// mp和hp的增减量
	float cur_mp, cur_hp;	// mp和hp的当前值, 用于动画效果

	POINT position = { 0, 0 };
	IMAGE* img_avator = nullptr;

	const int width = 275;

public:
	StatusBar() : mp(0), hp(100), delta_mp(0), delta_hp(0), cur_mp(0), cur_hp(100) {}
	~StatusBar() = default;

	void set_avator(IMAGE* img)
	{
		img_avator = img;
	}
	void set_position(int x, int y)
	{
		position.x = x;
		position.y = y;
	}
	void set_mp(int mp)
	{
		this->mp = mp;
	}
	void set_hp(int hp)
	{
		this->hp = hp;
	}
	void add_delta_mp(int delta_mp)
	{
		this->delta_mp += delta_mp;
	}
	void add_delta_hp(int delta_hp)
	{
		this->delta_hp += delta_hp;
	}
	int get_mp() const
	{
		return mp;
	}
	int get_hp() const
	{
		return hp;
	}
	void on_update(int dtime)
	{
		if (cur_hp > hp)
			cur_hp -= (float)dtime / 60.0f;
		if (cur_hp < hp)
			cur_hp = hp;
		if (cur_mp > mp)
			cur_mp -= (float)dtime / 15.0f;
		if (cur_mp < mp)
			cur_mp = mp;
	}
	void on_draw()
	{
		putimage_with_alpha(position.x, position.y, img_avator);

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3 * 2, position.y + 36, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3 * 2, position.y + 71, 8, 8);
		setfillcolor(RGB(67, 47, 47));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3, position.y + 33, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3, position.y + 68, 8, 8);

		float cur_hp_bar_width = width * max(0, cur_hp) / 100.0f;
		float cur_mp_bar_width = width * min(100, cur_mp) / 100.0f;
		setfillcolor(RGB(255, 255, 255));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + (int)cur_hp_bar_width + 3, position.y + 33, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + (int)cur_mp_bar_width + 3, position.y + 68, 8, 8);

		float hp_bar_width = width * max(0, hp) / 100.0f;
		float mp_bar_width = width * min(100, mp) / 100.0f;
		setfillcolor(RGB(197, 61, 67));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + (int)hp_bar_width + 3, position.y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + (int)mp_bar_width + 3, position.y + 68, 8, 8);
	}
};

#endif // !_STATUS_BAR_H_
