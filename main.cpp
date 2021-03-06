#include <fstream>
#include <iostream>
#include <vector>

#include "common.h"
#include "Image.h"
#include "Player.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

constexpr GLsizei WINDOW_WIDTH = 1920, WINDOW_HEIGHT = 1024;
constexpr int tileMapWidth = WINDOW_WIDTH/tileSize, tileMapHeight = WINDOW_HEIGHT/tileSize;

Image wall_upleft_tile("./resources/wall_upleft.png");

Image wall_upright_tile("./resources/wall_upright.png");

Image wall_downleft_tile("./resources/wall_downleft.png");

Image wall_downright_tile("./resources/wall_downright.png");

Image wall_up_tile("./resources/wall_up.png");

Image wall_left_tile("./resources/wall_left.png");

Image wall_down_tile("./resources/wall_down.png");

Image wall_right_tile("./resources/wall_right.png");

Image floor_tile("./resources/floor_tile.png");

Image wall_angleleft_tile("./resources/wall_angleleft.png");

Image wall_angleright_tile("./resources/wall_angleright.png");

Image hatch_tile("./resources/hatch.png");

Image floor_cracks1_tile("./resources/floor_cracks1.png");

Image floor_cracks2_tile("./resources/floor_cracks2.png");

Image door1_tile("./resources/door1.png");

Image velvety_door_tile("./resources/velvety_door.png");

Image gameOverBuffer("./resources/game_over.png");

Image gameEndBuffer("./resources/well_played.png");

Image trap1_tile("./resources/trap1.png");

Image skeleton_tile("./resources/skeleton.png");

Image stone_tile("./resources/stone.png");


struct InputState
{
    bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
    GLfloat lastX = 400, lastY = 300; //исходное положение мыши
    bool firstMouse = true;
    bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
    bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
	if (action == GLFW_PRESS)
	    glfwSetWindowShouldClose(window, GL_TRUE);
	break;
    case GLFW_KEY_1:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    case GLFW_KEY_2:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
    default:
	if (action == GLFW_PRESS)
            Input.keys[key] = true;
	else if (action == GLFW_RELEASE)
            Input.keys[key] = false;
    }
}

void processPlayerMovement(Player &player, int width, int height, int* tileMap, int& gameOverFlag, int& doorOpened)
{
    if (Input.keys[GLFW_KEY_E])
        player.ProcessInput(Actions::INTERACTION, width, height, tileMap, gameOverFlag, doorOpened);
    if (Input.keys[GLFW_KEY_W])
        player.ProcessInput(Actions::UP, width, height, tileMap, gameOverFlag, doorOpened);
    else if (Input.keys[GLFW_KEY_S])
        player.ProcessInput(Actions::DOWN, width, height, tileMap, gameOverFlag, doorOpened);
    if (Input.keys[GLFW_KEY_A])
        player.ProcessInput(Actions::LEFT, width, height, tileMap, gameOverFlag, doorOpened);
    else if (Input.keys[GLFW_KEY_D])
        player.ProcessInput(Actions::RIGHT, width, height, tileMap, gameOverFlag, doorOpened);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        Input.captureMouse = !Input.captureMouse;
    
    if (Input.captureMouse){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Input.capturedMouseJustNow = true;
    } else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    if (Input.firstMouse) {
        Input.lastX = float(xpos);
        Input.lastY = float(ypos);
        Input.firstMouse = false;
    }

    GLfloat xoffset = float(xpos) - Input.lastX;
    GLfloat yoffset = Input.lastY - float(ypos);

    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
	std::cout << "Failed to initialize OpenGL context" << std::endl;
	return -1;
    }

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "Controls: "<< std::endl;
    std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
    std::cout << "W, A, S, D - movement  "<< std::endl;
    std::cout << "press ESC to exit" << std::endl;

    return 0;
}

