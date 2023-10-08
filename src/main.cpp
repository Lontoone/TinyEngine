#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "basic/Mesh.h"
#include "basic/Fastsetup.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "basic/camera.h"
#include "basic/Hierarchy.h"
//#include "basic/GameObject.h"
//#include "basic/TransformObject.h"

#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)

void init();
void update();
void render();
void input();

using namespace std;
void processInput(GLFWwindow* window, double dt);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
MechainState state;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float SCR_WIDTH		= 800;
float SCR_HEIGHT	= 600;

float vertices[] = {
		-0.5f , 1.0f , 0.0f,
		0.0f , 0.5f , 0.0f,
		0.5f , 0.0f , 0.0f,
};


static string src_path = EXPAND(_PRJ_SRC_PATH);  //TODO: move to manager script


int main(int argc , char** argv) {
	std::cout << "hello"<<endl<<endl;

	init();  // TODO: init

#pragma region LoadWindow

	// 檢查
	GLFWwindow* window = glfwCreateWindow(800, 600, "title", NULL, NULL); //monitor先設為NULL
	if (window == NULL) {
		cout << "No Window" << endl;
		glfwTerminate();
		return -1;
	}
	//把window放到thread上
	glfwMakeContextCurrent(window);

	gladLoadGL();	
	SetProgram(state , 
		src_path + string("\\assets\\shaders\\vert.glsl"),
		src_path + string("\\assets\\shaders\\frag.glsl"));
	/*
	*/	
	Shader s_default_shader(
		src_path + string("\\assets\\shaders"),
		"default"
	);
	s_default_shader.activate();
	cout << "activate pg " << s_default_shader.m_state.programId << endl;

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed init glad" << endl;
		glfwTerminate();
		return-1;
	}
	//定義視窗
	glViewport(0, 0, 800, 600); //(x, y , width , height)

	//指定視窗resize的處理方法
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);
#pragma endregion
	
#pragma region Test_Hirerarchy

	//GameObject* obj = new GameObject();
	GameObject obj = GameObject();
	//Mesh mesh = Mesh(src_path + "\\assets\\TrollApose_low.fbx", s_default_shader);
	Mesh mesh = Mesh(src_path + "\\assets\\models\\sponza\\sponza.obj" , s_default_shader);
	//Mesh mesh = Mesh(src_path + "\\assets\\sponza.obj");
	//Mesh mesh = Mesh(src_path + "\\assets\\Room1.obj");
	//Mesh mmesh = Mesh();
	//Component* model_comp = dynamic_cast<Component*>(&mmesh);
	//Component* test_comp = new Component();
	Component test_comp = Component();
	obj.set_name("New Obj ");
	obj.add_component(&test_comp);
	obj.add_component(&mesh);
	//obj.execute();
	Hierarchy hierarchy = Hierarchy();
	//hierarchy.add_main_camera();
	hierarchy.add_object(&obj);
	hierarchy.execute();

#pragma endregion


	while (!glfwWindowShouldClose(window)) { // 等到console 送出kill flag
		processInput(window , 0.1f);
		//glUseProgram(state.programId);
		s_default_shader.activate();

		//update();
		//-------------- [ TEMP 暫時的MVP ] ------------------
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-30.0f), glm::vec3(0.5f));
		model = glm::scale(model, vec3(0.1f));
		//view = glm::translate(view, glm::vec3( -0 , -0 , -3 ));  // view已轉換坐標系統
		view = camera.getViewMatrix();// view已轉換坐標系統
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
		/*
		glUniformMatrix4fv(glGetUniformLocation(state.programId, "model"), 1, GL_FALSE, value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(state.programId, "view"), 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(state.programId, "projection"), 1, GL_FALSE, value_ptr(projection));
		*/
		glUniformMatrix4fv(glGetUniformLocation(s_default_shader.m_state.programId, "model"), 1, GL_FALSE, value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(s_default_shader.m_state.programId, "view"), 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(s_default_shader.m_state.programId, "projection"), 1, GL_FALSE, value_ptr(projection));

		//清除顏色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		hierarchy.execute();
		//mesh.Render();
		//glBindVertexArray(VAO);
		//glUseProgram(state.programId);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//--------------------------------------------

		glfwSwapBuffers(window); //GPU在渲染當前的frame時，會準備下一個frame，當此frame結束時直接swap過去。
		glfwPollEvents(); // processes all pending events。 (處理如input....等事件)
	}

	glfwTerminate(); //Close


	return 0;
}
void init() {
	glfwInit();

	src_path.erase(0, 1); // erase the first quote
	src_path.erase(src_path.size() - 1); // erase the last quote and the dot	
	printf(src_path.c_str());

	// open gl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //版本3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //版本x.3

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}
void update() {}
void render() {}
void input() {}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window, double dt) {
	//若按下esc key => 關掉window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::FORWARD, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::BACKWARD, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::RIGHT, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::LEFT, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::UP, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::DOWN, dt);
	}
}
