enum ValTypes
{
    VT_INTEGER,
    VT_BOOLEAN,
    VT_DOUBLE,
    VT_FLOAT,
    VT_USERDATA,
    VT_STRING,
    VT_NIL
};
typedef std::vector<std::variant<void*, lua_Number, float>> TExecVector;
typedef struct {
    int callbackidx;
    FARPROC func;
    TExecVector args;
    size_t nargs;
    bool hasNumber;
    bool retNum;
    bool retF32;
    void* result;
    ValTypes retType;
    lua_Number numResult;
} DispatchInfo;
//-------------------- Helpers --------------------
template<typename T>
static void makeUd(lua_State* L, T value) {
    T* ud = (T*)lua_newuserdata(L, sizeof(T));
    *ud = value;
}

template<typename T>
static T directReadMem(lua_State* L, T* value, const char* retType) {
    T ret;
    __try { ret = *value; }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        luaL_error(L, "Failed to read memory at %p for type %s", value, retType);
    }
    return ret;
}

static void crashError(lua_State* L, const char* funcName, const char* action, const uintptr_t& errcode)
{
    char buf[128];
    _snprintf_s(buf, sizeof(buf), _TRUNCATE,
        "%s: %s crashed (0x%llx)", funcName, action,
        (unsigned long long)errcode);
    luaL_error(L, "%s", buf);

}

template<typename T>
static T getArg(const TExecVector& args, size_t i) {
    return std::get<T>(args[i]);
}

// Call with sign
template<typename Ret, typename... Args, size_t... I>
static Ret callFunc(DispatchInfo* info, std::index_sequence<I...>) {
    return ((Ret(*)(Args...))info->func)(getArg<Args>(info->args, I)...);
}

// Select sign
template<typename Ret>
static Ret execFunc(lua_State* L, DispatchInfo* info) {
    Ret result{};
    bool success = false;
    uintptr_t exceptionCode = 0;
    bool hasNumber = info->hasNumber;
    size_t nargs = info->nargs;
    __try {
        switch (nargs) {
        case 0:
            result = ((Ret(*)())info->func)();
            break;
        case 1:
            result = hasNumber ? callFunc<Ret, lua_Number>(info, std::index_sequence<0>{})
                : callFunc<Ret, void*>(info, std::index_sequence<0>{});
            break;
        case 2:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number>(info, std::index_sequence<0, 1>{})
                : callFunc<Ret, void*, void*>(info, std::index_sequence<0, 1>{});
            break;
        case 3:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2>{})
                : callFunc<Ret, void*, void*, void*>(info, std::index_sequence<0, 1, 2>{});
            break;
        case 4:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3>{})
                : callFunc<Ret, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3>{});
            break;
        case 5:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4>{})
                : callFunc<Ret, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4>{});
            break;
        case 6:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5>{});
            break;
        case 7:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6>{});
            break;
        case 8:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7>{});
            break;
        case 9:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8>{});
            break;
        case 10:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>{});
            break;
        case 11:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10>{});
            break;
        case 12:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11>{});
            break;
        case 13:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>{});
            break;
        case 14:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13>{});
            break;
        case 15:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14>{});
            break;
        case 16:
            result = hasNumber ? callFunc<Ret, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number, lua_Number>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15>{})
                : callFunc<Ret, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*>(info, std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15>{});
            break;
        default:
            luaL_error(L, "Too many arguments: %d", nargs);
        }
        success = true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        success = false;
        exceptionCode = GetExceptionCode();
    }

    if (!success) {
        crashError(L, "Addr2Val", "function call has", exceptionCode);
    }

    return result;
}

