///////////////////////////////////////////////////////////////////////////////
/*!
\file   Particle.cpp
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
cpp file for particle class.
*/
///////////////////////////////////////////////////////////////////////////////

// Edited by Edward Chen
// Added: New Particle SFX, Emitter can set to follow based on specific target.

#include "Particle.h"

Particle::Particle()
	: m_age(0)
	, m_isActive(false)
	, m_scale(Vector3(.1, .1, .1))
	, m_position(Vector3(0, 0, 0))
	, m_velocity(Vector3(0, 0, 0))
	, m_direction(Vector3(1, 0, 0))
{
}

Particle::Particle(bool active, Vector3 scale, Vector3 pos, Vector3 vel, Vector3 dir) 
	: m_age(0)
	, m_isActive(active)
	, m_scale(scale)
	, m_position(pos)
	, m_velocity(vel)
	, m_direction(dir)
{
}

Particle::~Particle()
{
}