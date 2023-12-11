# About Tiny Engine

This is a class project, but I extended it with more flexible command (shader playground , Blit , loading model via ffile explorer etc...)


### What can I do with it?

* You can use it as a starter framework of your project.
* I will keep adding items till this semester end.


# Documant

## Main struct

![struct](https://i.imgur.com/DSRfSB0.png)

### Main Logic

- Hieraarchy is a singleton class.
- Component added to Hieraarchy will call their ```Do()``` function in main logic each frame.
```C++
Hierarchy Hierarchy::sInstance;
```
 
![struct](https://i.imgur.com/mRXMimT.png)

## Component Class

- Any attachable element is a sub class of Component .
- Please ovride functions below.

```C++
	virtual void Do(){};
	virtual void DO_Before_Frame() {};
	virtual void Do_End_Frame() {};
```

Cast Component
```C++
GameObject* gm = this->m_gameobject->cast_component<GameObject>(); // Get this gameobj's transform   
Mesh* mesh = this->m_gameobject->cast_component<Mesh>(); // Get this gameobj's Mesh component
```


## GameObject Class
- class GameObject : public Component , public UiPanel
- GameObject are created with TransformObject.

Create gameobject
```C++
GameObject obj = GameObject("Slime");
obj.set_name("Slime");  // or set its name via function
Hierarchy::instance().add_object(&obj);
```

Attach Component
```C++
// Add Mesh object.
Mesh* mesh_b1 = new Mesh(src_path + "\\assets\\models\\bush\\grassB.obj");
obj.add_component(dog_mesh);

```



## TransformObject Class
- Gameobject can use m_transform to set the transform. (Model matrix will be updated in main logic)

```C++

obj.m_transform->m_rotation = vec3(-30, 0, 0);
obj.m_transform->m_position = vec3(0, 0, 50);
obj.m_transform->m_scale = vec3(1, 1,1);

```

## Mesh Class 
class Mesh : public UiableComponent

Load Model

```C++
Mesh* dog_mesh =new Mesh(src_path + "\\assets\\models\\cute_dog\\cute_dg.obj" , s_default_shader); //give render shader
Mesh* mesh_b1 = new Mesh(src_path + "\\assets\\models\\bush\\grassB.obj");  //or only get the mesh data.

obj.add_component(dog_mesh);  // Attach to gameobject , it will be rendered in main logic.
```

## Texture Class

Create

```C++
	Texture(); //empty
	Texture(unsigned _width , unsigned int _height);  //create blank buffer
	Texture(const string _file_path);  // load image
```

### Texture Type 
See BasicFlags.h

- Material will set this as uniform variables.
- When loading a model, Mesh class will set them if they exist is obj file.
```C++
const enum class Bind_Type 
{
	DIFFUSE,
	NORMAL,
};

```

- To access them, simply use these keyword.

```GLSL
uniform sampler2D DIFFUSE;
uniform sampler2D NORMAL;
```

Usage: See Material.cpp


## Shader Class

Create Shader
```C++
	Shader s_default_shader(
		src_path + string("\\assets\\shaders\\default_vert.glsl"),
		src_path + string("\\assets\\shaders\\default_frag.glsl"));
```

Create Compute Shader

If you only pass in the vert shader, I will be compiled as compute shader.

```C++
	this->cs_reset_shader =  Shader(GET_SRC_FOLDER() + string("\\assets\\shaders\\hw3_reset_cs.glsl"));

```



## IndirectInstance Mesh Class
class IndirectInstancedMesh: public UiableComponent 

```C++
	vector<Mesh> ind_mesh_list = {*mesh_b1  , *mesh_b2 , *mesh_b3 };	//Loaded 3 mesh 	
	IndirectInstancedMesh id_mesh(ind_mesh_list); // pass in to create IndirectInstancedMesh
```

Render

ToDo: merge it into Hierarchy pipeline.
```C++
// reset
id_mesh.cs_reset_shader.activate();
id_mesh.dispatch_reset();

// culling
id_mesh.cs_view_culling_shader.activate();
id_mesh.dispatch_culling();	

// Render
id_mesh.indirect_render_shader.activate();
id_mesh.Do();

```

## FramebufferObject Class

Create fbo:
```C++
static const GLenum draw_buffers[]={
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2	
	};

FramebufferObject* fbo = frame_buffer_debugger.gen_frame_object_and_registor(&frameBuffer_shader , &draw_buffers[0], 1, HALF_SCR_WIDTH , SCR_HEIGHT);
// It is the same as below but frame_buffer_debugger will log it on an UI.
// FramebufferObject* game_fbo = FramebufferObject(&frameBuffer_shader , &draw_buffers[0], 1, HALF_SCR_WIDTH , SCR_HEIGHT);
```

Blit 

Blit with bloom screen shader.
```C++
// Bloom
FramebufferObject* bloom_fbo = frame_buffer_debugger.gen_frame_object_and_registor(&frame_bloom_shader, &draw_buffers[0], 1, SCR_WIDTH, SCR_HEIGHT);
fbo->blit(fbo->framebuffer_texture[0], *bloom_fbo);
bloom_fbo->blit(bloom_fbo->framebuffer_texture[0], *compare_fbo);  // blit to othere fbo

//.... Render fbo
compare_fbo->shader->activate();
// set up your uniform....
compare_fbo->blit(compare_fbo->framebuffer_texture[0], 0);  // blit main screen texture

```

### Blit with multiple textures
```C++
compare_fbo->blit(compare_fbo->framebuffer_texture[0], 0, fbo->framebuffer_texture[0]);
```

Read in frag shader.
```GLSL
//in frag 
layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 1) uniform sampler2D texture2;  // additional texture
layout (binding = 2) uniform sampler2D texture3;  
//.....texture4,texture5...
```
## Camera
class Camera : public Component

Create Camera object
```C++
Camera* game_camera = new Camera(glm::vec3(0.0f, 0.0f, 0.001f) , (float)SCR_WIDTH  / (float)SCR_HEIGHT ,1,150);
GameObject game_camera_obj = GameObject();
game_camera_obj.add_component((Component*)game_camera);  // Attach it to game object

```

Move Camera along its forward direction.
```C++
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {		
		game_camera_obj.m_transform->m_position -= vec3( vec4(game_camera_obj.m_transform->m_forward * vec3(dt), 1.0));		
	}
```


## UiManager Class

Create your own ui on screen.

Example : create nav bar with button action.
```C++
	auto my_button_with_action = [&]() { if (Button("Button text")) {
		cout<<"hi";
		return true;
	} };
	ui_manager.m_menu_cmds.push_back(my_button_with_action);

```

Render UI at the end of the rendering stage.
```C++
		ui_manager.create_hierarchy_window(Hierarchy::instance().m_game_objects); //update hierarchy
		ui_manager.create_menubar();
		ui_manager.render_ui(); 

		glfwSwapBuffers(window); 
		glfwPollEvents(); 
		Hierarchy::instance().execute(EXECUTE_TIMING::AFTER_FRAME);

```


## UiPanel Class
Bind your ui to heriarchy.

class TransformObject :public UiableComponent

class UiableComponent : virtual public UiPanel, virtual public Component

Example : Binding Transform value. (TransformObject inherit from UiableComponent.)
```C++
TransformObject::TransformObject()
{
	this->m_position = vec3(0.0f);
	this->m_scale = vec3(1.0f);
	this->m_rotation = vec3(0.0f);

	auto pos_inp= [&]() { return InputFloat3("Position", &this->m_position.x); };
	auto rot_inp = [&]() { return InputFloat3("Rotation", &this->m_rotation.x); };
	auto scale_inp = [&]() { return InputFloat3("Scale", &this->m_scale.x); };

	this->add_draw_item(pos_inp);
	this->add_draw_item(rot_inp);
	this->add_draw_item(scale_inp);
	
}

```

Result
![](https://i.imgur.com/IVvLl8B.png)

------


## Framedebugger Class
```C++
//In render loop:

frame_buffer_debugger.Init_Panel(0, 150);
frame_buffer_debugger.Begin_Panel();

// Render.....

frame_buffer_debugger.End_Panel();
frame_buffer_debugger.create_hw2_panel(hw2_effect_panel);

```

## ShaderEditor Class

Demo :(https://www.youtube.com/watch?v=WuO4l4AzwXs&feature=youtu.be&ab_channel=CuneiSolar%E6%A5%94%E9%99%BD)

```C++
// Init 
ShaderEditor shaderEditor;

// In render Loop
shaderEditor.begin_panel();
```