#include "pch.h"
#include "RLPrediction.h"


// BakkesMod Macro / init function.
BAKKESMOD_PLUGIN(RLPrediction, "RL Twitch Prediction", "0.3.2", PLUGINTYPE_FREEPLAY)

// The entry point of your plugin.
void RLPrediction::onLoad()
{
	this->Log("This is my first BakkesMod Plugin!!");

	cvarManager->registerCvar(CVAR_TOKEN, "", "");
	cvarManager->registerCvar(CVAR_CLIENTID, "h2sdygmx345l6kh24ao4m3fgtnok0r");
	cvarManager->registerCvar(CVAR_BROADCASTER, "42412771");

	this->LoadHooks();
}

// The exit point of your plugin.
void RLPrediction::onUnload()
{
	// Nothing needs to happen here.
}

// The hooks that we bind to function calls in Rocket League.
// If function A occurs, we tell bakkes mod to call our custom method.
void RLPrediction::LoadHooks()
{
	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.Active.StartRound", std::bind(&RLPrediction::GameStart, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", std::bind(&RLPrediction::GameEndedEvent, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.AchievementManager_TA.HandleMatchEnded", std::bind(&RLPrediction::GameEndedEvent, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.AchievementSystem_TA.CheckWonMatch", std::bind(&RLPrediction::GameEndedEvent, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_TA.Destroyed", std::bind(&RLPrediction::GameDestroyed, this, std::placeholders::_1));
}


void RLPrediction::GameStart(std::string eventName)
{
	if (gameWrapper->IsInFreeplay()) {
		return;
	}

	if (this->currentStatus != WAIT) {
		return;
	}

	this->currentStatus = STARTING;
	this->Log("Game started, starting prediction...");
	
	StartPrediction();
}

// Custom call hooked when Rocket League executes functions associated with the end of a game. 
// Стартует 2 раза
void RLPrediction::GameEndedEvent(std::string name)
{
	if (this->currentStatus != OK) {
		return;
	}
	this->currentStatus = ENDING;

	auto currentGame = gameWrapper->GetCurrentGameState();
	// just to make sure you're in a match first
	if (!currentGame)
	{
		this->Log("no current game");
		return;
	}
	auto pri = currentGame.GetLocalPrimaryPlayer();
	if (!pri) { 
		this->Log("no local primary player");
		return;
	}

	int my_team_num = pri.GetTeamNum2();

	ServerWrapper server = gameWrapper->GetOnlineGame();
	if (server.IsNull())
	{
		this->Log("No server");
		return;
	}
	TeamWrapper winningTeam = server.GetGameWinner();
	if (winningTeam.IsNull())
	{
		this->Log("No winning team");
		return;
	}

	int teamnum = winningTeam.GetTeamNum();

	this->Log("GameEnd => winning team:" + std::to_string(teamnum) + " my_team_num:" + std::to_string(my_team_num));

	if (teamnum == my_team_num)
	{
		WinPrediction();
	}
	else {
		LosePrediction();
	}
}

void RLPrediction::GameDestroyed(std::string eventName)
{
	if (this->currentStatus != OK) {
		return;
	}
	this->currentStatus = ENDING;
	this->Log("Game destroyed, cancelling prediction...");
	CancelPrediction();
}

// A custom log wrapper. 
void RLPrediction::Log(std::string msg)
{
	cvarManager->log("AutoTrainingPlugin: " + msg);
}

void RLPrediction::StartPrediction() {
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

	req.body = Replace(req.body, "BROADID", cvarManager->getCvar(CVAR_BROADCASTER).getStringValue());


	this->Log(req.body);


	req.headers = { {"Authorization", "Bearer " + cvarManager->getCvar(CVAR_TOKEN).getStringValue()},
		{"Client-Id",cvarManager->getCvar(CVAR_CLIENTID).getStringValue()},
		{"Content-Type", "application/json"} };

	this->Log("sending body request");
	HttpWrapper::SendCurlRequest(req, [this](int code, std::string result)
		{
			this->Log(std::to_string(code));
			this->Log(result);
			nlohmann::json json = nlohmann::json::parse(result.c_str());
			nlohmann::json data = json["data"][0];
			this->ids.id = data["id"];
			this->ids.winId = data["outcomes"][0]["id"];
			this->ids.loseId = data["outcomes"][1]["id"];
			this->currentStatus = OK;
		});
}


//TODO: объединить завершение предикшена в одну функцию
void RLPrediction::CancelPrediction() {
	CurlRequest req;
	req.url = "https://api.twitch.tv/helix/predictions?broadcaster_id=BROADID&id=PREDID&status=CANCELED";
	req.verb = "PATCH";
	req.url = Replace(req.url, "BROADID", cvarManager->getCvar(CVAR_BROADCASTER).getStringValue());
	req.url = Replace(req.url, "PREDID", this->ids.id);
	req.headers = { {"Authorization", "Bearer " + cvarManager->getCvar(CVAR_TOKEN).getStringValue()},
		{"Client-Id",cvarManager->getCvar(CVAR_CLIENTID).getStringValue()},
		{"Content-Type", "application/json"} };
	HttpWrapper::SendCurlRequest(req, [this](int code, std::string result)
		{
			this->Log(std::to_string(code));
			this->Log(result);
			this->currentStatus = WAIT;
		});
}

void RLPrediction::WinPrediction() {
	CurlRequest req;
	req.url = "https://api.twitch.tv/helix/predictions?broadcaster_id=BROADID&id=PREDID&status=RESOLVED&winning_outcome_id=OUTCOMEID";
	req.verb = "PATCH";
	req.url = Replace(req.url, "BROADID", cvarManager->getCvar(CVAR_BROADCASTER).getStringValue());
	req.url = Replace(req.url, "PREDID", this->ids.id);
	req.url = Replace(req.url, "OUTCOMEID", this->ids.winId);
	req.headers = { {"Authorization", "Bearer " + cvarManager->getCvar(CVAR_TOKEN).getStringValue()},
		{"Client-Id",cvarManager->getCvar(CVAR_CLIENTID).getStringValue()},
		{"Content-Type", "application/json"} };
	HttpWrapper::SendCurlRequest(req, [this](int code, std::string result)
		{
			this->Log(std::to_string(code));
			this->Log(result);
			this->currentStatus = WAIT;
		});
}
void RLPrediction::LosePrediction() {
	CurlRequest req;
	req.url = "https://api.twitch.tv/helix/predictions?broadcaster_id=BROADID&id=PREDID&status=RESOLVED&winning_outcome_id=OUTCOMEID";
	req.verb = "PATCH";
	req.url = Replace(req.url, "BROADID", cvarManager->getCvar(CVAR_BROADCASTER).getStringValue());
	req.url = Replace(req.url, "PREDID", this->ids.id);
	req.url = Replace(req.url, "OUTCOMEID", this->ids.loseId);
	req.headers = { {"Authorization", "Bearer " + cvarManager->getCvar(CVAR_TOKEN).getStringValue()},
		{"Client-Id",cvarManager->getCvar(CVAR_CLIENTID).getStringValue()},
		{"Content-Type", "application/json"} };
	HttpWrapper::SendCurlRequest(req, [this](int code, std::string result)
		{
			this->Log(std::to_string(code));
			this->Log(result);
			this->currentStatus = WAIT;
		});
}