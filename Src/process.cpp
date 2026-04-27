#pragma warning(disable: 6335) // pi.hProcess and pi.hThread will get closed in Lua
// Convierte una tabla Lua en STARTUPINFOA
static STARTUPINFOA table2STARTUPINFOA(lua_State* L, int index)
{
    index = lua_absindex(L, index);
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
	if (lua_isnoneornil(L, index))
        return si;
    lua_getfield(L, index, "cb");
    si.cb = (DWORD)luaL_optinteger(L, -1, si.cb);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwFlags");
    si.dwFlags = (DWORD)luaL_optinteger(L, -1, si.dwFlags);
    lua_pop(L, 1);

    lua_getfield(L, index, "wShowWindow");
    si.wShowWindow = (WORD)luaL_optinteger(L, -1, si.wShowWindow);
    lua_pop(L, 1);

    lua_getfield(L, index, "lpReserved");
    si.lpReserved = (LPSTR)luaL_optstring(L, -1, si.lpReserved);
    lua_pop(L, 1);

    lua_getfield(L, index, "lpDesktop");
    si.lpDesktop = (LPSTR)luaL_optstring(L, -1, si.lpDesktop);
    lua_pop(L, 1);

    lua_getfield(L, index, "lpTitle");
    si.lpTitle = (LPSTR)luaL_optstring(L, -1, si.lpTitle);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwX");
    si.dwX = (DWORD)luaL_optinteger(L, -1, si.dwX);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwY");
    si.dwY = (DWORD)luaL_optinteger(L, -1, si.dwY);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwXSize");
    si.dwXSize = (DWORD)luaL_optinteger(L, -1, si.dwXSize);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwYSize");
    si.dwYSize = (DWORD)luaL_optinteger(L, -1, si.dwYSize);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwXCountChars");
    si.dwXCountChars = (DWORD)luaL_optinteger(L, -1, si.dwXCountChars);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwYCountChars");
    si.dwYCountChars = (DWORD)luaL_optinteger(L, -1, si.dwYCountChars);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwFillAttribute");
    si.dwFillAttribute = (DWORD)luaL_optinteger(L, -1, si.dwFillAttribute);
    lua_pop(L, 1);

    lua_getfield(L, index, "cbReserved2");
    si.cbReserved2 = (DWORD)luaL_optinteger(L, -1, si.cbReserved2);
    lua_pop(L, 1);

    lua_getfield(L, index, "lpReserved2");
    si.lpReserved2 = (LPBYTE)lua_touserdata(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "hStdInput");
    si.hStdInput = luaL_wingetbyudata(L, -1, HANDLE);
    lua_pop(L, 1);

    lua_getfield(L, index, "hStdOutput");
    si.hStdOutput = luaL_wingetbyudata(L, -1, HANDLE);
    lua_pop(L, 1);

    lua_getfield(L, index, "hStdError");
    si.hStdError = luaL_wingetbyudata(L, -1, HANDLE);
    lua_pop(L, 1);

    return si;
}


// Convierte una tabla Lua en SECURITY_ATTRIBUTES
SECURITY_ATTRIBUTES table2SECURITY_ATTRIBUTES(lua_State* L, int index)
{
    index = lua_absindex(L, index);
    SECURITY_ATTRIBUTES sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = TRUE;

    // bInheritHandle
    lua_getfield(L, index, "bInheritHandle");
    if (!lua_isnil(L, -1))
        sa.bInheritHandle = lua_toboolean(L, -1);
    lua_pop(L, 1);

    // lpSecurityDescriptor
    lua_getfield(L, index, "lpSecurityDescriptor");
    if (!lua_isnil(L, -1))
        sa.lpSecurityDescriptor = lua_touserdata(L, -1);
    lua_pop(L, 1);

    return sa;
}
void SECURITY_ATTRIBUTTES2table(lua_State* L, int index, const SECURITY_ATTRIBUTES& sa)
{
    index = lua_absindex(L, index);
    lua_pushboolean(L, sa.bInheritHandle);
    lua_setfield(L, index, "bInheritHandle");
    lua_pushlightuserdata(L, sa.lpSecurityDescriptor);
    lua_setfield(L, index, "lpSecurityDescriptor");
}

// Llena una tabla Lua con los campos de PROCESS_INFORMATION
static void table_fillPROCESS_INFORMATION(lua_State* L, int index, const PROCESS_INFORMATION& pi)
{
    index = lua_absindex(L, index);

    lua_pushinteger(L, (lua_Integer)pi.dwProcessId);
    lua_setfield(L, index, "dwProcessId");

    lua_pushinteger(L, (lua_Integer)pi.dwThreadId);
    lua_setfield(L, index, "dwThreadId");

    pushWindowStruct(L, HANDLE, pi.hProcess);
    lua_setfield(L, index, "hProcess");

    pushWindowStruct(L, HANDLE, pi.hThread);
    lua_setfield(L, index, "hThread");
}

Lua_Function(CreateProcess)
{
    LPCSTR lpApp = lua_tostring(L, 1);
    LPSTR lpCmd = (LPSTR)luaL_optstring(L, 2, nullptr);
    auto lpProcAttr = table2SECURITY_ATTRIBUTES(L, 3);
    auto lpThreadAttr = table2SECURITY_ATTRIBUTES(L, 4);
    BOOL inherit = lua_toboolean(L, 5);
    DWORD flags = (DWORD)luaL_checkinteger(L, 6);
    LPVOID env = lua_touserdata(L, 7);
    LPCSTR cwd = luaL_optstring(L, 8, nullptr);
    STARTUPINFOA si = table2STARTUPINFOA(L, 9);
    luaL_checktype(L, 10, LUA_TTABLE);
    PROCESS_INFORMATION pi;
    BOOL success = ::CreateProcessA(lpApp, lpCmd, &lpProcAttr, &lpThreadAttr, inherit, flags, env, cwd, &si, &pi);
    table_fillPROCESS_INFORMATION(L, 10, pi);

    lua_pushboolean(L, success);
    return 1;
}

Lua_Function(TerminateProcess)
{
    HANDLE hProcess = luaL_wingetbycheckudata(L, 1, HANDLE);
    UINT exitCode = (UINT)luaL_checkinteger(L, 2);
    BOOL success = ::TerminateProcess(hProcess, exitCode);
    lua_pushboolean(L, success);
	return 1;
}
