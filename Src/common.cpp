Lua_Function(SleepEx)
{
    DWORD milliseconds = (DWORD)luaL_checkinteger(L, 1);
    BOOL alertable = lua_isboolean(L,2) ? lua_toboolean(L, 2) : FALSE;
    DWORD result = SleepEx(milliseconds, alertable);
    lua_pushinteger(L, (lua_Integer)result);
    return 1;
}
Lua_Function(GetCurrentProcess)
{
    HANDLE hProc = GetCurrentProcess();
    pushWindowStruct(L, HANDLE, hProc);
    return 1;
}
Lua_Function(GetSystemMetrics)
{
    int index = (int)luaL_checkinteger(L, 1);
    int value = GetSystemMetrics(index);
    lua_pushinteger(L, value);
    return 1;
}
Lua_Function(GetCurrentProcessId)
{
    DWORD pid = GetCurrentProcessId();
    lua_pushinteger(L, pid);
    return 1;
}

Lua_Function(GetCurrentProcessToken)
{
    HANDLE token = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        pushWindowStruct(L, HANDLE, token);
        return 1;
    }
    return luaL_error(L, "Failed to get process token");
}

Lua_Function(GetCurrentThread)
{
    HANDLE hThread = GetCurrentThread();
    pushWindowStruct(L, HANDLE, hThread);
    return 1;
}

Lua_Function(GetCurrentThreadId)
{
    DWORD tid = GetCurrentThreadId();
    lua_pushinteger(L, tid);
    return 1;
}

Lua_Function(GetCurrentThreadToken)
{
    HANDLE token = NULL;
    if (OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &token)) {
        pushWindowStruct(L, HANDLE, token);
        return 1;
    }
    return luaL_error(L, "Failed to get thread token");
}

// Requiere Windows 8+
Lua_Function(GetCurrentThreadStackLimits)
{
    ULONG_PTR low = 0, high = 0;
    GetCurrentThreadStackLimits(&low, &high);
    lua_pushinteger(L, (lua_Integer)low);
    lua_pushinteger(L, (lua_Integer)high);
    return 2;
}
Lua_Function(GetDoubleClickTime)
{
    UINT time = GetDoubleClickTime();
    lua_pushinteger(L, time);
    return 1;
}
Lua_Function(GetCurrentThreadEffectiveToken)
{
    HANDLE token = GetCurrentThreadEffectiveToken();
    if (!token)
        return luaL_error(L, "Failed to get effective token");
    pushWindowStruct(L, HANDLE, token);
    return 1;
}

Lua_Function(GetCurrentDirectory)
{
    DWORD len = GetCurrentDirectoryA(0, NULL);
    std::string buffer(len, 0);
    GetCurrentDirectoryA(len, &buffer[0]);
    lua_pushstring(L, buffer.c_str());
    return 1;
}
Lua_Function(GetTickCount)
{
#ifdef _WIN64
    lua_pushinteger(L, (lua_Integer)GetTickCount64());
#else
    lua_pushinteger(L, (lua_Integer)GetTickCount());
#endif
    return 1;
}
Lua_Function(GetCurrentConsoleFontEx)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX font = { sizeof(font) };
    if (!GetCurrentConsoleFontEx(hOut, FALSE, &font))
        return luaL_error(L, "Failed to get console font");

    lua_newtable(L);
    lua_pushstring(L, "FontFamily"); lua_pushinteger(L, font.FontFamily); lua_settable(L, -3);
    lua_pushstring(L, "FontWeight"); lua_pushinteger(L, font.FontWeight); lua_settable(L, -3);
    lua_pushstring(L, "FontSizeX"); lua_pushinteger(L, font.dwFontSize.X); lua_settable(L, -3);
    lua_pushstring(L, "FontSizeY"); lua_pushinteger(L, font.dwFontSize.Y); lua_settable(L, -3);
    lua_pushstring(L, "FaceName"); lua_pushstring(L, (char*)font.FaceName); lua_settable(L, -3);
    return 1;
}

Lua_Function(GetCurrentActCtx)
{
    HANDLE hActCtx = NULL;
    BOOL result = GetCurrentActCtx(&hActCtx);
    pushWindowStruct(L, HANDLE, hActCtx);
    lua_pushboolean(L, result);
    return 2;
}

Lua_Function(GetCurrentHwProfile)
{
    HW_PROFILE_INFOA info;
    if (!GetCurrentHwProfileA(&info))
        return luaL_error(L, "Failed to get HW profile");

    lua_newtable(L);
    lua_pushstring(L, "DockInfo"); lua_pushinteger(L, info.dwDockInfo); lua_settable(L, -3);
    lua_pushstring(L, "HwProfileGuid"); lua_pushstring(L, info.szHwProfileGuid); lua_settable(L, -3);
    lua_pushstring(L, "HwProfileName"); lua_pushstring(L, info.szHwProfileName); lua_settable(L, -3);
    return 1;
}

Lua_Function(GetCurrentPositionEx)
{
    HDC hdc = luaL_wingetbyudata(L, 1, HDC);
    POINT pt;
    if (!GetCurrentPositionEx(hdc, &pt))
        return luaL_error(L, "Failed to get current position");
    lua_pushinteger(L, pt.x);
    lua_pushinteger(L, pt.y);
    return 2;
}

Lua_Function(GetCurrentObject)
{
    HDC hdc = luaL_wingetbyudata(L, 1, HDC);
    UINT objType = (UINT)luaL_checkinteger(L, 2); 
    HGDIOBJ obj = GetCurrentObject(hdc, objType);
    if (!obj)
        return luaL_error(L, "Failed to get current object");
    pushWindowStruct(L, HGDIOBJ, obj);
    return 1;
}
Lua_Function(IsZoomed)
{
    HWND hWnd = luaL_wingetbyudata(L, 1, HWND);
    BOOL zoomed = IsZoomed(hWnd);
    lua_pushboolean(L, zoomed);
    return 1;
}