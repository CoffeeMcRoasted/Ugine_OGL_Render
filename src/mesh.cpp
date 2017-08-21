#include "..\include\mesh.h"
#include "../include/resourcemanager.h"
#include "../include/material.h"

std::shared_ptr<Mesh> Mesh::Create()
{
	return std::shared_ptr<Mesh>(new Mesh(""), Mesh::Delete);
}

std::shared_ptr<Mesh> Mesh::Create(const std::string & filename)
{
	return std::shared_ptr<Mesh>(new Mesh(filename), Mesh::Delete);
}

const std::string & Mesh::GetFilename() const
{
	return mFilename;
}

void Mesh::AddSubmesh(const std::shared_ptr<Submesh>& submesh)
{
	mSubmeshes.push_back(submesh);
	submesh.get()->Rebuild();
}

void Mesh::RemoveSubmesh(uint32 i)
{
	mSubmeshes.erase(mSubmeshes.begin() + i);
}

void Mesh::RemoveSubmesh(const std::shared_ptr<Submesh>& submesh)
{
	// TODO: Check this
	//Careful with the behaviour of erasing the las element or no coincidences,
	// as find returns last if it doesnt find an element within.
	auto it = std::find(mSubmeshes.begin(), mSubmeshes.end(), submesh);
	mSubmeshes.erase(it);
}

uint32 Mesh::NumSubmeshes() const
{
	return mSubmeshes.size();
}

const std::shared_ptr<Submesh>& Mesh::GetSubmesh(uint32 i)
{
	return mSubmeshes[i];
}

void Mesh::Render()
{
	for (auto i = mSubmeshes.begin(); i != mSubmeshes.end(); i++)
	{
		i->get()->Render();
	}
}

void Mesh::CalculateAnimMatrices(float frame, std::vector<glm::mat4>& animMatrices)
{
	for (int i = 0; i < mBones.size(); ++i)
	{
		int parentIdx = mBones[i]->GetParentIndex();
		if (parentIdx != -1)
		{
			animMatrices[i] = animMatrices[parentIdx] * mBones[i]->CalculateAnimMatrix(frame);
		}
		else
		{
			animMatrices[i] = mBones[i]->CalculateAnimMatrix(frame);
		}
	}
	for (int i = 0; i < animMatrices.size(); ++i)
	{
		animMatrices[i] *= mBones[i]->GetInversePoseMatrix();
	}
}

int Mesh::GetBoneIndex(const std::string & name)
{
	int i = 0;
	for (auto it = mBones.begin(); it != mBones.end(); ++it, ++i)
	{
		if (it->get()->GetName() == name) return i;
	}
	return -1;
}

