#include "Renderer.h"
#include "Camera.h"
#include "Helper.h"
#include "ImageLoader.h"
#include <cmath>

using namespace std;

// Globals
GLuint gProgram;
GLuint gVAO;

Renderer::Renderer()
{
}

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	MyMesh newMesh;
	glDeleteProgram(m_program);
	glDeleteProgram(newMesh.s_VAO);
	glDeleteProgram(newMesh.m_VAO);
	glDeleteProgram(newMesh.t_VAO);
}

//skybox
bool Renderer::Skybox()
{
	Helpers::ModelLoader loader;
	if (!loader.LoadFromFile("Data\\Models\\Sky\\Hills\\skybox.x"))
		return false;

	// Now we can loop through all the mesh in the loaded model:
	for (const Helpers::Mesh& mesh : loader.GetMeshVector())
	{
		MyMesh newMesh;

		//positions
		GLuint positionsVBO;
		glGenBuffers(1, &positionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
		//clear binding
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//normals
		GLuint normalsVBO;
		glGenBuffers(1, &normalsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.normals.size(), mesh.normals.data(), GL_STATIC_DRAW);
		//clear binding
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//elements
		GLuint elementsEBO;
		glGenBuffers(1, &elementsEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.elements.size(), mesh.elements.data(), GL_STATIC_DRAW);
		//clear binding
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//uvcoords
		GLuint uvcoordsVBO;
		glGenBuffers(1, &uvcoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, uvcoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvCoords.size(), mesh.uvCoords.data(), GL_STATIC_DRAW);
		//clear binding
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		newMesh.m_numElements = mesh.elements.size();


		//VAO
		glGenVertexArrays(1, &newMesh.m_VAO);
		glBindVertexArray(newMesh.m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ARRAY_BUFFER, uvcoordsVBO);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
		glBindVertexArray(0);


		std::string fname{ loader.GetMaterialVector()[mesh.materialIndex].diffuseTextureFilename };

		//texture
		Helpers::ImageLoader imageLoader;
		if (!imageLoader.Load("Data/Models/Sky/Hills/" + fname))
			return false;

		glGenTextures(1, &newMesh.tex);
		glBindTexture(GL_TEXTURE_2D, newMesh.tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageLoader.Width(), imageLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageLoader.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		m_sky.push_back(newMesh);
	}
	return true;
}

//Sphere
bool Renderer::Sphere(glm::vec3& pos, float radius) {
	Helpers::ModelLoader Loader;
	if (!Loader.LoadFromFile("Data\\Models\\Sphere\\ball.obj"))
		return false;

	// Now we can loop through all the jeep_mesh in the loaded model:
	for (Helpers::Mesh mesh : Loader.GetMeshVector())
	{
		// Scale the vertices by the given radius and translate them to the position
		std::vector<glm::vec3> scaledVertices = mesh.vertices;
		for (auto& vertex : scaledVertices)
		{
			// Scale the vertex by the radius and translate it to the given position
			vertex = (vertex * radius) + pos; // Apply scaling and translation
		}
		MyMesh newMesh;
		//positions
		GLuint positionsVBO;
		glGenBuffers(1, &positionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
		//clear binding
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//normals
		GLuint normalsVBO;
		glGenBuffers(1, &normalsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.normals.size(), mesh.normals.data(), GL_STATIC_DRAW);
		//clear binding
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//elements
		GLuint elementsEBO;
		glGenBuffers(1, &elementsEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.elements.size(), mesh.elements.data(), GL_STATIC_DRAW);
		//clear binding
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//uvcoords
		GLuint uvcoordsVBO;
		glGenBuffers(1, &uvcoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, uvcoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvCoords.size(), mesh.uvCoords.data(), GL_STATIC_DRAW);
		//clear binding
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//VAO
		glGenVertexArrays(1, &newMesh.s_VAO);
		glBindVertexArray(newMesh.s_VAO);

		//elements
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ARRAY_BUFFER, uvcoordsVBO);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);

		glBindVertexArray(0);

		newMesh.s_numElements = (GLuint)mesh.elements.size();


		//texture
		Helpers::ImageLoader imageLoader;
		if (!imageLoader.Load("Data\\Models\\Sphere\\metal.jpg"))
			return false;

		glGenTextures(1, &newMesh.tex);
		glBindTexture(GL_TEXTURE_2D, newMesh.tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageLoader.Width(), imageLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageLoader.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


		s_meshes.push_back(newMesh);
	}

	return true;
}

// Noise
float Renderer::Noise(int x, int y)
{
	int n = x + y * 46;
	n = (n >> 13) ^ n;
	int nn = (n * (n * n * 60493 + 199990303) + 1376312589) & 0x7fffffff;
	return 1.0f - ((float)nn / 1073741824.0f);
}

//Terrain
bool Renderer::Terrain(glm::vec3 pos, int sizeX, int sizeZ, glm::quat rot, int spacing, bool noise)
{
	MyMesh newMesh;

	newMesh.position = pos;
	newMesh.rotation = rot;

	//vectors
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> colours;
	std::vector<glm::vec2> texcoords;
	std::vector<GLuint> elements;
	std::vector<glm::vec3> normals;

	//check if size is an even number
	if (sizeX  % 2 != 0)
		sizeX += 1;
	if (sizeZ % 2 != 0)
		sizeZ += 1;

	//number of verts
	int numberOfVertsInX = sizeX;
	int numberOfVertsInZ = sizeZ;

	//total cells
	int numCellX = numberOfVertsInX - 1;
	int numCellZ = numberOfVertsInZ - 1;

	//toggle for diamond shape
	bool toggleForDiamondPattern = true;
	float textureScale = .05f;

	// Calculate half the width and depth to center the terrain around pos
	float halfWidth = (numberOfVertsInX - 1) * spacing * 0.5f;
	float halfDepth = (numberOfVertsInZ - 1) * spacing * 0.5f;

	// terrain verts + texture coordinates
	for (int cellZ = 0; cellZ < numberOfVertsInZ; cellZ++)
	{
		for (int cellX = 0; cellX < numberOfVertsInX; cellX++)
		{
			// Calculate vertex position centered around (0, 0, 0) before rotation
			glm::vec3 centeredPos(cellX * spacing - halfWidth, pos.y, cellZ * spacing - halfDepth);

			// Apply rotation using the quaternion, then translate to pos
			glm::vec3 rotatedPos = rot * centeredPos + pos;

			// Store the rotated vertex position
			verts.push_back(rotatedPos);

			// Calculate texture coordinates
			float xTex = cellX * textureScale;
			float yTex = cellZ * textureScale;
			texcoords.push_back(glm::vec2(xTex, yTex));
			colours.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
		}
	}


	// terrain elements (indices)
	for (int cellZ = 0; cellZ < numCellZ; cellZ++)
	{
		for (int cellX = 0; cellX < numCellX; cellX++)
		{
			int startVertIndex = cellZ * numberOfVertsInX + cellX;

			if (toggleForDiamondPattern)
			{
				elements.push_back(startVertIndex + numberOfVertsInX);
				elements.push_back(startVertIndex + 1);
				elements.push_back(startVertIndex);
				elements.push_back(startVertIndex + numberOfVertsInX);
				elements.push_back(startVertIndex + numberOfVertsInX + 1);
				elements.push_back(startVertIndex + 1);
			}
			else
			{
				elements.push_back(startVertIndex + numberOfVertsInX + 1);
				elements.push_back(startVertIndex + 1);
				elements.push_back(startVertIndex);
				elements.push_back(startVertIndex + numberOfVertsInX);
				elements.push_back(startVertIndex + numberOfVertsInX + 1);
				elements.push_back(startVertIndex);
			}
			toggleForDiamondPattern = !toggleForDiamondPattern;
		}
	}

	// Apply noise to vertex heights
	bool noiseOn = noise;
	bool makeExt = true;
	if (noiseOn)
	{
		int index = 0;
		for (int i = 0; i < numberOfVertsInZ; i++) {
			for (int j = 0; j < numberOfVertsInX; j++) {
				float noiseValue = Noise(i, j);
				noiseValue = (noiseValue + 1.00001f) / 2;
				if (makeExt) {
					noiseValue *= 20;
				}
				verts[index].y += noiseValue;
				index++;
			}
		}
	}

	// Initialize normals
	normals.resize(verts.size(), glm::vec3(0.0f));

	// Accumulate normals for each triangle
	for (size_t i = 0; i < elements.size(); i += 3)
	{
		GLuint idx0 = elements[i];
		GLuint idx1 = elements[i + 1];
		GLuint idx2 = elements[i + 2];

		glm::vec3 v0 = verts[idx0];
		glm::vec3 v1 = verts[idx1];
		glm::vec3 v2 = verts[idx2];

		// Calculate the normal of the triangle
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;
		glm::vec3 triangleNormal = glm::normalize(glm::cross(edge1, edge2));

		// Accumulate the normal into each vertex normal
		normals[idx0] += triangleNormal;
		normals[idx1] += triangleNormal;
		normals[idx2] += triangleNormal;
	}

	// Normalize all vertex normals
	for (auto& normal : normals) {
		normal = glm::normalize(normal);
	}

	newMesh.t_numElements = (GLuint)elements.size();

	for (size_t i = 0; i < newMesh.t_numElements; i += 3)
	{
		GLuint idx0 = elements[i];
		GLuint idx1 = elements[i + 1];
		GLuint idx2 = elements[i + 2];

		Triangle tri;
		tri.v0 = verts[idx0];
		tri.v1 = verts[idx1];
		tri.v2 = verts[idx2];

		// Calculate the normal of the triangle
		glm::vec3 edge1 = tri.v1 - tri.v0;
		glm::vec3 edge2 = tri.v2 - tri.v0;
		tri.normal = glm::normalize(glm::cross(edge1, edge2));

		newMesh.triangles.push_back(tri);
	}

	//positions
	GLuint positionsVBO;
	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * verts.size(), verts.data(), GL_STATIC_DRAW);
	//clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//colours
	GLuint coloursVBO;
	glGenBuffers(1, &coloursVBO);
	glBindBuffer(GL_ARRAY_BUFFER, coloursVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colours.size(), colours.data(), GL_STATIC_DRAW);
	//clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//normals
	GLuint normalsVBO;
	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	//clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//texcoords
	GLuint texcoordsVBO;
	glGenBuffers(1, &texcoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texcoords.size(), texcoords.data(), GL_STATIC_DRAW);
	//clear binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//elements
	GLuint elementsEBO;
	glGenBuffers(1, &elementsEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);
	//clear binding
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//VAO
	glGenVertexArrays(1, &newMesh.m_VAO);
	glBindVertexArray(newMesh.m_VAO);


	//positions
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//texcoords
	glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//texcoords
	glBindBuffer(GL_ARRAY_BUFFER, coloursVBO);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	//elements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);

	glBindVertexArray(0);

	//texture
	Helpers::ImageLoader imageLoader;
	if (!imageLoader.Load("Data\\Models\\Sphere\\metal2.jpg"))
		return false;

	glGenTextures(1, &newMesh.tex);
	glBindTexture(GL_TEXTURE_2D, newMesh.tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageLoader.Width(), imageLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageLoader.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	t_meshes.push_back(newMesh);

	return true;
}

// Use IMGUI for a simple on-screen GUI
void Renderer::DefineGUI()
{

	// Loop through each sphere and create a collapsible menu for each one
	for (size_t i = 0; i < c_physicsSpheres.size(); i++)
	{
		// Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		ImGui::Begin(("Sphere " + std::to_string(i)).c_str());
		// Set each header to open by default on first load
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		ImGui::SetWindowPos(ImVec2(10, 300*i+30)); 
		ImGui::SetWindowSize(ImVec2(300, 300));
		// Normal header color
		ImVec4 normheaderColor = ImVec4(c_physicsSpheres[i].colour.x, c_physicsSpheres[i].colour.y, c_physicsSpheres[i].colour.z, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Header, normheaderColor);

		// Hovered header color
		ImVec4 hovheaderColor = ImVec4(c_physicsSpheres[i].colour.x * 1.1f, c_physicsSpheres[i].colour.y * 1.1f, c_physicsSpheres[i].colour.z * 1.1f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, hovheaderColor);

		// Active header color
		ImVec4 activheaderColor = ImVec4(c_physicsSpheres[i].colour.x * 0.9f, c_physicsSpheres[i].colour.y * 0.9f, c_physicsSpheres[i].colour.z * 0.9f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, activheaderColor);

		// Reset color back to default
		ImGui::PopStyleColor(3);

		// DragFloat to change the radius of each physics sphere
		ImGui::DragFloat(("Radius##" + std::to_string(i)).c_str(), &c_physicsSpheres[i].radius, 1.0f, 5.0f, 100.0f, "%.1f");

		// DragFloat to change the mass of each physics sphere
		ImGui::DragFloat(("Mass##" + std::to_string(i)).c_str(), &c_physicsSpheres[i].mass, 1.0f, 5.0f, 100.0f, "%.1f");
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		// Header for Position
		if (ImGui::CollapsingHeader(("Position " + std::to_string(i)).c_str()))
		{
			// DragFloat to change the velocity of each physics sphere
			ImGui::DragFloat(("X##" + std::to_string(i)).c_str(), &c_physicsSpheres[i].position.x, 1.0f, -999.0f, 999.0f, "%.1f");
			ImGui::DragFloat(("Y##" + std::to_string(i)).c_str(), &c_physicsSpheres[i].position.y, 1.0f, -999.0f, 999.0f, "%.1f");
			ImGui::DragFloat(("Z##" + std::to_string(i)).c_str(), &c_physicsSpheres[i].position.z, 1.0f, -999.0f, 999.0f, "%.1f");
		}
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		// Header for velocity
		if (ImGui::CollapsingHeader(("Velocity " + std::to_string(i)).c_str()))
		{
			// DragFloat to change the velocity of each physics sphere
			ImGui::DragFloat(("X##" + std::to_string(i)).c_str(), &c_physicsSpheres[i].velocity.x, 1.0f, -999.0f, 999.0f, "%.1f");
			ImGui::DragFloat(("Y##" + std::to_string(i)).c_str(), &c_physicsSpheres[i].velocity.y, 1.0f, -999.0f, 999.0f, "%.1f");
			ImGui::DragFloat(("Z##" + std::to_string(i)).c_str(), &c_physicsSpheres[i].velocity.z, 1.0f, -999.0f, 999.0f, "%.1f");
		}

		ImGui::End();
	}
	
	// General settings GUI
	{
		ImGui::Begin("3GP");

		// Display some general text (you can use format strings too)
		ImGui::Text("Settings:");

		// General checkboxes not related to individual spheres
		ImGui::Checkbox("Wireframe", &m_wireframe);
		ImGui::Checkbox("Cull Face", &m_cullFace);

		if (ImGui::Button("Restart"))
		{
			m_shouldRestart = true;
		}

		//Pause
		ImGui::Checkbox("Pause", &m_isPaused);
		
		//Gravity
		ImGui::InputFloat("Gravity", &gravity.y, 0.1f, 1.0f, "%.2f");
		if (ImGui::Button("ResetGravity"))
			gravity = Initialgravity;

		// Display application stats
		ImGui::Text("Application average %.0f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()
{
	if (!m_program)
		glDeleteProgram(m_program);

	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program))
		return false;

	return true;
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry() 
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;
	//enmviroment
	Skybox();
	//terrain
	float spacing = 100.0f;
	// Ground
	Terrain(glm::vec3(0.0f), 50, 50, glm::quat(), spacing, false);

	// Back Wall
	Terrain(glm::vec3(0.0f, 450.0f, -2900.0f), 50, 10, glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)), spacing, false);

	// Front Wall
	Terrain(glm::vec3(0.0f, 450.0f, 2900.0f), 50, 10, glm::angleAxis(glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f)), spacing, false);

	// Left Wall
	Terrain(glm::vec3(-2900.0f, 450.0f, 0.0f), 10, 50, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f)), spacing, false);

	// Right Wall
	Terrain(glm::vec3(2900.0f, 450.0f, 0.0f), 10, 50, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)), spacing, false);

	//Spheres
	c_physicsSpheres.emplace_back(glm::vec3(2000.0f, 650.0f, 400.0f), glm::vec3(0.0f, 0.0f, 0.0f), 200.0f, 50.0f, 0.05f); // Sphere 1
	c_physicsSpheres.emplace_back(glm::vec3(-2000.0f, 650.0f, 400.0f), glm::vec3(0.0f, 0.0f, 0.0f), 150.0f, 60.0f, 0.05f); // Sphere 2

	// Create visual representations for spheres with different colors
	for (auto& sphere : c_physicsSpheres)
	{
		Sphere(sphere.position, sphere.radius); // genereta sphere(s)
	}

	return true;
}

