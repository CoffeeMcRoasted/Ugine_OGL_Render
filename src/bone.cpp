#include "../include/bone.h"

std::shared_ptr<Bone> Bone::Create(const std::string & name, int parentIndex)
{
	return std::shared_ptr<Bone>(new Bone(name, parentIndex), Delete);
}

const std::string & Bone::GetName() const
{
	return mName;
}

int Bone::GetParentIndex() const
{
	return mParentIndex;
}

void Bone::SetInversePoseMatrix(const glm::mat4 & matrix)
{
	mInvPoseMatrix = matrix;
}

const glm::mat4 & Bone::GetInversePoseMatrix() const
{
	return mInvPoseMatrix;
}

glm::mat4 Bone::CalculateAnimMatrix(float frame) const
{
	glm::quat q = GetRotation(frame);
	float bangle = glm::angle(q);
	glm::vec3 baxis = glm::axis(q);
	glm::vec3 bposition = GetPosition(frame);
	glm::vec3 bscale = GetScale(frame);

	glm::mat4 scale = glm::scale<float>(glm::mat4(1), bscale);
	glm::mat4 rotation = glm::rotate<float>(glm::mat4(1), bangle, baxis);
	glm::mat4 translation = glm::translate<float>(glm::mat4(1), bposition);

	return translation * rotation * scale;
}

void Bone::AddPosition(uint32 frame, const glm::vec3 & position)
{
	mPositions.push_back(std::pair<uint32, glm::vec3>(frame, position));
}

void Bone::AddRotation(uint32 frame, const glm::quat & rotation)
{
	mRotations.push_back(std::pair<uint32, glm::quat>(frame, rotation));
}

void Bone::AddScale(uint32 frame, const glm::vec3 & scale)
{
	mScales.push_back(std::pair<uint32, glm::vec3>(frame, scale));
}

Bone::Bone(const std::string & name, uint32 parentIndex): mName(name), mParentIndex(parentIndex)
{
}

void Bone::Delete(Bone * b)
{
	delete b;
}

glm::vec3 Bone::GetPosition(float frame) const
{
	if (!mPositions.empty()) 
	{
		for (auto it = mPositions.begin(); it != mPositions.end(); ++it)
		{
			if (it->first == frame) return it->second;
			else if (it->first > frame)
			{
				float a = (frame - static_cast<float>(it[-1].first)) / static_cast<float>(it->first - it[-1].first);
				return glm::mix(it[-1].second, it->second, a);
			}
		}
		return mPositions.back().second;
	}
	else
	{
		return glm::vec3();
	}
}

glm::quat Bone::GetRotation(float frame) const
{
	if (!mRotations.empty())
	{
		for (auto it = mRotations.begin(); it != mRotations.end(); ++it)
		{
			//TODO: Change order for lazy checking?
			if (it->first == frame) return it->second;
			else if (it->first > frame)
			{
				float a = (frame - static_cast<float>(it[-1].first)) / static_cast<float>(it->first - it[-1].first);
				return glm::slerp(it[-1].second, it->second, a);
			}
		}
		return mRotations.back().second;
	}
	else
	{
		return glm::quat();
	}
}

glm::vec3 Bone::GetScale(float frame) const
{
	if (!mScales.empty())
	{
		for (auto it = mScales.begin(); it != mScales.end(); ++it)
		{
			if (it->first == frame) return it->second;
			else if (it->first > frame)
			{
				float a = (frame - static_cast<float>(it[-1].first)) / static_cast<float>(it->first - it[-1].first);
				return glm::mix(it[-1].second, it->second, a);
			}
		}
		return mScales.back().second;
	}
	else
	{
		return glm::vec3(1.0f);
	}
}
