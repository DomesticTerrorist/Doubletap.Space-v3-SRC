// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "other_esp.h"
#include "..\autowall\autowall.h"
#include "..\ragebot\antiaim.h"
#include "..\misc\logs.h"
#include "..\misc\misc.h"
#include "..\lagcompensation\local_animations.h"

bool can_penetrate(weapon_t* weapon)
{
	auto weapon_info = weapon->get_csweapon_info();

	if (!weapon_info)
		return false;

	Vector view_angles;
	m_engine()->GetViewAngles(view_angles);

	Vector direction;
	math::angle_vectors(view_angles, direction);

	CTraceFilter filter;
	filter.pSkip = g_ctx.local();

	trace_t trace;
	util::trace_line(g_ctx.globals.eye_pos, g_ctx.globals.eye_pos + direction * weapon_info->flRange, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &trace);

	if (trace.fraction == 1.0f) //-V550
		return false;

	auto eye_pos = g_ctx.globals.eye_pos;
	auto hits = 1;
	auto damage = (float)weapon_info->iDamage;
	auto penetration_power = weapon_info->flPenetration;

	static auto damageReductionBullets = m_cvar()->FindVar(crypt_str("ff_damage_reduction_bullets"));
	static auto damageBulletPenetration = m_cvar()->FindVar(crypt_str("ff_damage_bullet_penetration"));

	return autowall::get().handle_bullet_penetration(weapon_info, trace, eye_pos, direction, hits, damage, penetration_power, damageReductionBullets->GetFloat(), damageBulletPenetration->GetFloat());
}

void otheresp::penetration_reticle()
{
	if (!g_cfg.player.enable)
		return;

	if (!g_cfg.esp.penetration_reticle)
		return;

	if (!g_ctx.local()->is_alive())
		return;

	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	auto color = Color::Red;

	if (!weapon->is_non_aim() && weapon->m_iItemDefinitionIndex() != WEAPON_TASER && can_penetrate(weapon))
		color = Color::Green;

	static int width, height;
	m_engine()->GetScreenSize(width, height);
	
	render::get().rect_filled(width / 2, height / 2 - 1, 1, 3, color);
	render::get().rect_filled(width / 2 - 1, height / 2, 3, 1, color);
}

