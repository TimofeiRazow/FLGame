#include "Shader.h"

#include "AndroidOut.h"
#include "Utility.h"


Shader *Shader::loadShader(
        const std::string &vertexSource,
        const std::string &fragmentSource,
        const std::string &positionAttributeName,
        const std::string &uvAttributeName,
        const std::string &projectionMatrixUniformName) {
    Shader *shader = nullptr;

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) {
        return nullptr;
    }

    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        glDeleteShader(vertexShader);
        return nullptr;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint logLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

            // If we fail to link the shader program, log the result for debugging
            if (logLength) {
                GLchar *log = new GLchar[logLength];
                glGetProgramInfoLog(program, logLength, nullptr, log);
                aout << "Failed to link program with:\n" << log << std::endl;
                delete[] log;
            }

            glDeleteProgram(program);
        } else {
            // Get the attribute and uniform locations by name. You may also choose to hardcode
            // indices with layout= in your shader, but it is not done in this sample
            GLint positionAttribute = glGetAttribLocation(program, positionAttributeName.c_str());
            GLint uvAttribute = glGetAttribLocation(program, uvAttributeName.c_str());
            GLint projectionMatrixUniform = glGetUniformLocation(program,projectionMatrixUniformName.c_str());
            //GLint lookAtAttribute= glGetAttribLocation(program,lookAtMatrixUniformName.c_str());

            // Only create a new shader if all the attributes are found.

            if (positionAttribute != -1
                && uvAttribute != -1
                && projectionMatrixUniform != -1) {

                shader = new Shader(
                        program,
                        positionAttribute,
                        uvAttribute,
                        projectionMatrixUniform);
            } else {
                glDeleteProgram(program);
            }
        }
    }

    // The shaders are no longer needed once the program is linked. Release their memory.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader;
}
Shader *Shader::loadShader(const std::string &vertexSource, const std::string &fragmentSource,
                           const std::string &positionAttributeName,
                           const std::string &projectionMatrixUniformName) {
    Shader *shader= nullptr;
    GLint vertexShader=   loadShader(GL_VERTEX_SHADER,vertexSource);
    GLint fragmentShader= loadShader(GL_FRAGMENT_SHADER,fragmentSource);
    if(!vertexShader){
        aout<<"Ошибка в вершинном шейдере"<<std::endl;
        return nullptr;
    }
    if(!fragmentShader){
        glDeleteShader(vertexShader);
        aout<<"Ошибка в фрагментном шейдере"<<std::endl;
        return nullptr;
    }
    GLint program=glCreateProgram();
    if(program){
        glAttachShader(program,vertexShader);
        glAttachShader(program,fragmentShader);
        glLinkProgram(program);
        GLint linkStatus=GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if(linkStatus!=GL_TRUE){
            glDeleteProgram(program);
            aout<<"Ошибка в линке"<<std::endl;
        }else{
            GLint attr= glGetAttribLocation(program,positionAttributeName.c_str());
            GLint proj= glGetUniformLocation(program,projectionMatrixUniformName.c_str());
            if(attr!=-1&&proj!=-1){
                shader=new Shader(program,attr,proj);
            } else{
                aout<<"Ошибка при создании программы"<<std::endl;
                glDeleteProgram(program);
            }
        }
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shader;

}
GLuint Shader::loadShader(GLenum shaderType, const std::string &shaderSource) {
    Utility::assertGlError();
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        auto *shaderRawString = (GLchar *) shaderSource.c_str();
        GLint shaderLength = shaderSource.length();
        glShaderSource(shader, 1, &shaderRawString, &shaderLength);
        glCompileShader(shader);

        GLint shaderCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);

        // If the shader doesn't compile, log the result to the terminal for debugging
        if (!shaderCompiled) {
            GLint infoLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

            if (infoLength) {
                auto *infoLog = new GLchar[infoLength];
                glGetShaderInfoLog(shader, infoLength, nullptr, infoLog);
                aout << "Failed to compile with:\n" << infoLog << std::endl;
                delete[] infoLog;
            }
            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}

void Shader::activate() const {
    glUseProgram(program_);
}

void Shader::deactivate() const {
    glUseProgram(0);
}


void Shader::drawGameObject(const GameObject &gameObject) const {
    glVertexAttribPointer(
            position_,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vector3),
            gameObject.getVertex());
    glEnableVertexAttribArray(position_);
    glVertexAttribPointer(
            uv_,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vector2),
            gameObject.getVertexUV()
    );
    glEnableVertexAttribArray(uv_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gameObject.getTexture().getTextureID());
    glDrawElements(GL_TRIANGLES,gameObject.getIndexPosCount(),GL_UNSIGNED_INT,gameObject.getIndexPos());
    glDisableVertexAttribArray(position_);
    glDisableVertexAttribArray(uv_);
}
void Shader::drawUIElement(const UIElement &uiElement) const{
    glVertexAttribPointer(
            position_,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vector3),
            uiElement.getVertexPosition());
    glEnableVertexAttribArray(position_);
    glVertexAttribPointer(
            uv_,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vector2),
            uiElement.getVertexUV()
            );
    glEnableVertexAttribArray(uv_);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, uiElement.getTexture().getTextureID());

    glDrawElements(GL_TRIANGLES,uiElement.getIndexPosCount(),GL_UNSIGNED_INT,uiElement.getIndexPos());
    //glDrawArrays(GL_TRIANGLES,0,gameObject.getIndexPosCount());
    glDisableVertexAttribArray(position_);
    glDisableVertexAttribArray(uv_);
}

void Shader::setProjectionMatrix(float *projectionMatrix) const {
    glUniformMatrix4fv(projectionMatrix_, 1, false, projectionMatrix);
}
void Shader::setLookAtMatrix(float *projectionMatrix) const {
    glUniformMatrix4fv(lookAtMatrix_, 1, false, projectionMatrix);
}