#include "../lib/glew/glew.h"
#include "../include/renderer.h"
#include "../include/vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

std::shared_ptr<Renderer> Renderer::mInstance = nullptr;

Renderer::Renderer() {
	mDefaultProgram = CreateProgram(ReadString("data/vertex.glsl"), ReadString("data/fragment.glsl"));
	UseProgram(mDefaultProgram);
}

void Renderer::Setup3D() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthFunc(GL_LEQUAL);
}

void Renderer::SetMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
	glUniformMatrix4fv(mMVPLoc, 1, false, glm::value_ptr(projection * view * model));
	glUniformMatrix4fv(mMVLoc, 1, false, glm::value_ptr(view * model));
	glUniformMatrix4fv(mNormLoc, 1, false, glm::value_ptr(glm::transpose(glm::inverse(view * model))));
	glUniformMatrix4fv(mModelLoc, 1, false, glm::value_ptr(model));
}

void Renderer::SetViewport(int x, int y, int w, int h) {
	glViewport(x, y, w, h);
	glScissor(x, y, w, h);
}

void Renderer::ClearColorBuffer(const glm::vec3& color) {
	glClearColor(color.r, color.g, color.b, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ClearDepthBuffer() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

uint32 Renderer::CreateBuffer() {
	uint32_t buffer;
	glGenBuffers(1, &buffer);
	return buffer;
}

void Renderer::FreeBuffer(uint32 buffer) {
	glDeleteBuffers(1, &buffer);
}

void Renderer::SetVertexBufferData(uint32 vertexBuffer, const void* data, uint32 dataSize) {
	//Support for animations added
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(mVPosLoc);
	glEnableVertexAttribArray(mVTexLoc);
	glEnableVertexAttribArray(mVNormLoc);
	glEnableVertexAttribArray(mVTangentLoc);
	glEnableVertexAttribArray(mVBoneIndicesLoc);
	glEnableVertexAttribArray(mVBoneWeightsLoc);
	glVertexAttribPointer(mVPosLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mPosition)));
	glVertexAttribPointer(mVTexLoc, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mTexture)));
	glVertexAttribPointer(mVNormLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mNormal)));
	glVertexAttribPointer(mVTangentLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mTangent)));
	glVertexAttribPointer(mVBoneIndicesLoc, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mBoneIndices)));
	glVertexAttribPointer(mVBoneWeightsLoc, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mBoneWeights)));
	//Send data and unbind buffer
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void Renderer::SetIndexBufferData(uint32 indexBuffer, const void* data, uint32 dataSize) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::DrawBuffers(uint32 vertexBuffer, uint32 indexBuffer, uint32 numIndices) {
	//Support for animations added
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glEnableVertexAttribArray(mVPosLoc);
	glEnableVertexAttribArray(mVTexLoc);
	glEnableVertexAttribArray(mVNormLoc); //was mNormLoc before...
	glEnableVertexAttribArray(mVTangentLoc);
	glEnableVertexAttribArray(mVBoneIndicesLoc);
	glEnableVertexAttribArray(mVBoneWeightsLoc);
	glVertexAttribPointer(mVPosLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mPosition)));
	glVertexAttribPointer(mVTexLoc, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mTexture)));
	glVertexAttribPointer(mVNormLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mNormal)));
	glVertexAttribPointer(mVTangentLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mTangent)));
	glVertexAttribPointer(mVBoneIndicesLoc, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mBoneIndices)));
	glVertexAttribPointer(mVBoneWeightsLoc, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mBoneWeights)));


	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(0));
	
	glDisableVertexAttribArray(mVPosLoc);
	glDisableVertexAttribArray(mVTexLoc);
	glDisableVertexAttribArray(mVNormLoc);
	glDisableVertexAttribArray(mVTangentLoc);
	glDisableVertexAttribArray(mVBoneIndicesLoc);
	glDisableVertexAttribArray(mVBoneWeightsLoc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32 Renderer::CreateProgram(const std::string& vertex, const std::string& fragment) {
	//Error management
	GLint retCode;
	char errorLog[1024];

	//Vertex Shader Creation
	const char* vShaderSrc = vertex.c_str();
	uint32_t vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vShaderSrc, nullptr);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(vShader, sizeof(errorLog), NULL, errorLog);
		mProgramError = errorLog;
		return 0;
	}

	//Fragment Shader Creation
	const char* fShaderSrc = fragment.c_str();
	uint32_t fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderSrc, nullptr);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(fShader, sizeof(errorLog), NULL, errorLog);
		mProgramError = errorLog;
		return 0;
	}

	//Program Creation
	uint32_t program;
	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetProgramInfoLog(program, sizeof(errorLog), nullptr, errorLog);
		mProgramError = errorLog;
		return 0;
	}

	return program;
}

