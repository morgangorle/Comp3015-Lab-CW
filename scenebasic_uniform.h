#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"


#include "helper/plane.h"
//#include "helper/cube.h"
//#include "helper/torus.h"
#include "helper/teapot.h"
#include "helper/sphere.h"
//#include "helper/objmesh.h"
//#include "helper/skybox.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;
    GLuint hdrFBO;
    GLuint quad;
    GLuint hdrTex, avgTex;

    float angle;
    float tPrev;
    float rotSpeed; 
    Teapot SceneTeapot;
    Sphere SceneSphere;
    Plane ScenePlane;

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void setMatrices();

    void setupFBO();
    void pass1();
    void pass2();
    void computeLogAveLuminance();
    void drawScene();

    float gauss(float, float);
};

#endif // SCENEBASIC_UNIFORM_H