void otheresp::indicators()
{
	if (!g_ctx.local()->is_alive()) //-V807
		return;

	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	if (g_cfg.esp.indicators[INDICATOR_FAKE] && (antiaim::get().type == ANTIAIM_LEGIT || g_cfg.antiaim.type[antiaim::get().type].desync))
	{
		auto color = Color(130, 20, 20);
		auto animstate = g_ctx.local()->get_animation_state();

		if (animstate && local_animations::get().local_data.animstate)
		{
			auto delta = fabs(math::normalize_yaw(animstate->m_flGoalFeetYaw - local_animations::get().local_data.animstate->m_flGoalFeetYaw));
			auto desync_delta = max(g_ctx.local()->get_max_desync_delta(), 58.0f);

			color = Color(235, 72 + (int)(min(delta / desync_delta, 1.0f) * 150.0f), 72);
		}

		m_indicators.push_back(m_indicator("FAKE", color));
	}

	if (g_cfg.esp.indicators[INDICATOR_DESYNC_SIDE] && (antiaim::get().type == ANTIAIM_LEGIT && g_cfg.antiaim.desync == 1 || antiaim::get().type != ANTIAIM_LEGIT && g_cfg.antiaim.type[antiaim::get().type].desync == 1) && !antiaim::get().condition(g_ctx.get_command()))
	{
		auto side = antiaim::get().desync_angle > 0.0f ? "SIDE: R" : "SIDE: L";

		if (antiaim::get().type == ANTIAIM_LEGIT)
			side = antiaim::get().desync_angle > 0.0f ? "LEFT" : "RIGHT";

		m_indicators.push_back(m_indicator(side, Color(235, 72, 72)));
	}

	auto choke_indicator = false;

	if (g_cfg.esp.indicators[INDICATOR_CHOKE] && !fakelag::get().condition && !misc::get().double_tap_enabled && !misc::get().hide_shots_enabled)
	{
		m_indicators.push_back(m_indicator(("FL: " + std::to_string(fakelag::get().max_choke)), Color(235, 72, 72)));
		choke_indicator = true;
	}

	if (g_cfg.esp.indicators[INDICATOR_DAMAGE] && g_ctx.globals.current_weapon != -1 && key_binds::get().get_key_bind_state(4 + g_ctx.globals.current_weapon) && !weapon->is_non_aim())
	{
		if (g_cfg.ragebot.weapon[g_ctx.globals.current_weapon].minimum_override_damage > 100)
			m_indicators.push_back(m_indicator(("DMG: HP + " + std::to_string(g_cfg.ragebot.weapon[g_ctx.globals.current_weapon].minimum_override_damage - 100)), Color(235, 72, 72)));
		else
			m_indicators.push_back(m_indicator(("DMG: " + std::to_string(g_cfg.ragebot.weapon[g_ctx.globals.current_weapon].minimum_override_damage)), Color(235, 72, 72)));
	}

	if (g_cfg.esp.indicators[INDICATOR_SAFE_POINTS] && key_binds::get().get_key_bind_state(3) && !weapon->is_non_aim())
		m_indicators.push_back(m_indicator("SAFEPOINT", Color(235, 72, 72)));

	if (g_cfg.esp.indicators[INDICATOR_BODY_AIM] && key_binds::get().get_key_bind_state(22) && !weapon->is_non_aim())
		m_indicators.push_back(m_indicator("BAIM", Color(235, 72, 72)));

	if (choke_indicator)
		return;

	if (g_cfg.esp.indicators[INDICATOR_DT] && g_cfg.ragebot.double_tap && g_cfg.ragebot.double_tap_key.key > KEY_NONE && g_cfg.ragebot.double_tap_key.key < KEY_MAX && misc::get().double_tap_key)
		m_indicators.push_back(m_indicator("DT", !g_ctx.local()->m_bGunGameImmunity() && !(g_ctx.local()->m_fFlags() & FL_FROZEN) && !antiaim::get().freeze_check && misc::get().double_tap_enabled && !weapon->is_grenade() && weapon->m_iItemDefinitionIndex() != WEAPON_TASER && weapon->m_iItemDefinitionIndex() != WEAPON_REVOLVER && weapon->can_fire(false) ? Color(235, 72, 72) : Color(255, 255, 0)));

	if (g_cfg.esp.indicators[INDICATOR_HS] && g_cfg.antiaim.hide_shots && g_cfg.antiaim.hide_shots_key.key > KEY_NONE && g_cfg.antiaim.hide_shots_key.key < KEY_MAX && misc::get().hide_shots_key)
		m_indicators.push_back(m_indicator("HS", !g_ctx.local()->m_bGunGameImmunity() && !(g_ctx.local()->m_fFlags() & FL_FROZEN) && !antiaim::get().freeze_check && misc::get().hide_shots_enabled ? Color(235, 72, 72) : Color(255, 255, 0)));
}

