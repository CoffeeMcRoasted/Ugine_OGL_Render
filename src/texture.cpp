#include "../include/texture.h"

std::shared_ptr<Texture> Texture::Create(const std::string& filename)
{
	std::shared_ptr<Texture> texPtr(new Texture(filename), Texture::Delete);
	if (texPtr->mHandle > 0) return texPtr;
	else return nullptr;
}

std::shared_ptr<Texture> Texture::Create(const std::string & left, const std::string & right, const std::string & front, const std::string & back, const std::string & top, const std::string & bottom)
{
	std::shared_ptr<Texture> cubePtr(new Texture(left,right,front,back,top,bottom), Texture::Delete);
	if (cubePtr->mHandle > 0) return cubePtr;
	else return nullptr;
}

Texture::Texture(const std::string & left, const std::string & right, const std::string & front, const std::string & back, const std::string & top, const std::string & bottom)
	: mFilename(left),
	mIsCubeMap(true),
	mHandle(Renderer::Instance()->LoadCubeTexture(left, right, front, back, top, bottom, mWidth, mHeight))
{
}

Texture::Texture(const std::string& filename): 
	mFilename(filename),
	mIsCubeMap(false),
	mHandle(Renderer::Instance()->LoadTexture(filename, mWidth, mHeight))
{}

Texture::~Texture()
{
	Renderer::Instance()->FreeTexture(mHandle);
}