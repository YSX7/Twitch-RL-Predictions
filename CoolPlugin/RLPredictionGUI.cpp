#include "pch.h"
#include "RLPrediction.h"

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void RLPrediction::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Name of the plugin to be shown on the f2 -> plugins list
std::string RLPrediction::GetPluginName()
{
	return "TwitchPrediction";
}

void RLPrediction::RenderSettings() {
	ImGui::Text("RL Twitch Predictions Plugin v%s", plugin_version);

	ImGui::Checkbox("Enable plugin", &this->IsEnabled);

	if (ImGui::Button("Autheticate")) {

	}
	if (ImGui::Button("Test Start Prediction"))
	{ 
		this->StartPrediction();
	}
	if (ImGui::Button("Test Cancel Prediction"))
	{
		this->CancelPrediction();
	}
	if (ImGui::Button("Test Lose Prediction"))
	{
		this->LosePrediction();
	}
	if (ImGui::Button("Test Win Prediction"))
	{
		this->WinPrediction();
	}
	//=========== Token Inputs =====================
	//==== Auth Token
	static bool tokenCheck = false;
	CVarWrapper tokenCVar = cvarManager->getCvar(CVAR_TOKEN);
	static char authTokenValue[64] = "";
	if (tokenCVar) {  
		strcpy(authTokenValue, tokenCVar.getStringValue().data());
	}
	if (ImGui::InputText("Auth Token", authTokenValue, IM_ARRAYSIZE(authTokenValue), tokenCheck ? 0 : ImGuiInputTextFlags_Password)) {
		tokenCVar.setValue(authTokenValue);
	}
	ImGui::SameLine(); 
	ImGui::Checkbox("Show", &tokenCheck);

	//==== Client ID
	CVarWrapper clientCVar = cvarManager->getCvar(CVAR_CLIENTID);
	static char clientTokenValue[64] = "";
	if (clientCVar) {
		strcpy(clientTokenValue, clientCVar.getStringValue().data());
	}
	if (ImGui::InputText("Client Id", clientTokenValue, IM_ARRAYSIZE(clientTokenValue))) {
		clientCVar.setValue(clientTokenValue);
	}

	//==== Broadcaster ID
	CVarWrapper broadcasterCVar = cvarManager->getCvar(CVAR_BROADCASTER);
	static char broadcasterTokenValue[64] = "";
	if (broadcasterCVar) {
		strcpy(broadcasterTokenValue, broadcasterCVar.getStringValue().data());
	}
	if (ImGui::InputText("Broadcaster ID", broadcasterTokenValue, IM_ARRAYSIZE(broadcasterTokenValue))) {
		broadcasterCVar.setValue(broadcasterTokenValue);
	}

#pragma region Examples



	/*if (ImGui::Button("Ball On Top")) {
		gameWrapper->Execute([this](GameWrapper* gw) { cvarManager->executeCommand("CoolerBallOnTop"); });
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Activate Ball On Top");
	}

	CVarWrapper enableCvar = cvarManager->getCvar("cool_enabled");

	if (!enableCvar) {
		return;
	}

	bool enabled = enableCvar.getBoolValue();

	if (ImGui::Checkbox("Enable plugin", &enabled)) {
		enableCvar.setValue(enabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Toggle Cool Plugin");
	}

	CVarWrapper distanceCvar = cvarManager->getCvar("cool_distance");

	if (!distanceCvar) {
		return;
	}

	float distance = distanceCvar.getFloatValue();
	if (ImGui::SliderFloat("Distance", &distance, 0.0, 500.0)) {
		distanceCvar.setValue(distance);
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverText = "distance is " + std::to_string(distance);
		ImGui::SetTooltip(hoverText.c_str());
	}*/
#pragma endregion
}

std::string Replace(std::string str, std::string findstr, std::string replaceWith) {
	auto p = str.find(findstr);
	if (p != str.npos)
	{
		str.replace(p, findstr.length(), replaceWith);
		return str;
	}
	return str;
}

/*
// Do ImGui rendering here
void RLPrediction::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string RLPrediction::GetMenuName()
{
	return "RLPrediction";
}

// Title to give the menu
std::string RLPrediction::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void RLPrediction::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool RLPrediction::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool RLPrediction::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void RLPrediction::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void RLPrediction::OnClose()
{
	isWindowOpen_ = false;
}
*/
