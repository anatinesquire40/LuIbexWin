Lua_Function(MAKEINTRESOURCE)
{
    lua_Integer id = luaL_checkinteger(L, 1);
    LPSTR res = MAKEINTRESOURCEA((WORD)id);
	pushWindowStruct(L, LPSTR, res);
	return 1;
}
Lua_Function(LoadIcon)
{
    // El primer argumento es un HINSTANCE o NULL
    HINSTANCE hInstance =  luaL_wingetbyudata(L, 1, HINSTANCE);

    // El segundo argumento es el nombre del cursor o un ID predefinido
    // LoadCursor acepta LPCSTR o MAKEINTRESOURCE, así que lo interpretamos:
    LPCSTR lpIconName = luaL_wingetbycheckudata(L, 2, LPCSTR);
    HICON icon = LoadIconA(hInstance, lpIconName);
    pushWindowStruct(L, HICON, icon);
    return 1;
}
void register_icon(lua_State* L)
{
    REGUMACRO(LPSTR, IDI_APPLICATION);
}