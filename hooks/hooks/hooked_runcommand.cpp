// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "..\hooks.hpp"
#include "..\..\cheats\misc\prediction_system.h"
#include "..\..\cheats\lagcompensation\local_animations.h"
#include "..\..\cheats\misc\misc.h"
#include "..\..\cheats\misc\logs.h"

using RunCommand_t = void(__thiscall*)(void*, player_t*, CUserCmd*, IMoveHelper*);

void __fastcall hooks::hooked_runcommand(void* ecx, void* edx, player_t* player, CUserCmd* m_pcmd, IMoveHelper* move_helper)
{
	static auto original_fn = prediction_hook->get_func_address <RunCommand_t> (19);
	g_ctx.local((player_t*)m_entitylist()->GetClientEntity(m_engine()->GetLocalPlayer()), true);

	if (!player || player != g_ctx.local())
		return original_fn(ecx, player, m_pcmd, move_helper);

	if (!m_pcmd)
		return original_fn(ecx, player, m_pcmd, move_helper);

	if (m_pcmd->m_tickcount > m_globals()->m_tickcount + 72) //-V807
	{
		m_pcmd->m_predicted = true;
		player->set_abs_origin(player->m_vecOrigin());

		if (m_globals()->m_frametime > 0.0f && !m_prediction()->EnginePaused)
			++player->m_nTickBase();

		return;
	}

	if (g_cfg.ragebot.enable && player->is_alive())
	{
		auto weapon = player->m_hActiveWeapon().Get();

		if (weapon)
		{
			static float tickbase_records[MULTIPLAYER_BACKUP];
			static bool in_attack[MULTIPLAYER_BACKUP];
			static bool can_shoot[MULTIPLAYER_BACKUP];

			tickbase_records[m_pcmd->m_command_number % MULTIPLAYER_BACKUP] = player->m_nTickBase();
			in_attack[m_pcmd->m_command_number % MULTIPLAYER_BACKUP] = m_pcmd->m_buttons & IN_ATTACK || m_pcmd->m_buttons & IN_ATTACK2;
			can_shoot[m_pcmd->m_command_number % MULTIPLAYER_BACKUP] = weapon->can_fire(false);

			if (weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER) 
			{
				auto postpone_fire_ready_time = FLT_MAX;
				auto tickrate = (int)(1.0f / m_globals()->m_intervalpertick);

				if (tickrate >> 1 > 1)
				{
					auto command_number = m_pcmd->m_command_number - 1;
					auto shoot_number = 0;
			
					for (auto i = 1; i < tickrate >> 1; ++i)
					{
						shoot_number = command_number;

						if (!in_attack[command_number % MULTIPLAYER_BACKUP] || !can_shoot[command_number % MULTIPLAYER_BACKUP])
							break;

						--command_number;
					}

					if (shoot_number)
					{
						auto tick = 1 - (int)(-0.03348f / m_globals()->m_intervalpertick);

						if (m_pcmd->m_command_number - shoot_number >= tick)
							postpone_fire_ready_time = TICKS_TO_TIME(tickbase_records[(tick + shoot_number) % MULTIPLAYER_BACKUP]) + 0.2f;
					}
				}

				weapon->m_flPostponeFireReadyTime() = postpone_fire_ready_time;
			}
		}

		auto backup_velocity_modifier = player->m_flVelocityModifier();

		player->m_flVelocityModifier() = g_ctx.globals.last_velocity_modifier;
		original_fn(ecx, player, m_pcmd, move_helper);

		if (!g_ctx.globals.in_createmove)
			player->m_flVelocityModifier() = backup_velocity_modifier;
	}
	else
		return original_fn(ecx, player, m_pcmd, move_helper);
}

using InPrediction_t = bool(__thiscall*)(void*);

bool __stdcall hooks::hooked_inprediction()
{
	static auto original_fn = prediction_hook->get_func_address <InPrediction_t> (14);
	static auto maintain_sequence_transitions = util::FindSignature(crypt_str("client.dll"), crypt_str("84 C0 74 17 8B 87"));

	if ((g_cfg.ragebot.enable || g_cfg.legitbot.enabled) && g_ctx.globals.setuping_bones && (uintptr_t)_ReturnAddress() == maintain_sequence_transitions)
		return true;

	return original_fn(m_prediction());
}

using WriteUsercmdDeltaToBuffer_t = bool(__thiscall*)(void*, int, void*, int, int, bool);
void WriteUserÑmd(void* buf, CUserCmd* incmd, CUserCmd* outcmd);

bool __fastcall hooks::hooked_writeusercmddeltatobuffer(void* ecx, void* edx, int slot, bf_write* buf, int from, int to, bool isnewcommand)
{
	static auto orig_write_user_cmd_delta_to_buffer = client_hook->get_func_address <WriteUsercmdDeltaToBuffer_t>(24);


	static auto sendmovecall = (void*)util::FindSignature(crypt_str("engine.dll"), crypt_str("55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98"));

	if (_ReturnAddress() != sendmovecall || g_ctx.globals.tickbase_shift <= 0) {
		g_ctx.globals.tickbase_shift = 0;
		return orig_write_user_cmd_delta_to_buffer(ecx, slot, buf, from, to, isnewcommand);
	}

	if (from != -1)
		return true;

	int m_nTickbase = g_ctx.globals.tickbase_shift;
	g_ctx.globals.tickbase_shift = 0;

	*(int*)((uintptr_t)buf - 0x30) = 0;
	int* m_pnNewCmds = (int*)((uintptr_t)buf - 0x2C);

	int m_nNewCmds = *m_pnNewCmds;
	int m_nNextCmd = m_clientstate()->nLastOutgoingCommand + m_clientstate()->iChokedCommands + 1;
	int m_nTotalNewCmds = min(m_nNewCmds + abs(m_nTickbase), 62);

	*m_pnNewCmds = m_nTotalNewCmds;

	for (to = m_nNextCmd - m_nNewCmds + 1; to <= m_nNextCmd; to++) {
		if (!orig_write_user_cmd_delta_to_buffer(ecx, slot, buf, from, to, true))
			return false;

		from = to;
	}

	CUserCmd* m_pCmd = m_input()->GetUserCmd1(slot, from);//g_pInput->GetUserCmd(nslot, from);
	if (!m_pCmd)
		return true;

	CUserCmd m_FromCmd = *m_pCmd, m_ToCmd = *m_pCmd;
	m_ToCmd.m_command_number++;
	m_ToCmd.m_tickcount += 3 * (int)std::round(1.f / m_globals()->m_intervalpertick);//g_pGlobals->interval_per_tick);

	for (int i = m_nNewCmds; i <= m_nTotalNewCmds; i++) {
		WriteUserÑmd(buf, &m_ToCmd, &m_FromCmd);
		m_FromCmd = m_ToCmd;

		m_ToCmd.m_command_number++;
		m_ToCmd.m_tickcount++;
	}

	return true;
}

void WriteUserÑmd(void* buf, CUserCmd* incmd, CUserCmd* outcmd)
{
	using WriteUserCmd_t = void(__fastcall*)(void*, CUserCmd*, CUserCmd*);
	static auto Fn = (WriteUserCmd_t)util::FindSignature(crypt_str("client.dll"), crypt_str("55 8B EC 83 E4 F8 51 53 56 8B D9"));

	__asm
	{
		mov     ecx, buf
		mov     edx, incmd
		push    outcmd
		call    Fn
		add     esp, 4
	}
}