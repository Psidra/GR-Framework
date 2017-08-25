#include "Keyboard.h"
#include <iostream>
#include <sstream>
using namespace std;

#include "KeyboardController.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "Loader.h"

const bool _CONTROLLER_KEYBOARD_DEBUG = false;

Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

// Create this controller
bool Keyboard::Create(Player* thePlayerInfo)
{
	Controller::Create(thePlayerInfo);
	//KeyList[CONTROLLER_MOVEUP] = 'W'; 
	//KeyList[CONTROLLER_MOVEDOWN] = 'S'; 
	//KeyList[CONTROLLER_MOVELEFT] = 'A'; 
	//KeyList[CONTROLLER_MOVERIGHT] = 'D'; 
	//KeyList[CONTROLLER_SHOOT] = VK_SPACE; 
	//KeyList[CONTROLLER_RELOAD] = 'R'; 
	//KeyList[CONTROLLER_CHANGE_WEAPON] = 'Q'; 
	//KeyList[CONTROLLER_SPAWN_ENEMY] = 'M'; 
	return false;
}

bool Keyboard::Load(std::string _filePath)
{	//using hexadecimal
	Loader::GetInstance()->LoadData(_filePath);
	std::vector<std::string> temp = Loader::GetInstance()->GetData();

	for (size_t i = 0; i < Loader::GetInstance()->GetData().size();++i)
	{
		int index = atoi(temp[i].substr(0, temp[i].find('=')).c_str());
		std::stringstream ss;
		ss << std::hex << temp[i].substr(temp[i].find('=') + 1, temp[i].find(','));
		ss >> KeyList[index];
		int methodIndex = atoi(temp[i].substr(temp[i].find(',') + 1, std::string::npos).c_str());
		pressMethod[index] = methodIndex;
	}
    return false;
}

// Read from the controller
int Keyboard::Read(const float deltaTime)
{
	Controller::Read(deltaTime);
	if (_CONTROLLER_KEYBOARD_DEBUG)
	return 0;
	map<string, string> lel;

	for (int i = 0;i < NUM_CONRTOLLER;++i)
	{
		switch (pressMethod[i])
		{
		case 1:
			if (KeyboardController::GetInstance()->IsKeyDown(KeyList[i]))
			{
				(this->*(controllerfunc[i]))(deltaTime);
			}
			break;
		case 2:
			if (KeyboardController::GetInstance()->IsKeyUp(KeyList[i]))
			{
				(this->*(controllerfunc[i]))(deltaTime);
			}
			break;
		case 3:
			if (KeyboardController::GetInstance()->IsKeyPressed(KeyList[i]))
			{
				(this->*(controllerfunc[i]))(deltaTime);
			}
			break;
		case 4:
			if (KeyboardController::GetInstance()->IsKeyReleased(KeyList[i]))
			{
				(this->*(controllerfunc[i]))(deltaTime);
			}
			break;
		default:
			break;
		}
	}
}

void Keyboard::Write(std::string _filePath)
{
	std::vector<std::string> temp = Loader::GetInstance()->GetData();
	std::vector<std::string> newlist;
	
	string storedIndex;
	string newHex;
	string storedMethodIndex;
	
	//read file
	for (size_t i = 0; i < Loader::GetInstance()->GetData().size();++i)
	{
		int index = atoi(temp[i].substr(0, temp[i].find('=')).c_str());
		std::stringstream ss;

		if (index != 1)
			newlist.push_back(temp[i]);
		else
		{//converting int to hex
			ss << std::hex << 90; //<<------------ int to hex//90 is z
			std::string result(ss.str());

			storedIndex = temp[i].substr(0, temp[i].find('='));
			newHex = "0x"+ result;			//<<----------successfully change with hex
			storedMethodIndex = temp[i].substr(temp[i].find(',') + 1, temp[i].size() - 6);

			newlist.push_back(storedIndex + "=" + newHex + "," + storedMethodIndex);
		}
		
	}
	ofstream myfile(_filePath, ofstream::out);
	for (int i = 0; i < newlist.size(); ++i)
	{
		if (i == newlist.size() - 1)
			myfile << newlist[i];
		else
			myfile << newlist[i] + "\n";

		cout << "this is the new list of keys" << newlist[i] << endl;
	}
	myfile.close();
}


