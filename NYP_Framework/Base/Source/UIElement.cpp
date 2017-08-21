#include "UIElement.h"
#include "MeshBuilder.h"
#include "UIManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Application.h"
#include "MouseController.h"
#include "PlayerInfo\PlayerInfo.h"

UIElement::UIElement()
{
}

UIElement::UIElement(Mesh* _mesh) : mesh(_mesh)
{
}

UIElement::~UIElement()
{
}

void UIElement::SetPosition(const Vector3 & _value)
{
	this->position = _value;
}

Vector3 UIElement::GetPosition()
{
	return this->position;
}

void UIElement::SetScale(const Vector3 & _value)
{
	this->scale = _value;
}

Vector3 UIElement::GetScale()
{
	return this->scale;
}

bool UIElement::HasCollider(void) const
{
	return this->m_bCollider;
}

void UIElement::SetCollider(const bool _value)
{
	this->m_bCollider = _value;
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
		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		float w = Application::GetInstance().GetWindowWidth();
		float h = Application::GetInstance().GetWindowHeight();
		float posX = (static_cast<float>(x) - (w * 0.5f)) / w * (12.5f * w/h);	// WTF DOES 12.5f EVEN MEAN
		float posY = ((h - static_cast<float>(y)) - (h * 0.5f)) / h * 12.5f;	// WHY DOES IT MAKE IT WORK

		this->SetPosition(Vector3(posX + Player::GetInstance()->GetPos().x, posY + Player::GetInstance()->GetPos().y, 0));
	}
}

void UIElement::Render()
{
	float w = Application::GetInstance().GetWindowWidth();
	float h = Application::GetInstance().GetWindowHeight();

	switch (UIManager::GetInstance()->state) {
	case UIManager::GAME_STATE::MAIN_MENU:

		break;
	case UIManager::GAME_STATE::PLAYING:
	{
		float displace_x = (-5.75f * w/h); // magic math bullshit that is totally hardcoded i have no idea how to fix this ples halp

		for (float i = Player::GetInstance()->GetHealth(); i > 10.f; i -= 20.f)
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate(displace_x + Player::GetInstance()->GetPos().x,
				5.6f + Player::GetInstance()->GetPos().y, 0);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("full_hp"));
			modelStack.PopMatrix();

			displace_x += scale.x;
		}

		if (static_cast<int>(Player::GetInstance()->GetHealth()) % 20 > 9)
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate((displace_x * (float)Application::GetInstance().GetWindowWidth() / (float)Application::GetInstance().GetWindowHeight()) + Player::GetInstance()->GetPos().x,
				5.6f + Player::GetInstance()->GetPos().y, 0);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("half_hp"));
			modelStack.PopMatrix();

			displace_x += scale.x;
		}

		for (float i = (Player::GetInstance()->GetMaxHealth() - Player::GetInstance()->GetHealth()); i > 0; i -= 20.f)
		{
			MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
			modelStack.PushMatrix();
			modelStack.Translate((displace_x * (float)Application::GetInstance().GetWindowWidth() / (float)Application::GetInstance().GetWindowHeight()) + Player::GetInstance()->GetPos().x,
				5.6f + Player::GetInstance()->GetPos().y, 0);
			modelStack.Scale(scale.x, scale.y, scale.z);
			RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("no_hp"));
			modelStack.PopMatrix();

			displace_x += scale.x;
		}

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