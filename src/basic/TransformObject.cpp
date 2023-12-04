#include "TransformObject.h"

void test_print(string msg);
void TransformObject::Do()
{
	if (m_auto_update_matrix) {
	
		// TODO: Check parent....
		this->update_translate_matrix();
		this->update_rotation_matrix_eular();  // TODO: qutanion
		this->update_scale_matrix();

		if (this->m_parent != nullptr) {
			this->m_model_matrix = this->m_parent->m_model_matrix * this->m_translate_matrix * this->m_rot_matrix * this->m_scale_matrix;
			this->m_world_position = this->m_parent->m_model_matrix * vec4( this->m_position , 1.0);
		}
		else {
			this->m_model_matrix = this->m_translate_matrix * this->m_rot_matrix * this->m_scale_matrix;
			this->m_world_position = this->m_position;
		}

		this->update_local();
	}
}

void TransformObject::init_ui_content()
{

	//======== Define Draw UI cmd =================
	//this->create_panel(this->m_name);

	auto title_text = [&]() {Text("========= [ Transform ] ============");return true;};
	auto pos_inp = [&]() { return InputFloat3("Position", &this->m_position.x); };
	auto rot_inp = [&]() { return InputFloat3("Rotation", &this->m_rotation.x); };
	auto scale_inp = [&]() { return InputFloat3("Scale", &this->m_scale.x); };

	this->add_draw_item(title_text);
	this->add_draw_item(pos_inp);
	this->add_draw_item(rot_inp);
	this->add_draw_item(scale_inp);

	//======== Define select parent dropdown =================

	auto obj_list = Hierarchy::instance().get_gameobjs_list();
	static const char* current_item = "Set Parent";
	auto dp_inp = [&]() {
		if (ImGui::BeginCombo("##combo1", current_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (auto opt : *Hierarchy::instance().get_gameobjs_list())
			{
				bool is_selected = (current_item == opt); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(opt.c_str(), is_selected)) {
					current_item = opt.c_str();

					std::cout << "selected " << current_item << std::endl;
					GameObject* _new_parent_obj = Hierarchy::instance().get_gameobjs_by_name(current_item);
					this->set_transform_parent(_new_parent_obj->m_transform);
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		return true;
		};
	this->add_draw_item(dp_inp);
}

void TransformObject::set_transform_parent(TransformObject* _new_parent)
{
	if (_new_parent == this) {
		//TODO: this is TEMP : remove parent workaround.
		if (this->m_parent != nullptr) {
			this->m_parent->m_gameobject->remove_child(this->m_gameobject);
			_new_parent->m_gameobject->remove_child(this->m_gameobject);
			this->m_parent = nullptr;
		}
	}
	else if (_new_parent->m_gameobject->add_child(this->m_gameobject)) {
		this->m_parent = _new_parent;
		cout << "Set Parent " << _new_parent->m_name;
	};
	//TODO: Transform....
}

Component* TransformObject::copy()
{	
	return nullptr;
}


void TransformObject::update_rotation_matrix_eular()
{
	mat4 zr = glm::rotate(mat4(1.0f), glm::radians(this->m_rotation.z), WORLD_FORWARD);
	mat4 yr = glm::rotate(mat4(1.0f), glm::radians(this->m_rotation.y), WORLD_UP);
	mat4 xr = glm::rotate(mat4(1.0f), glm::radians(this->m_rotation.x), WORLD_RIGHT);

	this->m_rot_matrix = xr * yr * zr;

}

void TransformObject::update_translate_matrix()
{	
	this->m_translate_matrix = glm::translate(mat4(1.0f) , this->m_position );
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

void TransformObject::update_local()
{		
	//mat4 local_dir_trans = inverse( this->m_rot_matrix);
	mat4 local_dir_trans = this->m_rot_matrix;
	this->m_forward = normalize( local_dir_trans * glm::vec4(WORLD_FORWARD, 1.0));
	this->m_right = normalize (local_dir_trans * glm::vec4(WORLD_RIGHT, 1.0));
	this->m_up= normalize( local_dir_trans * glm::vec4(WORLD_UP, 1.0));


}

TransformObject::TransformObject(GameObject* _obj):TransformObject()
{
	this->m_gameobject = _obj;
}

TransformObject::TransformObject() :Component("Transform")
{
	this->m_position = vec3(0.0f);
	this->m_scale = vec3(1.0f);
	this->m_rotation = vec3(0.0f);
	//this->m_panel_names = this->m_name;

	this->update_translate_matrix();
	this->update_rotation_matrix_eular();  // TODO: qutanion
	this->update_scale_matrix();

	this->init_ui_content();
}



void test_print(string msg) {
	cout << msg << endl;
}

void TransformObject::move(vec3 _m)
{
	this->m_position += _m;
	this->update_translate_matrix();
}



