#ifndef UGINE_MODEL_H
#define UGINE_MODEL_H

#include "../include/types.h"
#include "../include/mesh.h"
#include "../include/entity.h"

class Model : public Entity {
public:
	static std::shared_ptr<Model> Create(const std::shared_ptr<Mesh>&
		mesh = nullptr);
	virtual void Update(float elapsed);
	virtual void Render();
	//Animation
	void Animate(bool animate) {
		if (mAnimMatrices.size() > 0) mAnimating = animate;
	}
	int GetFPS() const { return mFPS; }
	void SetFPS(int fps) { mFPS = fps; }
	float GetCurrentFrame() const { return mCurrentFrame; }
	void SetCurrentFrame(float frame) { mCurrentFrame = frame; }

protected:
	Model(const std::shared_ptr<Mesh>& mesh);
	virtual ~Model() {}
private:
	std::shared_ptr<Mesh> mMesh;
	//Animation Properties
	bool mAnimating;
	int mFPS;
	float mCurrentFrame;
	std::vector<glm::mat4> mAnimMatrices;
};
#endif//UGINE_MODEL_H

