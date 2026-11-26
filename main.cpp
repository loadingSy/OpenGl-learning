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
#include <memory>
#include <minwindef.h>
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



bool smoothFov = false;
float sprintFovMultiplier = 1.3f;
float targetFov = 45.0f;


template <typename T>
void lerpValue(T &current, T target, T a)
{
    current = current + (target - current) * a;
}

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

    
    float normalBoxVertices[] = {
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
    GLuint boxIndices[] = {
        0,  1,  2,   0,  2,  3,
        4,  5,  6,   4,  6,  7,
        8,  9, 10,   8, 10, 11,
        12, 13, 14,  12, 14, 15,
        16, 17, 18,  16, 18, 19,
        20, 21, 22,  20, 22, 23
    };

float skyBoxVertices[] = {
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.50f, 0.6666666666f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.50f, 1.000000f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.25f, 1.000000f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.25f, 0.6666666666f,

     1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.25f, 0.3333333333f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.25f, 0.6666666666f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.00f, 0.6666666666f,
    -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.00f, 0.3333333333f,

    -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  1.00f, 0.3333333333f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  1.00f, 0.6666666666f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.75f, 0.6666666666f,
    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.75f, 0.3333333333f,

    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.50f, 0.0000000000f,
     1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.50f, 0.3333333333f,
     1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.25f, 0.3333333333f,
    -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.25f, 0.0000000000f,

     1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.50f, 0.3333333333f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.50f, 0.6666666666f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.25f, 0.6666666666f,
     1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,  0.25f, 0.3333333333f,

    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.75f, 0.3333333333f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.75f, 0.6666666666f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.50f, 0.6666666666f,
     1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.50f, 0.3333333333f
};


std::shared_ptr<Mesh> boxPtr = std::make_shared<Mesh>(Shader(getProjectPath("assets/defaultVertex.glsl"),getProjectPath("assets/defaultFragment.glsl")),
            VAO(),
            VBO(normalBoxVertices, sizeof(normalBoxVertices)),
            EBO(boxIndices, sizeof(boxIndices))
        );

std::shared_ptr<Mesh> skyBoxPtr = std::make_shared<Mesh>(Shader(getProjectPath("assets/defaultVertex.glsl"),getProjectPath("assets/defaultFragment.glsl")),
            VAO(),
            VBO(skyBoxVertices, sizeof(skyBoxVertices)),
            EBO(boxIndices, sizeof(boxIndices))
        );

    Object boxObjects[4] = 
    {
        Object(
            boxPtr
        ),

        Object(
            boxPtr
        ),

        Object(
            boxPtr
        ),
        Object(
            skyBoxPtr
        )
    };
    
    float *boxesRotationFloats[4][3];
    float *boxesPositionFloats[4][3];
    float *boxesScalesFloats[4][3];
    std::string imguiBoxesHeaderNames[4];

    Image boxesImages[4] = 
    {
        Image(getProjectPath("assets/textures/toms.jpg"), 4),
        Image(getProjectPath("assets/textures/basboos.jpg"), 4),
        Image(getProjectPath("assets/textures/qalbi.jpg"), 4),
        Image(getProjectPath("assets/textures/skyBox.png"), 4, GL_NEAREST)
    };


    for(int i = 0; i < std::size(boxObjects); i++)
    {
        Object *box = &boxObjects[i];
        box->mesh->linkAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
        box->mesh->linkAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
        box->mesh->linkAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float)));
        
        boxesPositionFloats[i][0] = &box->position.x;
        boxesPositionFloats[i][1] = &box->position.y;
        boxesPositionFloats[i][2] = &box->position.z;

        boxesRotationFloats[i][0] = &box->rotation.x;
        boxesRotationFloats[i][1] = &box->rotation.y;
        boxesRotationFloats[i][2] = &box->rotation.z;

        boxesScalesFloats[i][0] = &box->scale.x;
        boxesScalesFloats[i][1] = &box->scale.y;
        boxesScalesFloats[i][2] = &box->scale.z;

        boxObjects[i].setUniform("imgId", 0, true);
        boxObjects[i].mesh->getShader()->unUse();

        boxesImages[i].bind(GL_TEXTURE0 + i);

        boxObjects[i].changePos(vector3(0, i*2, 0));

    }
    //View Setup
    glm::mat4 projection(1.0f);
    float fov = 45.0f;

    mainCam.updateCameraVectors();
    mainCam.changePos(vector3(-6,0,0));
    mainCam.changeRotation(90,0);

    projection = glm::perspective(glm::radians(45.0f), (float)mainWinWidth/mainWinHeight, 0.01f, 10000.0f);


    bool renderBox = true;
    
    GLdouble lastTime = 0;
    GLdouble deltaTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1,0.1,0.1, 0);


    for(int i = 0; i < std::size(boxObjects); i++)
        {
            const char *boxNamePrefix = "Box ";
            imguiBoxesHeaderNames[i] = std::string(boxNamePrefix) + std::to_string(i+1);
        }


        *boxesScalesFloats[3][0] = 200;
        *boxesScalesFloats[3][1] = 200;
        *boxesScalesFloats[3][2] = 200;
    
    while (!glfwWindowShouldClose(mainWin)) 
    {
        //DT calculation
        bool newSecond = false;
        static int lastSecond = glfwGetTime();
        

        if(lastSecond != static_cast<int>(glfwGetTime()))
        {
            newSecond = true;
            lastSecond = static_cast<int>(glfwGetTime());
        }

        double time = glfwGetTime();
        deltaTime = time - lastTime;
        
        lastTime = time;

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
        ImGui::Begin("Editor");

        //Camera 
        mainCam.rotate(mouseInput.x, mouseInput.y);
        mainCam.move(inputVector*deltaTime);
        
        if(smoothFov)
        {
            lerpValue(fov, targetFov, 0.05f);
            if(fov == targetFov)
                smoothFov = false;
        }

        projection = glm::perspective(glm::radians(fov), (float)mainWinWidth/mainWinHeight, 0.01f, 10000.0f);

        
            

        //Imgui Ui
        static std::string fpsCounter = "Fps: " + std::to_string(1/deltaTime);
        static const char* fpsCounterC_str = fpsCounter.c_str();

        if(newSecond)
        {
            fpsCounter = "Fps: " + std::to_string(1/deltaTime);
            fpsCounterC_str = fpsCounter.c_str();
        }

        for(int i = 0; i < std::size(boxObjects); i++)
        {
            //Rendering
            if(renderBox)
            {
                boxesImages[i].bind(i);
                glUseProgram(boxObjects[i].mesh->shaderId);
                glBindVertexArray(boxObjects[i].mesh->vaoId);
                boxObjects[i].setUniform("textureId", i);
                boxObjects[i].render(mainCam.getMatrix(), projection);
            }

            ImGui::PushID(i); 
            if (ImGui::CollapsingHeader(imguiBoxesHeaderNames[i].c_str()))
            {
                ImGui::SliderFloat3("Position", *boxesPositionFloats[i], -5, 5);
                ImGui::SliderFloat3("Rotation", *boxesRotationFloats[i], -360, 360);
                ImGui::SliderFloat3("Scale", *boxesScalesFloats[i], 0.1, 4);
            } 

            ImGui::PopID();
        }

        ImGui::Checkbox("Render Box", &renderBox);
        ImGui::Separator();
        if (ImGui::CollapsingHeader("Camera")) 
        {
            ImGui::Text("Camera");
            ImGui::SliderFloat("X Sensitivity", &mainCam.sensitivity.x, 0.01f, 1.0f);
            ImGui::SliderFloat("Y Sensitivity", &mainCam.sensitivity.y, 0.01f, 1.0f);
            ImGui::SliderFloat("Fov", &fov, 5, 120);
            ImGui::Separator();
            ImGui::Text("%s", fpsCounterC_str);
        }
        
        
        
        
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
        mainCam.speed = 10;
        smoothFov = true;
        targetFov *= sprintFovMultiplier;
    }
    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
    {
        mainCam.speed = 4;
        smoothFov = true;
        targetFov /= sprintFovMultiplier;
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