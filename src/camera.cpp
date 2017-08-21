#include "../include/camera.h"
#include "../include/screen.h"

std::shared_ptr<Camera> Camera::Create()
{
	return std::shared_ptr<Camera>(new Camera(), Delete);
}

int32 Camera::GetViewportX() const
{
	return mVX;
}

int32 Camera::GetViewportY() const
{
	return mVY;
}

uint16 Camera::GetViewportWidth() const
{
	return mVW;
}

uint16 Camera::GetViewportHeight() const
{
	return mVH;
}

void Camera::SetViewport(int32 x, int32 y, uint16 w, uint16 h)
{
	mVX = x;
	mVY = y;
	mVH = h;
	mVW = w;
}

void Camera::SetProjection(const glm::mat4 & proj)
{
	mProjMatrix = proj;
}

const glm::mat4 & Camera::GetProjection() const
{
	return mProjMatrix;
}

const glm::mat4 & Camera::GetView() const
{
	return mViewMatrix;
}

const glm::vec3 & Camera::GetColor() const
{
	return mColor;
}

void Camera::SetColor(const glm::vec3 & color)
{
	mColor = color;
}

bool Camera::GetUsesTarget() const
{
	return mUsesTarget;
}

void Camera::SetUsesTarget(bool usesTarget)
{
	mUsesTarget = usesTarget;
}

const glm::vec3 & Camera::GetTarget() const
{
	return mTarget;
}

glm::vec3 & Camera::GetTarget()
{
	return mTarget;
}

void Camera::Prepare()
{
	Renderer::Instance()->SetViewport(mVX, mVY, mVW, mVH);

	if (mUsesTarget)
	{
		mViewMatrix = glm::lookAt(GetPosition(), mTarget, glm::vec3(0, 1, 0) /** GetRotation()*/);
	}
	else
	{
		glm::mat4 view;
		view = glm::rotate<float>(view, -glm::angle(GetRotation()), glm::axis(GetRotation()));
		view = glm::translate<float>(view, -GetPosition());
		mViewMatrix = view;

    //Other option
    //glm::vec3 target = GetPosition() + GetRotation() * glm::vec3(0, 0, -1);
    //mViewMatrix = glm::lookAt(GetPosition(), target, glm::vec3(0, 1, 0));
    //mViewMatrix = glm::translate(glm::rotate(glm::mat4(), -glm::angle(GetRotation()), glm::axis(GetRotation())), -GetPosition());
	}
	// Color and Depth buffer are cleaned
	Renderer::Instance()->SetEyePos(GetPosition());
	Renderer::Instance()->ClearColorBuffer(mColor);
	Renderer::Instance()->ClearDepthBuffer();
}

Camera::Camera()
{
  mVX = 0;
  mVY = 0;
  mVW = Screen::Instance()->GetWidth();
  mVH = Screen::Instance()->GetHeight();
  mColor = glm::vec3(0.2f, 0.6f, 1.0f);
  mUsesTarget = false;
  mTarget = glm::vec3(0, 0, 0);
}
