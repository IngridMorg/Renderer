#include "model.h"

void Model::draw(Shader& shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}


void Model::loadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR:: Assimp LoadModel() from Model.h and Model.cpp has created an error when loading the model" << import.GetErrorString() << std::endl;
	}
	directory = path.substr(0, path.find_last_of('/'));
	//process node is a recursive function, hence why we only need to pass the root node

	processNode(scene->mRootNode, scene);
}
//this recursive process is really important as it allows us to maintain parent child relationships, that are
//incredibly important when applying any kind of transformation to a mesh
void Model::processNode(aiNode* node, const aiScene* scene) {
	//given the node mesh had nodes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	//now repeat this process for all child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//for every vertex in this mesh, we loop over and process all the relevant information regarding the vertex
	//REMEMBER:: The mesh variable we are dealing with here is an aimesh object, not an object from te mesh class we created
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;
		//set up vertex positions
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Position = vector;

		//process the normals
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.x = mesh->mNormals[i].x;
			vector.x = mesh->mNormals[i].x;
			vertex.Normal = vector;
		}
		//texture coordinates
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			//currently this is very simple, so we are assuming that the first one is the array is all we need
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			//tangent and bitangent values
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			//bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	//indice stuff that defines how we interact with faces etc
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	//now we need to fiddle with some materials
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	}
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		//to acoid repeatedly loading the same vector over and over and over again
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		Texture texture;
		texture.texID = TextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}