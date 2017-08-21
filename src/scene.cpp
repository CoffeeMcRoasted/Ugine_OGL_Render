#include "..\include\scene.h"

std::shared_ptr<Scene> Scene::mInstance = nullptr;

const std::shared_ptr<Camera>& Scene::GetCurrentCamera()
{
	return mCurrentCamera;
}

const glm::mat4 & Scene::GetModel() const
{
	return mModelMatrix;
}

void Scene::SetModel(const glm::mat4 & m)
{
	mModelMatrix = m;
	Renderer::Instance()->SetMatrices(m, mCurrentCamera->GetView(), mCurrentCamera->GetProjection());
}

void Scene::AddEntity(const std::shared_ptr<Entity>& entity)
{
	mEntities.push_back(entity);
	if (std::shared_ptr<Camera> cam = std::dynamic_pointer_cast<Camera, Entity>(entity))
	{
		mCameras.push_back(cam);
	}
	if (std::shared_ptr<Light> light = std::dynamic_pointer_cast<Light, Entity>(entity))
	{
		mLights.push_back(light);
	}
}

void Scene::RemoveEntity(const std::shared_ptr<Entity>& entity)
{
	mEntities.erase(std::find(mEntities.begin(), mEntities.end(), entity));
	if (std::shared_ptr<Camera> cam = std::dynamic_pointer_cast<Camera, Entity>(entity))
	{
		mCameras.erase(std::find(mCameras.begin(), mCameras.end(), cam));
	}
	if (std::shared_ptr<Light> light = std::dynamic_pointer_cast<Light, Entity>(entity))
	{
		mLights.erase(std::find(mLights.begin(), mLights.end(),light));
	}
}

uint32 Scene::GetNumEntities() const
{
	return static_cast<uint32>(mEntities.size());
}

const std::shared_ptr<Entity>& Scene::GetEntity(uint32 index)
{
	if (index > GetNumEntities())
	{
		return nullptr;//RETURNS TEMPORARY
	}
	return mEntities[index];
}

void Scene::Update(float elapsed)
{
	for (auto it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		it->get()->Update(elapsed);
	}
}

void Scene::Render()
{
	for (auto it = mCameras.begin(); it != mCameras.end(); ++it)
	{
		mCurrentCamera = *it;
		mCurrentCamera->Prepare();
		if (mLights.size() > 0)
		{
			Renderer::Instance()->EnableLighting(true);
		}
		for (auto lightIt = mLights.begin(); lightIt != mLights.end(); ++lightIt)
		{
			lightIt->get()->Prepare();
		}
		for (auto enIt = mEntities.begin(); enIt != mEntities.end(); ++enIt)
		{
			//Renderer::Instance()->SetMatrices(mModelMatrix, mCurrentCamera.get()->GetView(), mCurrentCamera.get()->GetProjection());
			enIt->get()->Render();
		}
		//We disable the lights
		Renderer::Instance()->EnableLighting(false);
		for (int i = 0; i < MAX_LIGHTS; ++i)
		{
			Renderer::Instance()->EnableLight(i, false);
		}
	}
}

void Scene::SetAmbient(const glm::vec3 & ambient)
{
	Renderer::Instance()->SetAmbient(ambient);
}

Scene::Scene()
{
}
