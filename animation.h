#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "atlas.h"
#include "camera.h"
#include "utils.h"

#include <functional>

class Animation
{
private:
	int timer = 0;					// 计时器
	int interval = 0;				// 帧间隔
	int index_frame = 0;			// 帧索引
	bool isLoop = false;			// 是否循环播放
	Atlas* atlas = nullptr;
	std::function<void()> callback;	// 动画结束回调函数

public:
	Animation() = default;
	~Animation() = default;

	void reset()
	{
		timer = 0;
		index_frame = 0;
	}
	void set_atlas(Atlas* atlas)
	{
		reset();
		this->atlas = atlas;
	}
	void set_loop(bool flag)
	{
		isLoop = flag;
	}
	void set_interval(int interval)
	{
		this->interval = interval;
	}
	int get_index_frame() const
	{
		return index_frame;
	}
	IMAGE* get_frame() const
	{
		return atlas->get_image(index_frame);
	}
	bool check_finished() const
	{
		if (isLoop) return false;
		return index_frame == atlas->get_size() - 1;
	}
	void on_update(int dtime)
	{
		timer += dtime;
		if (timer >= interval)
		{
			timer = 0;
			index_frame++;
			if (index_frame >= atlas->get_size())
			{
				index_frame = isLoop ? 0 : atlas->get_size() - 1;
				if (!isLoop && callback)
				{
					callback();
				}
			}
		}
	}
	void on_draw(const Camera& camera, int x, int y) const
	{
		putimage_with_alpha(camera, x, y, atlas->get_image(index_frame));
	}
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}
};

#endif // !_ANIMATION_H_
