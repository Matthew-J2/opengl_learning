#include <iostream>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource0 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
"{\n"
    "FragColor = vec4(1.0f, 0.8f, 0.8f, 1.0f);\n"
"}\0";

const char *fragmentShaderSource1 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
"{\n"
    "FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void checkCompile(GLuint shader, const char* name) {
    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char buf[1024];
        glGetShaderInfoLog(shader, sizeof(buf), nullptr, buf);
        std::cerr << "[ERROR] Shader compile failed (" << name << "):\n"
                  << buf << "\n";
    }
}

void checkLink(GLuint program, const char* name) {
    GLint ok = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char buf[1024];
        glGetProgramInfoLog(program, sizeof(buf), nullptr, buf);
        std::cerr << "[ERROR] Program link failed (" << name << "):\n"
                  << buf << "\n";
    }
}

int main(){
	std::cout << "Helloooo\n";

	// Initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create a window with GLFW
	GLFWwindow* window = glfwCreateWindow(800,600,"Sevy Window", NULL, NULL);
	if (window == NULL){
		std::cout << "Could not make a GLFW window";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// Set viewport
	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// Triangle coords
	float vertices0[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f,  // right 
		0.0f,  0.5f, 0.0f,  // top   
	};
	float vertices1[] = {
		0.7f, 0.8f, 0.0f,   // top
		0.6f, 0.6f, 0.0f,   // left
		0.8f, 0.6f, 0.0f,   // right
	};

	// Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkCompile(vertexShader,   "vertex");
	
	// Fragment shader
	unsigned int fragmentShader0 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader0, 1, &fragmentShaderSource0, NULL);
	glCompileShader(fragmentShader0);
	checkCompile(fragmentShader0,   "frag0");

	unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);
	checkCompile(fragmentShader1,   "frag1");

	// Link both shaders then delete the individual shader objects once linked. Use shader program.
	unsigned int shaderProgram0 = glCreateProgram();
	glAttachShader(shaderProgram0, vertexShader);
	glAttachShader(shaderProgram0, fragmentShader0);
	glLinkProgram(shaderProgram0);
	checkLink(shaderProgram0, "prog0");
	glUseProgram(shaderProgram0);

	// Link both shaders then delete the individual shader objects once linked. Use shader program.
	unsigned int shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);
	checkLink(shaderProgram1, "prog1");
	glUseProgram(shaderProgram1);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader0);
	glDeleteShader(fragmentShader1);

	// Create names for VAO VBO, generate buffers in context
	unsigned int VBO0, VAO0, VBO1, VAO1;
	glGenVertexArrays(1, &VAO0);
	glGenBuffers(1, &VBO0);
	// Bind VAO 
	glBindVertexArray(VAO0);
	
	// Load vertex data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
	
	// Describe vertex layout to GPU
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0); 

	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glBindVertexArray(VAO1);

	// Load vertex data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	// Describe vertex layout to GPU
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while(!glfwWindowShouldClose(window)){
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
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// De-allocate
	glDeleteVertexArrays(1, &VAO0);
	glDeleteBuffers(1, &VBO0);
	glDeleteProgram(shaderProgram0);
	glDeleteProgram(shaderProgram1);


	glfwTerminate();
	return 0;
}
