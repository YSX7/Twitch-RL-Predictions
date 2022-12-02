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
	ImGui::TextUnformatted("A really cool plugin");

	if (ImGui::Button("Button"))
	{ 

		socket

		CurlRequest req;
		req.url = "https://api.twitch.tv/helix/predictions";
		req.body = R"T({
  "broadcaster_id":"BROADID",
  "title": "Win or Lose?",
  "outcomes": [
    {
      "title": "Win"
    },
    {
      "title": "Lose"
    }
  ],
  "prediction_window": 60
})T";
		std::string s = "BROADID";
		auto p = req.body.find(s);
		if (p != req.body.npos)
		{
			req.body.replace(p, s.length(), cvarManager->getCvar(CVAR_BROADCASTER).getStringValue());
		}

		this->Log(req.body);

		this->Log(req.headers["Client-Id"]);


		req.headers = { {"Authorization", "Bearer " + cvarManager->getCvar(CVAR_TOKEN).getStringValue()},
			{"Client-Id",cvarManager->getCvar(CVAR_CLIENTID).getStringValue()},
			{"Content-Type", "application/json"}};

		this->Log("sending body request");
		HttpWrapper::SendCurlRequest(req, [this](int code, std::string result)
			{
				this->Log(result);
			});
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
