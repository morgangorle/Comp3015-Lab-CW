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

#include "helper/glutils.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
GLfloat angle = 0.0f;

SceneBasic_Uniform::SceneBasic_Uniform()
{
    //ScenePlane(50.0f, 50.0f, 1, 1)
    tPrev = 0;
    ogre = ObjMesh::load("media/bs_ears.obj", false, true);
    //SceneCube(1.0f)

}



void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
    view = glm::lookAt(vec3(-1.0f, 0.25f, 2.0f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);
    prog.setUniform("Light.La", vec3(0.5f));
    prog.setUniform("Light.Ld", vec3(0.9f));
    prog.setUniform("Light.Ls", vec3(0.9f));

    GLuint ColorTex =
        Texture::loadTexture("../Comp3015-Lab-Work/media/texture/ogre_diffuse.png");
    GLuint NormalMapTex =
        Texture::loadTexture("../Comp3015-Lab-Work/media/texture/ogre_normalmap.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ColorTex);
    //CHANGE ACTIVE TEXTURE
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, NormalMapTex);
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

void SceneBasic_Uniform::update(float t)
{
    float deltaT = t - tPrev;
    if (tPrev == 0.0f)
        deltaT = 0.0f;
    tPrev = t;
    angle += 0.25f * deltaT;
    if (angle > glm::two_pi<float>())
        angle -= glm::two_pi<float>();
}
//old lovely rotate method
//model = glm::rotate(model, glm::radians(angle), vec3(0, angle, angle));
void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Set up spotlight
    vec4 lightPos = vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
    prog.setUniform("Light.Position", view * lightPos);
    //mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    //prog.setUniform("Light.Direction", normalMatrix * vec3(-lightPos));
    
    //Set material uniforms for cube
    prog.setUniform("Material.Kd", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ks", 0.05f, 0.05f, 0.05f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 100.0f);
    //Handle positioning of cube
    model = mat4(1.0f);
    //model = glm::translate(model, vec3(dist* 0.6f - 1.0f, 7.5f, -dist));
    //model = glm::rotate(model, glm::radians(45.0f), vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));
    setMatrices();
    ogre->render();

    //Set Material Unifroms for Plane
    //prog.setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
    //prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    //prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    //prog.setUniform("Material.Shininess", 180.0f);
    //model = mat4(1.0f);
    //setMatrices();
    //ScenePlane.render();
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