static void checkArgs(lua_State* L, DispatchInfo* info)
{
    const TExecVector& args = info->args;
    for (size_t i = 0; i < info->nargs; ++i) {
        if (std::holds_alternative<float>(args[i])) {
            luaL_error(L, "Float32 is not supported on the default trampoline.");
        }
        if (info->hasNumber && !std::holds_alternative<lua_Number>(args[i])) {
            luaL_error(L, "All arguments must be numbers when at least one argument is a number.");
        }
    }
}
static lua_Integer getArgInt(TExecVector args, lua_Integer i)
{
    if (std::holds_alternative<lua_Number>(args[i])) {
        return std::bit_cast<lua_Integer>(getArg<lua_Number>(args, i));
    }
    else if(std::holds_alternative<float>(args[i])) {
        return (lua_Integer)std::bit_cast<uint32_t>(getArg<float>(args, i));
    } else {
        return (lua_Integer)getArg<void*>(args,i);
    }
}
static void pushArgsLua(lua_State* L, DispatchInfo* info)
{
    lua_newtable(L);
    int argsIdx = lua_absindex(L, -1);
	for (lua_Integer i = 0; i < info->nargs; ++i) {
        lua_Integer la = getArgInt(info->args, i);
        lua_pushinteger(L, la);
        lua_rawseti(L, argsIdx, i+1);
    }
}
template<typename T>
static T luacallt(lua_State* L, void* func)
{
    T result;
    bool success = false;
    uintptr_t exceptionCode = 0;

    __try {
        result = ((T(*)())func)();
        success = true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        success = false;
        exceptionCode = GetExceptionCode();
    }

    if (!success) {
        crashError(L, "Addr2Val", "function call has", exceptionCode);
    }

    return result;
}
static bool pushRet(lua_State* L, DispatchInfo* info)
{
    const ValTypes& retType = info->retType;
    void* result = info->result;
    if (retType == VT_INTEGER) lua_pushinteger(L, (lua_Integer)result);
    else if (retType == VT_DOUBLE or retType == VT_FLOAT) lua_pushnumber(L, info->numResult);
    else if (retType == VT_USERDATA) lua_pushlightuserdata(L, (void*)result);
    else if (retType == VT_STRING) lua_pushstring(L, (const char*)result);
    else if (retType == VT_BOOLEAN) lua_pushboolean(L, (bool)result);
    else if (retType == VT_NIL) return false;
    else luaL_error(L, "Unsupported return type: %s", retType);
    return true;
}
static void getArgsLua(lua_State* L, DispatchInfo* info)
{
    for (lua_Integer i = 1; i <= info->nargs; ++i) {
        lua_rawgeti(L, lua_upvalueindex(2), i);
        ValTypes type = (ValTypes)lua_tointeger(L, -1);
        lua_pop(L, 1);
        size_t veci = i - 1;
        if (type == VT_INTEGER) info->args[veci] = (void*)lua_tointeger(L, i);
        else if (type == VT_DOUBLE) { info->args[veci] = lua_tonumber(L, i); info->hasNumber = true; }
        else if (type == VT_FLOAT)  info->args[veci] = (float)lua_tonumber(L, i);
        else if (type == VT_BOOLEAN) info->args[veci] = (void*)(uintptr_t)(lua_toboolean(L, i) ? 1 : 0);
        else if (type == VT_USERDATA)  info->args[veci] = lua_touserdata(L, i);
        else if (type == VT_STRING) info->args[veci] = (void*)lua_tostring(L, i);
        else luaL_error(L, "Unsupported arg type: %d", type);
    }
}

