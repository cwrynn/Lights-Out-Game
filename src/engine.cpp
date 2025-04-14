//
// Created by Tyler Sheehan on 4/8/25.
//
#include "engine.h"
#include <iostream>
#include <sstream>
#include <ctime>

Engine::Engine() : keys() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    this->initWindow();
    this->initShaders();
    this->initShapes();
    this->randomSquares();
    timeStarted = glfwGetTime();
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "Lights Out", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout <<"Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    // This defines the size of the area OpenGL should render to.
    glViewport(0, 0, width, height);
    // This enables depth testing which prevents triangles from overlapping.
    glEnable(GL_BLEND);
    // Alpha blending allows for transparent backgrounds.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert",
                                                  "../res/shaders/shape.frag",
                                                  nullptr, "shape");

    // Set uniforms that never change
    shapeShader.use().setMatrix4("projection", this->PROJECTION);

    //Configure text shader and renderer
    textShader = shaderManager->loadShader("../res/shaders/text.vert","../res/shaders/text.frag", nullptr, "text");
    fontRenderer = make_unique<FontRenderer>(shaderManager->getShader("text"),"../res/fonts/MxPlus_IBM_BIOS.ttf",24);

}

void Engine::initShapes() {
    vec2 rectSize(100.0f, 100.0f);
    vec2 borderSize(110.0f, 110.0f);

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            float x = startX + column * spacing;
            float y = startY + row * spacing;
            vec2 pos(x, y);

            shapes.push_back(make_unique<Rect>(shapeShader, pos, rectSize, color(1,1,0,1)));
            borders.push_back(make_unique<Rect>(shapeShader, pos, borderSize, color(1,0,0,0)));
        }
    }
}

void Engine::randomSquares() {
    // Initializes all squares off
    for (auto& shape : shapes) {
        shape->setColor(vec4(0.5f, 0.5f, 0.5f, 1.0f));
    }

    // Makes sure its solvable by applying moves
    int randomClicks = 10 + std::rand() % 10; // Apply 10–19 random moves
    for (int i = 0; i < randomClicks; ++i) {
        int row = std::rand() % rows;
        int column = std::rand() % columns;
        toggleColor(row, column);
    }
}

void Engine::toggleSquareColor(int row, int column) {
    // Toggle the color of the square at row and column
    int index = row * columns + column;
    auto& shape = shapes[index];

    vec4 current = shape->getColor4();
    if (current == vec4(1.0, 1.0, 0.0, 1.0)) {
        shape->setColor(vec4(0.5, 0.5, 0.5, 1.0)); // Gray
    } else {
        shape->setColor(vec4(1.0, 1.0, 0.0, 1.0)); // Yellow
    }
}

void Engine::toggleColor(int row, int column) {
    // Toggle the clicked square
    toggleSquareColor(row, column);

    // Toggle the left, right, top, and bottom squares
    if (row > 0) { // Top
        toggleSquareColor(row - 1, column);
    }
    if (row < 4) { // Bottom
        toggleSquareColor(row + 1, column);
    }
    if (column > 0) { // Left
        toggleSquareColor(row, column - 1);
    }
    if (column < 4) { // Right
        toggleSquareColor(row, column + 1);
    }
}


void Engine::processInput() {
    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    // Gets cursor position, used to click lights
    glfwGetCursorPos(window, &MouseX, &MouseY);
    MouseY = height - MouseY;

    bool leftButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    vec2 rectSize(100.0f, 100.0f);

    //handles hover
    for(int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            float x = startX + column * spacing;
            float y = startY + row * spacing;

            if(MouseX >= x - rectSize.x / 2 && MouseX <= x + rectSize.x /2 &&
                MouseY >= y - rectSize.y / 2 && MouseY <= y+ rectSize.y / 2) {
                //retrieves border
                int index = row * columns + column;
                borders[index] ->setColor(vec4(1,0,0,1));
            } else {
                // Reset this border to invisible
                int index = row * columns + column;
                borders[index]->setColor(vec4(1.0, 0.0, 0.0, 0.0));
            }
        }
    }


    //handles on click event
    if (leftButtonPressed && !leftButtonPressedLastFrame) {
        for(int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                float x = startX + column * spacing;
                float y = startY + row * spacing;

                if (MouseX >= x - rectSize.x/2 && MouseX <= x + rectSize.x/2 && MouseY >= y - rectSize.y/2 && MouseY <= y + rectSize.y/2) {
                    toggleColor(row, column);
                    numOfClicks++;
                }
            }
        }
    }
    leftButtonPressedLastFrame = leftButtonPressed;

    //win logic
    if (!win) {
        win = true;
        for (const auto& shape: shapes) {
            if(shape-> getColor4() == vec4(1.0f,1.0f,0.0f,1.0f)) {
                win = false;
                break;
            }
        }
        if (win)
        {
            timeWon = glfwGetTime() - timeStarted;
        }
    }
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;


    // This function polls for events like keyboard input and mouse movement
    // It needs to be called every frame
    // Without this function, the window will freeze and become unresponsive
    glfwPollEvents();
}

void Engine::render() {
    glClearColor(0,0,0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const unique_ptr<Shape>& b : borders) {
        b->setUniforms();
        b->draw();
    }
    for (const unique_ptr<Shape>& s : shapes) {
        s->setUniforms();
        s->draw();
    }

    if(win) {
        fontRenderer->renderText("You Win!", width / 2 - 200, (height/ 2), this->PROJECTION, 1.0f, vec3(1,1,1));

        std::ostringstream timeStream;
        timeStream << "Time: " << timeWon << " seconds";
        fontRenderer->renderText(timeStream.str(), width / 2 - 200, (height/ 2) - 40, this->PROJECTION, 1.0f, vec3(1,1,1));

        std::ostringstream clickStream;
        clickStream << "Number of Clicks: " << numOfClicks;
        fontRenderer->renderText(clickStream.str(), width / 2 - 200, (height/ 2) - 80, this->PROJECTION, 1.0f, vec3(1,1,1));
    }

    // This is glfw function call is required to display the final image on the screen
    // The front buffer contains the final image that is displayed.
    // The back buffer contains the image that is currently being rendered.
    glfwSwapBuffers(window);

}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}

