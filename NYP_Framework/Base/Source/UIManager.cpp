#include "DetectMemoryLeak.h"
#include "UIManager.h"
#include "UIElement.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "CollisionManager.h"
#include "PlayerInfo\PlayerInfo.h"
#include "Application.h"
#include "GLFW\glfw3.h"
#include "AudioEngine.h"

UIManager::UIManager(): cur_state(UIManager::GAME_STATE::MAIN_MENU), checkingInput(false), inputKey(0)
{
	AudioEngine::GetInstance()->Init();
	AudioEngine::GetInstance()->AddSound("mainmenu", "Audio/MainMenu.mp3");
	AudioEngine::GetInstance()->AddSound("gameBGM1", "Audio/GameBGM1.mp3");
	AudioEngine::GetInstance()->AddSound("gameBGM2", "Audio/GameBGM2.mp3");
	AudioEngine::GetInstance()->setVolume(50);

	playingSound = false;
}

UIManager::~UIManager()
{
	for (std::list<UIElement*>::iterator it = UIList.begin();
		it != UIList.end();++it)
	{
		UIElement* temp = *it;
		delete temp;
		temp = NULL;
	}
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
	if (this->state == MAIN_MENU)
	{
		if (!AudioEngine::GetInstance()->CheckSoundOn("mainmenu"))
			AudioEngine::GetInstance()->StopAllSounds();

		AudioEngine::GetInstance()->PlayASound("mainmenu", true);
	}
	else if (this->state == PLAYING || this->state == PAUSE)
	{
		if (!AudioEngine::GetInstance()->CheckSoundOn("lastbattle"))
		{
			if (AudioEngine::GetInstance()->CheckSoundOn("mainmenu"))
				AudioEngine::GetInstance()->StopAllSounds();

			std::string currentSong, firstSong, secondSong;
			firstSong = "gameBGM1";
			secondSong = "gameBGM2";

			if (AudioEngine::GetInstance()->CheckSoundOn(firstSong))
				currentSong = firstSong;
			else if (AudioEngine::GetInstance()->CheckSoundOn(secondSong))
				currentSong = secondSong;

			if (!AudioEngine::GetInstance()->CheckSoundOn(currentSong))
			{
				playingSound = !playingSound;
				AudioEngine::GetInstance()->PlayASound((playingSound ? firstSong : secondSong));
			}
		}
	}

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
					cur_state = UIManager::GAME_STATE::PLAYING; //set cur state to playing
					this->Playing();
					break;
				case UIElement::ELEMENT_TYPE::RESUME:
					this->Playing();
					break;
				case UIElement::ELEMENT_TYPE::OPTION:
					UIManager::state = UIManager::GAME_STATE::OPTIONS;
					break;
				case UIElement::ELEMENT_TYPE::tOVERVIEW:
					UIManager::state = UIManager::GAME_STATE::sOVERVIEW;
					break;
				case UIElement::ELEMENT_TYPE::CANCEL:
					//set to not save changed keys

					if (cur_state == UIManager::GAME_STATE::PLAYING)	//checks curstate is in playing, option cancel returns to pause menu
						this->state = PAUSE;

					else if (cur_state == UIManager::GAME_STATE::MAIN_MENU) //when curstate in menu, option cancel returns to main menu
						UIManager::state = UIManager::GAME_STATE::MAIN_MENU;
					break;
				case UIElement::ELEMENT_TYPE::CONFIRM:
					//set to save changed keys

					std::cout << "KEY SET" << std::endl;
					checkingInput = false;
					if (!checkingInput)
					{
						keyboard->Write("Keybind//keyconfigtest.txt", index, inputKey);
						keyboard->Load("Keybind//keyconfigtest.txt");
					}


					//if (cur_state == UIManager::GAME_STATE::PLAYING)	//checks curstate is in playing, option cancel returns to pause menu
					//	this->state = PAUSE;

					//else if (cur_state == UIManager::GAME_STATE::MAIN_MENU) //when curstate in menu, option cancel returns to main menu
					//	UIManager::state = UIManager::GAME_STATE::MAIN_MENU;
					break;

				case UIElement::ELEMENT_TYPE::INPUT_RELOAD:
					std::cout << "pressed reload, please input a key" << std::endl;

					checkingInput = true;
					index = 0;	//key index for reload
					break;

				case UIElement::ELEMENT_TYPE::INPUT_MOVE_UP://index 0 
					std::cout << "pressed move up, please input a key" << std::endl;

					checkingInput = true;
					index = 1;	//key index for moveup
					break;
				case UIElement::ELEMENT_TYPE::INPUT_MOVE_DOWN://index 1
					std::cout << "pressed move down, please input a key" << std::endl;

					checkingInput = true;
					index = 2;	//key index for movedown
					break;
				case UIElement::ELEMENT_TYPE::INPUT_MOVE_LEFT:
					std::cout << "pressed move left, please input a key" << std::endl;

					checkingInput = true;
					index = 3;	//key index for moveleft
					break;
				case UIElement::ELEMENT_TYPE::INPUT_MOVE_RIGHT:
					std::cout << "pressed move right, please input a key" << std::endl;

					checkingInput = true;
					index = 4;	//key index for moveright
					break;

				case UIElement::ELEMENT_TYPE::INPUT_NEXT_GUN:
					std::cout << "pressed next gun, please input a key" << std::endl;

					checkingInput = true;
					index = 5;	//key index for next gun
					break;

				case UIElement::ELEMENT_TYPE::INPUT_BLANK:
					std::cout << "pressed blank, please input a key" << std::endl;

					checkingInput = true;
					index = 6;	//key index for next gun
					break;

				case UIElement::ELEMENT_TYPE::INPUT_PAUSE:
					std::cout << "pressed pause, please input a key" << std::endl;

					checkingInput = true;
					index = 7;	//key index for pause
					break;

				case UIElement::ELEMENT_TYPE::EXIT:
					exit(0);
					break;
				}
			}
		}
	}
	if (checkingInput)
	{
		keyboard->ConvertInt();
		
		if (keyboard->GetKey() != 0)
		{
			inputKey = keyboard->GetKey();
			std::cout << keyboard->GetKey() << std::endl;
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

std::string UIManager::GetIndex()
{
	switch (index) //hardcoded 
	{
	case 0:
		return "Reload Selected";
		break;
	case 1:
		return "Move Up Selected";
		break;
	case 2:
		return "Move Down Selected";
		break;
	case 3:
		return "Move Left Selected";
		break;
	case 4:
		return "Move Right Selected";
		break;
	case 5:
		return "Next Gun Selected";
		break;
	case 6:
		return "Blank Selected";
		break;
	case 7:
		return "Pause Selected";
		break;
	default:
		return "Nothing Selected";
		break;
	}

	return "";
}