void Renderer::FreeProgram(uint32 program) {
	glDeleteProgram(program);
}

void Renderer::UseProgram(uint32 program) {
	glUseProgram(program);
	//Get matrices location
	mMVPLoc = glGetUniformLocation(program, "MVP");
	mMVLoc = glGetUniformLocation(program, "modelView");
	mNormLoc = glGetUniformLocation(program, "normalMatrix");
	//Vertex information location
	mVPosLoc = glGetAttribLocation(program, "vpos");
	mVTexLoc = glGetAttribLocation(program, "vtex");
	mVNormLoc = glGetAttribLocation(program, "vnormal");
	mVTangentLoc = glGetAttribLocation(program, "vtangent");

	//Texture
	//mTexSLoc = glGetUniformLocation(program, "texsampler");
	//mUsesTexLoc = glGetUniformLocation(program, "usestex");

	//Lighting enabling
	mLightingEnableLoc = glGetUniformLocation(program, "lightingEnabled");
	//Location for each light
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		mLightEnabledLoc[i] = glGetUniformLocation(program, std::string("lightEnabled[" + std::to_string(i) + "]").c_str());
		mLightPosLoc[i] = glGetUniformLocation(program, std::string("lightPos[" + std::to_string(i) + "]").c_str());
		mLightColorLoc[i] = glGetUniformLocation(program, std::string("lightColor[" + std::to_string(i) + "]").c_str());
		mLightAttLoc[i] = glGetUniformLocation(program, std::string("lightAtt[" + std::to_string(i) + "]").c_str());
	}

	//Shader Lighting variables
	mAmbientLoc = glGetUniformLocation(program, "ambient");
	mDiffuseLoc = glGetUniformLocation(program, "diffuse");
	mShininessLoc = glGetUniformLocation(program, "shininess");

	//Animation
	mSkinnedLoc = glGetUniformLocation(program, "skinned");
	mAnimMatricesLoc = glGetUniformLocation(program, "animMatrices");
	mVBoneIndicesLoc = glGetAttribLocation(program, "vboneIndices");
	mVBoneWeightsLoc = glGetAttribLocation(program, "vboneWeights");

	//Advanced Lightning
	mModelLoc = glGetUniformLocation(program, "model");
	mEyePosLoc = glGetUniformLocation(program, "eyePos");
	mRefractCoefLoc = glGetUniformLocation(program, "refractCoef");

	//Texture Samplers
	//Tex usage
	mBaseTexModeLoc = glGetUniformLocation(program, "baseTexMode");
	mUseReflectTexLoc = glGetUniformLocation(program, "useReflectTex");
	mUseRefractTexLoc = glGetUniformLocation(program, "useRefractTex");
	mUseNormalTexLoc = glGetUniformLocation(program, "useNormalTex");

	//Tex samplers
	mBaseTexSLoc = glGetUniformLocation(program, "baseTexSampler");
	mBaseCubeTexSLoc = glGetUniformLocation(program, "baseCubeSampler");
	mReflectTexSLoc = glGetUniformLocation(program, "reflectCubeSampler");
	mRefractTexSLoc = glGetUniformLocation(program, "refractCubeSampler");
	mNormalTexSLoc = glGetUniformLocation(program, "normalTexSampler");
}

const std::string& Renderer::GetProgramError() {
	return mProgramError;
}

uint32 Renderer::LoadTexture(const std::string & filename, uint32 & width, uint32 & height)
{
	// Image loaded with stb_image
	//Tmp variables coz casts
	int wdt, hgt;
	std::shared_ptr<uint8_t> texBuffer = std::shared_ptr<uint8_t>(stbi_load(filename.c_str(),
		&wdt, &hgt, nullptr, 4), stbi_image_free);
	if (texBuffer == nullptr) return 0;
	std::shared_ptr<uint8_t> mirroredTexBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(wdt * hgt * 4)), free);
	for (int line = 0; line < hgt; ++line) {
		memcpy(mirroredTexBuffer.get() + line*wdt * 4, texBuffer.get() + (hgt - line - 1)*wdt * 4, wdt * 4);
	}

	// Generation of the texture
	uint32 tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//By default, GL_TEXTURE_WRAP is repeat, so no further actions are needed.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wdt, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, mirroredTexBuffer.get());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Returns
	width = static_cast<uint32>(wdt);
	height = static_cast<uint32>(hgt);
	return tex;
}

void Renderer::FreeTexture(uint32 tex)
{
	glDeleteTextures(1, &tex);
}

