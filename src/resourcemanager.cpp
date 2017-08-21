#include "../include/resourcemanager.h"

std::shared_ptr<ResourceManager> ResourceManager::mInstance = nullptr;

std::shared_ptr<Mesh> ResourceManager::LoadMesh(const std::string & filename)
{
	if (mMeshes.count(filename) > 0)
	{
		return mMeshes[filename];
	}
	else
	{ 
		std::shared_ptr<Mesh> mesh = Mesh::Create(filename);
		if (mesh) mMeshes[filename] = mesh;
		return mesh;
	}
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string & filename)
{
	if (mTextures.count(filename) > 0)
	{
		return mTextures.find(filename)->second;
	}
	else
	{
		std::shared_ptr<Texture> texture = Texture::Create(filename);
		if (texture) mTextures[filename] = texture;
		return texture;
	}
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string & left, const std::string & right, const std::string & front, const std::string & back, const std::string & top, const std::string & bottom)
{
	if (mTextures.count(left) > 0)
	{
		return mTextures.find(left)->second;
	}
	else
	{
		std::shared_ptr<Texture> cube = Texture::Create(left, right, front, back, top, bottom);
		if (cube) mTextures[left] = cube;
		return cube;
	}
}

void ResourceManager::FreeMeshes()
{
	mMeshes.clear();
}

void ResourceManager::FreeTextures()
{
	mTextures.clear();
}

void ResourceManager::FreeResources()
{
	FreeMeshes();
	FreeTextures();
}
