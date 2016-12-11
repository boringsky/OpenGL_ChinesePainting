//
//  main.cpp
//  ModelTest
//
//  Created by Sky on 5/4/16.
//  Copyright (c) 2016 Sky. All rights reserved.
//

// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

#define random(x) (rand()%x)

// Properties
GLuint screenWidth = 800*2, screenHeight = 600*2;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void genTexCoordOffsets(GLuint width, GLuint height, GLfloat step);

// Framebuffer
GLuint loadTexture(GLchar* path, GLboolean alpha = false);
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;



GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


// Set up texture sampling offset storage
const GLint tcOffsetColumns = 5;
const GLint tcOffsetRows    = 5;
GLfloat	texCoordOffsets[tcOffsetColumns * tcOffsetRows * 2];


// Light attributes
glm::vec3 lightPos(0.f, -5.f, 0.5f);
//glm::vec3 lightPos(0.f, 5.f, 5.f);


// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// make macos happy! \(^ 3 ^ )/
    
    GLFWwindow* window = glfwCreateWindow(screenWidth/2, screenHeight/2, "LearnOpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);
    
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();
    
    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);
    
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    
    //    glDepthFunc(GL_LEQUAL);
    
    //Open the culling
    //        glEnable(GL_CULL_FACE);
    //        glCullFace(GL_BACK);
    //        glFrontFace(GL_CW);
    
    
    
    
    
    
    //SHOW SHADER
    //动物内部颜色
    Shader shader("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/model_loading.vs", "/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/model_loading.frag");
                  
//                  , "/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/model_loading.gs");
    
    //NORMAL SHADER
    //动物边缘线着色
    Shader normalShader("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/model_normal.vs", "/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/model_normal.frag","/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/model_normal.gs");
    
    //DEPTH SHADER
    //深度贴图着色器
    Shader depthShader("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/shadow_mapping_depth.vs", "/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/shadow_mapping_depth.frag");
    
    //FRAMEBUFFER SHADER
    //第一层 只用于内部着色的 做多用15*15模糊
    Shader frameShader("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/screen.vs", "/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/screen.frag");
    
    //FRAMEBUFFER SHADER2
    //第二层 用于median filter 边缘
    Shader frameShader2("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/screen2.vs", "/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/screen2.frag");
    
    //FRAMEBUFFER SHADER3 FOR INNER
    //最后一层 外面的 稍微ker一下
    Shader frameShader3("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/screen3_in.vs", "/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/screen3_in.frag");
    
    
    
    
    //SHOW SHADER
    //地形内部颜色
    Shader montshader("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/mont_loading.vs", "/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/mont_loading.frag");
    
    //NORMAL SHADER
    //地形边缘线着色
    Shader montnormalShader("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/mont_normal.vs", "/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/mont_normal.frag","/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/mont_normal.gs");
    
    
    
    
    
    
    
    //For the final window effect
    GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // Positions   // TexCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        
        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    // Setup screen VAO
    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
    
    
    
    
    
    
    
    
    
    //     Load models
    
    //    Model ourModel("/Users/apple/Downloads/nanosuit/nanosuit.obj");
    
    //    Model ourModel("/Users/apple/Documents/maya/projects/default/scenes/PENRU.obj");
    
    //    Model ourModel("/Users/apple/Downloads/ConsoleApplication3/tails/Tails.obj");
    
    //        Model ourModel("/Users/apple/Documents/maya/projects/default/scenes/ANT.obj");
    
    //        Model ourModel("/Users/apple/Documents/maya/projects/default/scenes/CAMEL.obj");
    
    //    Model ourModel("/Users/apple/Documents/maya/projects/default/scenes/CAMEL2.obj");
    
        Model ourModel("/Users/apple/Documents/maya/projects/default/scenes/sparrow.obj");
    
//    Model ourModel("/Users/apple/Documents/maya/projects/default/scenes/spr.obj");
    
