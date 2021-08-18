#pragma once
#include "../includes.hpp"


class c_menu : public singleton<c_menu> {
public:
	void draw(bool is_open);
	void menu_setup(ImGuiStyle& style);

	ImFont* icons;
	ImFont* big;
	ImFont* bigger;
	ImFont* weapon_ic;
	ImFont* new_icons;
	ImFont* verdana_12;
	ImFont* vermin;
	float child_height;
	int current_profile = -1;

	float dpi_scale = 1.f;
	float public_alpha;
	float color_buffer[4] = { 1.f, 1.f, 1.f, 1.f };
	ImGuiStyle style;


	int players_section;

	IDirect3DDevice9* device;
	LPDIRECT3DTEXTURE9 CTmd1 = nullptr;
	LPDIRECT3DTEXTURE9 head = nullptr;
	LPDIRECT3DTEXTURE9 head1 = nullptr;

	LPDIRECT3DTEXTURE9 neck = nullptr;
	LPDIRECT3DTEXTURE9 neck1 = nullptr;
	LPDIRECT3DTEXTURE9 upchest = nullptr;
	LPDIRECT3DTEXTURE9 upchest1 = nullptr;
	LPDIRECT3DTEXTURE9 chest = nullptr;
	LPDIRECT3DTEXTURE9 chest1 = nullptr;
	LPDIRECT3DTEXTURE9 lowchest = nullptr;
	LPDIRECT3DTEXTURE9 lowchest1 = nullptr;
	LPDIRECT3DTEXTURE9 pel = nullptr;
	LPDIRECT3DTEXTURE9 pel1 = nullptr;

	LPDIRECT3DTEXTURE9 arms = nullptr;
	LPDIRECT3DTEXTURE9 arms1 = nullptr;
	LPDIRECT3DTEXTURE9 arms2 = nullptr;
	LPDIRECT3DTEXTURE9 arms3 = nullptr;

	LPDIRECT3DTEXTURE9 legs = nullptr;
	LPDIRECT3DTEXTURE9 legs1 = nullptr;
	LPDIRECT3DTEXTURE9 legs2 = nullptr;
	LPDIRECT3DTEXTURE9 legs3 = nullptr;

	LPDIRECT3DTEXTURE9 feet = nullptr;
	LPDIRECT3DTEXTURE9 feet1 = nullptr;
	LPDIRECT3DTEXTURE9 feet2 = nullptr;
	LPDIRECT3DTEXTURE9 feet3 = nullptr;


private:
	struct {
		ImVec2 WindowPadding;
		float  WindowRounding;
		ImVec2 WindowMinSize;
		float  ChildRounding;
		float  PopupRounding;
		ImVec2 FramePadding;
		float  FrameRounding;
		ImVec2 ItemSpacing;
		ImVec2 ItemInnerSpacing;
		ImVec2 TouchExtraPadding;
		float  IndentSpacing;
		float  ColumnsMinSpacing;
		float  ScrollbarSize;
		float  ScrollbarRounding;
		float  GrabMinSize;
		float  GrabRounding;
		float  TabRounding;
		float  TabMinWidthForUnselectedCloseButton;
		ImVec2 DisplayWindowPadding;
		ImVec2 DisplaySafeAreaPadding;
		float  MouseCursorScale;
	} styles;

	void dpi_resize(float scale_factor, ImGuiStyle& style);
	IDirect3DTexture9* m_pTexture;
	float preview_alpha = 1.f;


	int m_nCurrentTab;
	std::string preview = crypt_str("None");
};
