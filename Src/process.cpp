#pragma warning(disable: 6335) // pi.hProcess and pi.hThread will get closed in Lua
// Convierte una tabla Lua en STARTUPINFOA
static void table2STARTUPINFOA(lua_State* L, int index, LPSTARTUPINFOA lpsi)
{
    index = lua_absindex(L, index);
    ZeroMemory(lpsi, sizeof(*lpsi));
    lpsi->cb = sizeof(*lpsi);
	if (lua_isnoneornil(L, index))
        return;
    lua_getfield(L, index, "cb");
    lpsi->cb = (DWORD)luaL_optinteger(L, -1, lpsi->cb);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwFlags");
    lpsi->dwFlags = (DWORD)luaL_optinteger(L, -1, lpsi->dwFlags);
    lua_pop(L, 1);

    lua_getfield(L, index, "wShowWindow");
    lpsi->wShowWindow = (WORD)luaL_optinteger(L, -1, lpsi->wShowWindow);
    lua_pop(L, 1);

    lua_getfield(L, index, "lpReserved");
    lpsi->lpReserved = (LPSTR)luaL_optstring(L, -1, lpsi->lpReserved);
    lua_pop(L, 1);

    lua_getfield(L, index, "lpDesktop");
    lpsi->lpDesktop = (LPSTR)luaL_optstring(L, -1, lpsi->lpDesktop);
    lua_pop(L, 1);

    lua_getfield(L, index, "lpTitle");
    lpsi->lpTitle = (LPSTR)luaL_optstring(L, -1, lpsi->lpTitle);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwX");
    lpsi->dwX = (DWORD)luaL_optinteger(L, -1, lpsi->dwX);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwY");
    lpsi->dwY = (DWORD)luaL_optinteger(L, -1, lpsi->dwY);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwXSize");
    lpsi->dwXSize = (DWORD)luaL_optinteger(L, -1, lpsi->dwXSize);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwYSize");
    lpsi->dwYSize = (DWORD)luaL_optinteger(L, -1, lpsi->dwYSize);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwXCountChars");
    lpsi->dwXCountChars = (DWORD)luaL_optinteger(L, -1, lpsi->dwXCountChars);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwYCountChars");
    lpsi->dwYCountChars = (DWORD)luaL_optinteger(L, -1, lpsi->dwYCountChars);
    lua_pop(L, 1);

    lua_getfield(L, index, "dwFillAttribute");
    lpsi->dwFillAttribute = (DWORD)luaL_optinteger(L, -1, lpsi->dwFillAttribute);
    lua_pop(L, 1);

    lua_getfield(L, index, "cbReserved2");
    lpsi->cbReserved2 = (DWORD)luaL_optinteger(L, -1, lpsi->cbReserved2);
    lua_pop(L, 1);

    lua_getfield(L, index, "lpReserved2");
    lpsi->lpReserved2 = (LPBYTE)lua_touserdata(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "hStdInput");
    lpsi->hStdInput = luaL_wingetbyudata(L, -1, HANDLE);
    lua_pop(L, 1);

    lua_getfield(L, index, "hStdOutput");
    lpsi->hStdOutput = luaL_wingetbyudata(L, -1, HANDLE);
    lua_pop(L, 1);

    lua_getfield(L, index, "hStdError");
    lpsi->hStdError = luaL_wingetbyudata(L, -1, HANDLE);
    lua_pop(L, 1);
}


