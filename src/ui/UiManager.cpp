#include "UiManager.h"
void test_menu();
UiManager::UiManager()
{
	
}

void UiManager::init_imgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;	
	StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window , true);
	ImGui_ImplOpenGL3_Init("#version 330");


	// test: BUTTON
	/*
	auto pos_inp = [&]() { if (Button("h")) {
		printf("click" );
		return true;
	} };
	this->m_menu_cmds.push_back(pos_inp);
	*/
}

void UiManager::new_frame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();

	//create_window();
}

void UiManager::destory()
{

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	DestroyContext();
}

void UiManager::create_menubar()
{
	ImGui::BeginMainMenuBar();
	Text("Tiny Engine___Lontoone");
	for (auto _cmd : this-> m_menu_cmds) {
		//_item->run_all_draw_cmd();
		_cmd();
	}
	ImGui::EndMainMenuBar();
	//Begin("Temp Menu");
	//Text("Hi");
	//End();
}

void UiManager::create_sceneNgame_window(uint &scene_buffer_id, uint &game_buffer_id  )
{
	ImGuiIO io = ImGui::GetIO();
	uint w = io.DisplaySize.x;
	uint h = io.DisplaySize.y;
	
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowSize(ImVec2(w/2 , h));

	auto flag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoInputs;
	ImGui::Begin("Scene" ,0, flag);	
	ImGui::Image((void*)scene_buffer_id, io.DisplaySize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(w/2 , 0));
	ImGui::SetNextWindowSize(ImVec2(w / 2, h));
	ImGui::Begin("Game" ,0 , flag);
	ImGui::Image((void*)game_buffer_id, io.DisplaySize, ImVec2(0, 1), ImVec2(1, 0));
	//ImGui::Image(0, io.DisplaySize, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();


}

void UiManager::render_ui()
{
	/*
	Begin("Test");
	Text("Hi");
	End();
	*/
	Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}

void UiManager::create_window(const char* _window_name)
{
	Begin(_window_name);
	// TODO: Register.....

	End();
}

void recursive_add_node(const char* _name, GameObject* current,vector<GameObject*>& _nexts , int n)
{
	//PushID(_id);
	PushID(_name);
	bool tree_node_open = TreeNodeEx(
		_name,
		ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding |
		ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth
	);

	if (tree_node_open) {
		current->update_ui_tree();
		for (auto _nextPanel : _nexts) {			
			recursive_add_node(_nextPanel->name.c_str(), _nextPanel, _nextPanel->m_childs, n + 1);
			
		}
		/*  [Demo Dragging]
		//ImGui::Selectable(current->name);
		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (n_next >= 0 && n_next < 3)
			{
				ImGui::ResetMouseDragDelta();
			}
		}
		*/
		TreePop();
	}
	PopID();
}

void recursive_add_item(const char* _name, GameObject* current, vector<GameObject*>& _nexts, int n) {
	
}

void UiManager::create_hierarchy_window(vector<GameObject*>& objs) {	
	
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	
	ImGui::SetNextWindowPos(viewport->WorkPos);
	
	Begin("Object List");

	int  i = 0;
	for (auto& obj : objs) {
		if (obj->m_transform->m_parent == nullptr) {
			recursive_add_node(obj->name.c_str(), obj, obj->m_childs, i);
			i++;
		}
	}
	End();


}
