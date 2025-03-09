#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "camera.h"
#include "scene.h"
#include "scene_manager.h"
#include <graphics.h>

extern IMAGE img_menu_bg;

class MenuScene : public Scene
{
private:

public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter()
	{
		mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);
	}
	void on_update(int dtime)
	{

	}
	void on_draw(const Camera& camera)
	{
		putimage(0, 0, &img_menu_bg);
	}
	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYUP)
		{
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
			scene_manager.switch_to(SceneManager::SceneType::SELECTOR);
		}
	}
	void on_exit()
	{
		mciSendString(_T("pause bgm_menu"), NULL, 0, NULL);
	}
};

#endif // !_MENU_SCENE_H_