static void dispatchCustom(lua_State* L, DispatchInfo* info)
{
    lua_pushvalue(L, info->callbackidx);
    lua_pushvalue(L, lua_upvalueindex(1));
    lua_pushvalue(L, lua_upvalueindex(2));
    pushArgsLua(L, info);
    lua_pushinteger(L, info->nargs);
    lua_pushboolean(L, info->retNum);
    lua_pushboolean(L, info->retF32);
    lua_call(L, 6, 1);
    if (!lua_isnil(L, -1))
    {
        void* f = lua_touserdata(L, -1);
        if (info->retNum) {
            info->numResult = luacallt<lua_Number>(L, f);
        }
        else if (info->retF32) {
            info->numResult = (lua_Number)luacallt<float>(L, f);
        }
        else {
            info->result = luacallt<void*>(L, f);
        }
        if (lua_isfunction(L, lua_upvalueindex(6)))
        {
            lua_pushvalue(L, lua_upvalueindex(6));
            lua_pushvalue(L, -2);
            lua_call(L, 1, 0);
        }
        lua_pop(L, 1); // remove f from stack
    }
}
static void dispatchDefault(lua_State* L, DispatchInfo* info)
{
    checkArgs(L, info);
    if (info->retNum)
    {
        info->numResult = execFunc<lua_Number>(L, info);
    }
    else
    {
        info->result = execFunc<void*>(L, info);
    }
}
static int executeProcAddr(lua_State* L) {
    DispatchInfo info;
    info.func = (FARPROC)lua_touserdata(L, lua_upvalueindex(1));
    luaL_checktype(L, lua_upvalueindex(2), LUA_TTABLE);
    info.retType = (ValTypes)lua_tointeger(L, lua_upvalueindex(4));
    info.callbackidx = lua_upvalueindex(5);
    bool hascustomback = lua_isfunction(L, info.callbackidx);
    info.nargs = lua_tointeger(L, lua_upvalueindex(3));
    if (lua_gettop(L) != info.nargs) luaL_error(L, "the number of arguments given doesn't match with the function signature");
    if (!hascustomback && info.nargs > 16) luaL_error(L, "too many arguments, max is 16");
    info.args = TExecVector(info.nargs);
    info.hasNumber = false;
    info.retNum = info.retType == VT_DOUBLE;
    info.retF32 = info.retType == VT_FLOAT;
    info.numResult = 0.0;
    info.result = 0;
    getArgsLua(L, &info);
    hascustomback ? dispatchCustom(L, &info) : dispatchDefault(L, &info);
    if (pushRet(L, &info)) {
        return 1;
    }
    return 0;
}

Lua_Function(Addr2Val)
{
    void* ptr = lua_touserdata(L, 1);
    if (!ptr) {
        return luaL_error(L, "null pointers are not allowed, make sure you use correct pointers");
    }
    bool isFunctionPointer = lua_istable(L, 3);
    const ValTypes retType = (ValTypes)luaL_checkinteger(L, 2);

    if (isFunctionPointer) {
        lua_Integer nargs = (lua_Integer)lua_rawlen(L, 3);
        lua_pushvalue(L, 1);
        lua_pushvalue(L, 3);
        lua_pushinteger(L, nargs);
        lua_pushinteger(L, retType);
        if (lua_isfunction(L, 4)) {
            lua_pushvalue(L, 4);
        }
        else {
            lua_pushnil(L);
        }
        if (lua_isfunction(L, 5)) {
            lua_pushvalue(L, 5);
		}
        else {
            lua_pushnil(L);
        }
        lua_pushcclosure(L, executeProcAddr, 6);
        return 1;
    }
    else
    {
        static const std::unordered_map<ValTypes, std::function<void(lua_State*, void*)>> retHandlers = {
            { VT_INTEGER, [](lua_State* L, void* p) { lua_pushinteger(L, directReadMem<lua_Integer>(L, (lua_Integer*)p, "integer")); } },
            { VT_BOOLEAN, [](lua_State* L, void* p) { lua_pushboolean(L, directReadMem<bool>(L, (bool*)p, "boolean")); } },
            { VT_DOUBLE,  [](lua_State* L, void* p) { lua_pushnumber(L, directReadMem<lua_Number>(L, (lua_Number*)p, "double")); } },
            { VT_FLOAT,   [](lua_State* L, void* p) { lua_pushnumber(L, (lua_Number)directReadMem<float>(L, (float*)p, "float")); } },
            { VT_USERDATA,[](lua_State* L, void* p) { lua_pushvalue(L, 1); } },
            { VT_STRING,  [](lua_State* L, void* p) { 
                if (lua_isinteger(L, 3)) {
                    lua_Integer size = lua_tointeger(L, 3);
                    if (size > 0) {
                        lua_pushlstring(L, (const char*)p, (size_t)size);
                        return;
                    }
                }
                lua_pushstring(L, (const char*)p);
            } },
        };

        auto it = retHandlers.find(retType);
        if (it != retHandlers.end()) {
            it->second(L, ptr);
        }
        else {
            luaL_error(L, "Unsupported return type: %s", retType);
        }
        return 1;
    }
}



