// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <ShlObj.h>
#include <ShlObj_core.h>
#include "includes.hpp"
#include "utils\ctx.hpp"
#include "utils\recv.h"
#include "utils\imports.h"
//#include "utils\anti_debug.h"
#include "nSkinz\SkinChanger.h"
#include "fff.h"
#include "minhook/MinHook.h"
//#include "Bytesa.h"


#include "inventory/kitparser.h"
#include "skinchanger/skins.h"


//#include "utils\sha-256.h"
//#include "utils\protect.h"

//using namespace jwt::params;

template< typename o = void*, typename i = void* >
__forceinline o f_force_cast(i in) {
	union { i in; o out; }
	u = { in };
	return u.out;
};


VMT f_client;
VMT m_net_channel;
using LevelInitPostEntity_t = void(__thiscall*)(void*);
using LevelShutdown_t = void(__thiscall*)(void*);
using LevelInitPreEntity_t = void(__thiscall*)(void*, const char*);
using SendNetMsg_t = bool(__thiscall*)(void*, INetMessage&, bool, bool);
using ProcessPacket_t = void(__thiscall*)(void*, void*, bool);
class fooks {
public:
	void                     LevelInitPostEntity();
	void                     LevelInitPreEntity(const char* map);
	void                     ProcessPacket(void* packet, bool header);
	bool					 SendNetMsg11(INetMessage& msg, bool bForceReliable, bool bVoice);
};
#include "cheats/misc/logs.h"
void fooks::LevelInitPreEntity(const char* map) {
	std::stringstream yah;
	yah << "welcome on " << map;
	eventlogs::get().addnew(yah.str(), Color::Yellow, false); //-V106
	f_client.GetOldMethod< LevelInitPreEntity_t >(5)(this, map);
}


void fooks::ProcessPacket(void* packet, bool header) {
	m_net_channel.GetOldMethod< ProcessPacket_t >(39)(this, packet, header);

	// get this from CL_FireEvents string "Failed to execute event for classId" in engine.dll
	for (CEventInfo* it{ m_clientstate()->m_events }; it != nullptr; it = it->m_next) {
		if (!it->m_class_id)
			continue;

		// set all delays to instant.
		it->m_fire_delay = 0.f;
	}

	// game events are actually fired in OnRenderStart which is WAY later after they are received
	// effective delay by lerp time, now we call them right after theyre received (all receive proxies are invoked without delay).
	m_engine()->FireEvents();
}

bool fooks::SendNetMsg11(INetMessage& msg, bool bForceReliable, bool bVoice)
{
	if (!m_engine()->GetNetChannelInfo())
		return m_net_channel.GetOldMethod<SendNetMsg_t>(40)(this, msg, bForceReliable, bVoice);

	if (m_gamerules()->m_bIsValveDS() && msg.GetType() == 14) // Return and don't send messsage if its FileCRCCheck
		return false;

	if (msg.GetGroup() == 9) { // Fix lag when transmitting voice and fakelagging
		bVoice = true;
	}

	return m_net_channel.GetOldMethod<SendNetMsg_t>(40)(this, msg, bForceReliable, bVoice);
}



void fooks::LevelInitPostEntity() {


	// if the INetChannelInfo pointer has changed, store it for later.
	auto m_net = m_engine()->GetNetChannelInfo();

	if (m_net) {
		m_net_channel.reset();
		m_net_channel.init(m_net);
		m_net_channel.add(39 /*PROCESSPACKET*/, f_force_cast(&fooks::ProcessPacket));
		//m_net_channel.add(INetChannel::SENDDATAGRAM, util::force_cast(&Hooks::SendDatagram));
		m_net_channel.add(40 /*SENDNETMSG*/, f_force_cast(&fooks::SendNetMsg11));
	}




	f_client.GetOldMethod< LevelInitPostEntity_t >(6)(this);
}





PVOID base_address = nullptr;

class c_bphook
{
public:
	explicit c_bphook(void* ent)
	{
		base = reinterpret_cast<uintptr_t**>(ent);
	}

