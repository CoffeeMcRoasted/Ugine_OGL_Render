#ifndef UGINE_SCENE_H
#define UGINE_SCENE_H

#include "../include/types.h"
#include "../include/entity.h"
#include "../include/light.h"
#include "../include/model.h"
#include "../include/camera.h"

class Scene {
public:
	static const std::shared_ptr<Scene>& Instance();
	const std::shared_ptr<Camera>& GetCurrentCamera();
	const glm::mat4& GetModel() const;
	void SetModel(const glm::mat4& m);
	void AddEntity(const std::shared_ptr<Entity>& entity);
	void RemoveEntity(const std::shared_ptr<Entity>& entity);
	uint32 GetNumEntities() const;
	const std::shared_ptr<Entity>& GetEntity(uint32 index);
	void Update(float elapsed);
	void Render();
	void SetAmbient(const glm::vec3& ambient);
protected:
	Scene();
	~Scene() {}
	static void Delete(Scene* s) { delete s; }
private:
	static std::shared_ptr<Scene> mInstance;
	std::shared_ptr<Camera> mCurrentCamera;
	glm::mat4 mModelMatrix;
	std::vector<std::shared_ptr<Camera>> mCameras;
	std::vector<std::shared_ptr<Entity>> mEntities;
	std::vector<std::shared_ptr<Light>> mLights;
};

inline const std::shared_ptr<Scene>& Scene::Instance() {
	if (!mInstance)
		mInstance = std::shared_ptr<Scene>(new Scene(), Delete);
	return mInstance;
}

#endif//UGINE_SCENE_H
