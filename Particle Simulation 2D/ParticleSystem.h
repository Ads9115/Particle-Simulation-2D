#pragma once
#include"Particle.h"
#include"Shaders.h"
#include<vector>

class ParticleSystem {
public:

	ParticleSystem(unsigned int maxParticles = 1000);

	void update(float dt);
	void Draw(const glm::mat4 projection);

	void Emit(const ParticleProps& particleProps);

private:

	std::vector<Particle> m_ParticlePool;
	unsigned int m_PoolIndex;

	unsigned int m_VAO, m_VBO;
	Shader m_ParticleShader;

};
