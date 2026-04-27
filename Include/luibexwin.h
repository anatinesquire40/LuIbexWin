#pragma once
#include "includes.h"
void register_window_funcs(lua_State* L);
void register_all_wm_messages(lua_State* L);
void register_cursor(lua_State* L);
void register_icon(lua_State* L);
void register_style(lua_State* L);
void register_brush(lua_State* L);
void register_sound(lua_State* L);
void register_vkeys(lua_State* L);
void register_commcontrol(lua_State* L);
void register_explorerutills(lua_State* L);
void register_winmemory(lua_State* L);
void register_commonwinutils(lua_State* L);
void register_render(lua_State* L);
void register_filesystem(lua_State* L);