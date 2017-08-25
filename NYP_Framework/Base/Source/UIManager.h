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
		MAIN_MENU = 0,
		PLAYING,
		PAUSE,
		OPTIONS,
		ALL,
	}state;

	void Pause();
	void Playing();

	void Update();
	void Render();

	void AddEntity(UIElement* result);
private:
	std::list<UIElement*> UIList;
};

#endif // UIMANAGER_H