#include "menu.h"
#include "..\configs\cfg_system.h"
#include "../ImGui/code_editor.h"
#include "../constchars.h"
#include "../cheats/misc/misc.h"
#include "../../inventory/inventorychanger.h"
#include "../../protobuffs/Protobuffs.h"
#include "../../inventory/items.h"
#include "../skinchanger/skins.h"
#include "../../protobuffs/Messages.h"

#define ALPHA (ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar| ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float)
#define NOALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float)

static auto menu_setupped = false;

std::string FindURl(std::string paintName)
{
	std::ifstream inFile;
	inFile.open(".\\csgo\\scripts\\items\\items_game_cdn.txt");
	std::string line;
	//int founds = 0;
	unsigned int curLine = 0;
	std::string search = paintName.append("=");
	//string weapon = "aug";
	while (getline(inFile, line)) {
		curLine++;
		if (line.find(search, 0) != std::string::npos)
		{
			return line.substr(line.find(search)).erase(0, search.length());
		}

	}
	return "";
}
static IDirect3DTexture9* some_texture_test;
void DownloadBytes(const char* const szUrl)
{
	char sysdir[MAX_PATH] = { 0 };
	char Path[MAX_PATH] = { 0 };
	GetWindowsDirectory(sysdir, MAX_PATH);
	sprintf(Path, "C:\\test\\image.png", sysdir);
	URLDownloadToFile(NULL, szUrl, Path, 0, NULL);
}

// p hittings foenem
#include <WinInet.h>

std::string fDownloadBytes(const char* const szUrl)
{
	HINTERNET hOpen = NULL;
	HINTERNET hFile = NULL;
	char* lpBuffer = NULL;
	DWORD dwBytesRead = 0;
	//Pointer to dynamic buffer.
	char* data = 0;
	//Dynamic data size.
	DWORD dataSize = 0;

	hOpen = InternetOpenA("smef.pw INDIGO/4.3 paste", NULL, NULL, NULL, NULL);
	if (!hOpen) return (char*)"";

	hFile = InternetOpenUrlA(hOpen, szUrl, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, NULL);

	if (!hFile) {
		InternetCloseHandle(hOpen);
		return (char*)"";
	}

	std::string output;
	do {
		char buffer[2000];
		InternetReadFile(hFile, (LPVOID)buffer, _countof(buffer), &dwBytesRead);
		output.append(buffer, dwBytesRead);
	} while (dwBytesRead);

	InternetCloseHandle(hFile);
	InternetCloseHandle(hOpen);

	return output;
}

static std::string old_link;
void UpdatePic(IDirect3DDevice9* thisptr, std::string link)
{
	if (old_link == link)
		return;
	std::string imData = fDownloadBytes(link.c_str());
	D3DXCreateTextureFromFileInMemoryEx(thisptr
		, imData.data(), imData.length(),
		300, 300, D3DUSAGE_DYNAMIC, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &some_texture_test);

	old_link = link;
}



static const char* zweaponnames(const short id)
{
	switch (id)
	{
	case WEAPON_DEAGLE:
		return "deagle";
	case WEAPON_ELITE:
		return "elite";
	case WEAPON_FIVESEVEN:
		return "fiveseven";
	case WEAPON_GLOCK:
		return "glock";
	case WEAPON_AK47:
		return "ak47";
	case WEAPON_AUG:
		return "aug";
	case WEAPON_AWP:
		return "awp";
	case WEAPON_FAMAS:
		return "famas";
	case WEAPON_G3SG1:
		return "g3sg1";
	case WEAPON_GALILAR:
		return "galilar";
	case WEAPON_M249:
		return "m249";
	case WEAPON_M4A1_SILENCER:
		return "m4a1_silencer";
	case WEAPON_M4A1:
		return "m4a1";
	case WEAPON_MAC10:
		return "mac10";
	case WEAPON_P90:
		return "p90";
	case WEAPON_UMP45:
		return "ump45";
	case WEAPON_XM1014:
		return "xm1014";
	case WEAPON_BIZON:
		return "bizon";
	case WEAPON_MAG7:
		return "mag7";
	case WEAPON_NEGEV:
		return "negev";
	case WEAPON_SAWEDOFF:
		return "sawedoff";
	case WEAPON_TEC9:
		return "tec9";
	case WEAPON_HKP2000:
		return "hkp2000";
	case WEAPON_MP5SD:
		return "mp5sd";
	case WEAPON_MP7:
		return "mp7";
	case WEAPON_MP9:
		return "mp9";
	case WEAPON_NOVA:
		return "nova";
	case WEAPON_P250:
		return "p250";
	case WEAPON_SCAR20:
		return "scar20";
	case WEAPON_SG556:
		return "sg556";
	case WEAPON_SSG08:
		return "ssg08";
	case WEAPON_USP_SILENCER:
		return "usp_silencer";
	case WEAPON_CZ75A:
		return "cz75a";
	case WEAPON_REVOLVER:
		return "revolver";
	case WEAPON_KNIFE:
		return "knife";
	case WEAPON_KNIFE_T:
		return "knife_t";
	case WEAPON_KNIFE_M9_BAYONET:
		return "knife_m9_bayonet";
	case WEAPON_BAYONET:
		return "bayonet";
	case WEAPON_KNIFE_FLIP:
		return "knife_flip";
	case WEAPON_KNIFE_GUT:
		return "knife_gut";
	case WEAPON_KNIFE_KARAMBIT:
		return "knife_karambit";
	case WEAPON_KNIFE_TACTICAL:
		return "knife_tactical";
	case WEAPON_KNIFE_FALCHION:
		return "knife_falchion";
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		return "knife_survival_bowie";
	case WEAPON_KNIFE_BUTTERFLY:
		return "knife_butterfly";
	case WEAPON_KNIFE_PUSH:
		return "knife_push";
	case WEAPON_KNIFE_URSUS:
		return "knife_ursus";
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		return "knife_gypsy_jackknife";
	case WEAPON_KNIFE_STILETTO:
		return "knife_stiletto";
	case WEAPON_KNIFE_WIDOWMAKER:
		return "knife_widowmaker";
	case WEAPON_KNIFE_SKELETON:
		return "knife_skeleton";
	case WEAPON_KNIFE_OUTDOOR:
		return "knife_outdoor";
	case WEAPON_KNIFE_CANIS:
		return "knife_canis";
	case WEAPON_KNIFE_CORD:
		return "knife_cord";
	case WEAPON_KNIFE_CSS:
		return "knife_css";
	case STUDDED_BLOODHOUND_GLOVES:
		return "studded_bloodhound_gloves";
	case T_GLOVES:
		return "t_gloves";
	case CT_GLOVES:
		return "ct_gloves";
	case SPORTY_GLOVES:
		return "sporty_gloves";
	case SLICK_GLOVES:
		return "slick_gloves";
	case LEATHER_HANDWRAPS:
		return "leather_handwraps";
	case MOTORCYCLE_GLOVES:
		return "motorcycle_gloves";
	case SPECIALIST_GLOVES:
		return "specialist_gloves";
	case STUDDED_HYDRA_GLOVES:
		return "studded_hydra_gloves";
	default:
		return "";
	}
}

void c_menu::menu_setup(ImGuiStyle& style) {
	ImGui::StyleColorsClassic(); // colors setup

	styles.WindowPadding = style.WindowPadding;
	styles.WindowRounding = style.WindowRounding;
	styles.WindowMinSize = style.WindowMinSize;
	styles.ChildRounding = style.ChildRounding;
	styles.PopupRounding = style.PopupRounding;
	styles.FramePadding = style.FramePadding;
	styles.FrameRounding = style.FrameRounding;
	styles.ItemSpacing = style.ItemSpacing;
	styles.ItemInnerSpacing = style.ItemInnerSpacing;
	styles.TouchExtraPadding = style.TouchExtraPadding;
	styles.IndentSpacing = style.IndentSpacing;
	styles.ColumnsMinSpacing = style.ColumnsMinSpacing;
	styles.ScrollbarSize = style.ScrollbarSize;
	styles.ScrollbarRounding = style.ScrollbarRounding;
	styles.GrabMinSize = style.GrabMinSize;
	styles.GrabRounding = style.GrabRounding;
	styles.TabRounding = style.TabRounding;
	styles.TabMinWidthForUnselectedCloseButton = style.TabMinWidthForUnselectedCloseButton;
	styles.DisplayWindowPadding = style.DisplayWindowPadding;
	styles.DisplaySafeAreaPadding = style.DisplaySafeAreaPadding;
	styles.MouseCursorScale = style.MouseCursorScale;

	menu_setupped = true;
}

void c_menu::dpi_resize(float scale_factor, ImGuiStyle& style) //-V688
{
	style.WindowPadding = (styles.WindowPadding * scale_factor);
	style.WindowRounding = (styles.WindowRounding * scale_factor);
	style.WindowMinSize = (styles.WindowMinSize * scale_factor);
	style.ChildRounding = (styles.ChildRounding * scale_factor);
	style.PopupRounding = (styles.PopupRounding * scale_factor);
	style.FramePadding = (styles.FramePadding * scale_factor);
	style.FrameRounding = (styles.FrameRounding * scale_factor);
	style.ItemSpacing = (styles.ItemSpacing * scale_factor);
	style.ItemInnerSpacing = (styles.ItemInnerSpacing * scale_factor);
	style.TouchExtraPadding = (styles.TouchExtraPadding * scale_factor);
	style.IndentSpacing = (styles.IndentSpacing * scale_factor);
	style.ColumnsMinSpacing = (styles.ColumnsMinSpacing * scale_factor);
	style.ScrollbarSize = (styles.ScrollbarSize * scale_factor);
	style.ScrollbarRounding = (styles.ScrollbarRounding * scale_factor);
	style.GrabMinSize = (styles.GrabMinSize * scale_factor);
	style.GrabRounding = (styles.GrabRounding * scale_factor);
	style.TabRounding = (styles.TabRounding * scale_factor);
	if (styles.TabMinWidthForUnselectedCloseButton != FLT_MAX) //-V550
		style.TabMinWidthForUnselectedCloseButton = (styles.TabMinWidthForUnselectedCloseButton * scale_factor);
	style.DisplayWindowPadding = (styles.DisplayWindowPadding * scale_factor);
	style.DisplaySafeAreaPadding = (styles.DisplaySafeAreaPadding * scale_factor);
	style.MouseCursorScale = (styles.MouseCursorScale * scale_factor);
}

__forceinline void padding(float x, float y)
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x * c_menu::get().dpi_scale);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y * c_menu::get().dpi_scale);
}

void draw_combo(const char* name, int& variable, const char* labels[], int count)
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6 * c_menu::get().dpi_scale);
	ImGui::Text(name);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5 * c_menu::get().dpi_scale);
	ImGui::Combo(std::string(crypt_str("##COMBO__") + std::string(name)).c_str(), &variable, labels, count);
}

/*void draw_combo(const char* name, int& variable, bool (*items_getter)(void*, int, const char**), void* data, int count)
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6 * c_menu::get().dpi_scale);
	ImGui::Text(name);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5 * c_menu::get().dpi_scale);
	ImGui::Combo(std::string(crypt_str("##COMBO__") + std::string(name)).c_str(), &variable, items_getter, data, count);
}*/

void draw_multicombo(std::string name, std::vector<int>& variable, const char* labels[], int count, std::string& preview)
{
	padding(-3, -6);
	ImGui::Text((crypt_str(" ") + name).c_str());
	padding(0, -5);

	auto hashname = crypt_str("##") + name; // we dont want to render name of combo

	for (auto i = 0, j = 0; i < count; i++)
	{
		if (variable[i])
		{
			if (j)
				preview += crypt_str(", ") + (std::string)labels[i];
			else
				preview = labels[i];

			j++;
		}
	}

	if (ImGui::BeginCombo(hashname.c_str(), preview.c_str())) // draw start
	{
		ImGui::Spacing();
		ImGui::BeginGroup();
		{

			for (auto i = 0; i < count; i++)
				ImGui::Selectable(labels[i], (bool*)&variable[i], ImGuiSelectableFlags_DontClosePopups);

		}
		ImGui::EndGroup();
		ImGui::Spacing();

		ImGui::EndCombo();
	}

	preview = crypt_str("None"); // reset preview to use later
}

