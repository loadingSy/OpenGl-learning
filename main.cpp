#include "glm/glm/ext/vector_float3.hpp"
#include "imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "glad/glad.h"
#include "glfw/include/GLFW/glfw3.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <math.h>
#include <string.h>
#include <string>
#include "imgui/imgui.h"
#include "include/Camera.hpp"
#include "include/EBO.hpp"
#include "include/FilePaths.hpp"
#include "include/FileReader.hpp"
#include "include/Mesh.hpp"
#include "include/Shader.hpp"
#include "include/Vector.hpp"
#include "stb/stb_image.h"
#include "include/Image.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vector.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "Object.hpp"




int mainWinWidth = 0;
int mainWinHeight = 0;

//glfw
void windowScaleChanged(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwInputPolling(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);

Camera mainCam;

vector3 inputVector(0,0,0);
vector2 mouseInput(0,0);

int main()
{
    //setting up glfw
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //main window
    GLFWwindow* mainWin = glfwCreateWindow(800, 800, "Open gl", NULL, NULL);
    if(mainWin == NULL)
    {
        std::cout << "Main Win failed to be initialized";
        return -1;
    }

    glfwGetWindowSize(mainWin, &mainWinWidth, &mainWinHeight);

    glfwMakeContextCurrent(mainWin);



    //glad loading
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD";
        return -1;
    }

    //open gl loading
    glViewport(0, 0, 800,800);

    //glfw callbacks
    glfwSetWindowSizeCallback(mainWin, windowScaleChanged);
    glfwSetKeyCallback(mainWin, key_callback);
    
    glfwSetCursorPosCallback(mainWin, mouse_callback);


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(mainWin, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    
    //rendering data
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,  1.0f, 0.2f, 0.2f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.1f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.9f, 0.2f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.9f, 0.3f, 0.5f,  0.0f, 1.0f,

        0.5f, -0.5f, -0.5f,  0.1f, 0.8f, 0.8f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.1f, 0.4f, 0.9f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.3f, 0.2f, 0.9f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.2f, 0.9f, 0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,  0.2f, 0.9f, 0.3f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.6f, 1.0f, 0.2f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.1f, 0.7f, 0.6f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 0.8f,  0.0f, 1.0f,

        0.5f, -0.5f,  0.5f,  0.9f, 0.2f, 0.7f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.6f, 0.1f, 0.9f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.9f, 0.4f, 0.9f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.3f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  0.3f, 0.7f, 1.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.5f, 0.9f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.8f, 0.6f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.6f, 0.4f, 1.0f,  0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,  0.9f, 0.7f, 0.2f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.8f, 0.4f, 0.1f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.7f, 0.9f, 0.3f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.5f, 0.8f, 0.2f,  0.0f, 1.0f
    };

    GLuint indices[] = {
        0,  1,  2,   0,  2,  3,
        4,  5,  6,   4,  6,  7,
        8,  9, 10,   8, 10, 11,
        12, 13, 14,  12, 14, 15,
        16, 17, 18,  16, 18, 19,
        20, 21, 22,  20, 22, 23
    };
    //model handling
    Image img1(getProjectPath("assets/textures/toms.jpg"), 4);
    img1.bind(0);

    Image img2(getProjectPath("assets/textures/basboos.jpg"), 4);
    img2.bind(1);



    Object box1O(
        Mesh(
            Shader(getProjectPath("assets/defaultVertex.glsl"),getProjectPath("assets/defaultFragment.glsl")),
            VAO(),
            VBO(vertices, sizeof(vertices)),
            EBO(indices, sizeof(indices))
        )
    );
    
    Object box2O(
        Mesh(
            vertices, sizeof(vertices), indices, sizeof(indices),
            getProjectPath("assets/defaultVertex.glsl"),
            getProjectPath("assets/defaultFragment.glsl")
        )
    );

    box2O.mesh.linkAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    box2O.mesh.linkAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
    box2O.mesh.linkAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float)));

    box1O.mesh.linkAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    box1O.mesh.linkAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
    box1O.mesh.linkAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float)));


    glClearColor(0.1,0.1,0.1, 0);

    glUseProgram(box1O.mesh.shaderId);
    GLint scaleUniform = box1O.mesh.getUniformLocation("scale");
    GLint img1Uniform = box1O.mesh.getUniformLocation("ourTexture");
    GLint modelUniform = box1O.mesh.getUniformLocation("model");
    GLint viewUniform = box1O.mesh.getUniformLocation("view");
    GLint projectionUniform =  box1O.mesh.getUniformLocation("projection");

    glUseProgram(box2O.mesh.shaderId);
    GLint scaleUniform2 = box2O.mesh.getUniformLocation("scale");
    GLint img1Uniform2 = box2O.mesh.getUniformLocation("ourTexture");
    GLint modelUniform2 = box2O.mesh.getUniformLocation("model");
    GLint viewUniform2 = box2O.mesh.getUniformLocation("view");
    GLint projectionUniform2 =  box2O.mesh.getUniformLocation("projection");


    glm::mat4 model(1.0f);
    glm::mat4 model2(1.0f);
    glm::mat4 projection(1.0f);

    mainCam.updateCameraVectors();
    mainCam.changePos(vector3(0,0,-3));

    glEnable(GL_DEPTH_TEST);

    float rotation[3] = {0.0f,0.0f,0.0f};
    float scale = 1;
    bool renderBox = true;

    projection = glm::perspective(glm::radians(45.0f), (float)mainWinWidth/mainWinHeight, 0.01f, 10000.0f);

    glUseProgram(box1O.mesh.shaderId);
    glUniform1i(img1Uniform, 0);
    glUniform1f(scaleUniform, scale);

    glUseProgram(box2O.mesh.shaderId);
    glUniform1i(img1Uniform2, 1);
    glUniform1f(scaleUniform2, scale);

    GLfloat lastTime = 0;
    float deltaTime = 0;


    model2 = glm::translate(model2, glm::vec3(1,1,0));

    //main loop
    while (!glfwWindowShouldClose(mainWin)) 
    {
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        glfwInputPolling(mainWin);  

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::Begin("Debug");
        
        model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0,0,1));
        
        model2 = glm::rotate(model2, glm::radians(-rotation[0]), glm::vec3(1,0,0));
        model2 = glm::rotate(model2, glm::radians(-rotation[1]), glm::vec3(0,1,0));
        model2 = glm::rotate(model2, glm::radians(-rotation[2]), glm::vec3(0,0,1));
        

        //camera 
        mainCam.rotate(mouseInput.x, mouseInput.y);
        mainCam.move(inputVector*deltaTime);
        projection = glm::perspective(glm::radians(45.0f), (float)mainWinWidth/mainWinHeight, 0.01f, 10000.0f);

        img1.bind(0);
        glUseProgram(box1O.mesh.shaderId);
        glBindVertexArray(box1O.mesh.vaoId);
        // glBindBuffer(GL_INDEX_ARRAY, box1O.mesh.eboId);
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(mainCam.getMatrix()));
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(scaleUniform, scale);

    
        box1O.render();
        

        // if(renderBox)
        //     glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
        
        img2.bind(1);
        glUseProgram(box2O.mesh.shaderId);
        glBindVertexArray(box2O.mesh.vaoId);
        // glBindBuffer(GL_INDEX_ARRAY, box1O.mesh.eboId);
        glUniformMatrix4fv(viewUniform2, 1, GL_FALSE, glm::value_ptr(mainCam.getMatrix()));
        glUniformMatrix4fv(modelUniform2, 1, GL_FALSE, glm::value_ptr(model2));
        glUniformMatrix4fv(projectionUniform2, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(scaleUniform2, scale);
        // if(renderBox)
        //     glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
        
        box2O.render();

        //imgui debugging
        ImGui::Text("Object");
        ImGui::Checkbox("Render Box", &renderBox);
        ImGui::SliderFloat3("rotation", rotation, -3, 3);
        ImGui::SliderFloat("Scale", &scale, 0.1, 4);
        ImGui::Separator();
        ImGui::Text("Camera");
        ImGui::SliderFloat("X Sensitivity", &mainCam.sensitivity.x, 0.01f, 1.0f);
        ImGui::SliderFloat("Y Sensitivity", &mainCam.sensitivity.y, 0.01f, 1.0f);



        ImGui::Separator();
        std::string fpsCounter = "Fps: " + std::to_string(1/deltaTime);
        const char* fpsCounterC_str = fpsCounter.c_str();
        ImGui::Text("%s", fpsCounterC_str);

        ImGui::End();
        ImGui::Render();
        ImGui::UpdatePlatformWindows();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mainWin);



        mouseInput = vector2(0,0);
    }

    

    glUseProgram(0);
    glfwTerminate();
}

