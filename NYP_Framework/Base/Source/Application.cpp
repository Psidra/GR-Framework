#include "Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "MeshList.h"

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
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		glfwPollEvents();
		UpdateInput();
		
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
		MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
		MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
		MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
		MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 1.f);
		MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
		MeshList::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
		MeshList::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);

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



		//-------ENEMY SPRITES
		//-------enemy1
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_fstand1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_fstand1")->textureID[0] = LoadTGA("Image/Enemies/enemy1_fstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_fstand2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_fstand2")->textureID[0] = LoadTGA("Image/Enemies/enemy1_fstand2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_bstand1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_bstand1")->textureID[0] = LoadTGA("Image/Enemies/enemy1_bstand1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_bstand2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_bstand2")->textureID[0] = LoadTGA("Image/Enemies/enemy1_bstand2.tga");

		MeshBuilder::GetInstance()->GenerateQuad("enemy1_fwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_fwalk1")->textureID[0] = LoadTGA("Image/Enemies/enemy1_fwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_fwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_fwalk2")->textureID[0] = LoadTGA("Image/Enemies/enemy1_fwalk2.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_bwalk1", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_bwalk1")->textureID[0] = LoadTGA("Image/Enemies/enemy1_bwalk1.tga");
		MeshBuilder::GetInstance()->GenerateQuad("enemy1_bwalk2", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("enemy1_bwalk2")->textureID[0] = LoadTGA("Image/Enemies/enemy1_bwalk2.tga");


		MeshBuilder::GetInstance()->GenerateQuad("Wall", Color(0, 0, 0), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("Floor", Color(1, 0, 0), 1.f);
		MeshBuilder::GetInstance()->GenerateQuad("Coord", Color(0, 0, 1), 1.f);

		//--------UI
		MeshBuilder::GetInstance()->GenerateQuad("full_hp", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("full_hp")->textureID[0] = LoadTGA("Image/UI/PlayerFullHealth.tga");
		MeshBuilder::GetInstance()->GenerateQuad("half_hp", Color(1, 1, 1), 1.f);
		MeshList::GetInstance()->GetMesh("half_hp")->textureID[0] = LoadTGA("Image/UI/PlayerHalfHealth02.tga");
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