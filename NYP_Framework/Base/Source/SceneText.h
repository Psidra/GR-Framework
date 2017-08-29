#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "Enemy.h"
#include "AI FSM\Ai_1.h"
#include "AI FSM\Ai_obstacle.h"
#include "AI FSM\Ai_firstboss.h"
#include "Boss.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class Keyboard;
class CMinimap;
class Level;
class QuadTree;

class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	GenericEntity** playerAnimated;
private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	Player* playerInfo;
	CEnemy* minion;
	GroundEntity* groundEntity;
	FPSCamera *camera;
	TextEntity* textObj[4];
	TextEntity* optionTextObj[10];
	std::vector<unsigned int> buttonInt;
	//Light* lights[2];

	GenericEntity* HuntTarget;
	Keyboard* keyboard;
	static SceneText* sInstance; // The pointer to the object that gets registered

	double elapsedTime, RealLoopTime, huntTime;

	float fontSize, halfFontSize;

	void RenderPassGPass();//PRE RENDER PASS
	void RenderPassMain();//MAIN RENDER PASS
	void RenderWorld();
	//Mesh* light_depth_mesh;
	CMinimap* minimap;

	QuadTree* quadTree;
	Level* level;
};

#endif