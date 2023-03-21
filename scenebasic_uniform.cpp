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

SceneBasic_Uniform::SceneBasic_Uniform() : SceneCube(1.0f) , ScenePlane(5.0f, 5.0f, 1, 1)
{
    //ScenePlane(50.0f, 50.0f, 1, 1)
    tPrev = 0.0f;
    angle = 0.0f;
    rotSpeed = (glm::pi<float>() / 8.0f);
    //sky(100.0f)
    chest = ObjMesh::load("media/chest.obj", false, false);
    //SceneCube(1.0f)
    //SceneTeapot(14, mat4(1.0f))

}


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
    //load the texture and the model
    GLuint spotTexture = Texture::loadTexture("media/spot/spot_texture.png");
    spot = ObjMesh::load("media/spot/spot_triangulated.obj");
    projection = mat4(1.0f);
    angle = glm::radians(140.0f);
    prog.setUniform("Light.Ld", vec3(1.0f));
    prog.setUniform("Light.Ls", vec3(1.0f));
    prog.setUniform("Light.La", vec3(0.15f));
    setupFBO(); //we call the setup for our fbo
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spotTexture);
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
    angle += rotSpeed * deltaT;
    if (angle > glm::two_pi<float>())
        angle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render()
{
    //bind the buffer
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    //Render everything that doesn't want the Spot item on it;
    //glFlush();
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //renderNoSpot();
    //render to texture
    //renderToTexture();
    //flush the buffer
    glFlush();
    //unbind the write buffer and bind the default buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //render the scene using the newly written texture
    renderScene();
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
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]),vec3(mv[1]),vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ProjectionMatrix", projection);


}

void SceneBasic_Uniform::renderScene() {
    // Render all items with no spot item first
    glBindFramebuffer(GL_FRAMEBUFFER, sceneHandle);
    prog.setUniform("RenderTex", 0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 cameraPos = vec3(2.0f * cos(angle), 1.5f, 2.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.3f, 100.0f);
    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    setMatrices();
    ScenePlane.render();
    glFlush();
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    prog.setUniform("RenderTex", 1);
    glViewport(0, 0, 512, 512);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view = glm::lookAt(vec3(0.0f, 0.0f, 2.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(50.0f), 1.0f, 0.3f, 100.0f);
    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Shininess", 100.0f);
    model = mat4(1.0f);
    model = glm::rotate(model, angle, vec3(0.0f, 0.0f, 1.0f));
    setMatrices();
    spot->render();
    glFlush();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    prog.setUniform("RenderTex", 0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.3f, 100.0f);
    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    setMatrices();
    chest->render();
}


void SceneBasic_Uniform::setupFBO() {
    // Generate and bind the framebuffer
    glGenFramebuffers(1, &fboHandle);
    glGenFramebuffers(2, &sceneHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    // Create the texture object
    GLuint renderTex;
    glGenTextures(1, &renderTex);
    glActiveTexture(GL_TEXTURE0); // Use texture unit 0
    glBindTexture(GL_TEXTURE_2D, renderTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        renderTex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneHandle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        renderTex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    // Create the depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
    // Bind the depth buffer to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depthBuf);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneHandle);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depthBuf);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    // Set the targets for the fragment output variables
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result == GL_FRAMEBUFFER_COMPLETE) {
        cout << "Framebuffer is complete" << endl;
    }
    else {
        cout << "Framebuffer error: " << result << endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, sceneHandle);
    GLenum drawBuffers2[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers2);
    GLenum result2 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result2 == GL_FRAMEBUFFER_COMPLETE) {
        cout << "Framebuffer is complete" << endl;
    }
    else {
        cout << "Framebuffer error: " << result << endl;
    }
    // Unbind the framebuffer, and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//void SceneBasic_Uniform::renderToTexture() {
  //  prog.setUniform("RenderTex", 1);
   // glViewport(0, 0, 512, 512);
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // view = glm::lookAt(vec3(0.0f, 0.0f, 2.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
   // projection = glm::perspective(glm::radians(50.0f), 1.0f, 0.3f, 100.0f);
   // prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
   // prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
   // prog.setUniform("Material.Shininess", 100.0f);
   // model = mat4(1.0f);
   // model = glm::rotate(model, angle, vec3(0.0f, 0.0f, 1.0f));
   // setMatrices();
   // spot->render();
//}