// Convierte una tabla Lua en SECURITY_ATTRIBUTES
void table2SECURITY_ATTRIBUTES(lua_State* L, int index, LPSECURITY_ATTRIBUTES sa)
{
    index = lua_absindex(L, index);
    ZeroMemory(sa, sizeof(*sa));
    sa->nLength = sizeof(*sa);
    sa->lpSecurityDescriptor = nullptr;
    sa->bInheritHandle = TRUE;

    // bInheritHandle
    lua_getfield(L, index, "bInheritHandle");
    if (!lua_isnil(L, -1))
        sa->bInheritHandle = lua_toboolean(L, -1);
    lua_pop(L, 1);

    // lpSecurityDescriptor
    lua_getfield(L, index, "lpSecurityDescriptor");
    if (!lua_isnil(L, -1))
        sa->lpSecurityDescriptor = lua_touserdata(L, -1);
    lua_pop(L, 1);
}
void SECURITY_ATTRIBUTTES2table(lua_State* L, int index, LPSECURITY_ATTRIBUTES sa)
{
    index = lua_absindex(L, index);
    lua_pushboolean(L, sa->bInheritHandle);
    lua_setfield(L, index, "bInheritHandle");
    lua_pushlightuserdata(L, sa->lpSecurityDescriptor);
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
    LPCSTR lpApp = lua_isstring(L, 1) ? lua_tostring(L, 1) : nullptr;
    LPSTR lpCmd = (LPSTR)luaL_optstring(L, 2, nullptr);

    LPSECURITY_ATTRIBUTES lpProcAttr = nullptr;
    LPSECURITY_ATTRIBUTES lpThreadAttr = nullptr;

    LPSTARTUPINFOA lpsi = nullptr;
    LPPROCESS_INFORMATION lppi = nullptr;
    // lpProcessAttributes
    if (lua_isnil(L, 3)) {
        lpProcAttr = nullptr;
    }
    else if (lua_istable(L, 3)) {
        lpProcAttr = new SECURITY_ATTRIBUTES{};
        table2SECURITY_ATTRIBUTES(L, 3, lpProcAttr);
    }
    else if (lua_isuserdata(L, 3)) {
        lpProcAttr = (SECURITY_ATTRIBUTES*)lua_touserdata(L, 3);
    }
    else {
        return luaL_error(L, "lpProcessAttributes must be table, userdata or nil");
    }
    // lpThreadAttributes
    if (lua_isnil(L, 4)) {
        lpThreadAttr = nullptr;
    }
    else if (lua_istable(L, 4)) {
        lpThreadAttr = new SECURITY_ATTRIBUTES{};
		table2SECURITY_ATTRIBUTES(L, 4, lpThreadAttr);
    }
    else if (lua_isuserdata(L, 4)) {
        lpThreadAttr = (SECURITY_ATTRIBUTES*)lua_touserdata(L, 4);
    }
    else {
        return luaL_error(L, "lpThreadAttributes must be table, userdata or nil");
    }
    BOOL inherit = lua_toboolean(L, 5);
    DWORD flags = (DWORD)luaL_checkinteger(L, 6);

    LPVOID env = lua_touserdata(L, 7);

    LPCSTR cwd = luaL_optstring(L, 8, nullptr);
    // STARTUPINFOA
    if (lua_istable(L, 9)) {
		lpsi = new STARTUPINFOA{};
        table2STARTUPINFOA(L, 9, lpsi);
    }
    else if (lua_isuserdata(L, 9)) {
        lpsi = (LPSTARTUPINFOA)lua_touserdata(L, 9);
    }
    else {
        return luaL_error(L, "STARTUPINFO must be table or userdata");
    }

    if (lua_istable(L, 10)) {
        lppi = new PROCESS_INFORMATION{};
    }
    else if (lua_isuserdata(L, 10)) {
        lppi = (LPPROCESS_INFORMATION)lua_touserdata(L, 9);
    }
    else {
        return luaL_error(L, "PROCESS_INFORMATION must be table or userdata");
    }
    BOOL success = ::CreateProcessA(
        lpApp,
        lpCmd,
        lpProcAttr,
        lpThreadAttr,
        inherit,
        flags,
        env,
        cwd,
        lpsi,
        lppi
    );
	if (lua_istable(L, 9))
    {
        delete lpsi;
    }
    if (lua_istable(L, 10))
    {
        table_fillPROCESS_INFORMATION(L, 10, *lppi);
        delete lppi;
    }
    if (lua_istable(L, 3))
    {
        delete lpProcAttr;
    }
    if (lua_istable(L, 4))
    {
        delete lpThreadAttr;
    }
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
