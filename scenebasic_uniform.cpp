#include "scenebasic_uniform.h"

#include <sstream>
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

SceneBasic_Uniform::SceneBasic_Uniform() : ScenePlane(10.0f,10.0f,100,100) 
{
    mesh = ObjMesh::load("../Comp3015-Lab-Work/media/pig_triangulated.obj",true);
}



void SceneBasic_Uniform::initScene()
{
    compile();

    glEnable(GL_DEPTH_TEST);

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(160.0f), vec3(0.0f, 0.0f, 1.0f));
    view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    // Set Lighting Uniform Values
    float x, z;
    for (int i = 0; i < 3; i++)
    {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3 * i));
        z = 2.0f * sinf((glm::two_pi<float>() / 3 * i));
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 0.0f));
    }
    prog.setUniform("lights[0].Ld", 0.0f, 0.0f, 0.8f);
    prog.setUniform("lights[1].Ld", 0.0f, 0.8f, 0.0f);
    prog.setUniform("lights[2].Ld", 0.8f, 0.0f, 0.0f);

    prog.setUniform("lights[0].La", 0.0f, 0.0f, 0.1f);
    prog.setUniform("lights[1].La", 0.0f, 0.1f, 0.0f);
    prog.setUniform("lights[2].La", 0.1f, 0.0f, 0.0f);

    prog.setUniform("lights[0].Ls", 0.0f, 0.0f, 0.8f);
    prog.setUniform("lights[1].Ls", 0.0f, 0.8f, 0.0f);
    prog.setUniform("lights[2].Ls", 0.8f, 0.0f, 0.0f);

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
   //angle = 1.1f;
   //model = glm::rotate(model, glm::radians(angle), vec3(0, angle, angle));

}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    //Set Material Uniform values
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Shininess", 180.0f);

    model = mat4(1.0f);
    //model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    mesh->render();

    //Set Material Uniform values
    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Shininess", 180.0f);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();
    ScenePlane.render();

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
    prog.setUniform("ProjectionMatrix", projection);


}
