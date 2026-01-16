#pragma once
#define Lua_Function(name) int ll_##name(lua_State* L)
#define luaL_wingetbycheckudata(L, idx, type) \
    *(type*)luaL_checkuserdata(L, idx)
#define registerNewStruct(L, clazz) registerNew<clazz>(L, #clazz)
#define pushWindowStruct(L,clazz, val) pushUdataWithMetatable<clazz>(L, #clazz, val)
#define INIT_LUAOPEN() extern "C" __declspec(dllexport) int luaopen_lwindows(lua_State* L){
#define END_LUAOPEN() return 0;}
#define REGSTRUCT(T) registerNewStruct(L, ##T##);
#define REGVALUEMACRO(name,type) (lua_push##type(L, ##name##), lua_setglobal(L, #name));
#define REGIMACRO(N) REGVALUEMACRO(##N##, integer)
#define REGSMACRO(S) REGVALUEMACRO(##S##, string)
#define REGUMACRO(T, v) (pushWindowStruct(L,##T##,##v##), lua_setglobal(L, #v));
#define INIT_WPR() luaL_Reg winapi_reg[] = {
#define ADD2WPR(name) {#name, ll_##name},
#define END_WPR() {NULL,NULL}};REGISTER_WPR()
#define REGISTER_WPR() (lua_pushglobaltable(L),luaL_setfuncs(L, winapi_reg, 0));
#define REGISTERINH(name) Lua_Function(##name##);
#define luaL_checkuserdata(L, idx) \
    (luaL_checktype(L, idx, LUA_TUSERDATA), lua_touserdata(L, idx))
#define luaL_checklightuserdata(L, idx) \
    (luaL_checktype(L, idx, LUA_TLIGHTUSERDATA), lua_touserdata(L, idx))
#define luaL_wingetbyudata(L, idx, type) ([](lua_State* L_, int i) -> type { \
    void* p = lua_touserdata(L_, i);                                       \
    return p ? *(type*)p : NULL;                                           \
})(L, idx)
template <typename T>
inline void pushUdataWithMetatable(lua_State* L, const char* metaname, const T& val)
{
    T* userdata = (T*)lua_newuserdata(L, sizeof(T));
    *userdata = val;
    int idx = lua_absindex(L, -1);

    if (!lua_getmetatable(L, idx)) {
        lua_newtable(L);
        lua_pushvalue(L, -1);
        lua_setmetatable(L, idx);
    }
    int metai = lua_absindex(L, -1);
    lua_pushstring(L, "__name");
    lua_pushstring(L, metaname);
    lua_rawset(L, metai);
    lua_pushstring(L, "__index");
    lua_pushcfunction(L, [](lua_State* L) -> int {
        void** p = (void**)luaL_checkuserdata(L, 1);
        std::string key = luaL_checkstring(L, 2);
        if (key == "addr") {
            lua_pushlightuserdata(L, *p);
            return 1;
        }
        lua_pushnil(L);
        return 1;
        });
    lua_rawset(L, metai);
    lua_pushstring(L, "__eq");
    lua_pushcfunction(L, [](lua_State* L) -> int {
        void* addr = *(void**)luaL_checkuserdata(L, 1);
        void* addr2 = *(void**)luaL_checkuserdata(L, 2);
        lua_pushboolean(L, addr == addr2);
        return 1;
        });
    lua_rawset(L, metai);
    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, [](lua_State* L) -> int {
        void* p = lua_touserdata(L, 1);
        if (lua_type(L, 2) == LUA_TNUMBER)
        {
            lua_Integer idx = lua_tointeger(L, 2);

            if (lua_type(L, 3) == LUA_TNUMBER)
            {
                lua_Integer val = lua_tointeger(L, 3);
                ((char*)p)[idx] = (char)val;
            }
            else if (lua_type(L, 3) == LUA_TSTRING)
            {
                size_t len;
                const char* s = lua_tolstring(L, 3, &len);
                if (len == 1)
                {
                    ((char*)p)[idx] = s[0];
                }
                else
                {
                    return luaL_error(L, "Value string must be length 1");
                }
            }
            else
            {
                return luaL_error(L, "Value must be number or a char");
            }
        }
        return 0;
        });
    lua_rawset(L, metai);
    lua_pop(L, 1);
}

template <typename T>
inline void registerNew(lua_State* L, const char* name)
{
    lua_newtable(L);
    lua_pushstring(L, name);
    lua_pushcclosure(L, [](lua_State* L) -> int {
        pushUdataWithMetatable<T>(L, lua_tostring(L, lua_upvalueindex(1)), T{});
        return 1;
        }, 1);
    lua_setfield(L, -2, "new");
    lua_setglobal(L, name);
}
bool removeLWinProc(lua_State* L, lua_Integer i, const std::string& regName);
bool getLWinProci(lua_State* L, lua_Integer i, const std::string& regName);
int addLWinProc(lua_State* L, int index, const std::string& regName);
void table_toLPRECT(lua_State* L, int index, LPRECT prc);
void LPRECT_to_table(lua_State* L, const LPRECT prc, int tindex = 0);
SECURITY_ATTRIBUTES table2SECURITY_ATTRIBUTES(lua_State* L, int index);
void SECURITY_ATTRIBUTTES2table(lua_State* L, int index, const SECURITY_ATTRIBUTES& sa);