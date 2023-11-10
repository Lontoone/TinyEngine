#include "GameObject.h"
string get_uuid() {
	// this is not safe
	static random_device dev;
	static mt19937 rng(dev());

	uniform_int_distribution<int> dist(0, 15);

	const char* v = "0123456789abcdef";
	const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

	string res;
	for (int i = 0; i < 16; i++) {
		if (dash[i]) res += "-";
		res += v[dist(rng)];
		res += v[dist(rng)];
	}
	return res;
}

GameObject::GameObject()
{
	TransformObject* trans = new TransformObject(this);	
	//this->m_instance_id = reinterpret_cast<int32>(this);
	this->add_component(trans);
	this->m_transform = trans;	

}
GameObject::GameObject(TransformObject* trans)
{
	this->add_component(trans);
	this->m_transform = trans;
}
GameObject::GameObject(const char* _obj_name):GameObject()
{
	/*
	string _uuid = get_uuid();
	this->m_instance_id = _uuid.c_str();
	string _tmp = string("GameObject##") + _uuid[0] + _uuid[2] + _uuid[5];
	cout << "game id " << _tmp << endl;
	*/
	this->set_name(_obj_name);
	this->m_panel_names = _obj_name;
	cout << "obj name" << m_panel_names<<endl;
	this->create_panel(this->m_panel_names);

}
void GameObject::DO_Before_Frame() {
	panel_begin();
}

void GameObject::Do_End_Frame()
{
	panel_end();
}

bool GameObject::add_child(GameObject* _child)
{
	if (_child != this && !this->check_is_child(_child)) {
		if(_child->m_transform->m_parent != nullptr)
			_child->m_transform->m_parent->m_gameobject->remove_child(_child);
		this->m_childs.push_back(_child);
		return true;
	}
	else
	{
		cout << "[invalid operation] Set Parent" << endl;
		return false;
	}
}

void GameObject::remove_child(GameObject* _child)
{
	this->m_childs.erase(std::remove(this->m_childs.begin(), this->m_childs.end(), _child), this->m_childs.end());
}

bool GameObject::check_is_child(GameObject* _child)
{	
	return this->m_childs.size()>0 && std::find(this->m_childs.begin(), this->m_childs.end(), _child) != this->m_childs.end();
}

vector<GameObject*> GameObject::clone()
{
	vector<GameObject*> result;

	string _clone_name = this->name + "_Clone";
	GameObject* _clone = new GameObject(_clone_name.c_str());
	result.push_back(_clone);

	for (auto comp : this->m_comps) {
		auto _comp_clone = comp->copy();
		if (_comp_clone != nullptr)
			_comp_clone->attatch_to(_clone);
	}
	for (int i = 0; i < this->m_childs.size(); i++) {
		auto child_clone = (GameObject*)(this->m_childs[i]->copy());
		result.push_back(child_clone);
		if (i == 0) {
			child_clone->m_transform->set_transform_parent(this->m_transform->m_parent);
		}
		else
		{
			child_clone->m_transform->set_transform_parent(this->m_childs[i - 1]->m_transform);
		}
	}
	return result;
}

GameObject::~GameObject()
{
	for (auto& ptr : this->m_comps) {
		try
		{
			if (ptr != nullptr) {
				cout << this->name << ptr->m_name << " freeing " << ptr << endl;
				delete ptr;
				ptr = NULL;
			}
		}
		catch (const std::exception&)
		{
			cout << this->name << ptr->m_name << " freeing " << ptr << endl;

		}
	}
	/*
	
	for (int i = 0; i < this->m_comps.size(); i++) {
		cout << "freeing " << m_comps[i] << endl;
		delete this->m_comps[i];
	}*/

	this->m_comps.clear();
	cout << this->m_comps.size()<<endl;
}


void GameObject::execute(EXECUTE_TIMING timimg )
{	
		/*
	switch (timimg)
	{	
	case EXECUTE_TIMING::BEFORE_FRAME:		
		this->DO_Before_Frame();
		break;
	case EXECUTE_TIMING::AFTER_FRAME:
		this->Do_End_Frame();
		break;
	default:
		break;
	}
		*/
	
	for (int i = 0; i < this->m_comps.size(); i++) {		

		switch (timimg)
		{
		case EXECUTE_TIMING::MAIN_LOGIC:			
			this->m_comps[i]->Do();
			break;
		case EXECUTE_TIMING::BEFORE_FRAME:			
			this->m_comps[i]->DO_Before_Frame();
			break;
		case EXECUTE_TIMING::AFTER_FRAME:			
			this->m_comps[i]->Do_End_Frame();
			break;
		default:
			break;
		}
	}
	/*
	if (timimg == EXECUTE_TIMING::MAIN_LOGIC) {
		panel_end();
	}
	*/

}

void GameObject::update_ui_tree()
{
	//panel_begin();	
	for (int i = 0; i < this->m_components_panels.size(); i++) {		
		this->m_components_panels[i]->draw_ui_panel();		
	}
	//panel_end();
}

void GameObject::set_name(const char* new_name)
{
	this->name = (char*)new_name;	
}
/*
void GameObject::set_name(size_t  _idx)
{
	string _tmp_name = (string("New object") + to_string(_idx));
	this->name = _tmp_name.c_str();
}
*/

void GameObject::add_component(Component* _new_comp)
{
	this->m_comps.push_back(_new_comp);
	_new_comp->attatch_to(this);	
}

void GameObject::add_component(UiableComponent* _new_comp)
{
	this->m_components_panels.push_back((UiPanel*)_new_comp);
	this->add_component((Component*)_new_comp);
}