void otheresp::DrawArrows()
{

	if (g_cfg.antiaim.sidearrow)
	{
		if (!g_ctx.local()->is_alive()) //-V807
			return;

		static int width, height;
		m_engine()->GetScreenSize(width, height);

		static auto alpha = 1.0f;

		auto color = g_cfg.antiaim.invert_indicator_color;
		color.SetAlpha((int)(min(255.0f * alpha, color.a())));

		render::get().triangle(Vector2D(width / 2 - 55, height / 2 + 10), Vector2D(width / 2 - 75, height / 2), Vector2D(width / 2 - 55, height / 2 - 10), Color(0, 0, 0, 150));
		render::get().triangle(Vector2D(width / 2 + 55, height / 2 - 10), Vector2D(width / 2 + 75, height / 2), Vector2D(width / 2 + 55, height / 2 + 10), Color(0, 0, 0, 150));

		if (antiaim::get().desync_angle > 0.0f)
			render::get().triangle(Vector2D(width / 2 - 55, height / 2 + 10), Vector2D(width / 2 - 75, height / 2), Vector2D(width / 2 - 55, height / 2 - 10), color);

		if (antiaim::get().desync_angle < -0.0f)
			render::get().triangle(Vector2D(width / 2 + 55, height / 2 - 10), Vector2D(width / 2 + 75, height / 2), Vector2D(width / 2 + 55, height / 2 + 10), color);

	}
}
/*void otheresp::draw_velocity()
{
	if (!g_cfg.esp.velocity_graph)
		return;

	if (!g_ctx.local())
		return;

	if (!m_engine()->IsInGame() || !g_ctx.local()->is_alive())
		return;

	static std::vector<float> velData(120, 0);

	Vector vecVelocity = g_ctx.local()->m_vecVelocity();
	float currentVelocity = sqrt(vecVelocity.x * vecVelocity.x + vecVelocity.y * vecVelocity.y);

	velData.erase(velData.begin());
	velData.push_back(currentVelocity);

	int vel = g_ctx.local()->m_vecVelocity().Length2D();

	static int width, height;
	m_engine()->GetScreenSize(width, height);
	render::get().text(fonts[VELOCITY], width / 2, height / 1.1, Color(0, 255, 100, 255), HFONT_CENTERED_X | HFONT_CENTERED_Y, "(%i)", vel);


	for (auto i = 0; i < velData.size() - 1; i++)
	{
		int cur = velData.at(i);
		int next = velData.at(i + 1);

		render::get().line(
			width / 2 + (velData.size() * 5 / 2) - (i - 1) * 5.f,
			height / 1.15 - (std::clamp(cur, 0, 450) * .2f),
			width / 2 + (velData.size() * 5 / 2) - i * 5.f,
			height / 1.15 - (std::clamp(next, 0, 450) * .2f), Color(255, 255, 255, 255)
		);
	}
}*/
/*void otheresp::draw_binds()
{

	// Constants for pos, padding, etc...
	const auto pos = ImVec2(10, 500);
	const auto padding = 4; // Padding between keybind elements

	// Constants for colors
	const auto col_background = Color(0, 0, 0, 240); // Watermark background color
	const auto col_accent = Color(90, 120, 240); // Watermark line accent color
	const auto col_text = Color(255, 255, 255); // Watermark text color

	// Element name
	const std::string name = "keybinds";
	const auto name_size = render::get().text(fonts[NAME], name);
	//float name_size_x, name_size_y; render::get_text_size(font::verdana_12, name, name_size_x, name_size_y);

	//pos
	// v
	// [      keybinds      ]
	// ill have two number 9s
	// a number 9 large
	//
	// ^-^ padding
	// ^--------------^ size

	// List of keybinds
	std::vector<std::string> keybinds;

	if (true) // Here your config bool
		keybinds.push_back("ill have two number 9s");

	if (true) // Here your config bool
		keybinds.push_back("a number 9 large");

	if (true) // Here your config bool
		keybinds.push_back("a number 6 with extra dip");

	if (true) // Here your config bool
		keybinds.push_back("a number 7");

	if (true) // Here your config bool
		keybinds.push_back("two number 45s");

	if (true) // Here your config bool
		keybinds.push_back("one with cheese");

	if (true) // Here your config bool
		keybinds.push_back("and a large soda");

	// Adjust width for biggest entry
	auto biggest_text_width = name_size.x;
	// auto biggest_text_width = name_size_width;
	auto keybind_position_y = padding + name_size.y + padding + padding;

	// Run on hooks::panel::paint_traverse
	// Surface rendering
	for (std::string keybind : keybinds) {
		// Get current keybind text size
		const auto keybind_size = render::measure_text(font::verdana_12, keybind);
		// float keybind_size_x, keybind_size_y; render::get_text_size(font::verdana_12, name, keybind_size_x, keybind_size_y);

		// Check if bigger one
		biggest_text_width = std::fmaxf(biggest_text_width, keybind_size.x);

		// Render keybind name
		render::text(pos.x + padding, pos.y + keybind_position_y, font::verdana_12, keybind, col_text);

		// Calculating position for our next keybind
		keybind_position_y += keybind_size.y + padding;
	}

	auto bg_size = vec2(padding + biggest_text_width + padding,
		padding + name_size.y + padding);

	render::get().rect_filled(pos.x, pos.y, bg_size.x, bg_size.y, col_background); // Background
	render::get().rect_filled(pos.x, pos.y, bg_size.x, 2, col_accent); // Accent line
	render::get().text(pos.x + (bg_size.x * 0.5f) - (name_size.x * 0.5f), pos.y + padding, font::verdana_12, name, col_text); // Element name
	// If your rendering class has ALIGN_CENTER flag, you can use this flag instead of - (name_size.x * 0.5f)


}
*/

