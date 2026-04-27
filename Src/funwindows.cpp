Lua_Function(MessageBoxEx)
{
    HWND hWnd = luaL_wingetbyudata(L, 1, HWND);
    LPCSTR lpText = (LPCSTR)luaL_checkstring(L, 2);
    LPCSTR lpCaption = (LPCSTR)luaL_checkstring(L, 3);
    UINT uType = (UINT)luaL_checkinteger(L, 4);
    WORD wLanguageId = (WORD)lua_tonumber(L, 5);
    lua_pushinteger(L, MessageBoxExA(hWnd, lpText, lpCaption, uType, wLanguageId));
    return 1;
}
Lua_Function(CreateWindowEx)
{
    DWORD dwExStyle = (DWORD)lua_tointeger(L, 1);
    LPCSTR lpClassName = luaL_checkstring(L, 2);
    LPCSTR lpWindowName = lua_tostring(L, 3);
    DWORD dwStyle = (DWORD)lua_tointeger(L, 4);
    int X = (int)lua_tointeger(L, 5);
    int Y = (int)lua_tointeger(L, 6);
    int nWidth = (int)lua_tointeger(L, 7);
    int nHeight = (int)lua_tointeger(L, 8);
    HWND hWndParent = luaL_wingetbyudata(L, 9, HWND);
    HMENU hMenu = (HMENU)lua_tointeger(L, 10);
    HINSTANCE hInstance = luaL_wingetbycheckudata(L, 11, HINSTANCE);
    LPVOID lpParam = luaL_wingetbyudata(L, 12, LPVOID);
    HWND hwnd = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle,
        X, Y, nWidth, nHeight, hWndParent, hMenu,
        hInstance, lpParam);
    pushWindowStruct(L, HWND, hwnd);
    return 1;
}
Lua_Function(ShowWindow)
{

    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    int nCmdShow = (int)lua_tointeger(L, 2);
    BOOL res = ShowWindow(hwnd, nCmdShow);
    lua_pushboolean(L, res);
    return 1;
}
Lua_Function(DestroyWindow)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    BOOL res = DestroyWindow(hwnd);
    lua_pushboolean(L, res);
    return 1;
}
Lua_Function(UpdateWindow)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    BOOL res = UpdateWindow(hwnd);
    lua_pushboolean(L, res);
    return 1;
}
#define GetXXXWindow(name)                          \
Lua_Function(Get##name##Window)                     \
    {                                               \
    pushWindowStruct(L, HWND, Get##name##Window()); \
    return 1;                                       \
}
GetXXXWindow(Active)
GetXXXWindow(Console)
Lua_Function(SetWindowText)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    LPCSTR lpString = luaL_checkstring(L, 2);
    BOOL result = SetWindowTextA(hwnd, lpString);
    lua_pushboolean(L, result);
    return 1;
}

Lua_Function(GetWindowText)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    int maxCount = (int)luaL_optinteger(L, 2, 1024);
    std::string buffer(maxCount, '\0');
    int len = GetWindowTextA(hwnd, &buffer[0], maxCount);
    buffer.resize(len);
    lua_pushstring(L, buffer.c_str());
    return 1;
}

Lua_Function(SetWindowPos)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    HWND hWndInsertAfter = luaL_wingetbyudata(L, 2, HWND);
    int X = (int)luaL_checkinteger(L, 3);
    int Y = (int)luaL_checkinteger(L, 4);
    int cx = (int)luaL_checkinteger(L, 5);
    int cy = (int)luaL_checkinteger(L, 6);
    UINT uFlags = (UINT)luaL_checkinteger(L, 7);
    BOOL result = SetWindowPos(hwnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
    lua_pushboolean(L, result);
    return 1;
}

Lua_Function(GetWindowRect)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
	luaL_checktype(L, 2, LUA_TTABLE);
    RECT rc;
    BOOL result = GetWindowRect(hwnd, &rc);
    lua_pushboolean(L, result);
    if (result) {
		LPRECT_to_table(L, &rc, 2);
    }
    return 1;
}

Lua_Function(MoveWindow)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    int X = (int)luaL_checkinteger(L, 2);
    int Y = (int)luaL_checkinteger(L, 3);
    int nWidth = (int)luaL_checkinteger(L, 4);
    int nHeight = (int)luaL_checkinteger(L, 5);
    BOOL bRepaint = lua_toboolean(L, 6);
    BOOL result = MoveWindow(hwnd, X, Y, nWidth, nHeight, bRepaint);
    lua_pushboolean(L, result);
    return 1;
}

