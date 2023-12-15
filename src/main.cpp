#pragma once
#include <iostream>

#include <filesystem>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/*
#ifndef GLEW_INC
#define GLEW_INC
#include <GL/glew.h>
#endif // !GLEW_INC

#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)

*/
#include <env_path.h>

#include "ui/UiManager.h"
#include "basic/Mesh.h"
#include "basic/Fastsetup.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "basic/camera.h"
#include "basic/Hierarchy.h"
#include <Light.h>
#include <GameObject.h>
//#include "basic/GameObject.h"
#include "FileDialog.h"
#include <Debugger.hpp>
#include <FrameBufferObject.h>
#include <FrameBufferDebugger.h>
#include <RuntimeShaderEditor.h>
#include <DefaultGrid.h>
#include <CameraFrustum.h>
#include <IndirectInstancedMesh.h>
#include <LightingManager.h>

//===========
// For HW3
#include <Trajectory.h>
using namespace INANOA::SCENE::EXPERIMENTAL;
Trajectory t ;
void hw3_move_slim(GameObject& obj) {
	
	t.update();
	obj.m_transform->m_position = t.position();
	obj.Do();
}

//===========

GLFWwindow* init();
void update();
void render();
void input();
void render_grid(mat4& vp);
void setup_menu(UiManager * uimanager);
void render_game_view( Camera* camera, Shader* shader);

using namespace std;
void processInput(GLFWwindow* window, double dt);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_blit_stacks(vector<Shader>& blits_shaders , unsigned src_id );
void set_shader_mvp(Shader* shader, Camera* camera);  //Debug...
MechainState state;

unsigned int SCR_WIDTH		= 1344;
unsigned int SCR_HEIGHT		= 756;
unsigned int HALF_SCR_WIDTH = SCR_WIDTH / 2;
unsigned int HALF_SCR_HEIGHT = SCR_HEIGHT / 2;

Camera* game_camera = new Camera(glm::vec3(21.77f, 5.58f, -17.39f) ,1,150, SCR_WIDTH  ,SCR_HEIGHT );
Camera* scene_camera = new Camera(glm::vec3(0.0f, 0.0f, 0.10f) , (float)SCR_WIDTH  /(float)SCR_HEIGHT ,1,500 );

GameObject game_camera_obj = GameObject();
GameObject scene_camera_obj = GameObject();
//=====================================
// |        Basice Set up             |
//=====================================
const string src_path = GET_SRC_FOLDER();//EXPAND(_PRJ_SRC_PATH);  //TODO: move to manager script
//const string src_path = ".\\src";
UiManager ui_manager = UiManager();
Hierarchy Hierarchy::sInstance;
DefaultEditorGrid* grid;
vec3 sun_postion = normalize(vec3(0, 1, 1)); // temp debug


#include <windows.h>
#include <string>
#include <iostream>

using namespace std;
#include <direct.h>
std::string get_current_dir() {
	char buff[FILENAME_MAX]; //create string buffer to hold path
	_getcwd(buff, FILENAME_MAX);
	string current_working_dir(buff);
	return current_working_dir;
}

