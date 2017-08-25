#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <list>
#include <iostream>
#include "SingletonTemplate.h"
#include "HardwareAbstraction\Keyboard.h"

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
	unsigned int cur_state;

	void Pause();
	void Playing();

	void Update();
	void Render();

	void AddEntity(UIElement* result);
	void SetKeyboard(Keyboard* _keyboard){	this->keyboard = _keyboard; }

private:
	std::list<UIElement*> UIList;
	Keyboard* keyboard;
	bool checkingInput;
	int temp;
};

#endif // UIMANAGER_H