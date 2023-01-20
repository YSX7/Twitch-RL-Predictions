#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

const std::string CVAR_TOKEN = "RLPredict_auth_token";
const std::string CVAR_CLIENTID = "RLPredict_client_Id";
const std::string CVAR_BROADCASTER = "RLPredict_broadcaster_Id";

std::string Replace(std::string str, std::string findstr, std::string replaceWith);

struct PredictionIds {
	std::string id;
	std::string winId;
	std::string loseId;
};

class RLPrediction : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow
{

	enum Statuses {
		WAIT,
		STARTING,
		OK,
		ENDING
	};

public:
	virtual void onLoad();
	virtual void onUnload();
	virtual void GameStart(std::string eventName);
	virtual void GameDestroyed(std::string eventName);

	void LoadHooks();
	void GameEndedEvent(std::string name);
	void StartPrediction();
	void CancelPrediction();
	void WinPrediction();
	void LosePrediction();

private:
	PredictionIds ids;
	Statuses currentStatus = WAIT;

	void Log(std::string msg);

    void RenderSettings() override;
    std::string GetPluginName() override;
    void SetImGuiContext(uintptr_t ctx) override;
};