/*void otheresp::draw_binds()
{
	if (!g_cfg.misc.draw_keys)
		return;

	//if (!g_ctx.local()->is_alive()) //-V807
		//return;

	static int width, height;
	m_engine()->GetScreenSize(width, height);

	//auto color = g_cfg.menu.menu_theme;
	int x{ 15 };
	int offset = 1;

	// overlays 
	render::get().rect_filled(x + 10, height / 2 + 10, 200, 5 + 15 * offset, { 10, 10, 10, 160 });
	// line.
	render::get().rect_filled(x + 10, height / 2 + 10, 200, 2, { 111,0,255, 255 });
	// text.
	render::get().text(fonts[KEYBINDS], x + 10 + 60, height / 2 + 10.5, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Key Binds");

	// condition dt
	if (misc::get().double_tap_key)
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "DoubleTap");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Enabled");
		offset = offset + 2;
	}

	// hs
	if (misc::get().hide_shots_key)
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Hide Shots");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Enabled");
		offset = offset + 2;
	}

	// mindmg 
	if (key_binds::get().get_key_bind_state(4 + g_ctx.globals.current_weapon))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Min DMG");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Enabled");
		offset = offset + 2;
	}

	// baim
	if (key_binds::get().get_key_bind_state(22))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "BAIM");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Enabled");
		offset = offset + 2;
	}

	// resolver override
	if (key_binds::get().get_key_bind_state(24))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Resolver Override");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Enabled");
		offset = offset + 2;
	}

	// thirdperson
	if (key_binds::get().get_key_bind_state(17))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Thirdperson");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Enabled");
		offset = offset + 2;
	}

	// slowwalk
	if (key_binds::get().get_key_bind_state(21))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Slow Walk");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "enable");
		offset = offset + 2;
	}

	// fakeduck
	if (key_binds::get().get_key_bind_state(20))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Fake Duck");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "enable");
		offset = offset + 2;
	}

	// edgejump
	if (key_binds::get().get_key_bind_state(19))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Edge jump");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "enable");
		offset = offset + 2;
	}

	// auto peek 
	if (key_binds::get().get_key_bind_state(18))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Auto Peek");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "enable");
		offset = offset + 2;
	}

	// auto peek 
	if (key_binds::get().get_key_bind_state(16))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "AA Invert");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "enable");
		offset = offset + 2;
	}

	// freestanding
	if (key_binds::get().get_key_bind_state(26))
	{
		render::get().text(fonts[KEYBINDS], x + 10 + 6, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "Freestanding");
		//render::get().text(fonts[KEYBINDS], x + 10 + 163, height / 2 + 15 + 15 * offset, { 255, 255, 255, 255 }, HFONT_CENTERED_NONE, "enable");
		offset = offset + 2;
	}
}*/
void otheresp::draw_indicators()
{
	if (!g_ctx.local()->is_alive()) //-V807
		return;

	static int width, height;
	m_engine()->GetScreenSize(width, height);

	auto h = height - 325;

	for (auto& indicator : m_indicators)
	{
		render::get().text(fonts[INDICATORFONT], 15, h, indicator.m_color, HFONT_CENTERED_Y, indicator.m_text.c_str());
		h -= 40;
	}

	m_indicators.clear();
}

