#include "../include/material.h"

Material::Material(const std::shared_ptr<Texture>& baseTexture)
	: mBaseTexture(baseTexture)
{
}

const glm::vec4 & Material::GetBaseColor() const
{
	return mBaseColor;
}

void Material::SetBaseColor(const glm::vec4 & color)
{
	mBaseColor = color;
}

const std::shared_ptr<Texture>& Material::GetBaseTexture()
{
	return mBaseTexture;
}

void Material::SetBaseTexture(const std::shared_ptr<Texture>& tex)
{
	mBaseTexture = tex;
}

const std::shared_ptr<Texture>& Material::GetReflectionTexture()
{
	return mReflectionTexture;
}

void Material::SetReflectionTexture(const std::shared_ptr<Texture>& tex)
{
	mReflectionTexture = tex;
}

const std::shared_ptr<Texture>& Material::GetRefractionTexture()
{
	return mRefractionTexture;
}

void Material::SetRefractionTexture(const std::shared_ptr<Texture>& tex)
{
	mRefractionTexture = tex;
}

const std::shared_ptr<Texture>& Material::GetNormalTexture()
{
	return mNormalTexture;
}

void Material::SetNormalTexture(const std::shared_ptr<Texture>& tex)
{
	mNormalTexture = tex;
}

float Material::GetRefractionCoef() const
{
	return mRefractCoef;
}

void Material::SetRefractionCoef(float coef)
{
	mRefractCoef = coef;
}

Renderer::BlendMode Material::GetBlendMode() const
{
	return mBlendMode;
}

void Material::SetBlendMode(Renderer::BlendMode mode)
{
	mBlendMode = mode;
}

bool Material::GetCulling() const
{
	return mCulling;
}

void Material::SetCulling(bool enable)
{
	mCulling = enable;
}

bool Material::GetDepthWrite() const
{
	return mDepthWrite;
}

void Material::SetDepthWrite(bool enable)
{
	mDepthWrite = enable;
}

uint8 Material::GetShininess() const
{
	return mShininess;
}

void Material::SetShininess(uint8 shininess)
{
	mShininess = shininess;
}

void Material::Prepare() const
{
	Renderer::Instance()->SetDiffuse(mBaseColor);
	//TODO::estableceremos en el Renderer el color difuso,el modo de mezclado,
	//activaremos o desactivaremos el culling y escritura en el depth buffer, indicaremos la intensidad
	//especular y el coeficiente de refracción, y enlazaremos todas las capas de textura(si no hay
	//textura definida en alguna de las capas, se enlaza el índice de textura 0).
	Renderer::Instance()->SetBlendMode(mBlendMode);
	Renderer::Instance()->SetShininess(mShininess);
	Renderer::Instance()->SetRefractCoef(mRefractCoef);
	
	uint32 baseTex = mBaseTexture ? mBaseTexture->GetHandle() : 0;
	uint32 reflTex = mReflectionTexture ? mReflectionTexture->GetHandle() : 0;
	uint32 refrTex = mRefractionTexture ? mRefractionTexture->GetHandle() : 0;
	uint32 normalTex = mNormalTexture ? mNormalTexture->GetHandle() : 0;
	bool isCube = mBaseTexture ? mBaseTexture->IsCubeMap() : false;
	Renderer::Instance()->SetTextures(baseTex, reflTex, refrTex, normalTex, isCube);
}
