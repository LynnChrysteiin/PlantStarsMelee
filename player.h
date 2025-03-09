#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "animation.h"
#include "bullet.h"
#include "camera.h"
#include "particle.h"
#include "platform.h"
#include "player_id.h"
#include "timer.h"
#include "vector2.h"

#include <functional>
#include <graphics.h>

extern bool is_debug;

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

extern IMAGE img_1P_cursor;
extern IMAGE img_2P_cursor;

extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;

class Player
{
protected:
	PlayerID id = PlayerID::NONE;			// ��ұ��

	int hp = 100;							// ����ֵ
	int mp = 0;								// ħ��ֵ

	Vector2 size;							// ��ײ���
	Vector2 position;						// λ��
	Vector2 velocity;						// �ٶ�

	const float run_velocity = 0.4f;		// �ƶ��ٶ�
	const float jump_velocity = -0.8f;		// ��Ծ�ٶ�
	const float gravity = 1.6e-3f;			// �������ٶ�
	bool jump = false;						// �Ƿ������Ծ

	Animation animation_idle_left;			// վ������(��)
	Animation animation_idle_right;			// վ������(��)
	Animation animation_run_left;			// �ƶ�����(��)
	Animation animation_run_right;			// �ƶ�����(��)
	Animation animation_die_left;			// ��������(��)
	Animation animation_die_right;			// ��������(��)
	Animation animation_attack_ex_left;		// ���⹥������(��)
	Animation animation_attack_ex_right;	// ���⹥������(��)
	Animation animation_jump_effect;		// ��Ծ��Ч����
	Animation animation_land_effect;		// �����Ч����

	bool is_jump_effect_visible = false;	// ��Ծ�����Ƿ�ɼ�
	bool is_land_effect_vivible = false;	// ��ض����Ƿ�ɼ�

	Vector2 position_jump_effect;			// ��Ծ����λ��
	Vector2 position_land_effect;			// ��ض���λ��

	Animation* current_animation = nullptr;	// ��ǰ����

	bool is_cursor_visible = true;			// ��ҹ��ָʾ�Ƿ�ɼ�
	Timer timer_cursor_visibility;			// ��ҹ��ָʾ�ɼ��Զ�ʱ��

	bool is_left_key_down = false;			// �Ƿ������
	bool is_right_key_down = false;			// �Ƿ����Ҽ�

	bool is_facing_right = true;			// �Ƿ������ұ�

	Timer timer_attack_cd;					// ��ͨ������ȴ��ʱ��
	bool can_attack = true;					// �Ƿ������ͨ����
	int attack_cd = 500;					// ��ͨ������ȴʱ��

	bool is_attacking = false;				// �Ƿ�������ͨ����
	bool is_attacking_ex = false;			// �Ƿ��������⹥��

	IMAGE img_sketch_white;					// ���׼�Ӱ֡
	bool is_invunerable = false;			// �Ƿ����޵�״̬
	bool is_showing_sketch_frame = false;	// �Ƿ���ʾ���׼�Ӱ֡
	Timer timer_invunerable;				// �޵�ʱ�䶨ʱ��
	Timer timer_invunerable_blink;			// �޵�ʱ����˸��ʱ��

	std::vector<Particle> particle_list;	// ���Ӷ����б�
	Timer timer_run_effect_generation;		// �ƶ���Ч�������ɶ�ʱ��
	Timer timer_die_effect_generation;		// ������Ч�������ɶ�ʱ��

