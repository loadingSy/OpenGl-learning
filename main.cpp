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
#include "include/VAO.hpp"
#include "include/VBO.hpp"
#include "include/Vector.hpp"
#include "stb/stb_image.h"
#include "include/Image.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vector.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "Object.hpp"
#define DEBUG_UNIFORM_LOGS




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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWin = glfwCreateWindow(800, 800, "Open gl", NULL, NULL);
    if(mainWin == NULL)
    {
        std::cout << "Main Win failed to be initialized";
        return -1;
    }
    glfwGetWindowSize(mainWin, &mainWinWidth, &mainWinHeight);
    glfwMakeContextCurrent(mainWin);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD";
        return -1;
    }

    glViewport(0, 0, 800,800);

    glfwSetWindowSizeCallback(mainWin, windowScaleChanged);
    glfwSetKeyCallback(mainWin, key_callback);
    glfwSetCursorPosCallback(mainWin, mouse_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL(mainWin, true);
    ImGui_ImplOpenGL3_Init();

    
    float vertices[] = {
        -1.0f, -1.0f,  1.0f,  1.0f, 0.2f, 0.2f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f,  1.0f, 0.6f, 0.1f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.9f, 0.2f,  1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.9f, 0.3f, 1.0f,  0.0f, 1.0f,

        1.0f, -1.0f, -1.0f,  0.1f, 0.8f, 0.8f,  0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.1f, 0.4f, 0.9f,  1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.3f, 0.2f, 0.9f,  1.0f, 1.0f,
        1.0f,  1.0f, -1.0f,  0.2f, 0.9f, 1.0f,  0.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,  0.2f, 0.9f, 0.3f,  0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.6f, 1.0f, 0.2f,  1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  0.1f, 0.7f, 0.6f,  1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.8f,  0.0f, 1.0f,

        1.0f, -1.0f,  1.0f,  0.9f, 0.2f, 0.7f,  0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,  0.6f, 0.1f, 0.9f,  1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  0.9f, 0.4f, 0.9f,  1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.3f,  0.0f, 1.0f,

        -1.0f,  1.0f,  1.0f,  0.3f, 0.7f, 1.0f,  0.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.9f, 1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  0.8f, 0.6f, 1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  0.6f, 0.4f, 1.0f,  0.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,  0.9f, 0.7f, 0.2f,  0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,  0.8f, 0.4f, 0.1f,  1.0f, 0.0f,
        1.0f, -1.0f,  1.0f,  0.7f, 0.9f, 0.3f,  1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, 0.8f, 0.2f,  0.0f, 1.0f
    };
    GLuint indices[] = {
        0,  1,  2,   0,  2,  3,
        4,  5,  6,   4,  6,  7,
        8,  9, 10,   8, 10, 11,
        12, 13, 14,  12, 14, 15,
        16, 17, 18,  16, 18, 19,
        20, 21, 22,  20, 22, 23
    };


    //Img Setup
    Image img1(getProjectPath("assets/textures/toms.jpg"), 4);
    img1.bind(0);

    Image img2(getProjectPath("assets/textures/basboos.jpg"), 4);
    img2.bind(1);


    //Models Creation
    Object box1(
        Mesh(
            Shader(getProjectPath("assets/defaultVertex.glsl"),getProjectPath("assets/defaultFragment.glsl")),
            VAO(),
            VBO(vertices, sizeof(vertices)),
            EBO(indices, sizeof(indices))
        )
    );
    
    Object box2(
        Mesh(
            Shader(getProjectPath("assets/defaultVertex.glsl"),
            getProjectPath("assets/defaultFragment.glsl")),
            VAO(),
            VBO(vertices, sizeof(vertices)),
            EBO(indices, sizeof(indices))
        )
    );

    //Vao Linking
    box2.mesh.linkAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    box2.mesh.linkAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
    box2.mesh.linkAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float)));
    box1.mesh.linkAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    box1.mesh.linkAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
    box1.mesh.linkAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float)));

    //View Setup
    glm::mat4 model(1.0f);
    glm::mat4 model2(glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)));
    glm::mat4 projection(1.0f);

    mainCam.updateCameraVectors();
    mainCam.changePos(vector3(0,0,-6));

    projection = glm::perspective(glm::radians(45.0f), (float)mainWinWidth/mainWinHeight, 0.01f, 10000.0f);

    float rotation[3] = {0.0f,0.0f,0.0f};
    float scale = 1;
    bool renderBox = true;

    //Setting Up Starting Uniforms
    box1.setUniform("imgId", 0, true);
    box1.setUniform("scale", scale);
    box1.mesh.getShader()->unUse();
    box2.setUniform("imgId", 0, true);
    box2.setUniform("scale", scale);

    GLfloat lastTime = 0;
    GLfloat deltaTime = 0;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1,0.1,0.1, 0);

    while (!glfwWindowShouldClose(mainWin)) 
    {
        //DT calculation
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        //Open gl
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Glfw
        glfwPollEvents();
        glfwInputPolling(mainWin);  

        //Imgui Setting
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::Begin("Debug");
        
        //Models
        model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0,0,1));
        
        model2 = glm::rotate(model2, glm::radians(-rotation[0]), glm::vec3(1,0,0));
        model2 = glm::rotate(model2, glm::radians(-rotation[1]), glm::vec3(0,1,0));
        model2 = glm::rotate(model2, glm::radians(-rotation[2]), glm::vec3(0,0,1));
        

        //Camera 
        mainCam.rotate(mouseInput.x, mouseInput.y);
        mainCam.move(inputVector*deltaTime);
        projection = glm::perspective(glm::radians(45.0f), (float)mainWinWidth/mainWinHeight, 0.01f, 10000.0f);

        //Rendering
        if(renderBox)
        {
            img1.bind(0);
            glUseProgram(box1.mesh.shaderId);
            glBindVertexArray(box1.mesh.vaoId);
            box1.setUniform("view", mainCam.getMatrix());
            box1.setUniform("model", model);
            box1.setUniform("projection", projection);
            box1.setUniform("scale", scale);
            box1.render(mainCam.getMatrix(), projection);

            img2.bind(1);
            glUseProgram(box2.mesh.shaderId);
            glBindVertexArray(box2.mesh.vaoId);
            box2.setUniform("view", mainCam.getMatrix());
            box2.setUniform("model", model2);
            box2.setUniform("projection", projection);
            box2.setUniform("scale", scale);
            box2.render(mainCam.getMatrix(), projection);
        }
            

        //Imgui Ui
        std::string fpsCounter = "Fps: " + std::to_string(1/deltaTime);
        const char* fpsCounterC_str = fpsCounter.c_str();

        ImGui::Text("Object");
        ImGui::Checkbox("Render Box", &renderBox);
        ImGui::SliderFloat3("rotation", rotation, -3, 3);
        ImGui::SliderFloat("Scale", &scale, 0.1, 4);
        ImGui::Separator();
        ImGui::Text("Camera");
        ImGui::SliderFloat("X Sensitivity", &mainCam.sensitivity.x, 0.01f, 1.0f);
        ImGui::SliderFloat("Y Sensitivity", &mainCam.sensitivity.y, 0.01f, 1.0f);
        ImGui::Separator();
        ImGui::Text("%s", fpsCounterC_str);
        
        //Imgui End
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