#define VAL2UD(TYPE, CHECK, UD_TYPE) \
    if (CHECK(L, 1)) { \
        TYPE v = lua_to##UD_TYPE(L, 1); \
        makeUd<TYPE>(L, v); \
        return 1; \
    }

#define NILBUF() \
    if (lua_isnil(L, 1) && lua_isinteger(L, 2)) { \
        const size_t bufs = luaL_checkinteger(L, 2); \
        std::string buf(bufs, '\0'); \
        lua_pushlstring(L, buf.data(), buf.size()); \
        lua_pushinteger(L, bufs); \
        return 2; \
    }

#define NILUDSZ() \
    if (lua_isnil(L, 1) && lua_isinteger(L, 2) && (lua_isboolean(L, 3) && (bool)lua_toboolean(L, 3) == true)) { \
        const size_t udsize = luaL_checkinteger(L, 2); \
        void* ud = lua_newuserdata(L, udsize); \
        memset(ud, 0, udsize); \
        lua_pushinteger(L, udsize); \
        return 2; \
    }
#define NULLPTR() \
    if (lua_isnil(L, 1))\
    { \
        lua_pushlightuserdata(L, nullptr); \
        return 1; \
    }
#define STR2UD() \
    if (lua_isstring(L, 1)) { \
        size_t len; \
        const char* s = lua_tolstring(L, 1, &len); \
        void* ud = lua_newuserdata(L, len); \
        memcpy(ud, s, len); \
        lua_pushinteger(L, len); \
        return 2; \
    }

Lua_Function(Val2Addr)
{
    VAL2UD(lua_Integer, lua_isinteger, integer)
    VAL2UD(lua_Number, lua_isnumber, number)
    VAL2UD(bool, lua_isboolean, boolean)
    NILUDSZ()
    NILBUF()
    NULLPTR()
    STR2UD()
    lua_pushnil(L);
    return 1;
}
static bool trymemcpy(void* dest, const void* src, size_t size, uintptr_t& code)
{
    bool suc = true;
    __try {
        memcpy(dest, src, size);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        code = GetExceptionCode();
        suc = false;
    };
    return suc;
}

Lua_Function(CopyAddr)
{
    size_t size = (size_t)luaL_checkinteger(L, 2);
    if (size == 0)
        return luaL_error(L, "CopyAddr: size must be > 0");
    uintptr_t errcode = 0;
    int t = lua_type(L, 1);
    if (t == LUA_TSTRING)
    {
        const char* src = lua_tostring(L, 1);
        if (lua_toboolean(L, 3))
        {
            void* dst = lua_newuserdata(L, size);
            if (!trymemcpy(dst, src, size, errcode))
            {
                crashError(L, "CopyAddr", "Memory copy", errcode);
            }
            return 1;
        }
        std::string buf(src, size);
        lua_pushlstring(L, buf.data(), buf.size());
        return 1;
    }

    if (t == LUA_TUSERDATA)
    {
        void* src = lua_touserdata(L, 1); 
        if (lua_toboolean(L, 3))
        {
            std::string buf((char*)src, size);
            lua_pushlstring(L, buf.data(), buf.size());
            return 1;
        }
        void* dst = lua_newuserdata(L, size);
        if (!trymemcpy(dst, src, size, errcode))
        {
            crashError(L, "CopyAddr", "Memory copy", errcode);
        }
        return 1;
    }

    lua_pushnil(L);
    return 1;
}

