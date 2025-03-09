#ifndef _BULLET_H_
#define _BULLET_H_

#include "camera.h"
#include "player_id.h"
#include "utils.h"
#include "Vector2.h"

#include <functional>
#include <graphics.h>

extern bool is_debug;

class Bullet
{
protected:
	Vector2 size;
	Vector2 position;
	Vector2 velocity;
	int damage = 0;
	bool valid = true;
	bool can_remove = false;
	std::function<void()> call_back;
	PlayerID target_id = PlayerID::NONE;

protected:
	bool check_if_exceeds_screen() const
	{
		return (position.x + size.x <= 0 || position.x >= getwidth())
			|| (position.y + size.y <= 0 || position.y >= getheight());
	}

public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int dmg)
	{
		damage = dmg;
	}
	int get_damage() const
	{
		return damage;
	}
	void set_position(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
	const Vector2& get_position() const
	{
		return position;
	}
	const Vector2& get_size() const
	{
		return size;
	}
	void set_velocity(float x, float y)
	{
		velocity.x = x;
		velocity.y = y;
	}
	void set_collide_target(PlayerID id)
	{
		target_id = id;
	}
	PlayerID get_collide_target() const
	{
		return target_id;
	}
	void set_callback(std::function<void()> cb)
	{
		call_back = cb;
	}
	void set_valid(bool v)
	{
		valid = v;
	}
	bool is_valid() const
	{
		return valid;
	}
	bool is_can_remove() const
	{
		return can_remove;
	}
	virtual void on_collide()
	{
		if (call_back)
		{
			call_back();
		}
	}
	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}
	virtual void on_update(int dtime) = 0;
	virtual void on_draw(const Camera& camera) const
	{
		if (is_debug)
		{
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			rectangle(camera, (int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
			solidcircle(camera, (int)(position.x + size.x / 2), (int)(position.y + size.y / 2), 5);
		}
	}
};

#endif // !_BULLET_H_
