#ifndef _MINIMAP_H
#define _MINIMAP_H

//#include "EntityBase.h"
#include "../GenericEntity.h"
#include "SingletonTemplate.h"
#include "Mesh.h"
#include <map>
#include <list>
#include <vector>

class CMinimap : public GenericEntity, public Singleton<CMinimap>
{
private:
	static const int NUM_TYPE = 4;
public:
	enum SPRITE_RENDERMODE
	{
		MODE_2D,
		MODE_3D,
		NUM_MODE
	};

	enum MINIMAP_STATE
	{
		NONE,
		NORMAL,
		ENLARGED,
	} mapState;

	CMinimap(void);
	virtual ~CMinimap(void);

	Mesh* m_cMinimap_Background;
	Mesh* m_cMinimap_Border;
	Mesh* m_cMinimap_Avatar;
	Mesh* m_cMinimap_Target;
	Mesh* m_cMinimap_Stencil;
	Mesh* m_cMinimap_Object;

	// Initialise this class instance
	bool Init(void);

	// Set the target (a.k.a. the enemies in the minimap)
	bool SetTarget(Mesh* aTarget);
	// Get the target (a.k.a. the enemies in the minimap)
	Mesh* GetTarget(void) const;

	// Set the background mesh to this class
	bool SetBackground(Mesh* aBackground);
	// Get the background mesh to this class
	Mesh* GetBackground(void) const;

	// Set the Border mesh to this class
	bool SetBorder(Mesh* aBorder);
	// Get the Border mesh to this class
	Mesh* GetBorder(void) const;

	// Set the Avatar mesh to this class
	bool SetAvatar(Mesh* anAvatar);
	// Get the Avatar mesh to this class
	Mesh* GetAvatar(void) const;

	// Set rotation angle of avatar
	bool SetAngle(const int angle);
	// Get rotation angle of avatar
	int GetAngle(void) const;

	// Set the Stencil mesh to this class
	bool SetStencil(Mesh* aStencil);
	// Get the Stencil mesh to this class
	Mesh* GetStencil(void) const;

	// Set the Enemy mesh to this class
	bool SetObjectMesh(Mesh* _mesh);
	// Get the Enemy mesh to this class
	Mesh* GetObjectMesh(void) const;
	// Set object pos
	void setObject(Vector3 _pos, Vector3 _scale);
	// Get minimap map
	std::map <std::string, std::vector<Vector3>> getMinimapData();
	// set minimap map pos
	void setObjectPos(std::string _type, Vector3 _pos);
	// set minimap map scale
	void setObjectScale(std::string _type, Vector3 _scale);
	// get player map scale
	Vector3 getPlayerMapScale();
	// set player map scale
	void setPlayerMapScale(Vector3 _scale);
	// get bool enlarged
	bool getIsEnlarged();
	// set bool enlarged
	void setIsEnlarged(bool _isEnlarged);
	// enlarge minimap
	void EnlargeMap(bool _isEnlarged);
	//Add all telepos to list
	void addTeleporterPos(Vector3 _pos);
	//// Set position of avatar in minimap
	//bool SetPosition(const int x, const int y);
	//// Get position x of avatar in minimap
	//int GetPosition_x(void) const;
	//// Get position y of avatar in minimap
	//int GetPosition_y(void) const;

	//// Set size of minimap (for calculation of avatar in minimap)
	//bool SetSize(const int size_x, const int size_y);
	//// Get size of minimap (for calculation of avatar in minimap)
	//int GetSize_x(void) const;
	//// Get size of minimap (for calculation of avatar in minimap)
	//int GetSize_y(void) const;
	virtual void Update(double dt);
	virtual void RenderUI();

protected:
	// Rotation from First Angle
	int m_iAngle;
	// Number of wall
	int m_iNumWall;
	// Number of tele
	int m_iNumTele;
	//bool is enlarged
	bool m_bEnlarged;
	//obj pos & scale on map
	Vector3 objPos;
	Vector3 objScale;
	// player scale on map
	Vector3 playerMapScale;
	//player pos relative to map
	Vector3 playerMapPos;
	//range of minimap
	float m_fRange;
	//temporary storage method for render of map (will be changed maybe)
	std::map <std::string, std::vector<Vector3>> minimapData;
	//storing string id of map
	std::string mapID[NUM_TYPE];
	//teleporter pos
	std::vector<Vector3> teleporterActPos;
	SPRITE_RENDERMODE mode;
};

namespace Create
{
	CMinimap* Minimap(const bool m_bAddToLibrary = true);
};

#endif