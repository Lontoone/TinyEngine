#pragma once
#include <glad/glad.h>
#include <camera.h>
class RViewFrustum
{
public:
	explicit RViewFrustum(int numCascade, Camera* camera);
	virtual ~RViewFrustum();

	RViewFrustum(const RViewFrustum&) = delete;
	RViewFrustum(const RViewFrustum&&) = delete;
	RViewFrustum& operator=(const RViewFrustum&) = delete;

public:
	void init( Camera* camera);
	inline void update(const Camera* camera) {
		this->m_modelMat = camera->m_parent->m_model_matrix;
	}
	void resize( Camera* camera);
	void render();


private:
	const int m_numCascade;
	const int m_numVertex;
	const int m_numIndex;
	GLuint m_vertexBufferHandle = 0u;
	float* m_vertexBuffer = nullptr;

	GLuint m_vaoHandle = 0u;

	glm::mat4 m_modelMat;
};