bool LabelClick(const char* label, bool* v, const char* unique_id)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	// The concatoff/on thingies were for my weapon config system so if we're going to make that, we still need this aids.
	char Buf[64];
	_snprintf(Buf, 62, crypt_str("%s"), label);

	char getid[128];
	sprintf_s(getid, 128, crypt_str("%s%s"), label, unique_id);


	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(getid);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	const ImRect check_bb(window->DC.CursorPos, ImVec2(label_size.y + style.FramePadding.y * 2 + window->DC.CursorPos.x, window->DC.CursorPos.y + label_size.y + style.FramePadding.y * 2));
	ImGui::ItemSize(check_bb, style.FramePadding.y);

	ImRect total_bb = check_bb;

	if (label_size.x > 0)
	{
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		const ImRect text_bb(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y + style.FramePadding.y), ImVec2(window->DC.CursorPos.x + label_size.x, window->DC.CursorPos.y + style.FramePadding.y + label_size.y));

		ImGui::ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
		total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
	}

	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
		*v = !(*v);

	if (*v)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(126 / 255.f, 131 / 255.f, 219 / 255.f, 1.f));
	if (label_size.x > 0.0f)
		ImGui::RenderText(ImVec2(check_bb.GetTL().x + 12, check_bb.GetTL().y), Buf);
	if (*v)
		ImGui::PopStyleColor();

	return pressed;

}
void draw_keybind(const char* label, key_bind* key_bind, const char* unique_id)
{
	// reset bind if we re pressing esc
	if (key_bind->key == KEY_ESCAPE)
		key_bind->key = KEY_NONE;

	auto clicked = false;
	auto text = (std::string)m_inputsys()->ButtonCodeToString(key_bind->key);

	if (key_bind->key <= KEY_NONE || key_bind->key >= KEY_MAX)
		text = crypt_str("None");

	// if we clicked on keybind
	if (hooks::input_shouldListen && hooks::input_receivedKeyval == &key_bind->key)
	{
		clicked = true;
		text = crypt_str("...");
	}

	auto textsize = ImGui::CalcTextSize(text.c_str()).x + 8 * c_menu::get().dpi_scale;
	auto labelsize = ImGui::CalcTextSize(label);

	ImGui::Text(label);
	ImGui::SameLine();

	ImGui::SetCursorPosX(ImGui::GetWindowSize().x - (ImGui::GetWindowSize().x - ImGui::CalcItemWidth()) - max(50 * c_menu::get().dpi_scale, textsize));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3 * c_menu::get().dpi_scale);

	if (ImGui::KeybindButton(text.c_str(), unique_id, ImVec2(max(50 * c_menu::get().dpi_scale, textsize), 23 * c_menu::get().dpi_scale), clicked))
		clicked = true;


	if (clicked)
	{
		hooks::input_shouldListen = true;
		hooks::input_receivedKeyval = &key_bind->key;
	}

	static auto hold = false, toggle = false;

	switch (key_bind->mode)
	{
	case HOLD:
		hold = true;
		toggle = false;
		break;
	case TOGGLE:
		toggle = true;
		hold = false;
		break;
	}

	if (ImGui::BeginPopup(unique_id))
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6 * c_menu::get().dpi_scale);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((ImGui::GetCurrentWindow()->Size.x / 2) - (ImGui::CalcTextSize(crypt_str("Hold")).x / 2)));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 11);

		if (LabelClick(crypt_str("Hold"), &hold, unique_id))
		{
			if (hold)
			{
				toggle = false;
				key_bind->mode = HOLD;
			}
			else if (toggle)
			{
				hold = false;
				key_bind->mode = TOGGLE;
			}
			else
			{
				toggle = false;
				key_bind->mode = HOLD;
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((ImGui::GetCurrentWindow()->Size.x / 2) - (ImGui::CalcTextSize(crypt_str("Toggle")).x / 2)));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 11);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9 * c_menu::get().dpi_scale);

		if (LabelClick(crypt_str("Toggle"), &toggle, unique_id))
		{
			if (toggle)
			{
				hold = false;
				key_bind->mode = TOGGLE;
			}
			else if (hold)
			{
				toggle = false;
				key_bind->mode = HOLD;
			}
			else
			{
				hold = false;
				key_bind->mode = TOGGLE;
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
void draw_semitabs(const char* labels[], int count, int& tab, ImGuiStyle& style)
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (2 * c_menu::get().dpi_scale));

	// center of main child
	float offset = 343 * c_menu::get().dpi_scale;

	// text size padding + frame padding
	for (int i = 0; i < count; i++)
		offset -= (ImGui::CalcTextSize(labels[i]).x) / 2 + style.FramePadding.x * 2;

	// set new padding
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
	ImGui::BeginGroup();

	for (int i = 0; i < count; i++)
	{
		// switch current tab
		if (ImGui::ContentTab(labels[i], tab == i))
			tab = i;

		// continue drawing on same line 
		if (i + 1 != count)
		{
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + style.ItemSpacing.x);
		}
	}
	ImGui::EndGroup();
}
void RenderCheckMar1k(ImVec2 pos, ImU32 col, float sz)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	float thickness = ImMax(sz / 5.0f, 1.0f);
	sz -= thickness * 0.5f;
	pos = ImVec2(pos.x + thickness * 0.25f, pos.y + thickness * 0.25f);

	float third = sz / 3.0f;
	float bx = pos.x + third;
	float by = pos.y + sz - third * 0.5f;
	window->DrawList->PathLineTo(ImVec2(bx - third, by - third));
	window->DrawList->PathLineTo(ImVec2(bx, by));
	window->DrawList->PathLineTo(ImVec2(bx + third * 2, by - third * 2));
	window->DrawList->PathStroke(col, false, thickness);
}

bool Checkbox(const char* label, bool* v)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	const float square_sz = ImGui::GetFrameHeight();
	const ImVec2 pos = window->DC.CursorPos;
	const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + label_size.y + style.FramePadding.y * 2.0f));
	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
	{
		*v = !(*v);
		ImGui::MarkItemEdited(id);
	}

	const ImRect check_bb(pos, ImVec2(pos.x + square_sz, pos.y + square_sz));

	float t = *v ? 1.0f : 0.0f;

	float ANIM_SPEED = 0.05f;
	if (g.LastActiveIdTimer == g.CurrentWindow->GetID(label))// && g.LastActiveIdTimer < ANIM_SPEED)
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (t_anim) : (1.0f - t_anim);
	}

	ImU32 col_bg = ImGui::GetColorU32(ImVec4(120 / 255.f, 120 / 255.f, 120 / 255.f, 120 / 255.f));
	ImU32 col_bg2;
	ImU32 col_bg3;

	col_bg2 = ImGui::GetColorU32(ImLerp(ImVec4(190 / 255.f, 190 / 255.f, 190 / 255.f, 0 / 255.f), ImVec4(5 / 255.f, 193 / 255.f, 245 / 255.f, 255.f / 255.f), t));
	col_bg3 = ImGui::GetColorU32(ImLerp(ImVec4(190 / 255.f, 190 / 255.f, 190 / 255.f, 0 / 255.f), ImVec4(255, 255, 255, 255), t));

	window->DrawList->AddRect(check_bb.Min, check_bb.Max, col_bg, 4.f, 15, 2.f);
	window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, col_bg2, 4.f);

	if (*v)
	{
		RenderCheckMar1k(ImVec2{ check_bb.Min.x + 4,check_bb.Min.y + 4 }, col_bg3, square_sz - 8);
	}

	if (label_size.x > 0.0f)
		ImGui::RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);

	return pressed;
}
void child_title(const char* label)
{
	ImGui::PushFont(c_menu::get().big);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImGui::GetStyle().Colors[ImGuiCol_Text].x, ImGui::GetStyle().Colors[ImGuiCol_Text].y, ImGui::GetStyle().Colors[ImGuiCol_Text].z, 255.f));

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (12 * c_menu::get().dpi_scale));
	ImGui::Text(label);

	ImGui::PopStyleColor();
	ImGui::PopFont();
}
bool Tab(const char* icon, const char* label, const char* desc, const ImVec2& size_arg, const bool selected)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	static float sizeplus = 0.f;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 pos = window->DC.CursorPos;

	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

	if (selected)
		window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(25, 30, 35));

	ImGui::PushFont(c_menu::get().big);
	window->DrawList->AddText(ImVec2(bb.Min.x + 35, bb.Min.y + 18), ImColor(255 / 255.f, 255 / 255.f, 255 / 255.f, 255.f / 255.f), label);
	ImGui::PopFont();

	//window->DrawList->AddText(ImVec2(bb.Min.x + 35, bb.Min.y + 26), ImColor(100 / 255.f, 100 / 255.f, 100 / 255.f, 255.f / 255.f), desc);

	ImGui::PushFont(c_menu::get().new_icons);
	window->DrawList->AddText(ImVec2(bb.Min.x + 5, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(icon).y / 2), ImColor(111 / 255.f, 0 / 255.f, 255 / 255.f, 255.f / 255.f), icon);
	ImGui::PopFont();

	return pressed;
}


bool SubTab(const char* label, const ImVec2& size_arg, const bool selected)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	static float sizeplus = 0.f;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;

	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

	if (selected)
		window->DrawList->AddRectFilled({ bb.Min.x,bb.Min.y }, { bb.Max.x,bb.Max.y }, ImColor(30, 35, 40, 200));

	if (selected)
		window->DrawList->AddRectFilled({ bb.Max.x,bb.Max.y }, { bb.Max.x - 3,bb.Min.y }, ImColor(111 / 255.f, 0 / 255.f, 255 / 255.f, 255.f / 255.f));

	ImGui::PushFont(c_menu::get().big);
	window->DrawList->AddText(ImVec2(bb.Min.x + 5, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2), ImColor(255 / 255.f, 255 / 255.f, 255 / 255.f, 255.f / 255.f), label);
	ImGui::PopFont();


	return pressed;
}


