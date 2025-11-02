#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"
#include "PhysicsSphere.h"

struct Triangle
{
	glm::uvec3 indices;	
	glm::vec3 v0; // First vertex
	glm::vec3 v1; // Second vertex
	glm::vec3 v2; // Third vertex
	glm::vec3 normal; // Normal of the triangle
};

struct MyMesh
{
	GLuint m_VAO{ 0 };
	GLuint s_VAO{ 0 };
	GLuint t_VAO{ 0 };

	GLuint m_numElements{ 0 };
	GLuint s_numElements{ 0 };
	GLuint t_numElements{ 0 };
	
	glm::vec3 position;
	float radius;
	glm::quat rotation;

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colour;
	std::vector<GLuint> elements;
	
	std::vector<Triangle> triangles;

	float bounciness;

	int numberOfVertsInX;
	int numberOfVertsInZ;

	GLuint tex{ 0 };
};

class Renderer
{
private:
	// Program object - to host shaders
	GLuint m_program{ 0 };

	//vectors
	std::vector<MyMesh> s_meshes;
	std::vector<MyMesh> t_meshes;
	std::vector<MyMesh> m_sky;

	bool m_wireframe{ false };
	bool m_cullFace{ false };


	//phyisics spheres
	std::vector<PhysicsSphere> c_physicsSpheres;



public:
	Renderer();
	~Renderer();

	bool m_isPaused = false;
	bool m_shouldRestart = false;

	//gravity
	glm::vec3 Initialgravity = glm::vec3(0.0f, -98.1f, 0.0f);
	glm::vec3 gravity = glm::vec3(0.0f, -98.1f, 0.0f);

	float radius = 50;
	float mass = 50;
	
	//Skyblock
	bool Skybox();



	//meshes
	bool Sphere(glm::vec3& pos, float radius);
	float Noise(int x, int y);
	bool Terrain(glm::vec3 pos, int sizeX, int sizeZ, glm::quat rot, int spacing, bool noise);

	// Draw GUI
	void DefineGUI();

	// Create the program. This is the compiled shaders.
	bool CreateProgram();



	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();
	
	bool Renderer::sphereTriangleCollision(const glm::vec3& sphereCenter, float sphereRadius, const Triangle& triangle);

	glm::vec3 Renderer::closestPointOnTriangle(const glm::vec3& point, const Triangle& triangle);

	void Renderer::handleTriangleCollision(PhysicsSphere& sphere, const Triangle& triangle);

	//Update physics
	void UpdatePhysics(float deltaTime);

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};