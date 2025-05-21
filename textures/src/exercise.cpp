#include <glad/glad.h> 
#include <iostream> 
#include <math.h>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos, 1.0);\n"
    "ourColor = aColor;\n"
    "TexCoord = aTexCoord;\n"
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
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture1;\n"
    "void main()\n"
    "{\n"
      "FragColor = texture(texture1, TexCoord) * vec4 (ourColor, 1.0);\n"
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
        // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };

  unsigned int indices[] = {  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  float texCoords[]{
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.5f, 1.0f
  };

  //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
  
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

if (FILE *file = fopen("wall.jpg", "r")) {
  std::cout << "Found wall.jpg!\n";
  fclose(file);
} else {
  std::cerr << "Could not find wall.jpg in working directory!\n";
}

if (data) {
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    else {
        std::cout << "Unexpected number of channels: " << nrChannels << std::endl;
        format = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
} else {
    std::cout << "Failed to load texture" << std::endl;
}
stbi_image_free(data);

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
  // Use shader program.
  unsigned int shaderProgram0 = glCreateProgram();
  createVertexFragProgram(shaderProgram0, vertexShader0, fragmentShader0, "prog0");
  unsigned int shaderProgram1 = glCreateProgram();
  createVertexFragProgram(shaderProgram1, vertexShader1, fragmentShader1, "prog1");

  glDeleteShader(vertexShader0);
  glDeleteShader(fragmentShader0);
  glDeleteShader(vertexShader1);
  glDeleteShader(fragmentShader1);

  // Create names for VAO VBO, generate buffers in context
  unsigned int VBO0, VAO0, VBO1, VAO1, EBO1;
  loadTriangle(VAO0, VBO0, vertices0, 3);

  glGenVertexArrays(1, &VAO1);
  glGenBuffers(1, &VBO1);
  glGenBuffers(1, &EBO1);

  glBindVertexArray(VAO1);

  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw 2 triangles via the shader program and VAO
    glUseProgram(shaderProgram0);
    glBindVertexArray(VAO0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // render container
    glUseProgram(shaderProgram1);
    glUniform1i(glGetUniformLocation(shaderProgram1, "texture1"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO1);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // De-allocate
  glDeleteVertexArrays(1, &VAO0);
  glDeleteBuffers(1, &VBO0);
  glDeleteProgram(shaderProgram0);

  glfwTerminate();
  return 0;
}
