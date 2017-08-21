#ifndef UGINE_MATERIAL_H
#define UGINE_MATERIAL_H

#include "types.h"
#include "texture.h"

class Material {
public:
	explicit Material(const std::shared_ptr<Texture>& baseTexture =
		nullptr);
	const glm::vec4& GetBaseColor() const;
	void SetBaseColor(const glm::vec4& color);

	const std::shared_ptr<Texture>& GetBaseTexture();
	void SetBaseTexture(const std::shared_ptr<Texture>& tex);

	const std::shared_ptr<Texture>& GetReflectionTexture();
	void SetReflectionTexture(const std::shared_ptr<Texture>& tex);

	const std::shared_ptr<Texture>& GetRefractionTexture();
	void SetRefractionTexture(const std::shared_ptr<Texture>& tex);

	const std::shared_ptr<Texture>& GetNormalTexture();
	void SetNormalTexture(const std::shared_ptr<Texture>& tex);

	float GetRefractionCoef() const;
	void SetRefractionCoef(float coef);

	Renderer::BlendMode GetBlendMode() const;
	void SetBlendMode(Renderer::BlendMode mode);

	bool GetCulling() const;
	void SetCulling(bool enable);

	bool GetDepthWrite() const;
	void SetDepthWrite(bool enable);

	uint8 GetShininess() const;
	void SetShininess(uint8 shininess);

	void Prepare() const;
private:
	glm::vec4 mBaseColor;
	std::shared_ptr<Texture> mBaseTexture;
	std::shared_ptr<Texture> mReflectionTexture;
	std::shared_ptr<Texture> mRefractionTexture;
	std::shared_ptr<Texture> mNormalTexture;
	Renderer::BlendMode mBlendMode;
	bool mCulling;
	bool mDepthWrite;
	uint8 mShininess;
	float mRefractCoef;
};

#endif//UGINE_MATERIAL_H