//    Model ourModel2("/Users/apple/Documents/maya/projects/default/scenes/mont.obj");
//    
//    Model ourModel3("/Users/apple/Documents/maya/projects/default/scenes/trr.obj");
    
    
    //    Model ourModel("/Users/apple/Documents/maya/projects/default/scenes/caml.obj");
    //
    //    Model ourModel2("/Users/apple/Documents/maya/projects/default/scenes/mont2.obj");
    
    
    
    
    
    
    
    
    // Set projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)screenWidth/(GLfloat)screenHeight, 1.0f, 100.0f);
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // Also one for normal shader
    normalShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(normalShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    depthShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    montnormalShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(normalShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    
    
    
    
    
    
    
    normalShader.Use();
    glUniform1i(glGetUniformLocation(normalShader.Program, "shadowMap"), 0);
    shader.Use();
    glUniform1i(glGetUniformLocation(shader.Program, "shadowMap"), 0);
    montnormalShader.Use();
    glUniform1i(glGetUniformLocation(normalShader.Program, "shadowMap"), 0);
    montshader.Use();
    glUniform1i(glGetUniformLocation(normalShader.Program, "shadowMap"), 0);
    
    
    
    
    
    
    
    
    
    // Framebuffers
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // Create a color attachment texture
    GLuint textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // Use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
    // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    
    
    // Framebuffers
    GLuint framebuffer2;
    glGenFramebuffers(1, &framebuffer2);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
    // Create a color attachment texture
    GLuint textureColorbuffer2;
    glGenTextures(1, &textureColorbuffer2);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2, 0);
    // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    GLuint rbo2;
    glGenRenderbuffers(1, &rbo2);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo2);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // Use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo2); // Now actually attach it
    // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    
    
    
    // Framebuffers
    GLuint framebuffer3;
    glGenFramebuffers(1, &framebuffer3);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer3);
    // Create a color attachment texture
    GLuint textureColorbuffer3;
    glGenTextures(1, &textureColorbuffer3);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer3, 0);
    // Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    GLuint rbo3;
    glGenRenderbuffers(1, &rbo3);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo3);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // Use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo3); // Now actually attach it
    // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    
    
    
    
    
    // Configure depth map FBO
    
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // - Create depth texture
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    //    glDrawBuffer(GL_NONE);
    //    glReadBuffer(GL_NONE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    
    
    
    
    
    
    
    //LOAD THE STROKE PARTTTTTT
    GLuint texture1;
    // ====================
    // Texture 1
    // ====================
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/strokes/stroke4.bmp", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    
    
    
    

    
    
    //for the noise texture
    GLuint noiseTex;
    // ====================
    // Texture 1
    // ====================
    glGenTextures(1, &noiseTex);
    glBindTexture(GL_TEXTURE_2D, noiseTex); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    int width3, height3;
    unsigned char* image3 = SOIL_load_image("/Users/apple/GitHub/OpenGL_Model_Outline/OpenGL_Model_Outline/strokes/noise.jpg", &width3, &height3, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, image3);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image3);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    
    
    

    
    
    // Draw in wirefrasme
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    
    
    
    
    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        
        
        // Check and call events
        glfwPollEvents();
        Do_Movement();
    
        
        
        // For the first buffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        
        
        
        // Clear the colorbuffer
        glClearColor(1.f, 1.f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        
        
        
        
        
        
        // 1 RENDER THE DEPTH BUFFER
        glViewport(0, 0, screenWidth, screenHeight);
        // For the silhouette test
        GLfloat near_plane = 0.3f, far_plane = 7.5f;
        // - now render scene from light's point of view
        depthShader.Use();
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 dprojection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightSpaceMatrix = dprojection * view;
        // Draw the loaded model
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniformMatrix4fv(glGetUniformLocation(depthShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        ourModel.Draw(depthShader);
        
        
        
        /*Framebuffer transition*/
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        
        /*Framebuffer transition*/
        
        
        
        // 3 DRAW THE INTERIOR
        shader.Use();   // <-- Don't forget this one!
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        // Bind depth Textures
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //         Bind Textures using texture units
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        // Transformation matrices
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // Add time component to geometry shader in the form of a uniform
        glUniform1f(glGetUniformLocation(shader.Program, "time"), glfwGetTime());
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        //for the light effect
        GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
        GLint lightColorLoc  = glGetUniformLocation(shader.Program, "lightColor");
        GLint lightPosLoc    = glGetUniformLocation(shader.Program, "lightPos");
        glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
        //           glUniform3f(objectColorLoc, 0.5f, 0.5f, 0.5f);
        glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
        glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
        ourModel.Draw(shader);
        

        /*//
        ///
        /////////////////
        ///////
        ////////
        
        
        ///////
        
        
        /////////
        
//        
//        
//        // 3 DRAW THE MOUNTAIN GROUND AND SOMETHING
//        montnormalShader.Use();   // <-- Don't forget this one!
//        
//        glm::mat4 model2 = glm::mat4();
//        model2 = glm::translate(model2, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
//        model2 = glm::scale(model2, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
//        
//        
//        
//        
//        // Bind depth Textures
//        glActiveTexture(GL_TEXTURE0);
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, depthMap);
//        //         Bind Textures using texture units
//        glActiveTexture(GL_TEXTURE1);
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, texture2);
//        
//        glUniformMatrix4fv(glGetUniformLocation(montnormalShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
//        glUniformMatrix4fv(glGetUniformLocation(montnormalShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model2));
//        glUniformMatrix4fv(glGetUniformLocation(montnormalShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
//        glUniform1i(glGetUniformLocation(montnormalShader.Program, "ourTexture1"), 1);
//        glUniform2fv(glGetUniformLocation(montnormalShader.Program, "tcOffset"),50, texCoordOffsets); // Pass in 25 vec2s in our texture coordinate offset array
//        ourModel3.Draw(montnormalShader);
//        
//        
//        
//        
//        
//        
//        
//        
//        
//        
//        
//        
//        
//        
//        montshader.Use();   // <-- Don't forget this one!
//        
//        model = glm::mat4();
//        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
//        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
//        
//        
//        // Bind depth Textures
//        glActiveTexture(GL_TEXTURE0);
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, depthMap);
//        //         Bind Textures using texture units
//        
//        
//        glUniformMatrix4fv(glGetUniformLocation(montshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
//        // Transformation matrices
//        glUniformMatrix4fv(glGetUniformLocation(montshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//        glUniformMatrix4fv(glGetUniformLocation(montshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
//        // Add time component to geometry shader in the form of a uniform
//        glUniform1f(glGetUniformLocation(montshader.Program, "time"), glfwGetTime());
//        glUniformMatrix4fv(glGetUniformLocation(montshader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
//        
//        //for the light effect
//        objectColorLoc = glGetUniformLocation(montshader.Program, "objectColor");
//        lightColorLoc  = glGetUniformLocation(montshader.Program, "lightColor");
//        lightPosLoc    = glGetUniformLocation(montshader.Program, "lightPos");
//        glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
//        //           glUniform3f(objectColorLoc, 0.5f, 0.5f, 0.5f);
//        glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
//        glUniform3f(lightPosLoc,    0.0f, 1.0f, 1.0f);
//        ourModel2.Draw(montshader);
//        
//        
///*/
        
        
        /*Framebuffer transition*/
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
        /*Framebuffer transition*/
        
        
        
        // First framebuffer thing
        // Clear all relevant buffers
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
        glViewport(0, 0, screenWidth, screenHeight);
        // Draw Screen
        frameShader.Use();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, noiseTex);
        float xx = random(100);
        glUniform1i(glGetUniformLocation(frameShader.Program, "noiseTexture"), 1);
        glUniform1f(glGetUniformLocation(frameShader.Program, "xxnumber"), xx);
        //for the light effect
        GLint uQuantLevel = glGetUniformLocation(frameShader.Program, "uQuantLevel");
        GLint uWaterPower  = glGetUniformLocation(frameShader.Program, "uWaterPower");
        glUniform1f(uQuantLevel, 6.0f);
        glUniform1f(uWaterPower, 8.0f);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
  
        
        
        
        
        
        
        
        
        
    
        // 4 FRAMEBUFFER USING
        /////////////////////////////////////////////////////
        // Bind to default framebuffer again and draw the
        // quad plane with attched screen texture.
        // //////////////////////////////////////////////////
        ///////2222222222222 DRAW THE SILHOUETTE
        normalShader.Use();
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        // Bind depth Textures
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //         Bind Textures using texture units
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniformMatrix4fv(glGetUniformLocation(normalShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(normalShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(normalShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform1i(glGetUniformLocation(normalShader.Program, "ourTexture1"), 1);
        glUniform2fv(glGetUniformLocation(normalShader.Program, "tcOffset"),50, texCoordOffsets); // Pass in 25 vec2s in our texture coordinate offset array
        ourModel.Draw(normalShader);

        
        
 
        
        
        
        /*Framebuffer transition*/
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer3);
        /*Framebuffer transition*/
        
        
        // Second Framebuffer doing
        // Clear all relevant buffers
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Sert clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
        glViewport(0, 0, screenWidth, screenHeight);
        // Draw Screen
        frameShader2.Use();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);	// Use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        
        /*Framebuffer transition*/
        //        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        /*Framebuffer transition*/
        

    
        /*Framebuffer transition*/
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
        /*Framebuffer transition*/

        
        
        
        
        
        // Third Framebuffer Thing
        // Clear all relevant buffers
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
        glViewport(0, 0, screenWidth, screenHeight);
        // Draw Screen
        frameShader3.Use();
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, noiseTex);
         xx = random(100);
        glUniform1i(glGetUniformLocation(frameShader3.Program, "noiseTexture"), 1);
        glUniform1f(glGetUniformLocation(frameShader3.Program, "xxnumber"), xx);
        //for the light effect
         uQuantLevel = glGetUniformLocation(frameShader3.Program, "uQuantLevel");
         uWaterPower  = glGetUniformLocation(frameShader3.Program, "uWaterPower");
        glUniform1f(uQuantLevel, 6.0f);
        glUniform1f(uWaterPower, 8.0f);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glBindVertexArray(0);

        
        
        glBindVertexArray(0);
        
        
        
        
        /////////////////////////////////////////////////////////////////////////////////////////////
        
        /////////////////////////////////////////////////////////////////////////////////////////////
        
      
        
        
        
        
        // Swap the buffers
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}








// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
    // What enum to use?
    GLenum attachment_type = GL_RGB;
    if(!depth && !stencil)
        attachment_type = GL_RGB;
    else if(depth && !stencil)
        attachment_type = GL_DEPTH_COMPONENT;
    else if(!depth && stencil)
        attachment_type = GL_STENCIL_INDEX;
    
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    if(!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, screenWidth, screenHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
    else // Using both a stencil and depth test, needs special format arguments
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return textureID;
}

// Calculate texture coordinate offsets for kernel convolution effects
void genTexCoordOffsets(GLuint width, GLuint height, GLfloat step) // Note: Change this step value to increase the number of pixels we sample across...
{
    // Note: You can multiply the step to displace the samples further. Do this with diff values horiz and vert and you have directional blur of a sort...
    float xInc = step / (GLfloat)(screenWidth);
    float yInc = step / (GLfloat)(screenHeight);
    
    for (int i = 0; i < tcOffsetColumns; i++)
    {
        for (int j = 0; j < tcOffsetRows; j++)
        {
            texCoordOffsets[(((i*5)+j)*2)+0] = (-2.0f * xInc) + ((GLfloat)i * xInc);
            texCoordOffsets[(((i*5)+j)*2)+1] = (-2.0f * yInc) + ((GLfloat)j * yInc);
        }
    }
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

#pragma endregion