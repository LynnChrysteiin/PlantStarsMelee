#ifndef _SUN_BULLET_H_
#define _SUN_BULLET_H_

#include "animation.h"
#include "bullet.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;
extern Camera main_camera;

class SunBullet : public Bullet
{
private:
	Animation animation_default;
	Animation animation_explode;
	Vector2 explode_render_offset;

	const float gravity = 1e-3f;

public:
	SunBullet()
	{
		size.x = 96;
		size.y = 96;
		damage = 20;

		animation_default.set_atlas(&atlas_sun);
		animation_default.set_interval(50);
		animation_default.set_loop(true);

		animation_explode.set_atlas(&atlas_sun_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() {can_remove = true; });

		IMAGE* frame_default = animation_default.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();
		explode_render_offset.x = (frame_default->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_default->getheight() - frame_explode->getheight()) / 2.0f;
	}
	~SunBullet() = default;

	void on_collide()
	{
		Bullet::on_collide();
		main_camera.shake(5, 250);
		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
	}
	void on_update(int dtime)
	{
		if (valid)
		{
			velocity.y += gravity * dtime;
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

#endif // !_SUN_BULLET_H_
