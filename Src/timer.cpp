#define REGNAME "timerProcRegs"
typedef std::function<void(HWND, UINT, UINT_PTR, DWORD)> TIMERPROCfn;
std::unordered_map<UINT_PTR, TIMERPROCfn> timerproc_callbacks;
std::unordered_map<UINT_PTR, int> timerproc_luarefs;
static void trampoline(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	auto it = timerproc_callbacks.find(idEvent);
	if (it != timerproc_callbacks.end())
	{
		it->second(hWnd, uMsg, idEvent, dwTime);
	}
}
static bool safeCall(lua_State* L, bool& islExcept, unsigned long& err)
{
	islExcept = false;
	err = 0;
	int islOk = 0;
	__try {
		islOk = lua_pcall(L, 4, 1, 0);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		islExcept = true;
		err = GetExceptionCode();
	}
	return islOk == LUA_OK;
}
static TIMERPROCfn makeLambda(lua_State* L, int findex, UINT_PTR nIDEvent)
{
	int ref = addLWinProc(L, findex, REGNAME);
	timerproc_luarefs[nIDEvent] = ref;
	return [L, ref](HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) -> void {
		if (getLWinProci(L, ref, REGNAME))
		{
			pushWindowStruct(L, HWND, hWnd);
			lua_pushinteger(L, uMsg);
			lua_pushinteger(L, (lua_Integer)idEvent);
			lua_pushinteger(L, (lua_Integer)dwTime);
			bool islExcept = false;
			unsigned long err = 0;
			bool islOk = safeCall(L, islExcept, err);
			if (islExcept) {
				lua_getglobal(L, "print");
				lua_pushfstring(L, "Exception 0x%d in TimerProc.", err);
				lua_call(L, 1, 0);
				return;
			}
			if (!islOk) {
				const char* err = lua_tostring(L, -1);
				lua_getglobal(L, "print");
				luaL_traceback(L, L, err, 1);
				lua_call(L, 1, 0);
				lua_pop(L, 1);
				return;
			}
			lua_pop(L, 1);
			return;
		}
		else {
			lua_getglobal(L, "print");
			lua_pushfstring(L, "Invalid TimerProc reference. %d", ref);
			return;
		}
		};
}
Lua_Function(SetTimer)
{
	HWND hWnd = luaL_wingetbyudata(L, 1, HWND);
	UINT_PTR nIDEvent = (UINT_PTR)luaL_checkinteger(L, 2);
	UINT uElapse = (UINT)luaL_checkinteger(L, 3);
	TIMERPROC lpTimerFunc = NULL;
	if (lua_islightuserdata(L, 4)) {
		lpTimerFunc = (TIMERPROC)lua_touserdata(L, 4);
	} else if (lua_isfunction(L, 4)) {
		lpTimerFunc = trampoline;
	}
	UINT_PTR result = SetTimer(hWnd, nIDEvent, uElapse, lpTimerFunc);
	timerproc_callbacks[result] = makeLambda(L, 4, result);
	lua_pushinteger(L, (lua_Integer)result);
	return 1;
}
Lua_Function(KillTimer)
{
	HWND hWnd = luaL_wingetbyudata(L, 1, HWND);
	UINT_PTR nIDEvent = (UINT_PTR)luaL_checkinteger(L, 2);
	bool res = KillTimer(hWnd, nIDEvent);
	timerproc_callbacks.erase(nIDEvent);
	bool res2 = removeLWinProc(L, timerproc_luarefs[nIDEvent], REGNAME);
	timerproc_luarefs.erase(nIDEvent);
	lua_pushboolean(L, res && res2);
	return 1;
}