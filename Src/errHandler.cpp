Lua_Function(GetLastError)
{
    DWORD err = GetLastError();
    lua_pushinteger(L, (lua_Integer)err);
    return 1;
}
Lua_Function(MAKELANGID)
{
    WORD primary = (WORD)luaL_checkinteger(L, 1);
    WORD sublang = (WORD)luaL_checkinteger(L, 2);
    WORD langid = (WORD)(((sublang << 10) & 0xFC00) | (primary & 0x03FF));
    lua_pushinteger(L, langid);
    return 1;
}


// Llame Lua a esta función para obtener el mensaje formateado
Lua_Function(FormatMessage)
{

    DWORD flags = (DWORD)luaL_optinteger(L, 1,
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY);
    LPCVOID lpSource = luaL_wingetbyudata(L, 2, LPCVOID);
    DWORD err = (DWORD)luaL_checkinteger(L, 3);
    if (err == 0)
    {
        lua_pushstring(L, "No error");
        return 1;
    }
    DWORD langId = (DWORD)luaL_optinteger(L, 4, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
    DWORD nSize = (DWORD)luaL_optinteger(L, 5, 0);
    int nargs = lua_gettop(L) - 5;

    // Creamos un arreglo de LPVOID para los argumentos que usará FormatMessage
    LPVOID* args = NULL;
    int i;
    // Variables auxiliares para argumentos numéricos (evitar punteros a estáticos)
    int* numArgs = NULL;

    if (nargs > 0)
    {
        args = (LPVOID*)malloc(sizeof(LPVOID) * nargs);
        if (!args)
            return luaL_error(L, "Out of memory");

        // Para números almacenamos en heap para evitar errores
        numArgs = (int*)malloc(sizeof(int) * nargs);
        if (!numArgs)
        {
            free(args);
            return luaL_error(L, "Out of memory");
        }

        for (i = 0; i < nargs; i++)
        {
            int idx = 6 + i;
            if (lua_type(L, idx) == LUA_TSTRING)
            {
                args[i] = (LPVOID)lua_tostring(L, idx);
            }
            else if (lua_type(L, idx) == LUA_TNUMBER)
            {
                numArgs[i] = (int)lua_tointeger(L, idx);
                args[i] = &numArgs[i];
            }
            else
            {
                free(args);
                free(numArgs);
                return luaL_error(L, "Unsupported argument type at position %d", i + 1);
            }
        }
    }

    LPVOID msgBuf = NULL;
    // IMPORTANTE: Si usas FORMAT_MESSAGE_ALLOCATE_BUFFER, lpBuffer debe ser puntero a LPSTR y nSize 0
    DWORD result = FormatMessageA(
        flags,
        lpSource,
        err,
        langId,
        (LPSTR)&msgBuf,
        nSize,
        (va_list*)args);

    if (args) free(args);
    if (numArgs) free(numArgs);

    if (msgBuf)
    {
        lua_pushstring(L, (const char*)msgBuf);
        LocalFree(msgBuf);
    }
    else
        lua_pushnil(L);
    lua_pushinteger(L, (lua_Integer)result);
    return 2;
}
Lua_Function(CommDlgExtendedError)
{
    DWORD err = CommDlgExtendedError();
    lua_pushinteger(L, (lua_Integer)err);
    return 1;
}
void register_errors(lua_State* L)
{
    REGUMACRO(HANDLE, INVALID_HANDLE_VALUE)
    REGIMACRO(INVALID_FILE_ATTRIBUTES)
    REGIMACRO(INVALID_SET_FILE_POINTER)
}