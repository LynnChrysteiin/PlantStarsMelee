#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
private:
	int time_elapsed = 0;			// 已过时间
	int time_waiting = 0;			// 等待时间
	bool paused = false;			// 是否暂停
	bool shotted = false;			// 是否已触发
	bool shot_once = false;			// 是否只触发一次
	std::function<void()> callback; // 回调函数

public:
	Timer() = default;
	~Timer() = default;

	void restart()
	{
		time_elapsed = 0;
		shotted = false;
	}
	void set_time_waiting(int val)
	{
		time_waiting = val;
	}
	void set_shot_once(bool flag)
	{
		shot_once = flag;
	}
	void set_callback(const std::function<void()>& func)
	{
		callback = func;
	}
	void pause()
	{
		paused = true;
	}
	void resume()
	{
		paused = false;
	}
	void on_update(int dtime)
	{
		if (paused)
			return;
		time_elapsed += dtime;
		if (time_elapsed >= time_waiting)
		{
			if ((!shot_once || (shot_once && !shotted)) && callback)
				callback();
			shotted = true;
			time_elapsed = 0;
		}
	}
};

#endif // !_TIMER_H_