	template<typename function, typename original_function>
	function apply(uint32_t index, original_function func)
	{
		auto old_func = base[index];
		DWORD old;
		VirtualProtect(&base[index], sizeof uintptr_t, PAGE_EXECUTE_READWRITE, &old);
		base[index] = reinterpret_cast<uintptr_t*>(func);
		VirtualProtect(&base[index], sizeof uintptr_t, old, &old);
		return reinterpret_cast<function>(old_func);
	}
private:

	std::uintptr_t** base;
};

c_bphook* clientstateplus8;

//inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }


enum error_type
{
	ERROR_NONE,
	ERROR_DEBUG,
	ERROR_OPEN_KEY,
	ERROR_QUERY_DATA,
	ERROR_CONNECT,
	ERROR_1,
	ERROR_2,
	ERROR_3,
	ERROR_4,
	ERROR_5,
	ERROR_6,
	ERROR_7,
	ERROR_8,
	ERROR_9,
	ERROR_CHECK_HASH
};	

//PVOID base_address = nullptr;
//Anti_debugger anti_debugger;
volatile error_type error = ERROR_NONE;

LONG CALLBACK ExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo);
//__forceinline void crash(bool debug = false);
__forceinline void setup_render();
__forceinline void setup_netvars();
__forceinline void setup_skins();
__forceinline void setup_hooks();

// need this for 1 thing lol
//#include "supremacy merge/modules.h"
//#include "supremacy merge/hash.h"

HANDLE FindHandle(std::string name)
{
	return GetModuleHandle(name.c_str());
}

#include "mutiny merge/merged.h"
#include "mutiny merge/console.hpp"
#include "mutiny merge/NetworkedVariables.h"

DWORD WINAPI mutiny_merge(LPVOID lpThreadParameter)
{

	//shonax_console.initialize();

	//logger::add(LSUCCESS, "reached mutiny merge thread");


	if (!ClientHandle)
		ClientHandle = FindHandle("client.dll");


	if (!MatchmakingHandle)
		MatchmakingHandle = FindHandle("matchmaking.dll");


	if (!VPhysicsHandle)
		VPhysicsHandle = FindHandle("vphysics.dll");


	if (!VSTDLIBHandle)
		VSTDLIBHandle = FindHandle("vstdlib.dll");

	if (!DatacacheHandle)
		DatacacheHandle = FindHandle("datacache.dll");

	if (!Tier0Handle)
		Tier0Handle = FindHandle("tier0.dll");

	if (!MaterialSystemHandle)
		MaterialSystemHandle = FindHandle("materialsystem.dll");

	if (!StudioRenderHandle)

		StudioRenderHandle = FindHandle(("studiorender.dll"));

	if (!FileSystemStdioHandle)
		FileSystemStdioHandle = FindHandle(("filesystem_stdio.dll"));
	if (!ServerHandle)

		ServerHandle = FindHandle(("server.dll"));
	if (!EngineHandle)
		EngineHandle = FindHandle(("engine.dll"));

	if (!VGUIMatSurfaceHandle)
		VGUIMatSurfaceHandle = FindHandle(("vguimatsurface.dll"));


	//logger::add(LSUCCESS, "so it will be hook some usefull functions and console will be closed at the end");

	//logger::add(LWARN, "you already can use cheat!!!");


	shonax_mutiny.run();



	//shonax_console.close();

	return true;

}


