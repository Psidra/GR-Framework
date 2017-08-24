#include "UIManager.h"
#include "UIElement.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "CollisionManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "Application.h"
#include "GLFW\glfw3.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Pause()
{
	//glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	switch (this->state) {
	case PLAYING:
		this->state = PAUSE;
		break;
	case OPTIONS:
		this->state = PAUSE;
		break;
	case PAUSE:
		this->Playing();
		break;

	default:
		break;
	}
}

void UIManager::Playing()
{
	//glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	switch (this->state) {
	case MAIN_MENU:
		this->state = PLAYING;
		break;
	case PAUSE:
		this->state = PLAYING;
		break;

	default:
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
		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		int w = Application::GetInstance().GetWindowWidth();
		int h = Application::GetInstance().GetWindowHeight();
		x = x + Player::GetInstance()->GetPos().x - (w * 0.5f);
		y = y - Player::GetInstance()->GetPos().y + (h * 0.5f);
		float posX = static_cast<float>(x);
		float posY = (h - static_cast<float>(y));

		std::list<UIElement*>::iterator it, it2, end;
		end = UIList.end();
		for (it = UIList.begin(); it != end; ++it)
		{
			if ((*it)->type == UIElement::ELEMENT_TYPE::CURSOR) // (*it) is always cursor
				continue;
			if ((*it)->elestate != ALL && this->state != (*it)->elestate)
				continue;
			
			if (CollisionManager::GetInstance()->UI_CheckAABBCollision(Vector3(posX, posY, 0), (*it)))
			{
				switch ((*it)->type) {
				case UIElement::ELEMENT_TYPE::START:
					this->Playing();
					break;

				case UIElement::ELEMENT_TYPE::RESUME:
					this->Playing();
					break;
				case UIElement::ELEMENT_TYPE::OPTION:
					std::cout << "Opened options menu! not really. Kind of." << std::endl;
					// TODO
					break;
				case UIElement::ELEMENT_TYPE::EXIT:
					exit(0);
					break;
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
		if ((*it)->elestate != ALL && this->state != (*it)->elestate)
			continue;

		(*it)->Render();
	}
}

void UIManager::AddEntity(UIElement * result)
{
	this->UIList.push_back(result);
}
