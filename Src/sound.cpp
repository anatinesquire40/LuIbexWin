#pragma comment(lib, "winmm.lib")
Lua_Function(MessageBeep)
{
    UINT uType = (UINT)luaL_checkinteger(L, 1); // MB_OK, MB_ICONEXCLAMATION, etc.
    ::MessageBeep(uType);
    return 0; // nada que devolver a Lua
}

Lua_Function(Beep)
{
    DWORD freq = (DWORD)luaL_checkinteger(L, 1);      // frecuencia en Hz
    DWORD duration = (DWORD)luaL_checkinteger(L, 2);  // duración en ms
    ::Beep(freq, duration);
    return 0;
}

Lua_Function(PlaySound)
{
    const char* filename = luaL_checkstring(L, 1);
    HMODULE mod = luaL_wingetbyudata(L, 2, HMODULE);
    UINT flags = (UINT)luaL_optinteger(L, 3, SND_FILENAME | SND_ASYNC);
    ::PlaySoundA(filename, mod, flags);
    return 0;
}
void register_sound(lua_State* L)
{
    REGIMACRO(SND_SYNC)
    REGIMACRO(SND_ASYNC)
    REGIMACRO(SND_NODEFAULT)
    REGIMACRO(SND_MEMORY)
    REGIMACRO(SND_LOOP)
    REGIMACRO(SND_NOSTOP)
    REGIMACRO(SND_NOWAIT)
    REGIMACRO(SND_ALIAS)
    REGIMACRO(SND_ALIAS_ID)
    REGIMACRO(SND_FILENAME)
    REGIMACRO(SND_RESOURCE)
    REGIMACRO(SND_PURGE)
    REGIMACRO(SND_APPLICATION)
    REGIMACRO(SND_SYSTEM)
    REGIMACRO(SND_RING)
}