//void Renderer::SetTexture(uint32 tex)
//{
//	glBindTexture(GL_TEXTURE_2D, tex);
//	glUniform1i(mTexSLoc, 0);
//	if (tex == 0)
//	{
//		glUniform1i(mUsesTexLoc, false);
//	}
//	else
//	{
//		glUniform1i(mUsesTexLoc, true);
//	}
//}

void Renderer::SetTextures(uint32 baseTexture, uint32 reflectionTexture,
	uint32 refractionTexture, uint32 normalTexture, bool isBaseTexCub)
{
	if( baseTexture == 0 )
	{
		glUniform1i(mBaseTexModeLoc, 0);
	}
	//else if (!isBaseTexCub)
	//{
	//	glUniform1i(mBaseTexModeLoc, 1);
	//}
	//else if (isBaseTexCub)
	//{
	//	glUniform1i(mBaseTexModeLoc, 2);
	//}
	else
	{
		if (isBaseTexCub)
		{
			glUniform1i(mBaseTexModeLoc, 2);
		}
		else
		{
			glUniform1i(mBaseTexModeLoc, 1);
		}
	}

	
	if (isBaseTexCub)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, baseTexture);
		glUniform1i(mBaseCubeTexSLoc, 1/*BASECUBETEX_UNIT*/);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, baseTexture);
		glUniform1i(mBaseTexSLoc, 0/*BASETEX_UNIT*/);
	}

	if (reflectionTexture == 0)
	{
		glUniform1i(mUseReflectTexLoc, false);
	}
	else
	{
		glUniform1i(mUseReflectTexLoc, true);
	}
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionTexture);
	glUniform1i(mReflectTexSLoc, 2/*REFLECTTEX_UNIT*/);

	if (refractionTexture == 0)
	{
		glUniform1i(mUseRefractTexLoc, false);
	}
	else
	{
		glUniform1i(mUseRefractTexLoc, true);
	}
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, refractionTexture);
	glUniform1i(mRefractTexSLoc, 3/*REFRACTTEX_UNIT*/);

	if (normalTexture == 0)
	{
		glUniform1i(mUseNormalTexLoc, false);
	}
	else
	{
		glUniform1i(mUseNormalTexLoc, true);
	}
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, normalTexture);
	glUniform1i(mNormalTexSLoc, 4/*NORMALTEX_UNIT*/);
}

void Renderer::SetDiffuse(const glm::vec4 & color)
{
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::vec4(color, 1.f).data);
	glUniform4fv(mDiffuseLoc, 1, glm::value_ptr(color));
}

void Renderer::SetAmbient(const glm::vec3 & color)
{
	//glMaterialfv(GL_FRONT, GL_AMBIENT, glm::vec4(color, 1.f).data);
	glUniform3fv(mAmbientLoc, 1, glm::value_ptr(color));
}

void Renderer::SetShininess(uint8 shininess)
{
	//float aux[] = { static_cast<float>(shininess) };
	//glMaterialfv(GL_FRONT, GL_SHININESS, aux);
	glUniform1i(mShininessLoc, shininess);
}

void Renderer::EnableLighting(bool enable)
{
	glUniform1i(mLightingEnableLoc, enable);
}

void Renderer::EnableLight(uint32 index, bool enabled)
{
	glUniform1i(mLightEnabledLoc[index], enabled);
}

void Renderer::SetLightData(uint32 index, const glm::vec4 & vector, const glm::vec3 & color, float attenuation)
{
	glUniform4fv(mLightPosLoc[index], 1, glm::value_ptr(vector));
	glUniform3fv(mLightColorLoc[index], 1, glm::value_ptr(color));
	glUniform1f(mLightAttLoc[index], attenuation);
}

void Renderer::SetSkinned(bool skinned)
{
	glUniform1i(mSkinnedLoc, skinned);
}

void Renderer::SetAnimMatrices(const std::vector<glm::mat4>& matrices)
{
	if (mAnimMatricesLoc > -1)
	{
		size_t numBones = std::min<size_t>(MAX_BONES, matrices.size());
		if (numBones != 0) 
		{
			glUniformMatrix4fv(mAnimMatricesLoc, numBones, false, glm::value_ptr(matrices[0]));
		}
	}
}

void Renderer::SetBlendMode(BlendMode mode)
{
	switch (mode)
	{
	case(ALPHA):
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case(MULTIPLICATIVE):
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		break;
	case(ADDITIVE):
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	default:
		//Alpha is default
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	}
}

void Renderer::SetCulling(bool enable)
{
	if (enable) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);
}

void Renderer::SetDepthWriting(bool enable)
{
	if (enable) glDepthMask(GL_TRUE);
	else glDepthMask(GL_FALSE);
}

