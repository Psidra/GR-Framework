#include "Minimap.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "GL\glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "MeshList.h"


CMinimap::CMinimap(void)
	: m_cMinimap_Background(NULL)
	, m_cMinimap_Border(NULL)
	, m_cMinimap_Avatar(NULL)
	, m_cMinimap_Target(NULL)
	, m_cMinimap_Stencil(NULL)
	, m_iAngle(-90)
	, mode(MODE_2D)
{
	Init();
}
CMinimap::~CMinimap(void)
{
	if (m_cMinimap_Background)
	{
		delete m_cMinimap_Background;
		m_cMinimap_Background = NULL;
	}
	if (m_cMinimap_Border)
	{
		delete m_cMinimap_Border;
		m_cMinimap_Border = NULL;
	}
	if (m_cMinimap_Avatar)
	{
		delete m_cMinimap_Avatar;
		m_cMinimap_Avatar = NULL;
	}
	if (m_cMinimap_Target)
	{
		delete m_cMinimap_Target;
		m_cMinimap_Target = NULL;
	}
}

// Initialise this class instance
bool CMinimap::Init(void)
{
	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	m_iAngle = 0;

	//position.Set(halfWindowWidth, halfWindowHeight, 0.0f);
	//magical numbers here 
	position.Set(halfWindowWidth - 65.f, halfWindowHeight - 65.f, 0.0f);
	scale.Set(100.0f, 100.0f, 100.0f);
	//playerMapScale = Player::GetInstance()->GetScale() * (0.1);
	playerMapScale = Vector3(10,10,10) * (1 / scale.x);
	m_iNumWall = 0;
	m_iNumTele = 0;
	m_bEnlarged = false;
	m_fRange = scale.x * 0.1;
	mapState = NORMAL;

	//map id
	mapID[0] = "wallpos";
	mapID[1] = "wallscale";
	mapID[2] = "telepos";
	mapID[3] = "telescale";
	return true;
}

bool CMinimap::SetTarget(Mesh* aTarget)
{
	if(aTarget != NULL)
	{
		m_cMinimap_Target = aTarget;
		return true;
	}
	return false;
}
Mesh* CMinimap::GetTarget(void) const
{
	return m_cMinimap_Target;
}
// Set the background mesh to this class
bool CMinimap::SetBackground(Mesh* aBackground)
{
	if (aBackground != NULL)
	{
		m_cMinimap_Background = aBackground;
		return true;
	}
	return false;
}
// Get the background mesh to this class
Mesh* CMinimap::GetBackground(void) const
{
	return m_cMinimap_Background;
}
// Set the Border mesh to this class
bool CMinimap::SetBorder(Mesh* aBorder)
{
	if (aBorder != NULL)
	{
		m_cMinimap_Border = aBorder;
		return true;
	}
	return false;
}
// Get the Border mesh to this class
Mesh* CMinimap::GetBorder(void) const
{
	return m_cMinimap_Border;
}
// Set the Avatar mesh to this class
bool CMinimap::SetAvatar(Mesh* anAvatar)
{
	if (anAvatar != NULL)
	{
		m_cMinimap_Avatar = anAvatar;
		return true;
	}
	return false;
}
// Get the Avatar mesh to this class
Mesh* CMinimap::GetAvatar(void) const
{
	return m_cMinimap_Avatar;
}

// Set m_iAngle of avatar
bool CMinimap::SetAngle(const int m_iAngle)
{
	this->m_iAngle = m_iAngle;
	return true;
}
// Get m_iAngle
int CMinimap::GetAngle(void) const
{
	return m_iAngle;
}

// Set the Stencil mesh to this class
bool CMinimap::SetStencil(Mesh* aStencil)
{
	if (aStencil != NULL)
	{
		m_cMinimap_Stencil = aStencil;
		return true;
	}
	return false;
}
// Get the Stencil mesh to this class
Mesh* CMinimap::GetStencil(void) const
{
	return m_cMinimap_Stencil;
}

void CMinimap::setObjectPos(std::string _type, Vector3 _pos)
{
	minimapData[_type].push_back(_pos);
}

// Set the Enemy mesh to this class
bool CMinimap::SetObjectMesh(Mesh* _mesh)
{
	if (_mesh == NULL)
		return false;

	m_cMinimap_Object = _mesh;
	return true;
}

// Get the Enemy mesh to this class
Mesh* CMinimap::GetObjectMesh(void) const
{
	return m_cMinimap_Object;
}

