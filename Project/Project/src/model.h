#pragma once

#include <glad.h>


#include <vector>
#include <string>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shaders/shader.h"
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
//class that contains an entire model/scene
class Model{
private:
	//data for the model
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	Model(char* path) {
		loadModel(path);
	}

	void draw(Shader& shader);
};

