#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

//#include "helper/torus.h"

#include "helper/plane.h"
#include "helper/objmesh.h"

class SceneBasic_Uniform : public Scene
{
private:
    //GLuint vaoHandle;
    GLSLProgram prog;
    //float angle;
    Plane plane; //plane surface
    std::unique_ptr<ObjMesh> mesh; //pig mesh

    //Torus SceneTorus;

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void setMatrices();
};

#endif // SCENEBASIC_UNIFORM_H
