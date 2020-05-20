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
#include "Renderer.h"
#include "CubeMap.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"

#include "SOIL2/SOIL2.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//defining structures
struct light {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;
bool left_button_down = false;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback(GLFWwindow* window, int button, int action, int mods);
void MousePosCallback(GLFWwindow* window, double xPos, double yPos);

void SetLightProperties(Shader shader, light l);

void GLAPIENTRY ErrorMessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

void DoMovement( );

void drawSkybox(glm::mat4& view, glm::mat4& projection, Shader& skyboxShader, VertexArray& skyboxVA, GLuint& skyboxtexture);

// Camera
Camera camera( glm::vec3( 2.0f, 2.0f, 10.0f ) );//change to set the initial positionof the camera
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Renderer renderer;

GLFWwindow* init() {

    // Init GLFW
    glfwInit( );

    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGl Project", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        exit(EXIT_FAILURE);
        //return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);


    //sync to monitor refresh rate
    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MousePosCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);

    //error callback
    //glDebugMessageCallback(ErrorMessageCallback, (void*)0);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;


    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
        //return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);//Debug output

    return(window);
}

int main( )
{
    GLFWwindow* window = init();

    glDebugMessageCallback(ErrorMessageCallback, NULL);
    
    // Setup and compile our shaders
    //Shader shader( "res/shaders/basicShader_withTexture.vs", "res/shaders/basicShader_withTexture.frag" );

    Shader modelshader("res/shaders/object_basic_lighting_shader.vs", "res/shaders/object_basic_lighting_shader.frag");
    //Shader modelshader("res/shaders/object_basic_lighting_shader.vs", "res/shaders/modelLoading.frag");

    Shader lightingShader("res/shaders/object_basic_lighting_shader.vs", "res/shaders/object_basic_lighting_shader.frag");
    Shader lampShader("res/shaders/lamp.vs", "res/shaders/lamp.frag");

    //Shader basicShader("res/shaders/basicShader_withTexture.vs", "res/shaders/basicShader_withTexture.frag");

    Shader skyboxShader("res/shaders/skybox.vs", "res/shaders/skybox.frag");

    //some usefull
    unsigned int CuboidIndices[36] = {
        //bottom face
        0, 1, 2,
        2, 3, 0,
        //top face
        4, 5, 6,
        6, 7, 4,
        //left face
        0, 4, 7,
        7, 3, 0,
        //right face
        6, 5, 1,
        1, 2, 6,
        //back face
        4, 5, 1,
        1, 0, 4,
        //font face
        7, 6, 2,
        2, 3, 7
    };


    IndexBuffer cuboidIB(CuboidIndices, sizeof(CuboidIndices) / sizeof(unsigned int));

    // Setup room

    GLfloat roomVertices[] =
    {
        // Positions                 //normals              // Texture Coords    //texID

        //back
          0.0f,   0.0f,  0.0f,       0.0f, 0.0f ,1.0f,       0.0f, 0.0f,         0,
         10.0f,   0.0f,  0.0f,       0.0f, 0.0f ,1.0f,       1.0f, 0.0f,         0,
         10.0f,  10.0f,  0.0f,       0.0f, 0.0f ,1.0f,       1.0f, 1.0f,         0,
         10.0f,  10.0f,  0.0f,       0.0f, 0.0f ,1.0f,       1.0f, 1.0f,         0,
          0.0f,  10.0f,  0.0f,       0.0f, 0.0f ,1.0f,       0.0f, 1.0f,         0,
          0.0f,   0.0f,  0.0f,       0.0f, 0.0f ,1.0f,       0.0f, 0.0f,         0,
                                                                                
        //left                                                                  
          0.0f,  10.0f, 10.0f,       1.0f, 0.0f ,0.0f,       1.0f, 0.0f,         0,
          0.0f,  10.0f,  0.0f,       1.0f, 0.0f ,0.0f,       1.0f, 1.0f,         0,
          0.0f,   0.0f,  0.0f,       1.0f, 0.0f ,0.0f,       0.0f, 1.0f,         0,
          0.0f,   0.0f,  0.0f,       1.0f, 0.0f ,0.0f,       0.0f, 1.0f,         0,
          0.0f,   0.0f, 10.0f,       1.0f, 0.0f ,0.0f,       0.0f, 0.0f,         0,
          0.0f,  10.0f, 10.0f,       1.0f, 0.0f ,0.0f,       1.0f, 0.0f,         0,
                                                                                 
          //bottom                                                               
          0.0f,   0.0f,   0.0f,       0.0f, 1.0f ,0.0f,       0.0f, 1.0f,        1,
         10.0f,   0.0f,   0.0f,       0.0f, 1.0f ,0.0f,       1.0f, 1.0f,        1,
         10.0f,   0.0f,  10.0f,       0.0f, 1.0f ,0.0f,       1.0f, 0.0f,        1,
         10.0f,   0.0f,  10.0f,       0.0f, 1.0f ,0.0f,       1.0f, 0.0f,        1,
          0.0f,   0.0f,  10.0f,       0.0f, 1.0f ,0.0f,       0.0f, 0.0f,        1,
          0.0f,   0.0f,   0.0f,       0.0f, 1.0f ,0.0f,       0.0f, 1.0f,        1,

    };

    VertexArray roomVA;
    VertexBuffer roomVB(roomVertices, sizeof(roomVertices));

    VertexBufferLayout roomlayout;
    //positions
    roomlayout.Push<float>(3);
    //texture cordinates
    roomlayout.Push<float>(3);
    //normals
    roomlayout.Push<float>(2);
    //texture id
    roomlayout.Push<unsigned int>(1);


    roomVA.AddBuffer(roomVB, roomlayout);
    //textures setup
    TextureClass wallTexture("res/images/wall-texture.png");
    TextureClass floorTexture("res/images/white.png");


    glBindVertexArray(0);

    
    /*
    //desk setup

    GLfloat tableTopHeight = 0.1f, tableTopWidth = 2.0f, tableTopBreadth = 4.0f;
    GLfloat tableTopOrigin[3] = { 2.0f, 3.0f, 1.0f };

    GLfloat tableTopVertices[] = {
        //positions                                                                                                     //texture cords         //normals
        //bottom face
        tableTopOrigin[0]                  , tableTopOrigin[1]                 , tableTopOrigin[2],                      0.0f, 1.0f,            1.0f, 1.0f, 1.0f,
        tableTopOrigin[0] + tableTopBreadth, tableTopOrigin[1]                 , tableTopOrigin[2],                      1.0f, 1.0f,            1.0f, 1.0f, 1.0f,
        tableTopOrigin[0] + tableTopBreadth, tableTopOrigin[1]                 , tableTopOrigin[2] + tableTopWidth,      1.0f, 0.0f,            1.0f, 1.0f, 1.0f,
        tableTopOrigin[0]                  , tableTopOrigin[1]                 , tableTopOrigin[2] + tableTopWidth,      0.0f, 0.0f,            1.0f, 1.0f, 1.0f,

        //top face                                                                                                                              
        tableTopOrigin[0]                  , tableTopOrigin[1] + tableTopHeight , tableTopOrigin[2],                     0.0f, 1.0f,            1.0f, 1.0f, 1.0f,
        tableTopOrigin[0] + tableTopBreadth, tableTopOrigin[1] + tableTopHeight , tableTopOrigin[2],                     1.0f, 1.0f,            1.0f, 1.0f, 1.0f,
        tableTopOrigin[0] + tableTopBreadth, tableTopOrigin[1] + tableTopHeight , tableTopOrigin[2] + tableTopWidth,     1.0f, 0.0f,            1.0f, 1.0f, 1.0f,
        tableTopOrigin[0]                  , tableTopOrigin[1] + tableTopHeight , tableTopOrigin[2] + tableTopWidth,     0.0f, 0.0f,            1.0f, 1.0f, 1.0f,
    };


    unsigned int tableTopIndices[] = {
        //bottom face
        0, 1, 2,
        2, 3, 0,
        //top face
        4, 5, 6,
        6, 7, 4,
        //left face
        0, 4, 7,
        7, 3, 0,
        //right face
        6, 5, 1,
        1, 2, 6,
        //back face
        4, 5, 1,
        1, 0, 4,
        //font face
        7, 6, 2,
        2, 3, 7
    };

    VertexArray deskVA;
    VertexBuffer deskVB(tableTopVertices, sizeof(tableTopVertices));
    IndexBuffer deskIB(tableTopIndices, sizeof(tableTopIndices)/sizeof(unsigned int));
    VertexBufferLayout desklayout;
    //positions
    desklayout.Push<float>(3);
    //texture cordinates
    desklayout.Push<float>(2);
    //normals
    desklayout.Push<float>(3);

    deskVA.AddBuffer(deskVB, desklayout);
    //textures setup
    TextureClass deskTexture("/res/images/container2.png");

    glBindVertexArray(0);
    */

    // Light attributes
    light light1;
    
    light1.position = glm::vec3(1.0f, 1.71f, 3.0f);
    light1.color = glm::vec3(1.0f, 1.0f, 0.9f);
    light1.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    light1.diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
    light1.specular = glm::vec3(0.5f, 0.5f, 0.5f);
  


    //start of lamp code
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat lampVertices[] = {

        // Positions            // Normals              // Texture Coords
        1.0f,  1.0f,  1.0f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
        2.0f,  1.0f,  1.0f,    0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
        2.0f,  2.0f,  1.0f,    0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        2.0f,  2.0f,  1.0f,    0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        1.0f,  2.0f,  1.0f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
        1.0f,  1.0f,  1.0f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
              
        1.0f,  1.0f,  2.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
        2.0f,  1.0f,  2.0f,    0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
        2.0f,  2.0f,  2.0f,    0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
        2.0f,  2.0f,  2.0f,    0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
        1.0f,  2.0f,  2.0f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
        1.0f,  1.0f,  2.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

        1.0f,  2.0f,  2.0f,   -1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        1.0f,  2.0f,  1.0f,   -1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        1.0f,  1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        1.0f,  1.0f,  2.0f,   -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
        1.0f,  2.0f,  2.0f,   -1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

        2.0f,  2.0f,  2.0f,    1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        2.0f,  2.0f,  1.0f,    1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
        2.0f,  1.0f,  1.0f,    1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        2.0f,  1.0f,  1.0f,    1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        2.0f,  1.0f,  2.0f,    1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
        2.0f,  2.0f,  2.0f,    1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

        1.0f,  1.0f,  1.0f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
        2.0f,  1.0f,  1.0f,    0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
        2.0f,  1.0f,  2.0f,    0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
        2.0f,  1.0f,  2.0f,    0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
        1.0f,  1.0f,  2.0f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
        1.0f,  1.0f,  1.0f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

        1.0f,  2.0f,  1.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
        2.0f,  2.0f,  1.0f,    0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
        2.0f,  2.0f,  2.0f,    0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
        2.0f,  2.0f,  2.0f,    0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
        1.0f,  2.0f,  2.0f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
        1.0f,  2.0f,  1.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
    };


    // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    VertexArray lampLightVA;
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    //lampCommonVB.Bind();
    VertexBuffer lampCommonVB(lampVertices, sizeof(lampVertices));
    VertexBufferLayout lampLightlayout;
    //positions
    lampLightlayout.Push<float>(3);
    //normals
    lampLightlayout.Push<float>(3);
    //texture cordinates
    lampLightlayout.Push<float>(2);

    lampLightVA.AddBuffer(lampCommonVB, lampLightlayout);

    glBindVertexArray(0);

    //skybox setup
    GLfloat skyboxVertices[36 * 3] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // Setup skybox VAO
    VertexArray skyboxVA;
    VertexBuffer skyboxVB(skyboxVertices, sizeof(skyboxVertices));

    VertexBufferLayout skyboxlayout;
    //positions
    skyboxlayout.Push<float>(3);
    /*
    //texture cordinates
    skyboxlayout.Push<float>(2);
    */


    skyboxVA.AddBuffer(skyboxVB, skyboxlayout);


    // Load textures
    //GLuint cubeTexture = Texture( "res/images/container2.png" );

   // GLuint floorTexture = TextureLoading::LoadTexture("res/images/floor-texture.png");

    // Cubemap (Skybox)
    vector<const GLchar*> faces;
    faces.push_back( "res/images/skybox/right.jpg" );
    faces.push_back( "res/images/skybox/left.jpg" );
    faces.push_back( "res/images/skybox/top.jpg" );
    faces.push_back( "res/images/skybox/bottom.jpg" );
    faces.push_back( "res/images/skybox/back.jpg" );
    faces.push_back( "res/images/skybox/front.jpg" );
    GLuint skyboxTexture = CubeMap::LoadCubemap( faces );

    //get the projection matrix
    glm::mat4 projection = glm::perspective( camera.GetFov( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 1000.0f );
   

    // Load models
    Model nanosuitModel("res/models/nanosuit.obj");
    Model benchModel("res/models/bench.obj");
    Model cupModel("res/models/cup.obj");
    Model lampModel("res/models/simple-lamp.obj");


    //cout << "debug1"<<endl;
    //int r;
    //glGetIntegerv(GL_CURRENT_PROGRAM, &r);
    //cout << r << " is the bound program" << endl;

    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //GLint temp = (GLint)currentFrame % 2;
        
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        
        // Create camera transformations
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model(1.0f);

        //draw room
        // Pass the matrices to the shader
        model = glm::mat4(1.0f);

        lightingShader.SetUniform4fv("model", model, GL_FALSE);
        lightingShader.SetUniform4fv("view", view, GL_FALSE);
        lightingShader.SetUniform4fv("projection", projection, GL_FALSE);


        lightingShader.SetUniform3f("light.color", light1.color);
        lightingShader.SetUniform3f("light.position", light1.position);
        lightingShader.SetUniform3f("light.diffuse", light1.diffuse);
        lightingShader.SetUniform3f("light.ambient", light1.ambient);
        lightingShader.SetUniform3f("light.specular", light1.specular);


        lightingShader.SetUniform3f("viewPos", camera.GetPosition());
       
        lightingShader.SetUniform3f("material.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.SetUniform3f("material.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.SetUniform3f("material.specular", 0.1f, 0.1f, 0.1f);
        lightingShader.SetUniform1f("material.shininess", 32.0f);
        lightingShader.SetUniform3f("material.color", 0.0f, 1.0f, 0.0f);
        
        wallTexture.Bind(0);
        lightingShader.SetUniform1i("u_texture0" , 0);
        floorTexture.Bind(1);
        lightingShader.SetUniform1i("u_texture1" , 1);
        lightingShader.SetUniform1i("texture_enable", 1);

        renderer.DrawWithoutIB(roomVA, lightingShader, 18);

        wallTexture.UnBind();
        glBindVertexArray(0);



        // Also draw the lamp object (The shell which will appear the color of the lamp)
        //binding the appropriate shader

        // Set matrices
        lampShader.SetUniform4fv("view", view, GL_FALSE);
        lampShader.SetUniform4fv("projection", projection, GL_FALSE);

        lampShader.SetUniform1f("lightcolor", light1.color);
        //GL_CHECK(lampShader.SetUniform1f("lightbrightness", 1.0f));


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f, 1.71f, 2.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.3f)); // Make it a smaller cube
        lampShader.SetUniform4fv("model", model, GL_FALSE);

        renderer.DrawWithoutIB(lampLightVA, lampShader, 36);

        glBindVertexArray(0);

        //draw bench

        modelshader.SetUniform3f("light.color", light1.color);
        modelshader.SetUniform3f("light.position", light1.position);
        modelshader.SetUniform3f("light.diffuse", light1.diffuse);
        modelshader.SetUniform3f("light.ambient", light1.ambient);
        modelshader.SetUniform3f("light.specular", light1.specular);


        modelshader.SetUniform3f("viewPos", camera.GetPosition());

        modelshader.SetUniform3f("material.ambient", 0.5f, 0.5f, 0.5f);
        modelshader.SetUniform3f("material.diffuse", 3.0f, 3.0f, 3.0f);
        modelshader.SetUniform3f("material.specular", 0.1f, 0.1f, 0.1f);
        modelshader.SetUniform1f("material.shininess", 32.0f);
        //modelshader.SetUniform3f("material.color", 0.0f, 1.0f, 0.0f);


        

        // Draw the loaded models
        modelshader.SetUniform1i("isModel", 1);
        modelshader.SetUniform4fv("view", view, GL_FALSE);
        modelshader.SetUniform4fv("projection", projection, GL_FALSE);


        //nanosuit model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, 2.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        modelshader.SetUniform4fv("model", model, GL_FALSE);

        nanosuitModel.Draw(modelshader);

        //bench model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 3.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// It's a bit too big for our scene, so scale it down
        modelshader.SetUniform4fv("model", model, GL_FALSE);

        benchModel.Draw(modelshader);




        //cup model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 1.71f, 3.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	
        model = glm::rotate(model, 90.0f, glm::vec3(0.0f, -1.0f, 0.0f));	
        modelshader.SetUniform4fv("model", model, GL_FALSE);

        modelshader.SetUniform1f("material.shininess", 1.0f);

        cupModel.Draw(modelshader);

        //draw lamp
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 1.71f, 2.5f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// It's a bit too big for our scene, so scale it down
        modelshader.SetUniform4fv("model", model, GL_FALSE);

        lampModel.Draw(modelshader);


        //uncomment to enable skybox
        //drawSkybox(view, projection, skyboxShader, skyboxVA, skyboxTexture);

        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
}