Lua_Function(WriteAddr)
{
    void* dst = lua_touserdata(L, 1);
	int t = lua_type(L, 2);
    uintptr_t errcode = 0;
	if (t == LUA_TSTRING)
    {
        size_t len;
        const char* src = lua_tolstring(L, 2, &len);
        if (lua_isinteger(L, 3))
            len = lua_tointeger(L, 3);
        if (!trymemcpy(dst, src, len, errcode))
        {
            crashError(L, "WriteAddr", "Memory copy", errcode);
        }
        return 0;
    }
    else if (t == LUA_TUSERDATA)
    {
        void* src = lua_touserdata(L, 2);
        size_t udsize = luaL_checkinteger(L, 3);
        if (udsize == 0)
			return luaL_error(L, "WriteAddr: size must be > 0");
        if (!trymemcpy(dst, src, udsize, errcode))
        {
            crashError(L, "WriteAddr", "Memory copy", errcode);
        }
        return 0;
	}
    luaL_error(L, "WriteAddr: unsupported source type");
	return 0;
}

Lua_Function(Num2Addr)
{
    lua_Integer num = luaL_checkinteger(L, 1);
    void* addr = (void*)num;
    lua_pushlightuserdata(L, addr);
    return 1;
}
Lua_Function(Addr2Num)
{
    void* addr = (void*)lua_topointer(L, 1);
    lua_pushinteger(L, (lua_Integer)addr);
    return 1;
}

Lua_Function(VirtualQueryEx)
{
    HANDLE hProcess = luaL_wingetbycheckudata(L, 1, HANDLE);
    void* lpAddress = lua_touserdata(L, 2);

    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T result = VirtualQueryEx(hProcess, lpAddress, &mbi, sizeof(mbi));

    if (result == 0) {
        lua_pushnil(L);
        lua_pushstring(L, "VirtualQueryEx failed");
        return 2;
    }

    lua_newtable(L);

    lua_pushstring(L, "BaseAddress");
    lua_pushlightuserdata(L, mbi.BaseAddress);
    lua_settable(L, -3);

    lua_pushstring(L, "AllocationBase");
    lua_pushlightuserdata(L, mbi.AllocationBase);
    lua_settable(L, -3);

    lua_pushstring(L, "AllocationProtect");
    lua_pushinteger(L, mbi.AllocationProtect);
    lua_settable(L, -3);

    lua_pushstring(L, "RegionSize");
    lua_pushinteger(L, mbi.RegionSize);
    lua_settable(L, -3);

    lua_pushstring(L, "State");
    lua_pushinteger(L, mbi.State);
    lua_settable(L, -3);

    lua_pushstring(L, "Protect");
    lua_pushinteger(L, mbi.Protect);
    lua_settable(L, -3);

    lua_pushstring(L, "Type");
    lua_pushinteger(L, mbi.Type);
    lua_settable(L, -3);

    return 1;
}
Lua_Function(OpenProcess)
{
    DWORD dwDesiredAccess = (DWORD)luaL_checkinteger(L, 1);
    luaL_checktype(L, 2, LUA_TBOOLEAN);
    BOOL bInheritHandle = (BOOL)lua_toboolean(L, 2);
    DWORD dwProcessId = (DWORD)luaL_checkinteger(L, 3);
    HANDLE m_Proccess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
    pushWindowStruct(L, HANDLE, m_Proccess);
    return 1;
}
Lua_Function(CloseHandle)
{
    lua_pushboolean(L,CloseHandle(luaL_wingetbycheckudata(L, 1, HANDLE)));
    return 1;
}
Lua_Function(ReadProcessMemory)
{
    HANDLE hProcess = luaL_wingetbycheckudata(L, 1, HANDLE);
    LPCVOID lpBaseAddress = lua_touserdata(L, 2);
    SIZE_T nSize = (SIZE_T)luaL_checkinteger(L, 3);

    std::vector<char> buffer(nSize);
    SIZE_T bytesRead = 0;

    if (ReadProcessMemory(hProcess, lpBaseAddress, buffer.data(), nSize, &bytesRead)) {
        lua_pushlstring(L, buffer.data(), bytesRead);
        lua_pushinteger(L, bytesRead);
        return 2;
    }
    else {
        lua_pushnil(L);
        lua_pushstring(L, "ReadProcessMemory failed");
        return 2;
    }
}
Lua_Function(WriteProcessMemory)
{
    HANDLE hProcess = (HANDLE)luaL_wingetbycheckudata(L, 1, HANDLE);
    LPVOID lpBaseAddress = (LPVOID)(uintptr_t)luaL_checkinteger(L, 2);

    size_t len;
    const char* buffer = luaL_checklstring(L, 3, &len);
    SIZE_T bytesWritten = 0;

    if (WriteProcessMemory(hProcess, lpBaseAddress, buffer, len, &bytesWritten)) {
        lua_pushboolean(L, true);
        lua_pushinteger(L, bytesWritten);
        return 2;
    }
    else {
        lua_pushboolean(L, false);
        lua_pushstring(L, "WriteProcessMemory failed");
        return 2;
    }
}

