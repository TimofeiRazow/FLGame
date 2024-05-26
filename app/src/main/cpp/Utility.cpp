#include "Utility.h"
#include "AndroidOut.h"

#include <GLES3/gl3.h>
#include "math.h"

#define CHECK_ERROR(e) case e: aout << "GL Error: "#e << std::endl; break;

bool Utility::checkAndLogGlError(bool alwaysLog) {
    GLenum error = glGetError();
    if (error == GL_NO_ERROR) {
        if (alwaysLog) {
            aout << "No GL error" << std::endl;
        }
        return true;
    } else {
        switch (error) {
            CHECK_ERROR(GL_INVALID_ENUM)
            CHECK_ERROR(GL_INVALID_VALUE)
            CHECK_ERROR(GL_INVALID_OPERATION)
            CHECK_ERROR(GL_INVALID_FRAMEBUFFER_OPERATION)
            CHECK_ERROR(GL_OUT_OF_MEMORY)
            default:
                aout << "Unknown GL error: " << error << std::endl;
        }
        return false;
    }
}

float *
Utility::buildOrthographicMatrix(float *outMatrix, float halfHeight, float aspect, float near,
                                 float far) {
    float halfWidth = halfHeight * aspect;

    // column 1
    outMatrix[0] = 1.f / halfWidth;
    outMatrix[1] = 0.f;
    outMatrix[2] = 0.f;
    outMatrix[3] = 0.f;

    // column 2
    outMatrix[4] = 0.f;
    outMatrix[5] = 1.f / halfHeight;
    outMatrix[6] = 0.f;
    outMatrix[7] = 0.f;

    // column 3
    outMatrix[8] = 0.f;
    outMatrix[9] = 0.f;
    outMatrix[10] = -2.f / (far - near);
    outMatrix[11] = -(far + near) / (far - near);

    // column 4
    outMatrix[12] = 0.f;
    outMatrix[13] = 0.f;
    outMatrix[14] = 0.f;
    outMatrix[15] = 1.f;

    return outMatrix;
}

float *Utility::buildIdentityMatrix(float *outMatrix) {
    // column 1
    outMatrix[0] = 1.f;
    outMatrix[1] = 0.f;
    outMatrix[2] = 0.f;
    outMatrix[3] = 0.f;

    // column 2
    outMatrix[4] = 0.f;
    outMatrix[5] = 1.f;
    outMatrix[6] = 0.f;
    outMatrix[7] = 0.f;

    // column 3
    outMatrix[8] = 0.f;
    outMatrix[9] = 0.f;
    outMatrix[10] = 1.f;
    outMatrix[11] = 0.f;

    // column 4
    outMatrix[12] = 0.f;
    outMatrix[13] = 0.f;
    outMatrix[14] = 0.f;
    outMatrix[15] = 1.f;

    return outMatrix;
}
float *Utility::buildPerspectiveMatrix(float *outMatrix, float fov, float aspect, float near,
                                       float far){
    // column 1
    outMatrix[0] = 1.0f/(tan(fov/2)*aspect);
    outMatrix[1] = 0.f;
    outMatrix[2] = 0.f;
    outMatrix[3] = 0.f;

    // column 2
    outMatrix[4] = 0.f;
    outMatrix[5] = 1.0f/tan(fov/2);
    outMatrix[6] = 0.f;
    outMatrix[7] = 0.f;

    // column 3
    outMatrix[8] = 0.f;
    outMatrix[9] = 0.f;
    outMatrix[10] = (-near-far)/(near-far);
    outMatrix[11] = 1.f;

    // column 4
    outMatrix[12] = 0.f;
    outMatrix[13] = 0.f;
    outMatrix[14] = near*far*2/(near-far);;
    outMatrix[15] = 0.f;

    return outMatrix;
}
float *Utility::buildLookAtMatrix(float *outMatrix, Vector3 direction, Vector3 up, Vector3 eye) {
    Vector3 zaxis= normalize(Vector3{direction.x-eye.x,direction.y-eye.y,direction.z-eye.z});
    Vector3 yaxis= normalize(cross(up,zaxis));
    Vector3 xaxis= normalize(cross(zaxis,yaxis));
    outMatrix[0] = xaxis.x;
    outMatrix[1] = yaxis.x;
    outMatrix[2] = zaxis.x;
    outMatrix[3] = 0.f;

    // column 2
    outMatrix[4] = xaxis.y;
    outMatrix[5] = yaxis.y;
    outMatrix[6] = zaxis.y;
    outMatrix[7] = 0.f;

    // column 3
    outMatrix[8] = xaxis.z;
    outMatrix[9] = yaxis.z;
    outMatrix[10] = zaxis.z;
    outMatrix[11] = 0.0f;

    // column 4
    outMatrix[12] = -scalarMultiply(xaxis,eye);
    outMatrix[13] = -scalarMultiply(yaxis,eye);
    outMatrix[14] = -scalarMultiply(zaxis,eye);
    outMatrix[15] = 1.f;

    return outMatrix;
}
Vector3 Utility::cross(Vector3 v1, Vector3 v2) {
    return Vector3{v1.y*v2.z-v1.y*v2.z,
                   v1.z*v2.x-v2.z*v1.x,
                   v1.z*v2.y-v1.x*v2.y};
}
Vector3 Utility::normalize(Vector3 v) {
    float len= sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);
    return Vector3{v.x/len,v.y/len,v.z/len};
}
float Utility::scalarMultiply(Vector3 v1, Vector3 v2) {
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}