#ifndef UGINE_LIGHT_H
#define UGINE_LIGHT_H

#include "../include/entity.h"
#include "../include/renderer.h"


class Light : public Entity{
public:
	enum Type {
		DIRECTIONAL,
		POINT
	};
	static std::shared_ptr<Light> Create();
	Type GetType() const;
	void SetType(Type type);
	const glm::vec3& GetColor() const;
	void SetColor(const glm::vec3& color);
	float GetAttenuation() const;
	void SetAttenuation(float att);
	void Prepare();
	virtual void Render() {}
protected:
	Light();
	virtual ~Light();
private:
	static bool mLightsUsed[MAX_LIGHTS];
	int mIndex;
	Type mType;
	glm::vec3 mColor;
	float mAttenuation;
};

#endif//UGINE_LIGHT_H