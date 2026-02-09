#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

//mod for the model's pos
float movMod = 0.0015f;

bool AKeyHeld = false, DKeyHeld = false, WKeyHeld = false, SKeyHeld = false;
bool UpKeyHeld = false, DownKeyHeld = false, RightKeyHeld = false, LeftKeyHeld = false;
bool QKeyHeld = false, EKeyHeld = false;

float x = 0, y = 0, z = 0;
float scaleX = 1.f, scaleY = 1.f, scaleZ = 1.f;
float theta = 0, axisX = 0, axisY = 1, axisZ = 0;
float rotX = 0.0f, rotY = 0.0f, rotSpeed = 0.0005f;

void Key_Callback(GLFWwindow* window,
                int key,
                int scancode,
                int action,
                int mods)
{
    //W
    if (key == GLFW_KEY_W && action == GLFW_PRESS)              WKeyHeld = true;
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE)       WKeyHeld = false;
    //A
    if (key == GLFW_KEY_A && action == GLFW_PRESS)              AKeyHeld = true;
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)       AKeyHeld = false;
    //S
    if (key == GLFW_KEY_S && action == GLFW_PRESS)              SKeyHeld = true;
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)       SKeyHeld = false;
    //D
    if (key == GLFW_KEY_D && action == GLFW_PRESS)              DKeyHeld = true;
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)       DKeyHeld = false;

    //Q
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)              QKeyHeld = true;
    else if (key == GLFW_KEY_Q && action == GLFW_RELEASE)       QKeyHeld = false;
    //E
    if (key == GLFW_KEY_E && action == GLFW_PRESS)              EKeyHeld = true;
    else if (key == GLFW_KEY_E && action == GLFW_RELEASE)       EKeyHeld = false;

    //UP
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)             UpKeyHeld = true;
    else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)      UpKeyHeld = false;
    //DOWN
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)           DownKeyHeld = true;
    else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)    DownKeyHeld = false;
    //LEFT
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)           LeftKeyHeld = true;
    else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)    LeftKeyHeld = false;
    //RIGHT
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)          RightKeyHeld = true;
    else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)   RightKeyHeld = false;
}

void InputHandling()
{
    //Translation
    if (AKeyHeld) x -= movMod;
    if (DKeyHeld) x += movMod;
    if (WKeyHeld) y += movMod; //z -= movMod;
    if (SKeyHeld) y -= movMod; //z += movMod;

    //Rotation
    if (LeftKeyHeld)    rotY -= rotSpeed;
    if (RightKeyHeld)   rotY += rotSpeed;
    if (UpKeyHeld)      rotX -= rotSpeed;
    if (DownKeyHeld)    rotX += rotSpeed;

    //Scale
    if (QKeyHeld){
        scaleX -= movMod;
        scaleY -= movMod;
        scaleZ -= movMod;
    }
    if (EKeyHeld) {
        scaleX += movMod;
        scaleY += movMod;
        scaleZ += movMod;
    }
}