DWORD WINAPI main(PVOID base)
{
	auto m_kernel32_dll = PE::GetModule(HASH("kernel32.dll"));
	auto m_user32_dll = PE::GetModule(HASH("user32.dll"));
	auto m_shell32_dll = PE::GetModule(HASH("shell32.dll"));
	auto m_shlwapi_dll = PE::GetModule(HASH("shlwapi.dll"));
	auto m_client_dll = PE::GetModule(HASH("client.dll"));
	auto m_engine_dll = PE::GetModule(HASH("engine.dll"));
	auto m_vstdlib_dll = PE::GetModule(HASH("vstdlib.dll"));
	auto m_tier0_dll = PE::GetModule(HASH("tier0.dll"));
	auto m_shaderapidx9_dll = PE::GetModule(HASH("shaderapidx9.dll"));


	g_csgo.fRandomInt = PE::GetExport(m_vstdlib_dll, HASH("RandomInt")).as< RandomInt_t >();


	g_ctx.signatures =
	{
		crypt_str("A1 ? ? ? ? 50 8B 08 FF 51 0C"),
		crypt_str("B9 ?? ?? ?? ?? A1 ?? ?? ?? ?? FF 10 A1 ?? ?? ?? ?? B9"),
		crypt_str("0F 11 05 ?? ?? ?? ?? 83 C8 01"),
		crypt_str("8B 0D ?? ?? ?? ?? 8B 46 08 68"),
		crypt_str("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"),
		crypt_str("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7"),
		crypt_str("A1 ? ? ? ? 8B 0D ? ? ? ? 6A 00 68 ? ? ? ? C6"),
		crypt_str("80 3D ? ? ? ? ? 53 56 57 0F 85"),
		crypt_str("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C"),
		crypt_str("80 3D ? ? ? ? ? 74 06 B8"),
		crypt_str("55 8B EC 83 E4 F0 B8 D8"),
		crypt_str("55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C"),
		crypt_str("55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6"),
		crypt_str("55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36"),
		crypt_str("56 8B F1 8B 8E ? ? ? ? 83 F9 FF 74 23"),
		crypt_str("55 8B EC 83 E4 F8 83 EC 5C 53 8B D9 56 57 83"),
		crypt_str("55 8B EC A1 ? ? ? ? 83 EC 10 56 8B F1 B9"),
		crypt_str("57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02"),
		crypt_str("55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80"),
		crypt_str("53 0F B7 1D ? ? ? ? 56"),
		crypt_str("8B 0D ? ? ? ? 8D 95 ? ? ? ? 6A 00 C6")
	};

	g_ctx.indexes =
	{
		5,
		33,
		339,
		218,
		219,
		34,
		157,
		75,
		460,
		482,
		452,
		483,
		284,
		223,
		246,
		27,
		17,
		123
	};

	while (!IFH(GetModuleHandle)(crypt_str("serverbrowser.dll")))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));








	//shonax
	static TCHAR path[MAX_PATH];
	std::string folder;
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, NULL, path);
	folder = std::string(path) + crypt_str("\\DoubleTap.Space\\");
	CreateDirectory(folder.c_str(), 0);



	// inv shit no kizwalds

	typedef uint32_t SteamPipeHandle;
	typedef uint32_t SteamUserHandle;
	SteamUserHandle hSteamUser = ((SteamUserHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamUser"))();
	SteamPipeHandle hSteamPipe = ((SteamPipeHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamPipe"))();
	SteamClient = ((ISteamClient * (__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamClient"))();
	SteamGameCoordinator = (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface(hSteamUser, hSteamPipe, "SteamGameCoordinator001");
	SteamUser = (ISteamUser*)SteamClient->GetISteamUser(hSteamUser, hSteamPipe, "SteamUser019");
	SteamFriends = SteamClient->GetISteamFriends(hSteamUser, hSteamPipe, "SteamFriends015");
	static auto SteamInventory = SteamClient->GetISteamInventory(hSteamUser, hSteamPipe, "STEAMINVENTORY_INTERFACE_V002");
	auto _ = SteamFriends->GetLargeFriendAvatar(SteamUser->GetSteamID());
	SteamUtils = SteamClient->GetISteamUtils(hSteamPipe, "SteamUtils009");



	kit_parser.setup();
	g_Skins.ParseSkins();

	base_address = base;


	setup_sounds();
	setup_skins();
	setup_netvars();
	setup_render();
	cfg_manager->setup();
	//c_lua::get().initialize();
	key_binds::get().initialize_key_binds();
	setup_hooks();
	Netvars::Netvars();
	return EXIT_SUCCESS;
	//bool active = false;



}


int __stdcall hGetUnverifiedFileHashes(void* _this, void* someclass, int nMaxFiles)
{
	return 0;
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		IFH(DisableThreadLibraryCalls)(hModule);
		CreateThread(nullptr, 0, main, hModule, 0, nullptr); //-V718 //-V513
	}

	return TRUE;
}
//void __fastcall hooks::hooked_CRCcheck(void* ecx, void* edx) {
	//return;
//}

__forceinline void setup_render()
{
	static auto create_font = [](const char* name, int size, int weight, DWORD flags) -> vgui::HFont
	{
		g_ctx.last_font_name = name;

		auto font = m_surface()->FontCreate();
		m_surface()->SetFontGlyphSet(font, name, size, weight, 0, 0, flags);

		return font;
	};
	fonts[LOGS] = create_font(crypt_str("Lucida Console"), 10, FW_MEDIUM, FONTFLAG_DROPSHADOW);
	fonts[ESP] = create_font(crypt_str("Lucida Console"), 11, FW_MEDIUM, FONTFLAG_OUTLINE);
	fonts[NAME] = create_font(crypt_str("Lucida Console"), 11, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[SUBTABWEAPONS] = create_font(crypt_str("undefeated"), 13, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[KNIFES] = create_font(crypt_str("icomoon"), 13, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[GRENADES] = create_font(crypt_str("undefeated"), 20, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[INDICATORFONT] = create_font(crypt_str("Verdana"), 25, FW_HEAVY, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[DAMAGE_MARKER] = create_font(crypt_str("Digital-7 Mono"), 32, FW_HEAVY, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	fonts[WATERMARK] = create_font(crypt_str("Vermin"), 10, FW_MEDIUM, FONTFLAG_DROPSHADOW);
	fonts[VELOCITY] = create_font(crypt_str("Verdana"), 35, FW_HEAVY, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[KEYBINDS] = create_font(crypt_str("Verdana"), 12, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[GRENADE_HELP] = create_font(crypt_str("CrashNumberingGothic"), 15, FW_HEAVY, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	g_ctx.last_font_name.clear();
}

__forceinline void setup_netvars()
{
	netvars::get().tables.clear();
	auto client = m_client()->GetAllClasses();

	if (!client)
		return;

	while (client)
	{
		auto recvTable = client->m_pRecvTable;

		if (recvTable)
			netvars::get().tables.emplace(std::string(client->m_pNetworkName), recvTable);

		client = client->m_pNext;
	}

}

__forceinline void setup_skins()
{
	auto items = std::ifstream(crypt_str("csgo/scripts/items/items_game_cdn.txt"));
	auto gameItems = std::string(std::istreambuf_iterator <char> { items }, std::istreambuf_iterator <char> { });

	if (!items.is_open())
		return;

	items.close();
	memory.initialize();

	for (auto i = 0; i <= memory.itemSchema()->paintKits.lastElement; i++)
	{
		auto paintKit = memory.itemSchema()->paintKits.memory[i].value;

		if (paintKit->id == 9001)
			continue;

		auto itemName = m_localize()->FindSafe(paintKit->itemName.buffer + 1);
		auto itemNameLength = WideCharToMultiByte(CP_UTF8, 0, itemName, -1, nullptr, 0, nullptr, nullptr);

		if (std::string name(itemNameLength, 0); WideCharToMultiByte(CP_UTF8, 0, itemName, -1, &name[0], itemNameLength, nullptr, nullptr))
		{
			if (paintKit->id < 10000)
			{
				if (auto pos = gameItems.find('_' + std::string{ paintKit->name.buffer } + '='); pos != std::string::npos && gameItems.substr(pos + paintKit->name.length).find('_' + std::string{ paintKit->name.buffer } + '=') == std::string::npos)
				{
					if (auto weaponName = gameItems.rfind(crypt_str("weapon_"), pos); weaponName != std::string::npos)
					{
						name.back() = ' ';
						name += '(' + gameItems.substr(weaponName + 7, pos - weaponName - 7) + ')';
					}
				}
				SkinChanger::skinKits.emplace_back(paintKit->id, std::move(name), paintKit->name.buffer);
			}
			else
			{
				std::string_view gloveName{ paintKit->name.buffer };
				name.back() = ' ';
				name += '(' + std::string{ gloveName.substr(0, gloveName.find('_')) } + ')';
				SkinChanger::gloveKits.emplace_back(paintKit->id, std::move(name), paintKit->name.buffer);
			}
		}
	}

	std::sort(SkinChanger::skinKits.begin(), SkinChanger::skinKits.end());
	std::sort(SkinChanger::gloveKits.begin(), SkinChanger::gloveKits.end());
}

class CHLClient
{
public:
	ClientClass * GetAllClasses() {
		return call_virtual< ClientClass* (__thiscall*)(decltype(this))>(this, 8)(this);
	}
};
inline CHLClient* fm_client;


class Netvars_merge {
private:
	struct NetvarData {
		bool        m_datamap_var; // we can't do proxies on stuff from datamaps :).
		RecvProp* m_prop_ptr;
		size_t      m_offset;

		__forceinline NetvarData() : m_datamap_var{}, m_prop_ptr{}, m_offset{} { }
	};

	std::unordered_map< hash32_t, int > m_client_ids;

	// netvar container.
	std::unordered_map< hash32_t,		// hash key + tables
		std::unordered_map< hash32_t,	// hash key + props
		NetvarData						// prop offset / prop ptr
		> > m_offsets;

public:
	// ctor.
	Netvars_merge() : m_offsets{} {}

	void init() {
		ClientClass* list;



		// sanity check on client.
		if (!fm_client)
			return;

		// grab linked list.
		list = fm_client->GetAllClasses();
		if (!list)
			return;

		// iterate list of netvars.
		for (; list != nullptr; list = list->m_pNext)
			StoreDataTable(list->m_pRecvTable->netTableName, list->m_pRecvTable);

		// find all datamaps
		FindAndStoreDataMaps();
	}

	// dtor.
	~Netvars_merge() {
		m_offsets.clear();
	}

	// gather all classids dynamically on runtime.
	void SetupClassData() {
		ClientClass* list;

		// clear old shit.
		m_client_ids.clear();

		// sanity check on client.
		if (!m_client())
			return;

		// grab linked list.
		list = m_client()->GetAllClasses();
		if (!list)
			return;

		// iterate list.
		for (; list != nullptr; list = list->m_pNext)
			m_client_ids[FNV1a::get(list->m_pNetworkName)] = list->m_ClassID;
	}

	void StoreDataTable(const std::string& name, RecvTable* table, size_t offset = 0) {
		hash32_t	var, base{ FNV1a::get(name) };
		RecvProp* prop;
		RecvTable* child;

		// iterate props
		for (int i{}; i < table->propCount; ++i) {
			prop = &table->props[i];
			child = prop->dataTable;

			// we have a child table, that contains props.
			if (child && child->propCount > 0)
				StoreDataTable(name, child, prop->offset + offset);

			// hash var name.
			var = FNV1a::get(prop->name);

			// insert if not present.
			if (!m_offsets[base][var].m_offset) {
				m_offsets[base][var].m_datamap_var = false;
				m_offsets[base][var].m_prop_ptr = prop;
				m_offsets[base][var].m_offset = (size_t)(prop->offset + offset);
			}
		}
	}

	// iterate client module and find all datamaps.
	void FindAndStoreDataMaps() {
		pattern::patterns_t matches{};

		matches = pattern::FindAll(PE::GetModule(HASH("client.dll")), ("C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C3 CC"));
		if (matches.empty())
			return;

		for (auto& m : matches)
			StoreDataMap(m);
	}

	void StoreDataMap(Address ptr) {
		datamap_t* map;
		hash32_t            base, var;
		typedescription_t* entry;

		// get datamap and verify.
		map = ptr.at(2).sub(4).as< datamap_t* >();

		//dataNumFields - m_size
		//dataClassName - m_name

		if (!map || !map->dataNumFields || map->dataNumFields > 200 || !map->dataDesc || !map->dataClassName)
			return;

		// hash table name.
		base = FNV1a::get(map->dataClassName);

		for (int i{}; i < map->dataNumFields; ++i) {
			entry = &map->dataDesc[i];
			if (!entry->fieldName)
				continue;

			// hash var name.
			var = FNV1a::get(entry->fieldName);

			// if we dont have this var stored yet.
			if (!m_offsets[base][var].m_offset) {
				m_offsets[base][var].m_datamap_var = true;
				m_offsets[base][var].m_offset = (size_t)entry->fieldOffset[TD_OFFSET_NORMAL];
				m_offsets[base][var].m_prop_ptr = nullptr;
			}
		}
	}

	// get client id.
	int GetClientID(hash32_t network_name) {
		return m_client_ids[network_name];
	}

	// get netvar offset.
	size_t get(hash32_t table, hash32_t prop) {
		return m_offsets[table][prop].m_offset;
	}

	// get netvar proxy.
	recvProxy GetProxy(hash32_t table, hash32_t prop, recvProxy proxy) {
		// todo - dex; should we return null here if m_is_valid_netvar is false? probably doesn't matter anyway.
		return m_offsets[table][prop].m_prop_ptr->proxy;
	}

	// set netvar proxy.
	void SetProxy(hash32_t table, hash32_t prop, void* proxy, recvProxy& original) {
		auto netvar_entry = m_offsets[table][prop];

		// we can't set a proxy on a datamap.
		if (netvar_entry.m_datamap_var)
			return;

		// save original.
		original = netvar_entry.m_prop_ptr->proxy;

		// redirect.
		netvar_entry.m_prop_ptr->proxy = (recvProxy)proxy;
	}
};

Netvars_merge g_netvars;

__forceinline void setup_hooks()
{


	//g_detours.init();
	static auto getforeignfallbackfontname = (DWORD)(util::FindSignature(crypt_str("vguimatsurface.dll"), g_ctx.signatures.at(9).c_str()));
	hooks::original_getforeignfallbackfontname = (DWORD)DetourFunction((PBYTE)getforeignfallbackfontname, (PBYTE)hooks::hooked_getforeignfallbackfontname); //-V206

	static auto setupbones = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(10).c_str()));
	hooks::original_setupbones = (DWORD)DetourFunction((PBYTE)setupbones, (PBYTE)hooks::hooked_setupbones); //-V206

	static auto doextrabonesprocessing = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(11).c_str()));
	hooks::original_doextrabonesprocessing = (DWORD)DetourFunction((PBYTE)doextrabonesprocessing, (PBYTE)hooks::hooked_doextrabonesprocessing); //-V206

	static auto standardblendingrules = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(12).c_str()));
	hooks::original_standardblendingrules = (DWORD)DetourFunction((PBYTE)standardblendingrules, (PBYTE)hooks::hooked_standardblendingrules); //-V206

	static auto updateclientsideanimation = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(13).c_str()));
	hooks::original_updateclientsideanimation = (DWORD)DetourFunction((PBYTE)updateclientsideanimation, (PBYTE)hooks::hooked_updateclientsideanimation); //-V206
	
	static auto physicssimulate = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(14).c_str()));
	hooks::original_physicssimulate = (DWORD)DetourFunction((PBYTE)physicssimulate, (PBYTE)hooks::hooked_physicssimulate);

	static auto modifyeyeposition = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(15).c_str()));
	hooks::original_modifyeyeposition = (DWORD)DetourFunction((PBYTE)modifyeyeposition, (PBYTE)hooks::hooked_modifyeyeposition);

	static auto calcviewmodelbob = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(16).c_str()));
	hooks::original_calcviewmodelbob = (DWORD)DetourFunction((PBYTE)calcviewmodelbob, (PBYTE)hooks::hooked_calcviewmodelbob);

	static auto shouldskipanimframe = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(17).c_str()));
	DetourFunction((PBYTE)shouldskipanimframe, (PBYTE)hooks::hooked_shouldskipanimframe);

	static auto checkfilecrcswithserver = (DWORD)(util::FindSignature(crypt_str("engine.dll"), g_ctx.signatures.at(18).c_str()));
	DetourFunction((PBYTE)checkfilecrcswithserver, (PBYTE)hooks::hooked_checkfilecrcswithserver);





	//auto hkCheckFileCRCsWithServer_target = (DWORD)(util::FindSignature(crypt_str("engine.dll"), g_ctx.signatures.at(18).c_str()));
	//DetourFunction((PBYTE)hkCheckFileCRCsWithServer_target, (PBYTE)hooks::hooked_hkCheckFileCRCsWithServer_target);

	auto svpure_target = reinterpret_cast<void*>(util::FindSignature(crypt_str("engine.dll"), g_ctx.signatures.at(18).c_str()));
	if (svpure_target) {
		//minhook
		svpure_target, hooks::hooked_checkfilecrcswithserver, nullptr;
		svpure_target;
	}

	static auto processinterpolatedlist = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(19).c_str()));
	hooks::original_processinterpolatedlist = (DWORD)DetourFunction((byte*)processinterpolatedlist, (byte*)hooks::processinterpolatedlist); //-V206










	
	hooks::client_hook = new vmthook(reinterpret_cast<DWORD**>(m_client()));
	hooks::client_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_fsn), 37); //-V107 //-V221
	hooks::client_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_writeusercmddeltatobuffer), 24); //-V107 //-V221

	hooks::clientstate_hook = new vmthook(reinterpret_cast<DWORD**>((CClientState*)(uint32_t(m_clientstate()) + 0x8)));
	hooks::clientstate_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_packetstart), 5); //-V107 //-V221
	hooks::clientstate_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_packetend), 6); //-V107 //-V221
	
	hooks::panel_hook = new vmthook(reinterpret_cast<DWORD**>(m_panel())); //-V1032
	hooks::panel_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_painttraverse), 41); //-V107 //-V221

	hooks::clientmode_hook = new vmthook(reinterpret_cast<DWORD**>(m_clientmode()));
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_createmove), 24); //-V107 //-V221
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_postscreeneffects), 44); //-V107 //-V221
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_overrideview), 18); //-V107 //-V221
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_drawfog), 17); //-V107 //-V221
	
	hooks::inputinternal_hook = new vmthook(reinterpret_cast<DWORD**>(m_inputinternal())); //-V114
	hooks::inputinternal_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_setkeycodestate), 91); //-V107 //-V221
	hooks::inputinternal_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_setmousecodestate), 92); //-V107 //-V221

	hooks::engine_hook = new vmthook(reinterpret_cast<DWORD**>(m_engine()));
	hooks::engine_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_isconnected), 27); //-V107 //-V221
	hooks::engine_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_getscreenaspectratio), 101); //-V107 //-V221
	hooks::engine_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_ishltv), 93); //-V107 //-V221

	hooks::renderview_hook = new vmthook(reinterpret_cast<DWORD**>(m_renderview()));
	hooks::renderview_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_sceneend), 9); //-V107 //-V221

	hooks::materialsys_hook = new vmthook(reinterpret_cast<DWORD**>(m_materialsystem())); //-V1032
	hooks::materialsys_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_beginframe), 42); //-V107 //-V221
	hooks::materialsys_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_getmaterial), 84); //-V107 //-V221

	hooks::modelrender_hook = new vmthook(reinterpret_cast<DWORD**>(m_modelrender()));
	hooks::modelrender_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_dme), 21); //-V107 //-V221
	
	hooks::surface_hook = new vmthook(reinterpret_cast<DWORD**>(m_surface()));
	hooks::surface_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_lockcursor), 67); //-V107 //-V221

	hooks::bspquery_hook = new vmthook(reinterpret_cast<DWORD**>(m_engine()->GetBSPTreeQuery()));
	hooks::bspquery_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_listleavesinbox), 6); //-V107 //-V221

	hooks::prediction_hook = new vmthook(reinterpret_cast<DWORD**>(m_prediction())); //-V1032
	hooks::prediction_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_runcommand), 19); //-V107 //-V221

	hooks::trace_hook = new vmthook(reinterpret_cast<DWORD**>(m_trace()));
	hooks::trace_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_clip_ray_collideable), 4); //-V107 //-V221
	hooks::trace_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_trace_ray), 5); //-V107 //-V221

	hooks::filesystem_hook = new vmthook(reinterpret_cast<DWORD**>(util::FindSignature(crypt_str("engine.dll"), g_ctx.signatures.at(20).c_str()) + 0x2));
	hooks::filesystem_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_loosefileallowed), 128); //-V107 //-V221
