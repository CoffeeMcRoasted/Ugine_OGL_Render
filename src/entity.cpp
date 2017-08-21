#include "../include/entity.h"
#include "../include/scene.h"

std::shared_ptr<Entity> Entity::Create()
{
	return std::shared_ptr<Entity>(new Entity(), Delete);
}

const glm::vec3 & Entity::GetPosition() const
{
	return mPosition;
}

glm::vec3 & Entity::GetPosition()
{
	return mPosition;
}

const glm::quat & Entity::GetRotation() const
{
	return mRotation;
}

glm::quat & Entity::GetRotation()
{
	return mRotation;
}

const glm::vec3 & Entity::GetScale() const
{
	return mScale;
}

glm::vec3 & Entity::GetScale()
{
	return mScale;
}

void Entity::Move(const glm::vec3 & speed)
{
	mPosition += mRotation * speed;
}

void Entity::Render()
{
	glm::mat4 model = glm::scale<float>(glm::rotate<float>(glm::translate<float>(
		glm::mat4(), mPosition), glm::angle(mRotation), glm::axis(mRotation)), mScale);
	Scene::Instance()->SetModel(model);
}

Entity::Entity() : mScale(1.0f, 1.0f, 1.0f)
{
}