// Handle collision response
void handleCollision(PhysicsSphere& a, PhysicsSphere& b)
{
	glm::vec3 normal = b.position - a.position;
	float dist = glm::length(normal);
	if (dist == 0.0f) return; // prevent divide by zero
	normal /= dist;

	// Position correction
	float overlap = (a.radius + b.radius) - dist;
	if (overlap > 0.0f)
	{
		float totalMass = a.mass + b.mass;
		a.position -= normal * (overlap * (b.mass / totalMass));
		b.position += normal * (overlap * (a.mass / totalMass));
	}

	// Relative velocity
	glm::vec3 relativeVelocity = b.velocity - a.velocity;
	float velAlongNormal = glm::dot(relativeVelocity, normal);
	if (velAlongNormal > 0.0f) return;

	// Impulse calculation
	float e = std::min(a.restitution, b.restitution);
	float j = -(1.0f + e) * velAlongNormal / (1.0f / a.mass + 1.0f / b.mass);
	glm::vec3 impulse = j * normal;

	// Apply impulse
	a.velocity -= impulse / a.mass;
	b.velocity += impulse / b.mass;
}

//Sphere2Sphere Collision Check
bool TestSphereToSphereCollision(PhysicsSphere& a, PhysicsSphere& b)
{
	float totalRadius = a.radius + b.radius;

	float Distance = glm::distance
	(
		glm::vec3(a.position.x, a.position.y, a.position.z),
		glm::vec3(b.position.x, b.position.y, b.position.z)
	);

	if (Distance < totalRadius)
		return true;

	return false;
}

