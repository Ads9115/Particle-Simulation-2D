#include"Shaders.h"
#include"Circles.h"
#include<vector>
#include<numbers>

void checkGLError() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL Error: " << err << std::endl;
	}
}

Circle::Circle(glm::vec2 position, float radius, int segments, glm::vec4 color)
	:m_Position(position), m_radius(radius), m_segments(segments), m_Color(color) {

	setupMesh();
};


Circle::~Circle() {

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void Circle::setupMesh() {

	std::vector<glm::vec2> vertices;

	vertices.push_back(m_Position);
	float angleIncrement = 2.0f * std::numbers::pi_v<float> / m_segments;
	for (int i = 0; i <= m_segments; i++) {

		float angle = i * angleIncrement;
		vertices.push_back(glm::vec2(
			m_Position.x + m_radius * cos(angle),
			m_Position.y + m_radius * sin(angle)

		));
	}

	m_IndexCount = vertices.size();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkGLError();

}

void Circle::draw(Shader& shader) {

	shader.use();

	shader.setVec4("uColor", m_Color);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_IndexCount);

	glBindVertexArray(0);

	checkGLError();
}