void Renderer::SetEyePos(const glm::vec3 & pos)
{
	glUniform3fv(mEyePosLoc, 1, glm::value_ptr(pos));
}

void Renderer::SetRefractCoef(float coef)
{
	glUniform1f(mRefractCoefLoc, coef);
}

uint32 Renderer::LoadCubeTexture(const std::string & left, const std::string & right, const std::string & front, const std::string & back, const std::string & top, const std::string & bottom, uint32 & width, uint32 & height)
{
	
	// Image loaded with stb_image
	//Tmp variables coz casts
	//TODO:: OBTAIN VALUES OF WITDH HEIGHT ETC FOR EACH IMAGE
	int wdt, hgt;
	std::shared_ptr<uint8_t> leftBuffer = std::shared_ptr<uint8_t>(stbi_load(left.c_str(),
		&wdt, &hgt, nullptr, 4), stbi_image_free);
	if (leftBuffer == nullptr) return 0;
	std::shared_ptr<uint8_t> mirroredLeftBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(wdt * hgt * 4)), free);
	for (int line = 0; line < hgt; ++line) {
		memcpy(mirroredLeftBuffer.get() + line*wdt * 4, leftBuffer.get() + (hgt - line - 1)*wdt * 4, wdt * 4);
	}

	std::shared_ptr<uint8_t> rightBuffer = std::shared_ptr<uint8_t>(stbi_load(right.c_str(),
		&wdt, &hgt, nullptr, 4), stbi_image_free);
	if (rightBuffer == nullptr) return 0;
	std::shared_ptr<uint8_t> mirroredRightBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(wdt * hgt * 4)), free);
	for (int line = 0; line < hgt; ++line) {
		memcpy(mirroredRightBuffer.get() + line*wdt * 4, rightBuffer.get() + (hgt - line - 1)*wdt * 4, wdt * 4);
	}

	std::shared_ptr<uint8_t> frontBuffer = std::shared_ptr<uint8_t>(stbi_load(front.c_str(),
		&wdt, &hgt, nullptr, 4), stbi_image_free);
	if (frontBuffer == nullptr) return 0;
	std::shared_ptr<uint8_t> mirroredFrontBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(wdt * hgt * 4)), free);
	for (int line = 0; line < hgt; ++line) {
		memcpy(mirroredFrontBuffer.get() + line*wdt * 4, frontBuffer.get() + (hgt - line - 1)*wdt * 4, wdt * 4);
	}

	std::shared_ptr<uint8_t> backBuffer = std::shared_ptr<uint8_t>(stbi_load(back.c_str(),
		&wdt, &hgt, nullptr, 4), stbi_image_free);
	if (backBuffer == nullptr) return 0;
	std::shared_ptr<uint8_t> mirroredBackBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(wdt * hgt * 4)), free);
	for (int line = 0; line < hgt; ++line) {
		memcpy(mirroredBackBuffer.get() + line*wdt * 4, backBuffer.get() + (hgt - line - 1)*wdt * 4, wdt * 4);
	}

	std::shared_ptr<uint8_t> topBuffer = std::shared_ptr<uint8_t>(stbi_load(top.c_str(),
		&wdt, &hgt, nullptr, 4), stbi_image_free);
	if (topBuffer == nullptr) return 0;
	std::shared_ptr<uint8_t> mirroredTopBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(wdt * hgt * 4)), free);
	for (int line = 0; line < hgt; ++line) {
		memcpy(mirroredTopBuffer.get() + line*wdt * 4, topBuffer.get() + (hgt - line - 1)*wdt * 4, wdt * 4);
	}

	std::shared_ptr<uint8_t> bottomBuffer = std::shared_ptr<uint8_t>(stbi_load(bottom.c_str(),
		&wdt, &hgt, nullptr, 4), stbi_image_free);
	if (bottomBuffer == nullptr) return 0;
	std::shared_ptr<uint8_t> mirroredBottomBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(wdt * hgt * 4)), free);
	for (int line = 0; line < hgt; ++line) {
		memcpy(mirroredBottomBuffer.get() + line*wdt * 4, bottomBuffer.get() + (hgt - line - 1)*wdt * 4, wdt * 4);
	}

	// Generation of the texture
	uint32 tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	//AS configured as of now, cubemaps must be cubes, with width and height being the same for every image
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, wdt, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, leftBuffer.get());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, wdt, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, rightBuffer.get());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, wdt, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, frontBuffer.get());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, wdt, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, backBuffer.get());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, wdt, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, topBuffer.get());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, wdt, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottomBuffer.get());

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Returns
	width = static_cast<uint32>(wdt);
	height = static_cast<uint32>(hgt);
	return tex;
}
