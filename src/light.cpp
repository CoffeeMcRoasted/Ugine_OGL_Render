#include "../include/light.h"
#include "../include/scene.h"

//Initialization in order to avoid Linkeage errors
bool Light::mLightsUsed[] = { false };

std::shared_ptr<Light> Light::Create()
{
	return std::shared_ptr<Light>(new Light(), Light::Delete);
}

Light::Type Light::GetType() const
{
	return mType;
}

void Light::SetType(Type type)
{
	mType = type;
}

const glm::vec3 & Light::GetColor() const
{
	return mColor;
}

void Light::SetColor(const glm::vec3 & color)
{
	mColor = color;
}

float Light::GetAttenuation() const
{
	return mAttenuation;
}

void Light::SetAttenuation(float att)
{
	mAttenuation = att;
}

void Light::Prepare()
{
	//TODO: Ask if we do need to disable
	Renderer::Instance()->EnableLight(mIndex, true);
	//We set the light using observer pace instead of scene space
	glm::vec4 tmpPos = Scene::Instance()->GetCurrentCamera()->GetView() * glm::vec4(GetPosition(),
		mType == POINT ? 1.0f : 0.0f);
	//if (mType == POINT)
	//{
	//	tmpPos.w = 1.0f;
	//}
	//else if (mType == DIRECTIONAL)
	//{
	//	tmpPos.w = 0.0f;
	//}
	Renderer::Instance()->Renderer::SetLightData(mIndex, tmpPos,mColor, mAttenuation);
}

Light::Light()
{
	int i = 0;
	while (mLightsUsed[i])
		++i;
	mIndex = i;
	mLightsUsed[i] = true;
}


Light::~Light()
{
	mLightsUsed[mIndex] = false;
}
