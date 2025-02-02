#pragma once
#include "glm/glm.hpp"

#include "NonCopyable.h"
#include <map>
#include <string>

typedef unsigned int GLuint;

class GLSLShader : public NonCopyable
{
private:
	GLuint m_ID;
	std::map<std::string, int> m_Uniforms;

	void checkCompileErrors(unsigned int shader, std::string type);
	void compileShader(std::string VertexCode, std::string FragmentCode);
	bool isUniformExist(const std::string &uniformName) const;
	void uniformErrorPrintLog(const std::string& name) const;
public:
	void use();

	GLSLShader(const char* vertexPath, const char* fragmentPath);
	~GLSLShader();

	void addUniform(std::string uniformName);

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
};