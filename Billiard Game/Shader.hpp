#pragma once

#include <gl/glew.h>

#include <string>
#include "util.hpp"

class Shader{
private:
	GLenum shader_Type;
	GLuint shader_ID;
public:
	Shader();
	Shader(const std::string& path, GLenum shader_Type);
	virtual ~Shader();

	void load_Shader(const std::string& path, GLenum shader_Type);

	GLenum get_shader_Type() const;
	GLuint get_shader_ID() const;
};

class Program{
private:
	GLuint shader_Program;
public:
	Program();
	virtual ~Program();

	void attach_Shader(Shader* shader);
	static void useProgram(const Program* prog);

	GLuint getProgram() const;
};