GLfloat UV[]{
    0.f, 1.f,
    0.f, 0.f,
    1.f, 1.f,
    1.f, 0.f,
    1.f, 1.f,
    1.f, 0.f,
    0.f, 1.f,
    0.f, 0.f
};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float windowWidth = 800;
    float windowHeight = 800;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "EJ Taylan", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    /*-----------------------------------------------------LOADING TEXTURES-----------------------------------------------------------*/
    int img_width, img_height, colorChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes =
        stbi_load("3D/ayaya.png", &img_width, &img_height, &colorChannels, 0);

    //OpenGL ref to the tex
    GLuint texture;
    glGenTextures(1, &texture);
    //Set the curr tex we're working on to Texture 0
    glActiveTexture(GL_TEXTURE0);
    //Bind our next tasks to Tex0 to our curr ref
    //Similar to what did with VBOs
    glBindTexture(GL_TEXTURE_2D, texture);

    //Assing the loaded tex to the OpenGL ref
    glTexImage2D(GL_TEXTURE_2D,
        0,                  //Texture 0
        GL_RGBA,            //Target color format of the tex
        img_width,          
        img_height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        tex_bytes);         //Loaded tex in bytes

    //Generate mipmaps to the curr tex
    glGenerateMipmap(GL_TEXTURE_2D);
    //Free up loaded bytes
    stbi_image_free(tex_bytes);

    glEnable(GL_DEPTH_TEST);

    /*-----------------------------------------------------TEXTURES DONE-----------------------------------------------------------*/

    /*-----------------------------------------------------LOADING SHADERS-----------------------------------------------------------*/
    //Load shader file into a string stream - VERT file
    std::fstream vertSrc("Shaders/Sample.vert");
    std::stringstream vertBuff;
    //Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();
    //Convert the stream to a char array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //Load shader file into a string stream - FRAG file
    std::fstream fragSrc("Shaders/Sample.frag");
    std::stringstream fragBuff;
    //Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();
    //Convert the stream to a char array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    /*---------------------------------------------------------COMPILING SHADERS--------HAS TO BE AFTER gladLoadGL()--------------------------*/
    //Create a Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Assign the source to the Vertex Shader
    glShaderSource(vertexShader, 1, &v, NULL); //<-used to take &v
    //Compile the vertex Shader
    glCompileShader(vertexShader);

    //Create a Fragment Shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Assign the source to the fragment Shader
    glShaderSource(fragShader, 1, &f, NULL); //<-used to take &f
    //Compile the fragment Shader
    glCompileShader(fragShader);

    //Create the shader program
    GLuint shaderProg = glCreateProgram();
    //attach the compiled vertex shader
    glAttachShader(shaderProg, vertexShader);
    //Attach the compiled frag shader
    glAttachShader(shaderProg, fragShader);

    //Finalize compilation
    glLinkProgram(shaderProg);
    /*-------------------------------------------------------------SHADERS DONE--------------------------------------------------------------*/

    //For user input
    glfwSetKeyCallback(window, Key_Callback);

    GLuint indices[](
        0,1,2
    );

    //create a 3x3 identity matrix
    glm::mat3 identity_matrix3 = glm::mat3(1.0f);
    //create 4x4 identity matrix
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);

    /*----------------------------------------------------------------LOADING OBJECT-----------------------------------------------------------*/
    std::string path = "3D/myCube.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &error,
        path.c_str()
    );

    //EBO array
    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) //loop for indices
    {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO); //how many vaos, dump them where
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_UV);      //how many vbos, dump them where
    glGenBuffers(1, &EBO);

    //currVAO = null
    glBindVertexArray(VAO); //bind to where
    //currVAO = VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*----------------------------------------------POSITION BUFFER--------------------------------------s*/
    //Assigns data to currVBO
    glBufferData(GL_ARRAY_BUFFER,                       //type of buffer
        sizeof(GLfloat) * attributes.vertices.size(),   //total size in bytes
        &attributes.vertices[0],                        //data
        //Optimization only
        GL_STATIC_DRAW                                  //static if almost no movement, Dynamic if constantly moving
    );

    //Tell how to interpret data
    glVertexAttribPointer(
        //0 = Position
        0,                  //Index / Buffer Index
        3,                  //x,y,z
        GL_FLOAT,           //array of floats
        GL_FALSE,           //If its normalized
        3*sizeof(float),    //size of data per vertex
        (void*)0
    );

    //Enable index 0
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    /*----------------------------------------------TEXTURE BUFFER------------------------------------------------*/

    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV); //define what type of buffer, bind to where
    //currVBO = VBO <- GL_ARRAY_BUFFER
    //currVAO.push(currVBO)

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(float) * (sizeof(UV) / sizeof(UV[0])),
        &UV[0],
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(2);

    //currVAO = VAO
    glBindVertexArray(0); //bind to where
    //currVAO = null

    /*------------------------------------------------------------PROJECTIONS----------------------------------------------*/
    //glm::mat4 proj = glm::ortho(
    //    -2.f,   //L
    //    2.f,    //R
    //    -2.f,   //D
    //    2.f,    //U
    //    -1.f,   //Near
    //    1.f     //Far
    //);

    glm::mat4 proj = glm::perspective(
        glm::radians(60.f), //FOV
        windowWidth/windowHeight, //Aspect ratio
        0.1f,   //Distance to cam Near, must be > 0
        100.f   //Distance to cam Far
    );

    z = -5.f;
    /*------------------------------------------------------------PROJECTIONS----------------------------------------------*/


    //--------------------------------MAIN LOOP HERE--------------------------------------------
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        InputHandling();

        //Camera Position
        glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 10.f);
        glm::mat4 cameraPosMatrix = 
            glm::translate(identity_matrix4, cameraPos * -1.f);

        //Camera Orientation
        glm::vec3 WorldUp = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cameraCenter = glm::vec3(0.f, 3.f, 0.f);

        /*------------------------------------------------BUILDING CAMERA ROTATION MATRIX---------------------------------------------
        //F
        glm::vec3 F = cameraCenter - cameraPos;
        F = glm::normalize(F);
        //R
        glm::vec3 R = glm::cross(F, WorldUp);
        //U
        glm::vec3 U = glm::cross(R, F);

        //Construct orientation matrix
        glm::mat4 cameraRotMatrix = identity_matrix4;
        //Matrix [Col][Row]
        //R
        cameraRotMatrix[0][0] = R.x;
        cameraRotMatrix[1][0] = R.y;
        cameraRotMatrix[2][0] = R.z;
        //U
        cameraRotMatrix[0][1] = U.x;
        cameraRotMatrix[1][1] = U.y;
        cameraRotMatrix[2][1] = U.z;
        //-F
        cameraRotMatrix[0][2] = -F.x;
        cameraRotMatrix[1][2] = -F.y;
        cameraRotMatrix[2][2] = -F.z;
        
        //V = cR * cP
        //glm::mat4 view = cameraRotMatrix * cameraPosMatrix;
        -------------------------------------------------------SUMMARIZED BY LINE BELOW----------------------------------------------*/

        glm::mat4 view = glm::lookAt(cameraPos, cameraCenter, WorldUp);

        //start with the translation matrix
        glm::mat4 transformation_matrix = glm::translate(identity_matrix4, 
            glm::vec3(x, y, z));

        //mult the result with scale matrix
        transformation_matrix = glm::scale(transformation_matrix, 
            glm::vec3(scaleX, scaleY, scaleZ));

        //finally, mult with rot matrix
        /*transformation_matrix = glm::rotate(transformation_matrix, 
            glm::radians(theta), glm::normalize(glm::vec3(axisX, axisY, axisZ)));*/

        transformation_matrix = glm::rotate(transformation_matrix, rotX, glm::vec3(1.0f, 0.0f, 0.0f)); //To enable rotation on both axes
        transformation_matrix = glm::rotate(transformation_matrix, rotY, glm::vec3(0.0f, 1.0f, 0.0f));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        //Tell openGL to use this shader
        //for the vaos below ---- every model after this will use this shader
        glUseProgram(shaderProg);

        //currVAO = null
        glBindVertexArray(VAO);
        //currVAO = VAO
        
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


