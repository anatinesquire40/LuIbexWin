static void prepareLuaProcTable(lua_State* L, const std::string& regName)
{
    lua_pushlstring(L, regName.c_str(), regName.size());
    lua_newtable(L);
    lua_rawset(L, LUA_REGISTRYINDEX);
}

static void getLProcTable(lua_State* L, const std::string& regName)
{
    lua_pushlstring(L, regName.c_str(), regName.size());
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (!lua_istable(L, -1))
    {
        lua_pop(L, 1);
        prepareLuaProcTable(L, regName);
        lua_pushlstring(L, regName.c_str(), regName.size());
        lua_rawget(L, LUA_REGISTRYINDEX);
    }
}

static bool hasEqualFunction(lua_State* L, int funcIndex, int tableIndex, size_t tableLen, size_t& equalFunci)
{
    funcIndex = lua_absindex(L, funcIndex);
    tableIndex = lua_absindex(L, tableIndex);

    for (size_t i = 1; i <= tableLen; ++i)
    {
        lua_geti(L, tableIndex, i);
        bool isEqual = lua_rawequal(L, funcIndex, -1);
        lua_pop(L, 1);
        if (isEqual)
        {
            equalFunci = i;
            return true;
        }
    }
    return false;
}

static void pushTheFunction(lua_State* L, int funcIndex, int tableIndex)
{
    tableIndex = lua_absindex(L, tableIndex);
    funcIndex = lua_absindex(L, funcIndex);

    size_t equalFunc;
    if (hasEqualFunction(L, funcIndex, tableIndex, (size_t)lua_rawlen(L, tableIndex), equalFunc))
    {
        lua_pushinteger(L, (lua_Integer)equalFunc);
    }
    else
    {
        lua_pushvalue(L, funcIndex);
    }
}

int addLWinProc(lua_State* L, int index, const std::string& regName)
{
    int funcIndex = lua_absindex(L, index);
    getLProcTable(L, regName);
    int luaTable = lua_absindex(L, -1);

    pushTheFunction(L, funcIndex, luaTable);

    int newIndex = (int)lua_rawlen(L, luaTable) + 1;
    lua_seti(L, luaTable, newIndex);
    lua_pop(L, 1);
    return newIndex;
}

bool getLWinProci(lua_State* L, lua_Integer i, const std::string& regName)
{
    getLProcTable(L, regName);
    int tableIndex = lua_absindex(L, -1);

    lua_geti(L, tableIndex, i);
    if (!lua_isfunction(L, -1) && !lua_isinteger(L, -1))
    {
        lua_pop(L, 1);
        return false;
    }

    if (lua_isinteger(L, -1))
    {
        lua_Integer newi = lua_tointeger(L, -1);
        lua_pop(L, 1);
        return getLWinProci(L, newi, regName);
    }

    lua_remove(L, tableIndex);
    return true;
}

bool removeLWinProc(lua_State* L, lua_Integer i, const std::string& regName)
{
    getLProcTable(L, regName);
    int tableIndex = lua_absindex(L, -1);

    size_t len = lua_rawlen(L, tableIndex);
    if (i < 1 || i >(lua_Integer)len)
    {
        lua_pop(L, 1);
        return false;
    }

    lua_pushnil(L);
    lua_seti(L, tableIndex, i);

    lua_pop(L, 1);
    return true;
}
