#include "DetectMemoryLeak.h"
#include "UIElement.h"
#include "MeshBuilder.h"
#include "UIManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Application.h"
#include "MouseController.h"
#include "PlayerInfo\PlayerInfo.h"
#include "GL\glew.h"
#include "Boss.h"
#include "LevelStuff\Level.h"

UIElement::UIElement()
{
	//textObj = Create::Text2DObject("text", Vector3(1.0f, 1.0f, 0.0f), "", Vector3(25.f, 25.f, 25.f), Color(0.0f, 1.0f, 0.0f));
}

UIElement::UIElement(Mesh* _mesh) : mesh(_mesh)
{
}

UIElement::~UIElement()
{
}

void UIElement::Update()
{
	switch (elestate) {
	case MAIN_MENU:
		break;
	case PLAYING:
		break;
	case PAUSE:
		break;
	case OPTIONS:
		break;
	case VICTORY:
		break;
	case DEFEAT:
		break;
	default:
		// nothing should be here.
		break;
	}

	/*
	UIElement* resume = Create::UIEntity("resume_button", Vector3(0, 90, 9.5f), Vector3(175, 25, 1), true);
	resume->elestate = UIElement::ELEMENT_STATE::PAUSE;
	resume->type = UIElement::ELEMENT_TYPE::RESUME;

	UIElement* option = Create::UIEntity("option_button", Vector3(0, 40, 9.5f), Vector3(175, 25, 1), true);
	option->elestate = UIElement::ELEMENT_STATE::PAUSE;
	option->type = UIElement::ELEMENT_TYPE::OPTION;

	UIElement* exit = Create::UIEntity("exit_button", Vector3(0, -10, 9.5f), Vector3(175, 25, 1), true);
	exit->elestate = UIElement::ELEMENT_STATE::PAUSE;
	exit->type = UIElement::ELEMENT_TYPE::EXIT;
	*/

	float w = Application::GetInstance().GetWindowWidth();
	float hhalf = Application::GetInstance().GetWindowHeight() * 0.5f;

	// Main Menu
	// Pause
	if (this->type == START)
	{
		this->position.Set(0, hhalf * 0.3f, 9.5f);
		this->scale.Set(175 * w / 800, 25 * hhalf / 300, 1);
	}

	if (this->type == RESUME)
	{
		this->position.Set(0, hhalf * 0.3f, 9.5f);
		this->scale.Set(175 * w / 800, 25 * hhalf / 300, 1);
	}

	if (this->type == OPTION)
	{
		this->position.Set(0, hhalf * 0.1f, 9.5f);
		this->scale.Set(175 * w / 800, 25 * hhalf / 300, 1);
	}

	if (this->type == EXIT)
	{
		this->position.Set(0, hhalf * -0.1f, 9.5f);
		this->scale.Set(175 * w / 800, 25 * hhalf / 300, 1);
	}

	if (this->type == CANCEL)
	{
		this->position.Set(-60 * w / 800, hhalf * -0.8f, 9.5f);
		this->scale.Set(90 * w / 800, 15 * hhalf / 300, 1);
	}

	if (this->type == CONFIRM)
	{
		this->position.Set(80 * w / 800, hhalf * -0.8f, 9.5f);
		this->scale.Set(90 * w / 800, 15 * hhalf / 300, 1);
	}

	if (this->type == INPUT_RELOAD)
	{
		this->position.Set(-70 * w / 800, hhalf * 0.05f, 9.5f);
		this->scale.Set(60 * w / 800, 17 * hhalf / 300, 1);
	}

	if (this->type == INPUT_MOVE_UP)
	{
		this->position.Set(-65 * w / 800, hhalf * -0.05f, 9.5f);//
		this->scale.Set(73 * w / 800, 20 * hhalf / 300, 1);
	}

	if (this->type == INPUT_MOVE_DOWN)
	{
		this->position.Set(-61 * w / 800, hhalf * -0.14f, 9.5f);//
		this->scale.Set(80 * w / 800, 18 * hhalf / 300, 1);
	}
	
	if (this->type == INPUT_MOVE_LEFT)
	{
		this->position.Set(-61 * w / 800, hhalf *  -0.24f, 9.5f);//
		this->scale.Set(80 * w / 800, 18 * hhalf / 300, 1);
	}
	
	if (this->type == INPUT_MOVE_RIGHT)
	{
		this->position.Set(-59 * w / 800, hhalf * -0.34f, 9.5f);
		this->scale.Set(83 * w / 800, 19.5 * hhalf / 300, 1);
	}

	if (this->type == INPUT_NEXT_GUN)
	{
		this->position.Set(-59 * w / 800, hhalf * -0.44f, 9.5f);
		this->scale.Set(83 * w / 800, 19.5 * hhalf / 300, 1);
	}

	if (this->type == INPUT_BLANK)
	{
		this->position.Set(-59 * w / 800, hhalf * -0.54f, 9.5f);
		this->scale.Set(83 * w / 800, 19.5 * hhalf / 300, 1);
	}

	if (this->type == INPUT_PAUSE)
	{
		this->position.Set(-70 * w / 800, hhalf * -0.64f, 9.5f);
		this->scale.Set(53 * w / 800, 19.5 * hhalf / 300, 1);
	}

	if (this->type == tOVERVIEW)
	{
		this->position.Set(-150 * w/800, hhalf * 0.1f, 9.5f);
		this->scale.Set(80 * w / 800, 30 * hhalf / 300, 1);
	}

	if (this->m_bCollider)
	{
		Vector3 offset = this->position;
		offset.y += 15.f;

		this->SetAABB(Vector3((this->scale * 0.5f) + offset), Vector3((this->scale * -0.5f) + offset));
	}

	if (this->type == CURSOR)
	{
		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		float halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
		float halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;
		float posX = (static_cast<float>(x) - halfWindowWidth);
		float posY = (halfWindowHeight - static_cast<float>(y));

		this->SetPosition(Vector3(posX, posY, 10.f));
	}
}

