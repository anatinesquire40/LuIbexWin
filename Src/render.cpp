void table_toLPRECT(lua_State* L, int index, LPRECT rect)
{
    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, index, "left");
    rect->left = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "top");
    rect->top = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "right");
    rect->right = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "bottom");
    rect->bottom = (LONG)luaL_checkinteger(L, -1);
    lua_pop(L, 1);
}
void LPRECT_to_table(lua_State* L, const LPRECT rect, int tindex)
{
    tindex = lua_absindex(L, tindex);

    lua_pushinteger(L, rect->left);
    lua_setfield(L, tindex, "left");

    lua_pushinteger(L, rect->top);
    lua_setfield(L, tindex, "top");

    lua_pushinteger(L, rect->right);
    lua_setfield(L, tindex, "right");

    lua_pushinteger(L, rect->bottom);
    lua_setfield(L, tindex, "bottom");
}
Lua_Function(FillRect)
{
	RECT rect;
	HDC hdc = (HDC)luaL_wingetbycheckudata(L, 1, HDC);
	table_toLPRECT(L, 2, &rect);
	HBRUSH hbr = luaL_wingetbycheckudata(L, 3, HBRUSH);
	BOOL result = FillRect(hdc, &rect, hbr);
	lua_pushboolean(L, result);
	return 1;
}
Lua_Function(GetDC)
{
    HWND hWnd = luaL_wingetbycheckudata(L, 1, HWND);
    HDC hdc = GetDC(hWnd);
    pushWindowStruct(L, HDC, hdc);
    return 1;
}
Lua_Function(ReleaseDC)
{
    HWND hWnd = luaL_wingetbycheckudata(L, 1, HWND);
    HDC hdc = luaL_wingetbycheckudata(L, 2, HDC);
    int result = ReleaseDC(hWnd, hdc);
    lua_pushinteger(L, result);
    return 1;
}
Lua_Function(CreateCompatibleDC)
{
    HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
    HDC compDC = CreateCompatibleDC(hdc);
    pushWindowStruct(L, HDC, compDC);
    return 1;
}
Lua_Function(CreateCompatibleBitmap)
{
    HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
    int width = (int)luaL_checkinteger(L, 2);
    int height = (int)luaL_checkinteger(L, 3);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    pushWindowStruct(L, HBITMAP, hBitmap);
    return 1;
}
Lua_Function(DeleteDC)
{
    HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
    BOOL result = DeleteDC(hdc);
    lua_pushboolean(L, result);
    return 1;
}
Lua_Function(SelectObject)
{
    HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
    HGDIOBJ hObj = luaL_wingetbycheckudata(L, 2, HGDIOBJ);
    HGDIOBJ prevObj = SelectObject(hdc, hObj);
    pushWindowStruct(L, HGDIOBJ, prevObj);
	return 1;
}
Lua_Function(DeleteObject)
{
    HGDIOBJ hObj = luaL_wingetbycheckudata(L, 1, HGDIOBJ);
    BOOL result = DeleteObject(hObj);
    lua_pushboolean(L, result);
    return 1;
}
Lua_Function(BitBlt)
{
    HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);
    int cx = (int)luaL_checkinteger(L, 4);
    int cy = (int)luaL_checkinteger(L, 5);
    HDC hdcSrc = luaL_wingetbycheckudata(L, 6, HDC);
    int x1 = (int)luaL_checkinteger(L, 7);
    int y1 = (int)luaL_checkinteger(L, 8);
    DWORD rop = (DWORD)luaL_checkinteger(L, 9);
    BOOL result = BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
    lua_pushboolean(L, result);
	return 1;
}
Lua_Function(GetPixel)
{
    HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);
    COLORREF color = GetPixel(hdc, x, y);
    lua_pushinteger(L, color);
    return 1;
}
Lua_Function(SetPixel)
{
    HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
    int x = (int)luaL_checkinteger(L, 2);
    int y = (int)luaL_checkinteger(L, 3);
    COLORREF color = (COLORREF)luaL_checkinteger(L, 4);
    COLORREF result = SetPixel(hdc, x, y, color);
    lua_pushinteger(L, result);
    return 1;
}
static void PAINTSTRUCT_to_table(lua_State* L, const PAINTSTRUCT* ps, int tableindex)
{
	tableindex = lua_absindex(L, tableindex);
    // hdc
    lua_pushstring(L, "hdc");
    pushWindowStruct(L, HDC, ps->hdc);
    lua_settable(L, tableindex);
    // fErase
    lua_pushstring(L, "fErase");
    lua_pushboolean(L, ps->fErase);
    lua_settable(L, tableindex);

    // rcPaint
    lua_pushstring(L, "rcPaint");
	RECT rcPaint = ps->rcPaint;
    lua_newtable(L);
    LPRECT_to_table(L, &rcPaint, -1);
    lua_settable(L, tableindex);
    // fRestore
    lua_pushstring(L, "fRestore");
    lua_pushboolean(L, ps->fRestore);
    lua_settable(L, tableindex);

    // fIncUpdate
    lua_pushstring(L, "fIncUpdate");
    lua_pushboolean(L, ps->fIncUpdate);
    lua_settable(L, tableindex);
    // rgbReserved
    lua_pushstring(L, "rgbReserved");
    lua_newtable(L);
    for (int i = 0; i < 32; ++i)
    {
        lua_pushinteger(L, i + 1);
        lua_pushinteger(L, ps->rgbReserved[i]);
        lua_settable(L, -3);
    }
    lua_settable(L, tableindex);
}
static LPPAINTSTRUCT table_to_PAINTSTRUCT(lua_State* L, int index)
{
    luaL_checktype(L, index, LUA_TTABLE);
	lua_getfield(L, index, "p");
    LPPAINTSTRUCT ps = (LPPAINTSTRUCT)luaL_checkuserdata(L, -1);
    lua_pop(L, 1);  
    // hdc
    lua_getfield(L, index, "hdc");
    ps->hdc = luaL_wingetbycheckudata(L, -1, HDC);
    lua_pop(L, 1);
    // fErase
    lua_getfield(L, index, "fErase");
    ps->fErase = (BOOL)lua_toboolean(L, -1);
    lua_pop(L, 1);
    // rcPaint
    lua_getfield(L, index, "rcPaint");
    table_toLPRECT(L, -1, &ps->rcPaint);
    lua_pop(L, 1);
    // fRestore
    lua_getfield(L, index, "fRestore");
    ps->fRestore = (BOOL)lua_toboolean(L, -1);
    lua_pop(L, 1);
    // fIncUpdate
    lua_getfield(L, index, "fIncUpdate");
    ps->fIncUpdate = (BOOL)lua_toboolean(L, -1);
    lua_pop(L, 1);
    // rgbReserved
    lua_getfield(L, index, "rgbReserved");
    for (int i = 0; i < 32; ++i)
    {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, -2);
        ps->rgbReserved[i] = (BYTE)luaL_optinteger(L, -1, 0);
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
	return ps;
}
Lua_Function(BeginPaint)
{
	HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    luaL_checktype(L, 2, LUA_TTABLE);
	LPPAINTSTRUCT lpps = (LPPAINTSTRUCT)lua_newuserdata(L, sizeof(PAINTSTRUCT));
	lua_setfield(L, 2, "p");
	HDC hdc = BeginPaint(hwnd, lpps);
	PAINTSTRUCT_to_table(L, lpps, 2);
	pushWindowStruct(L, HDC, hdc);
    return 1;
}
Lua_Function(EndPaint)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    
    luaL_checktype(L, 2, LUA_TTABLE);
    LPPAINTSTRUCT lpps = table_to_PAINTSTRUCT(L, 2);
    BOOL result = EndPaint(hwnd, lpps);
    lua_pushboolean(L, result);
	return 1;
}
Lua_Function(InvalidateRect)
{
    HWND hwnd = luaL_wingetbycheckudata(L, 1, HWND);
    RECT rect;
	LPRECT rectPtr = nullptr;
    if (lua_type(L, 2) == LUA_TTABLE)
    {
        table_toLPRECT(L, 2, &rect);
		rectPtr = &rect;
    }
    BOOL erase = (BOOL)lua_toboolean(L, 3);
    BOOL result = InvalidateRect(hwnd, rectPtr, erase);
    lua_pushboolean(L, result);
    return 1;
}
void register_render(lua_State* L)
{
    REGIMACRO(SRCCOPY)
    REGIMACRO(SRCPAINT)
    REGIMACRO(SRCAND)
    REGIMACRO(SRCINVERT)
    REGIMACRO(SRCERASE)
    REGIMACRO(NOTSRCCOPY)
    REGIMACRO(NOTSRCERASE)
    REGIMACRO(MERGECOPY)
    REGIMACRO(MERGEPAINT)
    REGIMACRO(PATCOPY)
    REGIMACRO(PATPAINT)
    REGIMACRO(PATINVERT)
    REGIMACRO(DSTINVERT)
    REGIMACRO(BLACKNESS)
    REGIMACRO(WHITENESS)
    // background modes
	REGIMACRO(TRANSPARENT)
    REGIMACRO(OPAQUE)
    REGIMACRO(BKMODE_LAST)
}