	Vector2 last_hurt_direction;			// ���һ�����˷���

public:
	Player(bool is_facing_right = true) : is_facing_right(is_facing_right)
	{
		current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;

		animation_idle_left.set_loop(true);
		animation_idle_right.set_loop(true);
		animation_run_left.set_loop(true);
		animation_run_right.set_loop(true);

		animation_jump_effect.set_atlas(&atlas_jump_effect);
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback([&]() { is_jump_effect_visible = false; });

		animation_land_effect.set_atlas(&atlas_land_effect);
		animation_land_effect.set_interval(50);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_callback([&]() { is_land_effect_vivible = false; });

		timer_cursor_visibility.set_time_waiting(2000);
		timer_cursor_visibility.set_shot_once(true);
		timer_cursor_visibility.set_callback([&]() { is_cursor_visible = false; });

		timer_attack_cd.set_time_waiting(attack_cd);
		timer_attack_cd.set_shot_once(true);
		timer_attack_cd.set_callback([&]() { can_attack = true; });

		timer_invunerable.set_time_waiting(750);
		timer_invunerable.set_shot_once(true);
		timer_invunerable.set_callback([&]() { is_invunerable = false; });

		timer_invunerable_blink.set_time_waiting(75);
		timer_invunerable_blink.set_shot_once(false);
		timer_invunerable_blink.set_callback([&]() { is_showing_sketch_frame = !is_showing_sketch_frame; });

		timer_run_effect_generation.set_time_waiting(75);
		timer_run_effect_generation.set_shot_once(false);
		timer_run_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
			}
		);

		timer_die_effect_generation.set_time_waiting(35);
		timer_die_effect_generation.set_shot_once(false);
		timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			}
		);
	}
	~Player() = default;

	virtual void on_update(int dtime)
	{
		int dir = is_right_key_down - is_left_key_down;

		if (dir != 0)
		{
			is_facing_right = dir > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			float distance = dir * run_velocity * dtime;
			on_run(distance);
		}
		else if (is_attacking_ex)
		{
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;
			timer_run_effect_generation.pause();
		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
			timer_run_effect_generation.pause();
		}

		if (hp <= 0)
			current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;

		on_move_and_collide(dtime);

		if (is_attacking)
		{
			if (can_attack)
			{
				on_attack();
				can_attack = false;
				timer_attack_cd.restart();
			}
		}

		current_animation->on_update(dtime);
		animation_jump_effect.on_update(dtime);
		animation_land_effect.on_update(dtime);

		timer_cursor_visibility.on_update(dtime);
		timer_attack_cd.on_update(dtime);
		timer_invunerable.on_update(dtime);
		timer_invunerable_blink.on_update(dtime);
		timer_run_effect_generation.on_update(dtime);
		if (hp <= 0)
			timer_die_effect_generation.on_update(dtime);

		particle_list.erase(std::remove_if(
			particle_list.begin(), particle_list.end(),
			[](const Particle& particle)
			{
				return !particle.is_valid();
			}),
			particle_list.end()
		);
		for (auto& particle : particle_list)
			particle.on_update(dtime);

		if (is_showing_sketch_frame)
			sketch_frame(current_animation->get_frame(), &img_sketch_white);
	}
	virtual void on_draw(const Camera& camera)
	{
		if (is_jump_effect_visible)
			animation_jump_effect.on_draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
		if (is_land_effect_vivible)
			animation_land_effect.on_draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);

		for (const auto& particle : particle_list)
			particle.on_draw(camera);

		if (hp > 0 && is_invunerable && is_showing_sketch_frame)
			putimage_with_alpha(camera, (int)position.x, (int)position.y, &img_sketch_white);
		else
			current_animation->on_draw(camera, (int)position.x, (int)position.y);

		if (is_debug)
		{
			setlinecolor(RGB(0, 127, 255));
			rectangle(camera, (int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}

		if (is_cursor_visible)
		{
			switch (id)
			{
			case PlayerID::P1:
				putimage_with_alpha(camera, (int)(position.x + (size.x - img_1P_cursor.getwidth()) / 2)
					, (int)(position.y - img_1P_cursor.getheight()), &img_1P_cursor);
				break;
			case PlayerID::P2:
				putimage_with_alpha(camera, (int)(position.x + (size.x - img_2P_cursor.getwidth()) / 2)
					, (int)(position.y - img_2P_cursor.getheight()), &img_2P_cursor);
				break;
			}
		}
	}
	virtual void on_run(float distance)
	{
		if (is_attacking_ex)
			return;

		position.x += distance;
		timer_run_effect_generation.resume();
	}
	virtual void on_jump()
	{
		if (jump || is_attacking_ex) return;
		velocity.y = jump_velocity;
		jump = true;
		is_jump_effect_visible = true;
		animation_jump_effect.reset();

		IMAGE* effect_frame = animation_jump_effect.get_frame();
		position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_jump_effect.y = position.y + size.y - effect_frame->getheight();
	}
	virtual void on_land()
	{
		is_land_effect_vivible = true;
		animation_land_effect.reset();

		IMAGE* effect_frame = animation_land_effect.get_frame();
		position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_land_effect.y = position.y + size.y - effect_frame->getheight();
	}
	virtual void on_move_and_collide(int dtime)
	{
		float last_tick_velocity_y = velocity.y;

		velocity.y += gravity * dtime;
		position += velocity * (float)dtime;

		if (hp <= 0)
			return;

		if (velocity.y > 0)
		{
			for (const Platform& platform : platform_list)
			{
				auto shape = platform.shape;
				bool is_collide_x = (max(position.x + size.x, shape.right_x) - min(position.x, shape.left_x) <= size.x + (shape.right_x - shape.left_x));
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);
				if (is_collide_x && is_collide_y)
				{
					float delta_pos_y = velocity.y * dtime;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
					if (last_tick_foot_pos_y <= shape.y)
					{
						position.y = shape.y - size.y;
						velocity.y = 0;
						jump = false;
						if (last_tick_velocity_y)
							on_land();
						break;
					}
				}
			}
		}

		if (!is_invunerable)
		{
			for (Bullet* bullet : bullet_list)
			{
				if (!bullet->is_valid() || bullet->get_collide_target() != id)
					continue;
				if (bullet->check_collision(position, size))
				{
					bullet->on_collide();
					bullet->set_valid(false);
					hp -= bullet->get_damage();
					make_invunerable();
					last_hurt_direction = bullet->get_position() - position;
					if (hp <= 0)
					{
						velocity.x = last_hurt_direction.x < 0 ? 0.35f : -0.35f;
						velocity.y = -1.0f;
					}
				}
			}
		}
	}
	virtual void on_attack() = 0;
	virtual void on_attack_ex() = 0;
	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 'W'
				case 0x57:
					on_jump();
					break;
					// 'A'
				case 0x41:
					is_left_key_down = true;
					break;
					// 'D'
				case 0x44:
					is_right_key_down = true;
					break;
					// 'F'
				case 0x46:
					is_attacking = true;
					break;
					// 'G'
				case 0x47:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
					// '��'
				case VK_UP:
					on_jump();
					break;
					// '��'
				case VK_LEFT:
					is_left_key_down = true;
					break;
					// '��'
				case VK_RIGHT:
					is_right_key_down = true;
					break;
					// '.'
				case VK_OEM_PERIOD:
					is_attacking = true;
					break;
					// '/'
				case VK_OEM_2:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 'A'
				case 0x41:
					is_left_key_down = false;
					break;
					// 'D'
				case 0x44:
					is_right_key_down = false;
					break;
					// 'F'
				case 0x46:
					is_attacking = false;
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
					// '��'
				case VK_LEFT:
					is_left_key_down = false;
					break;
					// '��'
				case VK_RIGHT:
					is_right_key_down = false;
					break;
					// '.'
				case VK_OEM_PERIOD:
					is_attacking = false;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	void make_invunerable()
	{
		is_invunerable = true;
		timer_invunerable.restart();
	}

	void set_id(PlayerID id)
	{
		this->id = id;
	}
	void set_position(float x, float y)
	{
		this->position.x = x;
		this->position.y = y;
	}
	const PlayerID& get_id() const
	{
		return id;
	}
	const Vector2& get_position() const
	{
		return position;
	}
	const Vector2& get_size() const
	{
		return size;
	}
	int get_hp() const
	{
		return hp;
	}
	int get_mp() const
	{
		return mp;
	}
	void set_hp(int hp)
	{
		this->hp = hp;
	}
	void set_mp(int mp)
	{
		this->mp = mp;
	}
};

#endif // !_PLAYER_H_
