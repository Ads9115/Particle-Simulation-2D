#include"ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

float RandomFloat(float max, float min) {

	return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));

}

ParticleSystem::ParticleSystem(unsigned int maxParticles)
    :m_ParticlePool(maxParticles),m_PoolIndex(maxParticles - 1),m_ParticleShader("particle.vert", "particle.frag") {

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (const void*)offsetof(Particle, Position));


    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const void*)offsetof(Particle, Color));

    glBindVertexArray(0);
}


void ParticleSystem::update(float dt) {

    static bool s_DebugParticleFound = false;

    const float gravity = -2.0f;

    for (auto& particle : m_ParticlePool) {

        if (!particle.IsActive())
            continue;

        if (!s_DebugParticleFound)
        {
            // Print the state of the first particle we find
            std::cout << "Pos: (" << particle.Position.x << ", " << particle.Position.y
                << ") | Vel: (" << particle.Velocity.x << ", " << particle.Velocity.y << ")" << std::endl;
        }

        particle.LifeRemaining -= dt;


        if (particle.LifeRemaining <= 0.0f) {
            particle.Color.a = 0.0f;
            if (!s_DebugParticleFound) {
                std::cout << "--- Particle Died ---" << std::endl;
                s_DebugParticleFound = true; // Stop printing after this one dies
            }
            continue;
        }

        float life = particle.LifeRemaining / particle.LifeTime;
        
        particle.Color.a = life;
        particle.Velocity.y += gravity * dt;
        particle.Position += particle.Velocity * dt;


    }
}


void ParticleSystem::Draw(const glm::mat4 projection) {

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_ParticlePool.size() * sizeof(Particle), m_ParticlePool.data());

    m_ParticleShader.use();
    m_ParticleShader.setMat4("projection", projection);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    glBindVertexArray(m_VAO);
    
    glDrawArrays(GL_POINTS, 0, m_ParticlePool.size());
    glBindVertexArray(0);

    glDisable(GL_BLEND);



}


void ParticleSystem::Emit(const ParticleProps& particleProps) {

    Particle& particle = m_ParticlePool[m_PoolIndex];

    particle.Position = particleProps.Position;
    particle.LifeRemaining = particleProps.LifeTime;
    particle.LifeTime = particleProps.LifeTime;

    particle.Velocity = particleProps.Velocity;
    particle.Velocity.x += particleProps.velocityVariation.x * RandomFloat(-0.5f, 0.5f);
    particle.Velocity.y += particleProps.velocityVariation.y * RandomFloat(-0.5f, 0.5f);

    particle.Color = particleProps.colorStart;

    m_PoolIndex = (m_PoolIndex - 1 + m_ParticlePool.size()) % m_ParticlePool.size();
    


}

