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
#include <FrameBufferObject.h>
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
void process_blit_stacks(vector<Shader>& blits_shaders , unsigned src_id );
MechainState state;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
int SCR_WIDTH		= 720;
int SCR_HEIGHT	= 720;

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "title", NULL, NULL); //monitor先設為NULL
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
		src_path + string("\\assets\\shaders\\default_vert.glsl"),
		src_path + string("\\assets\\shaders\\default_frag.glsl"));
	*/	
	//Shader s_default_shader(src_path + string("\\assets\\shaders"),"default");
	Shader s_default_shader(src_path + string("\\assets\\shaders\\default_vert.glsl"),src_path + string("\\assets\\shaders\\default_frag.glsl"));
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
	//GameObject dog_obj = GameObject("Dog");
	GameObject camera_obj = GameObject((TransformObject*)&camera);
	camera_obj.set_name("Camera");
	
	//camera_obj.m_transform->set_transform_parent(camera_root.m_transform);
	//Mesh* mesh =new Mesh(src_path + "\\assets\\models\\cy_sponza\\sponza.obj" , s_default_shader);		
	//Mesh* mesh =new Mesh(src_path + "\\assets\\models\\sponza\\sponza.obj" , s_default_shader);		
	Mesh* mesh =new Mesh(src_path + "\\assets\\models\\cute_dog\\cute_dg.obj" , s_default_shader);		
	//Mesh* mesh =new Mesh(src_path + "\\assets\\models\\sibenik\\sibenik.obj" , s_default_shader);		
	//Mesh*  dog_mesh = new Mesh(src_path + "\\assets\\models\\cute_dog\\cute_dg.obj", s_default_shader);
	obj.add_component(mesh);	
	//dog_obj.add_component(dog_mesh);
	camera_obj.add_component((Component*)&camera);

	Hierarchy::instance().add_object(&obj);
	//Hierarchy::instance().add_object(&dog_obj);
	//Hierarchy::instance().add_object((GameObject*)&camera);
	Hierarchy::instance().add_object(&camera_obj);
	vec3 sun_postion = normalize(vec3(0, 100, 0));
	
	vector<Shader> stacked_blits_shaders;

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


#pragma region FrameBuffer
	Shader frameBuffer_shader = Shader(src_path + string("\\assets\\shaders\\frame_vert.glsl") , src_path + string("\\assets\\shaders\\frame_frag.glsl"));
	Shader frame_blur_shader = Shader(src_path + string("\\assets\\shaders\\frame_blur_vert.glsl"), src_path + string("\\assets\\shaders\\frame_blur_frag.glsl"));
	Shader frame_quantization_shader = Shader(src_path + string("\\assets\\shaders\\frame_blur_vert.glsl"), src_path + string("\\assets\\shaders\\frame_quantization_frag.glsl"));
	Shader frame_dog_shader = Shader(src_path + string("\\assets\\shaders\\frame_blur_vert.glsl"), src_path + string("\\assets\\shaders\\frame_dog_frag.glsl"));
	Shader frame_abst_shader = Shader(src_path + string("\\assets\\shaders\\frame_blur_vert.glsl"), src_path + string("\\assets\\shaders\\frame_abstraction_frag.glsl"));

	Shader frame_water_shader = Shader(src_path + string("\\assets\\shaders\\frame_blur_vert.glsl"), src_path + string("\\assets\\shaders\\frame_water_frag.glsl"));
	Shader frame_mag_shader = Shader(src_path + string("\\assets\\shaders\\frame_blur_vert.glsl"), src_path + string("\\assets\\shaders\\frame_mag_frag.glsl"));
	Shader frame_bloom_shader = Shader(src_path + string("\\assets\\shaders\\frame_blur_vert.glsl"), src_path + string("\\assets\\shaders\\frame_bloom_frag.glsl"));
	//Shader frameBuffer_shader = Shader(src_path + string("\\assets\\shaders"), "frame");
	//Shader frame_blur_shader = Shader(src_path + string("\\assets\\shaders"), "frame_blur");
	static const GLenum draw_buffers[]={
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2
		//GL_DEPTH_ATTACHMENT,
	};
	FramebufferObject fbo = FramebufferObject(&frameBuffer_shader , &draw_buffers[0], 3, SCR_WIDTH, SCR_HEIGHT);
	FramebufferObject blur_fbo = FramebufferObject(&frame_blur_shader, &draw_buffers[0], 3, SCR_WIDTH, SCR_HEIGHT);
	FramebufferObject qua_fbo = FramebufferObject(&frame_quantization_shader, &draw_buffers[0], 3, SCR_WIDTH, SCR_HEIGHT);
	FramebufferObject dog_fbo = FramebufferObject(&frame_dog_shader, &draw_buffers[0], 3, SCR_WIDTH, SCR_HEIGHT);
	FramebufferObject abst_fbo = FramebufferObject(&frame_abst_shader, &draw_buffers[0], 3, SCR_WIDTH, SCR_HEIGHT);
	FramebufferObject water_fbo = FramebufferObject(&frame_water_shader, &draw_buffers[0], 3, SCR_WIDTH, SCR_HEIGHT);
	FramebufferObject mag_fbo = FramebufferObject(&frame_mag_shader, &draw_buffers[0], 3, SCR_WIDTH, SCR_HEIGHT);
	FramebufferObject bloom_fbo = FramebufferObject(&frame_bloom_shader, &draw_buffers[0], 3, SCR_WIDTH, SCR_HEIGHT);

	Texture noise_texture = Texture(src_path + string("\\assets\\textures\\water_noise.png"));
	//Texture noise_texture = Texture("C:\\Users\\User\\Downloads\\water_noise.png");
