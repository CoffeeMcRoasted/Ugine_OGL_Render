#include "..\include\submesh.h"

std::shared_ptr<Submesh> Submesh::Create(const Material& mat)
{
	return std::shared_ptr<Submesh>(new Submesh(mat), Submesh::Delete);
}

void Submesh::AddVertex(const Vertex & v)
{
	mVertices.push_back(v);
}

void Submesh::AddTriangle(uint16 v0, uint16 v1, uint16 v2)
{
	mIndices.push_back(v0);
	mIndices.push_back(v1);
	mIndices.push_back(v2);
}

//const std::shared_ptr<Texture>& Submesh::GetTexture() const
//{
//	return mTexture;
//}
//
//void Submesh::SetTexture(const std::shared_ptr<Texture>& tex)
//{
//	mTexture = tex;
//}

Material & Submesh::getMaterial()
{
	return mMaterial;
}

const std::vector<Vertex>& Submesh::GetVertices() const
{
	return mVertices;
}

std::vector<Vertex>& Submesh::GetVertices()
{
	return mVertices;
}

const glm::vec4 & Submesh::GetColor() const
{
	return mMaterial.GetBaseColor();
}

void Submesh::SetColor(const glm::vec4 & color)
{
	mMaterial.SetBaseColor(color);
}

uint8 Submesh::GetShininess() const
{
	return mMaterial.GetShininess();
}

void Submesh::SetShininess(uint8 shininess)
{
	mMaterial.SetShininess(shininess);
}

void Submesh::Rebuild()
{
	Renderer::Instance()->SetVertexBufferData(mVertexBuffer, mVertices.data(), mVertices.size() * sizeof(Vertex));
	Renderer::Instance()->SetIndexBufferData(mIndexBuffer, mIndices.data(), mIndices.size() * sizeof(uint16));
}

void Submesh::Render()
{
	//Renderer::Instance()->SetDiffuse(mDiffuse);
	//Renderer::Instance()->SetShininess(mShininess);
	//Renderer::Instance()->SetTexture(mTexture ? mTexture->GetHandle() : 0);
	mMaterial.Prepare();
	Renderer::Instance()->DrawBuffers(mVertexBuffer, mIndexBuffer, mIndices.size());
}

Submesh::Submesh(const Material& mat)
{
	mVertexBuffer = Renderer::Instance()->CreateBuffer();
	mIndexBuffer = Renderer::Instance()->CreateBuffer();
	mMaterial = mat;
}

Submesh::~Submesh()
{
	Renderer::Instance()->FreeBuffer(mVertexBuffer);
	Renderer::Instance()->FreeBuffer(mIndexBuffer);
}