//--------------------------------------MAKING A TRIANGLE----------------------------------------------
//Before main
//TRIANGLE
//GLfloat vertices[]{ 
//        0.f, 0.5f, 0.f,
//        -0.5f, -0.5f, 0.f,
//        0.5f, -0.5f, 0.f
//};
//In main loop
// glBegin(GL_TRIANGLE_FAN);
//       /* glVertex2f(-0.5f, -0.5f);
//        glVertex2f(0.0f, 0.5f);
//        glVertex2f(0.5f, -0.5f);*/
//
//        //Creating a nonagon
//        int sides = 9, degs = 40;
//        float centerX = 0.0f, centerY = 0.5f, radius = 0.5f;
//        float angle = degs * M_PI / 180; //deg to rads
//
//        for (int i = 0; i < sides; i++)
//        {
//            //Formula for vertex is x = Cx + R * cos(i * 40deg)
//            glVertex2f(centerX + radius * cos(i * angle),
//                centerY + radius * sin(i * angle));
//        }
//
//        glEnd();
//---------------------------------------DEPRECATED-----------------------------------------------------


//---------------------------------------CREATING A MATRIX----------------------------------------------
void sampleMatrixTransformations()
{
    //create a 3x3 identity matrix
    glm::mat3 identity_matrix3 = glm::mat3(1.0f);
    //create 4x4 identity matrix
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);

    //creates 3d translation matrix
    glm::mat4 translation =
        glm::translate(identity_matrix4,    //usually start with identity
            glm::vec3(x, y, z));   //move by xyz units from center

    glm::mat4 scale =
        glm::scale(identity_matrix4,        //usually start with identity
            glm::vec3(x, y, z));    //scale by xyz units from center

    glm::mat4 rotation =
        glm::rotate(identity_matrix4,        //usually start with identity
            glm::radians(theta),    //rotate by theta deg
            glm::vec3(x, y, z));    //xyz component of normalized axis vector
}
//------------------------------------------MATRIX END--------------------------------------------------