Lua_Function(IsWindowVisible)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    BOOL result = IsWindowVisible(hwnd);
    lua_pushboolean(L, result);
    return 1;
}

Lua_Function(EnableWindow)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    BOOL bEnable = lua_toboolean(L, 2);
    BOOL result = EnableWindow(hwnd, bEnable);
    lua_pushboolean(L, result);
    return 1;
}

Lua_Function(SetFocus)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    HWND result = SetFocus(hwnd);
    pushWindowStruct(L, HWND, result);
    return 1;
}

Lua_Function(GetFocus)
{
    HWND result = GetFocus();
    pushWindowStruct(L, HWND, result);
    return 1;
}
Lua_Function(GetClientRect)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    RECT rc;
    BOOL result = GetClientRect(hwnd, &rc);
	luaL_checktype(L, 2, LUA_TTABLE);
	LPRECT_to_table(L,&rc, 2);
    lua_pushboolean(L, result);
    return 1;
}
Lua_Function(SendMessage)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    UINT msg = (UINT)luaL_checkinteger(L, 2);
    WPARAM wParam = (WPARAM)luaL_optinteger(L, 3, 0);
    LPARAM lParam = (LPARAM)luaL_optinteger(L, 4, 0);
    LRESULT result = SendMessageA(hwnd, msg, wParam, lParam);
    lua_pushinteger(L, (lua_Integer)result);
    return 1;
}
Lua_Function(SetParent)
{
    HWND hwndChild = luaL_wingetbycheckudata(L, 1, HWND);
    HWND hwndNewParent = luaL_wingetbycheckudata(L, 2, HWND);
    HWND result = SetParent(hwndChild, hwndNewParent);
    pushWindowStruct(L, HWND, result);
    return 1;
}
Lua_Function(GetParent)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    HWND result = GetParent(hwnd);
    pushWindowStruct(L, HWND, result);
    return 1;
}   
Lua_Function(GetWindowLong)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    int nIndex = (int)luaL_checkinteger(L, 2);
    LONG result = GetWindowLongA(hwnd, nIndex);
    lua_pushinteger(L, (lua_Integer)result);
    return 1;
}
Lua_Function(SetWindowLong)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    int nIndex = (int)luaL_checkinteger(L, 2);
    LONG dwNewLong = (LONG)luaL_checkinteger(L, 3);
    LONG result = SetWindowLongA(hwnd, nIndex, dwNewLong);
    lua_pushinteger(L, (lua_Integer)result);
    return 1;
}
Lua_Function(GetWindowLongPtr)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    int nIndex = (int)luaL_checkinteger(L, 2);
    LONG_PTR result = GetWindowLongPtrA(hwnd, nIndex);
    lua_pushinteger(L, (lua_Integer)result);
    return 1;
}
Lua_Function(SetWindowLongPtr)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    int nIndex = (int)luaL_checkinteger(L, 2);
    LONG_PTR dwNewLongPtr = (LONG_PTR)luaL_checkinteger(L, 3);
    LONG_PTR result = SetWindowLongPtrA(hwnd, nIndex, dwNewLongPtr);
    lua_pushinteger(L, (lua_Integer)result);
    return 1;
}
Lua_Function(GetClassName)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    size_t sizebuffer;
	char* buffer = (char*)lua_tolstring(L, 2, &sizebuffer);
    lua_Integer len = (lua_Integer)GetClassNameA(hwnd, buffer, sizebuffer);
	lua_pushinteger(L, len);
    return 1;
}
Lua_Function(IsWindow)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    BOOL result = IsWindow(hwnd);
    lua_pushboolean(L, result);
    return 1;
}
Lua_Function(SetForegroundWindow)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    BOOL result = SetForegroundWindow(hwnd);
    lua_pushboolean(L, result);
    return 1;
}
Lua_Function(GetForegroundWindow)
{
    HWND result = GetForegroundWindow();
    pushWindowStruct(L, HWND, result);
    return 1;
}
static void pushwindowplacement(lua_State* L, int tablei, const WINDOWPLACEMENT& wp)
{
	tablei = lua_absindex(L, tablei);

    lua_pushinteger(L, wp.flags);
    lua_setfield(L, tablei, "flags");

    lua_pushinteger(L, wp.showCmd);
    lua_setfield(L, tablei, "showCmd");

    lua_newtable(L);
    lua_pushinteger(L, wp.ptMinPosition.x);
    lua_setfield(L, -2, "x");
    lua_pushinteger(L, wp.ptMinPosition.y);
    lua_setfield(L, -2, "y");
    lua_setfield(L, tablei, "ptMinPosition");

    lua_newtable(L);
    lua_pushinteger(L, wp.ptMaxPosition.x);
    lua_setfield(L, -2, "x");
    lua_pushinteger(L, wp.ptMaxPosition.y);
    lua_setfield(L, -2, "y");
    lua_setfield(L, tablei, "ptMaxPosition");

    lua_newtable(L);
    lua_pushinteger(L, wp.rcNormalPosition.left);
    lua_setfield(L, -2, "left");
    lua_pushinteger(L, wp.rcNormalPosition.top);
    lua_setfield(L, -2, "top");
    lua_pushinteger(L, wp.rcNormalPosition.right);
    lua_setfield(L, -2, "right");
    lua_pushinteger(L, wp.rcNormalPosition.bottom);
    lua_setfield(L, -2, "bottom");
    lua_setfield(L, tablei, "rcNormalPosition");
}
static WINDOWPLACEMENT table2windowplacement(lua_State* L, int index)
{
	index = lua_absindex(L, index);

    WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };

    lua_getfield(L, index, "flags");
    wp.flags = (UINT)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "showCmd");
    wp.showCmd = (UINT)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "ptMinPosition");
    lua_getfield(L, -1, "x");
    wp.ptMinPosition.x = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "y");
    wp.ptMinPosition.y = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 2);

    lua_getfield(L, index, "ptMaxPosition");
    lua_getfield(L, -1, "x");
    wp.ptMaxPosition.x = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "y");
    wp.ptMaxPosition.y = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 2);

    lua_getfield(L, index, "rcNormalPosition");
    lua_getfield(L, -1, "left");
    wp.rcNormalPosition.left = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "top");
    wp.rcNormalPosition.top = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "right");
    wp.rcNormalPosition.right = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, -1, "bottom");
    wp.rcNormalPosition.bottom = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 2);
    return wp;
}
Lua_Function(GetWindowPlacement)
{ 
	HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    luaL_checktype(L, 2, LUA_TTABLE);
	WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
    BOOL result = GetWindowPlacement(hwnd, &wp);
    pushwindowplacement(L, 2, wp);
    lua_pushboolean(L, result);
	return 1;
}
Lua_Function(SetWindowPlacement)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    luaL_checktype(L, 2, LUA_TTABLE);
    WINDOWPLACEMENT wp = table2windowplacement(L, 2);
    BOOL result = SetWindowPlacement(hwnd, &wp);
    lua_pushboolean(L, result);
    return 1;
}
Lua_Function(SetLayeredWindowAttributes)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    COLORREF crKey = (COLORREF)luaL_checkinteger(L, 2);
    BYTE bAlpha = (BYTE)luaL_checkinteger(L, 3);
    DWORD dwFlags = (DWORD)luaL_checkinteger(L, 4);
    BOOL result = SetLayeredWindowAttributes(hwnd, crKey, bAlpha, dwFlags);
    lua_pushboolean(L, result);
	return 1;
}
Lua_Function(GetLayeredWindowAttributes)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    COLORREF* pcrKey = (COLORREF*)lua_touserdata(L, 2);
	BYTE* pbAlpha = (BYTE*)lua_touserdata(L, 3);
	DWORD* pdwFlags = (DWORD*)lua_touserdata(L, 4);
    BOOL result = GetLayeredWindowAttributes(hwnd, pcrKey, pbAlpha, pdwFlags);
    lua_pushboolean(L, result);
    return 1;
}
void register_commonwinutils(lua_State* L)
{
        // WS_*
        REGIMACRO(WS_OVERLAPPED)
        REGIMACRO(WS_POPUP)
        REGIMACRO(WS_VISIBLE)
        REGIMACRO(WS_CHILD)
        REGIMACRO(WS_BORDER)
        REGIMACRO(WS_DLGFRAME)
        REGIMACRO(WS_CAPTION)
        REGIMACRO(WS_SYSMENU)
        REGIMACRO(WS_THICKFRAME)
        REGIMACRO(WS_MINIMIZEBOX)
        REGIMACRO(WS_MAXIMIZEBOX)
        REGIMACRO(WS_OVERLAPPEDWINDOW)
        REGIMACRO(WS_POPUPWINDOW)
        REGIMACRO(WS_VSCROLL)
        REGIMACRO(WS_HSCROLL)
        REGIMACRO(WS_GROUP)
        REGIMACRO(WS_TABSTOP)
        REGIMACRO(WS_DISABLED)
        REGIMACRO(WS_CLIPSIBLINGS)
        REGIMACRO(WS_CLIPCHILDREN)
        REGIMACRO(WS_MINIMIZE)
        REGIMACRO(WS_MAXIMIZE)

        // M�s macros comunes para ShowWindow y dem�s
        REGIMACRO(SW_SHOW)
        REGIMACRO(SW_HIDE)
        REGIMACRO(SW_MINIMIZE)
        REGIMACRO(SW_MAXIMIZE)
        REGIMACRO(SW_RESTORE)
        REGIMACRO(SW_SHOWDEFAULT)
        REGIMACRO(SW_FORCEMINIMIZE)
        REGIMACRO(SW_SHOWNOACTIVATE)
        REGIMACRO(SW_SHOWNA)
        REGIMACRO(SW_SHOWNORMAL)
        REGIMACRO(SW_SHOWMINIMIZED)
        REGIMACRO(SW_SHOWMAXIMIZED)
        REGIMACRO(SW_SHOWNOACTIVATE)
        REGIMACRO(SW_RESTORE)
        REGIMACRO(SW_MINIMIZE)
        REGIMACRO(SW_MAX)
        // Estilos extendidos
        REGIMACRO(WS_EX_APPWINDOW)
        REGIMACRO(WS_EX_CLIENTEDGE)
        REGIMACRO(WS_EX_TOOLWINDOW)
        // Constantes CW_ para posici�n y tama�o por defecto
        REGIMACRO(CW_USEDEFAULT)
        // Lang
        REGIMACRO(LANG_NEUTRAL)
        REGIMACRO(SUBLANG_DEFAULT)
        // Constantes MB_ para message box
        REGIMACRO(MB_OK)                  // Bot�n OK
        REGIMACRO(MB_OKCANCEL)            // Botones OK y Cancel
        REGIMACRO(MB_ABORTRETRYIGNORE)    // Botones Abort, Retry, Ignore
        REGIMACRO(MB_YESNOCANCEL)         // Botones Yes, No, Cancel
        REGIMACRO(MB_YESNO)               // Botones Yes y No
        REGIMACRO(MB_RETRYCANCEL)         // Botones Retry y Cancel
        REGIMACRO(MB_CANCELTRYCONTINUE)   // Botones Cancel, Try, Continue

        REGIMACRO(MB_ICONHAND)            // Icono de error (hand/crash)
        REGIMACRO(MB_ICONQUESTION)        // Icono de pregunta
        REGIMACRO(MB_ICONEXCLAMATION)     // Icono de advertencia
        REGIMACRO(MB_ICONASTERISK)        // Icono de informaci�n
        REGIMACRO(MB_USERICON)            // Icono personalizado del usuario
        REGIMACRO(MB_ICONWARNING)         // Alias de MB_ICONEXCLAMATION
        REGIMACRO(MB_ICONERROR)           // Alias de MB_ICONHAND
        REGIMACRO(MB_ICONINFORMATION)     // Alias de MB_ICONASTERISK
        REGIMACRO(MB_ICONSTOP)            // Alias de MB_ICONHAND

        // Constantes ID_ generalmente usadas para el MSGBOX
        REGIMACRO(IDOK)
        REGIMACRO(IDCANCEL)
        REGIMACRO(IDABORT)
        REGIMACRO(IDRETRY)
        REGIMACRO(IDIGNORE)
        REGIMACRO(IDYES)
        REGIMACRO(IDNO)
        REGIMACRO(IDTRYAGAIN)
        REGIMACRO(IDCONTINUE)
        // Format message
        REGIMACRO(FORMAT_MESSAGE_ALLOCATE_BUFFER)
        REGIMACRO(FORMAT_MESSAGE_FROM_SYSTEM)
        REGIMACRO(FORMAT_MESSAGE_IGNORE_INSERTS)
        // Color
        REGIMACRO(COLOR_WINDOW)
        REGIMACRO(COLOR_BTNFACE)
        REGIMACRO(COLOR_HIGHLIGHT)
        REGIMACRO(COLOR_SCROLLBAR)
        // HWnds predefinidos (handles especiales)
        REGUMACRO(HWND, HWND_DESKTOP)
        REGUMACRO(HWND, HWND_BOTTOM)
        REGUMACRO(HWND, HWND_TOP)
        REGUMACRO(HWND, HWND_TOPMOST)
        REGUMACRO(HWND, HWND_NOTOPMOST)
        // Sleep
        REGIMACRO(WAIT_IO_COMPLETION)
        // PM messages
        REGIMACRO(PM_REMOVE)
        REGIMACRO(PM_NOYIELD)
        REGIMACRO(PM_NOREMOVE)
        // Proceso
        REGIMACRO(PROCESS_ALL_ACCESS)
        REGIMACRO(PROCESS_VM_READ)
        REGIMACRO(PROCESS_QUERY_INFORMATION)
        // Button
        REGIMACRO(BS_PUSHBUTTON)
        REGIMACRO(BS_DEFPUSHBUTTON)
        REGIMACRO(BS_CHECKBOX)
        REGIMACRO(BS_AUTOCHECKBOX)
        REGIMACRO(BS_RADIOBUTTON)
        REGIMACRO(BS_3STATE)
        REGIMACRO(BS_AUTO3STATE)
        REGIMACRO(BS_GROUPBOX)
        REGIMACRO(BS_USERBUTTON)
        REGIMACRO(BS_AUTORADIOBUTTON)
        REGIMACRO(BS_OWNERDRAW)
        REGIMACRO(BS_PUSHLIKE)
        REGIMACRO(BS_FLAT)
        REGIMACRO(BS_RIGHTBUTTON)

        // System metrics
        REGIMACRO(SM_CXSCREEN)
        REGIMACRO(SM_CYSCREEN)
        REGIMACRO(SM_CXVSCROLL)
        REGIMACRO(SM_CYHSCROLL)
        REGIMACRO(SM_CYCAPTION)
        REGIMACRO(SM_CXBORDER)
        REGIMACRO(SM_CYBORDER)
        REGIMACRO(SM_CXDLGFRAME)
        REGIMACRO(SM_CYDLGFRAME)
        REGIMACRO(SM_CYVTHUMB)
        REGIMACRO(SM_CXHTHUMB)
        REGIMACRO(SM_CXICON)
        REGIMACRO(SM_CYICON)
        REGIMACRO(SM_CXCURSOR)
        REGIMACRO(SM_CYCURSOR)
        REGIMACRO(SM_CYMENU)
        REGIMACRO(SM_CXFULLSCREEN)
        REGIMACRO(SM_CYFULLSCREEN)
        REGIMACRO(SM_CYKANJIWINDOW)
        REGIMACRO(SM_MOUSEPRESENT)
        REGIMACRO(SM_CYVSCROLL)
        REGIMACRO(SM_CXHSCROLL)
        REGIMACRO(SM_DEBUG)
        REGIMACRO(SM_SWAPBUTTON)
        REGIMACRO(SM_RESERVED1)
        REGIMACRO(SM_RESERVED2)
        REGIMACRO(SM_RESERVED3)
        REGIMACRO(SM_RESERVED4)
        REGIMACRO(SM_CXMIN)
        REGIMACRO(SM_CYMIN)
        REGIMACRO(SM_CXSIZE)
        REGIMACRO(SM_CYSIZE)
        REGIMACRO(SM_CXFRAME)
        REGIMACRO(SM_CYFRAME)
        REGIMACRO(SM_CXMINTRACK)
        REGIMACRO(SM_CYMINTRACK)
        REGIMACRO(SM_CXDOUBLECLK)
        REGIMACRO(SM_CYDOUBLECLK)
        REGIMACRO(SM_CXICONSPACING)
        REGIMACRO(SM_CYICONSPACING)
        REGIMACRO(SM_MENUDROPALIGNMENT)
        REGIMACRO(SM_PENWINDOWS)
        REGIMACRO(SM_DBCSENABLED)
        REGIMACRO(SM_CMOUSEBUTTONS)
        REGIMACRO(SM_CXFIXEDFRAME)
        REGIMACRO(SM_CYFIXEDFRAME)
        REGIMACRO(SM_CXSIZEFRAME)
        REGIMACRO(SM_CYSIZEFRAME)
        REGIMACRO(SM_SECURE)
        REGIMACRO(SM_CXEDGE)
        REGIMACRO(SM_CYEDGE)
        REGIMACRO(SM_CXMINSPACING)
        REGIMACRO(SM_CYMINSPACING)
        REGIMACRO(SM_CXSMICON)
        REGIMACRO(SM_CYSMICON)
        REGIMACRO(SM_CYSMCAPTION)
        REGIMACRO(SM_CXSMSIZE)
        REGIMACRO(SM_CYSMSIZE)
        REGIMACRO(SM_CXMENUSIZE)
        REGIMACRO(SM_CYMENUSIZE)
        REGIMACRO(SM_ARRANGE)
        REGIMACRO(SM_CXMINIMIZED)
        REGIMACRO(SM_CYMINIMIZED)
        REGIMACRO(SM_CXMAXTRACK)
        REGIMACRO(SM_CYMAXTRACK)
        REGIMACRO(SM_CXMAXIMIZED)
        REGIMACRO(SM_CYMAXIMIZED)
        REGIMACRO(SM_NETWORK)
        REGIMACRO(SM_CLEANBOOT)
        REGIMACRO(SM_CXDRAG)
        REGIMACRO(SM_CYDRAG)
        REGIMACRO(SM_SHOWSOUNDS)
        REGIMACRO(SM_CXMENUCHECK)
        REGIMACRO(SM_CYMENUCHECK)
        REGIMACRO(SM_SLOWMACHINE)
        REGIMACRO(SM_MIDEASTENABLED)
        REGIMACRO(SM_MOUSEWHEELPRESENT)
        REGIMACRO(SM_XVIRTUALSCREEN)
        REGIMACRO(SM_YVIRTUALSCREEN)
        REGIMACRO(SM_CXVIRTUALSCREEN)
        REGIMACRO(SM_CYVIRTUALSCREEN)
        REGIMACRO(SM_CMONITORS)
        REGIMACRO(SM_SAMEDISPLAYFORMAT)
        REGIMACRO(SM_IMMENABLED)
        REGIMACRO(SM_CXFOCUSBORDER)
        REGIMACRO(SM_CYFOCUSBORDER)
        REGIMACRO(SM_TABLETPC)
        REGIMACRO(SM_SERVERR2)

        REGIMACRO(SWP_NOSIZE)
        REGIMACRO(SWP_NOMOVE)
        REGIMACRO(SWP_NOZORDER)
        REGIMACRO(SWP_NOREDRAW)
        REGIMACRO(SWP_NOACTIVATE)
        REGIMACRO(SWP_FRAMECHANGED)
        REGIMACRO(SWP_SHOWWINDOW)
        REGIMACRO(SWP_HIDEWINDOW)
        REGIMACRO(SWP_NOCOPYBITS)
        REGIMACRO(SWP_NOOWNERZORDER)
        REGIMACRO(SWP_NOSENDCHANGING)
        REGIMACRO(SWP_DRAWFRAME)
        REGIMACRO(SWP_NOREPOSITION)
        REGIMACRO(SWP_DEFERERASE)
        REGIMACRO(SWP_ASYNCWINDOWPOS)

        REGIMACRO(WS_EX_DLGMODALFRAME)
REGIMACRO(WS_EX_NOPARENTNOTIFY)
REGIMACRO(WS_EX_TOPMOST)
REGIMACRO(WS_EX_ACCEPTFILES)
REGIMACRO(WS_EX_TRANSPARENT)
REGIMACRO(WS_EX_MDICHILD)
REGIMACRO(WS_EX_TOOLWINDOW)
REGIMACRO(WS_EX_WINDOWEDGE)
REGIMACRO(WS_EX_CLIENTEDGE)
REGIMACRO(WS_EX_CONTEXTHELP)
REGIMACRO(WS_EX_RIGHT)
REGIMACRO(WS_EX_LEFT)
REGIMACRO(WS_EX_RTLREADING)
REGIMACRO(WS_EX_LTRREADING)
REGIMACRO(WS_EX_LEFTSCROLLBAR)
REGIMACRO(WS_EX_RIGHTSCROLLBAR)
REGIMACRO(WS_EX_CONTROLPARENT)
REGIMACRO(WS_EX_STATICEDGE)
REGIMACRO(WS_EX_APPWINDOW)
REGIMACRO(WS_EX_OVERLAPPEDWINDOW)
REGIMACRO(WS_EX_PALETTEWINDOW)
REGIMACRO(WS_EX_LAYERED)
REGIMACRO(WS_EX_NOINHERITLAYOUT)
REGIMACRO(WS_EX_NOREDIRECTIONBITMAP)
REGIMACRO(WS_EX_LAYOUTRTL)
REGIMACRO(WS_EX_COMPOSITED)
REGIMACRO(WS_EX_NOACTIVATE)
}