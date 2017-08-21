#include "UIManager.h"
#include "UIElement.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "CollisionManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Pause()
{
	switch (this->state) {
	case PLAYING:
		this->state = PAUSE;
		break;
	case OPTIONS:
		this->state = PAUSE;

	default:
		std::cout << "Already paused!" << std::endl;
		break;
	}
}

void UIManager::Playing()
{
	switch (this->state) {
	case MAIN_MENU:
		this->state = PLAYING;
		break;
	case PAUSE:
		this->state = PLAYING;
		break;

	default:
		std::cout << "Already playing!" << std::endl;
		break;
	}
}

void UIManager::Update()
{
	std::list<UIElement*>::iterator it, end;
	end = UIList.end();
	for (it = UIList.begin(); it != end; ++it)
	{
		(*it)->Update();
	}

	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		std::list<UIElement*>::iterator it, it2, end;
		end = UIList.end();
		for (it = UIList.begin(); it != end; ++it)
		{
			for (it2 = std::next(it, 1); it2 != end; ++it2)
			{
				if ((*it)->type != UIElement::ELEMENT_TYPE::CURSOR) // (*it) is always cursor
					continue;

				if (CollisionManager::GetInstance()->UI_CheckAABBCollision((*it), (*it2)))
				{

				}
			}
		}
	}

	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{

	}
}

void UIManager::Render()
{
	std::list<UIElement*>::iterator it, end;
	end = UIList.end();
	for (it = UIList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

void UIManager::AddEntity(UIElement * result)
{
	this->UIList.push_back(result);
}
