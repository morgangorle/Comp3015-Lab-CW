#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;
GLfloat angle = 0.0f;

SceneBasic_Uniform::SceneBasic_Uniform() : SceneTorus(0.8f,0.3f,75,75) {}



void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    model = mat4(1.0f);
    //model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
    view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    
    prog.setUniform("Kd", 0.2f,0.55f,0.9f);
    prog.setUniform("Ld", 1.0f,1.0f,1.0f);

    prog.setUniform("Ka", 0.2f, 0.55f, 0.9f);
    prog.setUniform("La", 1.0f, 1.0f, 1.0f);

    prog.setUniform("Ks", 0.2f, 0.55f, 0.9f);
    prog.setUniform("Ls", 1.0f, 1.0f, 1.0f);


    prog.setUniform("LightPosition", view *
        glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));

}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    angle = 1.1f;
    model = glm::rotate(model, glm::radians(angle), vec3(0, angle, angle));

}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    setMatrices();
    SceneTorus.render();
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

    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),vec3(mv[1]),vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);


}
