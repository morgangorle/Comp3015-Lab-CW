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

SceneBasic_Uniform::SceneBasic_Uniform() : SceneTeapot(14, mat4(1.0f)) , SceneSphere(2.0f,50,50), ScenePlane(20.0f, 50.0f, 1, 1)
{
    //ScenePlane(50.0f, 50.0f, 1, 1)
    tPrev = 0.0f;
    angle = 0.0f;
    rotSpeed = (glm::pi<float>() / 8.0f);
    //sky(100.0f)
    //ogre = ObjMesh::load("media/bs_ears.obj", false, true);
    //SceneCube(1.0f)
    //SceneTeapot(14, mat4(1.0f))

}

void SceneBasic_Uniform::initScene()
{
    compile();
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    projection = mat4(1.0f);
    angle = glm::pi<float>() / 4.0f;
    setupFBO();
    //Set up lighting uniforms
    vec3 intense = vec3(5.0f);
    prog.setUniform("Lights[0].L", intense);
    prog.setUniform("Lights[1].L", intense);
    prog.setUniform("Lights[2].L", intense);
    intense = vec3(0.2f);
    prog.setUniform("Lights[0].La", intense);
    prog.setUniform("Lights[1].La", intense);
    prog.setUniform("Lights[2].La", intense);
    // Array for full-screen quad
    GLfloat verts[] = {
    -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    GLfloat tc[] = {
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    // Set up the buffers
    unsigned int handle[2];
    glGenBuffers(2, handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);
    // Set up the vertex array object
    glGenVertexArrays(1, &quad);
    glBindVertexArray(quad);
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0); // Vertex position
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2); // Texture coordinates
    glBindVertexArray(0);
    //prog.setUniform("Light.L", vec3(1.0f));
    //prog.setUniform("Light.La", vec3(0.2f));
    float weights[5], sum, sigma2 = 8.0f;
    // Compute and sum the weights
    weights[0] = gauss(0, sigma2);
    sum = weights[0];
    for (int i = 1; i < 5; i++) {
        weights[i] = gauss(float(i), sigma2);
        sum += 2 * weights[i];
    }
    // Normalize the weights and set the uniform
    for (int i = 0; i < 5; i++) {
        std::stringstream uniName;
        uniName << "Weight[" << i << "]";
        float val = weights[i] / sum;
        prog.setUniform(uniName.str().c_str(), val);
    }
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

void SceneBasic_Uniform::pass1()
{
    prog.setUniform("Pass", 1);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    view = glm::lookAt(vec3(2.0f, 0.0f, 14.0f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)width / height,
        0.3f, 100.0f);
    drawScene();
}

void SceneBasic_Uniform::pass2()
{
    prog.setUniform("Pass", 2);
    // Revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    view = mat4(1.0);
    model = mat4(1.0);
    projection = mat4(1.0);
    setMatrices();
    // Render the quad
    glBindVertexArray(quad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneBasic_Uniform::render()
{
    pass1();

    computeLogAveLuminance();
    pass2();;
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


void SceneBasic_Uniform::setupFBO(){
    GLuint depthBuf;
    // Create and bind the FBO
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    // The depth buffer
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    // The HDR color buffer
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &hdrTex);
    glBindTexture(GL_TEXTURE_2D, hdrTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Attach the images to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depthBuf);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        hdrTex, 0);
    GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(2, drawBuffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float SceneBasic_Uniform::gauss(float x, float sigma2)
{
    double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
    double expon = -(x * x) / (2.0 * sigma2);
    return (float)(coeff * exp(expon));
}

void SceneBasic_Uniform::computeLogAveLuminance()
{
    int size = width * height;
    std::vector<GLfloat> texData(size * 3);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTex);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData.data());
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        float lum = glm::dot(vec3(texData[i * 3 + 0], texData[i * 3 + 1],
            texData[i * 3 + 2]),
            vec3(0.2126f, 0.7152f, 0.0722f));
        sum += logf(lum + 0.00001f);
    }
    prog.setUniform("AveLum", expf(sum / size));
}

void SceneBasic_Uniform::drawScene()
{
    vec3 intense = vec3(1.0f);
    prog.setUniform("Lights[0].L", intense);
    prog.setUniform("Lights[1].L", intense);
    prog.setUniform("Lights[2].L", intense);
    vec4 lightPos = vec4(0.0f, 4.0f, 2.5f, 1.0f);
    lightPos.x = -7.0f;
    prog.setUniform("Lights[0].Position", view * lightPos);
    lightPos.x = 0.0f;
    prog.setUniform("Lights[1].Position", view * lightPos);
    lightPos.x = 7.0f;
    prog.setUniform("Lights[2].Position", view * lightPos);
    prog.setUniform("Material.Kd", 0.9f, 0.3f, 0.2f);
    prog.setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Shininess", 100.0f);
    // The backdrop plane
    model = glm::rotate(mat4(1.0f), glm::radians(90.0f), vec3(1.0f, 0.0f,
        0.0f));
    setMatrices();
    ScenePlane.render();
    // The bottom plane
    model = glm::translate(mat4(1.0f), vec3(0.0f, -5.0f, 0.0f));
    setMatrices();
    ScenePlane.render();
    // Top plane
    model = glm::translate(mat4(1.0f), vec3(0.0f, 5.0f, 0.0f));
    model = glm::rotate(model, glm::radians(180.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    ScenePlane.render();
    prog.setUniform("Material.Kd", vec3(0.4f, 0.9f, 0.4f));
    model = glm::translate(mat4(1.0f), vec3(-3.0f, -3.0f, 2.0f));
    setMatrices();
    SceneSphere.render();
    prog.setUniform("Material.Kd", vec3(0.4f, 0.4f, 0.9f));
    model = glm::translate(mat4(1.0f), vec3(3.0f, -5.0f, 1.5f));
    //model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    SceneTeapot.render();
}