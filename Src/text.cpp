Lua_Function(SetTextColor)
{
	HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
	COLORREF color = (COLORREF)luaL_checkinteger(L, 2);
	COLORREF prevColor = SetTextColor(hdc, color);
	lua_pushinteger(L, prevColor);
	return 1;
}
Lua_Function(GetTextColor)
{
	HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
	COLORREF color = GetTextColor(hdc);
	lua_pushinteger(L, color);
	return 1;
}
Lua_Function(SetBkColor)
{
	HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
	COLORREF color = (COLORREF)luaL_checkinteger(L, 2);
	COLORREF prevColor = SetBkColor(hdc, color);
	lua_pushinteger(L, prevColor);
	return 1;
}
Lua_Function(GetBkColor)
{
	HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
	COLORREF color = GetBkColor(hdc);
	lua_pushinteger(L, color);
	return 1;
}
Lua_Function(SetBkMode)
{
	HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
	int mode = (int)luaL_checkinteger(L, 2);
	int prevMode = SetBkMode(hdc, mode);
	lua_pushinteger(L, prevMode);
	return 1;
}
Lua_Function(GetBkMode)
{
	HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
	int mode = GetBkMode(hdc);
	lua_pushinteger(L, mode);
	return 1;
}
Lua_Function(TextOut)
{
	HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
	int x = (int)luaL_checkinteger(L, 2);
	int y = (int)luaL_checkinteger(L, 3);
	size_t len = 0;
	const char* str = luaL_checklstring(L, 4, &len);
	BOOL result = TextOutA(hdc, x, y, str, (int)len);
	lua_pushboolean(L, result);
	return 1;
}
Lua_Function(GetTextExtentPoint32)
{
	HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
	size_t len = 0;
	const char* str = luaL_checklstring(L, 2, &len);
	LPSIZE lpSize = (LPSIZE)lua_newuserdata(L, sizeof(SIZE));
	BOOL result = GetTextExtentPoint32A(hdc, str, (int)len, lpSize);
	lua_pushboolean(L, result);
	if (result) {
		lua_newtable(L);
		lua_pushinteger(L, lpSize->cx);
		lua_setfield(L, -2, "cx");
		lua_pushinteger(L, lpSize->cy);
		lua_setfield(L, -2, "cy");
	} else {
		lua_pushnil(L);
	}
	return 2;
}
Lua_Function(DrawTextEx)
{
	HDC hdc = luaL_wingetbycheckudata(L, 1, HDC);
	LPCSTR lpString = luaL_checkstring(L, 2);
	size_t len = (size_t)luaL_checkinteger(L, 3);
	RECT rect;
	luaL_checktype(L, 4, LUA_TTABLE);
	table_toLPRECT(L, 4, &rect);
	LPDRAWTEXTPARAMS lpDTParams = (LPDRAWTEXTPARAMS)lua_touserdata(L, 5);
	int result = DrawTextExA(hdc, (LPSTR)lpString, (int)len, &rect, 0, lpDTParams);
	lua_pushinteger(L, result);
	return 1;
}