glm::vec3 Renderer::closestPointOnTriangle(const glm::vec3& point, const Triangle& triangle)
{
	// Get the triangle vertices
	glm::vec3 v0 = triangle.v0;
	glm::vec3 v1 = triangle.v1;
	glm::vec3 v2 = triangle.v2;

	// Compute the edges of the triangle
	glm::vec3 e0 = v1 - v0;
	glm::vec3 e1 = v2 - v0;
	glm::vec3 e2 = point - v0;

	// Compute the dot products
	float a = glm::dot(e0, e0);
	float b = glm::dot(e0, e1);
	float c = glm::dot(e1, e1);
	float d = glm::dot(e0, e2);
	float e = glm::dot(e1, e2);

	// Calculate barycentric coordinates
	float denominator = a * c - b * b;
	float s = (b * e - c * d) / denominator;
	float t = (a * e - b * d) / denominator;

	// Clamp the coordinates to the triangle
	s = glm::clamp(s, 0.0f, 1.0f);
	t = glm::clamp(t, 0.0f, 1.0f);

	// If the point is outside the triangle, adjust s and t to the closest edge
	if (s + t > 1.0f) {
		float temp = s + t;
		s /= temp;
		t /= temp;
	}

	// Calculate the closest point on the triangle
	glm::vec3 closestPoint = v0 + s * e0 + t * e1;
	return closestPoint;
}

