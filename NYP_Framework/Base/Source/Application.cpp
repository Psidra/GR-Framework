#include "DetectMemoryLeak.h"
#include "Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "MeshList.h"
#include "EntityManager.h"
#include "TextEntityManager.h"
#include "UIManager.h"
#include "AudioEngine.h"
#include "Loader.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "LoadTGA.h"
#include "Utility.h"

#include "SceneText.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
 // Should make these not hard-coded :P
int m_window_width = 800;
int m_window_height = 600;
class Mesh;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	m_window_width = w;
	m_window_height = h;
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "NYP Framework", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	// Init systems
	GraphicsManager::GetInstance()->Init();
	InitAllMeshes();

}

void Application::Run()
{
	SceneManager::GetInstance()->SetActiveScene("Start");
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	static double dt = m_timer.getElapsedTime();
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		glfwPollEvents();
		UpdateInput();
		
		if (dt < 0.016666667)
			dt = 0.016666667;

		SceneManager::GetInstance()->Update(m_timer.getElapsedTime());
		SceneManager::GetInstance()->Render();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		PostInputUpdate();

        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
		
	}
	SceneManager::GetInstance()->Exit();
}

void Application::Exit()
{
	MeshList::Destroy();
	GraphicsManager::Destroy();
	AudioEngine::Destroy();
	Loader::Destroy();
	MeshBuilder::Destroy();
	SceneManager::Destroy();
	MouseController::Destroy();
	
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

void Application::UpdateInput()
{
	// Update Mouse Position
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);

	// Update Keyboard Input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::PostInputUpdate()
{
	// If mouse is centered, need to update the center position for next frame
	/*if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
		double mouse_currX, mouse_currY;
		mouse_currX = m_window_width >> 1;
		mouse_currY = m_window_height >> 1;
		MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);
		glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
	}*/

	// Call input systems to update at end of frame
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
}

void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

int Application::GetWindowHeight()
{
	return m_window_height;
}

int Application::GetWindowWidth()
{
	return m_window_width;
}

void Application::InitAllMeshes()
{
	if (MeshList::GetInstance()->IsMeshEmpty())
	{
		MeshBuilder::GetInstance()->GenerateAxes("reference");
		MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
		MeshBuilder::GetInstance()->GenerateQuad("quad", Color(0.4f, 0.2f, 0.2f), 1.f);
		//MeshList::GetInstance()->GetMesh("quad")->textureID[0] = LoadTGA("Image//calibri.tga");
		MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
		MeshList::GetInstance()->GetMesh("text")->textureID[0] = LoadTGA("Image//calibri.tga");
		MeshList::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
		MeshBuilder::GetInstance()->GenerateText("text2", 16, 16);
		MeshList::GetInstance()->GetMesh("text2")->textureID[0] = LoadTGA("Image//calibri.tga");
		MeshList::GetInstance()->GetMesh("text2")->material.kAmbient.Set(1, 0, 0);
		MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
		MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
		MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
		MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 1.f);
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
		MeshList::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
		MeshList::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
		MeshBuilder::GetInstance()->GenerateCube("greenCube", Color(0.0f, 1.0f, 0.0f), 1.0f);

		// Player sprite
		MeshBuilder::GetInstance()->GenerateQuad("player", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("player")->textureID[0] = LoadTGA("Image/Player/placeholder.tga");
		
		//-------PLAYER SPRITES
		//stand
		MeshBuilder::GetInstance()->GenerateQuad("Player_fstand1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_fstand1")->textureID[0] = LoadTGA("Image/Player/Player_fstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_fstand2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_fstand2")->textureID[0] = LoadTGA("Image/Player/Player_fstand2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_bstand1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_bstand1")->textureID[0] = LoadTGA("Image/Player/Player_bstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_bstand2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_bstand2")->textureID[0] = LoadTGA("Image/Player/Player_bstand2.tga");
		//walk
		MeshBuilder::GetInstance()->GenerateQuad("Player_fwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_fwalk1")->textureID[0] = LoadTGA("Image/Player/Player_fwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_fwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_fwalk2")->textureID[0] = LoadTGA("Image/Player/Player_fwalk2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_bwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_bwalk1")->textureID[0] = LoadTGA("Image/Player/Player_bwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_bwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_bwalk2")->textureID[0] = LoadTGA("Image/Player/Player_bwalk2.tga");
		//hurt
		MeshBuilder::GetInstance()->GenerateQuad("Player_fHurt", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_fHurt")->textureID[0] = LoadTGA("Image/Player/Player_fHurt.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_bHurt", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_bHurt")->textureID[0] = LoadTGA("Image/Player/Player_bHurt.tga");
		//dodge
		MeshBuilder::GetInstance()->GenerateQuad("Player_fdodge1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_fdodge1")->textureID[0] = LoadTGA("Image/Player/Player_fdodge1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_fdodge2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_fdodge2")->textureID[0] = LoadTGA("Image/Player/Player_fdodge2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_bdodge1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_bdodge1")->textureID[0] = LoadTGA("Image/Player/Player_bdodge1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("Player_bdodge2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("Player_bdodge2")->textureID[0] = LoadTGA("Image/Player/Player_bdodge2.tga");

		//-------ENEMY SPRITES
		//-------enemy1
		//stand
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_fstand1", Color(1, 1, 1), 1.f); // THIS MUST BE THE WORK OF AN ENEMY [STAND]!!!
		MeshList::GetInstance()->GetMesh("enemy1_fstand1")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_fstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_fstand2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_fstand2")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_fstand2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_bstand1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_bstand1")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_bstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_bstand2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_bstand2")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_bstand2.tga");
		//walk
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_fwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_fwalk1")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_fwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_fwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_fwalk2")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_fwalk2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_bwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_bwalk1")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_bwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_bwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_bwalk2")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_bwalk2.tga");
		//hurt
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_fhurt", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_fhurt")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_fhurt.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_bhurt", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_bhurt")->textureID[0] = LoadTGA("Image/Enemies/Enemy1/enemy1_bhurt.tga");

		//-------enemy2
		//stand
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_fstand1", Color(1, 1, 1), 1.f); // THIS MUST BE THE WORK OF AN ENEMY [STAND]!!!
		MeshList::GetInstance()->GetMesh("enemy2_fstand1")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_fstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_fstand2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy2_fstand2")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_fstand2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_bstand1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy2_bstand1")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_bstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_bstand2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy2_bstand2")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_bstand2.tga");
		//walk
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_fwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy2_fwalk1")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_fwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_fwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy2_fwalk2")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_fwalk2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_bwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy2_bwalk1")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_bwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_bwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy2_bwalk2")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_bwalk2.tga");
		//hurt
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_fhurt", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy2_fhurt")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_fhurt.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy2_bhurt", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy2_bhurt")->textureID[0] = LoadTGA("Image/Enemies/Enemy2/enemy2_bhurt.tga");

		//-------enemy3
		//stand
		MeshBuilder::GetInstance()->GenerateQuad("enemy3_fstand1", Color(1, 1, 1), 1.f); 
		MeshList::GetInstance()->GetMesh("enemy3_fstand1")->textureID[0] = LoadTGA("Image/Enemies/Enemy3/enemy3_fstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy3_bstand1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy3_bstand1")->textureID[0] = LoadTGA("Image/Enemies/Enemy3/enemy3_bstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy3_stand2", Color(1, 1, 1), 1.f);//shares between front and back
		MeshList::GetInstance()->GetMesh("enemy3_stand2")->textureID[0] = LoadTGA("Image/Enemies/Enemy3/enemy3_stand2.tga");
		//walk
		MeshBuilder::GetInstance()->GenerateQuad("enemy3_fwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy3_fwalk1")->textureID[0] = LoadTGA("Image/Enemies/Enemy3/enemy3_fwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy3_fwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy3_fwalk2")->textureID[0] = LoadTGA("Image/Enemies/Enemy3/enemy3_fwalk2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy3_bwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy3_bwalk1")->textureID[0] = LoadTGA("Image/Enemies/Enemy3/enemy3_bwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy3_bwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy3_bwalk2")->textureID[0] = LoadTGA("Image/Enemies/Enemy3/enemy3_bwalk2.tga");
		//hurt
		MeshBuilder::GetInstance()->GenerateQuad("enemy3_fhurt", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy3_fhurt")->textureID[0] = LoadTGA("Image/Enemies/Enemy3/enemy3_fhurt.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy3_bhurt", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy3_bhurt")->textureID[0] = LoadTGA("Image/Enemies/Enemy3/enemy3_bhurt.tga");
			
		//-------FINAL BOSS
		//normal
		MeshBuilder::GetInstance()->GenerateQuad("ccore_normal1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ccore_normal1")->textureID[0] = LoadTGA("Image/Enemies/Boss/orig_n1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("ccore_normal2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ccore_normal2")->textureID[0] = LoadTGA("Image/Enemies/Boss/orig_n2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("ccore_normal3", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ccore_normal3")->textureID[0] = LoadTGA("Image/Enemies/Boss/orig_n3.tga");
		//hurt
		MeshBuilder::GetInstance()->GenerateQuad("ccore_hurt1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ccore_hurt1")->textureID[0] = LoadTGA("Image/Enemies/Boss/hurt_n1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("ccore_hurt2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ccore_hurt2")->textureID[0] = LoadTGA("Image/Enemies/Boss/hurt_n2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("ccore_hurt3", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ccore_hurt3")->textureID[0] = LoadTGA("Image/Enemies/Boss/hurt_n3.tga");
		//succ
		MeshBuilder::GetInstance()->GenerateQuad("ccore_succ1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ccore_succ1")->textureID[0] = LoadTGA("Image/Enemies/Boss/succ_n1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("ccore_succ2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ccore_succ2")->textureID[0] = LoadTGA("Image/Enemies/Boss/succ_n2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("ccore_succ3", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ccore_succ3")->textureID[0] = LoadTGA("Image/Enemies/Boss/succ_n3.tga");
		//projcircle
		MeshBuilder::GetInstance()->GenerateQuad("projcircle", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("projcircle")->textureID[0] = LoadTGA("Image/Enemies/Boss/projcircle.tga");
		//target
		MeshBuilder::GetInstance()->GenerateQuad("target", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("target")->textureID[0] = LoadTGA("Image/Enemies/Boss/target.tga");

		//fire
		MeshBuilder::GetInstance()->GenerateCube("fire", Color(1.0f, 0.7f, 0.0f), 1.0f);
		//slow
		MeshBuilder::GetInstance()->GenerateCube("slow", Color(0.2f, 0.2f, 1.0f), 1.0f);
		//poison
		MeshBuilder::GetInstance()->GenerateCube("poison", Color(0.2f, 1.0f, 0.2f), 1.0f);

		MeshBuilder::GetInstance()->GenerateQuad("Wall", Color(0, 0, 0), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("Floor", Color(1, 0, 0), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("Coord", Color(0, 0, 1), 1.f);

		//--------UI
		MeshBuilder::GetInstance()->GenerateQuad("full_hp", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("full_hp")->textureID[0] = LoadTGA("Image/UI/PlayerFullHealth.tga");
		MeshBuilder::GetInstance()->GenerateQuad("half_hp", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("half_hp")->textureID[0] = LoadTGA("Image/UI/PlayerHalfHealth.tga");
		MeshBuilder::GetInstance()->GenerateQuad("no_hp", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("no_hp")->textureID[0] = LoadTGA("Image/UI/PlayerEmptyHealth.tga");
		MeshBuilder::GetInstance()->GenerateQuad("blank", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("blank")->textureID[0] = LoadTGA("Image/UI/Blank.tga");
		MeshBuilder::GetInstance()->GenerateQuad("currency", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("currency")->textureID[0] = LoadTGA("Image/UI/Currency.tga");
		MeshBuilder::GetInstance()->GenerateQuad("player_cursor", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("player_cursor")->textureID[0] = LoadTGA("Image/UI/PlayerCursorV2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("boss_hp_border", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("boss_hp_border")->textureID[0] = LoadTGA("Image/UI/BossHealthBorder.tga");

		MeshBuilder::GetInstance()->GenerateQuad("bossHP", Color(1, 0, 0), 1.f);

		MeshBuilder::GetInstance()->GenerateQuad("main_menu", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("main_menu")->textureID[0] = LoadTGA("Image/UI/MainMenu.tga");
		MeshBuilder::GetInstance()->GenerateQuad("pause_menu", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("pause_menu")->textureID[0] = LoadTGA("Image/UI/PauseMenu.tga");
		MeshBuilder::GetInstance()->GenerateQuad("start_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("start_button")->textureID[0] = LoadTGA("Image/UI/StartButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("resume_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("resume_button")->textureID[0] = LoadTGA("Image/UI/ResumeButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("option_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("option_button")->textureID[0] = LoadTGA("Image/UI/OptionButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("exit_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("exit_button")->textureID[0] = LoadTGA("Image/UI/ExitButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("overview_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("overview_button")->textureID[0] = LoadTGA("Image/UI/OverviewButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("overview", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("overview")->textureID[0] = LoadTGA("Image/UI/Overview.tga");
		MeshBuilder::GetInstance()->GenerateQuad("victory", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("victory")->textureID[0] = LoadTGA("Image/UI/victory.tga");
		MeshBuilder::GetInstance()->GenerateQuad("defeat", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("defeat")->textureID[0] = LoadTGA("Image/UI/defeat.tga");

		//options related
		MeshBuilder::GetInstance()->GenerateQuad("option_menu", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("option_menu")->textureID[0] = LoadTGA("Image/UI/OptionMenu.tga");
		MeshBuilder::GetInstance()->GenerateQuad("confirm_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("confirm_button")->textureID[0] = LoadTGA("Image/UI/ConfirmButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("cancel_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("cancel_button")->textureID[0] = LoadTGA("Image/UI/CancelButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("moveup_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("moveup_button")->textureID[0] = LoadTGA("Image/UI/MoveUp_Button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("movedown_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("movedown_button")->textureID[0] = LoadTGA("Image/UI/MoveDown_Button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("moveleft_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("moveleft_button")->textureID[0] = LoadTGA("Image/UI/MoveLeft_Button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("moveright_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("moveright_button")->textureID[0] = LoadTGA("Image/UI/MoveRight_Button.tga");
		MeshBuilder::GetInstance()->GenerateQuad("reload_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("reload_button")->textureID[0] = LoadTGA("Image/UI/ReloadButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("nextwep_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("nextwep_button")->textureID[0] = LoadTGA("Image/UI/NextWepButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("blank_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("blank_button")->textureID[0] = LoadTGA("Image/UI/BlankButton.tga");
		MeshBuilder::GetInstance()->GenerateQuad("pause_button", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("pause_button")->textureID[0] = LoadTGA("Image/UI/PauseButton.tga");
		
		MeshBuilder::GetInstance()->GenerateQuad("gun_border", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("gun_border")->textureID[0] = LoadTGA("Image/UI/mapGroundTexture.tga");
		MeshBuilder::GetInstance()->GenerateQuad("mapGround", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("mapGround")->textureID[0] = LoadTGA("Image/UI/mapGroundTexture.tga");


		//----------GUN SPRITES
		MeshBuilder::GetInstance()->GenerateQuad("pistol", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("pistol")->textureID[0] = LoadTGA("Image/Weapon/pistol.tga");
		MeshBuilder::GetInstance()->GenerateQuad("shotgun", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("shotgun")->textureID[0] = LoadTGA("Image/Weapon/shotgun.tga");
		MeshBuilder::GetInstance()->GenerateQuad("rifle", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("rifle")->textureID[0] = LoadTGA("Image/Weapon/rifle.tga");
		MeshBuilder::GetInstance()->GenerateQuad("bow", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("bow")->textureID[0] = LoadTGA("Image/Weapon/bow.tga");
		MeshBuilder::GetInstance()->GenerateQuad("laser", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("laser")->textureID[0] = LoadTGA("Image/Weapon/laserGun.tga");

		MeshBuilder::GetInstance()->GenerateQuad("pistolLeft", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("pistolLeft")->textureID[0] = LoadTGA("Image/Weapon/pistolLeft.tga");
		MeshBuilder::GetInstance()->GenerateQuad("shotgunLeft", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("shotgunLeft")->textureID[0] = LoadTGA("Image/Weapon/shotgunLeft.tga");
		MeshBuilder::GetInstance()->GenerateQuad("rifleLeft", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("rifleLeft")->textureID[0] = LoadTGA("Image/Weapon/rifleLeft.tga");
		MeshBuilder::GetInstance()->GenerateQuad("bowLeft", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("bowLeft")->textureID[0] = LoadTGA("Image/Weapon/bowLeft.tga");
		MeshBuilder::GetInstance()->GenerateQuad("laserLeft", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("laserLeft")->textureID[0] = LoadTGA("Image/Weapon/laserGunLeft.tga");
		MeshBuilder::GetInstance()->GenerateQuad("minigun", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("minigun")->textureID[0] = LoadTGA("Image/Weapon/minigun.tga");
		MeshBuilder::GetInstance()->GenerateQuad("minigunLeft", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("minigunLeft")->textureID[0] = LoadTGA("Image/Weapon/minigunLeft.tga");
		MeshBuilder::GetInstance()->GenerateQuad("smg", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("smg")->textureID[0] = LoadTGA("Image/Weapon/smg.tga");
		MeshBuilder::GetInstance()->GenerateQuad("smgLeft", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("smgLeft")->textureID[0] = LoadTGA("Image/Weapon/smgLeft.tga");
		MeshBuilder::GetInstance()->GenerateQuad("rocketlauncher", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("rocketlauncher")->textureID[0] = LoadTGA("Image/Weapon/rocketlauncher.tga");
		MeshBuilder::GetInstance()->GenerateQuad("rocketlauncherLeft", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("rocketlauncherLeft")->textureID[0] = LoadTGA("Image/Weapon/rocketlauncherLeft.tga");

		//--------Bullets
		MeshBuilder::GetInstance()->GenerateQuad("pistolBullet", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("pistolBullet")->textureID[0] = LoadTGA("Image/Weapon/Bullets/bullet_pistol.tga");
		MeshBuilder::GetInstance()->GenerateQuad("rifleBullet", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("rifleBullet")->textureID[0] = LoadTGA("Image/Weapon/Bullets/bullet_rifle.tga");
		MeshBuilder::GetInstance()->GenerateQuad("laserBullet", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("laserBullet")->textureID[0] = LoadTGA("Image/Weapon/Bullets/bullet_lazer.tga");
		MeshBuilder::GetInstance()->GenerateQuad("bowBullet", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("bowBullet")->textureID[0] = LoadTGA("Image/Weapon/Bullets/bullet_bow.tga");

		//--------Ammo
		MeshBuilder::GetInstance()->GenerateQuad("ammo_pistol", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ammo_pistol")->textureID[0] = LoadTGA("Image/Weapon/Ammo/ammo_pistol.tga");

		MeshBuilder::GetInstance()->GenerateQuad("ammo_rifle", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ammo_rifle")->textureID[0] = LoadTGA("Image/Weapon/Ammo/ammo_rifle.tga");

		MeshBuilder::GetInstance()->GenerateQuad("ammo_rocket", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ammo_rocket")->textureID[0] = LoadTGA("Image/Weapon/Ammo/ammo_rocket.tga");

		MeshBuilder::GetInstance()->GenerateQuad("ammo_shotgun", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("ammo_shotgun")->textureID[0] = LoadTGA("Image/Weapon/Ammo/ammo_shotgun.tga");


		//--------Particles
		MeshBuilder::GetInstance()->GenerateQuad("blood", Color(0.7f, 0, 0), 1.f);

		//--------TileSet
		MeshBuilder::GetInstance()->GenerateQuad("tile_floor", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("tile_floor")->textureID[0] = LoadTGA("Image/TileSet/tile_floor.tga");
		MeshBuilder::GetInstance()->GenerateQuad("tile_floor2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("tile_floor2")->textureID[0] = LoadTGA("Image/TileSet/tile_floor2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("tile_stairs", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("tile_stairs")->textureID[0] = LoadTGA("Image/TileSet/tile_stairs.tga");
		MeshBuilder::GetInstance()->GenerateQuad("tile_wall", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("tile_wall")->textureID[0] = LoadTGA("Image/TileSet/tile_wall.tga");
		MeshBuilder::GetInstance()->GenerateQuad("tile_portal", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("tile_portal")->textureID[0] = LoadTGA("Image/TileSet/tile_portal.tga");

		
		/*MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
		MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");

		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
		MeshList::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");*/
	}
}