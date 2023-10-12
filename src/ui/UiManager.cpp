#include "UiManager.h"

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
}

void UiManager::new_frame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();
}

void UiManager::destory()
{

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	DestroyContext();
}

void UiManager::_test()
{
	Begin("Test");
	Text("Hi");
	End();

	Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}