void UIElement::Render()
{
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;

	switch (UIManager::GetInstance()->state) {
	case UIManager::GAME_STATE::MAIN_MENU:
	{
		Vector3 HUDposition(0.f, 0.f, 9.0f);
		Vector3 HUDscale(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 1.f);

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
		modelStack.Scale(HUDscale.x, HUDscale.y, HUDscale.z);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("main_menu"));
		modelStack.PopMatrix();
		break;
	}
	case UIManager::GAME_STATE::PLAYING:
	{
		Vector3 HUDposition(-halfWindowWidth + 45.f, halfWindowHeight - 45.f, 8.0f);
		Vector3 HUDscale(50, 50, 1);

		for (float i = Player::GetInstance()->GetHealth(); i >= 20.f; i -= 20.f)
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
			modelStack.Scale(HUDscale.x, HUDscale.y, HUDscale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("full_hp"));
			modelStack.PopMatrix();

			HUDposition.x += HUDscale.x + 0.1f;
		}

		if (static_cast<int>(Player::GetInstance()->GetHealth()) % 20 > 0)
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
			modelStack.Scale(HUDscale.x, HUDscale.y, HUDscale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("half_hp"));
			modelStack.PopMatrix();

			HUDposition.x += HUDscale.x + 0.1f;
		}

		float displace_x = HUDscale.x * Player::GetInstance()->GetMaxHealth() * 0.05f;
		HUDposition.Set(-halfWindowWidth + 45.f + displace_x, halfWindowHeight - 45.f, 8.0f);

		for (float i = (Player::GetInstance()->GetMaxHealth() - Player::GetInstance()->GetHealth()); i > 0; i -= 20.f)
		{
			HUDposition -= HUDscale.x - 0.1f;

			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
			modelStack.Scale(HUDscale.x, HUDscale.y, HUDscale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("no_hp"));
			modelStack.PopMatrix();
		}

		HUDposition.Set(-halfWindowWidth + 40.f, halfWindowHeight - 100.f, 8.0f);

		for (int i = 0; i < Player::GetInstance()->GetBlanks(); ++i)
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
			modelStack.Scale(HUDscale.x * 0.7f, HUDscale.y * 0.7f, HUDscale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("blank"));
			modelStack.PopMatrix();

			HUDposition += HUDscale.x * 0.7f;
		}

		HUDposition.Set(-halfWindowWidth + 40.f, halfWindowHeight - 150.f, 8.0f);

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
		modelStack.Scale(HUDscale.x * 0.6f, HUDscale.y * 0.6f, HUDscale.z);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("currency"));
		modelStack.PopMatrix();

		//render weapon in use
		modelStack.PushMatrix();
		modelStack.Translate(halfWindowWidth - 70.f, -halfWindowHeight + 60.f, 1.f);
		modelStack.Scale(100, 100, 100);
		RenderHelper::RenderMesh(Player::GetInstance()->getWeaponMesh());
		modelStack.PopMatrix();


		break;
	}
	case UIManager::GAME_STATE::PAUSE:
	{
		Vector3 HUDposition(0.f, 0.f, 9.0f);
		Vector3 HUDscale(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 1.f);

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
		modelStack.Scale(HUDscale.x, HUDscale.y, HUDscale.z);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("pause_menu"));
		modelStack.PopMatrix();

		//if (this->type == CURSOR)
		//	return;

		break;
	}
	case UIManager::GAME_STATE::VICTORY:
	{
		Vector3 HUDposition(0.f, 0.f, 9.0f);
		Vector3 HUDscale(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 1.f);

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
		modelStack.Scale(HUDscale.x, HUDscale.y, HUDscale.z);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("victory"));
		modelStack.PopMatrix();
		break;
	}
	case UIManager::GAME_STATE::DEFEAT:
	{
		Vector3 HUDposition(0.f, 0.f, 9.0f);
		Vector3 HUDscale(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 1.f);

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
		modelStack.Scale(HUDscale.x, HUDscale.y, HUDscale.z);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("defeat"));
		modelStack.PopMatrix();
		break;
	}
	case UIManager::GAME_STATE::OPTIONS:
	{
		Vector3 HUDposition(0.f, 0.f, 9.0f);
		Vector3 HUDscale(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 1.f);

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
		modelStack.Scale(HUDscale.x, HUDscale.y, HUDscale.z);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("option_menu"));
		modelStack.PopMatrix();
		break;
	}
	case UIManager::GAME_STATE::sOVERVIEW:
	{
		Vector3 HUDposition(0.f, 0.f, 9.0f);
		Vector3 HUDscale(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 1.f);

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
		modelStack.Scale(HUDscale.x, HUDscale.y, HUDscale.z);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("overview"));
		modelStack.PopMatrix();
		break;
	}

	}

	// TODO: Fix scaling with fullscreen. (windowwidth / 800), (windowheight / 800) <- something about this I think

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(mesh);
	modelStack.PopMatrix();
}

void UIElement::CollisionResponse(UIElement * ThatElement)
{

}

UIElement* Create::UIEntity(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale,
	bool _collision)
{
	Mesh* modelMesh = MeshList::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	UIElement* result = new UIElement(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(_collision);
	UIManager::GetInstance()->AddEntity(result);
	return result;
}