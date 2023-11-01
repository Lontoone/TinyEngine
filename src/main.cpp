#include <iostream>
#include <filesystem>
#include "ui/UiManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "basic/Mesh.h"
#include "basic/Fastsetup.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "basic/camera.h"
#include "basic/Hierarchy.h"
#include "basic/GameObject.h"
#include "FileDialog.h"
#include <Debugger.hpp>
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
float SCR_WIDTH		= 1280;
float SCR_HEIGHT	= 600;

float vertices[] = {
		-0.5f , 1.0f , 0.0f,
		0.0f , 0.5f , 0.0f,
		0.5f , 0.0f , 0.0f,
};


static string src_path = EXPAND(_PRJ_SRC_PATH);  //TODO: move to manager script
UiManager ui_manager = UiManager();
Hierarchy Hierarchy::sInstance;
using namespace std;
int main(int argc , char** argv) {
	std::cout << "hello"<<endl;	
	init();  // TODO: init

	double _previous_time = 0;
	float time_scale = 1;
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
	ui_manager.init_imgui(window);
	gladLoadGL();	
	/*
	SetProgram(state , 
		src_path + string("\\assets\\shaders\\vert.glsl"),
		src_path + string("\\assets\\shaders\\frag.glsl"));
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
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //(x, y , width , height)

	//指定視窗resize的處理方法
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); // counter clock wise
#pragma endregion
	
#pragma region DEBUG_PRE_LOAD_MODEL
	GameObject obj = GameObject("Building");
	GameObject camera_obj = GameObject((TransformObject*)&camera);
	camera_obj.set_name("Camera");
	Mesh* mesh =new Mesh(src_path + "\\assets\\models\\sponza\\sponza.obj" , s_default_shader);		
	//Mesh*  mesh = new Mesh(src_path + "\\assets\\models\\cute_dog\\cute_dg.obj", s_default_shader);
	obj.add_component(mesh);	
	camera_obj.add_component((Component*)&camera);

	Hierarchy::instance().add_object(&obj);
	//Hierarchy::instance().add_object((GameObject*)&camera);
	Hierarchy::instance().add_object(&camera_obj);
	/*
	Mesh* dog_mesh = new Mesh(src_path + "\\assets\\models\\cute_dog\\cute_dg.obj", s_default_shader);
	Mesh* head_mesh = new Mesh(src_path + "\\assets\\models\\hw1_robot\\head.obj", s_default_shader);	
	Mesh* body_mesh = new Mesh(src_path + "\\assets\\models\\hw1_robot\\body.obj", s_default_shader);
	Mesh* leg1_mesh = new Mesh(src_path + "\\assets\\models\\hw1_robot\\leg1.obj", s_default_shader);	
	Mesh* leg2_mesh = new Mesh(src_path + "\\assets\\models\\hw1_robot\\leg2.obj", s_default_shader);
	Mesh* leg3_mesh = new Mesh(src_path + "\\assets\\models\\hw1_robot\\leg3.obj", s_default_shader);

	GameObject dog = GameObject("dog");
	dog.add_component(dog_mesh);
	dog.m_transform->m_scale = vec3(1.5,1.5,1.5);
	dog.m_transform->m_position = vec3(0.33, 1.8,0);
	Hierarchy::instance().add_object(&dog);

	GameObject robot_empty = GameObject("ROOT");
	Hierarchy::instance().add_object(&robot_empty);

	GameObject robot_head = GameObject("Robot_Head");
	robot_head.add_component(head_mesh);
	Hierarchy::instance().add_object(&robot_head);

	GameObject robot_body = GameObject("Robot_Body");
	robot_body.add_component(body_mesh);
	Hierarchy::instance().add_object(&robot_body);


	GameObject robot_leg_r1_f = GameObject("Robot_leg_r1");
	robot_leg_r1_f.add_component(leg1_mesh);
	Hierarchy::instance().add_object(&robot_leg_r1_f);

	GameObject robot_leg_r2_f = GameObject("Robot_leg_r2");
	robot_leg_r2_f.add_component(leg2_mesh);
	Hierarchy::instance().add_object(&robot_leg_r2_f);

	GameObject robot_leg_r3_f = GameObject("Robot_leg_r3");
	robot_leg_r3_f.add_component(leg3_mesh);
	Hierarchy::instance().add_object(&robot_leg_r3_f);

	robot_body.m_transform->set_transform_parent(robot_empty.m_transform);
	robot_head.m_transform->set_transform_parent(robot_body.m_transform);
	robot_leg_r1_f.m_transform->set_transform_parent(robot_body.m_transform);
	robot_leg_r2_f.m_transform->set_transform_parent(robot_leg_r1_f.m_transform);
	robot_leg_r3_f.m_transform->set_transform_parent(robot_leg_r2_f.m_transform);
	dog.m_transform->set_transform_parent(robot_head.m_transform);

	robot_leg_r3_f.m_transform->move(vec3(0.25,0.2,0.3));
	robot_leg_r2_f.m_transform->move(vec3(0, -0.5, 0));
	robot_leg_r1_f.m_transform->move(vec3(0.5, -0.1, 0.325));
	dog.m_transform->move(vec3(0, 0.1,0.025));

	///////////////// FRONT LEFT LEG //////////////////////
	GameObject robot_leg_l1_f = GameObject("Robot_leg_l1-front");	
	GameObject robot_leg_l2_f = GameObject("Robot_leg_l2-front");
	GameObject robot_leg_l3_f = GameObject("Robot_leg_l3-front");
	robot_leg_l1_f.add_component(leg1_mesh->copy());
	robot_leg_l3_f.add_component(leg3_mesh->copy());
	robot_leg_l2_f.add_component(leg2_mesh->copy());
	Hierarchy::instance().add_object(&robot_leg_l1_f);
	Hierarchy::instance().add_object(&robot_leg_l2_f);
	Hierarchy::instance().add_object(&robot_leg_l3_f);

	robot_leg_l1_f.m_transform->set_transform_parent(robot_body.m_transform);
	robot_leg_l2_f.m_transform->set_transform_parent(robot_leg_l1_f.m_transform);
	robot_leg_l3_f.m_transform->set_transform_parent(robot_leg_l2_f.m_transform);

	robot_leg_l1_f.m_transform->move(vec3(-0.5, -0.1, 0.325));
	robot_leg_l1_f.m_transform->m_scale =vec3(-1, 1, 1);
	robot_leg_l2_f.m_transform->move(vec3(0, -0.5, 0));
	robot_leg_l3_f.m_transform->move(vec3(0.25, 0.2, 0.3));
	////////////////////////////////////////////////////////

	///////////////// BACK LEFT LEG //////////////////////
	GameObject robot_leg_l1_b = GameObject("Robot_leg_l1-back");
	GameObject robot_leg_l2_b = GameObject("Robot_leg_l2-back");
	GameObject robot_leg_l3_b = GameObject("Robot_leg_l3-back");
	robot_leg_l1_b.add_component(leg1_mesh->copy());
	robot_leg_l3_b.add_component(leg3_mesh->copy());
	robot_leg_l2_b.add_component(leg2_mesh->copy());
	Hierarchy::instance().add_object(&robot_leg_l1_b);
	Hierarchy::instance().add_object(&robot_leg_l2_b);
	Hierarchy::instance().add_object(&robot_leg_l3_b);

	robot_leg_l1_b.m_transform->set_transform_parent(robot_body.m_transform);
	robot_leg_l2_b.m_transform->set_transform_parent(robot_leg_l1_b.m_transform);
	robot_leg_l3_b.m_transform->set_transform_parent(robot_leg_l2_b.m_transform);

	robot_leg_l1_b.m_transform->move(vec3(-0.5, -0.1, -0.325));
	robot_leg_l1_b.m_transform->m_scale = vec3(-1, 1, -1);
	robot_leg_l2_b.m_transform->move(vec3(0, -0.5, 0));
	robot_leg_l3_b.m_transform->move(vec3(0.25, 0.2, 0.3));
	////////////////////////////////////////////////////////

	///////////////// BACK RIGHT LEG //////////////////////
	GameObject robot_leg_r1_b = GameObject("Robot_leg_r1-back");
	GameObject robot_leg_r2_b = GameObject("Robot_leg_r2-back");
	GameObject robot_leg_r3_b = GameObject("Robot_leg_r3-back");
	robot_leg_r1_b.add_component(leg1_mesh->copy());
	robot_leg_r3_b.add_component(leg3_mesh->copy());
	robot_leg_r2_b.add_component(leg2_mesh->copy());
	Hierarchy::instance().add_object(&robot_leg_r1_b);
	Hierarchy::instance().add_object(&robot_leg_r2_b);
	Hierarchy::instance().add_object(&robot_leg_r3_b);

	robot_leg_r1_b.m_transform->set_transform_parent(robot_body.m_transform);
	robot_leg_r2_b.m_transform->set_transform_parent(robot_leg_r1_b.m_transform);
	robot_leg_r3_b.m_transform->set_transform_parent(robot_leg_r2_b.m_transform);

	robot_leg_r1_b.m_transform->move(vec3(0.5, -0.1, -0.325));
	robot_leg_r1_b.m_transform->m_scale = vec3(1, 1, -1);
	robot_leg_r1_b.m_transform->m_rotation= vec3(0, 90, 0);
	robot_leg_r2_b.m_transform->move(vec3(0, -0.5, 0));
	robot_leg_r3_b.m_transform->move(vec3(0.25, 0.2, 0.3));
	////////////////////////////////////////////////////////
	*/
	/*  //////  HAVE BUG, NoT FIX YET
	vector<GameObject*> robot_leg_clone = robot_leg_r1.clone();
	for (auto _copy : robot_leg_clone) {
		Hierarchy::instance().add_object(_copy);
	
	}*/


#pragma endregion

#pragma region SET_UP_MENU
	////////////////// Add Load model opts: /////////////////
	auto pos_inp = [&]() { if (Button("Open Model")) {
		char output_path[260];
		bool has_selected = FileDialog::Open(*output_path);

		if (!has_selected)
			return false;
		
		string		_tmp_name		= string( (string("New object") + to_string(Hierarchy::instance().m_game_objects.size())));
		const char* _n				= _tmp_name.c_str();		
		Mesh*		mesh3			= new Mesh(output_path, s_default_shader);
		
		GameObject* obj3			= new GameObject(_n);
		obj3->add_component(mesh3);
		Hierarchy::instance().add_object(obj3);
		return true;
	} };
	ui_manager.m_menu_cmds.push_back(pos_inp);	

	auto animation_inp = [&]() { if (Button("PAUSE/RESUME")) {
		time_scale = abs(1 - time_scale);
		return true;
	} };
	ui_manager.m_menu_cmds.push_back(animation_inp);

#pragma endregion
	
	double delta_time = 0.025 ;
	while (!glfwWindowShouldClose(window)) { // 等到console 送出kill flag
		
		if( time_scale>0)
			_previous_time += delta_time * time_scale;
		
		processInput(window , 0.1f);
		//glUseProgram(state.programId);

		//清除顏色
		ui_manager.new_frame();
		Hierarchy::instance().execute(EXECUTE_TIMING::BEFORE_FRAME);
		/*
		////////////////////// HW1 Animation  ////////////////////////
		robot_leg_l1_f.m_transform->m_rotation = vec3(0, std::sin(_previous_time *4)*50,0);
		robot_leg_l2_f.m_transform->m_rotation = vec3(0, 0,std::abs( std::cos(_previous_time*2))*30);
		robot_leg_l3_f.m_transform->m_rotation = vec3(std::sin(_previous_time *4) * 30,0, 0);

		robot_leg_l1_b.m_transform->m_rotation = vec3(0, std::sin(_previous_time * 4 + 200) * 50, 0);
		robot_leg_l2_b.m_transform->m_rotation = vec3(0, 0, std::abs(std::cos(_previous_time * 2 + 200)) * 30);
		robot_leg_l3_b.m_transform->m_rotation = vec3(std::sin(_previous_time * 4) * 30, 0, 0);

		robot_leg_r1_f.m_transform->m_rotation = vec3(0, std::sin(_previous_time * 4 + 500) * 50, 0);
		robot_leg_r2_f.m_transform->m_rotation = vec3(0, 0, std::abs(std::cos(_previous_time * 2 +500)) * 30);
		robot_leg_r3_f.m_transform->m_rotation = vec3(std::sin(_previous_time * 4) * 30, 0, 0);

		robot_leg_r1_b.m_transform->m_rotation = vec3(0, std::sin(_previous_time * 4 +200) * 50, 0);
		robot_leg_r2_b.m_transform->m_rotation = vec3(0, 0, std::abs(std::cos(_previous_time * 2 +200 )) * 30);
		robot_leg_r3_b.m_transform->m_rotation = vec3(std::sin(_previous_time * 4) * 30, 0, 0);

		robot_head.m_transform->m_rotation = vec3(0 , std::sin(_previous_time *1.25) * 70, 0);
		robot_head.m_transform->m_position = (vec3(0, sin(1.25 * sin(10 * sin(_previous_time *0.2)) *7) *0.01 +0.15, 0));
		
		robot_body.m_transform->m_rotation = vec3(0, std::sin(_previous_time * 0.5) * 15, 0);
		robot_body.m_transform->m_position = (vec3(0, cos(1.25 * cos(10 * cos(_previous_time * 0.2)) * 7) * 0.01 , 0));
		
		dog.m_transform->m_position = (vec3(0, abs( sin(1.25 * sin(2.5 * sin(_previous_time * 0.5 + 500)) * 7)) * 0.1 + 1.7, 0));
		/////////////////////////////////////////////////////////////
		*/

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		s_default_shader.activate();
		//update();
		// 
		//-------------- [ TEMP 暫時的MVP ] ------------------
		glm::mat4 view			= glm::mat4(1.0f);
		glm::mat4 projection	= glm::mat4(1.0f);
		view			= camera.getViewMatrix();
		projection		= glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 1000.0f);
		
		//glUniformMatrix4fv(glGetUniformLocation(s_default_shader.m_state.programId, "model"), 1, GL_FALSE, value_ptr(model));		
		glUniformMatrix4fv(glGetUniformLocation(s_default_shader.m_state.programId, "view"), 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(s_default_shader.m_state.programId, "projection"), 1, GL_FALSE, value_ptr(projection));

		Hierarchy::instance().execute(EXECUTE_TIMING::MAIN_LOGIC);
		
		ui_manager.create_hierarchy_window(Hierarchy::instance().m_game_objects);
		ui_manager.create_menubar();
		ui_manager.render_ui(); 

		//--------------------------------------------

		glfwSwapBuffers(window); //GPU在渲染當前的frame時，會準備下一個frame，當此frame結束時直接swap過去。
		glfwPollEvents(); // processes all pending events。 (處理如input....等事件)
		Hierarchy::instance().execute(EXECUTE_TIMING::AFTER_FRAME);
	}
	ui_manager.destory();
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

