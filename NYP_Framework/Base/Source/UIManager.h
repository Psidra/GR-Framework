#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <list>
#include <iostream>
#include "SingletonTemplate.h"

class UIElement;

class UIManager : public Singleton<UIManager>
{
	friend Singleton<UIManager>;
public:
	UIManager();
	~UIManager();

	enum GAME_STATE {
		NONE = 0,
		MAIN_MENU,
		PLAYING,
		PAUSE,
		OPTIONS,
	}state;

	void Pause();
	void Playing();

	void Update();
	void Render();

private:
	std::list<UIElement*> UIList;
};

#endif // UIMANAGER_H