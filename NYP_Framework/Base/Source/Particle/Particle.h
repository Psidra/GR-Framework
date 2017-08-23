///////////////////////////////////////////////////////////////////////////////
/*!
\file   Particle.h
\author Lin Xin
\par	email: 163320B@mymail.nyp.edu.sg
\brief
Header file for particle functions. This class contains information of a single
particle.
*/
///////////////////////////////////////////////////////////////////////////////

// Edited by Edward Chen
// Added: New Particle SFX, Emitter can set to follow based on specific target.

#pragma once

#include "Vector3.h"

class Particle
{
public:
	Particle();
	Particle(bool active, Vector3 scale, Vector3 pos, Vector3 vel, Vector3 dir);
	~Particle();

	double m_age;			// How long has this particle been "alive"
	bool m_isActive;		// Determines whether to render it or not

	Vector3 m_scale;		// Scale for the particle

	Vector3 m_position;		// Position of this particle
	Vector3 m_velocity;		// Velocity of this particle
	Vector3 m_direction;	// Direction where this particle
};