void CMinimap::setObject(Vector3 _pos ,Vector3 _scale)
{
	objPos = _pos;
	objScale = _scale;
}

std::map<std::string, std::vector<Vector3>> CMinimap::getMinimapData()
{
	return minimapData;
}
// set minimap map scale
void CMinimap::setObjectScale(std::string _type, Vector3 _scale)
{
	minimapData[_type].push_back(_scale);
}
// get player map scale
Vector3 CMinimap::getPlayerMapScale()
{
	return playerMapScale;
}
// set player map scale
void CMinimap::setPlayerMapScale(Vector3 _scale)
{
	playerMapScale = _scale;
}
// get bool enlarged
bool CMinimap::getIsEnlarged()
{
	return m_bEnlarged;
}
// set bool enlarged
void CMinimap::setIsEnlarged(bool _isEnlarged)
{
	m_bEnlarged = _isEnlarged;
}
// enlarge minimap
void CMinimap::EnlargeMap(bool _isEnlarged)
{
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;

	if (_isEnlarged)
	{
		scale.Set(500, 500, 1);
		position.Set(0, 0, 9.f);
		m_fRange = scale.x * 0.001f;
		mapState = ENLARGED;
	}
	else
	{
		scale.Set(100, 100, 1);
		//position.Set(335.f, 235.f, 0.0f);
		position.Set(halfWindowWidth - 65.f, halfWindowHeight - 65.f, -1000.0f);
		m_fRange = scale.x * 0.01f;
		mapState = NORMAL;
	}
}

void CMinimap::addTeleporterPos(Vector3 _pos)
{
	teleporterActPos.push_back(_pos);
}

void CMinimap::addToMinimapList(EntityBase * _entity)
{
	minimapList.push_back(_entity);
}

std::list<EntityBase*>& CMinimap::getMinimapList(void)
{
	return minimapList;
}

std::vector<Level::Rectangle> CMinimap::getMiniMapRooomList()
{
	return mmRoomList;
}

void CMinimap::setMiniMapRoomList(std::vector<Level::Rectangle> _mmRoom)
{
	mmRoomList = _mmRoom;
}

//update minimap
void CMinimap::Update(double dt)
{	//temp storing method to be changed for storing the individual walls to instead the room only

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f; //400
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f; //300

	for (int i = 0; i < NUM_TYPE; ++i)
	{
		for (std::vector<Vector3>::iterator it = minimapData[mapID[i]].begin();
			it != minimapData[mapID[i]].end();++it)
		{
			if (mapID[i] == "wallscale")
				(*it) = (*it) * (1.0f / (scale.x * 0.05));
			else
				(*it) = (*it) * (1.0f / (scale.x * 0.1));
		}
	}

	m_iNumWall = minimapData["wallpos"].size();
	m_iNumTele = minimapData["telepos"].size();


	switch (mapState)
	{
	case NORMAL:
		position.Set(halfWindowWidth - 65.f, halfWindowHeight - 65.f, 0.0f);
		break;
	case ENLARGED:
	{
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

			Vector3 temp(posX, posY, 0);
			//std::cout << "mousePos: " << temp << "\n";

			for (int i = 0; i < m_iNumTele; ++i)
			{
				if ((minimapData["telepos"][i] - playerMapPos).LengthSquared() > m_fRange)
					continue;

				//std::cout << "mmPos: " << minimapData["telepos"][i] * scale.x << "\n";

				if (temp < minimapData["telepos"][i] * scale.x + minimapData["telescale"][i] * scale.x  &&
					temp > minimapData["telepos"][i] * scale.x - minimapData["telescale"][i] * scale.x)
				{
					Player::GetInstance()->SetPos(teleporterActPos[i]);
					EnlargeMap(false);
					m_bEnlarged = false;
					break;
					//std::cout << "index: " << i <<  " <in range to tele> \n\n";
				}
			}
		}
	}
		break;
	default:
		break;
	}

	playerMapScale = Vector3(10, 10, 10) * (1 / scale.x);
	playerMapPos = Player::GetInstance()->GetPos() * (1.0f / (scale.x * 0.2));
}