void otheresp::hitmarker_paint()
{
	if (!g_cfg.esp.hitmarker[0] && !g_cfg.esp.hitmarker[1])
	{
		hitmarker.hurt_time = FLT_MIN;
		hitmarker.point = ZERO;
		return;
	}

	if (!g_ctx.local()->is_alive())
	{
		hitmarker.hurt_time = FLT_MIN;
		hitmarker.point = ZERO;
		return;
	}

	if (hitmarker.hurt_time + 0.7f > m_globals()->m_curtime)
	{
		if (g_cfg.esp.hitmarker[0])
		{
			static int width, height;
			m_engine()->GetScreenSize(width, height);

			auto alpha = (int)((hitmarker.hurt_time + 0.7f - m_globals()->m_curtime) * 255.0f);
			hitmarker.hurt_color.SetAlpha(alpha);

			auto offset = 7.0f - (float)alpha / 255.0f * 7.0f;

			render::get().line(width / 2 + 5 + offset, height / 2 - 5 - offset, width / 2 + 12 + offset, height / 2 - 12 - offset, hitmarker.hurt_color);
			render::get().line(width / 2 + 5 + offset, height / 2 + 5 + offset, width / 2 + 12 + offset, height / 2 + 12 + offset, hitmarker.hurt_color);
			render::get().line(width / 2 - 5 - offset, height / 2 + 5 + offset, width / 2 - 12 - offset, height / 2 + 12 + offset, hitmarker.hurt_color);
			render::get().line(width / 2 - 5 - offset, height / 2 - 5 - offset, width / 2 - 12 - offset, height / 2 - 12 - offset, hitmarker.hurt_color);
		}

		if (g_cfg.esp.hitmarker[1])
		{
			Vector world;

			if (math::world_to_screen(hitmarker.point, world))
			{
				auto alpha = (int)((hitmarker.hurt_time + 0.7f - m_globals()->m_curtime) * 255.0f);
				hitmarker.hurt_color.SetAlpha(alpha);

				auto offset = 7.0f - (float)alpha / 255.0f * 7.0f;

				render::get().line(world.x + 5 + offset, world.y - 5 - offset, world.x + 12 + offset, world.y - 12 - offset, hitmarker.hurt_color);
				render::get().line(world.x + 5 + offset, world.y + 5 + offset, world.x + 12 + offset, world.y + 12 + offset, hitmarker.hurt_color);
				render::get().line(world.x - 5 - offset, world.y + 5 + offset, world.x - 12 - offset, world.y + 12 + offset, hitmarker.hurt_color);
				render::get().line(world.x - 5 - offset, world.y - 5 - offset, world.x - 12 - offset, world.y - 12 - offset, hitmarker.hurt_color);
			}
		}
	}
}

void otheresp::damage_marker_paint()
{
	for (auto i = 1; i < m_globals()->m_maxclients; i++) //-V807
	{
		if (damage_marker[i].hurt_time + 2.0f > m_globals()->m_curtime)
		{
			Vector screen;

			if (!math::world_to_screen(damage_marker[i].position, screen))
				continue;

			auto alpha = (int)((damage_marker[i].hurt_time + 2.0f - m_globals()->m_curtime) * 127.5f);
			damage_marker[i].hurt_color.SetAlpha(alpha);

			render::get().text(fonts[DAMAGE_MARKER], screen.x, screen.y, damage_marker[i].hurt_color, HFONT_CENTERED_X | HFONT_CENTERED_Y, "%i", damage_marker[i].damage);
		}
	}
}

