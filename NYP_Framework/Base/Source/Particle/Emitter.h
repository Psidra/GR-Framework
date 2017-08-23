///////////////////////////////////////////////////////////////////////////////
/*!
\file   Emitter.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for emitter functions. This class contains information of an emitter
that releases multiple particles.
*/
///////////////////////////////////////////////////////////////////////////////

// Edited by Edward Chen
// Added: New Particle SFX, Emitter can set to follow based on specific target.

#pragma once

#include <vector>
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "Mtx44.h"
#include "Particle.h"

class GenericEntity;

enum EFFECT_TYPE
{
	ET_FIRE = 0,
	ET_BLEED,
	ET_TOTAL
};

class Emitter
{
	const int MAX_PARTICLE = 100;
public:
	Emitter() : 
		m_particle_mesh(nullptr),
		m_effect_type(EFFECT_TYPE::ET_FIRE),
		m_position(Vector3(0, 0, 0)),
		m_velocity(Vector3(0, 0, 0)),
		m_age(0),
		m_isActive(true),
		m_isDone(false),
		m_emitterLifeSpan(10),
		m_particleLifeSpan(2)
	{};
	Emitter(EFFECT_TYPE type, Vector3 pos, Vector3 vel, double emitterLifeSpan, double particleLifeSpan, bool follow, GenericEntity* targetEnt)
	{
		this->m_effect_type = type;
		this->m_position = pos;
		this->m_velocity = vel;
		this->m_emitterLifeSpan = emitterLifeSpan;
		this->m_particleLifeSpan = particleLifeSpan;
		this->m_bFollow = follow;
		this->m_liveParticleCount = 0;
		this->m_age = 0;
		this->m_isActive = true;
		this->m_isDone = false;

		if (follow)
			this->targetEnt = targetEnt;
	};
	~Emitter() 
	{
		for (size_t i = 0; i < m_particles.size(); ++i)
		{
			m_particles[i] = nullptr;
			delete m_particles[i];
		}
	};

	/*Update the emitter and its particles*/
	void update(double dt);
	/*Render the emitter(if desired) and all active particles*/
	void render();

	/*Returns an inactive particle from particle pool*/
	Particle* fetchParticle(); 

	/*Set mesh for particles*/
	void setMesh(Mesh* meshName);
	/*Get mesh for particles*/
	Mesh* getMesh();

	void setPos(Vector3 pos);
	Vector3 getPos();

	/*Get isDone to signal entityManager to remove this instance*/
	bool getIsDone();

	// Get Follow
	bool getFollow();
	// Set Follow
	void setFollow(bool _follow);

private:
	std::vector<Particle*> m_particles; //All particles in this emitter
	EFFECT_TYPE m_effect_type;			//Type of effect. Affects how particles are updated
	Mesh* m_particle_mesh;				//Stores the particle's mesh & texture
	Vector3 m_position;					//Position of this emitter
	Vector3 m_velocity;					//Velocity of this emitter
	double m_age;						//How long this emitter is alive for
	bool m_isActive;						//Dead or alive (Used to stop spawning of particles)
	bool m_isDone;						//All particles are inactive and emitter is dead (Used to remove this entity)
	bool m_bFollow;						// Emitter follows target specified
	double m_emitterLifeSpan;			//How long this emitter can stay alive
	double m_particleLifeSpan;			//How long each particle can stay alive
	GenericEntity* targetEnt;			// target to follow, if exists
	int m_liveParticleCount;			//Total number of active particles
};