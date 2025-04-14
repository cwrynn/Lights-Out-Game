//
// Created by Tyler Sheehan on 4/8/25.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <memory>
#include <GLFW/glfw3.h>

#include "shader/shaderManager.h"
#include "shapes/rect.h"
#include "shapes/shape.h"
#include "font/fontRenderer.h"

using std::vector, std::unique_ptr, std::make_unique, glm::ortho, glm::mat4, glm::vec3, glm::vec4;

class Engine {
    GLFWwindow* window{};

    const unsigned int width = 800, height = 800;

    bool keys[1024];

    unique_ptr<ShaderManager> shaderManager;
    Shader shapeShader;
    Shader textShader;

    vector<unique_ptr<Shape>> shapes;
    vector<unique_ptr<Shape>> borders;

    double MouseX, MouseY;

    bool leftButtonPressedLastFrame = false;

    bool win = false;

    unique_ptr<FontRenderer> fontRenderer;

    //grid parameters
    int columns = 5;
    int rows = 5;
    float spacing = 120.0f;

    float gridWidth = (columns - 1) * spacing;
    float gridHeight = (rows - 1) * spacing;

    float startX = (width / 2.0f) - (gridWidth / 2.0f);
    float startY = (height / 2.0f) - (gridHeight / 2.0f);

    float timeStarted = 0.0f;
    float timeWon = 0.0f;
    int numOfClicks = 0;

public:
    Engine();

    ~Engine();

    unsigned int initWindow(bool debug = false);

    void initShaders();

    void initShapes();

    void randomSquares();

    void toggleSquareColor(int row, int column);

    void toggleColor(int row, int column);

    void processInput();

    void update();

    void render();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool shouldClose();

    mat4 PROJECTION = ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);

};



#endif //ENGINE_H
