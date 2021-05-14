#include "canvas.h"

Canvas::Canvas(GamePlay *game, int width, int height)
{
    this->game = game;
    this->board = new int*[height];
    for (int i = 0; i < height; i++) {
        board[i] = new int[width];
    }
    this->width = width;
    this->height = height;

    this->foodSnakePosition = new GLfloat[(width * height + 1) * 3];
    this->foodSnakeColor= new GLfloat[(width * height + 1) * 3];

    recalculateBuffers();

    pastTime = currentTime = high_resolution_clock::now();
    fpsCounter = 0;
}

void Canvas::init()
{
    if (!glfwInit()) {
        cout << "Failed to launch GLFW" << endl;
    }

    //glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width * GRIDSIZE, height * GRIDSIZE, "Snake", NULL, NULL);

    if (window == NULL) {
        cout << "Failed to open GLFW" << endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;

    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        glfwTerminate();
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    glPointSize(GRIDSIZE-0.4f);

    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    GLuint program = LoadShaders("shader.vert", "shader.frag");
    GLuint matrix = glGetUniformLocation(program, "mvp");

    float size = (float) width;

    //fucking magic over here

    glm::mat4 Projection = glm::ortho(-size/2, size/2, -size/2, size/2, 0.0f, 100.0f);
    glm::mat4 View       = glm::lookAt(
                glm::vec3(0,0,10),
                glm::vec3(0,0,0),
                glm::vec3(0,1,0)
                );
    glm::mat4 Model      = glm::mat4(1.0f);
    Model = glm::translate(Model, glm::vec3(0.5, 0.5, 0));
    glm::mat4 MVP        = Projection * View * Model;

    //why does this work^??????

    GLuint foodSnakePositionB;
    glGenBuffers(1, &foodSnakePositionB);
    glBindBuffer(GL_ARRAY_BUFFER, foodSnakePositionB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * (width * height + 1) , foodSnakePosition, GL_STATIC_DRAW);

    GLuint foodSnakeColorBuffer;
    glGenBuffers(1, &foodSnakeColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, foodSnakeColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * (width * height + 1), foodSnakeColor, GL_STATIC_DRAW);

    do {
        recalculateBuffers();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glUniformMatrix4fv(matrix, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, foodSnakePositionB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * (game->snake()->size() + 1), foodSnakePosition);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, foodSnakeColorBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * (game->snake()->size() + 1), foodSnakeColor);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glDrawArrays(GL_POINTS, 0, game->snake()->size()+1);

        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        calcInput();

        currentTime = high_resolution_clock::now();
        dur = currentTime - pastTime;
        if (dur.count() >= 1000/game->speed()) {
            if (!(get<0>(direction) + get<0>(game->direction()) == 0 && get<1>(direction) + get<1>(game->direction()) == 0))
                game->setDir(direction);
            game->tick();
            pastTime = high_resolution_clock::now();
        }
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void Canvas::recalculateBuffers()
{
    float snakeColor[3] = {0.2f, 0.8f, 0.2f};
    float foodColor[3] = {0.6f, 0.2f, 0.2f};

    foodSnakePosition[0] = get<0>(game->food());

    foodSnakePosition[1] = get<1>(game->food());
    foodSnakePosition[2] = 0;

    foodSnakeColor[0] = foodColor[0];
    foodSnakeColor[1] = foodColor[1];
    foodSnakeColor[2] = foodColor[2];

    int offset = 3;
    for (int i = 0; i < game->snake()->size(); i++) {
        foodSnakePosition[offset + i*3 + 0] = get<0>(game->snake()->at(i));
        foodSnakePosition[offset + i*3 + 1] = get<1>(game->snake()->at(i));
        foodSnakePosition[offset + i*3 + 2] = 0;

        foodSnakeColor[offset + i*3 + 0] = snakeColor[0];
        foodSnakeColor[offset + i*3 + 1] = snakeColor[1];
        foodSnakeColor[offset + i*3 + 2] = snakeColor[2];
    }
}

void Canvas::calcInput()
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        direction = tuple<int, int>(0, 1);
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        direction = tuple<int, int>(0, -1);
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        direction = tuple<int, int>(-1, 0);
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        direction = tuple<int, int>(1, 0);
    }
}