void draw_circe(float x, float y, float radius, int resolution, DWORD color, DWORD color2, LPDIRECT3DDEVICE9 device);

void otheresp::spread_crosshair(LPDIRECT3DDEVICE9 device)
{
	if (!g_cfg.player.enable)
		return;

	if (!g_cfg.esp.show_spread)
		return;

	if (!g_ctx.local()->is_alive())
		return;

	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (weapon->is_non_aim())
		return;

	int w, h;
	m_engine()->GetScreenSize(w, h);

	draw_circe((float)w * 0.5f, (float)h * 0.5f, g_ctx.globals.inaccuracy * 500.0f, 50, D3DCOLOR_RGBA(g_cfg.esp.show_spread_color.r(), g_cfg.esp.show_spread_color.g(), g_cfg.esp.show_spread_color.b(), g_cfg.esp.show_spread_color.a()), D3DCOLOR_RGBA(0, 0, 0, 0), device);
}

void draw_circe(float x, float y, float radius, int resolution, DWORD color, DWORD color2, LPDIRECT3DDEVICE9 device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2 = nullptr;
	std::vector <CUSTOMVERTEX2> circle(resolution + 2);

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0.0f;

	circle[0].rhw = 1.0f;
	circle[0].color = color2;

	for (auto i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - radius * cos(D3DX_PI * ((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - radius * sin(D3DX_PI * ((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0.0f;

		circle[i].rhw = 1.0f;
		circle[i].color = color;
	}

	device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX2), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, nullptr); //-V107

	if (!g_pVB2)
		return;

	void* pVertices;

	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX2), (void**)&pVertices, 0); //-V107
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX2));
	g_pVB2->Unlock();

	device->SetTexture(0, nullptr);
	device->SetPixelShader(nullptr);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX2));
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);

	g_pVB2->Release();
}

void otheresp::automatic_peek_indicator()
{
	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	static auto position = ZERO;

	if (!g_ctx.globals.start_position.IsZero())
		position = g_ctx.globals.start_position;

	if (position.IsZero())
		return;

	static auto alpha = 0.0f;

	if (!weapon->is_non_aim() && key_binds::get().get_key_bind_state(18) || alpha)
	{
		if (!weapon->is_non_aim() && key_binds::get().get_key_bind_state(18))
			alpha += 3.0f * m_globals()->m_frametime; //-V807
		else
			alpha -= 3.0f * m_globals()->m_frametime;

		alpha = math::clamp(alpha, 0.0f, 1.0f);
		render::get().Draw3DFilledCircle(position, 25.0f, g_ctx.globals.fired_shot ? Color(30, 240, 30, (int)(alpha * 255.0f)) : Color(240, 30, 30, (int)(alpha * 255.0f)));

		Vector screen;

		if (math::world_to_screen(position, screen))
		{
			static auto offset = 30.0f;

			if (!g_ctx.globals.fired_shot)
			{
				static auto switch_offset = false;

				if (offset <= 30.0f || offset >= 55.0f)
					switch_offset = !switch_offset;

				offset += switch_offset ? 22.0f * m_globals()->m_frametime : -22.0f * m_globals()->m_frametime;
				offset = math::clamp(offset, 30.0f, 55.0f);

				render::get().rect_filled(screen.x - 10.0f, screen.y - offset - 59.0f, 20.0f, 45.0f, Color(240, 30, 30, (int)(alpha * 168.0f)));
				render::get().triangle(Vector2D(screen.x, screen.y - offset), Vector2D(screen.x - 10.5f, screen.y - offset - 14.5f), Vector2D(screen.x + 9.5f, screen.y - offset - 14.5f), Color(240, 30, 30, (int)(alpha * 168.0f)));
			}
		}
	}
}

