#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"


#include "helper/plane.h"
//#include "helper/cube.h"
#include "helper/torus.h"
#include "helper/teapot.h"
//#include "helper/objmesh.h"
//#include "helper/skybox.h"

class SceneBasic_Uniform : public Scene
{
private:
    //GLuint vaoHandle;
    GLSLProgram prog;
    GLuint fsQuad;
    GLuint renderFBO, intermediateFBO;
    GLuint renderTex, intermediateTex;

    float angle;
    float tPrev;
    float rotSpeed; 
    Torus SceneTorus;
    Teapot SceneTeapot;
    //Cube SceneCube;
    Plane ScenePlane;
    //std::unique_ptr<ObjMesh> spot;
    //SkyBox sky;

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void setMatrices();
    void setupFBO();
    //void renderToTexture();
    //void renderScene();
    void pass1();
    void pass2();
    void pass3();
    float gauss(float, float);
};

#endif // SCENEBASIC_UNIFORM_H
