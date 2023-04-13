#include "scenebasic_uniform.h"

#include <sstream>
#include <cstdio>
#include <cstdlib>

#include "helper/texture.h";

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;
using std::cout;

#include "helper/glutils.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform()
{
    //ScenePlane(50.0f, 50.0f, 1, 1)
    //tPrev = 0.0f;
    //angle = 0.0f;
    //rotSpeed = (glm::pi<float>() / 8.0f);
    //sky(100.0f)
    //ogre = ObjMesh::load("media/bs_ears.obj", false, true);
    //SceneCube(1.0f)
    //SceneTeapot(14, mat4(1.0f))

}

void SceneBasic_Uniform::initScene()
{
    compile();

    glClearColor(0.5f, 0.5f,0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    //setup 50 Sprites
    numSprites = 50;
    locations = new float[numSprites * 3];
    srand((unsigned int)time(0));

    //For each sprite set up a location in the array

    for( int i = 0; i < numSprites;i ++)
    {
        vec3 p(((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f) - 1.0f,
            ((float)rand() / RAND_MAX * 2.0f - 1.0f));
        locations[i * 3] = p.x;
        locations[i * 3 + 1] = p.y;
        locations[i * 3 + 2] = p.z;

    }

    //Set up Buffers
    GLuint handle;
    glGenBuffers(1, &handle);

    glBindBuffer(GL_ARRAY_BUFFER,handle);
    glBufferData(GL_ARRAY_BUFFER,numSprites * 3 + sizeof(float), locations, GL_STATIC_DRAW);

    delete[] locations;


    // Set up Vertex Array Object
    glGenVertexArrays(1, &sprites);
    glBindVertexArray(sprites);

    glBindBuffer(GL_ARRAY_BUFFER,handle);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0, ((GLubyte*) NULL + (0)));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    //Load Texture File
    const char* texName = "../Comp3015-Lab-CW/media/texture/flower.png";
    Texture::loadTexture(texName);

    prog.setUniform("SpriteTex", 0);
    prog.setUniform("Size2", 0.15f);

}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
        prog.compileShader("shader/basic_uniform.gs");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update(float t)
{

}



void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Set Up Camera
    vec3 cameraPos(0.0f, 0.0f, 3.0f);
    view = glm::lookAt(cameraPos,
        vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));

    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(sprites);
    glDrawArrays(GL_POINTS,0,numSprites);

    glFinish();

}



void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h,
        0.3f, 100.0f);;
}

void SceneBasic_Uniform::setMatrices()
{
    glm::mat4 mv = model * view;

    //prog.setUniform("ModelMatrix", model);
    prog.setUniform("ModelViewMatrix", mv);
    //prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),vec3(mv[1]),vec3(mv[2])));
    //prog.setUniform("MVP", projection * mv);
    prog.setUniform("ProjectionMatrix", projection);


}