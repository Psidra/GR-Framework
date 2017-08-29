#include "DetectMemoryLeak.h"
#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"
#include "UIManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "UIElement.h"
#include "LevelStuff/TileEntity.h"
#include "LevelStuff/Level.h"
#include "LevelStuff\QuadTree.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "HardwareAbstraction\Keyboard.h"

#include <iostream>
#include "RenderHelper.h"
#include "WeaponManager.h"
#include "Minimap\Minimap.h"
#include "Projectile\ProjectileManager.h"
#include "Projectile\Projectile.h"
#include "WeaponInfo\WeaponInfo.h"
#include "Loader.h"
#include "TextEntityManager.h"

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
	
	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled[0]");
	currProg->AddUniform("colorTexture[0]");
	currProg->AddUniform("colorTextureEnabled[1]");
	currProg->AddUniform("colorTexture[1]");
	currProg->AddUniform("colorTextureEnabled[2]");
	currProg->AddUniform("colorTexture[2]");
	currProg->AddUniform("colorTextureEnabled[3]");
	currProg->AddUniform("colorTexture[3]");
	currProg->AddUniform("colorTextureEnabled[4]");
	currProg->AddUniform("colorTexture[4]");
	currProg->AddUniform("colorTextureEnabled[5]");
	currProg->AddUniform("colorTexture[5]");
	currProg->AddUniform("colorTextureEnabled[6]");
	currProg->AddUniform("colorTexture[6]");
	currProg->AddUniform("colorTextureEnabled[7]");
	currProg->AddUniform("colorTexture[7]");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");

	currProg->AddUniform("shadowMap");
	currProg->AddUniform("lightDepthMVP");
	
	GraphicsManager::GetInstance()->m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");
	GraphicsManager::GetInstance()->gPass_params[GraphicsManager::GPASS_UNIFORM_TYPE::U_SHADOW_COLOR_TEXTURE_ENABLED] =
		glGetUniformLocation(GraphicsManager::GetInstance()->m_gPassShaderID, "colorTextureEnabled[0]");
	GraphicsManager::GetInstance()->gPass_params[GraphicsManager::GPASS_UNIFORM_TYPE::U_SHADOW_COLOR_TEXTURE] =
		glGetUniformLocation(GraphicsManager::GetInstance()->m_gPassShaderID, "colorTexture[0]");
	GraphicsManager::GetInstance()->gPass_params[GraphicsManager::GPASS_UNIFORM_TYPE::U_SHADOW_COLOR_TEXTURE_ENABLED1] =
		glGetUniformLocation(GraphicsManager::GetInstance()->m_gPassShaderID, "colorTextureEnabled[1]");
	GraphicsManager::GetInstance()->gPass_params[GraphicsManager::GPASS_UNIFORM_TYPE::U_SHADOW_COLOR_TEXTURE1] =
		glGetUniformLocation(GraphicsManager::GetInstance()->m_gPassShaderID, "colorTexture[1]");
	GraphicsManager::GetInstance()->gPass_params[GraphicsManager::GPASS_UNIFORM_TYPE::U_SHADOW_COLOR_TEXTURE_ENABLED2] =
		glGetUniformLocation(GraphicsManager::GetInstance()->m_gPassShaderID, "colorTextureEnabled[2]");
	GraphicsManager::GetInstance()->gPass_params[GraphicsManager::GPASS_UNIFORM_TYPE::U_SHADOW_COLOR_TEXTURE2] =
		glGetUniformLocation(GraphicsManager::GetInstance()->m_gPassShaderID, "colorTexture[2]");
	GraphicsManager::GetInstance()->gPass_params[GraphicsManager::GPASS_UNIFORM_TYPE::U_SHADOW_COLOR_TEXTURE_ENABLED3] =
		glGetUniformLocation(GraphicsManager::GetInstance()->m_gPassShaderID, "colorTextureEnabled[3]");
	GraphicsManager::GetInstance()->gPass_params[GraphicsManager::GPASS_UNIFORM_TYPE::U_SHADOW_COLOR_TEXTURE3] =
		glGetUniformLocation(GraphicsManager::GetInstance()->m_gPassShaderID, "colorTexture[3]");

	GraphicsManager::GetInstance()->gPass_params[GraphicsManager::GPASS_UNIFORM_TYPE::U_LIGHT_DEPTH_MVP_GPASS] =
		glGetUniformLocation(GraphicsManager::GetInstance()->m_gPassShaderID, "lightDepthMVP");
	GraphicsManager::GetInstance()->m_lightDepthFBO.Init(1024, 1024);

	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	//lights[0] = new Light();
	//GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	//lights[0]->type = Light::LIGHT_DIRECTIONAL;
	//lights[0]->position.Set(0, 20, 0);
	//lights[0]->color.Set(1, 1, 1);
	//lights[0]->power = 1;
	//lights[0]->kC = 1.f;
	//lights[0]->kL = 0.01f;
	//lights[0]->kQ = 0.001f;
	//lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	//lights[0]->exponent = 3.f;
	//lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	//lights[0]->name = "lights[0]";

	//lights[1] = new Light();
	//GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	//lights[1]->type = Light::LIGHT_DIRECTIONAL;
	//lights[1]->position.Set(1, 1, 0);
	//lights[1]->color.Set(1, 1, 0.5f);
	//lights[1]->power = 0.4f;
	//lights[1]->name = "lights[1]";

	//currProg->UpdateInt("numLights", 2);
	currProg->UpdateInt("textEnabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player

	std::cout << _DEBUG << std::endl;
	//int a;

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	//Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball
	//GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f));
	//aCube->SetCollider(true);
	//aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
//	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	//Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	/*SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");*/

	// Customise the ground entity
//	groundEntity->SetPosition(Vector3(0, -10, 0));
//	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	//groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));

	UIManager::GetInstance()->state = UIManager::GAME_STATE::MAIN_MENU;

	// test walls
	GenericEntity* wall = Create::Entity("cube", Vector3(-20.0f, 0.0f, 0.0f), Vector3(2, 5, 2), true);
	wall->type = GenericEntity::OBJECT_TYPE::WALL;
	wall->SetAABB(wall->GetScale() * 0.5f + wall->GetPosition() , wall->GetScale() * -0.5f + wall->GetPosition());
	wall->setNormal(Vector3(1, 0, 0));

	GenericEntity* BwallA = Create::Entity("cube", Vector3(50.0f, 0.0f, 0.0f), Vector3(1, 30, 1), true);
	BwallA->type = GenericEntity::OBJECT_TYPE::WALL;
	BwallA->SetAABB(BwallA->GetScale() * 0.5f + BwallA->GetPosition(), BwallA->GetScale() * -0.5f + BwallA->GetPosition());
	BwallA->setNormal(Vector3(1, 0, 0));

	// :b:roke

	GenericEntity* BwallC = Create::Entity("cube", Vector3(65.0f, 15.0f, 0.0f), Vector3(30, 1, 1), true);
	BwallC->type = GenericEntity::OBJECT_TYPE::WALL;
	BwallC->SetAABB(BwallC->GetScale() * 0.5f + BwallC->GetPosition(), BwallC->GetScale() * -0.5f + BwallC->GetPosition());
	BwallC->setNormal(Vector3(1, 0, 0));

	GenericEntity* BwallD = Create::Entity("cube", Vector3(80.0f, 0.0f, 0.0f), Vector3(1, 30, 1), true);
	BwallD->type = GenericEntity::OBJECT_TYPE::WALL;
	BwallD->SetAABB(BwallD->GetScale() * 0.5f + BwallD->GetPosition(), BwallD->GetScale() * -0.5f + BwallD->GetPosition());
	BwallD->setNormal(Vector3(1, 0, 0));

	//GenericEntity* wall2 = Create::Entity("cube", Vector3(10.0f, 0.0f, -0.5f), Vector3(2, 10, 2), true);
	//wall2->type = GenericEntity::OBJECT_TYPE::WALL;
	//wall2->SetAABB(Vector3(10, 10, 10) + wall2->GetPosition(), Vector3(-10, -10, -10) + wall2->GetPosition());

	//// test fire
	//GenericEntity* fire = Create::Entity("fire", Vector3(-10.0f, -5.0f, 0.0f), Vector3(2, 2, 1), true);
	//fire->type = GenericEntity::OBJECT_TYPE::FIRE;
	//fire->SetAABB(fire->GetScale() * 0.5f + fire->GetPosition(), fire->GetScale() * -0.5f + fire->GetPosition());

	//// test slow
	//GenericEntity* slow = Create::Entity("slow", Vector3(0.0f, -5.0f, 0.0f), Vector3(2, 2, 1), true);
	//slow->type = GenericEntity::OBJECT_TYPE::SLOW;
	//slow->SetAABB(slow->GetScale() * 0.5f + slow->GetPosition(), slow->GetScale() * -0.5f + slow->GetPosition());

	//// test poison
	//GenericEntity* poison = Create::Entity("poison", Vector3(10.0f, -5.0f, 0.0f), Vector3(2, 2, 1), true);
	//poison->type = GenericEntity::OBJECT_TYPE::POISON;
	//poison->SetAABB(poison->GetScale() * 0.5f + poison->GetPosition(), poison->GetScale() * -0.5f + poison->GetPosition());

	//GenericEntity* testcube = Create::Entity("cube", Vector3(8, 6, 0));

	// Make UI

	UIElement* cursor = Create::UIEntity("player_cursor", Vector3(0, 0, 10), Vector3(35, 35, 1), false);
	cursor->elestate = UIElement::ELEMENT_STATE::ALL;
	cursor->type = UIElement::ELEMENT_TYPE::CURSOR;

	UIElement* startMM = Create::UIEntity("start_button", Vector3(0, 90, 9.5f), Vector3(175, 25, 1), true);
	startMM->elestate = UIElement::ELEMENT_STATE::MAIN_MENU;
	startMM->type = UIElement::ELEMENT_TYPE::START;

	UIElement* optionMM = Create::UIEntity("option_button", Vector3(0, 40, 9.5f), Vector3(175, 25, 1), true);
	optionMM->elestate = UIElement::ELEMENT_STATE::MAIN_MENU;
	optionMM->type = UIElement::ELEMENT_TYPE::OPTION;

	UIElement* exitMM = Create::UIEntity("exit_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	exitMM->elestate = UIElement::ELEMENT_STATE::MAIN_MENU;
	exitMM->type = UIElement::ELEMENT_TYPE::EXIT;

	UIElement* resumePP = Create::UIEntity("resume_button", Vector3(0, 90, 9.5f), Vector3(175, 25, 1), true);
	resumePP->elestate = UIElement::ELEMENT_STATE::PAUSE;
	resumePP->type = UIElement::ELEMENT_TYPE::RESUME;

	UIElement* optionPP = Create::UIEntity("option_button", Vector3(0, 40, 9.5f), Vector3(175, 25, 1), true);
	optionPP->elestate = UIElement::ELEMENT_STATE::PAUSE;
	optionPP->type = UIElement::ELEMENT_TYPE::OPTION;

	UIElement* exitPP = Create::UIEntity("exit_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	exitPP->elestate = UIElement::ELEMENT_STATE::PAUSE;
	exitPP->type = UIElement::ELEMENT_TYPE::EXIT;

	UIElement* cancelOp = Create::UIEntity("cancel_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	cancelOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	cancelOp->type = UIElement::ELEMENT_TYPE::CANCEL;

	UIElement* confirmOp = Create::UIEntity("confirm_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	confirmOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	confirmOp->type = UIElement::ELEMENT_TYPE::CONFIRM;

	UIElement* reloadOp = Create::UIEntity("reload_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	reloadOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	reloadOp->type = UIElement::ELEMENT_TYPE::INPUT_RELOAD;

	UIElement* moveupOp = Create::UIEntity("moveup_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	moveupOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	moveupOp->type = UIElement::ELEMENT_TYPE::INPUT_MOVE_UP;

	UIElement* movedownOp = Create::UIEntity("movedown_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	movedownOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	movedownOp->type = UIElement::ELEMENT_TYPE::INPUT_MOVE_DOWN;
	
	UIElement* moveleftOp = Create::UIEntity("moveleft_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	moveleftOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	moveleftOp->type = UIElement::ELEMENT_TYPE::INPUT_MOVE_LEFT;

	UIElement* moverightOp = Create::UIEntity("moveright_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	moverightOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	moverightOp->type = UIElement::ELEMENT_TYPE::INPUT_MOVE_RIGHT;

	UIElement* nextgunOp = Create::UIEntity("nextwep_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	nextgunOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	nextgunOp->type = UIElement::ELEMENT_TYPE::INPUT_NEXT_GUN;
	
	UIElement* blankOp = Create::UIEntity("blank_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	blankOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	blankOp->type = UIElement::ELEMENT_TYPE::INPUT_BLANK;
	
	UIElement* pauseOp = Create::UIEntity("pause_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	pauseOp->elestate = UIElement::ELEMENT_STATE::OPTIONS;
	pauseOp->type = UIElement::ELEMENT_TYPE::INPUT_PAUSE;
	
	//TELEPORTER
	//GenericEntity* teleporter = Create::Entity("greenCube", Vector3(-20.0f, 10.0f, 0.0f), Vector3(5, 5, 2), true);
	//teleporter->type = GenericEntity::OBJECT_TYPE::TELEPORTER;
	////teleporter->SetAABB(teleporter->GetScale() * 0.5f + teleporter->GetPosition(), teleporter->GetScale() * -0.5f + teleporter->GetPosition());
	////teleporter->setNormal(Vector3(1, 0, 0));
	//GenericEntity* teleporter2 = Create::Entity("greenCube", Vector3(5.0f, 10.0f, 0.0f), Vector3(5, 5, 2), true);
	//teleporter2->type = GenericEntity::OBJECT_TYPE::TELEPORTER;
	//teleporter2->SetAABB(teleporter->GetScale() * 0.5f + teleporter->GetPosition(), teleporter->GetScale() * -0.5f + teleporter->GetPosition());
	//teleporter->setNormal(Vector3(1, 0, 0));

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	fontSize = 25.0f;
	halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 4; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");

	for (int i = 0; i < 10; ++i)
	{
		optionTextObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}

	//theEditor = new Editor();

	Player::GetInstance()->Init();
	camera = new FPSCamera();
	Player::GetInstance()->AttachCamera(camera);
	GraphicsManager::GetInstance()->AttachCamera(Player::GetInstance()->getCamera());
	this->keyboard = new Keyboard();
	keyboard->Create();
	//load from file (uses hex)
	//keyboard->Load("Keybind//keyconfig.txt");
	keyboard->Load("Keybind//keyconfigtest.txt");
	//keyboard->Write("Keybind//keyconfigtest.txt");	//WIP- got it to write, but need to take in user input left
	//keyboard->Load("Keybind//keyconfigtest.txt");
	keyboard->MapKeys("Keybind//keys.txt");

	RealLoopTime = 0.0;
	elapsedTime = 0.0;
	huntTime = 0.0;
	
	HuntTarget = Create::Entity("target", Vector3(0, 0, 0), Vector3(3, 3, 1), false);
	HuntTarget->SetIsActive(false);

	UIManager::GetInstance()->SetKeyboard(keyboard);

	Controller playerControl;
	playerControl.Create(Player::GetInstance());


	/*CEnemy* NewEnemy =  Create::Enemy(Vector3(5, 5, 0), "player", false);
	NewEnemy->Init(50.0f, 1.5, 1);*/
	//NewEnemy->ChangeStrategy(new CStrategy_AI_1(), false);

	//CEnemy* NewObstacle = Create::Enemy(Vector3(-10, 10, 0), "player", false);
	//NewObstacle->Init(100.f, 0, 2, true);
	//NewObstacle->ChangeStrategy(new CStrategy_AI_Obstacle(), false);

	//Boss* FirstBoss = Create::SpawnBoss(Vector3(65.f, 5.f, 0), "player", Vector3(1.5f, 3, 3), false);
	//FirstBoss->Init(450.f, 0, 1, false); // HP default should be 1500.f but i wanted to see 2nd form skills xd
	//FirstBoss->ChangeStrategy(new CStrategy_AI_FirstBoss(), false);

	// Minimap
	minimap = Create::Minimap(false);
	minimap->SetBackground(MeshBuilder::GetInstance()->GenerateQuad("MINIMAP", Color(0, 0, 0), 1.f));
	minimap->GetBackground()->textureID[0] = LoadTGA("Image//UI/mapGroundTexture.tga");
	minimap->SetBorder(MeshBuilder::GetInstance()->GenerateQuad("MINIMAPBORDER", Color(1, 1, 1), 1.05f));
	minimap->SetAvatar(MeshBuilder::GetInstance()->GenerateQuad("MINIMAPAVATAR", Color(1, 1, 0), 1.0f));
	minimap->GetAvatar()->textureID[0] = LoadTGA("Image//UI/Avatar.tga");
	minimap->SetStencil(MeshBuilder::GetInstance()->GenerateQuad("MINIMAP_STENCIL", Color(1, 1, 1), 1.0f));
	minimap->SetObjectMesh(MeshBuilder::GetInstance()->GenerateQuad("MINIMAP_OBJECT", Color(1, 0, 0), 1.0f));
	//minimap->GetObjectMesh()->textureID[0] = LoadTGA("Image//UI/mapGroundTexture.tga");
	//light testing
	//light_depth_mesh = MeshBuilder::GetInstance()->GenerateQuad("light_depth_mesh", Color(1, 0, 1), 1);
	//light_depth_mesh->textureID[0] = GraphicsManager::GetInstance()->m_lightDepthFBO.GetTexture();
	//light_depth_mesh->textureID[0] = LoadTGA("Image//calibri.tga");

	Math::InitRNG();
	level = Level::GetInstance();
	level->init(32.f, 32.f, 12.f, 12.f, 30);
	level->setUp();
	//Player::GetInstance()->SetPos(Vector3(15, 15, 1));

	//minimap->setMiniMapRoomList(level->getRooms());

	//this should be the last to be called
	minimap->Init();
}

void SceneText::Update(double dt)
{
	double x, y;
	MouseController::GetInstance()->GetMousePosition(x, y);
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;
	float posX = (static_cast<float>(x) - halfWindowWidth);
	float posY = (halfWindowHeight - static_cast<float>(y));

	//keyboard->ConvertInt();
	float fps = (float)(1.f / dt);
	//std::cout << "fps:" << fps << "          ";
	//vector<EntityBase*> getNew = quadTree->queryRange(Player::GetInstance()->GetMinAABB().x, Player::GetInstance()->GetMaxAABB().x, Player::GetInstance()->GetMaxAABB().y, Player::GetInstance()->GetMinAABB().y);

	////vector<EntityBase*> getNew = quadTree->queryRange(Player::GetInstance()->GetMinAABB().x, Player::GetInstance()->GetMaxAABB().x, Player::GetInstance()->GetMaxAABB().y, Player::GetInstance()->GetMinAABB().y);
	//vector<EntityBase*> getNew = quadTree->getObjectsAt(Player::GetInstance()->GetPos().x, Player::GetInstance()->GetPos().y);

	//list<EntityBase*> temp;
	//std::copy(getNew.begin(), getNew.end(), std::back_inserter(temp));
	//printf("Objects at %lf %lf: %u\n", posX, posY, temp.size());
	//EntityManager::GetInstance()->setCollisionList(temp);
	//EntityManager::GetInstance()->setEntityList(temp);


	//double x, y;
	//MouseController::GetInstance()->GetMousePosition(x, y);
	//float w = Application::GetInstance().GetWindowWidth();
	//float h = Application::GetInstance().GetWindowHeight();
	//x -= (w * 0.5f);
	//y += (h * 0.5f);
	//float posX = (static_cast<float>(x) / 50.f) + Player::GetInstance()->GetPos().x; // dont even ask me why its 50.f, okay? I don't know either.
	//float posY = ((h - static_cast<float>(y)) / 50.f) + Player::GetInstance()->GetPos().y;
	
	try {
		Player::GetInstance()->SetView(Vector3(posX, posY, 0));
		throw DivideByZero();
	}
	catch(DivideByZero) {
		posX = 1;
		posY = 1;
	}



	keyboard->Read(dt);
	UIManager::GetInstance()->Update();

	switch (UIManager::GetInstance()->state) {
	case UIManager::GAME_STATE::PLAYING:
	{
		WeaponManager::GetInstance()->update(dt);
		minimap->Update(dt);

		RealLoopTime += dt;
		elapsedTime += dt;
		// Update the player position and other details based on keyboard and mouse inputs
		Player::GetInstance()->Update(dt);

		if (Player::GetInstance()->m_bHunted && elapsedTime > huntTime + 1.f)
		{
			Player::GetInstance()->m_bHunted = false;
			HuntTarget->SetIsActive(true);
			huntTime = elapsedTime + 3.f;
		}

		if (huntTime > elapsedTime)
			HuntTarget->SetPosition(Vector3(Player::GetInstance()->GetPos().x, Player::GetInstance()->GetPos().y, Player::GetInstance()->GetPos().z - 1.f));
			

		if (elapsedTime > huntTime + 0.5f && HuntTarget->IsActive())
		{
			HuntTarget->SetIsActive(false);

			int rand_element = Math::RandIntMinMax(0, 2);
			// Of course they dont disappear, this is a boss fight, dont be a wuss
			switch (rand_element) {
			case 0:
			{
				GenericEntity* fire = Create::Entity("fire", HuntTarget->GetPosition(), Vector3(2, 2, 1), true);
				fire->type = GenericEntity::OBJECT_TYPE::FIRE;
				fire->SetAABB(fire->GetScale() * 0.5f + fire->GetPosition(), fire->GetScale() * -0.5f + fire->GetPosition());
				break;
			}
			case 1:
			{
				GenericEntity* slow = Create::Entity("slow", HuntTarget->GetPosition(), Vector3(2, 2, 1), true);
				slow->type = GenericEntity::OBJECT_TYPE::SLOW;
				slow->SetAABB(slow->GetScale() * 0.5f + slow->GetPosition(), slow->GetScale() * -0.5f + slow->GetPosition());
				break;
			}
			case 2:
			{
				GenericEntity* poison = Create::Entity("poison", HuntTarget->GetPosition(), Vector3(2, 2, 1), true);
				poison->type = GenericEntity::OBJECT_TYPE::POISON;
				poison->SetAABB(poison->GetScale() * 0.5f + poison->GetPosition(), poison->GetScale() * -0.5f + poison->GetPosition());
				break;
			}
			}
		}

		// Update our entities
		EntityManager::GetInstance()->Update(dt);
		level->updateEnemy();

		// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
		if (KeyboardController::GetInstance()->IsKeyDown('1'))
			glEnable(GL_CULL_FACE);
		if (KeyboardController::GetInstance()->IsKeyDown('2'))
			glDisable(GL_CULL_FACE);
		if (KeyboardController::GetInstance()->IsKeyDown('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (KeyboardController::GetInstance()->IsKeyDown('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//if (KeyboardController::GetInstance()->IsKeyDown('5'))
		//{
		//	lights[0]->type = Light::LIGHT_POINT;
		//}
		//else if (KeyboardController::GetInstance()->IsKeyDown('6'))
		//{
		//	lights[0]->type = Light::LIGHT_DIRECTIONAL;
		//}
		//else if (KeyboardController::GetInstance()->IsKeyDown('7'))
		//{
		//	lights[0]->type = Light::LIGHT_SPOT;
		//}

		//if (KeyboardController::GetInstance()->IsKeyDown('I'))
		//	lights[0]->position.z -= (float)(10.f * dt);
		//if (KeyboardController::GetInstance()->IsKeyDown('K'))
		//	lights[0]->position.z += (float)(10.f * dt);
		//if (KeyboardController::GetInstance()->IsKeyDown('J'))
		//	lights[0]->position.x -= (float)(10.f * dt);
		//if (KeyboardController::GetInstance()->IsKeyDown('L'))
		//	lights[0]->position.x += (float)(10.f * dt);
		//if (KeyboardController::GetInstance()->IsKeyDown('O'))
		//	lights[0]->position.y -= (float)(10.f * dt);
		//if (KeyboardController::GetInstance()->IsKeyDown('P'))
		//	lights[0]->position.y += (float)(10.f * dt);

		// if the left mouse button was released
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
		{
			std::cout << "Left Mouse Button was released!" << std::endl;
		}
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
		{
			std::cout << "Right Mouse Button was released!" << std::endl;
		}
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
		{
			std::cout << "Middle Mouse Button was released!" << std::endl;
		}
		if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
		{
			std::cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << std::endl;
		}
		if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
		{
			std::cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << std::endl;
		}
		// <THERE>

		//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	//	GraphicsManager::GetInstance()->UpdateLights(dt);

		// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
		// Eg. FPSRenderEntity or inside RenderUI for LightEntity
		std::ostringstream ss;
		fontSize = 25.0f;
		halfFontSize = fontSize / 2.0f;
		/*ss << Player::GetInstance()->getInvetory()->getWeaponList()[Player::GetInstance()->getWeaponIndex()]->GetMagRound() << "/" 
			<< Player::GetInstance()->getInvetory()->getWeaponList()[Player::GetInstance()->getWeaponIndex()]->GetTotalRound();*/
		CWeaponInfo* weapon = Player::GetInstance()->getInvetory()->getPrimaryWeapon();
		ss << weapon->GetMagRound() << "/" << weapon->GetTotalRound();
		textObj[0]->SetText(ss.str());
		textObj[0]->SetPosition(Vector3(halfWindowWidth - 200.f, -halfWindowHeight + 25, 0.0f));
		textObj[0]->SetScale(Vector3(25, 25, 25));

		ss.str("");
		ss.precision(5);
		float fps = (float)(1.f / dt);
		ss << "FPS: " << fps;
		textObj[1]->SetText(ss.str());

		// Update the player position into textObj[2]
		std::ostringstream ss1;
		ss1.precision(4);
		ss1 << "Player:" << Player::GetInstance()->GetHealth();
		textObj[2]->SetText(ss1.str());

		std::ostringstream curr;
		curr << Player::GetInstance()->GetMoney();
		textObj[3]->SetText(curr.str());
		textObj[3]->SetPosition(Vector3(-halfWindowWidth + 60.f, halfWindowHeight - 150.f, 10.0f));

		// Update textpos for fullscreening
		for (int i = 1; i < 3; ++i)
		{
			textObj[i]->SetPosition(Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f));
			textObj[i]->SetIsActive(true);
		}

		if (Player::GetInstance()->m_bNewLevel)
		{
			Level::GetInstance()->newLevel();
			Player::GetInstance()->m_bNewLevel = false;
		}
		break;
	}	
	case UIManager::GAME_STATE::OPTIONS:
	{
		std::vector<std::string> temp = Loader::GetInstance()->GetData();
		fontSize = ((halfWindowWidth * 2) / 800) * 20;
		halfFontSize = (fontSize / 2);
		int inputKey = 0;
		int hextoint;
		int index;

		buttonInt.clear();
		for (size_t i = 0; i < temp.size();++i) //read file
		{
			index = atoi(temp[i].substr(0, temp[i].find('=')).c_str());
			std::stringstream ss;
			ss << std::hex << temp[i].substr(temp[i].find('x') + 1, temp[i].find(','));
			ss >> std::hex >> hextoint;
			buttonInt.push_back(hextoint);
		}
		
		for (size_t i = 0; i < 9; ++i) //display key 
		{
			std::ostringstream ss1;
			ss1 << (char)buttonInt[i];
			optionTextObj[i]->SetText(ss1.str());
			optionTextObj[i]->SetPosition(Vector3((halfWindowWidth / 800) + 50, (halfWindowHeight / 300) - (fontSize + 2) * i + halfFontSize, 10.0f));
			optionTextObj[i]->SetIsActive(true);
		}
		
		std::ostringstream ss2;
		ss2 << UIManager::GetInstance()->GetIndex();
		optionTextObj[8]->SetText(ss2.str());
		optionTextObj[8]->SetPosition(Vector3((halfWindowWidth / 800) - 200, (halfWindowHeight / 300) + 80, 10.0f));
		optionTextObj[9]->SetIsActive(true);

		keyboard->ConvertInt();
		inputKey = keyboard->GetKey();
		if (inputKey != 0)
		{
			std::ostringstream ss3;
			ss3 << "Input:";
			optionTextObj[8]->SetText(ss3.str());
			optionTextObj[8]->SetPosition(Vector3((halfWindowWidth / 800) - 100, (halfWindowHeight / 300) + 60, 10.0f));

			ss3 << (char)inputKey;
			optionTextObj[9]->SetText(ss3.str());
			optionTextObj[9]->SetPosition(Vector3((halfWindowWidth / 800) - 100 + 10, (halfWindowHeight / 300) + 60, 10.0f));
		}
		break;
	}
	case UIManager::GAME_STATE::MAIN_MENU:
	{
		for (size_t i = 0; i < 10; ++i)
		{
			optionTextObj[i]->SetIsActive(false);
		}
		break;
	}
	case UIManager::GAME_STATE::PAUSE:
	{
		for (size_t i = 0; i < 10; ++i)
		{
			optionTextObj[i]->SetIsActive(false);
		}
		break;
	}
	}
}

void SceneText::Render()
{

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	//// Setup 3D pipeline then render 3D
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//GraphicsManager::GetInstance()->AttachCamera(Player::GetInstance()->getCamera());
	//EntityManager::GetInstance()->Render();

	//// Setup 2D pipeline then render 2D
	//int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	//int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	//GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	//GraphicsManager::GetInstance()->DetachCamera();
	//EntityManager::GetInstance()->RenderUI();

	//******************************* PRE RENDER PASS *************************************
	//RenderPassGPass();
	//******************************* MAIN RENDER PASS ************************************
	RenderPassMain();
}

void SceneText::RenderPassGPass()
{
	GraphicsManager* g = GraphicsManager::GetInstance();

	g->m_renderPass = g->RENDER_PASS_PRE;
	g->m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(g->m_gPassShaderID);
	//These matrices should change when light position or direction changes
	Light* light = dynamic_cast<Light*>(g->GetLight("lights[0]"));
	if (light->type == Light::LIGHT_DIRECTIONAL)
		g->m_lightDepthProj.SetToOrtho(-10, 10, -10, 10, -10, 20);
	else
		g->m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 10);


	g->m_lightDepthView.SetToLookAt(light->position.x,
		light->position.y, light->position.z, 0, 0, 0, 0, 1, 0);

	RenderWorld();
}
//******************************* MAIN RENDER PASS ************************************
void SceneText::RenderPassMain()
{

	GraphicsManager* g = GraphicsManager::GetInstance();
	MS& ms = GraphicsManager::GetInstance()->GetModelStack();

	g->m_renderPass = g->RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetInstance().GetWindowWidth(),
		Application::GetInstance().GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(g->GetActiveShader()->GetProgramID());
	//pass light depth texture
	g->m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(g->GetActiveShader()->GetUniform("shadowMap"), 8);
	//... old stuffs

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, (float)Application::GetInstance().GetWindowWidth() / (float)Application::GetInstance().GetWindowHeight(), 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(Player::GetInstance()->getCamera());

	ms.LoadIdentity();

	//Light* light = dynamic_cast<Light*>(g->GetLight("lights[0]"));
	//ms.PushMatrix();
	//ms.Translate(light->position.x,
	//	light->position.y,
	//	light->position.z);
	//ms.Scale(0.1f, 0.1f, 0.1f);
	//RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("sphere"));
	//ms.PopMatrix();

	//ms.PushMatrix();
	//ms.Translate(0, 0, -10);
	////ms.Rotate(-90, 1, 0, 0);
	//ms.Scale(10, 10, 10);
	//RenderHelper::RenderMesh(light_depth_mesh);
	//ms.PopMatrix();

	//placed down so alpha will work properly on ldq.
	switch (UIManager::GetInstance()->state) {
	case UIManager::GAME_STATE::PLAYING:
	{
		RenderWorld();
		break;
	}
	}

	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
	TextEntityManager::GetInstance()->RenderUI();
	UIManager::GetInstance()->Render();

	switch (UIManager::GetInstance()->state) {
	case UIManager::GAME_STATE::PLAYING:
	{
		minimap->RenderUI();
		//RenderHelper::RenderTextOnScreen(text, std::to_string(fps), Color(0, 1, 0), 2, 0, 0);
		break;
	}
	}
}

void SceneText::RenderWorld()
{
	MS& ms = GraphicsManager::GetInstance()->GetModelStack();

	//-----Do we even need this ?
	//ms.PushMatrix();
	//ms.Scale(0.1f, 0.1f, 0.1f);
	//RenderHelper::RenderMeshWithLight(MeshList::GetInstance()->GetMesh("sphere"));
	//ms.PopMatrix();

	//ms.PushMatrix();	
	//ms.Translate(0, -5, 0);
	//ms.Rotate(-90, 1, 0, 0);
	//ms.Scale(10, 10, 10);
	//RenderHelper::RenderMeshWithLight(MeshList::GetInstance()->GetMesh("quad"));
	//ms.PopMatrix();


	ms.PushMatrix();
	ms.Translate(0, 0, -5);
	ms.Scale(100, 100, 1);
	RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("quad"));
	ms.PopMatrix();

	EntityManager::GetInstance()->Render(); //place render entity after render map

	WeaponManager::GetInstance()->Render();

	ms.PushMatrix();
	ms.Translate(Player::GetInstance()->GetPos().x, Player::GetInstance()->GetPos().y, Player::GetInstance()->GetPos().z);
	RenderHelper::RenderMesh(Player::GetInstance()->GetPlayerAnimated()[Player::GetInstance()->GetAnimationIndex()]->GetMesh());
	ms.PopMatrix();

	if (huntTime + 0.7f > elapsedTime)
	{
		ms.PushMatrix();
		ms.Translate(HuntTarget->GetPosition().x, HuntTarget->GetPosition().y, HuntTarget->GetPosition().z);
		ms.Scale(HuntTarget->GetScale().x, HuntTarget->GetScale().y, Player::GetInstance()->GetPos().z);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("target"));
		ms.PopMatrix();
	}

	if (Player::GetInstance()->m_bProjectileCircle)
	{
		ms.PushMatrix();
		ms.Translate(Player::GetInstance()->GetPos().x, Player::GetInstance()->GetPos().y, Player::GetInstance()->GetPos().z);
		ms.Scale(11, 11, 1);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("projcircle"));
		ms.PopMatrix();
	}
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	//delete entitymanager
	EntityManager::GetInstance()->Destroy();



	//delete tetxentiymanager
	TextEntityManager::GetInstance()->Destroy();

	//delete weaponManager
	WeaponManager::GetInstance()->Destroy();

	Player::GetInstance()->Destroy();
	KeyboardController::GetInstance()->Destroy();
	UIManager::GetInstance()->Destroy();

	delete keyboard;
	delete camera;
	// Delete the lights
	//delete lights[0];
	//delete lights[1];

}
