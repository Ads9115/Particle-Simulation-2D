#pragma once
#include<glm/glm.hpp>

struct Particle {

	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec4 Color;
	float LifeRemaining = 0.0f;

	bool IsActive() const { return LifeRemaining > 0.0f; }

};

struct ParticleProps {

	glm::vec2 Position;
	glm::vec2 Velocity, velocityVariation;
	glm::vec4 colorStart, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float LifeTime = 1.0f;

};
