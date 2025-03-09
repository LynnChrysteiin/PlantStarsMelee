#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "camera.h"
#include "scene.h"

extern Scene* menu_scene;
extern Scene* selector_scene;
extern Scene* game_scene;

class SceneManager
{
private:
	Scene* current_scene = nullptr;

public:
	enum class SceneType
	{
		MENU,
		SELECTOR,
		GAME
	};

	SceneManager() = default;
	~SceneManager() = default;

	void set_current_scene(Scene* scene)
	{
		current_scene = scene;
		current_scene->on_enter();
	}
	void switch_to(SceneType type)
	{
		current_scene->on_exit();
		switch (type)
		{
		case SceneType::MENU:
			current_scene = menu_scene;
			break;
		case SceneType::SELECTOR:
			current_scene = selector_scene;
			break;
		case SceneType::GAME:
			current_scene = game_scene;
			break;
		default:
			break;
		}
		current_scene->on_enter();
	}
	void on_update(int dtime)
	{
		current_scene->on_update(dtime);
	}
	void on_draw(const Camera& camera)
	{
		current_scene->on_draw(camera);
	}
	void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}
};

#endif // !_SCENE_MANAGER_H_