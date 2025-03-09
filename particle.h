#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "atlas.h"
#include "camera.h"
#include "vector2.h"
#include "utils.h"

class Particle
{
private:
	int timer = 0;			// �������Ŷ�ʱ��
	int lifeSpan = 0;		// ��֡��������ʱ��
	int idx_frame = 0;		// ��ǰ֡����
	Vector2 position;		// ��������λ��
	bool valid = true;		// �����Ƿ���Ч
	Atlas* atlas = nullptr;	// ���Ӷ���ͼ��

public:
	Particle() = default;
	Particle(const Vector2& position, Atlas* atlas, int lifeSpan)
		: position(position), atlas(atlas), lifeSpan(lifeSpan) {}
	~Particle() = default;

	void on_update(int dtime)
	{
		timer += dtime;
		if (timer >= lifeSpan)
		{
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->get_size())
			{
				valid = false;
				idx_frame = atlas->get_size() - 1;
			}
		}
	}
	void on_draw(const Camera& camera) const
	{
		putimage_with_alpha(camera, (int)position.x, (int)position.y, atlas->get_image(idx_frame));
	}
	void set_atlas(Atlas* atlas)
	{
		this->atlas = atlas;
	}
	void set_position(const Vector2& position)
	{
		this->position = position;
	}
	void set_lifeSpan(int life_span)
	{
		this->lifeSpan = life_span;
	}
	bool is_valid() const
	{
		return valid;
	}
};

#endif // !_PARTICLE_H_
