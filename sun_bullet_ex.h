#ifndef _SUN_BULLET_EX_H_
#define _SUN_BULLET_EX_H_

#include "animation.h"
#include "bullet.h"

extern Atlas atlas_sun_ex;
extern Atlas atlas_sun_ex_explode;
extern Atlas atlas_sun_ex_text;
extern Camera main_camera;

class SunBulletEx : public Bullet
{
private:
	Animation animation_default;
	Animation animation_explode;
	Vector2 explode_render_offset;

public:
	SunBulletEx()
	{
		size.x = 288;
		size.y = 288;
		damage = 20;

		animation_default.set_atlas(&atlas_sun_ex);
		animation_default.set_interval(50);
		animation_default.set_loop(true);

		animation_explode.set_atlas(&atlas_sun_ex_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() {can_remove = true; });

		IMAGE* frame_default = animation_default.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();
		explode_render_offset.x = (frame_default->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_default->getheight() - frame_explode->getheight()) / 2.0f;
	}
	~SunBulletEx() = default;

	void on_collide()
	{
		Bullet::on_collide();
		main_camera.shake(20, 350);
		mciSendString(_T("play sun_ex_explode from 0"), NULL, 0, NULL);
	}
	bool check_collision(const Vector2& position, const Vector2& size)
	{
		bool is_collide_x = (max(this->position.x + this->size.x, position.x + size.x) - min(this->position.x, position.x) <= this->size.x + size.x);
		bool is_collide_y = (max(this->position.y + this->size.y, position.y + size.y) - min(this->position.y, position.y) <= this->size.y + size.y);
		return is_collide_x && is_collide_y;
	}
	void on_update(int dtime)
	{
		if (valid)
		{
			position += velocity * (float)dtime;
		}
		if (!valid)
		{
			animation_explode.on_update(dtime);
		}
		else
		{
			animation_default.on_update(dtime);
		}
		if (check_if_exceeds_screen())
		{
			can_remove = true;
		}
	}
	void on_draw(const Camera& camera) const
	{
		if (valid)
		{
			animation_default.on_draw(camera, (int)position.x, (int)position.y);
		}
		else
		{
			animation_explode.on_draw(camera, (int)(position.x + explode_render_offset.x), (int)(position.y + explode_render_offset.y));
		}

		Bullet::on_draw(camera);
	}
};

#endif // !_SUN_BULLET_EX_H_
