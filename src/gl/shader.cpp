#include "shader.h"

Shader::Shader(const string &vertexPath, const string &fragmentPath) {
    string vertexShaderString = readFile(vertexPath);
    string fragmentShaderString = readFile(fragmentPath);

    const char* vertexShader = vertexShaderString.c_str();
    const char* fragmentShader = fragmentShaderString.c_str();

    int vertexSuccess, fragmentSuccess, linkSuccess;
    char infoLog[512];

    int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, nullptr);
    glCompileShader(vs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &vertexSuccess);
    if (!vertexSuccess) {
        glGetShaderInfoLog(vs, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
    }

    int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, nullptr);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &fragmentSuccess);
    if (!fragmentSuccess) {
        glGetShaderInfoLog(fs, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, fs);
    glAttachShader(program, vs);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << infoLog << std::endl;
    }

    compiled = vertexSuccess && fragmentSuccess && linkSuccess;

    use();
    setInteger("CHUNK_SIZE_X", CHUNK_SIZE_X);
    setInteger("CHUNK_SIZE_Z", CHUNK_SIZE_Z);
}

bool Shader::isCompiled() const {
    return compiled;
}

void Shader::use() const {
    glUseProgram(program);
}

void Shader::setInteger(const GLchar* name, GLint value) const {
    int location = getUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::setMatrix4(const GLchar* name, const mat4 &matrix) {
    int location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

int Shader::getUniformLocation(const GLchar* name) const {
    return glGetUniformLocation(program, name);
}

string Shader::readFile(const string &path) {
    ifstream ifs(path);
    stringstream buffer;
    buffer << ifs.rdbuf();

    return buffer.str();
}

void Shader::setVector4f(const GLchar* name, const vec4& value) {
    int location = getUniformLocation(name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}