Mesh::Mesh(const std::string & filename)
{
	mFilename = filename;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());

	if (result)
	{
		std::cout << "XML [" << filename << "] parsed without errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n\n";
		pugi::xml_node materials = doc.child("mesh").child("materials");

		pugi::xml_node submeshes = doc.child("mesh").child("submeshes");

		for (pugi::xml_node submesh = submeshes.child("submesh"); submesh; submesh = submesh.next_sibling("submesh"))
		{
			//The information from the .msh for each submesh is extracted into strings
			std::string materialStr = submesh.child("material").text().as_string();
			std::string textureStr = submesh.child("texture").text().as_string();
			std::vector<std::string> indicesStrVec = SplitString(submesh.child("indices").text().as_string(), ',');
			std::vector<std::string> coordsStrVec = SplitString(submesh.child("coords").text().as_string(), ',');
			std::vector<std::string> texcoordsStrVec = SplitString(submesh.child("texcoords").text().as_string(), ',');
			std::vector<std::string> normStrVec = SplitString(submesh.child("normals").text().as_string(), ',');
			std::vector<std::string> tangStrVec = SplitString(submesh.child("tangents").text().as_string(), ',');
			std::vector<std::string> boneIndStrVec = SplitString(submesh.child("bone_indices").text().as_string(), ',');
			std::vector<std::string> boneWgtStrVec = SplitString(submesh.child("bone_weights").text().as_string(), ',');


			Material mat;
			//The texture for the submesh is instantiated, and with it the submesh is created
			if (materialStr.empty())
			{
				std::string fixed = ((ExtractPath(filename) != "") ? ExtractPath(filename) + "/" : "") + textureStr;
				std::shared_ptr<Texture> tex = ResourceManager::Instance()->LoadTexture(fixed);
				mat = Material(tex);
			}
			else
			{
				for (pugi::xml_node material = materials.child("material"); material; material = material.next_sibling("material"))
				{
					
					std::string name = material.child("name").text().as_string();
					if (name == materialStr) 
					{
						//If the name of the material is within the list of materials withing the file, we load the data.
						std::string fixed = ((ExtractPath(filename) != "") ? ExtractPath(filename) + "/" : "");
						std::string blendStr = material.child("blend").text().as_string();
						std::vector<std::string> diffuseStrVec = SplitString(material.child("diffuse").text().as_string(), ',');
						std::vector<std::string> textureNameVec = SplitString(material.child("texture").text().as_string(), ',');
						std::string normalName = material.child("normal_texture").text().as_string();
						std::vector<std::string> reflectNameVec = SplitString(material.child("reflect_texture").text().as_string(), ',');
						std::vector<std::string> refractNameVec = SplitString(material.child("refract_texture").text().as_string(), ',');
						float refractCoef = material.child("refract_coef").text().as_float(1.0f);
						int shininess = material.child("shininess").text().as_int();
						bool culling = material.child("culling").text().as_bool(true);
						bool depthWrite = material.child("depthwrite").text().as_bool(true);

						mat.SetBaseColor(glm::vec4(	NumberFromString<float>(diffuseStrVec[0]),
													NumberFromString<float>(diffuseStrVec[1]), 
													NumberFromString<float>(diffuseStrVec[2]), 
													NumberFromString<float>(diffuseStrVec[3])));
						if (blendStr.empty())
						{
							mat.SetBlendMode(Renderer::ALPHA);
						}
						else if (blendStr == "alpha")
						{
							mat.SetBlendMode(Renderer::ALPHA);
						}
						else if (blendStr == "add")
						{
							mat.SetBlendMode(Renderer::ADDITIVE);
						}
						else if (blendStr == "mul")
						{
							mat.SetBlendMode(Renderer::MULTIPLICATIVE);
						}

						if(textureNameVec.size() == 1)
						{
							mat.SetBaseTexture(ResourceManager::Instance()->LoadTexture(fixed  + textureNameVec[0]));
						}
						else if(textureNameVec.size() == 6)
						{
							mat.SetBaseTexture(ResourceManager::Instance()->LoadTexture(fixed + textureNameVec[0],
																						fixed + textureNameVec[1],
																						fixed + textureNameVec[2],
																						fixed + textureNameVec[3],
																						fixed + textureNameVec[4],
																						fixed + textureNameVec[5]));
						}

						mat.SetNormalTexture(ResourceManager::Instance()->LoadTexture(fixed + normalName));

						if (!reflectNameVec.empty()) {
							mat.SetReflectionTexture(ResourceManager::Instance()->LoadTexture(
								fixed + reflectNameVec[0], fixed + reflectNameVec[1], fixed + reflectNameVec[2],
								fixed + reflectNameVec[3], fixed + reflectNameVec[4], fixed + reflectNameVec[5]));
						}
						if (!refractNameVec.empty())
						{
							mat.SetRefractionTexture(ResourceManager::Instance()->LoadTexture(
								fixed + refractNameVec[0], fixed + refractNameVec[1], fixed + refractNameVec[2],
								fixed + refractNameVec[3], fixed + refractNameVec[4], fixed + refractNameVec[5]));
						}

						mat.SetRefractionCoef(refractCoef);
						mat.SetShininess(shininess);
						mat.SetCulling(culling);
						mat.SetDepthWrite(depthWrite);
						//continue;
					}
				}
			}

			std::shared_ptr<Submesh> smesh = Submesh::Create(mat);

			//Push the indices
			for (size_t i = 0; i < indicesStrVec.size(); i += 3)
			{
				smesh->AddTriangle(NumberFromString<float>(indicesStrVec[i]),
								   NumberFromString<float>(indicesStrVec[i + 1]),
								   NumberFromString<float>(indicesStrVec[i + 2]));
			}
			//Push the vertices
			for (size_t i = 0; i < coordsStrVec.size() / 3; ++i)
			{
				//Coords
				float x = NumberFromString<float>(coordsStrVec[i * 3]);
				float y = NumberFromString<float>(coordsStrVec[i * 3 + 1]);
				float z = NumberFromString<float>(coordsStrVec[i * 3 + 2]);
				
				// If any, we read the texture coordinates
				float u = 0, v = 0;
				if (texcoordsStrVec.size() > 0) 
				{
					u = NumberFromString<float>(texcoordsStrVec[i * 2]);
					v = NumberFromString<float>(texcoordsStrVec[i * 2 + 1]);
				}
				// If any, we read the normals for each vertex.
				float nx = 0, ny = 0, nz = 0;
				if (normStrVec.size() > 0) 
				{
					nx = NumberFromString<float>(normStrVec[i * 3]);
					ny = NumberFromString<float>(normStrVec[i * 3 + 1]);
					nz = NumberFromString<float>(normStrVec[i * 3 + 2]);
				}
				float tx = 0, ty = 0, tz = 0;
				if (tangStrVec.size() > 0)
				{
					tx = NumberFromString<float>(tangStrVec[i * 3]);
					ty = NumberFromString<float>(tangStrVec[i * 3 + 1]);
					tz = NumberFromString<float>(tangStrVec[i * 3 + 2]);
				}
				//If any, we read the bone indices for each vertex
				float i1 = -1, i2 = -1, i3 = -1, i4 = -1;
				if (boneIndStrVec.size() > 0)
				{
					i1 = NumberFromString<float>(boneIndStrVec[i * 4]);
					i2 = NumberFromString<float>(boneIndStrVec[i * 4 + 1]);
					i3 = NumberFromString<float>(boneIndStrVec[i * 4 + 2]);
					i4 = NumberFromString<float>(boneIndStrVec[i * 4 + 3]);
				}

				//If any, we read the bone weights for each vertex
				float w1 = 0, w2 = 0, w3 = 0, w4 = 0;
				if (boneWgtStrVec.size() > 0)
				{
					w1 = NumberFromString<float>(boneWgtStrVec[i * 4]);
					w2 = NumberFromString<float>(boneWgtStrVec[i * 4 + 1]);
					w3 = NumberFromString<float>(boneWgtStrVec[i * 4 + 2]);
					w4 = NumberFromString<float>(boneWgtStrVec[i * 4 + 3]);
				}
				smesh->AddVertex(Vertex(glm::vec3(x, y, z),
					glm::vec2(u, v),
					glm::vec3(nx, ny, nz),
					glm::vec3(tx, ty, tz),
					glm::vec4(i1, i2, i3, i4),
					glm::vec4(w1, w2, w3, w4)));
			}
			smesh.get()->Rebuild();
			mSubmeshes.push_back(smesh);
		}
		mLastFrame = NumberFromString<uint32>(doc.child("mesh").child("last_frame").text().as_string("0"));

		pugi::xml_node bones = doc.child("mesh").child("bones");
		if (bones) {
			for (pugi::xml_node bone = bones.child("bone"); bone; bone = bone.next_sibling("bone"))
			{
				//The bone info is extracted
				std::string name = bone.child("name").text().as_string();
				std::string parentStr = bone.child("parent").text().as_string();
				std::vector<std::string> invPoseStrVec = SplitString(bone.child("inv_pose").text().as_string(), ',');
				std::vector<std::string> positionsStrVec = SplitString(bone.child("positions").text().as_string(), ',');
				std::vector<std::string> rotationsStrVec = SplitString(bone.child("rotations").text().as_string(), ',');
				std::vector<std::string> scalesStrVec = SplitString(bone.child("scales").text().as_string(), ',');


				//The temp bone is generated
				std::shared_ptr<Bone> tbone = Bone::Create(name, GetBoneIndex(parentStr));
				glm::mat4 invPose;
				//Inv Pose Matrix is loaded
				for (int j = 0; j < 4; ++j)
				{
					for (int i = 0; i < 4; ++i) {
						// IF PROBLEMS DO DOUBLE ACCES MAT[][]
						//glm::vec4 row;
						//for (int i = 0; i < 4; ++i)
						//{
						//	row[i] = NumberFromString<float>(invPoseStrVec[i + 4 * j]);
						//}
						//invPose[j] = row;
						invPose[j][i] = NumberFromString<float>(invPoseStrVec[i + 4 * j]);
					}
				}
				//for (int i = 0; i < 4; ++i)
				//{
				//	invPose[i][0] = NumberFromString<float>(invPoseStrVec[4 * i]);
				//	invPose[i][1] = NumberFromString<float>(invPoseStrVec[4 * i + 1]);
				//	invPose[i][2] = NumberFromString<float>(invPoseStrVec[4 * i + 2]);
				//	invPose[i][3] = NumberFromString<float>(invPoseStrVec[4 * i + 3]);
				//}
				tbone->SetInversePoseMatrix(invPose);
				//Positions are loaded, if any
				if (!positionsStrVec.empty())
				{
					uint32 frame = 0;
					float px = 0, py = 0, pz = 0;
					for (int i = 0; i < positionsStrVec.size() / 4; ++i)
					{
						frame = NumberFromString<uint32>(positionsStrVec[i * 4]);
						px = NumberFromString<float>(positionsStrVec[i * 4 + 1]);
						py = NumberFromString<float>(positionsStrVec[i * 4 + 2]);
						pz = NumberFromString<float>(positionsStrVec[i * 4 + 3]);
						tbone->AddPosition(frame, glm::vec3(px, py, pz));
					}
				}
				//Rotations are loaded, if any
				if (!rotationsStrVec.empty())
				{
					uint32 frame = 0;
					float rw = 0, rx = 0, ry = 0, rz = 0;
					for (int i = 0; i < rotationsStrVec.size() / 5; ++i)
					{
						frame = NumberFromString<uint32>(rotationsStrVec[i * 5]);
						rw = NumberFromString<float>(rotationsStrVec[i * 5 + 1]);
						rx = NumberFromString<float>(rotationsStrVec[i * 5 + 2]);
						ry = NumberFromString<float>(rotationsStrVec[i * 5 + 3]);
						rz = NumberFromString<float>(rotationsStrVec[i * 5 + 4]);
						tbone->AddRotation(frame, glm::quat(rw, rx, ry, rz));
					}
				}
				if (!scalesStrVec.empty())
				{
					uint32 frame = 0;
					float sx = 0, sy = 0, sz = 0;
					for (int i = 0; i < scalesStrVec.size() / 4; ++i)
					{
						frame = NumberFromString<uint32>(scalesStrVec[i * 4]);
						sx = NumberFromString<float>(scalesStrVec[i * 4 + 1]);
						sy = NumberFromString<float>(scalesStrVec[i * 4 + 2]);
						sz = NumberFromString<float>(scalesStrVec[i * 4 + 3]);
						tbone->AddScale(frame, glm::vec3(sx, sy, sz));
					}
				}
				mBones.push_back(tbone);
			}
		}
	}
	else
	{
		if (filename.length() != 0) 
		{
			std::cout << "XML [" << filename << "] parsed with errors" << std::endl;
			std::cout << "Error description: " << result.description() << std::endl;
			std::cout << "Error offset: " << result.offset 
					  << " (error at [..." << (filename.c_str() + result.offset) << "]\n\n";
		}
		else
		{
			std::cout << "Empty mesh created." << std::endl;
		}
	}
}
