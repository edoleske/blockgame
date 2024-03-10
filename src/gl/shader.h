#ifndef BLOCKGAME_SHADER_H
#define BLOCKGAME_SHADER_H


#include "common.h"

class Shader {
public:
    Shader(const string& vertexPath, const string& fragmentPath);

    bool isCompiled() const;

    void use() const;

    void setInteger(const GLchar* name, GLint value) const;

    void setMatrix4(const GLchar* name, const glm::mat4 &matrix);

    void setVector4f(const GLchar* name, const vec4& value);
private:
    GLuint program;
    bool compiled = false;

    int getUniformLocation(const GLchar* name) const;

    static string readFile(const string &path);
};


#endif //BLOCKGAME_SHADER_H
