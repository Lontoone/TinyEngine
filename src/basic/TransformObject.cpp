#include "TransformObject.h"



void TransformObject::Do()
{
	// TODO: Check parent....
	this->update_translate_matrix();
	this->update_rotation_matrix_eular();  // TODO: qutanion
	this->update_scale_matrix();

	this->m_model_matrix = this->m_translate_matrix * this->m_rot_matrix * this->m_scale_matrix;
	//this->draw_ui_panel();

	//this->m_model_matrix =  this->m_scale_matrix;
	//this->m_model_matrix = mat4(1.0f);
	/*
	for (int i = 0; i < 4; i++) {
		cout << this->m_model_matrix[i][0] << " " << this->m_model_matrix[i][1] << " " << this->m_model_matrix[i][2] << " " << this->m_model_matrix[i][3]<< endl;
	}
	cout << endl;
	*/
	//this->m_model_matrix
}


void TransformObject::update_rotation_matrix_eular()
{
	mat4 zr = glm::rotate(mat4(1.0f), glm::radians(this->m_rotation.z), this->m_forward);
	mat4 yr = glm::rotate(mat4(1.0f), glm::radians(this->m_rotation.y), this->m_up);
	mat4 xr = glm::rotate(mat4(1.0f), glm::radians(this->m_rotation.x), this->m_right);
	
	this->m_rot_matrix = xr * yr * zr;

}

void TransformObject::update_translate_matrix()
{	
	this->m_translate_matrix = glm::translate(mat4(1.0f) , this->m_position);
}

void TransformObject::update_scale_matrix()
{
	this->m_scale_matrix = glm::scale(mat4(1.0f) , this->m_scale);
	/*
	this->m_scale_matrix= mat4(
		this->m_scale.x, 0, 0, 0,
		0, this->m_scale.y, 0, 0,
		0, 0, this->m_scale.z, 0,
		0, 0, 0, 1
	);
	*/
}

TransformObject::TransformObject() :Component("Transform")
{
	this->m_position = vec3(0.0f);
	this->m_scale = vec3(1.0f);
	this->m_rotation = vec3(0.0f);
	//this->m_panel_names = this->m_name;

	//======== Define Draw UI cmd =================
	auto pos_inp= [&]() { return InputFloat3("Position", &this->m_position.x); };
	auto rot_inp = [&]() { return InputFloat3("Rotation", &this->m_rotation.x); };
	auto scale_inp = [&]() { return InputFloat3("Scale", &this->m_scale.x); };

	this->create_panel(this->m_name);
	this->add_draw_item(pos_inp);
	this->add_draw_item(rot_inp);
	this->add_draw_item(scale_inp);
	
	//======== Define select parent dropdown =================
	/*
	auto set_parent_inp = [&](string selected) {
		this->set_parent(Hierarchy::instance->get_gameobjs_by_name(selected));
		return true; 
	};
	auto obj_list = Hierarchy::instance->get_gameobjs_list();
	add_drop_down_menu(*obj_list , set_parent_inp);
	*/
}

void TransformObject::move(vec3 _m)
{
	this->m_position += _m;
	this->update_translate_matrix();
}



