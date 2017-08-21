#include "../include/model.h"

std::shared_ptr<Model> Model::Create(const std::shared_ptr<Mesh>& mesh)
{
	return std::shared_ptr<Model>(new Model(mesh), Delete);
}

void Model::Render()
{
	Entity::Render();
	Renderer::Instance()->SetSkinned(mAnimating);
	//mMesh.get()->Render();
	if (mAnimating) Renderer::Instance()->SetAnimMatrices(mAnimMatrices);
	mMesh->Render();
}

Model::Model(const std::shared_ptr<Mesh>& mesh)
	:	mFPS(60),
		mCurrentFrame(0),
		mAnimating(false)
{
	mMesh = mesh;
	mAnimMatrices.resize(mesh->GetBones().size());
}

void Model::Update(float elapsed)
{
	mCurrentFrame += static_cast<float>(mFPS) * elapsed;
	if (mAnimating)
	{
		if (mCurrentFrame >= mMesh->GetLastFrame())
		{
			mCurrentFrame -= mMesh->GetLastFrame();
		}
		else if (mCurrentFrame <= 0)
		{
			mCurrentFrame += mMesh->GetLastFrame();
		}

		mMesh->CalculateAnimMatrices(mCurrentFrame, mAnimMatrices);
	}
}