Lua_Function(LOWORD)
{
    lua_Integer val = luaL_checkinteger(L, 1);
    lua_pushinteger(L, LOWORD(val));
    return 1;
}
Lua_Function(HIWORD)
{
    lua_Integer val = luaL_checkinteger(L, 1);
    lua_pushinteger(L, HIWORD(val));
    return 1;
}

Lua_Function(CoTaskMemAlloc)
{
    SIZE_T size = (SIZE_T)luaL_checkinteger(L, 1);
    void* ptr = CoTaskMemAlloc(size);
    lua_pushlightuserdata(L, ptr);
    return 1;
}
Lua_Function(CoTaskMemFree)
{
    void* ptr = lua_touserdata(L, 1);
    CoTaskMemFree(ptr);
    return 0;
}
Lua_Function(MultiByteToWideChar)
{
    UINT cp = luaL_checkinteger(L, 1);
    DWORD flags = luaL_optinteger(L, 2, 0);
    size_t mbLen;
    const char* mb = luaL_checklstring(L, 3, &mbLen);
    LPWSTR buf = (LPWSTR)lua_touserdata(L, 4);
    int wLen = (int)luaL_optinteger(L, 5, 0);

    int result = MultiByteToWideChar(cp, flags, mb, (int)mbLen, buf, wLen);

    lua_pushinteger(L, result);
    return 1;
}

// Abrir el portapapeles
Lua_Function(OpenClipboard)
{
    HWND hwnd = luaL_wingetbyudata(L, 1, HWND);
    lua_pushboolean(L, OpenClipboard(hwnd) != 0);
    return 1;
}

// Vaciar el portapapeles
Lua_Function(EmptyClipboard)
{
    lua_pushboolean(L, EmptyClipboard() != 0);
    return 1;
}

// Cerrar el portapapeles
Lua_Function(CloseClipboard)
{
    lua_pushboolean(L, CloseClipboard() != 0);
    return 1;
}

// Asignar datos al portapapeles
Lua_Function(SetClipboardData)
{
    UINT format = luaL_checkinteger(L, 1);
    HGLOBAL hMem = luaL_wingetbycheckudata(L, 2, HGLOBAL); // o userdata que contenga HGLOBAL
    lua_pushlightuserdata(L, SetClipboardData(format, hMem));
    return 1;
}