using namespace std;
int main(int argc , char** argv) {
	std::cout << "hello"<<endl;	
	GLFWwindow* window =  init();  // TODO: init 
	glfwSwapInterval(0);   // Disable v-sync   hey~hey
	double _previous_time = 0;
	float time_scale = 1;
	
	Hierarchy::instance().set_main_camera(game_camera);
	cout << get_current_dir() << endl;
#pragma region DEBUG_PRE_LOAD_MODEL
	Shader s_default_shader(
		src_path + string("\\assets\\shaders\\default_vert.glsl"),
		src_path + string("\\assets\\shaders\\default_frag.glsl"));
	
	grid = new DefaultEditorGrid(
		src_path + string("\\assets\\shaders\\unlit_vert.glsl") , 
		src_path + string("\\assets\\shaders\\frame_grid_frag.glsl"));

	
	//Mesh* mesh =new Mesh(src_path + "\\assets\\models\\cy_sponza\\sponza.obj" , s_default_shader);		
	//obj.m_transform->m_scale = vec3(0.05);
	//Mesh* dog_mesh =new Mesh(src_path + "\\assets\\models\\sponza\\sponza.obj" , s_default_shader);
	Mesh* dog_mesh = new Mesh(src_path + "\\assets\\models\\indoor\\new_house.obj", s_default_shader);
	//Mesh* dog_mesh =new Mesh(src_path + "\\assets\\models\\cute_dog\\cute_dg.obj" , s_default_shader);
	//Mesh* mesh_b1 = new Mesh(src_path + "\\assets\\models\\bush\\grassB.obj");
	//Mesh* mesh_b2 = new Mesh(src_path + "\\assets\\models\\bush\\bush01_lod2.obj");
	//Mesh* mesh_b3 =new Mesh(src_path + "\\assets\\models\\bush\\bush05_lod2.obj" );
	//Mesh* mesh = new Mesh(src_path + "\\assets\\models\\cube\\cube.obj", s_default_shader);

	GameObject obj = GameObject("Slime");
	obj.add_component(dog_mesh);
	obj.m_transform->m_scale = vec3(1);
	Hierarchy::instance().add_object(&obj);	
	Hierarchy::instance().add_renderer(dog_mesh);
	

	scene_camera_obj.m_transform->m_position = vec3(0, 150, 150);
	scene_camera_obj.m_transform->m_rotation = vec3(-30, 0, 0);
	game_camera_obj.m_transform->m_position = glm::vec3(3.42,0.7,-3);
	game_camera_obj.m_transform->m_rotation = glm::vec3(0, -600, 0);
	game_camera_obj.m_transform->m_scale = vec3(1, 1,1);
	
	//GameObject camera_obj = GameObject((TransformObject*)game_camera->m_parent);
	game_camera_obj.set_name("Camera");
	scene_camera_obj.set_name("Scene Camera");
	game_camera_obj.add_component((Component*)game_camera);
	scene_camera_obj.add_component((Component*)scene_camera);
	//camera_obj.m_transform = game_camera;
	//game_camera->set_transform_parent(camera_obj.m_transform);
	Hierarchy::instance().add_object(&game_camera_obj);
	Hierarchy::instance().add_object(&scene_camera_obj);
	

	Light* sun = new Light();
	GameObject sun_obj = GameObject("Sun");
	sun_obj.m_transform->m_position = vec3(1, 1, -3);
	sun_obj.add_component((Component*)sun);
	Hierarchy::instance().add_object(&sun_obj);
	Hierarchy::instance().add_light(sun);

#pragma endregion

	
	RViewFrustum frustum(1 , game_camera);

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
#pragma region FrameBuffer
	FrameBufferDebugger frame_buffer_debugger;
	ShaderEditor shaderEditor;

	//Shader cs_grass_shader = Shader(src_path + string("\\assets\\shaders\\cs_grass.glsl"),"");
	Shader frameBuffer_shader = Shader(src_path + string("\\assets\\shaders\\frame_vert.glsl") , src_path + string("\\assets\\shaders\\frame_frag.glsl"));
	Shader shadow_shader = Shader(src_path + string("\\assets\\shaders\\shadow_map_vert.glsl"), src_path + string("\\assets\\shaders\\shadow_map_frag.glsl"));
	Shader frameBuffer_scene_shader = Shader(src_path + string("\\assets\\shaders\\frame_vert.glsl"), src_path + string("\\assets\\shaders\\frame_frag.glsl"));
	Shader frame_grid_shader = Shader(src_path + string("\\assets\\shaders\\frame_vert.glsl"), src_path + string("\\assets\\shaders\\frame_grid_frag.glsl"));
	Shader gizmose_shader = Shader(src_path + string("\\assets\\shaders\\vertexShader_ogl_450.glsl"), src_path + string("\\assets\\shaders\\fragmentShader_ogl_450.glsl"));


	//Shader frameBuffer_shader = Shader(src_path + string("\\assets\\shaders"), "frame");
	//Shader frame_blur_shader = Shader(src_path + string("\\assets\\shaders"), "frame_blur");
	static const GLenum draw_buffers[]={
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2
		//GL_DEPTH_ATTACHMENT,
	};
	FramebufferObject* main_fbo = frame_buffer_debugger.gen_frame_object_and_registor(&frameBuffer_shader, &draw_buffers[0], 3, SCR_WIDTH, SCR_HEIGHT );
	//FramebufferObject* game_fbo = frame_buffer_debugger.gen_frame_object_and_registor(&frameBuffer_shader , &draw_buffers[0], 1, HALF_SCR_WIDTH , SCR_HEIGHT);
	//FramebufferObject* scene_fbo = frame_buffer_debugger.gen_frame_object_and_registor(&frameBuffer_scene_shader, &draw_buffers[0], 1, HALF_SCR_WIDTH, SCR_HEIGHT);
	

#pragma endregion

	//vector<Mesh> ind_mesh_list = {*mesh_b1  , *mesh_b2 , *mesh_b3 };	
	//vector<Mesh> ind_mesh_list = { *mesh_b1 , *mesh_b3 };
	//IndirectInstancedMesh id_mesh(ind_mesh_list);

	double delta_time = 0.025 ;
	while (!glfwWindowShouldClose(window)) { // 等到console 送出kill flag
		
		if( time_scale>0)
			_previous_time += delta_time * time_scale;
		

		processInput(window , 0.1f);
		//glBindFramebuffer(GL_FRAMEBUFFER , FBO);
		//glUseProgram(state.programId);

		//=====================
		//		CLEAR UP
		//=======================
		ui_manager.new_frame();
		ui_manager.create_fps_window();
		ui_manager.update_fps();

		//============================
		//		BEFORE FRAME SETTING
		//=============================
		Begin("Sun positon");
		SliderFloat3("sun position", &sun_obj.m_transform->m_position[0], -10, 10);
		End();

		Hierarchy::instance().execute(EXECUTE_TIMING::BEFORE_FRAME);				

		//hw3_move_slim(obj);
		//id_mesh.hw3_update_dog_position(obj.m_transform->m_position);

		// [ Set up FBO]
		glClearColor(0.2, 0.2, 0.2, 1.0f);
		// Depth testing needed for Shadow Map
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BACK);
		glViewport(0, 0, 1024, 1024);
		sun->fbo->activate();
		glClear(GL_DEPTH_BUFFER_BIT);

		// Render all object
		shadow_shader.activate();
		mat4 sun_vp = sun->get_light_vp_matrix();
		/**/
		glUniformMatrix4fv(glGetUniformLocation(shadow_shader.m_state.programId, "model"), 1, GL_FALSE, value_ptr(obj.m_transform->m_model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(shadow_shader.m_state.programId, "u_LIGHT_VP_MATRIX"), 1, GL_FALSE, value_ptr(sun_vp));
		for (auto renderer : Hierarchy::instance().m_meshes) {
			renderer->get_gameobject()->m_transform->Do();
			renderer->Render_without_material(shadow_shader);
		}
		
		//LightingManager::render_to_shadowmap();   // [ BUG : writing to wrong mvp? ]
		sun->fbo->blit(sun->fbo->framebuffer_texture[0], main_fbo->fbo, frameBuffer_shader);

		//---------------------------
		// [ Set up FBO]
		glClearColor(0.2, 0.2, 0.2, 1.0f);		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);		
		
		// Draw all items
		s_default_shader.activate();
		game_camera->bind_uniform(s_default_shader.m_state.programId);
		// Set uniform
		glUniform4fv(glGetUniformLocation(s_default_shader.m_state.programId, u_CAM_POS) , 1 , value_ptr(game_camera_obj.m_transform->m_position));
		glUniformMatrix4fv(glGetUniformLocation(s_default_shader.m_state.programId, u_LIGHT_VP_MATRIX), 1, GL_FALSE, value_ptr(sun_vp));
		// Bind the Shadow Map
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, sun->fbo->framebuffer_texture[0]);
		glUniform1i(glGetUniformLocation(s_default_shader.m_state.programId, u_TEX_SHADOW_MAP), 2);

		Hierarchy::instance().execute(EXECUTE_TIMING::MAIN_LOGIC);

		//  Blit to main screen
		//main_fbo->blit(main_fbo->framebuffer_texture[0] , 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		frameBuffer_shader.activate();
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(main_fbo->rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, main_fbo->framebuffer_texture[0]);
		glUniform1i(glGetUniformLocation(frameBuffer_shader.m_state.programId, "screenTexture"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*   [ HW3 ]
		//========== Split View ===========			
		//=======================
		//   For Game view
		//=========================
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(HALF_SCR_WIDTH, 0, HALF_SCR_WIDTH, SCR_HEIGHT);
		//Reseting
		id_mesh.cs_reset_shader.activate();
		id_mesh.dispatch_reset();

		// Culling 
		set_shader_mvp(&id_mesh.cs_view_culling_shader, game_camera);
		id_mesh.cs_view_culling_shader.activate();
		id_mesh.dispatch_culling();	
		
		render_game_view(game_camera, &s_default_shader);
		id_mesh.indirect_render_shader.activate();
		set_shader_mvp(&id_mesh.indirect_render_shader, game_camera);	
		id_mesh.Do(); //TEST

		//=======================		
		//   For scene view
		//=========================
		glViewport(0, 0, HALF_SCR_WIDTH, SCR_HEIGHT);
		render_game_view( scene_camera, &s_default_shader);		
		set_shader_mvp(&id_mesh.indirect_render_shader, scene_camera);//TEST
		id_mesh.Do(); //TEST


		// ========= Setting game camera for scene view =========
		gizmose_shader.activate();
		glm::mat4 view = scene_camera->getViewMatrix();
		glm::mat4 projection = scene_camera->getProjectionMatrix();
		glm::mat4 model = game_camera_obj.m_transform->m_model_matrix;

		glUniformMatrix4fv(glGetUniformLocation(gizmose_shader.m_state.programId, "MATRIX_M"), 1, GL_FALSE, value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(gizmose_shader.m_state.programId, "MATRIX_V"), 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(gizmose_shader.m_state.programId, "MATRIX_P"), 1, GL_FALSE, value_ptr(projection));
		
		frustum.update(game_camera);
		frustum.render();
		// =====================================================
		*/
		//--------------------------------------------			

		
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		//ui_manager.create_sceneNgame_window(scene_fbo->framebuffer_texture[0] , game_fbo->framebuffer_texture[0]);
		frame_buffer_debugger.Draw_Frames_on_Panel();
		frame_buffer_debugger.attach_texture(sun->fbo->framebuffer_texture[0]);
		frame_buffer_debugger.End_Panel();
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

void set_shader_mvp(Shader * shader , Camera * camera) {
	//-------------- [ TEMP 暫時的MVP ] ------------------
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = camera->getViewMatrix();
	projection = camera->getProjectionMatrix();
	glm::mat4 vp = projection * view;

	//render_grid(vp);
	shader->activate();
	//Render Game view
	//glUniform3fv(glGetUniformLocation(shader->m_state.programId, "CAMERA_WORLD_POSITION"), 1, value_ptr(camera->m_position));
	glUniform3fv(glGetUniformLocation(shader->m_state.programId, "sun_postion"), 1, value_ptr(sun_postion));
	//glUniformMatrix4fv(glGetUniformLocation(s_default_shader.m_state.programId, "model"), 1, GL_FALSE, value_ptr(model));		

	//glUniformMatrix4fv(glGetUniformLocation(shader->m_state.programId, "MATRIX_M"), 1, GL_FALSE, value_ptr(camera->m_model_matrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->m_state.programId, "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader->m_state.programId, "projection"), 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader->m_state.programId, "MATRIX_VP"), 1, GL_FALSE, value_ptr(vp));
}

void render_game_view( Camera* camera , Shader* shader) { 
	
	//-------------- [ TEMP 暫時的MVP ] ------------------
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = camera->getViewMatrix();
	projection = camera->getProjectionMatrix();
	glm::mat4 vp = projection * view;

	render_grid(vp);
	shader->activate();
	//Render Game view
	//glUniform3fv(glGetUniformLocation(s_default_shader.m_state.programId, "sun_postion"), 1, value_ptr(sun_postion));
	//glUniformMatrix4fv(glGetUniformLocation(s_default_shader.m_state.programId, "model"), 1, GL_FALSE, value_ptr(model));		
	glUniformMatrix4fv(glGetUniformLocation(shader->m_state.programId, "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader->m_state.programId, "projection"), 1, GL_FALSE, value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader->m_state.programId, "MATRIX_VP"), 1, GL_FALSE, value_ptr(vp));

	Hierarchy::instance().execute(EXECUTE_TIMING::MAIN_LOGIC);
	
}

GLFWwindow* init() {
	glfwInit();

	/*
	src_path.erase(0, 1); // erase the first quote
	src_path.erase(src_path.size() - 1); // erase the last quote and the dot	
	*/
	printf(src_path.c_str());

	// open gl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //版本3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //版本x.3

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#pragma region LoadWindow

	// 檢查
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "title", NULL, NULL); //monitor先設為NULL
	if (window == NULL) {
		cout << "No Window" << endl;
		glfwTerminate();
		return nullptr;
	}
	//把window放到thread上
	glfwMakeContextCurrent(window);
	ui_manager.init_imgui(window);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	//gladLoadGL();
	/*
	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed init glad" << endl;
		glfwTerminate();
		return nullptr;
	}
	*/
	//定義視窗
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //(x, y , width , height)

	//指定視窗resize的處理方法
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); // counter clock wise
#pragma endregion
	return window;
}
void update() {}
void render() {}
void input() {}

void render_grid( mat4& vp ) {
	// Render Grid
	grid->grid_shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(grid->grid_shader.m_state.programId, "MATRIX_VP"), 1, GL_FALSE, value_ptr(vp));
	glUniformMatrix4fv(glGetUniformLocation(grid->grid_shader.m_state.programId, "model"), 1, GL_FALSE, value_ptr(glm::mat4(1.0f)));
	grid->render();

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	cout << "resizing is banned" << endl;
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	//TODO:
	// Framebuffer textture要重新create
	
	//glViewport(0, 0, width, height);
	//SCR_WIDTH = width;
	//SCR_HEIGHT = height;
	/*
	// Ensure we don't divide by zero
		if (height == 0) {
			height = 1;
		}

		GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

		// Set the viewport to cover the new window
		glViewport(0, 0, width, height);

		// Set the projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Adjust the aspect ratio
		if (width >= height) {
			// width is larger, so widen the viewing volume
			glOrtho(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0, 1.0, -1.0);
		}
		else {
			// height is larger, so increase the viewing volume
			glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, 1.0, -1.0);
		}

		// Switch back to the model view matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	*/
}


//float prex =0 , prey=0;
vec3 prev_mouse = vec3(0);
vec3 mouse_pos = vec3(0);
vec3 endPos = vec3(0,0,-game_camera->zoom);
mat4 current_camera_model = mat4(0);
mat4 previouseRot = mat4(1);
float scroll_speed = 50.;
//vec2 prev_mouse = vec2(0);
static glm::vec2 screenCenter(SCR_WIDTH / 2, SCR_HEIGHT / 2);

void processInput(GLFWwindow* window, double dt) {
	//若按下esc key => 關掉window

	/*  TODO: 統一camera和object設計	*/
	/**/
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {		
		game_camera_obj.m_transform->m_position -= vec3( vec4(game_camera_obj.m_transform->m_forward * vec3(dt), 1.0));		
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		game_camera_obj.m_transform->m_position += vec3(vec4(game_camera_obj.m_transform->m_forward * vec3(dt), 1.0));		
	}	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		game_camera_obj.m_transform->m_rotation-= vec3(0, dt, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		game_camera_obj.m_transform->m_rotation += vec3(0, dt, 0);
	}


	/*
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		float move_x = -(xpos / SCR_WIDTH * 2 - 1);
		float move_y = -(ypos / SCR_HEIGHT * 2 - 1);
		TransformObject& scene_trans = *scene_camera->get_gameobject()->m_transform;

		mouse_pos = vec3(move_x, move_y, 0);

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

		if (glm::length(prev_mouse) == 0) {
			prev_mouse = mouse_pos;			
			return;
		}
		if (current_camera_model == mat4(0)) {
			current_camera_model = scene_trans.m_model_matrix;
		}

		vec3 op1 = prev_mouse;		

		float angle = glm::acos(glm::min(1.0f, glm::dot(op1, mouse_pos)));
		if(angle ==0 ){
			return;
		}
		//cout << " Angle " << angle << " dot" << dot(op1, mouse_pos) << endl;
		vec3 rotation_axis = glm::cross(op1, mouse_pos);


		//		Get the rotation axis in 3D	
		vec3 _startPos		= normalize(endPos - scene_camera->view_target) * scene_camera->zoom;
		vec4 zoom_off		= vec4(_startPos, 1); // start from vec4(0, 0, -camera->zoom, 0);		
		//vec4 zoom_off = vec4(0,0, -camera->zoom ,0); // start from vec4(0, 0, -camera->zoom, 0);		
		mat4 rotate_offset	=  glm::rotate(mat4(1.0), angle, rotation_axis); // rotate camera around orbit

		//scene_trans.m_position =rotate_offset * zoom_off;
		//game_camera->update_translate_matrix();

		scene_trans.m_forward = -scene_trans.m_forward;
		scene_trans.m_right= glm::normalize(glm::cross(scene_trans.m_forward, WORLD_UP));
		scene_trans.m_up = glm::normalize(glm::cross(scene_trans.m_right, scene_trans.m_forward));
		previouseRot *= rotate_offset;
		glm::mat4 view = glm::lookAt(
								scene_trans.m_position,
								//camera->m_position + camera->m_forward * camera->zoom,
								//camera->get_view_center_position(),
								scene_camera->view_target,
								scene_trans.m_up);
		glm::mat3 rotation(view);
		scene_trans.m_rot_matrix *=  mat4( transpose( rotation));  //rotation need to convert from world to camera space
		//scene_trans.m_model_matrix =  scene_trans.m_translate_matrix * scene_trans.m_rot_matrix * scene_trans.m_scale_matrix;
	}
	*/

	/////////////////////////// [ Move Camera Up and Down ] ////////////////////////////
	
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		
		game_camera_obj.m_transform->m_position += vec3(vec4((game_camera_obj.m_transform->m_up) * vec3(dt ), 1.0));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {		
		game_camera_obj.m_transform->m_position -= vec3(vec4((game_camera_obj.m_transform->m_up) * vec3(dt), 1.0));
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
		vec2 diff = (mouse_pos - prev_mouse );
		vec3 local_move = scene_camera->get_gameobject()->m_transform->m_right * diff.x * scroll_speed - scene_camera->get_gameobject()->m_transform->m_up * diff.y * scroll_speed;
		vec3 world_move = vec3(scene_camera->get_gameobject()->m_transform->m_rot_matrix * vec4(local_move, 1.0));
		
		scene_camera->get_gameobject()->m_transform->m_position += world_move;
		/*
		prev_mouse = mouse_pos;
		//scene_camera->get_gameobject()->m_transform->m_rotation += vec3(0, diff.x * scroll_speed,0);

		glm::mat4 view = glm::lookAt(
			scene_camera->get_gameobject()->m_transform->m_position,
			scene_camera->get_gameobject()->m_transform->m_position + scene_camera->get_gameobject()->m_transform->m_forward * scene_camera->zoom,
			//camera->get_view_center_position(),
			//scene_camera->view_target,
			scene_camera->get_gameobject()->m_transform->m_up);
		glm::mat3 rotation(view);
		scene_camera->get_gameobject()->m_transform->m_rot_matrix *= mat4(transpose(rotation));

		*/
	}	
	/*
	*/
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////// [ HW3 Move Scene Camera] ////////////////////////////
	/*
	*/
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		scene_camera_obj.m_transform->m_position += vec3(vec4((WORLD_FORWARD) * vec3(dt*2), 1.0));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		scene_camera_obj.m_transform->m_position -= vec3(vec4((WORLD_FORWARD)*vec3(dt * 2), 1.0));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		scene_camera_obj.m_transform->m_position -= vec3(vec4((WORLD_RIGHT)*vec3(dt * 2), 1.0));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		scene_camera_obj.m_transform->m_position += vec3(vec4((WORLD_RIGHT)*vec3(dt * 2), 1.0));
	}
	//scene_camera->Do();
	//scene_camera->update_local();
	///////////////////////////////////////////////////////////////////////////////

	/*  [ERROR !]
	*/
	if (glfwGetMouseButton(window, 0) == GLFW_RELEASE) {
		prev_mouse = vec3(0);
		//prev_mouse = mouse_pos;
		current_camera_model = scene_camera->get_gameobject()->m_transform->m_model_matrix;
		endPos =  normalize(scene_camera->get_gameobject()->m_transform->m_forward) * -scene_camera->zoom ;
		//endPos = camera->m_position;
		//endPos = normalize(endPos - camera->view_target) * camera->zoom;
	}
}



