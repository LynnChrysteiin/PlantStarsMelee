#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "camera.h"
#include "utils.h"

extern bool is_debug;

class Platform
{
private:

public:
	struct CollisionShape
	{
		float left_x, right_x;
		float y;
	};
	CollisionShape shape = { 0, 0, 0 };
	IMAGE *img = nullptr;
	POINT render_pos = { 0, 0 };

public:
	Platform() = default;
	~Platform() = default;
	
	void on_draw(const Camera& camera) const
	{
		putimage_with_alpha(camera, render_pos.x, render_pos.y, img);

		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left_x, (int)shape.y, (int)shape.right_x, (int)shape.y);
		}
	}
};

#endif // !_PLATFORM_H_