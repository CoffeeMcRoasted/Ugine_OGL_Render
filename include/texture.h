#ifndef UGINE_TEXTURE_H
#define UGINE_TEXTURE_H

#include "../include/types.h"
#include "../include/renderer.h"

class Texture {
public:
	static std::shared_ptr<Texture> Create(const std::string& filename);
	inline const std::string& GetFilename() const { return mFilename; }
	inline uint32 GetHandle() const { return mHandle; }
	inline uint32 GetWidth() const { return mWidth; }
	inline uint32 GetHeight() const { return mHeight; }
	//Cube mapping support
	static std::shared_ptr<Texture> Create(const std::string& left,
		const std::string& right, const std::string& front,
		const std::string& back, const std::string& top,
		const std::string& bottom);
	bool IsCubeMap() const { return mIsCubeMap; }
	Texture(const std::string& left, const std::string& right,
		const std::string& front, const std::string& back,
		const std::string& top, const std::string& bottom);
protected:
	Texture(const std::string& filename);
	~Texture();
	static void Delete(Texture* t) { delete t; }
private:
	std::string mFilename;
	uint32 mHandle;
	uint32 mWidth;
	uint32 mHeight;
	bool mIsCubeMap;
};

#endif//UGINE_TEXTURE_H