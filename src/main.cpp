#include <GLFW/glfw3.h>
#include <cmath>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    int cnt = 0;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        const float color[] = {(float)std::sin(cnt) * 0.5f + 0.5f,
                                (float)std::cos(cnt) * 0.5f + 0.5f,
                                 0.0f,
                                 1.0f
                                };
        glClearColor(color[0], color[1], color[2], color[3]);

        cnt++;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}