#include "RuntimeShaderEditor.h"

ShaderEditor::ShaderEditor()
{
	compile_shader();
}

void ShaderEditor::begin_panel()
{
	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + main_viewport->WorkSize.x - 400, main_viewport->WorkPos.y));
	ImGui::SetNextWindowSize(ImVec2(400, main_viewport->WorkSize.y));

	ImGui::Begin("Shader Playground Editor");

	//vert input
	//ImGui::InputTextMultiline("## Vertex Shader", vert_buffer, IM_ARRAYSIZE(vert_buffer), ImGui::GetContentRegionAvail());
	//ImGui::InputTextMultiline("## Vertex Shader", vert_buffer, IM_ARRAYSIZE(vert_buffer) , ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 32), ImGuiInputTextFlags_AllowTabInput);
	//Frag input
	ImGui::InputTextMultiline("## Fragment Shader", frag_buffer, IM_ARRAYSIZE(frag_buffer), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_AllowTabInput);
	
	if (ImGui::Button("Compile")) {
		cout << "Compile Shader Playground" << endl;
		this->compile_shader();
	}
	ImGui::End();

}

void ShaderEditor::compile_shader()
{
	/*
	char* _vert = this->vert_buffer;
	char* _frag = this->frag_buffer;
	_vert[std::remove(_vert, _vert + strlen(_vert), '\n') - _vert] = 0;
	_frag[std::remove(_frag, _frag + strlen(_frag), '\n') - _frag] = 0;
	std::cout << "compiled vert" << _vert << endl;

	SetProgramFromSource(this->shader.m_state , _vert, _frag);
	*/
	SetProgramFromSource(this->shader.m_state, this->vert_buffer, this->frag_buffer);
}