void createBackground(Image &frame, const std::string &filepath, int* tileMap, Point& starting_pos) {
    std::fstream fin;
    fin.open(filepath, std::ios::in);

    int i,j;

    for (i = 0; i < WINDOW_WIDTH; i++)
        for (j = 0; j < WINDOW_HEIGHT; j++)
            frame.PutPixel(i, j, backgroundColor);

    char symbol;

    int x = 0, y = 0;
    
    while(fin.get(symbol)) {
        switch (symbol) {
            case '1':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_upleft_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case '2':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_upright_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case '3':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_downleft_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case '4':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_downright_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case '^':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_up_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case '<':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_left_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case 'v':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_down_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case '>':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_right_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case '5':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_angleleft_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case '6':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, wall_angleright_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case 'C':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 0;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                        frame.AddPixel(x + i, y + j, floor_tile.GetPixel(i, j));
                        frame.AddPixel(x + i, y + j, floor_cracks1_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case 'c':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 0;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                        frame.AddPixel(x + i, y + j, floor_tile.GetPixel(i, j));
                        frame.AddPixel(x + i, y + j, floor_cracks2_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case 'h':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 0;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                        frame.AddPixel(x + i, y + j, floor_tile.GetPixel(i, j));
                        frame.AddPixel(x + i, y + j, hatch_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case '@':
                starting_pos.x = x;
                starting_pos.y = y;

                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 0;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                        frame.AddPixel(x + i, y + j, floor_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case 'x':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 3;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                        frame.AddPixel(x + i, y + j, door1_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case 'y': 
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 4;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                        frame.AddPixel(x + i, y + j, door1_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case 'z': 
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                        frame.AddPixel(x + i, y + j, door1_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case 'T': 
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 6;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                        frame.AddPixel(x + i, y + j, trap1_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case 'Q':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 5;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                        frame.AddPixel(x + i, y + j, velvety_door_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case 'S':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 0;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                       frame.AddPixel(x + i, y + j, floor_tile.GetPixel(i, j));
                       frame.AddPixel(x + i, y + j, skeleton_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case 's':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 1;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++){
                       frame.AddPixel(x + i, y + j, floor_tile.GetPixel(i, j));
                       frame.AddPixel(x + i, y + j, stone_tile.GetPixel(i, j));
                    }
                x += tileSize;
                break;
            case '.':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 0;
                for (i = 0; i < tileSize; i++)
                    for (j = 0; j < tileSize; j++)
                        frame.AddPixel(x + i, y + j, floor_tile.GetPixel(i, j));
                x += tileSize;
                break;
            case ' ':
                tileMap[y / tileSize * WINDOW_WIDTH / tileSize + x / tileSize] = 2;
                x += tileSize;
                break;
            case '\n':
                y += tileSize;
                x = 0;
                break;
            default:
                break;
        }
    }

    fin.close();
}

void ReadMap(std::vector<char>& rooms, const std::string& filepath) {
    std::fstream fin;
    fin.open(filepath);
    char roomLetter;

    while (fin.get(roomLetter)) {
        if ((roomLetter >= 'A') && (roomLetter <= 'Z')) {
            rooms.push_back(roomLetter);
        }
    }
}

int main(int argc, char** argv) {
    if(!glfwInit())
        return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Billy's dungeon adventures", nullptr, nullptr);
    if (window == nullptr){
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
    }
	
    glfwMakeContextCurrent(window); 

    glfwSetKeyCallback        (window, OnKeyboardPressed);  
    glfwSetCursorPosCallback  (window, OnMouseMove); 
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
    glfwSetScrollCallback     (window, OnMouseScroll);

    if(initGL() != 0) {
        printf("here!");
	return -1;
    }

    //Reset any OpenGL errors which could be present for some reason
    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR)
	gl_error = glGetError();


    Point starting_pos;

    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
    Image backGround(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

    glViewport(0, 0, WINDOW_WIDTH , WINDOW_HEIGHT);  GL_CHECK_ERRORS;

    int tileMap[tileMapWidth * tileMapHeight];

    int gameOverFlag = 0, doorOpened = 0, roomNumber = 1;

    std::vector<char> rooms;
        rooms.push_back('#');

    ReadMap(rooms, "./resources/roomMap");

//    for (int i = 0; i < rooms.size(); i++)
//        std::cout << rooms[i];

    switch(rooms[1]) {
        case 'A':
            createBackground(backGround, "./resources/roomA", tileMap, starting_pos);
            break;
        case 'B':
            createBackground(backGround, "./resources/roomB", tileMap, starting_pos);
            break;
        case 'C':
            createBackground(backGround, "./resources/roomC", tileMap, starting_pos);
            break;
        case 'D':
            createBackground(backGround, "./resources/roomD", tileMap, starting_pos);
            break;
        default:
            break;
    }

    Player player{"./resources/knight_frontright.png", starting_pos};

    while (!glfwWindowShouldClose(window)) {

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

        glfwPollEvents();

        processPlayerMovement(player, tileMapWidth, tileMapHeight, tileMap, gameOverFlag, doorOpened);
        if (doorOpened == 1) {

            roomNumber *= 2;

            switch(rooms[roomNumber]){
                case 'A':
                    createBackground(backGround, "./resources/roomA", tileMap, starting_pos);
                    break;
                case 'B':
                    createBackground(backGround, "./resources/roomB", tileMap, starting_pos);
                    break;
                case 'C':
                    createBackground(backGround, "./resources/roomC", tileMap, starting_pos);
                    break;
                case 'D':
                    createBackground(backGround, "./resources/roomD", tileMap, starting_pos);
                    break;
            }

            doorOpened = 0;
            player.MoveTo(starting_pos);

        } else if (doorOpened == 2) {

            roomNumber = roomNumber * 2 + 1;

            switch(rooms[roomNumber]){
                case 'A':
                    createBackground(backGround, "./resources/roomA", tileMap, starting_pos);
                    break;
                case 'B':
                    createBackground(backGround, "./resources/roomB", tileMap, starting_pos);
                    break;
                case 'C':
                    createBackground(backGround, "./resources/roomC", tileMap, starting_pos);
                    break;
                case 'D':
                    createBackground(backGround, "./resources/roomD", tileMap, starting_pos);
                    break;
            }

            doorOpened = 0;
            player.MoveTo(starting_pos);
        }


        player.Draw(screenBuffer, backGround);

        if (gameOverFlag == 1){

            glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, gameOverBuffer.Data()); GL_CHECK_ERRORS;

//	    glfwSetWindowShouldClose(window, GL_TRUE);

        } else if (gameOverFlag == 2){

            glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, gameEndBuffer.Data()); GL_CHECK_ERRORS;
            
//	    glfwSetWindowShouldClose(window, GL_TRUE);

        } else {

          glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

        }

	glfwSwapBuffers(window);
    }

//    for (int i = 0; i < WINDOW_HEIGHT / tileSize; i++){
//        for (int j = 0; j < WINDOW_WIDTH / tileSize; j++)
//            std::cout << tileMap[i * WINDOW_WIDTH / tileSize + j];
//        std::cout << std::endl;
//    }

    glfwTerminate();
    return 0;
}
