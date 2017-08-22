#include "UIElement.h"
#include "MeshBuilder.h"
#include "UIManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Application.h"
#include "MouseController.h"
#include "PlayerInfo\PlayerInfo.h"
#include "GL\glew.h"

UIElement::UIElement()
{
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

	default:
		// nothing should be here.
		break;
	}

	if (this->type == CURSOR)
	{
		//double x, y;
		//MouseController::GetInstance()->GetMousePosition(x, y);
		//float w = Application::GetInstance().GetWindowWidth();
		//float h = Application::GetInstance().GetWindowHeight();
		//float posX = (static_cast<float>(x) - (w * 0.5f)) / w * (12.5f * w/h);
		//float posY = ((h - static_cast<float>(y)) - (h * 0.5f)) / h * 12.5f;

		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
		float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
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

		break;
	case UIManager::GAME_STATE::PLAYING:
	{
		Vector3 HUDposition(-halfWindowWidth + 45.f, halfWindowHeight - 45.f, 8.0f);

		for (float i = Player::GetInstance()->GetHealth(); i > 10.f; i -= 20.f)
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("full_hp"));
			modelStack.PopMatrix();

			HUDposition.x += scale.x + 0.1f;
		}

		if (static_cast<int>(Player::GetInstance()->GetHealth()) % 20 == 10)
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("half_hp"));
			modelStack.PopMatrix();

			HUDposition.x += scale.x + 0.1f;
		}

		float displace_x = scale.x * Player::GetInstance()->GetMaxHealth() * 0.05f;
		HUDposition.Set(-halfWindowWidth + 45.f + displace_x, halfWindowHeight - 45.f, 8.0f);

		for (float i = (Player::GetInstance()->GetMaxHealth() - Player::GetInstance()->GetHealth()); i > 0; i -= 20.f)
		{
			HUDposition -= scale.x - 0.1f;

			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("no_hp"));
			modelStack.PopMatrix();
		}

		HUDposition.Set(-halfWindowWidth + 40.f, halfWindowHeight - 100.f, 8.0f);

		for (int i = 0; i < Player::GetInstance()->GetBlanks(); ++i)
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
			modelStack.Scale(scale.x * 0.7f, scale.y * 0.7f, scale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("blank"));
			modelStack.PopMatrix();

			HUDposition += scale.x * 0.7f;
		}

		HUDposition.Set(-halfWindowWidth + 40.f, halfWindowHeight - 150.f, 8.0f);

		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
		modelStack.PushMatrix();
		modelStack.Translate(HUDposition.x, HUDposition.y, HUDposition.z);
		modelStack.Scale(scale.x * 0.6f, scale.y * 0.6f, scale.z);
		RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("currency"));
		modelStack.PopMatrix();

		break;
	}
	case UIManager::GAME_STATE::PAUSE:

		break;
	case UIManager::GAME_STATE::OPTIONS:

		break;

	default:
		break;
	}

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