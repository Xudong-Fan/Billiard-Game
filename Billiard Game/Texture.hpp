#pragma once

#include <string>

#include <gl/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "util.hpp"

class Texture{
private:
	GLuint texture_Unit;
public:
	static GLuint uTextureCountLoc;

	Texture();
	virtual ~Texture();
	void clear();

	bool load_Texture(const std::string& path);
	GLuint get_Texture_Unit() const;
};

