#ifndef UGINE_VERTEX_H
#define UGINE_VERTEX_H

#include "types.h"

struct Vertex {
	glm::vec3 mPosition;
	glm::vec2 mTexture;
	glm::vec3 mNormal;
	glm::vec3 mTangent;
	glm::vec4 mBoneIndices;
	glm::vec4 mBoneWeights;
	Vertex(const glm::vec3& position,const glm::vec2& texture,
		const glm::vec3& normal, const glm::vec3& tangent, 
		const glm::vec4 boneIndices, const glm::vec4 boneWeights) 
		:	mPosition(position),
			mTexture(texture),
			mNormal(normal),
			mTangent(tangent),
			mBoneIndices(boneIndices),
			mBoneWeights(boneWeights)
	{}
};

#endif // UGINE_VERTEX_H
