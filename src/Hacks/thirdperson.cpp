#include "thirdperson.h"
#include "AntiAim/antiaim.h"

static bool buttonToggle = false;

void ThirdPerson::OverrideView(CViewSetup *pSetup)
{
	C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

	if (!localplayer) {
		return;
	}

	C_BaseCombatWeapon *activeWeapon = (C_BaseCombatWeapon *) entityList->GetClientEntityFromHandle(
			localplayer->GetActiveWeapon());

	if (activeWeapon && activeWeapon->GetCSWpnData() &&
	    activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_GRENADE) {
		input->m_fCameraInThirdPerson = false;
		Settings::ThirdPerson::toggled = false;
		return;
	}

	// if ( !localplayer->IsAlive() ){
	// 	if (input->m_fCameraInThirdPerson){
	// 		input->m_fCameraInThirdPerson = false;
	// 		input->m_vecCameraOffset = Vector(0.f, 0.f, 0.f);
	// 	}
	// 	if (Settings::ThirdPerson::toggled)
	// 	{
	// 		*localplayer->GetObserverMode() = ObserverMode_t::OBS_MODE_CHASE;
	// 	}else {
	// 		*localplayer->GetObserverMode() = ObserverMode_t::OBS_MODE_NONE;
	// 	}
	// }
	if (Settings::ThirdPerson::enabled && !engine->IsTakingScreenshot()) {
		/* Code For Toggle
			if (inputSystem->IsButtonDown(Settings::ThirdPerson::toggleThirdPerson) && buttonToggle == false)
				Settings::ThirdPerson::toggled = true;
			else if ( !inputSystem->IsButtonDown(Settings::ThirdPerson::toggleThirdPerson ) && Settings::ThirdPerson::toggled == true)
				Settings::ThirdPerson::toggled = false;
		*/

		/*
		* Button Toggel Code :)
		*/
		if (inputSystem->IsButtonDown(Settings::ThirdPerson::toggleThirdPerson) && !buttonToggle) {
			buttonToggle = true;
			Settings::ThirdPerson::toggled = !Settings::ThirdPerson::toggled;
		} else if (!inputSystem->IsButtonDown(Settings::ThirdPerson::toggleThirdPerson) && buttonToggle) {
			buttonToggle = false;
		}

		// END

		if (Settings::ThirdPerson::toggled) {
			C_BasePlayer *obs = nullptr;
			if (!localplayer->IsAlive()) {
				*localplayer->GetObserverMode() = ObserverMode_t::OBS_MODE_CHASE;
				// obs = (C_BasePlayer*)localplayer->GetObserverTarget();
			} else {
				obs = localplayer;

				QAngle viewAngles;
				engine->GetViewAngles(viewAngles);
				trace_t tr;
				Ray_t traceRay;
				Vector eyePos = obs->GetEyePosition();

				Vector camOff = Vector(cos(DEG2RAD(viewAngles.y)) * Settings::ThirdPerson::distance,
				                       sin(DEG2RAD(viewAngles.y)) * Settings::ThirdPerson::distance,
				                       sin(DEG2RAD(-viewAngles.x)) * Settings::ThirdPerson::distance);

				traceRay.Init(eyePos, (eyePos - camOff));
				CTraceFilter traceFilter;
				traceFilter.pSkip = localplayer;
				trace->TraceRay(traceRay, MASK_SOLID, &traceFilter, &tr);

				input->m_fCameraInThirdPerson = true;
				input->m_vecCameraOffset = Vector(viewAngles.x, viewAngles.y, Settings::ThirdPerson::distance *
				                                                              ((tr.fraction < 1.0f) ? tr.fraction
				                                                                                    : 1.0f));
				
				input->m_fCameraInThirdPerson = true;
			}
		} else {
			input->m_fCameraInThirdPerson = false;
			input->m_vecCameraOffset = Vector(0.f, 0.f, 0.f);
		}

	}

}

void ThirdPerson::FrameStageNotify(ClientFrameStage_t stage)
{
	// if (!engine->IsInGame()) return;
	// if (stage != ClientFrameStage_t::FRAME_RENDER_START) return;
	// C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

	// if ( !localplayer || !localplayer->IsAlive()) return;

	// if (!Settings::ThirdPerson::enabled && !input->m_fCameraInThirdPerson) return;

}