#pragma endregion


#pragma endregion
	
	double delta_time = 0.025 ;
	while (!glfwWindowShouldClose(window)) { // 等到console 送出kill flag
		
		if( time_scale>0)
			_previous_time += delta_time * time_scale;
		
		processInput(window , 0.1f);
		//glBindFramebuffer(GL_FRAMEBUFFER , FBO);
		fbo.activate();

		//glUseProgram(state.programId);

		//清除顏色
		ui_manager.new_frame();

		Begin("Sun positon");
		SliderFloat3("sun position", &sun_postion[0], -10, 10);
		End();

		Hierarchy::instance().execute(EXECUTE_TIMING::BEFORE_FRAME);		

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		s_default_shader.activate();
		glUniform3fv(glGetUniformLocation(s_default_shader.m_state.programId, "sun_postion")  , 1 , value_ptr(sun_postion));
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

		//--------------------------------------------
		// Default
		//fbo.blit( fbo.framebuffer_texture[1], 0);
		
		// Abstration
		//fbo.blit(fbo.framebuffer_texture[0],  blur_fbo.fbo);				
		//blur_fbo.blit(blur_fbo.framebuffer_texture[0], qua_fbo.fbo);
		//qua_fbo.blit(qua_fbo.framebuffer_texture[0], dog_fbo.fbo);		
		//dog_fbo.blit(fbo.framebuffer_texture[0], 0, qua_fbo.framebuffer_texture[0]);

		// Water Color
		/*
		fbo.blit(fbo.framebuffer_texture[0],  blur_fbo.fbo);				
		blur_fbo.blit(blur_fbo.framebuffer_texture[0], water_fbo.fbo);		
		water_fbo.blit(water_fbo.framebuffer_texture[0], qua_fbo.fbo, noise_texture.m_texture_id );
		qua_fbo.blit(qua_fbo.framebuffer_texture[0], 0);		
		*/

		// Magnifier 
		/*
		fbo.blit(fbo.framebuffer_texture[0], mag_fbo.fbo);
		mag_fbo.shader->activate();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);		
		//GLint location = glGetUniformLocation(mag_fbo.shader->m_state.programId, "mouse_pos");
		GLint location = mag_fbo.shader->shader_variables["mouse_pos"];
		glUniform2f(location ,  xpos /SCR_WIDTH , 1 - ypos / SCR_HEIGHT); ///TODO.... pass mouse position
		mag_fbo.blit(mag_fbo.framebuffer_texture[0], 0);
		*/

		// Bloom
		fbo.blit(fbo.framebuffer_texture[0], bloom_fbo.fbo);
		bloom_fbo.blit(bloom_fbo.framebuffer_texture[0], 0);


		//blur_fbo.draw_on_screen(blur_fbo.framebuffer_texture[0]);
		//--------------------------------------------
		
		ui_manager.create_hierarchy_window(Hierarchy::instance().m_game_objects);
		ui_manager.create_menubar();
		ui_manager.render_ui(); 

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

void process_blit_stacks(vector<Shader>& blits_shaders, unsigned src_id)
{

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
		camera.m_parent->m_position -= vec3(camera.m_rot_matrix * vec4( camera.m_parent->m_forward * vec3(dt),1.0));
		
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		
		camera.m_parent->m_position += vec3(camera.m_rot_matrix * vec4(camera.m_parent->m_forward * vec3(dt), 1.0));
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		float move_x = -(xpos / SCR_WIDTH * 2 - 1);
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
		vec3 _startPos		= normalize(endPos - camera.view_target) * camera.zoom;
		vec4 zoom_off		= vec4(_startPos, 1); // start from vec4(0, 0, -camera.zoom, 0);		
		//vec4 zoom_off = vec4(0,0, -camera.zoom ,0); // start from vec4(0, 0, -camera.zoom, 0);		
		mat4 rotate_offset	=  glm::rotate(mat4(1.0), angle, rotation_axis); // rotate camera around orbit

		camera.m_position =rotate_offset * zoom_off;

		camera.m_forward = -camera.get_view_dir();
		camera.m_right= glm::normalize(glm::cross(camera.m_forward, WORLD_UP));
		camera.m_up = glm::normalize(glm::cross(camera.m_right, camera.m_forward));
		previouseRot *= rotate_offset;
		glm::mat4 view = glm::lookAt(
								camera.m_position,
								//camera.m_position + camera.m_forward * camera.zoom,
								//camera.get_view_center_position(),
								camera.view_target,
								camera.m_up);
		glm::mat3 rotation(view);
		camera.m_rot_matrix =  mat4( transpose( rotation));  //rotation need to convert from world to camera space
		/*
		*/
	}

	/////////////////////////// [ Move Camera Up and Down ] ////////////////////////////
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		
		camera.m_parent->m_position += vec3(vec4((camera.m_up) * vec3(dt ), 1.0));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {		
		camera.m_parent->m_position -= vec3(vec4((camera.m_up) * vec3(dt ), 1.0));
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS ) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		float move_x = -xpos / SCR_WIDTH * 2 - 1;
		float move_y = -(ypos / SCR_HEIGHT * 2 - 1);

		mouse_pos = vec3(move_x, move_y, 0);
		
		if (glm::length(prev_mouse) == 0) {
			prev_mouse = mouse_pos;
			return;
		}

		vec2 diff = mouse_pos - prev_mouse;
		vec3 local_move = camera.m_right * diff.x * scroll_speed - camera.m_up * diff.y * scroll_speed;
		camera.m_parent->m_position += vec3( camera.m_rot_matrix * vec4(local_move,1.0));
		prev_mouse = mouse_pos;
	}
	
	///////////////////////////////////////////////////////////////////////////////

	if (glfwGetMouseButton(window, 0) == GLFW_RELEASE) {
		prev_mouse = vec3(0);
		//prev_mouse = mouse_pos;
		current_camera_model = camera.m_model_matrix;
		endPos =  normalize( camera.m_forward) * -camera.zoom;
		//endPos = camera.m_position;
		//endPos = normalize(endPos - camera.view_target) * camera.zoom;
	}
}


