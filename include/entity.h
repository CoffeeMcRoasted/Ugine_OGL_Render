#ifndef UGINE_ENTITY_H
#define UGINE_ENTITY_H

//#include "scene.h"
#include "../include/types.h"

//class Scene;

class Entity{
public:
	static std::shared_ptr<Entity> Create();
	const glm::vec3& GetPosition() const;
	glm::vec3& GetPosition();
	const glm::quat& GetRotation() const;
	glm::quat& GetRotation();
	const glm::vec3& GetScale() const;
	glm::vec3& GetScale();
	void Move(const glm::vec3& speed);
	virtual void Update(float elapsed) {}
	virtual void Render();
protected:
	Entity();
	virtual ~Entity() {}
	static void Delete(Entity* e) { delete e; }
private:
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;
};

#endif//UGINE_ENTITY_H

