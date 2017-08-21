#ifndef UGINE_RENDERER_H
#define UGINE_RENDERER_H

#include "types.h"

static constexpr int MAX_LIGHTS = 8;
static constexpr int MAX_BONES = 75;
static constexpr int BASETEX_UNIT = 0;
static constexpr int BASECUBETEX_UNIT = 1;
static constexpr int REFLECTTEX_UNIT = 2;
static constexpr int REFRACTTEX_UNIT = 3;
static constexpr int NORMALTEX_UNIT = 4;
static constexpr int DEPTHSAMPLER_UNIT = 15;

class Renderer {
public:
	static const std::shared_ptr<Renderer>& Instance();

	//Blend Modes
	enum BlendMode {
		ALPHA,
		MULTIPLICATIVE,
		ADDITIVE
	};
	// Setup
	void Setup3D();
	void SetMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	void SetViewport(int x, int y, int w, int h);

	// Drawing
	void ClearColorBuffer(const glm::vec3& color);
	void ClearDepthBuffer();

	// VBO
	uint32 CreateBuffer();
	void FreeBuffer(uint32 buffer);
	void SetVertexBufferData(uint32 vertexBuffer, const void* data, uint32 dataSize);
	void SetIndexBufferData(uint32 indexBuffer, const void* data, uint32 dataSize);
	void DrawBuffers(uint32 vertexBuffer, uint32 indexBuffer, uint32 numIndices);

	// Shaders
	uint32 CreateProgram(const std::string& vertex, const std::string& fragment);
	void FreeProgram(uint32 program);
	void UseProgram(uint32 programu = Renderer::Instance().get()->mDefaultProgram);
	const std::string& GetProgramError();

	//Textures
	uint32 LoadTexture(const std::string& filename, uint32& width, uint32& height);
	void FreeTexture(uint32 tex);
	//void SetTexture(uint32 tex);
	void SetTextures(uint32 baseTexture, uint32 reflectionTexture,
		uint32 refractionTexture, uint32 normalTexture, bool isBaseTexCube);

	//Lighting
	void SetDiffuse(const glm::vec4& color);
	void SetAmbient(const glm::vec3& color);
	void SetShininess(uint8 shininess);
	void EnableLighting(bool enable);
	void EnableLight(uint32 index, bool enabled);
	void SetLightData(uint32 index, const glm::vec4& vector, const glm::vec3& color, float attenuation);

	//Animation
	void SetSkinned(bool skinned);
	void SetAnimMatrices(const std::vector<glm::mat4>& matrices);

	//Advanced Lighting
	void SetBlendMode(BlendMode mode);
	void SetCulling(bool enable);
	void SetDepthWriting(bool enable);
	void SetEyePos(const glm::vec3& pos);
	void SetRefractCoef(float coef);
	uint32 LoadCubeTexture(const std::string& left, const std::string& right,
		const std::string& front, const std::string& back,
		const std::string& top, const std::string& bottom,
		uint32& width, uint32& height);

protected:
	Renderer();
	~Renderer() {}
	static void Delete(Renderer* r) { delete r; }
private:
	static std::shared_ptr<Renderer> mInstance;
	//MVP matrix
	int mMVPLoc;
	//Lighting matrices, model view and  normal
	int mMVLoc;
	int mNormLoc;
	//Vertex Buffers
	int mVPosLoc;
	int mVTexLoc;
	int mVNormLoc;
	//Texture location
	//int mTexSLoc;
	//int mUsesTexLoc;
	//Lighting enable
	int mLightingEnableLoc;
	int mLightEnabledLoc[MAX_LIGHTS];
	//Light Properties
	int mLightPosLoc[MAX_LIGHTS];
	int mLightColorLoc[MAX_LIGHTS];
	int mLightAttLoc[MAX_LIGHTS];
	//Shader Lighting Variables
	int mAmbientLoc;
	int mDiffuseLoc;
	int mShininessLoc;
	//Animation properties
	int mSkinnedLoc;
	int mAnimMatricesLoc;
	int mVBoneIndicesLoc;
	int mVBoneWeightsLoc;

	//Program attrib
	uint32 mDefaultProgram;
	std::string mProgramError;

	//Adavanced Illumination
	int mModelLoc;
	int mEyePosLoc;
	int mRefractCoefLoc;
	int mBaseTexModeLoc;
	int mUseReflectTexLoc;
	int mUseRefractTexLoc;
	int mUseNormalTexLoc;
	int mVTangentLoc;

	int mBaseTexSLoc;
	int mBaseCubeTexSLoc;
	int mReflectTexSLoc;
	int mRefractTexSLoc;
	int mNormalTexSLoc;

};

inline const std::shared_ptr<Renderer>& Renderer::Instance() {
	if ( !mInstance )
		mInstance = std::shared_ptr<Renderer>(new Renderer(), Delete);
	return mInstance;
}

#endif // UGINE_RENDERER_H
