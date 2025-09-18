#ifndef CIRCLE_H
#define CIRCLE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;

class Circle {
public:

	Circle(glm::vec2 position, float radius, int segments, glm::vec4 color);

	~Circle();

	void draw(Shader& shader);

private:

	unsigned int m_VAO, m_VBO;
	int m_IndexCount;

	glm::vec2 m_Position;
	float m_radius;
	int m_segments;
	glm::vec4 m_Color;

	void setupMesh();

};


#endif 

