#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "timer.h"
#include "vector2.h"

class Camera
{
private:
	Vector2 position;				// ���λ��
	Timer timer_shake;			// ������ʱ��
	bool is_shaking = false;	// �Ƿ����ڶ���
	float shaking_strength = 0;	// ��������

public:
	Camera()
	{
		timer_shake.set_shot_once(true);
		timer_shake.set_callback(
			[&]()
			{
				is_shaking = false;
				reset();
			}
		);
	}
	~Camera() = default;

	const Vector2& get_position() const
	{
		return position;
	}
	void reset()
	{
		position.x = 0;
		position.y = 0;
	}
	void on_update(int dtime)
	{
		timer_shake.on_update(dtime);

		if (is_shaking)
		{
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}
	void shake(float strength, int duration)
	{
		is_shaking = true;
		shaking_strength = strength;

		timer_shake.set_time_waiting(duration);
		timer_shake.restart();
	}
};

#endif // !_CAMERA_H_
