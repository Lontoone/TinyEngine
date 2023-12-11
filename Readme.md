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
```C++


```
