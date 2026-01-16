Lua_Function(CreateDirectoryEx)
{
    LPCSTR dirTemplate = lua_tostring(L, 1);
    LPCSTR dirNew = luaL_checkstring(L, 2);
    SECURITY_ATTRIBUTES sa_struct;
    LPSECURITY_ATTRIBUTES sa = nullptr;

    if (!lua_isnil(L, 3))
    {
        luaL_checktype(L, 3, LUA_TTABLE);
        sa_struct = table2SECURITY_ATTRIBUTES(L, 3);
        sa = &sa_struct;
    }
    bool result = CreateDirectoryExA(dirTemplate, dirNew, sa);
    lua_pushboolean(L, result);
    return 1;
}
Lua_Function(GetFileAttributes)
{
    LPCSTR filePath = luaL_checkstring(L, 1);
    DWORD attributes = GetFileAttributesA(filePath);
    lua_pushinteger(L, (lua_Integer)attributes);
    return 1;
}
void register_filesystem(lua_State* L)
{
    REGIMACRO(FILE_ATTRIBUTE_READONLY)
        REGIMACRO(FILE_ATTRIBUTE_HIDDEN)
        REGIMACRO(FILE_ATTRIBUTE_SYSTEM)
        REGIMACRO(FILE_ATTRIBUTE_DIRECTORY)
        REGIMACRO(FILE_ATTRIBUTE_ARCHIVE)
        REGIMACRO(FILE_ATTRIBUTE_NORMAL)
        REGIMACRO(FILE_ATTRIBUTE_TEMPORARY)
        REGIMACRO(FILE_ATTRIBUTE_OFFLINE)
        REGIMACRO(FILE_ATTRIBUTE_ENCRYPTED)
        REGIMACRO(FILE_ATTRIBUTE_COMPRESSED)
        REGIMACRO(FILE_ATTRIBUTE_REPARSE_POINT)
        REGIMACRO(FILE_FLAG_BACKUP_SEMANTICS)
        REGIMACRO(FILE_FLAG_DELETE_ON_CLOSE)
        REGIMACRO(FILE_FLAG_OVERLAPPED)
        REGIMACRO(FILE_FLAG_NO_BUFFERING)
        REGIMACRO(FILE_FLAG_RANDOM_ACCESS)
        REGIMACRO(FILE_FLAG_SEQUENTIAL_SCAN)
        REGIMACRO(GENERIC_READ)
        REGIMACRO(GENERIC_WRITE)
        REGIMACRO(GENERIC_EXECUTE)
        REGIMACRO(GENERIC_ALL)
        REGIMACRO(SECURITY_DESCRIPTOR_REVISION)
        REGIMACRO(DACL_SECURITY_INFORMATION)
        REGIMACRO(OWNER_SECURITY_INFORMATION)
        REGIMACRO(GROUP_SECURITY_INFORMATION)
}