void windowScaleChanged(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width, height);
    glfwGetWindowSize(window, &mainWinWidth, &mainWinHeight);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_SLASH && action == GLFW_PRESS)
    {
        bool cursorLocked = glfwGetInputMode(window, GLFW_CURSOR) ==  GLFW_CURSOR_DISABLED;
        if(cursorLocked)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
    {
        mainCam.speed = 4;
    }
    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
    {
        mainCam.speed = 2;
    }
}

void glfwInputPolling(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        inputVector.z = 1;
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        inputVector.z = -1;
    else
        inputVector.z = 0;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        inputVector.x = 1;
    else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        inputVector.x = -1;
    else
        inputVector.x = 0;
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        inputVector.y = 1;
    else if(glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        inputVector.y = -1;
    else
        inputVector.y = 0;

    //camera

    // if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //     mouseInput.x = 1;
    // else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //     mouseInput.x = -1;
    // else
    //     mouseInput.x = 0;
    // if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    //     mouseInput.y = 1;
    // else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //     mouseInput.y = -1;
    // else
    //     mouseInput.y = 0;

    }


void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    static bool firstMouse = true;
    static float lastX = 0;
    static float lasyY = 0;

    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL || firstMouse)
    {
        lastX = xPos;
        lasyY = yPos;
        firstMouse = false;
        return;
    }
    mouseInput = vector2(xPos-lastX, lasyY-yPos);

    lastX = xPos;
    lasyY = yPos;
    std::cout << "\nlastY: " << lasyY << "| lastX: " << lastX << '\n';
}