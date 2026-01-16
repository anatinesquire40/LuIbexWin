Lua_Function(RGB)
{
	lua_pushinteger(L, RGB(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3)));
	return 1;
}
Lua_Function(CreateSolidBrush)
{
	HBRUSH brush = CreateSolidBrush((COLORREF)luaL_checkinteger(L, 1));
	pushWindowStruct(L, HBRUSH, brush);
	return 1;
}
Lua_Function(GetStockObject)
{
	lua_Integer obj = luaL_checkinteger(L, 1);
	HGDIOBJ stockObj = GetStockObject((int)obj);
	pushWindowStruct(L, HGDIOBJ, stockObj);
	return 1;
}
Lua_Function(GetSysColor)
{
	int nIndex = (int)luaL_checkinteger(L, 1);
	COLORREF color = GetSysColor(nIndex);
	lua_pushinteger(L, color);
	return 1;
}
Lua_Function(GetSysColorBrush)
{
	int nIndex = (int)luaL_checkinteger(L, 1);
	HBRUSH hBrush = GetSysColorBrush(nIndex);
	pushWindowStruct(L, HBRUSH, hBrush);
	return 1;
}
void register_brush(lua_State* L)
{
	REGIMACRO(BLACK_BRUSH)
	REGIMACRO(WHITE_BRUSH)
	REGIMACRO(GRAY_BRUSH)
	REGIMACRO(LTGRAY_BRUSH)
	REGIMACRO(DKGRAY_BRUSH)
	REGIMACRO(HOLLOW_BRUSH)
	REGIMACRO(NULL_BRUSH)
	REGIMACRO(DC_BRUSH)
}