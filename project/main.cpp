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
#include "Indicies.h"
#include "Texture.h"

#include "SOIL2/SOIL2.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs


// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;
bool left_button_down = false;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback(GLFWwindow* window, int button, int action, int mods);
void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
void GLAPIENTRY ErrorMessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

void DoMovement( );

void drawSkybox(glm::mat4 view, glm::mat4 projection, Shader skyboxShader, VertexArray skyboxVA, GLuint skyboxtexture);

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
    
    // Setup and compile our shaders
    //Shader shader( "res/shaders/basicShader_withTexture.vs", "res/shaders/basicShader_withTexture.frag" );
    Shader skyboxShader( "res/shaders/skybox.vs", "res/shaders/skybox.frag" );

    Shader modelshader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");

    Shader lightingShader("res/shaders/object_basic_lighting_shader.vs", "res/shaders/object_basic_lighting_shader.frag");
    Shader lampShader("res/shaders/lamp.vs", "res/shaders/lamp.frag");

    // Setup room

    GLfloat roomVertices[] =
    {
        // Positions           // Texture Coords            //normals
       /*

       just vertices tto use for index buffer
         //bottom
          0.0f,   0.0f,   0.0f,   0.0f, 0.0f,
         10.0f,   0.0f,   0.0f,   1.0f, 0.0f,
         10.0f,   0.0f,  10.0f,   1.0f, 0.0f,
          0.0f,   0.0f,  10.0f,   0.0f, 0.0f,

         //top
        */

        //back
          0.0f,   0.0f,  0.0f,       0.0f, 0.0f,        0.0f, 0.0f ,0.0f,
         10.0f,   0.0f,  0.0f,       1.0f, 0.0f,        0.0f, 0.0f ,0.0f,
         10.0f,  10.0f,  0.0f,       1.0f, 1.0f,        0.0f, 0.0f ,0.0f,
         10.0f,  10.0f,  0.0f,       1.0f, 1.0f,        0.0f, 0.0f ,0.0f,
          0.0f,  10.0f,  0.0f,       0.0f, 1.0f,        0.0f, 0.0f ,0.0f,
          0.0f,   0.0f,  0.0f,       0.0f, 0.0f,        0.0f, 0.0f ,0.0f,

        //left
          0.0f,  10.0f, 10.0f,       1.0f, 0.0f,        0.0f, 0.0f ,0.0f,
          0.0f,  10.0f,  0.0f,       1.0f, 1.0f,        0.0f, 0.0f ,0.0f,
          0.0f,   0.0f,  0.0f,       0.0f, 1.0f,        0.0f, 0.0f ,0.0f,
          0.0f,   0.0f,  0.0f,       0.0f, 1.0f,        0.0f, 0.0f ,0.0f,
          0.0f,   0.0f, 10.0f,       0.0f, 0.0f,        0.0f, 0.0f ,0.0f,
          0.0f,  10.0f, 10.0f,       1.0f, 0.0f,        0.0f, 0.0f ,0.0f,

         //bottom
          0.0f, 0.0f,   0.0f,        0.0f, 1.0f,        0.0f, 0.0f ,0.0f,
         10.0f, 0.0f,   0.0f,        1.0f, 1.0f,        0.0f, 0.0f ,0.0f,
         10.0f, 0.0f,  10.0f,        1.0f, 0.0f,        0.0f, 0.0f ,0.0f,
         10.0f, 0.0f,  10.0f,        1.0f, 0.0f,        0.0f, 0.0f ,0.0f,
          0.0f, 0.0f,  10.0f,        0.0f, 0.0f,        0.0f, 0.0f ,0.0f,
          0.0f, 0.0f,   0.0f,        0.0f, 1.0f,        0.0f, 0.0f ,0.0f,

    };

    VertexArray roomVA;
    VertexBuffer roomVB(roomVertices, sizeof(roomVertices));

    VertexBufferLayout roomlayout;
    //positions
    roomlayout.Push<float>(3);
    //texture cordinates
    roomlayout.Push<float>(2);
    //normals
    roomlayout.Push<float>(3);

    roomVA.AddBuffer(roomVB, roomlayout);
    //textures setup
    TextureClass wallTexture("res/images/wall-texture.png");


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
    glm::vec3 lightPos(20.0f, 30.0f, 10.0f);


    //start of lamp code
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat lampVertices[] = {

        // Positions            // Normals              // Texture Coords
        1.0f, 1.0f, 1.0f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
        2.0f, 1.0f, 1.0f,    0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
        2.0f,  2.0f, 1.0f,   0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        2.0f,  2.0f, 1.0f,   0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        1.0f,  2.0f, 1.0f,   0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
        1.0f, 1.0f, 1.0f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

        1.0f, 1.0f,  2.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
        2.0f, 1.0f,  2.0f,    0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
        2.0f,  2.0f,  2.0f,   0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
        2.0f,  2.0f,  2.0f,   0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
        1.0f,  2.0f,  2.0f,   0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
        1.0f, 1.0f,  2.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

        1.0f,  2.0f,  2.0f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
        1.0f,  2.0f, 1.0f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
        1.0f,  1.0f, 1.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
        1.0f,  1.0f, 1.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
        1.0f,  1.0f,  2.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
        1.0f,  2.0f,  2.0f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

        2.0f,  2.0f,  2.0f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        2.0f,  2.0f, 1.0f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
        2.0f, 1.0f, 1.0f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        2.0f, 1.0f, 1.0f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        2.0f, 1.0f,  2.0f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
        2.0f,  2.0f,  2.0f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

        1.0f, 1.0f, 1.0f,     0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
        2.0f, 1.0f, 1.0f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
        2.0f, 1.0f,  2.0f,    0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
        2.0f, 1.0f,  2.0f,    0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
        1.0f, 1.0f,  2.0f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
        1.0f, 1.0f, 1.0f,     0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

        1.0f,  2.0f, 1.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
         2.0f,  2.0f, 1.0f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
         2.0f,  2.0f,  2.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
         2.0f,  2.0f,  2.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
        1.0f,  2.0f,  2.0f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
        1.0f,  2.0f, 1.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
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
    Model benchModel("res/models/bench.obj");
    Model cupModel("res/models/cup.obj");


    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        
        // Create camera transformations
        glm::mat4 view = camera.GetViewMatrix();

        //draw room
        glm::mat4 model(1.0f);

        // Pass the matrices to the shader
        lightingShader.Bind();
        lightingShader.SetUniform4fv("model", model, GL_FALSE);
        lightingShader.SetUniform4fv("view", view, GL_FALSE);
        lightingShader.SetUniform4fv("projection", projection, GL_FALSE);

        lightingShader.SetUniform3f("light.color", 1.0f, 1.0f, 1.0f);
        lightingShader.SetUniform3f("light.position", lightPos);
        lightingShader.SetUniform3f("viewPos", camera.GetPosition());

        lightingShader.SetUniform3f("material.ambient", 0.2f, 0.5f, 0.3f);
        lightingShader.SetUniform3f("material.diffuse", 0.8f, 0.5f, 0.7f);
        lightingShader.SetUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.SetUniform1f("material.shininess", 32.0f);
        //lightingShader.SetUniform3f("material.color", 0.5f, 0.1f, 0.5f);
        
        wallTexture.Bind(0);
        lightingShader.SetUniform1i("u_texture" , 0);
        lightingShader.SetUniform1i("texture_enable", 1);

        renderer.DrawWithoutIB(roomVA, lightingShader, 18);

        wallTexture.UnBind();
        glBindVertexArray(0);
  /*
        //draw desk
        model = glm::mat4(1.0f);
        
        deskTexture.Bind(1);
        //lightingShaderWithTexture.SetUniform1i("u_texture", glm::uvec1(1));

        lightingShader.Bind();
        lightingShader.SetUniform3f("light.color", 1.0f, 1.0f, 1.0f);
        lightingShader.SetUniform3f("light.position", lightPos);
        lightingShader.SetUniform3f("viewPos", camera.GetPosition());

        lightingShader.SetUniform3f("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.SetUniform3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShader.SetUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.SetUniform1f("material.shininess", 100.0f);
        lightingShader.SetUniform3f("material.color", 1.0f, 0.5f, 0.0f);

        // Calculate the model matrix for each object and pass it to shader before drawing
        lightingShader.SetUniform4fv("model", model, GL_FALSE);
        lightingShader.SetUniform4fv("view", view, GL_FALSE);
        lightingShader.SetUniform4fv("projection", projection, GL_FALSE);

        renderer.Draw(deskVA, deskIB, lightingShader);

        glBindVertexArray( 0 );
        */

        /*
        //drawing the light itself (not the shell)
        model = glm::mat4(1.0f);
        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Bind();

        lightingShader.SetUniform3f("light.color", 1.0f, 1.0f, 1.0f);
        lightingShader.SetUniform3f("light.position", lightPos);
        lightingShader.SetUniform3f("viewPos", camera.GetPosition());

        lightingShader.SetUniform3f("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.SetUniform3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShader.SetUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.SetUniform1f("material.shininess", 100.0f);
        lightingShader.SetUniform3f("material.color", 0.0f, 0.5f, 0.31f);

        // Pass the matrices to the shader
        lightingShader.SetUniform4fv("view", view, GL_FALSE);
        lightingShader.SetUniform4fv("projection", projection, GL_FALSE);
        lightingShader.SetUniform4fv("model", model, GL_FALSE);


        glBindVertexArray(0);
        */
        /*
        // Also draw the lamp object (The shell which will appear the color of the lamp)
        //binding the appropriate shader
        lampShader.Bind();
        // Set matrices
        lampShader.SetUniform4fv("view", view, GL_FALSE);
        lampShader.SetUniform4fv("projection", projection, GL_FALSE);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        lampShader.SetUniform4fv("model", model, GL_FALSE);

        renderer.DrawWithoutIB(lampLightVA, lampShader, 36);

        glBindVertexArray(0);
 */
        //draw bench
        lightingShader.Bind();

        lightingShader.SetUniform4fv("view", view, GL_FALSE);
        lightingShader.SetUniform4fv("projection", projection, GL_FALSE);

        // Draw the loaded models
        //desk model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.5f, 3.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// It's a bit too big for our scene, so scale it down
        lightingShader.SetUniform4fv("model", model, GL_FALSE);
        lightingShader.SetUniform1i("texture_enable", 1);
        benchModel.Draw(lightingShader);
        

        lightingShader.Bind();
        //cup model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 3.0f, 3.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        lightingShader.SetUniform4fv("model", model, GL_FALSE);
        lightingShader.SetUniform1i("texture_enable", 0);
        cupModel.Draw(lightingShader);

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

inline void drawSkybox(glm::mat4 view, glm::mat4 projection, Shader skyboxShader, VertexArray skyboxVA, GLuint skyboxTexture) {
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

