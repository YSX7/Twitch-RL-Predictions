#include "pch.h"
#include "RLPrediction.h"


// BakkesMod Macro / init function.
BAKKESMOD_PLUGIN(RLPrediction, "RL Twitch Prediction", "0.2.3", PLUGINTYPE_FREEPLAY)

// The entry point of your plugin.
void RLPrediction::onLoad()
{
	this->Log("This is my first BakkesMod Plugin!!");

	cvarManager->registerCvar(CVAR_TOKEN, "", "");
	cvarManager->registerCvar(CVAR_CLIENTID, "h2sdygmx345l6kh24ao4m3fgtnok0r");
	cvarManager->registerCvar(CVAR_BROADCASTER, "");

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
	gameWrapper->HookEvent("Function GameEvent_TA.Countdown.BeginState", std::bind(&RLPrediction::GameStart, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", std::bind(&RLPrediction::GameEndedEvent, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.AchievementManager_TA.HandleMatchEnded", std::bind(&RLPrediction::GameEndedEvent, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_TA.Destroyed", std::bind(&RLPrediction::GameDestroyed, this, std::placeholders::_1));
}


void RLPrediction::GameDestroyed(std::string eventName)
{
	if (!this->IsPollStarted) {
		return;
	}
	this->IsPollStarted = false;
	this->Log("Game destroyed");
}

// Custom call hooked when Rocket League executes functions associated with the end of a game. 
// Стартует 2 раза
void RLPrediction::GameEndedEvent(std::string name)
{
	if (!this->IsPollStarted) {
		return;
	}
	this->IsPollStarted = false;

	CarWrapper me = gameWrapper->GetLocalCar();
	if (me.IsNull())
		return;

	PriWrapper mePRI = me.GetPRI();
	if (mePRI.IsNull())
		return;

	TeamInfoWrapper myTeam = mePRI.GetTeam();
	if (myTeam.IsNull())
		return;

	// Get TeamNum
	int my_team_num = myTeam.GetTeamNum();

	ServerWrapper server = gameWrapper->GetOnlineGame();
	TeamWrapper winningTeam = server.GetGameWinner();
	if (winningTeam.IsNull())
		return;

	int teamnum = winningTeam.GetTeamNum();

	this->Log("GameEnd => winning team:" + std::to_string(teamnum) + " my_team_num:" + std::to_string(my_team_num));
}

void RLPrediction::GameStart(std::string eventName)
{
	if (gameWrapper->IsInFreeplay()) {
		return;
	}

	if (this->IsPollStarted) {
		return;
	}
	this->IsPollStarted = true;

	this->Log("Game started");
	
}

// A custom log wrapper. 
void RLPrediction::Log(std::string msg)
{
	cvarManager->log("AutoTrainingPlugin: " + msg);
}