#include "..\..\includes.hpp"

struct m_keybind
{
	std::string m_name;
	std::string m_mode;

	m_keybind(std::string name, std::string mode) :
		m_name(name), m_mode(mode)
	{

	}
};
class misc : public singleton <misc> 
{
public:
	void watermark();
	//void render_key_binds();
	void NoDuck(CUserCmd* cmd);
	void ChatSpamer();
	void AutoCrouch(CUserCmd* cmd);
	void SlideWalk(CUserCmd* cmd);
	void automatic_peek(CUserCmd* cmd, float wish_yaw);
	void ViewModel();
	void FullBright();
	void PovArrows(player_t* e, Color color);
	void NightmodeFix();
	void zeus_range();
	void desync_arrows();
	void aimbot_hitboxes();
	void ragdolls();
	void rank_reveal();
	void fast_stop(CUserCmd* m_pcmd);
	void spectators_list();	
	void auto_accept();
	bool double_tap(CUserCmd* m_pcmd);
	void hide_shots(CUserCmd* m_pcmd, bool should_work);
	void edgebug(CUserCmd* pCmd);
	void jumpbug(CUserCmd* pCmd);
	void grenade_help();
	void fake_prime();
	void KillVoice(IGameEvent* event);
	void ChangeRegion();
	void blockbot(CUserCmd* cmd);
	void get_keys();
	void KillEffect(IGameEvent* pEvent);
	void keybind_list();
	void left_hand_knife(ClientFrameStage_t stage);
	int width = 900, height = 600;

	void prime();

	bool recharging_double_tap = false;

	bool double_tap_enabled = false;
	bool double_tap_key = false;
	bool doubletap = false;

	bool hide_shots_enabled = false;
	bool hide_shots_key = false;
	std::vector<m_keybind> m_keybinds;
};

class CTeslaInfo
{
public:
	Vector m_vPos;
	Vector m_vAngles;
	int m_nEntIndex;
	const char* m_pszSpriteName;
	float m_flBeamWidth;
	int m_nBeams;
	Vector m_vColor;
	float m_flTimeVisible;
	float m_flRadius;
	float m_flRed;
	float m_flGreen;
	float m_flBlue;
	float m_flBrightness;
};