bool Renderer::sphereTriangleCollision(const glm::vec3& sphereCenter, float sphereRadius, const Triangle& triangle)
{
	// Calculate the closest point on the triangle to the sphere's center
	glm::vec3 closestPoint = closestPointOnTriangle(sphereCenter, triangle);

	//  Calculate the distance from the sphere's center to the closest point
	glm::vec3 displacement = closestPoint - sphereCenter;
	float distanceSquared = glm::dot(displacement, displacement);
	float radiusSquared = sphereRadius * sphereRadius;

	// Check for collision
	return distanceSquared < radiusSquared; // Collision occurs if the distance squared is less than the radius squared
}

void Renderer::handleTriangleCollision(PhysicsSphere& sphere, const Triangle& triangle)
{
	glm::vec3 closestPoint = closestPointOnTriangle(sphere.position, triangle);
	glm::vec3 displacement = sphere.position - closestPoint; // now points **out of triangle**
	float distance = glm::length(displacement);

	if (distance < sphere.radius && distance > 0.0001f) // avoid divide by zero
	{
		glm::vec3 normal = glm::normalize(displacement);

		// Position correction: move sphere out of triangle
		float overlap = sphere.radius - distance;
		sphere.position += normal * overlap;

		// Relative velocity along normal
		float velAlongNormal = glm::dot(sphere.velocity, normal);

		// Only apply impulse if moving into triangle
		if (velAlongNormal < 0.0f)
		{
			float e = sphere.restitution;
			float j = -(1.0f + e) * velAlongNormal;

			// Cap impulse to prevent huge spikes
			float maxImpulse = 50.0f; // tweak as needed
			j = glm::clamp(j, -maxImpulse, maxImpulse);

			sphere.velocity += (j / sphere.mass) * normal;
		}

		// Optional: stop tiny bouncing to prevent jitter
		if (glm::abs(sphere.velocity.y) < 0.01f)
			sphere.velocity.y = 0.0f;
	}
}