void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (GLFW_PRESS == action)
            left_button_down = true;
        else if (GLFW_RELEASE == action)
            left_button_down = false;
    }
}

void MousePosCallback( GLFWwindow *window, double xPos, double yPos )
{
    if (left_button_down) {
        if (firstMouse)
        {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        GLfloat xOffset = xPos - lastX;
        GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

        lastX = xPos;
        lastY = yPos;

        camera.ProcessMouseMovement(xOffset, yOffset);
    } else {
        firstMouse = true;
    }
}

void drawSkybox(glm::mat4& view, glm::mat4& projection, Shader& skyboxShader, VertexArray& skyboxVA, GLuint& skyboxTexture) {
        //draw skybox
        // Draw skybox as last
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content

        skyboxShader.Bind();

        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix

        skyboxShader.SetUniform4fv("view", view, GL_FALSE);
        skyboxShader.SetUniform4fv("projection", projection, GL_FALSE);

        // skybox cube
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

        renderer.DrawWithoutIB(skyboxVA, skyboxShader, 36);
        //glDrawArrays(GL_TRIANGLES, 0, 36);


        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
}

void GLAPIENTRY ErrorMessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << 
        " type = " << type << " severity = " << severity << " message = " << message << std::endl;
}

void SetLightProperties(Shader shader, light l) {
    shader.SetUniform3f("light.color", l.color);
    shader.SetUniform3f("light.position", l.position);
    shader.SetUniform3f("light.ambient", l.ambient);
    shader.SetUniform3f("light.diffuse", l.diffuse);
    shader.SetUniform3f("light.specular", l.specular);
}