// Reservar memoria global (GlobalAlloc)
Lua_Function(GlobalAlloc)
{
    UINT flags = luaL_checkinteger(L, 1);
    SIZE_T size = luaL_checkinteger(L, 2);
    pushWindowStruct(L, HGLOBAL, GlobalAlloc(flags, size));
    return 1;
}

// Bloquear memoria global (GlobalLock)
Lua_Function(GlobalLock)
{
    HGLOBAL hMem = luaL_wingetbycheckudata(L, 1, HGLOBAL);
    lua_pushlightuserdata(L, GlobalLock(hMem));
    return 1;
}
// Desbloquear memoria global (GlobalUnlock)
Lua_Function(GlobalUnlock)
{
    HGLOBAL hMem = luaL_wingetbycheckudata(L, 1, HGLOBAL);
    lua_pushboolean(L, GlobalUnlock(hMem) != 0);
    return 1;
}

// Liberar memoria global (GlobalFree)
Lua_Function(GlobalFree)
{
    HGLOBAL hMem = luaL_wingetbycheckudata(L, 1, HGLOBAL);
    HGLOBAL result = GlobalFree(hMem);
    if (result == NULL)
        lua_pushnil(L);
    else
        pushWindowStruct(L, HGLOBAL, result);
    return 1;
}

void register_winmemory(lua_State* L)
{
    REGIMACRO(CP_ACP)
        REGIMACRO(CP_OEMCP)
        REGIMACRO(CP_MACCP)
        REGIMACRO(CP_THREAD_ACP)
        REGIMACRO(CP_UTF7)
        REGIMACRO(CP_UTF8)
        REGIMACRO(CP_SYMBOL)

        REGIMACRO(GMEM_FIXED)
        REGIMACRO(GMEM_MOVEABLE)
        REGIMACRO(GMEM_NOCOMPACT)
        REGIMACRO(GMEM_NODISCARD)
        REGIMACRO(GMEM_ZEROINIT)
        REGIMACRO(GMEM_MODIFY)
        REGIMACRO(GMEM_DISCARDABLE)
        REGIMACRO(GMEM_NOT_BANKED)
        REGIMACRO(GMEM_NOTIFY)
        REGIMACRO(GMEM_LOWER)
        REGIMACRO(GMEM_VALID_FLAGS)
        REGIMACRO(GMEM_INVALID_HANDLE)
        REGIMACRO(GMEM_DISCARDED)
        REGIMACRO(GMEM_LOCKCOUNT)

        REGIMACRO(CF_TEXT)
        REGIMACRO(CF_BITMAP)
        REGIMACRO(CF_METAFILEPICT)
        REGIMACRO(CF_SYLK)
        REGIMACRO(CF_DIF)
        REGIMACRO(CF_TIFF)
        REGIMACRO(CF_OEMTEXT)
        REGIMACRO(CF_DIB)
        REGIMACRO(CF_PALETTE)
        REGIMACRO(CF_PENDATA)
        REGIMACRO(CF_RIFF)
        REGIMACRO(CF_WAVE)
        REGIMACRO(CF_UNICODETEXT)
        REGIMACRO(CF_ENHMETAFILE)
        REGIMACRO(CF_HDROP)
        REGIMACRO(CF_LOCALE)
        REGIMACRO(CF_DIBV5)
        REGIMACRO(CF_OWNERDISPLAY)
        REGIMACRO(CF_DSPTEXT)
        REGIMACRO(CF_DSPBITMAP)
        REGIMACRO(CF_DSPMETAFILEPICT)
        REGIMACRO(CF_DSPENHMETAFILE)
        REGIMACRO(GWLP_WNDPROC)
        REGIMACRO(GWLP_HINSTANCE)
        REGIMACRO(GWLP_HWNDPARENT)
        REGIMACRO(GWL_STYLE)
        REGIMACRO(GWL_EXSTYLE)
        REGIMACRO(GWLP_USERDATA)
        REGIMACRO(GWL_ID)
}