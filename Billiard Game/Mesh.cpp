#include "Mesh.hpp"

GLuint Mesh::aPositionLoc = 0;
GLuint Mesh::aTex_CoordLoc = 1;
GLuint Mesh::aNormalLoc = 2;

Mesh::Mesh(){
	clear(CLEAR_MESH | CLEAR_MATERIAL | CLEAR_TEXTURE);
}

Mesh::~Mesh(){
	clear(CLEAR_MESH | CLEAR_MATERIAL | CLEAR_TEXTURE);
}

void Mesh::setMesh(const aiMesh* mesh){
	clear(CLEAR_MESH);
	GLuint buffer;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int *faceArray;
	this->numIndices = mesh->mNumFaces * 3;
	faceArray = new unsigned int[this->numIndices];
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace* face = &mesh->mFaces[i];
		assert(face->mNumIndices == 3); //If mNumIndices != 3, program rase error
		memcpy(&faceArray[3 * i], face->mIndices, 3 * sizeof(unsigned int));
	}
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

	// Buffer for vertex positions
	if (mesh->HasPositions()) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D)* mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(aPositionLoc);
		glVertexAttribPointer(aPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Buffer for vertex normals
	if (mesh->HasNormals()) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D)* mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(aNormalLoc);
		glVertexAttribPointer(aNormalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Buffer for vertex texture coordinates
	// Now it only support single texture.
	if (mesh->HasTextureCoords(0)) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D)* mesh->mNumVertices, mesh->mTextureCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(aTex_CoordLoc);
		glVertexAttribPointer(aTex_CoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(aiVector3D), 0);
	}

	// Unbind buffers
	glBindVertexArray(NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void Mesh::setMaterial(const Material* material){
	this->material = material;
}

void Mesh::setTexture(const Texture* texture){
	this->texture = texture;
}

void Mesh::clear(GLenum option){
	if ((option & CLEAR_MESH) != 0x0){
		glDeleteVertexArrays(1, &VAO);
		numIndices = 0;
	}
	if ((option & CLEAR_MATERIAL) != 0x0){
		material = NULL;
	}
	if ((option & CLEAR_TEXTURE) != 0x0){
		texture = NULL;
	}
}

GLuint Mesh::getVAO() const{
	return this->VAO;
}

GLuint Mesh::getNumIndices() const{
	return this->numIndices;
}

const Material* Mesh::getMaterial() const{
	return this->material;
}

const Texture* Mesh::getTexture() const{
	return this->texture;
}