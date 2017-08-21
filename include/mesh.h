#ifndef UGINE_MESH_H
#define UGINE_MESH_H

#include <iostream>

#include "../include/types.h"
#include "../include/submesh.h"
#include "../include/bone.h"
//#include "../include/resourcemanager.h"
#include "../lib/pugixml/pugixml.hpp"
#include "../lib/pugixml/pugiconfig.hpp"
//class ResourceManager;

class Mesh {
public:
	static std::shared_ptr<Mesh> Create();
	static std::shared_ptr<Mesh> Create(const std::string& filename);
	const std::string& GetFilename() const;
	void AddSubmesh(const std::shared_ptr<Submesh>& submesh);
	void RemoveSubmesh(uint32 i);
	void RemoveSubmesh(const std::shared_ptr<Submesh>& submesh);
	uint32 NumSubmeshes() const;
	const std::shared_ptr<Submesh>& GetSubmesh(uint32 i);
	void Render();
	//Animation
	void AddBone(const std::shared_ptr<Bone>& bone) { mBones.push_back(bone); }
	const std::vector<std::shared_ptr<Bone>>& GetBones() const { return mBones; }
	uint32 GetLastFrame() const { return mLastFrame; }
	void SetLastFrame(uint32 lastFrame) { mLastFrame = lastFrame; }
	void CalculateAnimMatrices(float frame,
		std::vector<glm::mat4>& animMatrices);
	int GetBoneIndex(const std::string& name);
protected:
	Mesh(const std::string& filename);
	~Mesh() {}
	static void Delete(Mesh* m) { delete m; }
private:
	std::string mFilename;
	std::vector<std::shared_ptr<Submesh>> mSubmeshes;
	//Animation properties
	std::vector<std::shared_ptr<Bone>> mBones;
	uint32 mLastFrame;
};

#endif//UGINE_MESH_H

