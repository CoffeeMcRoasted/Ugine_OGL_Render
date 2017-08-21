#ifndef UGINE_BONE_H
#define UGINE_BONE_H

#include "../include/types.h"


class Bone {
public:
	static std::shared_ptr<Bone> Create(const std::string& name,
		int parentIndex);
	const std::string& GetName() const;
	int GetParentIndex() const;
	void SetInversePoseMatrix(const glm::mat4& matrix);
	const glm::mat4& GetInversePoseMatrix() const;
	glm::mat4 CalculateAnimMatrix(float frame) const;
	void AddPosition(uint32 frame, const glm::vec3& position);
	void AddRotation(uint32 frame, const glm::quat& rotation);
	void AddScale(uint32 frame, const glm::vec3& scale);
protected:
	Bone(const std::string& name, uint32 parentIndex);
	~Bone() {}
	static void Delete(Bone* b);
	glm::vec3 GetPosition(float frame) const;
	glm::quat GetRotation(float frame) const;
	glm::vec3 GetScale(float frame) const;
private:
	std::string mName;
	int mParentIndex;
	glm::mat4 mInvPoseMatrix;
	std::vector<std::pair<uint32, glm::vec3>> mPositions;
	std::vector<std::pair<uint32, glm::quat>> mRotations;
	std::vector<std::pair<uint32, glm::vec3>> mScales;
};

#endif//UGINE_BONE_H