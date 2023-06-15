#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shaders/shader.h"

//inspo: https://learnopengl.com/Model-Loading/Mesh

struct  Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	//tangent values etc
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};
struct Texture {
	unsigned int texID;
	std::string type;
	std::string path;
};
class Mesh
{
private:
	//funfact: structs are sequential in memory, which is useful for when working with buffer information
	
	//render data
	unsigned int VAO, VBO, EBO;
	void setupMesh();

public:
	//mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	//by passing a shader we can draw different meshes with different shaders!
	void draw(Shader& shader);
};

