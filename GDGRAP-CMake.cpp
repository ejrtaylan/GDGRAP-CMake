#include <GLFW/glfw3.h>
#include <cmath>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "GDGRAPH", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLE_FAN);
       /* glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);*/

        //Creating a nonagon
        int sides = 9;
        float centerX = 0.0f, centerY = 0.0f, radius = 0.5f;
        float angle = 40 * 3.1415 / 180; //deg to radse

        for (int i = 0; i < sides; i++)
        {
            //Formula for vertex is x = Cx + R * cos(i * 40deg)
            glVertex2f(centerX + radius * cos(i * angle),
                centerY + radius * sin(i * angle));
        }

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}