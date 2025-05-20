#include <glad/glad.h>
#include <iostream>

#include <GLFW/glfw3.h>

const char *vertexShaderSource0 =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *vertexShaderSource1 =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor;"
    "void main()\n"
    "{\n"
      "gl_Position = vec4(aPos, 1.0);\n"
      "vertexColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "}\0";

const char *fragmentShaderSource0 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
      "FragColor = vec4(1.0f, 0.8f, 0.8f, 1.0f);\n"
    "}\0";

const char *fragmentShaderSource1 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
      "FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec4 vertexColor;\n"
  "void main()\n"
  "{\n"
    "FragColor = vertexColor;\n"
  "}\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void checkCompile(GLuint shader, const char *name) {
  GLint ok = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    char buf[1024];
    glGetShaderInfoLog(shader, sizeof(buf), nullptr, buf);
    std::cerr << "[ERROR] Shader compile failed (" << name << "):\n"
              << buf << "\n";
  }
}

void checkLink(GLuint program, const char *name) {
  GLint ok = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &ok);
  if (!ok) {
    char buf[1024];
    glGetProgramInfoLog(program, sizeof(buf), nullptr, buf);
    std::cerr << "[ERROR] Program link failed (" << name << "):\n"
              << buf << "\n";
  }
}

void createShader(unsigned int* shader, const char** shader_src,
                  const char* shader_name) {
  glShaderSource(*shader, 1, shader_src, NULL);
  glCompileShader(*shader);
  checkCompile(*shader, shader_name);
}

void createVertexFragProgram(unsigned int shaderProgram, unsigned int vertexProgram, unsigned int fragmentProgram, const char* program_name){
  glAttachShader(shaderProgram, vertexProgram);
  glAttachShader(shaderProgram, fragmentProgram);
  glLinkProgram(shaderProgram);
  checkLink(shaderProgram, program_name);
  glUseProgram(shaderProgram);
}

void loadTriangle(unsigned int &VAO, unsigned int &VBO, const float *vertices, size_t vertexCount){
    // Create objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO then VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Upload data
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    // Describe layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO for safety
    glBindVertexArray(0);
}
int main() {
  std::cout << "Helloooo hello\n";

  // Initialize and configure GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window with GLFW
  GLFWwindow *window = glfwCreateWindow(800, 600, "Sevy Window", NULL, NULL);
  if (window == NULL) {
    std::cout << "Could not make a GLFW window";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // Set viewport
  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Triangle coords
  float vertices0[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f, // top
  };
  float vertices1[] = {
      0.7f, 0.8f, 0.0f, // top
      0.6f, 0.6f, 0.0f, // left
      0.8f, 0.6f, 0.0f, // right
  };

  float vertices2[] = {
      0.3f, 0.4f, 0.0f, // top
      0.2f, 0.2f, 0.0f, // left
      0.4f, 0.2f, 0.0f, // right
  };

  // Vertex shaders
  unsigned int vertexShader0 = glCreateShader(GL_VERTEX_SHADER);
  createShader(&vertexShader0, &vertexShaderSource0, "vertex0");
  unsigned int vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
  createShader(&vertexShader1, &vertexShaderSource1, "vertex1");

  // Fragment shaders
  unsigned int fragmentShader0 = glCreateShader(GL_FRAGMENT_SHADER);
  createShader(&fragmentShader0, &fragmentShaderSource0, "frag0");
  unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
  createShader(&fragmentShader1, &fragmentShaderSource1, "frag1");
  unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  createShader(&fragmentShader2, &fragmentShaderSource2, "frag2");

  // Link shaders then delete the individual shader objects once linked.
  // Use shader program.
  unsigned int shaderProgram0 = glCreateProgram();
  createVertexFragProgram(shaderProgram0, vertexShader0, fragmentShader0, "prog0");

  unsigned int shaderProgram1 = glCreateProgram();
  createVertexFragProgram(shaderProgram1, vertexShader1, fragmentShader1, "prog1");

  unsigned int shaderProgram2 = glCreateProgram();
  createVertexFragProgram(shaderProgram2, vertexShader1, fragmentShader2, "prog2");

  glDeleteShader(vertexShader0);
  glDeleteShader(fragmentShader0);
  glDeleteShader(fragmentShader1);
  glDeleteShader(vertexShader1);
  glDeleteShader(fragmentShader2);

  // Create names for VAO VBO, generate buffers in context
  unsigned int VBO0, VAO0, VBO1, VAO1, VBO2, VAO2;
  loadTriangle(VAO0, VBO0, vertices0, 3);
  // Unbind
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);
  loadTriangle(VAO1, VBO1, vertices1, 3);
  loadTriangle(VAO2, VBO2, vertices2, 3);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw 2 triangles via the shader program and VAO
    glUseProgram(shaderProgram0);
    glBindVertexArray(VAO0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(shaderProgram1);
    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(shaderProgram2);
    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // De-allocate
  glDeleteVertexArrays(1, &VAO0);
  glDeleteBuffers(1, &VBO0);
  glDeleteProgram(shaderProgram0);
  glDeleteVertexArrays(1, &VAO1);
  glDeleteBuffers(1, &VBO1);
  glDeleteProgram(shaderProgram1);
  glDeleteVertexArrays(1, &VAO2);
  glDeleteBuffers(1, &VBO2);
  glDeleteProgram(shaderProgram2);


  glfwTerminate();
  return 0;
}