void Renderer::UpdatePhysics(float deltaTime)
{
	// Gather all terrain triangles
	std::vector<Triangle> allTriangles;
	for (const MyMesh& terrain : t_meshes)
		allTriangles.insert(allTriangles.end(), terrain.triangles.begin(), terrain.triangles.end());

	for (size_t i = 0; i < c_physicsSpheres.size(); ++i)
	{
		auto& sphereA = c_physicsSpheres[i];

		// Apply gravity as a force: F = m * g
		sphereA.ApplyForce(gravity * sphereA.mass);

		// Optional friction / drag (tangential damping)
		if (sphereA.velocity.y < 1.0f)
		{
			glm::vec3 horizontalVel = glm::vec3(sphereA.velocity.x, 0.0f, sphereA.velocity.z);
			glm::vec3 frictionForce = -horizontalVel * 0.05f;
			sphereA.ApplyForce(frictionForce);
		}

		// Integrate motion
		sphereA.Update(deltaTime);

		// Terrain collision
		for (const Triangle& triangle : allTriangles)
		{
			if (sphereTriangleCollision(sphereA.position, sphereA.radius, triangle))
				handleTriangleCollision(sphereA, triangle);
		}

		// Sphere-to-sphere collisions
		for (size_t j = i + 1; j < c_physicsSpheres.size(); ++j)
		{
			auto& sphereB = c_physicsSpheres[j];
			if (TestSphereToSphereCollision(sphereA, sphereB))
				handleCollision(sphereA, sphereB); // now mass-aware
		}
	}
}


// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{
	//Configure pipeline settings
	glEnable(GL_DEPTH_TEST);

	if (m_cullFace)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, .1f, 40000.0f);

	// Compute camera view matrix and combine with projection matrix for passing to shader

	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 view_xform2 = glm::mat4(glm::mat3(view_xform));


	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);

	glm::mat4 model_xform(1);
	GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");
	glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

	// Clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Set up projection and view matrices
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);

	// Disable lighting for skybox
	glUniform1i(glGetUniformLocation(m_program, "useLighting"), 0);

	// Enable for skybox
	glUniform1i(glGetUniformLocation(m_program, "useTexture"), 1);


	//disable depth masking and testing
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	//skybox
	for (MyMesh& mesh : m_sky)
	{

		glm::mat4 combined_xform = glm::mat4(projection_xform * view_xform2);

		GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform");
		glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.tex);

		// Bind our VAO and render
		glBindVertexArray(mesh.m_VAO);
		glDrawElements(GL_TRIANGLES, mesh.m_numElements, GL_UNSIGNED_INT, (void*)0);
	}

	//enable depth masking and testing
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	// Enable texture for meshes
	glUniform1i(glGetUniformLocation(m_program, "useTexture"), 1);

	// Enable lighting for meshes
	glUniform1i(glGetUniformLocation(m_program, "useLighting"), 1);

	//render terrain
	for (size_t i = 0; i < t_meshes.size(); i++)
	{
		// Start with identity matrix
		glm::mat4 model_xform = glm::mat4(1.0f);

		// Apply translation first, moving to the sphere's position
		model_xform = glm::translate(model_xform, t_meshes[i].position);

		// Apply rotation from the quaternion;
		model_xform *= glm::mat4_cast(t_meshes[i].rotation);
		glm::mat4 combined_xform = projection_xform * view_xform;

		GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform");
		glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_meshes[i].tex);

		// Bind our VAO and render
		glBindVertexArray(t_meshes[i].m_VAO);
		glDrawElements(GL_TRIANGLES, t_meshes[i].t_numElements, GL_UNSIGNED_INT, (void*)0);
	}

	// Render each physics sphere
	for (size_t i = 0; i < c_physicsSpheres.size(); ++i)
	{
		// Start with identity matrix
		glm::mat4 model_xform = glm::mat4(1.0f);

		// Apply translation first, moving to the sphere's position
		model_xform = glm::translate(model_xform, c_physicsSpheres[i].position);

		// Apply scaling to adjust for the sphere's radius
		model_xform = glm::scale(model_xform, glm::vec3(c_physicsSpheres[i].radius/110));
		
		glm::mat4 combined_xform = projection_xform * view_xform;

		GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");
		glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

		GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform");
		glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));
		
		//texture 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_meshes[i].tex);

		// Bind and draw the corresponding VAO
		glBindVertexArray(s_meshes[i].s_VAO);
		glDrawElements(GL_TRIANGLES, s_meshes[i].s_numElements, GL_UNSIGNED_INT, (void*)0);
	}
}