//hooks::filesystem_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_getunverifiedfilehashes), 101); //-V107 //-V221
	hooks::filesystem_hook->hook_function(reinterpret_cast<uintptr_t>(hGetUnverifiedFileHashes), 101); //-V107 //-V221

	hooks::enginesound_hook = new vmthook(reinterpret_cast<DWORD**>(m_enginesound()));
	const auto emitsound_target = reinterpret_cast<void*>(get_virtual(m_enginesound(), 5));
	MH_CreateHook(emitsound_target, &hooks::hooked_emitsound, reinterpret_cast<void**>(&hooks::oEmitSound));

	while (!(INIT::Window = IFH(FindWindow)(crypt_str("Valve001"), nullptr)))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	INIT::OldWindow = (WNDPROC)IFH(SetWindowLongPtr)(INIT::Window, GWL_WNDPROC, (LONG_PTR)hooks::Hooked_WndProc);

	hooks::directx_hook = new vmthook(reinterpret_cast<DWORD**>(m_device()));
	hooks::directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::Hooked_EndScene_Reset), 16); //-V107 //-V221
	hooks::directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_present), 17); //-V107 //-V221
	hooks::directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::Hooked_EndScene), 42); //-V107 //-V221


	static const auto sv_cheats = m_cvar()->FindVar(crypt_str("sv_cheats"));
	hooks::sv_cheats_hook = new vmthook(reinterpret_cast<DWORD**>(sv_cheats));
	hooks::sv_cheats_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_sv_cheats), 13);  //-V107 //-V221

	MH_Initialize();
	{
		const auto create_move_target = reinterpret_cast<void*>(get_virtual(m_clientmode(), 24));
		const auto postscreeneffects_target = reinterpret_cast<void*>(get_virtual(m_clientmode(), 44));
		const auto overrideview_target = reinterpret_cast<void*>(get_virtual(m_clientmode(), 18));
		const auto drawfog_target = reinterpret_cast<void*>(get_virtual(m_clientmode(), 17));
		const auto fsn_target = reinterpret_cast<void*>(get_virtual(m_client(), 37));
		const auto sendmessage_target = reinterpret_cast<void*>(get_virtual(SteamGameCoordinator, 0));
		const auto retrievemessage_target = reinterpret_cast<void*>(get_virtual(SteamGameCoordinator, 2));
		const auto gameventmanag_target = reinterpret_cast<void*>(get_virtual(m_eventmanager(), 9));
		const auto gamemovement_target = reinterpret_cast<void*>(get_virtual(m_gamemovement(), 1));
		const auto dispatch_target = reinterpret_cast<void*>(get_virtual(m_client(), 38));
		{
			MH_CreateHook(sendmessage_target, &shonax_hooks::GCSendMessageHook, reinterpret_cast<void**>(&shonax_hooks::oGCSendMessage));
			MH_CreateHook(retrievemessage_target, &shonax_hooks::GCRetrieveMessageHook, reinterpret_cast<void**>(&shonax_hooks::oGCRetrieveMessage));
			MH_CreateHook(gameventmanag_target, &shonax_hooks::FireEventClientSide, reinterpret_cast<void**>(&shonax_hooks::oFireEventClientSidefn));
			//MH_CreateHook(gamemovement_target, &ProcessMovement, reinterpret_cast<void**>(&oProcessMovement));
			//MH_CreateHook(dispatch_target, &hkDispatchUserMessage, reinterpret_cast<void**>(&otDispatchUserMessage));
		}
	}
	MH_EnableHook(MH_ALL_HOOKS);


	//MH_EnableHook(MH_ALL_HOOKS);

	hooks::hooked_events.RegisterSelf();
}