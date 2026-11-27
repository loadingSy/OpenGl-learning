#include "imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "glad/glad.h"
#include "glfw/include/GLFW/glfw3.h"
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <math.h>
#include <memory>
#include <minwindef.h>
#include <string.h>
#include <string>
#include "imgui/imgui.h"
#include "include/Box.hpp"
#include "include/Camera.hpp"
#include "include/EBO.hpp"
#include "include/FilePaths.hpp"
#include "include/FileReader.hpp"
#include "include/Mesh.hpp"
#include "include/Object.hpp"
#include "include/Shader.hpp"
#include "include/VAO.hpp"
#include "include/VBO.hpp"
#include "include/Vector.hpp"
#include "stb/stb_image.h"
#include "include/Image.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Vector.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "Object.hpp"
#include "Box.hpp"
#include "FileReader.hpp"
#define DEBUG_UNIFORM_LOGS







template <typename T>
void lerpValue(T &current, T target, T a);
void linkFloatAttributes(std::shared_ptr<Mesh> mesh , std::initializer_list<int> attribs);
void linkFloatAttributes(std::shared_ptr<Mesh> mesh , std::vector<int> attribs);
Object createCubeObject(std::shared_ptr<Mesh> mesh, std::initializer_list<int> attribs);


//glfw
void windowScaleChanged(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwInputPolling(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);


//global values
int mainWinHeight = 0;
int mainWinWidth = 0;

bool smoothFov = false;
float sprintFovMultiplier = 1.3f;
float targetFov = 45.0f;

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


std::vector<GLuint> boxIndices = static_cast<std::vector<GLuint>>(readFileToVector<unsigned int>(getProjectPath("assets/data/boxIndices.txt"), ','));

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

float lightSourceBoxVertices[] = 
{
    +1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,

     +1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f,

    +1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,

    -1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
    +1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,

};

float boxWithNormal[] = {
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f, -1.0f, 

    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,

    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,

    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f
};

std::vector<int> testVec = readFileToVector<int>(getProjectPath("assets/data/test.txt"), ',');

std::cout << '\n';
for(int i = 0; i < testVec.size(); i++)
{
    std::cout << testVec[i] << '\n';
}

std::shared_ptr<Mesh> boxPtr = std::make_shared<Mesh>(Shader(getProjectPath("assets/Shaders/unlit/defaultVertex.glsl"),getProjectPath("assets/Shaders/unlit/defaultFragment.glsl")),
            VAO(),
            VBO(normalBoxVertices, sizeof(normalBoxVertices)),
            EBO(boxIndices.data(), boxIndices.size()*sizeof(GLuint))
        );

std::shared_ptr<Mesh> skyBoxPtr = std::make_shared<Mesh>(Shader(getProjectPath("assets/Shaders/unlit/defaultVertex.glsl"),getProjectPath("assets/Shaders/unlit/defaultFragment.glsl")),
            VAO(),
            VBO(skyBoxVertices, sizeof(skyBoxVertices)),
            EBO(boxIndices.data(), boxIndices.size()*sizeof(GLuint))
        );
std::shared_ptr<Mesh> lightSourcePtr = std::make_shared<Mesh>(Shader(getProjectPath("assets/Shaders/unlit/defaultVertex.glsl"),getProjectPath("assets/Shaders/unlit/defaultFragment.glsl")),
            VAO(),
            VBO(lightSourceBoxVertices, sizeof(lightSourceBoxVertices)),
            EBO(boxIndices.data(), boxIndices.size()*sizeof(GLuint))
        );
std::shared_ptr<Mesh> litBoxPtr = std::make_shared<Mesh>(Shader(getProjectPath("assets/Shaders/unlit/defaultVertex.glsl"),getProjectPath("assets/Shaders/lit/litFragment.glsl")),
            VAO(),
            VBO(normalBoxVertices, sizeof(normalBoxVertices)),
            EBO(boxIndices.data(), boxIndices.size()*sizeof(GLuint))
        );

#define BOXESCOUNT 6
    Box boxes[BOXESCOUNT] = 
    {
        Box(boxPtr, {3,3,2}, Image(getProjectPath("assets/textures/toms.jpg"), 4)),
        Box(boxPtr, {3,3,2}, Image(getProjectPath("assets/textures/basboos.jpg"), 4)),
        Box(boxPtr, {3,3,2}, Image(getProjectPath("assets/textures/qalbi.jpg"), 4)),
        Box(skyBoxPtr, {3,3,2}, Image(getProjectPath("assets/textures/skyBox.png"), 4, GL_NEAREST)),
        Box(lightSourcePtr, {3, 3}),
        Box(litBoxPtr, {3,3,2}, Image(getProjectPath("assets/textures/bigBasboos.jpg"), 4))

    };
    boxes[4].name = "Light Source";
    boxes[5].name = "Lit box";
    boxes[3].name = "Sky Box";
    boxes[4].obj.changePos(vector3(-5.0f,2.0f, 0.0f));
    boxes[5].isLit = true;
    std::string imguiBoxesHeaderNames[BOXESCOUNT];

    for(int i = 0; i < std::size(boxes); i++)
    {


        linkFloatAttributes(boxes[i].obj.mesh, boxes[i].attribsList);

        //image
        if(boxes[i].hasImage)
        {
            boxes[i].obj.setUniform("imgId", GL_TEXTURE0+i, true);
            boxes[i].image.bind(GL_TEXTURE0 + i);
        }


        boxes[i].obj.changePos(vector3(0, i*2, 0));

    }

    boxes[3].obj.scale = {200,200,200};

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


    for(int i = 0; i < std::size(boxes); i++)
        {
            const char *boxNamePrefix = boxes[i].name != "" ? boxes[i].name.c_str() : "Box ";
            imguiBoxesHeaderNames[i] = std::string(boxNamePrefix) + std::to_string(i+1);
        }

    
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

        for(int i = 0; i < std::size(boxes); i++)
        {
            Box *curBox = &boxes[i];
            //Rendering
            if(renderBox)
            {
                boxes[i].image.bind(GL_TEXTURE0 + i);
                glUseProgram(curBox->obj.mesh->shaderId);
                glBindVertexArray(curBox->obj.mesh->vaoId);
                if(curBox->hasImage)
                    curBox->obj.setUniform("textureId", GL_TEXTURE0+i);

                curBox->render(mainCam.getMatrix(), projection);
            }
            ImGui::PushID(i); 
            if (ImGui::CollapsingHeader(imguiBoxesHeaderNames[i].c_str()))
            {
                ImGui::SliderFloat3("Position", &curBox->obj.position.x, -5, 5);
                ImGui::SliderFloat3("Rotation", &curBox->obj.rotation.x, -360, 360);
                ImGui::SliderFloat3("Scale", &curBox->obj.scale.x, 0.1, 4);
                ImGui::Checkbox("Render Image", &curBox->hasImage);
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


void linkFloatAttributes(std::shared_ptr<Mesh> mesh , std::initializer_list<int> attribs)
{
    size_t attribsSize = attribs.size();
    int overall = 0;
    for(int n = 0; n < attribsSize; n++)
        overall += attribs.begin()[n];

    int untillNow = 0;
    for(int n = 0; n < attribsSize; n++)
    {
        mesh->linkAttribute(n, attribs.begin()[n], GL_FLOAT, overall * sizeof(float), (void*)(untillNow *sizeof(float)));
        untillNow += attribs.begin()[n];
    }
}

void linkFloatAttributes(std::shared_ptr<Mesh> mesh , std::vector<int> attribs)
{
    size_t attribsSize = attribs.size();
    int overall = 0;
    for(int n = 0; n < attribsSize; n++)
        overall += attribs[n];

    int untillNow = 0;
    for(int n = 0; n < attribsSize; n++)
    {
        mesh->linkAttribute(n, attribs[n], GL_FLOAT, overall * sizeof(float), (void*)(untillNow *sizeof(float)));
        untillNow += attribs[n];
    }
}

Object createCubeObject(std::shared_ptr<Mesh> mesh, std::initializer_list<int> attribs)
{
    Object obj(mesh, vector3(0), vector3(0), vector3(1));
    linkFloatAttributes(mesh, attribs);
    return obj;
}

template <typename T>
void lerpValue(T &current, T target, T a)
{
    current = current + (target - current) * a;
}