//float prex =0 , prey=0;
vec3 prev_mouse = vec3(0);
vec3 mouse_pos = vec3(0);
vec3 endPos = vec3(0,0,-camera.zoom);
mat4 current_camera_model = mat4(0);
mat4 previouseRot = mat4(1);
float scroll_speed = 5;
//vec2 prev_mouse = vec2(0);
static glm::vec2 screenCenter(SCR_WIDTH / 2, SCR_HEIGHT / 2);

void processInput(GLFWwindow* window, double dt) {
	//若按下esc key => 關掉window

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.zoom += dt;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.zoom -= dt;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		float move_x = xpos / SCR_WIDTH * 2 - 1;
		float move_y = -(ypos / SCR_HEIGHT * 2 - 1);

		mouse_pos = vec3(move_x, move_y, 0);
		//Debugger::Log(mouse_pos);

		float op_sqrd = move_x * move_x + move_y * move_y;
		mouse_pos.z = sqrt(1.0f - op_sqrd);
		if (op_sqrd <= 1.0f) {
			mouse_pos.z = sqrt(1.0f - op_sqrd);
		}
		else
		{
			return;
			mouse_pos = glm::normalize(mouse_pos);
		}
		//cout << "mouse pos " << "leng " << glm::length(mouse_pos) << endl;

		if (glm::length(prev_mouse) == 0) {
			prev_mouse = mouse_pos;			
			return;
		}
		if (current_camera_model == mat4(0)) {
			current_camera_model = camera.m_model_matrix;
		}

		vec3 op1 = prev_mouse;
		float angle = glm::acos(glm::min(1.0f, glm::dot(op1, mouse_pos)));
		if(angle ==0 ){
			return;
		}
		//cout << " Angle " << angle << " dot" << dot(op1, mouse_pos) << endl;
		vec3 rotation_axis = glm::cross(op1, mouse_pos);


		//		Get the rotation axis in 3D		
		vec4 zoom_off = vec4( endPos , 1); // start from vec4(0, 0, -camera.zoom, 0);
		mat4 rotate_offset =  glm::rotate(mat4(1.0), angle, rotation_axis); // rotate camera around orbit

		camera.m_position = rotate_offset * zoom_off;

		camera.m_forward = camera.get_view_dir();
		camera.m_right= glm::normalize(glm::cross(camera.m_forward, WORLD_UP));
		camera.m_up = glm::normalize(glm::cross(camera.m_right, camera.m_forward));
		previouseRot *= rotate_offset;

		glm::mat4 view = glm::lookAt(
								camera.m_position,
								//camera.m_position + camera.m_forward * camera.zoom,
								camera.get_view_center_position(),
								camera.m_up);
		glm::mat3 rotation(view);
		camera.m_rot_matrix =  mat4( transpose( rotation));  //rotation need to convert from world to camera space
	
	}


	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		float move_x = xpos / SCR_WIDTH * 2 - 1;
		float move_y = -(ypos / SCR_HEIGHT * 2 - 1);

		mouse_pos = vec3(move_x, move_y, 0);
		
		if (glm::length(prev_mouse) == 0) {
			prev_mouse = mouse_pos;
			return;
		}

		vec2 diff = mouse_pos - prev_mouse;
		camera.m_position += camera.m_right * diff.x * scroll_speed + camera.m_up * diff.y * scroll_speed;
		prev_mouse = mouse_pos;

	}


	if (glfwGetMouseButton(window, 0) == GLFW_RELEASE) {
		prev_mouse = vec3(0);
		//prev_mouse = mouse_pos;
		current_camera_model = camera.m_model_matrix;
		//endPos =  normalize( camera.m_forward) * -camera.zoom;
		endPos = camera.m_position;
	}
}