void c_menu::draw(bool is_open)
{
	static auto w = 0, h = 0, current_h = 0;
	m_engine()->GetScreenSize(w, current_h);
	static bool preview_reverse = false;

	// animation related code
	static float m_alpha = 1.0f; //0.0002f
	//m_alpha = math::clamp(m_alpha + (3.f * ImGui::GetIO().DeltaTime * (is_open ? 1.f : -1.f)), 0.0001f, 1.f);

	// set alpha in class to use later in widgets
	public_alpha = m_alpha;

	if (m_alpha <= 0.0001f)
		return;

	// set new alpha
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_alpha);

	if (!menu_setupped)
		menu_setup(ImGui::GetStyle());

	if (!hooks::menu_open && ImGui::GetStyle().Alpha > 0.f) {
		ImGui::GetStyle().Alpha = std::clamp(ImGui::GetStyle().Alpha - 255.f, 0.f, 1.f);
	}

	if (hooks::menu_open && ImGui::GetStyle().Alpha < 1.f) {
		ImGui::GetStyle().Alpha = std::clamp(ImGui::GetStyle().Alpha + 255.f, 0.f, 1.f);
	}

	if (!hooks::menu_open && ImGui::GetStyle().Alpha == 0.f)
		return;

	static int tab = 0;
	static int subtab = 99;
	static int type = 0;
	const char* players_sections[3] = { crypt_str("Enemy"), crypt_str("Team"), crypt_str("Local") };
	auto player = players_section;


	auto flags_menu = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::SetNextWindowSize({ 820,490 });
	ImGui::Begin("##menu", nullptr, flags_menu);
	{
		ImDrawList* draw = ImGui::GetWindowDrawList();
		ImVec2 pos = ImGui::GetWindowPos();

		draw->AddRectFilled({ pos.x,pos.y }, { pos.x + 820,pos.y + 50 }, ImColor(19, 22, 26), 6.f, ImDrawCornerFlags_Top);
		draw->AddRectFilled({ pos.x ,pos.y + 50 }, { pos.x + 820,pos.y + 490 }, ImColor(25, 30, 35), 6.f, ImDrawCornerFlags_All);
		draw->AddRectFilled({ pos.x ,pos.y + 470 }, { pos.x + 820,pos.y + 490 }, ImColor(45, 50, 55), 6.f, ImDrawCornerFlags_Bot);

		draw->AddText({ pos.x + 810 - ImGui::CalcTextSize("DoubleTap.Space v3").x,pos.y + 470 + 10 - ImGui::CalcTextSize("DoubleTap.Space v3").y / 2 }, ImColor(250, 250, 250), "DoubleTap.Space v3");

		ImGui::PushFont(vermin);
		ImGui::SetCursorPos({ 10,25 - ImGui::CalcTextSize("DoubleTap.Space").y / 2 });
		ImGui::Text("DoubleTap.Space");
		ImGui::PopFont();

		ImGui::SetCursorPos({ 200,0 });
		ImGui::BeginGroup();
		{
			if (Tab("e", "Ragebot", "", ImVec2(95, 50), tab == 0 ? true : false))
				tab = 0;
			ImGui::SameLine();
			if (Tab("e", "Legitbot", "", ImVec2(95, 50), tab == 5 ? true : false))
				tab = 5;
			ImGui::SameLine();
			if (Tab("b", "Visuals", "", ImVec2(95, 50), tab == 1 ? true : false))
				tab = 1;
			ImGui::SameLine();
			if (Tab("c", "Misc", "", ImVec2(95, 50), tab == 2 ? true : false))
				tab = 2;
			ImGui::SameLine();
			if (Tab("a", "Skins", "", ImVec2(95, 50), tab == 3 ? true : false))
				tab = 3;
			ImGui::SameLine();
			if (Tab("f", "Configs", "", ImVec2(95, 50), tab == 4 ? true : false))
				tab = 4;
		}
		ImGui::EndGroup();

		ImGui::SetCursorPos({ 10,60 });
		ImGui::BeginGroup();
		{
			if (tab == 0)
			{

				if (SubTab("Main", ImVec2(150, 25), subtab == 0 ? true : false))
					subtab = 0;

				if (SubTab("Accuracy", ImVec2(150, 25), subtab == 1 ? true : false))
					subtab = 1;


				if (SubTab("Anti-Aim", ImVec2(150, 25), subtab == 2 ? true : false))
					subtab = 2;


				if (SubTab("Exploits", ImVec2(150, 25), subtab == 3 ? true : false))
					subtab = 3;

				if (SubTab("Player List", ImVec2(150, 25), subtab == 17 ? true : false))
					subtab = 17;

				if (SubTab("", ImVec2(150, 25), subtab == 99 ? true : false))
					subtab = 99;


			}
			if (tab == 1)
			{
				if (SubTab("Main", ImVec2(150, 25), subtab == 4 ? true : false))
					subtab = 4;

				if (SubTab("Chams", ImVec2(150, 25), subtab == 5 ? true : false))
					subtab = 5;
				
				//if (SubTab("Chams 2", ImVec2(150, 25), subtab == 17 ? true : false))
					//subtab = 17;

				if (SubTab("Other", ImVec2(150, 25), subtab == 6 ? true : false))
					subtab = 6;

				if (SubTab("Other Ext", ImVec2(150, 25), subtab == 7 ? true : false))
					subtab = 7;

			}
			if (tab == 2)
			{
				if (SubTab("Main", ImVec2(150, 25), subtab == 8 ? true : false))
					subtab = 8;

				if (SubTab("Other", ImVec2(150, 25), subtab == 9 ? true : false))
					subtab = 9;
			}
			if (tab == 3)
			{
				if (SubTab("Skin Changer", ImVec2(150, 25), subtab == 10 ? true : false))
					subtab = 10;

				//if (SubTab("Inventory Changer", ImVec2(150, 25), subtab == 18 ? true : false))
					//subtab = 18;

				if (SubTab("Profile Changer", ImVec2(150, 25), subtab == 19 ? true : false))
					subtab = 19;

				if (SubTab("Medal Changer", ImVec2(150, 25), subtab == 20 ? true : false))
					subtab = 20;
			}
			if (tab == 4)
			{
				if (SubTab("Menu", ImVec2(150, 25), subtab == 11 ? true : false))
					subtab = 11;

				if (SubTab("Config", ImVec2(150, 25), subtab == 12 ? true : false))
					subtab = 12;


			}
			if (tab == 5)
			{
				if (SubTab("Main", ImVec2(150, 25), subtab == 13 ? true : false))
					subtab = 13;

				if (SubTab("Weapons", ImVec2(150, 25), subtab == 14 ? true : false))
					subtab = 14;

				if (SubTab("Recoil", ImVec2(150, 25), subtab == 15 ? true : false))
					subtab = 15;

				if (SubTab("Triggerbot", ImVec2(150, 25), subtab == 16 ? true : false))
					subtab = 16;
			}
		}
		ImGui::EndGroup();


		static bool checkboxboo2 = false;
		static int sliderint = 254;
		static int comboint = 1;
		static float coloredit[3] = { 0.f,0.f,0.f };
		const char* rage_weapons[8] = { crypt_str("Revolver / Deagle"), crypt_str("Pistols"), crypt_str("SMGs"), crypt_str("Rifles"), crypt_str("Auto"), crypt_str("Scout"), crypt_str("AWP"), crypt_str("Heavy") };

		ImGui::SetCursorPos({ 170,60 });
		ImGui::BeginGroup();
		{
			ImGui::BeginChild("items", { 600, 400 });
			{
				draw->AddRectFilled(ImGui::GetWindowPos(), { ImGui::GetWindowPos().x + 600,ImGui::GetWindowPos().y + 400 }, ImColor(30, 35, 40, 200), 8.f);

				ImGui::PushFont(big);
				ImGui::SetCursorPos({ 10,10 });
				ImGui::BeginGroup();
				{
					if (tab == 0)
					{
						if (subtab == 0)
						{
							ImGui::Checkbox(crypt_str("Enable"), &g_cfg.ragebot.enable);

							if (g_cfg.ragebot.enable) {
								g_cfg.legitbot.enabled = false;

								ImGui::Spacing();



								ImGui::PushItemWidth(150);
								ImGui::SliderInt(crypt_str("Field of view"), &g_cfg.ragebot.field_of_view, 1, 180, false, crypt_str("%d°"));

								ImGui::Checkbox(crypt_str("Silent aim"), &g_cfg.ragebot.silent_aim);

								ImGui::Checkbox(crypt_str("Autowall"), &g_cfg.ragebot.autowall);

								ImGui::Checkbox(crypt_str("Zeusbot"), &g_cfg.ragebot.zeus_bot);

								ImGui::Checkbox(crypt_str("Knifebot"), &g_cfg.ragebot.knife_bot);

								ImGui::Checkbox(crypt_str("Auto-fire"), &g_cfg.ragebot.autoshoot);

								ImGui::Checkbox(crypt_str("Auto-scope"), &g_cfg.ragebot.autoscope);

								ImGui::Checkbox(crypt_str("Pitch correction"), &g_cfg.ragebot.pitch_antiaim_correction);

								///ImGui::Combo("Pitch", &g_cfg.antiaim.type[type].pitch, pitch, IM_ARRAYSIZE(pitch));


							}


						}
						if (subtab == 1)
						{
							draw_combo(crypt_str("Current weapon"), hooks::rage_weapon, rage_weapons, ARRAYSIZE(rage_weapons));
							ImGui::Spacing();

							draw_combo(crypt_str("Target selection"), g_cfg.ragebot.weapon[hooks::rage_weapon].selection_type, selection, ARRAYSIZE(selection));
							padding(0, 3);
							//draw_multicombo(crypt_str("Hitboxes"), g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes, hitboxes, ARRAYSIZE(hitboxes), preview);
							ImGui::Checkbox(crypt_str("Hitboxes"), &g_cfg.ragebot.hitbox1);
							if (g_cfg.ragebot.hitbox1)
							{


								auto cur_window = ImGui::GetCurrentWindow();
								ImVec2 w_pos = cur_window->Pos;
								ImGuiStyle* Style = &ImGui::GetStyle();

								Style->WindowRounding = 0;
								Style->WindowBorderSize = 1;
								Style->WindowMinSize = { 1,1 };
								bool theme = true;
								Style->Colors[ImGuiCol_WindowBg] = ImColor(19, 19, 19, 255);
								Style->Colors[ImGuiCol_Button] = ImColor(0, 0, 0, 0);
								Style->Colors[ImGuiCol_ButtonHovered] = ImColor(29, 125, 229, 0);
								Style->Colors[ImGuiCol_ButtonActive] = ImColor(29, 125, 229, 0);


								ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.f);
								ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
								ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

								ImGui::SetNextWindowSize({ 340, 570 });

								ImGui::Begin("##test", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
								{

									ImGui::Spacing();
									ImGui::PushFont(big);
									ImGui::SetCursorPos({ 140, 5 });
									ImGui::TextColored(ImColor(14, 176, 255), "Hitboxes");
									ImGui::PopFont();
									ImGui::SetCursorPos({ 0, 10 });
									ImGui::Image(CTmd1, { 340,555 });

									//head
									{
										ImGui::SetCursorPos({ 164, 50 });
										if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(0))
										{
											if (ImGui::ImageButton((void*)head, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(0) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(0);
										}
										else
										{
											if (ImGui::ImageButton((void*)head1, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(0) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(0);
										}
									}
									//body
									{
										ImGui::SetCursorPos({ 164, 110 });
										if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(1))
										{
											if (ImGui::ImageButton((void*)upchest, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(1) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(1);
											}
										}
										else
										{
											if (ImGui::ImageButton((void*)upchest1, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(1) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(1);

											}
										}

										ImGui::SetCursorPos({ 164, 140 });
										if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(2))
										{
											if (ImGui::ImageButton((void*)chest, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(2) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(2);

											}
										}
										else
										{
											if (ImGui::ImageButton((void*)chest1, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(2) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(2);

											}
										}
										ImGui::SetCursorPos({ 164, 180 });
										if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(3))
										{
											if (ImGui::ImageButton((void*)lowchest, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{

												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(3) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(3);

											}
										}
										else
										{
											if (ImGui::ImageButton((void*)lowchest1, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{

												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(3) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(3);

											}
										}
										ImGui::SetCursorPos({ 164, 215 });
										if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(4))
										{
											if (ImGui::ImageButton((void*)neck, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{

												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(4) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(4);

											}
										}
										else
										{
											if (ImGui::ImageButton((void*)neck1, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{

												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(4) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(4);

											}
										}
										ImGui::SetCursorPos({ 164, 280 });
										if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(5))
										{
											if (ImGui::ImageButton((void*)pel, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{

												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(5) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(5);
											}
										}
										else
										{
											if (ImGui::ImageButton((void*)pel1, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{

												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(5) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(5);
											}
										}
									}

									//arms
									{
										ImGui::SetCursorPos({ 65, 200 });
										if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(6))
										{
											//left
											if (ImGui::ImageButton((void*)arms, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(6) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(6);

											ImGui::SetCursorPos({ 265, 200 });
											if (ImGui::ImageButton((void*)arms2, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(6) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(6);
										}
										else
										{
											//right
											if (ImGui::ImageButton((void*)arms1, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(6) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(6);
											ImGui::SetCursorPos({ 265, 200 });
											if (ImGui::ImageButton((void*)arms3, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(6) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(6);
										}
									}
									//legs
									{
										ImGui::SetCursorPos({ 135, 330 });
										if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(7))
										{
											if (ImGui::ImageButton((void*)legs, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(7) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(7);

											}
											ImGui::SetCursorPos({ 195, 330 });
											if (ImGui::ImageButton((void*)legs2, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(7) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(7);

											}
										}
										else
										{
											if (ImGui::ImageButton((void*)legs1, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(7) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(7);

											}
											ImGui::SetCursorPos({ 195, 330 });
											if (ImGui::ImageButton((void*)legs3, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{
												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(7) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(7);

											}
										}

										//feet
										ImGui::SetCursorPos({ 135, 480 });
										if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(8))
										{
											if (ImGui::ImageButton((void*)feet, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{

												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(8) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(8);
											}
											ImGui::SetCursorPos({ 195, 480 });
											if (ImGui::ImageButton((void*)feet2, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{

												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(8) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(8);
											}
										}
										else
										{
											if (ImGui::ImageButton((void*)feet1, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{


												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(8) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(8);
											}
											ImGui::SetCursorPos({ 195, 480 });
											if (ImGui::ImageButton((void*)feet3, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0)))
											{

												g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(8) = !g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes.at(8);
											}
										}
									}

								}

								ImGui::End();
								ImGui::PopStyleVar(3);
							}

							ImGui::Checkbox(crypt_str("Multipoint"), &g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints);

							if (g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints)
							{
								draw_multicombo(crypt_str("Multipoint hitboxes"), g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints_hitboxes, hitboxes, ARRAYSIZE(hitboxes), preview);
								ImGui::SliderFloat(crypt_str("Multipoint scale"), &g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints_scale, 0.0f, 1.0f, g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints_scale ? crypt_str("%.2f") : crypt_str("None"));
								ImGui::SliderFloat(crypt_str("Stomach scale"), &g_cfg.ragebot.weapon[hooks::rage_weapon].stomach_scale, 0.0f, 1.0f, g_cfg.ragebot.weapon[hooks::rage_weapon].stomach_scale ? crypt_str("%.2f") : crypt_str("None"));
							}

							ImGui::Checkbox(crypt_str("Enable max misses"), &g_cfg.ragebot.weapon[hooks::rage_weapon].max_misses);

							if (g_cfg.ragebot.weapon[hooks::rage_weapon].max_misses)
								ImGui::SliderInt(crypt_str("Max misses"), &g_cfg.ragebot.weapon[hooks::rage_weapon].max_misses_amount, 0, 6);

							ImGui::Checkbox(crypt_str("Prefer safe points"), &g_cfg.ragebot.weapon[hooks::rage_weapon].prefer_safe_points);
							ImGui::Checkbox(crypt_str("Prefer body aim"), &g_cfg.ragebot.weapon[hooks::rage_weapon].prefer_body_aim);

							draw_keybind(crypt_str("Force safe points"), &g_cfg.ragebot.safe_point_key, crypt_str("##HOKEY_FORCE_SAFE_POINTS"));
							draw_keybind(crypt_str("Force body aim"), &g_cfg.ragebot.body_aim_key, crypt_str("##HOKEY_FORCE_BODY_AIM"));

							ImGui::Checkbox(crypt_str("Automatic stop"), &g_cfg.ragebot.weapon[hooks::rage_weapon].autostop);

							if (g_cfg.ragebot.weapon[hooks::rage_weapon].autostop)
								draw_multicombo(crypt_str("Modifiers"), g_cfg.ragebot.weapon[hooks::rage_weapon].autostop_modifiers, autostop_modifiers, ARRAYSIZE(autostop_modifiers), preview);

							ImGui::Checkbox(crypt_str("Hitchance"), &g_cfg.ragebot.weapon[hooks::rage_weapon].hitchance);

							if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitchance)
								ImGui::SliderInt(crypt_str("Hitchance amount"), &g_cfg.ragebot.weapon[hooks::rage_weapon].hitchance_amount, 1, 100);



							ImGui::SliderInt(crypt_str("Minimum damage"), &g_cfg.ragebot.weapon[hooks::rage_weapon].minimum_damage, 1, 120, true);

							draw_keybind(crypt_str("Damage override"), &g_cfg.ragebot.weapon[hooks::rage_weapon].damage_override_key, crypt_str("##HOTKEY__DAMAGE_OVERRIDE"));

							if (g_cfg.ragebot.weapon[hooks::rage_weapon].damage_override_key.key > KEY_NONE && g_cfg.ragebot.weapon[hooks::rage_weapon].damage_override_key.key < KEY_MAX)
								ImGui::SliderInt(crypt_str("Minimum override damage"), &g_cfg.ragebot.weapon[hooks::rage_weapon].minimum_override_damage, 1, 120, true);
						}
						if (subtab == 2)
						{
							ImGui::Checkbox(crypt_str("Enable"), &g_cfg.antiaim.enable);

							if (g_cfg.antiaim.enable) {

								draw_combo(crypt_str("Anti-aim type"), g_cfg.antiaim.antiaim_type, antiaim_type, ARRAYSIZE(antiaim_type));

								if (g_cfg.antiaim.antiaim_type)
								{
									padding(0, 3);
									(crypt_str("Desync"), g_cfg.antiaim.desync, desync, ARRAYSIZE(desync));

									if (g_cfg.antiaim.desync)
									{
										padding(0, 3);
										draw_combo(crypt_str("LBY type"), g_cfg.antiaim.legit_lby_type, lby_type, ARRAYSIZE(lby_type));

										if (g_cfg.antiaim.desync == 1)
										{
											draw_keybind(crypt_str("Invert desync"), &g_cfg.antiaim.flip_desync, crypt_str("##HOTKEY_INVERT_DESYNC"));


										}

									}
								}
								else
								{

									//draw_combo(crypt_str("Movement type"), type, movement_type, ARRAYSIZE(movement_type));
									//padding(0, 3);
									ImGui::Checkbox(crypt_str("Lag Sync"), &g_cfg.antiaim.lagsync);
									padding(0, 3);
									draw_combo(crypt_str("Pitch"), g_cfg.antiaim.type[type].pitch, pitch, ARRAYSIZE(pitch));
									padding(0, 3);
									draw_combo(crypt_str("Yaw"), g_cfg.antiaim.type[type].yaw, yaw, ARRAYSIZE(yaw));
									padding(0, 3);
									draw_combo(crypt_str("Base angle"), g_cfg.antiaim.type[type].base_angle, baseangle, ARRAYSIZE(baseangle));

									draw_combo(crypt_str("Desync"), g_cfg.antiaim.type[type].desync, desync, ARRAYSIZE(desync));

									if (g_cfg.antiaim.type[type].desync)
									{
										if (type == ANTIAIM_STAND)
										{
											padding(0, 3);
											draw_combo(crypt_str("LBY type"), g_cfg.antiaim.lby_type, lby_type, ARRAYSIZE(lby_type));
										}

										if (type != ANTIAIM_STAND || !g_cfg.antiaim.lby_type)
										{
											ImGui::SliderInt(crypt_str("Desync range"), &g_cfg.antiaim.type[type].desync_range, 1, 60);
											ImGui::SliderInt(crypt_str("Inverted desync range"), &g_cfg.antiaim.type[type].inverted_desync_range, 1, 60);
											ImGui::SliderInt(crypt_str("Body lean"), &g_cfg.antiaim.type[type].body_lean, 0, 100);
											ImGui::SliderInt(crypt_str("Inverted body lean"), &g_cfg.antiaim.type[type].inverted_body_lean, 0, 100);
										}

										if (g_cfg.antiaim.type[type].desync == 1)
										{
											draw_keybind(crypt_str("Invert desync"), &g_cfg.antiaim.flip_desync, crypt_str("##HOTKEY_INVERT_DESYNC"));
										}
									}
									ImGui::Checkbox(crypt_str("Side Arrow"), &g_cfg.antiaim.sidearrow);
									ImGui::SameLine();
									ImGui::ColorEdit(crypt_str("##invd"), &g_cfg.antiaim.invert_indicator_color, ALPHA);
									ImGui::Checkbox(crypt_str("Fakeduck"), &g_cfg.misc.noduck);
									if (g_cfg.misc.noduck) {
										draw_keybind(crypt_str("Fake duck"), &g_cfg.misc.fakeduck_key, crypt_str("##FAKEDUCK__HOTKEY"));
										ImGui::Checkbox(crypt_str("MM Fakeduck"), &g_cfg.ragebot.fakeduck_fix);
									}
									draw_keybind(crypt_str("Slow walk"), &g_cfg.misc.slowwalk_key, crypt_str("##SLOWWALK__HOTKEY"));
									
									draw_keybind(crypt_str("Freestanding"), &g_cfg.antiaim.freestanding_key, crypt_str("##HOTKEY_FS_KEY"));

									draw_keybind(crypt_str("Manual back"), &g_cfg.antiaim.manual_back, crypt_str("##HOTKEY_INVERT_BACK"));

									draw_keybind(crypt_str("Manual left"), &g_cfg.antiaim.manual_left, crypt_str("##HOTKEY_INVERT_LEFT"));

									draw_keybind(crypt_str("Manual right"), &g_cfg.antiaim.manual_right, crypt_str("##HOTKEY_INVERT_RIGHT"));

									if (g_cfg.antiaim.manual_back.key > KEY_NONE && g_cfg.antiaim.manual_back.key < KEY_MAX || g_cfg.antiaim.manual_left.key > KEY_NONE && g_cfg.antiaim.manual_left.key < KEY_MAX || g_cfg.antiaim.manual_right.key > KEY_NONE && g_cfg.antiaim.manual_right.key < KEY_MAX)
									{
										ImGui::Checkbox(crypt_str("Manuals indicator"), &g_cfg.antiaim.flip_indicator);
										ImGui::SameLine();
										ImGui::ColorEdit(crypt_str("##invc"), &g_cfg.antiaim.flip_indicator_color, ALPHA);
									}

									ImGui::Checkbox(crypt_str("Fake lag"), &g_cfg.antiaim.fakelag);
									if (g_cfg.antiaim.fakelag)
									{
										draw_combo(crypt_str("Fake lag type"), g_cfg.antiaim.fakelag_type, fakelags, ARRAYSIZE(fakelags));
										ImGui::SliderInt(crypt_str("Limit"), &g_cfg.antiaim.fakelag_amount, 1, 16);

										draw_multicombo(crypt_str("Fake lag triggers"), g_cfg.antiaim.fakelag_enablers, lagstrigger, ARRAYSIZE(lagstrigger), preview);

										auto enabled_fakelag_triggers = false;

										for (auto i = 0; i < ARRAYSIZE(lagstrigger); i++)
											if (g_cfg.antiaim.fakelag_enablers[i])
												enabled_fakelag_triggers = true;

										if (enabled_fakelag_triggers)
											ImGui::SliderInt(crypt_str("Triggers limit"), &g_cfg.antiaim.triggers_fakelag_amount, 1, 16);
									}

								}
							}
						}
						if (subtab == 3)
						{
							ImGui::Checkbox(crypt_str("Double tap"), &g_cfg.ragebot.double_tap);
							if (g_cfg.ragebot.double_tap)
							{
								ImGui::SameLine();
								draw_keybind(crypt_str(""), &g_cfg.ragebot.double_tap_key, crypt_str("##HOTKEY_DT"));
								ImGui::Checkbox(crypt_str("Teleport DT"), &g_cfg.ragebot.slow_teleport);
							}
							if (g_cfg.ragebot.double_tap && hooks::rage_weapon <= 4)
							{
								ImGui::Checkbox(crypt_str("Double tap hitchance"), &g_cfg.ragebot.weapon[hooks::rage_weapon].double_tap_hitchance);

								if (g_cfg.ragebot.weapon[hooks::rage_weapon].double_tap_hitchance)
									ImGui::SliderInt(crypt_str("Double tap hitchance amount"), &g_cfg.ragebot.weapon[hooks::rage_weapon].double_tap_hitchance_amount, 1, 100);
							}
							ImGui::SliderInt(crypt_str("DoubleTap Speed"), &g_cfg.ragebot.weapon[hooks::rage_weapon].double_tap_shift_value, 8, 18);
							ImGui::Spacing();
							ImGui::Checkbox(crypt_str("Hide shots"), &g_cfg.antiaim.hide_shots);

							if (g_cfg.antiaim.hide_shots)
							{
								ImGui::SameLine();
								draw_keybind(crypt_str(""), &g_cfg.antiaim.hide_shots_key, crypt_str("##HOTKEY_HIDESHOTS"));
							}

						}
						if (subtab == 17)
						{

							auto player = players_section;

							static std::vector <Player_list_data> players;

							if (!g_cfg.player_list.refreshing)
							{
								players.clear();

								for (auto player : g_cfg.player_list.players)
									players.emplace_back(player);
							}

							static auto current_player = 0;
							ImGui::Columns(2, nullptr, false);
							child_title(crypt_str("Players"));
							ImGui::BeginChild("it3ems", { 300, 450 });
							{

								ImGui::SetCursorPos({ 1,1 });
								ImGui::BeginGroup();
								{
									if (!players.empty())
									{
										std::vector <std::string> player_names;

										for (auto player : players)
											player_names.emplace_back(player.name);

										ImGui::PushItemWidth(291 * dpi_scale);
										ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
										ImGui::ListBoxConfigArray(crypt_str("##PLAYERLIST"), &current_player, player_names, 14);
										ImGui::PopStyleVar();
										ImGui::PopItemWidth();
									}
								}
								ImGui::EndGroup();
							}
							ImGui::EndChild();
							ImGui::NextColumn();
							child_title(crypt_str("Settings"));
							ImGui::BeginChild("item6s2", { 300, 450 });
							{

								ImGui::SetCursorPos({ 10,10 });
								ImGui::BeginGroup();
								{

									if (!players.empty())
									{
										if (current_player >= players.size())
											current_player = players.size() - 1; //-V103

										ImGui::Checkbox(crypt_str("White list"), &g_cfg.player_list.white_list[players.at(current_player).i]); //-V106 //-V807

										if (!g_cfg.player_list.white_list[players.at(current_player).i]) //-V106
										{
											ImGui::Checkbox(crypt_str("High priority"), &g_cfg.player_list.high_priority[players.at(current_player).i]); //-V106
											ImGui::Checkbox(crypt_str("Force safe points"), &g_cfg.player_list.force_safe_points[players.at(current_player).i]); //-V106
											ImGui::Checkbox(crypt_str("Force body aim"), &g_cfg.player_list.force_body_aim[players.at(current_player).i]); //-V106
											ImGui::Checkbox(crypt_str("Low delta"), &g_cfg.player_list.low_delta[players.at(current_player).i]); //-V106
										}
									}



								}
								ImGui::EndGroup();
							}
							ImGui::EndChild();
						}

#include <fstream>
#include <shellapi.h>
#include <windows.h>
#include <iostream>
#include <string>

						if (subtab == 99)

						{
							padding(0, 6);

							if (ImGui::CustomButton("Join The Discord", "##fucksityeah", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
							{
								ShellExecute(0, 0, "https://discord.gg/M2yVtymqFk", 0, 0, SW_SHOW);
							}

							ImGui::Spacing();

							if (ImGui::CustomButton("Subscribe to my Youtube", "##fucksityeah1", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
							{
								ShellExecute(0, 0, "https://www.youtube.com/channel/UC-w2td9xyAymU-6yoiWGFdg", 0, 0, SW_SHOW);
							}

						}
					}
					if (tab == 5)
					{
						if (subtab == 13)
						{
							padding(0, 6);
							ImGui::Checkbox(crypt_str("Enabled"), &g_cfg.legitbot.enabled);
							ImGui::SameLine();
							draw_keybind(crypt_str(""), &g_cfg.legitbot.key, crypt_str("##HOTKEY_LGBT_KEY"));

							if (g_cfg.legitbot.enabled)
								g_cfg.ragebot.enable = false;

							ImGui::Checkbox(crypt_str("Friendly fire"), &g_cfg.legitbot.friendly_fire);
							ImGui::Checkbox(crypt_str("Automatic pistols"), &g_cfg.legitbot.autopistol);

							ImGui::Checkbox(crypt_str("Automatic scope"), &g_cfg.legitbot.autoscope);

							if (g_cfg.legitbot.autoscope)
								ImGui::Checkbox(crypt_str("Unscope after shot"), &g_cfg.legitbot.unscope);

							ImGui::Checkbox(crypt_str("Snipers in zoom only"), &g_cfg.legitbot.sniper_in_zoom_only);

							ImGui::Checkbox(crypt_str("Aim if in air"), &g_cfg.legitbot.do_if_local_in_air);
							ImGui::Checkbox(crypt_str("Aim if flashed"), &g_cfg.legitbot.do_if_local_flashed);
							ImGui::Checkbox(crypt_str("Aim thru smoke"), &g_cfg.legitbot.do_if_enemy_in_smoke);
						}

						if (subtab == 14)
						{
							const char* legit_weapons[6] = { crypt_str("Deagle"), crypt_str("Pistols"), crypt_str("Rifles"), crypt_str("SMGs"), crypt_str("Snipers"), crypt_str("Heavy") };
							const char* hitbox_legit[3] = { crypt_str("Nearest"), crypt_str("Head"), crypt_str("Body") };

							draw_combo(crypt_str("Current weapon"), hooks::legit_weapon, legit_weapons, ARRAYSIZE(legit_weapons));
							ImGui::Spacing();
							draw_combo(crypt_str("Hitbox"), g_cfg.legitbot.weapon[hooks::legit_weapon].priority, hitbox_legit, ARRAYSIZE(hitbox_legit));

							ImGui::Checkbox(crypt_str("Automatic stop"), &g_cfg.legitbot.weapon[hooks::legit_weapon].auto_stop);

							draw_combo(crypt_str("Maximum FOV type"), g_cfg.legitbot.weapon[hooks::legit_weapon].fov_type, LegitFov, ARRAYSIZE(LegitFov));
							ImGui::SliderFloat(crypt_str("Maximum FOV amount"), &g_cfg.legitbot.weapon[hooks::legit_weapon].fov, 0.f, 30.f, crypt_str("%.2f"));

							ImGui::Spacing();

							ImGui::SliderFloat(crypt_str("Silent FOV"), &g_cfg.legitbot.weapon[hooks::legit_weapon].silent_fov, 0.f, 30.f, (!g_cfg.legitbot.weapon[hooks::legit_weapon].silent_fov ? crypt_str("None") : crypt_str("%.2f"))); //-V550

							ImGui::Spacing();

							draw_combo(crypt_str("Smooth type"), g_cfg.legitbot.weapon[hooks::legit_weapon].smooth_type, LegitSmooth, ARRAYSIZE(LegitSmooth));
							ImGui::SliderFloat(crypt_str("Smooth amount"), &g_cfg.legitbot.weapon[hooks::legit_weapon].smooth, 1.f, 12.f, crypt_str("%.1f"));

							ImGui::Spacing();

							ImGui::SliderFloat(crypt_str("Backtrack ticks"), &g_cfg.legitbot.weapon[hooks::legit_weapon].backtrack_ticks, 0, 12);
							ImGui::SliderFloat(crypt_str("Target switch delay"), &g_cfg.legitbot.weapon[hooks::legit_weapon].target_switch_delay, 0.f, 1.f);



						}

						if (subtab == 15)
						{
							const char* legit_weapons[6] = { crypt_str("Deagle"), crypt_str("Pistols"), crypt_str("Rifles"), crypt_str("SMGs"), crypt_str("Snipers"), crypt_str("Heavy") };
							const char* hitbox_legit[3] = { crypt_str("Nearest"), crypt_str("Head"), crypt_str("Body") };


							draw_combo(crypt_str("Current weapon"), hooks::legit_weapon, legit_weapons, ARRAYSIZE(legit_weapons));

							draw_combo(crypt_str("RCS type"), g_cfg.legitbot.weapon[hooks::legit_weapon].rcs_type, RCSType, ARRAYSIZE(RCSType));
							ImGui::SliderFloat(crypt_str("RCS amount"), &g_cfg.legitbot.weapon[hooks::legit_weapon].rcs, 0.f, 100.f, crypt_str("%.0f%%"), 1.f);

							if (g_cfg.legitbot.weapon[hooks::legit_weapon].rcs > 0)
							{
								ImGui::SliderFloat(crypt_str("RCS custom FOV"), &g_cfg.legitbot.weapon[hooks::legit_weapon].custom_rcs_fov, 0.f, 30.f, (!g_cfg.legitbot.weapon[hooks::legit_weapon].custom_rcs_fov ? crypt_str("None") : crypt_str("%.2f"))); //-V550
								ImGui::SliderFloat(crypt_str("RCS Custom smooth"), &g_cfg.legitbot.weapon[hooks::legit_weapon].custom_rcs_smooth, 0.f, 12.f, (!g_cfg.legitbot.weapon[hooks::legit_weapon].custom_rcs_smooth ? crypt_str("None") : crypt_str("%.1f"))); //-V550
							}

						}

						if (subtab == 16)

						{
							const char* legit_weapons[6] = { crypt_str("Deagle"), crypt_str("Pistols"), crypt_str("Rifles"), crypt_str("SMGs"), crypt_str("Snipers"), crypt_str("Heavy") };
							const char* hitbox_legit[3] = { crypt_str("Nearest"), crypt_str("Head"), crypt_str("Body") };


							draw_keybind(crypt_str("Automatic fire key"), &g_cfg.legitbot.autofire_key, crypt_str("##HOTKEY_AUTOFIRE_LGBT_KEY"));
							ImGui::SliderInt(crypt_str("Automatic fire delay"), &g_cfg.legitbot.autofire_delay, 0, 12, false, (!g_cfg.legitbot.autofire_delay ? crypt_str("None") : (g_cfg.legitbot.autofire_delay == 1 ? crypt_str("%d tick") : crypt_str("%d ticks"))));

							ImGui::SliderInt(crypt_str("Automatic wall damage"), &g_cfg.legitbot.weapon[hooks::legit_weapon].awall_dmg, 0, 100, false, (!g_cfg.legitbot.weapon[hooks::legit_weapon].awall_dmg ? crypt_str("None") : crypt_str("%d")));
							ImGui::SliderInt(crypt_str("Automatic fire hitchance"), &g_cfg.legitbot.weapon[hooks::legit_weapon].autofire_hitchance, 0, 100, false, (!g_cfg.legitbot.weapon[hooks::legit_weapon].autofire_hitchance ? crypt_str("None") : crypt_str("%d")));
						}

					}

					if (tab == 1)
					{
						
						if (subtab == 4)
						{
							ImGui::Checkbox(crypt_str("Enable"), &g_cfg.player.enable);

							if (g_cfg.player.enable) {

								draw_keybind(crypt_str("Thirdperson"), &g_cfg.misc.thirdperson_toggle, crypt_str("##TPKEY__HOTKEY"));

								ImGui::Checkbox(crypt_str("Thirdperson when spectating"), &g_cfg.misc.thirdperson_when_spectating);

								if (g_cfg.misc.thirdperson_toggle.key > KEY_NONE && g_cfg.misc.thirdperson_toggle.key < KEY_MAX)
									ImGui::SliderInt(crypt_str("Thirdperson distance"), &g_cfg.misc.thirdperson_distance, 50, 300, false, crypt_str("%d°"));

								ImGui::SliderInt(crypt_str("Field of view"), &g_cfg.esp.fov, 0, 89, false, crypt_str("%d°"));

								ImGui::Checkbox(crypt_str("Preserve killfeed"), &g_cfg.esp.preserve_killfeed);

								ImGui::Checkbox(crypt_str("Arms chams"), &g_cfg.esp.arms_chams);
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##armscolor"), &g_cfg.esp.arms_chams_color, ALPHA);


								draw_combo(crypt_str("Arms chams material"), g_cfg.esp.arms_chams_type, chamstype, ARRAYSIZE(chamstype));

								if (g_cfg.esp.arms_chams_type != 6)
								{
									ImGui::Checkbox(crypt_str("Arms double material "), &g_cfg.esp.arms_double_material);
									ImGui::SameLine();
									ImGui::ColorEdit(crypt_str("##armsdoublematerial"), &g_cfg.esp.arms_double_material_color, ALPHA);
								}

								ImGui::Checkbox(crypt_str("Arms animated material "), &g_cfg.esp.arms_animated_material);
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##armsanimatedmaterial"), &g_cfg.esp.arms_animated_material_color, ALPHA);

								ImGui::Spacing();

								ImGui::Checkbox(crypt_str("Weapon chams"), &g_cfg.esp.weapon_chams);
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##weaponchamscolors"), &g_cfg.esp.weapon_chams_color, ALPHA);

								draw_combo(crypt_str("Weapon chams material"), g_cfg.esp.weapon_chams_type, chamstype, ARRAYSIZE(chamstype));

								if (g_cfg.esp.weapon_chams_type != 6)
								{
									ImGui::Checkbox(crypt_str("Double material "), &g_cfg.esp.weapon_double_material);
									ImGui::SameLine();
									ImGui::ColorEdit(crypt_str("##weapondoublematerial"), &g_cfg.esp.weapon_double_material_color, ALPHA);
								}

								ImGui::Checkbox(crypt_str("Animated material "), &g_cfg.esp.weapon_animated_material);
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##weaponanimatedmaterial"), &g_cfg.esp.weapon_animated_material_color, ALPHA);


							}







						}




						if (subtab == 5)
						{
							static const char* ts[] = { "Enemy", "Team", "Local" };
							padding(0, 3);
							draw_combo(crypt_str("Player Type"), players_section, ts, ARRAYSIZE(ts));

							if (player == LOCAL)
								draw_combo(crypt_str("Type"), g_cfg.player.local_chams_type, local_chams_type, ARRAYSIZE(local_chams_type));

							if (player != LOCAL || !g_cfg.player.local_chams_type)
								draw_multicombo(crypt_str("Chams"), g_cfg.player.type[player].chams, g_cfg.player.type[player].chams[PLAYER_CHAMS_VISIBLE] ? chamsvisact : chamsvis, g_cfg.player.type[player].chams[PLAYER_CHAMS_VISIBLE] ? ARRAYSIZE(chamsvisact) : ARRAYSIZE(chamsvis), preview);

							if (g_cfg.player.type[player].chams[PLAYER_CHAMS_VISIBLE] || player == LOCAL && g_cfg.player.local_chams_type)
							{
								if (player == LOCAL && g_cfg.player.local_chams_type)
								{
									ImGui::Checkbox(crypt_str("Enable desync chams"), &g_cfg.player.fake_chams_enable);
									ImGui::Checkbox(crypt_str("Visualize lag"), &g_cfg.player.visualize_lag);
									ImGui::Checkbox(crypt_str("Layered"), &g_cfg.player.layered);

									draw_combo(crypt_str("Player chams material"), g_cfg.player.fake_chams_type, chamstype, ARRAYSIZE(chamstype));

									ImGui::Text(crypt_str("Color "));
									ImGui::SameLine();
									ImGui::ColorEdit(crypt_str("##fakechamscolor"), &g_cfg.player.fake_chams_color, ALPHA);

									if (g_cfg.player.fake_chams_type != 6)
									{
										ImGui::Checkbox(crypt_str("Double material "), &g_cfg.player.fake_double_material);
										ImGui::SameLine();
										ImGui::ColorEdit(crypt_str("##doublematerialcolor"), &g_cfg.player.fake_double_material_color, ALPHA);
									}

									ImGui::Checkbox(crypt_str("Animated material"), &g_cfg.player.fake_animated_material);
									ImGui::SameLine();
									ImGui::ColorEdit(crypt_str("##animcolormat"), &g_cfg.player.fake_animated_material_color, ALPHA);
								}
								else
								{
									draw_combo(crypt_str("Player chams material"), g_cfg.player.type[player].chams_type, chamstype, ARRAYSIZE(chamstype));

									if (g_cfg.player.type[player].chams[PLAYER_CHAMS_VISIBLE])
									{
										ImGui::Text(crypt_str("Visible "));
										ImGui::SameLine();
										ImGui::ColorEdit(crypt_str("##chamsvisible"), &g_cfg.player.type[player].chams_color, ALPHA);
									}

									if (g_cfg.player.type[player].chams[PLAYER_CHAMS_VISIBLE] && g_cfg.player.type[player].chams[PLAYER_CHAMS_INVISIBLE])
									{
										ImGui::Text(crypt_str("Invisible "));
										ImGui::SameLine();
										ImGui::ColorEdit(crypt_str("##chamsinvisible"), &g_cfg.player.type[player].xqz_color, ALPHA);
									}

									if (g_cfg.player.type[player].chams_type != 6)
									{
										ImGui::Checkbox(crypt_str("Double material "), &g_cfg.player.type[player].double_material);
										ImGui::SameLine();
										ImGui::ColorEdit(crypt_str("##doublematerialcolor"), &g_cfg.player.type[player].double_material_color, ALPHA);
									}

									ImGui::Checkbox(crypt_str("Animated material"), &g_cfg.player.type[player].animated_material);
									ImGui::SameLine();
									ImGui::ColorEdit(crypt_str("##animcolormat"), &g_cfg.player.type[player].animated_material_color, ALPHA);

									if (player == ENEMY)
									{
										ImGui::Checkbox(crypt_str("Backtrack chams"), &g_cfg.player.backtrack_chams);

										if (g_cfg.player.backtrack_chams)
										{
											draw_combo(crypt_str("Backtrack chams material"), g_cfg.player.backtrack_chams_material, chamstype, ARRAYSIZE(chamstype));

											ImGui::Text(crypt_str("Color "));
											ImGui::SameLine();
											ImGui::ColorEdit(crypt_str("##backtrackcolor"), &g_cfg.player.backtrack_chams_color, ALPHA);
										}
									}
								}
							}


							ImGui::Checkbox(crypt_str("Ragdoll chams"), &g_cfg.player.type[player].ragdoll_chams);

							if (g_cfg.player.type[player].ragdoll_chams)
							{
								draw_combo(crypt_str("Ragdoll chams material"), g_cfg.player.type[player].ragdoll_chams_material, chamstype, ARRAYSIZE(chamstype));

								ImGui::Text(crypt_str("Color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##ragdollcolor"), &g_cfg.player.type[player].ragdoll_chams_color, ALPHA);
							}

							else if (!g_cfg.player.local_chams_type)
							{
								ImGui::Checkbox(crypt_str("Transparency in scope"), &g_cfg.player.transparency_in_scope);

								if (g_cfg.player.transparency_in_scope)
									ImGui::SliderFloat(crypt_str("Alpha"), &g_cfg.player.transparency_in_scope_amount, 0.0f, 1.0f);
							}

							ImGui::Spacing();

							ImGui::Checkbox(crypt_str("Glow"), &g_cfg.player.type[player].glow);

							if (g_cfg.player.type[player].glow)
							{
								draw_combo(crypt_str("Glow type"), g_cfg.player.type[player].glow_type, glowtype, ARRAYSIZE(glowtype));
								ImGui::Text(crypt_str("Color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##glowcolor"), &g_cfg.player.type[player].glow_color, ALPHA);
							}

							ImGui::Checkbox(crypt_str("DLight"), &g_cfg.player.type[player].dLight);
							ImGui::SameLine();
							ImGui::ColorEdit("##dlight_color", &g_cfg.player.type[player].dLight_color, ALPHA);

							if (g_cfg.player.type[player].dLight)
							{
								ImGui::SliderFloat("Dlight radius", &g_cfg.player.type[player].dLightR, 0, 1000);
								ImGui::SliderFloat("Dlight decay", &g_cfg.player.type[player].dLightDecay, 0, 400);
								ImGui::SliderInt("Dlight style", &g_cfg.player.type[player].dLightStyle, 0, 10);
								ImGui::SliderInt("Dlight expoent", &g_cfg.player.type[player].dLightExpoent, 0, 5);

							}


							ImGui::Checkbox(crypt_str("OOF arrows"), &g_cfg.player.arrows);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##arrowscolor"), &g_cfg.player.arrows_color, ALPHA);

							if (g_cfg.player.arrows)
							{
								ImGui::SliderInt(crypt_str("Arrows distance"), &g_cfg.player.distance, 1, 100);
								ImGui::SliderInt(crypt_str("Arrows size"), &g_cfg.player.size, 1, 100);
							}


							ImGui::Checkbox(crypt_str("Bounding box"), &g_cfg.player.type[player].box);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##boxcolor"), &g_cfg.player.type[player].box_color, ALPHA);

							ImGui::Checkbox(crypt_str("Name"), &g_cfg.player.type[player].name);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##namecolor"), &g_cfg.player.type[player].name_color, ALPHA);

							ImGui::Checkbox(crypt_str("Health bar"), &g_cfg.player.type[player].health);
							ImGui::Checkbox(crypt_str("Health color"), &g_cfg.player.type[player].custom_health_color);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##healthcolor"), &g_cfg.player.type[player].health_color, ALPHA);

							for (auto i = 0, j = 0; i < ARRAYSIZE(flags); i++)
							{
								if (g_cfg.player.type[player].flags[i])
								{
									if (j)
										preview += crypt_str(", ") + (std::string)flags[i];
									else
										preview = flags[i];

									j++;
								}
							}

							draw_multicombo(crypt_str("Flags"), g_cfg.player.type[player].flags, flags, ARRAYSIZE(flags), preview);
							draw_multicombo(crypt_str("Weapon"), g_cfg.player.type[player].weapon, weaponplayer, ARRAYSIZE(weaponplayer), preview);


							if (g_cfg.player.type[player].weapon[WEAPON_ICON] || g_cfg.player.type[player].weapon[WEAPON_TEXT])
							{
								ImGui::Text(crypt_str("Color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##weapcolor"), &g_cfg.player.type[player].weapon_color, ALPHA);
							}

							ImGui::Checkbox(crypt_str("Skeleton"), &g_cfg.player.type[player].skeleton);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##skeletoncolor"), &g_cfg.player.type[player].skeleton_color, ALPHA);

							ImGui::Checkbox(crypt_str("Ammo bar"), &g_cfg.player.type[player].ammo);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##ammocolor"), &g_cfg.player.type[player].ammobar_color, ALPHA);

							ImGui::Checkbox(crypt_str("Footsteps"), &g_cfg.player.type[player].footsteps);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##footstepscolor"), &g_cfg.player.type[player].footsteps_color, ALPHA);

							if (g_cfg.player.type[player].footsteps)
							{
								ImGui::SliderInt(crypt_str("Thickness"), &g_cfg.player.type[player].thickness, 1, 10);
								ImGui::SliderInt(crypt_str("Radius"), &g_cfg.player.type[player].radius, 50, 500);
							}



							ImGui::Checkbox(crypt_str("Snap lines"), &g_cfg.player.type[player].snap_lines);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##snapcolor"), &g_cfg.player.type[player].snap_lines_color, ALPHA);



							if (g_cfg.ragebot.enable)
							{
								ImGui::Checkbox(crypt_str("Aimbot points"), &g_cfg.player.show_multi_points);
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##showmultipointscolor"), &g_cfg.player.show_multi_points_color, ALPHA);
							}

							ImGui::Checkbox(crypt_str("Aimbot hitboxes"), &g_cfg.player.lag_hitbox);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##lagcompcolor"), &g_cfg.player.lag_hitbox_color, ALPHA);



							draw_combo(crypt_str("Player model T"), g_cfg.player.player_model_t, player_model_t, ARRAYSIZE(player_model_t));
							padding(0, 3);
							draw_combo(crypt_str("Player model CT"), g_cfg.player.player_model_ct, player_model_ct, ARRAYSIZE(player_model_ct));
							ImGui::Checkbox(crypt_str("Local trails"), &g_cfg.esp.trails);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##Localtrailscolor"), &g_cfg.esp.trails_color, ALPHA);

						}







						if (subtab == 6)
						{
							draw_multicombo(crypt_str("Indicators"), g_cfg.esp.indicators, indicators, ARRAYSIZE(indicators), preview);
							padding(0, 3);

							draw_multicombo(crypt_str("Removals"), g_cfg.esp.removals, removals, ARRAYSIZE(removals), preview);

							if (g_cfg.esp.removals[REMOVALS_ZOOM])
								ImGui::Checkbox(crypt_str("Fix zoom sensivity"), &g_cfg.esp.fix_zoom_sensivity);

							ImGui::Checkbox(crypt_str("Grenade prediction"), &g_cfg.esp.grenade_prediction);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##grenpredcolor"), &g_cfg.esp.grenade_prediction_color, ALPHA);

							if (g_cfg.esp.grenade_prediction)
							{
								ImGui::Checkbox(crypt_str("On click"), &g_cfg.esp.on_click);
								ImGui::Text(crypt_str("Tracer color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##tracergrenpredcolor"), &g_cfg.esp.grenade_prediction_tracer_color, ALPHA);
							}

							ImGui::Checkbox(crypt_str("Grenade projectiles"), &g_cfg.esp.projectiles);

							if (g_cfg.esp.projectiles)
								draw_multicombo(crypt_str("Grenade ESP"), g_cfg.esp.grenade_esp, proj_combo, ARRAYSIZE(proj_combo), preview);

							if (g_cfg.esp.grenade_esp[GRENADE_ICON] || g_cfg.esp.grenade_esp[GRENADE_TEXT])
							{
								ImGui::Text(crypt_str("Color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##projectcolor"), &g_cfg.esp.projectiles_color, ALPHA);
							}

							if (g_cfg.esp.grenade_esp[GRENADE_BOX])
							{
								ImGui::Text(crypt_str("Box color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##grenade_box_color"), &g_cfg.esp.grenade_box_color, ALPHA);
							}

							if (g_cfg.esp.grenade_esp[GRENADE_GLOW])
							{
								ImGui::Text(crypt_str("Glow color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##grenade_glow_color"), &g_cfg.esp.grenade_glow_color, ALPHA);
							}

							ImGui::Checkbox(crypt_str("Fire timer"), &g_cfg.esp.molotov_timer);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##molotovcolor"), &g_cfg.esp.molotov_timer_color, ALPHA);

							ImGui::Checkbox(crypt_str("Smoke timer"), &g_cfg.esp.smoke_timer);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##smokecolor"), &g_cfg.esp.smoke_timer_color, ALPHA);

							ImGui::Checkbox(crypt_str("Bomb indicator"), &g_cfg.esp.bomb_timer);
							draw_multicombo(crypt_str("Weapon ESP"), g_cfg.esp.weapon, weaponesp, ARRAYSIZE(weaponesp), preview);

							if (g_cfg.esp.weapon[WEAPON_ICON] || g_cfg.esp.weapon[WEAPON_TEXT] || g_cfg.esp.weapon[WEAPON_DISTANCE])
							{
								ImGui::Text(crypt_str("Color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##weaponcolor"), &g_cfg.esp.weapon_color, ALPHA);
							}

							ImGui::Checkbox(crypt_str("Rain"), &g_cfg.esp.rain);
							ImGui::Checkbox(crypt_str("Full bright"), &g_cfg.esp.bright);

							draw_combo(crypt_str("Skybox"), g_cfg.esp.skybox, skybox, ARRAYSIZE(skybox));

							ImGui::Text(crypt_str("Color "));
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##skyboxcolor"), &g_cfg.esp.skybox_color, NOALPHA);

							if (g_cfg.esp.skybox == 21)
							{
								static char sky_custom[64] = "\0";

								if (!g_cfg.esp.custom_skybox.empty())
									strcpy_s(sky_custom, sizeof(sky_custom), g_cfg.esp.custom_skybox.c_str());

								ImGui::Text(crypt_str("Name"));
								ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

								if (ImGui::InputText(crypt_str("##customsky"), sky_custom, sizeof(sky_custom)))
									g_cfg.esp.custom_skybox = sky_custom;

								ImGui::PopStyleVar();
							}

							ImGui::Checkbox(crypt_str("Color modulation"), &g_cfg.esp.nightmode);

							if (g_cfg.esp.nightmode)
							{
								ImGui::Text(crypt_str("World color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##worldcolor"), &g_cfg.esp.world_color, ALPHA);

								ImGui::Text(crypt_str("Props color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##propscolor"), &g_cfg.esp.props_color, ALPHA);
							}

							ImGui::Checkbox(crypt_str("World modulation"), &g_cfg.esp.world_modulation);

							if (g_cfg.esp.world_modulation)
							{
								ImGui::SliderFloat(crypt_str("Bloom"), &g_cfg.esp.bloom, 0.0f, 750.0f);
								ImGui::SliderFloat(crypt_str("Exposure"), &g_cfg.esp.exposure, 0.0f, 2000.0f);
								ImGui::SliderFloat(crypt_str("Ambient"), &g_cfg.esp.ambient, 0.0f, 1500.0f);
							}

							ImGui::Checkbox(crypt_str("Fog modulation"), &g_cfg.esp.fog);

							if (g_cfg.esp.fog)
							{
								ImGui::SliderInt(crypt_str("Distance"), &g_cfg.esp.fog_distance, 0, 2500);
								ImGui::SliderInt(crypt_str("Density"), &g_cfg.esp.fog_density, 0, 100);

								ImGui::Text(crypt_str("Color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##fogcolor"), &g_cfg.esp.fog_color, NOALPHA);
							}
						}






						if (subtab == 7)
						{
							ImGui::Checkbox(crypt_str("Client bullet impacts"), &g_cfg.esp.client_bullet_impacts);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##clientbulletimpacts"), &g_cfg.esp.client_bullet_impacts_color, ALPHA);

							ImGui::Checkbox(crypt_str("Server bullet impacts"), &g_cfg.esp.server_bullet_impacts);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##serverbulletimpacts"), &g_cfg.esp.server_bullet_impacts_color, ALPHA);

							ImGui::Checkbox(crypt_str("Local bullet tracers"), &g_cfg.esp.bullet_tracer);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##bulltracecolor"), &g_cfg.esp.bullet_tracer_color, ALPHA);

							ImGui::Checkbox(crypt_str("Enemy bullet tracers"), &g_cfg.esp.enemy_bullet_tracer);
							ImGui::SameLine();

							ImGui::ColorEdit(crypt_str("##enemybulltracecolor"), &g_cfg.esp.enemy_bullet_tracer_color, ALPHA);
							draw_multicombo(crypt_str("Hit marker"), g_cfg.esp.hitmarker, hitmarkers, ARRAYSIZE(hitmarkers), preview);
							ImGui::Checkbox(crypt_str("Damage marker"), &g_cfg.esp.damage_marker);
							ImGui::Checkbox(crypt_str("Kill effect"), &g_cfg.esp.kill_effect);
							//ImGui::Checkbox(crypt_str("Kill effect v2"), &g_cfg.misc.lightingonshot);

							if (g_cfg.esp.kill_effect)
								ImGui::SliderFloat(crypt_str("Duration"), &g_cfg.esp.kill_effect_duration, 0.01f, 3.0f);

							ImGui::Checkbox(crypt_str("Taser range"), &g_cfg.esp.taser_range);
							ImGui::Checkbox(crypt_str("Show Sniper Crosshair"), &g_cfg.misc.misc_sniper_crosshair);
							ImGui::Checkbox(crypt_str("Show spread"), &g_cfg.esp.show_spread);
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##spredcolor"), &g_cfg.esp.show_spread_color, ALPHA);
							ImGui::Checkbox(crypt_str("Penetration crosshair"), &g_cfg.esp.penetration_reticle);

							ImGui::Checkbox(crypt_str("Rare animations"), &g_cfg.skins.rare_animations);
							ImGui::SliderInt(crypt_str("Viewmodel field of view"), &g_cfg.esp.viewmodel_fov, 0, 89);
							ImGui::SliderInt(crypt_str("Viewmodel X"), &g_cfg.esp.viewmodel_x, -50, 50);
							ImGui::SliderInt(crypt_str("Viewmodel Y"), &g_cfg.esp.viewmodel_y, -50, 50);
							ImGui::SliderInt(crypt_str("Viewmodel Z"), &g_cfg.esp.viewmodel_z, -50, 50);
							ImGui::SliderInt(crypt_str("Viewmodel roll"), &g_cfg.esp.viewmodel_roll, -180, 180);

							ImGui::Checkbox(crypt_str("Aspect ratio"), &g_cfg.misc.aspect_ratio);

							if (g_cfg.misc.aspect_ratio)
							{
								padding(0, -5);
								ImGui::SliderFloat(crypt_str("Amount"), &g_cfg.misc.aspect_ratio_amount, 1.0f, 2.0f);
							}

						}


					}

					if (tab == 2)
					{
						if (subtab == 8)
						{


							ImGui::Checkbox(crypt_str("Watermark"), &g_cfg.menu.watermark);
							ImGui::Checkbox(crypt_str("Spectators list"), &g_cfg.misc.spectators_list);
							ImGui::Checkbox(crypt_str("Automatic jump"), &g_cfg.misc.bunnyhop);
							draw_combo(crypt_str("Automatic strafes"), g_cfg.misc.airstrafe, strafes, ARRAYSIZE(strafes));
							ImGui::Checkbox(crypt_str("Crouch in air"), &g_cfg.misc.crouch_in_air);
							ImGui::Checkbox(crypt_str("Gravity ragdolls"), &g_cfg.misc.ragdolls);
							ImGui::Checkbox(crypt_str("Fast stop"), &g_cfg.misc.fast_stop);
							ImGui::Checkbox(crypt_str("Slide walk"), &g_cfg.misc.slidewalk);
							ImGui::Checkbox(crypt_str("Left Hand Knife"), &g_cfg.misc.left_knife);

							draw_keybind(crypt_str("Auto peek"), &g_cfg.misc.automatic_peek, crypt_str("##AUTOPEEK__HOTKEY"));
							draw_keybind(crypt_str("Edge jump"), &g_cfg.misc.edge_jump, crypt_str("##EDGEJUMP__HOTKEY"));

						}


						if (subtab == 9)
						{
							ImGui::Checkbox(crypt_str("Clantag"), &g_cfg.misc.clantag_spammer);
							ImGui::Checkbox(crypt_str("Keybinds"), &g_cfg.misc.draw_keys);
							ImGui::Checkbox(crypt_str("Auto Accept"), &g_cfg.misc.auto_accept);
							//ImGui::Checkbox(crypt_str("Fake Prime"), &g_cfg.misc.prime);
							ImGui::Checkbox(crypt_str("Chat spam"), &g_cfg.misc.chat);
							ImGui::Checkbox(crypt_str("Grenade Helper"), &g_cfg.misc.ingame_grenade);
							ImGui::Checkbox(crypt_str("Enable buybot"), &g_cfg.misc.buybot_enable);

							if (g_cfg.misc.buybot_enable)
							{
								draw_combo(crypt_str("Snipers"), g_cfg.misc.buybot1, mainwep, ARRAYSIZE(mainwep));
								padding(0, 3);
								draw_combo(crypt_str("Pistols"), g_cfg.misc.buybot2, secwep, ARRAYSIZE(secwep));
								padding(0, 3);
								draw_multicombo(crypt_str("Other"), g_cfg.misc.buybot3, grenades, ARRAYSIZE(grenades), preview);
							}

							draw_combo(crypt_str("Hitsound"), g_cfg.esp.hitsound, sounds, ARRAYSIZE(sounds));
							ImGui::Checkbox(crypt_str("Killsound"), &g_cfg.esp.killsound);
							draw_multicombo(crypt_str("Logs"), g_cfg.misc.events_to_log, events, ARRAYSIZE(events), preview);
							padding(0, 3);
							draw_multicombo(crypt_str("Logs output"), g_cfg.misc.log_output, events_output, ARRAYSIZE(events_output), preview);

							if (g_cfg.misc.events_to_log[EVENTLOG_HIT] || g_cfg.misc.events_to_log[EVENTLOG_ITEM_PURCHASES] || g_cfg.misc.events_to_log[EVENTLOG_BOMB])
							{
								ImGui::Text(crypt_str("Color "));
								ImGui::SameLine();
								ImGui::ColorEdit(crypt_str("##logcolor"), &g_cfg.misc.log_color, ALPHA);
							}

							ImGui::Checkbox(crypt_str("Show CS:GO logs"), &g_cfg.misc.show_default_log);

							draw_combo(crypt_str("Force mm region"), g_cfg.misc.region_changer, mmregions, ARRAYSIZE(mmregions));
							if (ImGui::CustomButton(crypt_str("Apply region changes"), crypt_str("##CONFIG__CREATE"), ImVec2(220 * dpi_scale, 26 * dpi_scale)))
							{
								misc::get().ChangeRegion();
							}

							ImGui::Checkbox(crypt_str("Blockbot"), &g_cfg.misc.blockbot_enabled);

							if (g_cfg.misc.blockbot_enabled == 1)
							{
								draw_keybind(crypt_str("Blockbot key"), &g_cfg.misc.block_bot, crypt_str("##BLOCKBOT__HOTKEY"));
								padding(10, 0);
								draw_combo(crypt_str("Blockbot type"), g_cfg.misc.blockbot_type, blockbot_type_cb, ARRAYSIZE(blockbot_type_cb));
							}

						}


					}

					if (tab == 3)
					{
						if (subtab == 10)
						{


							// hey stewen, what r u doing there? he, hm heee, DRUGS
							static bool drugs = false;

							// some animation logic(switch)
							static bool active_animation = false;
							static bool preview_reverse = false;
							static float switch_alpha = 1.f;
							static int next_id = -1;
							if (active_animation)
							{
								if (preview_reverse)
								{
									if (switch_alpha == 1.f) //-V550
									{
										preview_reverse = false;
										active_animation = false;
									}

									switch_alpha = math::clamp(switch_alpha + (4.f * ImGui::GetIO().DeltaTime), 0.01f, 1.f);
								}
								else
								{
									if (switch_alpha == 0.01f) //-V550
									{
										preview_reverse = true;
									}

									switch_alpha = math::clamp(switch_alpha - (4.f * ImGui::GetIO().DeltaTime), 0.01f, 1.f);
								}
							}
							else
								switch_alpha = math::clamp(switch_alpha + (4.f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);

							//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, (1.f - preview_alpha) * public_alpha * switch_alpha);
							//child_title(current_profile == -1 ? crypt_str("Skinchanger") : game_data::weapon_names[current_profile].name);
							//ImGui::Spacing();
							//ImGui::BeginChild(crypt_str("##SKINCHANGER__CHILD"), ImVec2(586 * dpi_scale, (child_height - 35) * dpi_scale));
							//{
								// we need to count our items in 1 line
							auto same_line_counter = 0;

							// if we didnt choose any weapon
							if (current_profile == -1)
							{
								for (auto i = 0; i < g_cfg.skins.skinChanger.size(); i++)
								{
									// do we need update our preview for some reasons?
									if (!all_skins[i])
									{
										g_cfg.skins.skinChanger.at(i).update();
										all_skins[i] = get_skin_preview(get_wep(i, (i == 0 || i == 1) ? g_cfg.skins.skinChanger.at(i).definition_override_vector_index : -1, i == 0).c_str(), g_cfg.skins.skinChanger.at(i).skin_name, device); //-V810
									}

									// we clicked on weapon
									if (ImGui::ImageButton(all_skins[i], ImVec2(87, 76)))
									{
										next_id = i;
										active_animation = true;
									}

									// if our animation step is half from all - switch profile
									if (active_animation && preview_reverse)
									{
										ImGui::SetScrollY(0);
										current_profile = next_id;
									}

									if (same_line_counter < 4) { // continue push same-line
										ImGui::SameLine();
										same_line_counter++;
									}
									else { // we have maximum elements in 1 line
										same_line_counter = 0;
									}
								}
							}
							else
							{
								// update skin preview bool
								static bool need_update[36];

								// we pressed crypt_str("Save & Close") button
								static bool leave;

								// update if we have nullptr texture or if we push force update
								if (!all_skins[current_profile] || need_update[current_profile])
								{
									all_skins[current_profile] = get_skin_preview(get_wep(current_profile, (current_profile == 0 || current_profile == 1) ? g_cfg.skins.skinChanger.at(current_profile).definition_override_vector_index : -1, current_profile == 0).c_str(), g_cfg.skins.skinChanger.at(current_profile).skin_name, device); //-V810
									need_update[current_profile] = false;
								}

								// get settings for selected weapon
								auto& selected_entry = g_cfg.skins.skinChanger[current_profile];
								selected_entry.itemIdIndex = current_profile;

								ImGui::BeginGroup();
								ImGui::PushItemWidth(260 * dpi_scale);

								// search input later
								static char search_skins[64] = "\0";
								static auto item_index = selected_entry.paint_kit_vector_index;

								if (!current_profile)
								{
									ImGui::Text(crypt_str("Knife"));
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5 * c_menu::get().dpi_scale);
									if (ImGui::Combo(crypt_str("##Knife_combo"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
										{
											*out_text = game_data::knife_names[idx].name;
											return true;
										}, nullptr, IM_ARRAYSIZE(game_data::knife_names)))
										need_update[current_profile] = true; // push force update
								}
								else if (current_profile == 1)
								{
									ImGui::Text(crypt_str("Gloves"));
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5 * c_menu::get().dpi_scale);
									if (ImGui::Combo(crypt_str("##Glove_combo"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
										{
											*out_text = game_data::glove_names[idx].name;
											return true;
										}, nullptr, IM_ARRAYSIZE(game_data::glove_names)))
									{
										item_index = 0; // set new generated paintkits element to 0;
										need_update[current_profile] = true; // push force update
									}
								}
								else
									selected_entry.definition_override_vector_index = 0;

								if (current_profile != 1)
								{
									ImGui::Text(crypt_str("Search"));
									ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

									if (ImGui::InputText(crypt_str("##search"), search_skins, sizeof(search_skins)))
										item_index = -1;

									ImGui::PopStyleVar();
								}

								auto main_kits = current_profile == 1 ? SkinChanger::gloveKits : SkinChanger::skinKits;
								auto display_index = 0;

								SkinChanger::displayKits = main_kits;

								// we dont need custom gloves
								if (current_profile == 1)
								{
									for (auto i = 0; i < main_kits.size(); i++)
									{
										auto main_name = main_kits.at(i).name;

										for (auto i = 0; i < main_name.size(); i++)
											if (iswalpha((main_name.at(i))))
												main_name.at(i) = towlower(main_name.at(i));

										char search_name[64];

										if (!strcmp(game_data::glove_names[selected_entry.definition_override_vector_index].name, crypt_str("Hydra")))
											strcpy_s(search_name, sizeof(search_name), crypt_str("Bloodhound"));
										else
											strcpy_s(search_name, sizeof(search_name), game_data::glove_names[selected_entry.definition_override_vector_index].name);

										for (auto i = 0; i < sizeof(search_name); i++)
											if (iswalpha(search_name[i]))
												search_name[i] = towlower(search_name[i]);

										if (main_name.find(search_name) != std::string::npos)
										{
											SkinChanger::displayKits.at(display_index) = main_kits.at(i);
											display_index++;
										}
									}

									SkinChanger::displayKits.erase(SkinChanger::displayKits.begin() + display_index, SkinChanger::displayKits.end());
								}
								else
								{
									if (strcmp(search_skins, crypt_str(""))) //-V526
									{
										for (auto i = 0; i < main_kits.size(); i++)
										{
											auto main_name = main_kits.at(i).name;

											for (auto i = 0; i < main_name.size(); i++)
												if (iswalpha(main_name.at(i)))
													main_name.at(i) = towlower(main_name.at(i));

											char search_name[64];
											strcpy_s(search_name, sizeof(search_name), search_skins);

											for (auto i = 0; i < sizeof(search_name); i++)
												if (iswalpha(search_name[i]))
													search_name[i] = towlower(search_name[i]);

											if (main_name.find(search_name) != std::string::npos)
											{
												SkinChanger::displayKits.at(display_index) = main_kits.at(i);
												display_index++;
											}
										}

										SkinChanger::displayKits.erase(SkinChanger::displayKits.begin() + display_index, SkinChanger::displayKits.end());
									}
									else
										item_index = selected_entry.paint_kit_vector_index;
								}

								ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
								if (!SkinChanger::displayKits.empty())
								{
									if (ImGui::ListBox(crypt_str("##PAINTKITS"), &item_index, [](void* data, int idx, const char** out_text) //-V107
										{
											while (SkinChanger::displayKits.at(idx).name.find(crypt_str("С‘")) != std::string::npos) //-V807
												SkinChanger::displayKits.at(idx).name.replace(SkinChanger::displayKits.at(idx).name.find(crypt_str("С‘")), 2, crypt_str("Рµ"));

											*out_text = SkinChanger::displayKits.at(idx).name.c_str();
											return true;
										}, nullptr, SkinChanger::displayKits.size(), SkinChanger::displayKits.size() > 9 ? 9 : SkinChanger::displayKits.size()) || !all_skins[current_profile])
									{
										SkinChanger::scheduleHudUpdate();
										need_update[current_profile] = true;

										auto i = 0;

										while (i < main_kits.size())
										{
											if (main_kits.at(i).id == SkinChanger::displayKits.at(item_index).id)
											{
												selected_entry.paint_kit_vector_index = i;
												break;
											}

											i++;
										}

									}
								}
								//ImGui::PopStyleVar();

								if (ImGui::InputInt(crypt_str("Seed"), &selected_entry.seed, 1, 100))
									SkinChanger::scheduleHudUpdate();

								if (ImGui::InputInt(crypt_str("StatTrak"), &selected_entry.stat_trak, 1, 15))
									SkinChanger::scheduleHudUpdate();

								if (ImGui::SliderFloat(crypt_str("Wear"), &selected_entry.wear, 0.0f, 1.0f))
									drugs = true;
								else if (drugs)
								{
									SkinChanger::scheduleHudUpdate();
									drugs = false;
								}

								ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6 * c_menu::get().dpi_scale);
								ImGui::Text(crypt_str("Quality"));
								ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5 * c_menu::get().dpi_scale);
								if (ImGui::Combo(crypt_str("##Quality_combo"), &selected_entry.entity_quality_vector_index, [](void* data, int idx, const char** out_text)
									{
										*out_text = game_data::quality_names[idx].name;
										return true;
									}, nullptr, IM_ARRAYSIZE(game_data::quality_names)))
									SkinChanger::scheduleHudUpdate();

									if (current_profile != 1)
									{
										if (!g_cfg.skins.custom_name_tag[current_profile].empty())
											strcpy_s(selected_entry.custom_name, sizeof(selected_entry.custom_name), g_cfg.skins.custom_name_tag[current_profile].c_str());

										ImGui::Text(crypt_str("Name Tag"));
										ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

										if (ImGui::InputText(crypt_str("##nametag"), selected_entry.custom_name, sizeof(selected_entry.custom_name)))
										{
											g_cfg.skins.custom_name_tag[current_profile] = selected_entry.custom_name;
											SkinChanger::scheduleHudUpdate();
										}

										ImGui::PopStyleVar();
									}

									ImGui::PopItemWidth();

									ImGui::EndGroup();

									ImGui::SameLine();
									ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 30);

									ImGui::BeginGroup();
									if (ImGui::ImageButton(all_skins[current_profile], ImVec2(190, 155)))
									{
										// maybe i will do smth later where, who knows :/
									}

									if (ImGui::CustomButton(crypt_str("Close"), crypt_str("##CLOSE__SKING"), ImVec2(198 * dpi_scale, 26 * dpi_scale)))
									{
										// start animation
										active_animation = true;
										next_id = -1;
										leave = true;
									}
									ImGui::Checkbox(crypt_str("Force rare animations"), &g_cfg.skins.rare_animations);

									ImGui::EndGroup();

									// update element
									selected_entry.update();

									// we need to reset profile in the end to prevent render images with massive's index == -1
									if (leave && (preview_reverse || !active_animation))
									{
										ImGui::SetScrollY(0);
										current_profile = next_id;
										leave = false;
									}

							}
						}

						/*if (subtab == 18)
						{
							static int index = 0;
							static wskin weaponSkin;
							// we need to count our items in 1 line


							ImGui::BeginGroup();
							ImGui::PushItemWidth(260 * dpi_scale);
							ImGui::Checkbox("Enable inventory", &g_cfg.inventory.activate_inventory);
							ImGui::Checkbox("Enable autorarity", &g_cfg.inventory.autorarity);

							if (weaponSkin.wId == WEAPON_NONE)
								weaponSkin.wId = WEAPON_DEAGLE;



							if (ImGui::BeginCombo("Weapon", k_inventory_names.at(weaponSkin.wId), ImGuiComboFlags_HeightLargest))
							{
								for (const auto& weapon : k_inventory_names)
								{
									if (ImGui::CustomSelectable(weapon.second, weaponSkin.wId == weapon.first, 0, ImVec2()))
									{
										weaponSkin.wId = weapon.first;
										weaponSkin.paintKit = 0;
										//skinImage = nullptr;
									}
								}
								ImGui::EndCombo();
							}

							auto weaponName = zweaponnames(weaponSkin.wId);
							ImGui::Text("Skin");

							if (ImGui::BeginCombo("##Paint Kit", weaponSkin.paintKit > 0 ? _inv.inventory.skinInfo[weaponSkin.paintKit].name.c_str() : "", ImGuiComboFlags_HeightLargest))
							{
								int lastID = ImGui::GetItemID();

								for (auto skin : _inv.inventory.skinInfo)
								{
									for (auto names : skin.second.weaponName)
									{
										if (weaponName != names)
											continue;

										ImGui::PushID(lastID++);

										if (ImGui::CustomSelectable(skin.second.name.c_str(), skin.first == weaponSkin.paintKit, 0, ImVec2()))
											weaponSkin.paintKit = skin.first;

										ImGui::PopID();
									}
								}
								ImGui::EndCombo();
							}


							ImGui::SliderFloat("Wear", &weaponSkin.wear, 0.f, 1.f);
							ImGui::SliderInt("Seed", &weaponSkin.seed, 0, 100);

							static const char* t[] = { "common","uncommon","rare","mythical","legendary","ancient","immortal" };
							//draw_combo("Rarity", weaponSkin.quality, t, ARRAYSIZE(t));


							if (!g_cfg.inventory.autorarity)
								draw_combo("Rarity", weaponSkin.quality, t, ARRAYSIZE(t));


							static char skinname[64] = "\0";


							ImGui::InputText(("##skinname"), skinname, sizeof(skinname));
							ImGui::InputInt("Stattrak", &weaponSkin.stattrak);



							static int stickerkit[4] = { 0,0,0,0 };


							if (weaponSkin.wId <= 100 && weaponSkin.wId != 42 && weaponSkin.wId != 59)
							{

								if (ImGui::BeginCombo("Sticker 1", g_Stickers[stickerkit[0]].name.c_str(), ImGuiComboFlags_HeightLargest))
								{
									int lastID = ImGui::GetItemID();

									for (auto skin : fosso)
									{
										{
											ImGui::PushID(lastID++);
											if (ImGui::CustomSelectable(skin.second.name.c_str(), skin.second.paintKit == stickerkit[0], 0, ImVec2()))
												stickerkit[0] = skin.second.paintKit;
											ImGui::PopID();
										}
									}
									ImGui::EndCombo();
								}

								if (ImGui::BeginCombo("Sticker 2", g_Stickers[stickerkit[1]].name.c_str(), ImGuiComboFlags_HeightLargest))
								{
									int lastID = ImGui::GetItemID();

									for (auto skin : fosso)
									{
										{
											ImGui::PushID(lastID++);
											if (ImGui::CustomSelectable(skin.second.name.c_str(), skin.second.paintKit == stickerkit[1], 0, ImVec2()))
												stickerkit[1] = skin.second.paintKit;
											ImGui::PopID();
										}
									}
									ImGui::EndCombo();
								}

								if (ImGui::BeginCombo("Sticker 3", g_Stickers[stickerkit[2]].name.c_str(), ImGuiComboFlags_HeightLargest))
								{
									int lastID = ImGui::GetItemID();

									for (auto skin : fosso)
									{
										{
											ImGui::PushID(lastID++);
											if (ImGui::CustomSelectable(skin.second.name.c_str(), skin.second.paintKit == stickerkit[2], 0, ImVec2()))
												stickerkit[2] = skin.second.paintKit;
											ImGui::PopID();
										}
									}
									ImGui::EndCombo();
								}

								if (ImGui::BeginCombo("Sticker 4", g_Stickers[stickerkit[3]].name.c_str(), ImGuiComboFlags_HeightLargest))
								{
									int lastID = ImGui::GetItemID();

									for (auto skin : fosso)
									{
										{
											ImGui::PushID(lastID++);
											if (ImGui::CustomSelectable(skin.second.name.c_str(), skin.second.paintKit == stickerkit[3], 0, ImVec2()))
												stickerkit[3] = skin.second.paintKit;
											ImGui::PopID();
										}
									}
									ImGui::EndCombo();
								}


							}



							if (ImGui::CustomButton("Add", "##fuckyouasshole1", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
							{
								weaponSkin.sicker[0] = stickerkit[0];
								weaponSkin.sicker[1] = stickerkit[1];
								weaponSkin.sicker[2] = stickerkit[2];
								weaponSkin.sicker[3] = stickerkit[3];

								std::string str(skinname);
								if (str.length() > 0)
									weaponSkin.name = str;
								g_InventorySkins.insert({ g_csgo.fRandomInt(20000, 200000), weaponSkin });
								_inv.inventory.itemCount = g_InventorySkins.size();
								stickerkit[0] = 0;
								stickerkit[1] = 0;
								stickerkit[2] = 0;
								stickerkit[3] = 0;
								SkinChanger::scheduleHudUpdate();
								//index = 0;
							}
							if (ImGui::CustomButton("Apply", "##fuckyouasshole2", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
							{
								m_engine()->ExecuteClientCmd("econ_clear_inventory_images");
								write.SendClientHello();
								write.SendMatchmakingClient2GCHello();
								SkinChanger::scheduleHudUpdate();
							}
							if (g_InventorySkins.size() > 0)
							{
								if (ImGui::CustomButton("Delete selected element", "##fuckyouasshole3", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
								{
									// if (g_InventorySkins[index] != NULL)
									g_InventorySkins.erase(index);
									_inv.inventory.itemCount = g_InventorySkins.size();
								}
							}



							



							ImGui::EndGroup();




						//}
						//ImGui::EndChild();

						//ImGui::SameLine();

						//ImGui::BeginChild(crypt_str("##fuckmeass__CHILD"), ImVec2(486 * dpi_scale / 2, (child_height - 35)* dpi_scale));
						//{
							ImGui::BeginGroup();


							if (weaponSkin.wId > 0 && weaponSkin.wId <= 100)
							{
								std::string name(zweaponnames(weaponSkin.wId));
								std::string smallfix = "weapon_" + name + "_" + _inv.inventory.skinInfo[weaponSkin.paintKit].cdnName;
								auto url = FindURl(smallfix);
								{
									UpdatePic(c_menu::get().device, url);
									if (some_texture_test)
									{
										//ui::SetCursorPos(ImVec2(ui::GetCursorPos().x + 5, ui::GetCursorPos().y - 20));
										ImGui::ImageButton(some_texture_test, ImVec2(190, 155));
									}

								}
							}
							else
							{
								std::string name(zweaponnames(weaponSkin.wId));
								std::string smallfix = name + "_" + _inv.inventory.skinInfo[weaponSkin.paintKit].cdnName;
								auto url = FindURl(smallfix);
								{
									UpdatePic(c_menu::get().device, url);
									if (some_texture_test) {
										//ui::SetCursorPos(ImVec2(ui::GetCursorPos().x + 5, ui::GetCursorPos().y - 20));
										ImGui::ImageButton(some_texture_test, ImVec2(190, 155));
									}

								}
							}

							//ImGui::BeginChild("Weapon list", ImVec2(328, 260)); {

								int i = 0;
								for (auto weapon : g_InventorySkins) {

									std::string weap = zweaponnames(weapon.second.wId);
									std::stringstream whatever;
									whatever << ++i;
									if (ImGui::CustomSelectable(std::string("" + whatever.str() + " " + weap + " - " + (weapon.second.paintKit > 0 ? _inv.inventory.skinInfo[weapon.second.paintKit].name.c_str() : "") + "").c_str(), index == weapon.first)) {
										index = weapon.first;
									}

								}
								i = 0;
						//	}
							//ImGui::EndChild();
							ImGui::PopItemWidth();
							ImGui::EndGroup();


						//}
						//ImGui::EndChild();
					}*/


						//}
						if (subtab == 19)
						{

							// we need to count our items in 1 line


							ImGui::BeginGroup();

							ImGui::Checkbox("Enable profile changer", &g_cfg.profilechanger.enable);




							ImGui::InputInt("Friend", &g_cfg.profilechanger.profile_friend);
							ImGui::InputInt("Leader", &g_cfg.profilechanger.profile_leader);
							ImGui::InputInt("Teach", &g_cfg.profilechanger.profile_teach);




							static const char* tt[] = { "Off",
			"Silver 1",
			"Silver 2",
			"Silver 3",
			"Silver 4",
			"Silver elite",
			"Silver elite master",
			"Gold nova 1",
			"Gold nova 2",
			"Gold nova 3",
			"Gold nova master",
			"Master guardian 1",
			"Master guardian 2",
			"Master guardian elite",
			"Distinguished master guardian",
			"Legendary eagle",
			"Legendary eagle master",
			"Supreme master first class",
			"The global elite" };

							draw_combo("Rank", g_cfg.profilechanger.profile_rank, tt, ARRAYSIZE(tt));







							ImGui::InputInt("Wins", &g_cfg.profilechanger.profile_rank_wins);
							ImGui::SliderInt("Level", &g_cfg.profilechanger.profile_lvl, 0, 40);
							ImGui::SliderInt("Level xp", &g_cfg.profilechanger.profile_xp, 0, 5000);

							if (ImGui::CustomButton("Apply", "##fucksityeah", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
							{
								write.SendClientHello();
								write.SendMatchmakingClient2GCHello();
							}

							draw_combo("Rank wing", g_cfg.profilechanger.r_rank, tt, ARRAYSIZE(tt));


							ImGui::InputInt("Wins wing", &g_cfg.profilechanger.r_wins);

							if (ImGui::CustomButton("Apply", "##fucksityeahasgd", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
								write.SendClientGcRankUpdate1();

							static const char* ttt[] = { "Off",
		   "Lab rat 1",
		   "Lab rat 2",
		   "Sprinting Hare 1",
		   "Sprinting Hare 2",
		   "Wild Scout 1",
		   "Wild Scout Elite",
		   "Hunter Fox 1",
		   "Hunter Fox 2",
		   "Hunter Fox 3",
		   "Hunter Fox Elite",
		   "Timber Wolf",
		   "Ember Wolf",
		   "Wildfire Wolf",
		   "The Howling Alpha" };
							draw_combo("Rank zone", g_cfg.profilechanger.t_rank, ttt, ARRAYSIZE(ttt));


							ImGui::InputInt("Wins zone", &g_cfg.profilechanger.t_wins);

							if (ImGui::CustomButton("Apply", "##fuckassmymor", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
								write.SendClientGcRankUpdate2();




							ImGui::EndGroup();
						//}

						}

						if (subtab == 20)
						{

							static int wId = 0;
							ImGui::BeginGroup();


							static int paintKit = 874;
							ImGui::Checkbox("Enable", &g_cfg.inventory.activate_inventory);
							ImGui::Text("Medals");


							if (ImGui::BeginCombo("##PaintKit_yea", fosso[paintKit].name.c_str(), ImGuiComboFlags_HeightLargest))
							{
								int lastID = ImGui::GetItemID();

								for (auto skin : fosso)
								{
									{
										ImGui::PushID(lastID++);
										if (ImGui::CustomSelectable(skin.second.name.c_str(), skin.second.paintKit == paintKit, 0, ImVec2()))
											paintKit = skin.second.paintKit;
										ImGui::PopID();
									}
								}
								ImGui::EndCombo();
							}
							if (ImGui::CustomButton("Add medal", "##ass300%", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
								g_MedalSkins[g_csgo.fRandomInt(200001, 1000000)] = { paintKit , 0 };



							if (ImGui::CustomButton("Apply medals", "##ass3s00%", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
							{
								paintKit;
								m_engine()->ExecuteClientCmd("econ_clear_inventory_images");
								write.SendClientHello();
								write.SendMatchmakingClient2GCHello();
							}


							if (g_MedalSkins.size() > 0)
							{
								if (ImGui::CustomButton("Delete selected element", "##ass3asdg00%", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
									g_MedalSkins.erase(wId);

							}




							ImGui::EndGroup();
						//}

						ImGui::BeginGroup();
						int i = 0;
						for (auto weapon : g_MedalSkins) {

							std::stringstream whatever;
							whatever << ++i;
							if (ImGui::CustomSelectable(std::string(whatever.str() + " " + fosso[weapon.second.paintKit].name).c_str(), wId == weapon.first)) {
								wId = weapon.first;
							}
						}
						i = 0;

						ImGui::EndGroup();

						}
						}
						

					if (tab == 4)
					{
						if (subtab == 11)
						{
							ImGui::Checkbox(crypt_str("Anti-untrusted"), &g_cfg.misc.anti_untrusted);
							ImGui::Checkbox(crypt_str("Rank reveal"), &g_cfg.misc.rank_reveal);
							ImGui::Checkbox(crypt_str("Unlock inventory access"), &g_cfg.misc.inventory_access);
							ImGui::Checkbox(crypt_str("Bypass sv_pure"), &g_cfg.misc.no_pure);

						}




						if (subtab == 12)
						{
							static auto should_update = true;

							if (should_update)
							{
								should_update = false;

								cfg_manager->config_files();
								files = cfg_manager->files;

								for (auto& current : files)
									if (current.size() > 2)
										current.erase(current.size() - 3, 3);
							}

							if (ImGui::CustomButton(crypt_str("Open configs folder"), crypt_str("##CONFIG__FOLDER"), ImVec2(507 * dpi_scale, 26 * dpi_scale)))
							{
								std::string folder;

								auto get_dir = [&folder]() -> void
								{
									static TCHAR path[MAX_PATH];

									if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, NULL, path)))
										folder = std::string(path) + crypt_str("\\DoubleTap.Space\\");

									CreateDirectory(folder.c_str(), NULL);
								};

								get_dir();
								ShellExecute(NULL, crypt_str("open"), folder.c_str(), NULL, NULL, SW_SHOWNORMAL);
							}

							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
							ImGui::ListBoxConfigArray(crypt_str("##CONFIGS"), &g_cfg.selected_config, files, 6);
							ImGui::PopStyleVar();

							if (ImGui::CustomButton(crypt_str("Refresh configs"), crypt_str("##CONFIG__REFRESH"), ImVec2(507 * dpi_scale, 26 * dpi_scale)))
							{
								cfg_manager->config_files();
								files = cfg_manager->files;

								for (auto& current : files)
									if (current.size() > 2)
										current.erase(current.size() - 3, 3);
							}

							static char config_name[64] = "\0";

							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
							ImGui::InputText(crypt_str("##configname"), config_name, sizeof(config_name));
							ImGui::PopStyleVar();

							if (ImGui::CustomButton(crypt_str("Create config"), crypt_str("##CONFIG__CREATE"), ImVec2(507 * dpi_scale, 26 * dpi_scale)))
							{
								g_cfg.new_config_name = config_name;
								add_config();
							}

							static auto next_save = false;
							static auto prenext_save = false;
							static auto clicked_sure = false;
							static auto save_time = m_globals()->m_realtime;
							static auto save_alpha = 1.0f;

							//save_alpha = math::clamp(save_alpha + (4.f * ImGui::GetIO().DeltaTime * (!prenext_save ? 1.f : -1.f)), 0.01f, 1.f);
							//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, save_alpha * public_alpha * (1.f - preview_alpha));



							if (ImGui::CustomButton(crypt_str("Save config"), crypt_str("##CONFIG__SAVE"), ImVec2(507 * dpi_scale, 26 * dpi_scale)))
								save_config();




							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
							ImGui::PopStyleVar();

							if (ImGui::CustomButton(crypt_str("Load config"), crypt_str("##CONFIG__LOAD"), ImVec2(507 * dpi_scale, 26 * dpi_scale)))
								load_config();

							static auto next_delete = false;
							static auto prenext_delete = false;
							static auto clicked_sure_del = false;
							static auto delete_time = m_globals()->m_realtime;
							static auto delete_alpha = 1.0f;

							//delete_alpha = math::clamp(delete_alpha + (4.f * ImGui::GetIO().DeltaTime * (!prenext_delete ? 1.f : -1.f)), 0.01f, 1.f);
							//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, delete_alpha * public_alpha * (1.f - preview_alpha));



							if (ImGui::CustomButton(crypt_str("Remove config"), crypt_str("##CONFIG__delete"), ImVec2(507 * dpi_scale, 26 * dpi_scale)))
								remove_config();

							//if (ImGui::CustomButton(crypt_str("Import from Clipboard"), crypt_str("##CONFIG__delete"), ImVec2(507 * dpi_scale, 26 * dpi_scale)))
								//import_from_clipboard();

							ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
							ImGui::PopStyleVar();




						}



					}





							//ImGui::EndChild();

						}


					//}
					

					ImGui::EndGroup();
					ImGui::PopFont();
				}
				ImGui::EndChild();
			}
			ImGui::EndGroup();
		}
		ImGui::End();
		ImGui::PopStyleVar(3);
	}
	
//}