void CMinimap::RenderUI()
{
	if (mode == MODE_3D)
		return;

	//objPos = objPos * (1.0f / scale.x);
	//objScale = objScale * (1.0f / 50.f);

	//std::cout << map_ePos << std::endl;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
		// Translate the current transformation
		modelStack.Translate(position.x, position.y, position.z);
		// Scale the current transformation
		modelStack.Scale(scale.x, scale.y, scale.z);

		// Push the current transformation into the modelStack
		modelStack.PushMatrix();

			// Enable stencil mode
			glEnable(GL_STENCIL_TEST);

			// Configure stencil mode
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF); // Write to stencil buffer
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Don't write to colour buffer
			glDepthMask(GL_FALSE); // Don't write to depth buffer
			glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

			if (m_cMinimap_Stencil)
				RenderHelper::RenderMesh(m_cMinimap_Stencil);

			// Switch off stencil function
			glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
			glStencilMask(0x00); // Don't write anything to stencil buffer
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Write to colour buffer
			glDepthMask(GL_TRUE); // Write to depth buffer

			modelStack.PushMatrix();
				// Rotate the current transformation
				modelStack.Rotate(m_iAngle, 0.0f, 0.0f, -1.0f);

				if (m_cMinimap_Background)
					RenderHelper::RenderMesh(m_cMinimap_Background);
			
				// Render walls
				glDisable(GL_DEPTH_TEST);
				for (int i = 0; i < m_iNumWall; ++i)
				{
					//if (playerMapPos.LengthSquared() > minimapData["wallpos"][i].LengthSquared())
					if((minimapData["wallpos"][i] - playerMapPos).LengthSquared() < m_fRange)
					{
						//std::cout << minimapData["wallpos"][i] << " render\n";
						modelStack.PushMatrix();
						modelStack.Translate(minimapData["wallpos"][i].x, minimapData["wallpos"][i].y, 0);
						// Rotate the current transformation
						modelStack.Rotate(m_iAngle, 0.0f, 0.0f, -1.0f);
						modelStack.Scale(minimapData["wallscale"][i].x, minimapData["wallscale"][i].y, minimapData["wallscale"][i].z);
						// Render an object
						RenderHelper::RenderMesh(m_cMinimap_Object);
						modelStack.PopMatrix();
					}
				}
				for (int i = 0; i < m_iNumTele; ++i)
				{
					if ((minimapData["telepos"][i] - playerMapPos).LengthSquared() < m_fRange)
					{
						//std::cout << minimapData["wallpos"][i] << " render\n";
						modelStack.PushMatrix();
						modelStack.Translate(minimapData["telepos"][i].x, minimapData["telepos"][i].y, 0);
						// Rotate the current transformation
						modelStack.Rotate(m_iAngle, 0.0f, 0.0f, -1.0f);
						modelStack.Scale(minimapData["telescale"][i].x, minimapData["telescale"][i].y, minimapData["telescale"][i].z);
						// Render an object
						RenderHelper::RenderMesh(MeshList::GetInstance()->GetMesh("greenCube"));
						modelStack.PopMatrix();
					}
				}
				glEnable(GL_DEPTH_TEST);

			modelStack.PopMatrix();

			// Disable depth test
			glDisable(GL_DEPTH_TEST);

			// Display the Avatar
			if (m_cMinimap_Avatar)
			{
				modelStack.PushMatrix();
				//modelStack.Translate(Player::GetInstance()->GetPos().x  * (1 / scale.x), Player::GetInstance()->GetPos().y  * (1 / scale.x), Player::GetInstance()->GetPos().z);
				modelStack.Rotate(Math::RadianToDegree(atan2(Player::GetInstance()->GetView().y, Player::GetInstance()->GetView().x)) - 90, 0, 0, 1);
				modelStack.Scale(playerMapScale.x, playerMapScale.y, playerMapScale.z);
				RenderHelper::RenderMesh(m_cMinimap_Avatar);
				modelStack.PopMatrix();
			}

			// Enable depth test
			glEnable(GL_DEPTH_TEST);

			// Disable stencil test
			glDisable(GL_STENCIL_TEST);
		modelStack.PopMatrix();

		if (m_cMinimap_Border)
			RenderHelper::RenderMesh(m_cMinimap_Border);

	modelStack.PopMatrix();

	//clear all pos
	minimapData["wallpos"].clear();
	minimapData["wallscale"].clear();
	minimapData["telepos"].clear();
	minimapData["telescale"].clear();
	teleporterActPos.clear();
}

CMinimap* Create::Minimap(const bool m_bAddToLibrary)
{
	CMinimap* result = CMinimap::GetInstance();
	if (m_bAddToLibrary)
		EntityManager::GetInstance()->AddEntity(result, false);
	return result;
}
