#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

const std::string CVAR_TOKEN = "RLPredict_auth_token";
const std::string CVAR_CLIENTID = "RLPredict_client_Id";
const std::string CVAR_BROADCASTER = "RLPredict_broadcaster_Id";

class RLPrediction : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow
{

public:
	virtual void onLoad();
	virtual void onUnload();
	virtual void GameStart(std::string eventName);
	virtual void GameDestroyed(std::string eventName);

	void LoadHooks();
	void GameEndedEvent(std::string name);

private:

	bool IsPollStarted = false;

	void Log(std::string msg);

    void RenderSettings() override;
    std::string GetPluginName() override;
    void SetImGuiContext(uintptr_t ctx) override;
};

