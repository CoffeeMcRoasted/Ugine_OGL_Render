#ifndef UGINE_SUBMESH_H
#define UGINE_SUBMESH_H

#include "../include/types.h"
#include "../include/vertex.h"
#include "../include/material.h"

class Submesh {
public:
	static std::shared_ptr<Submesh> Create(
		const Material& mat);
	void AddVertex(const Vertex& v);
	void AddTriangle(uint16 v0, uint16 v1, uint16 v2);
	//const std::shared_ptr<Texture>& GetTexture() const;
	//void SetTexture(const std::shared_ptr<Texture>& tex);
	Material& getMaterial();
	const std::vector<Vertex>& GetVertices() const;
	std::vector<Vertex>& GetVertices();
	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);
	uint8 GetShininess() const;
	void SetShininess(uint8 shininess);
	void Rebuild();
	void Render();
protected:
	Submesh(const Material& mat);
	~Submesh();
	static void Delete(Submesh* s) { delete s; }
private:
	//std::shared_ptr<Texture> mTexture;
	uint32 mVertexBuffer;
	uint32 mIndexBuffer;
	//glm::vec3 mDiffuse;
	//uint8 mShininess;
	std::vector<Vertex> mVertices;
	std::vector<uint16> mIndices;
	Material mMaterial;
};

#endif//UGINE_SUBMESH_H