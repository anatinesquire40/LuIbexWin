#pragma comment(lib, "comctl32.lib")

Lua_Function(InitCommonControlsEx)
{

    INITCOMMONCONTROLSEX icex{};
    icex.dwSize = (DWORD)sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = (DWORD)luaL_optinteger(L, 1, ICC_STANDARD_CLASSES);

    BOOL ok = InitCommonControlsEx(&icex);
    lua_pushboolean(L, ok);
    return 1;
}
void register_commcontrol(lua_State* L)
{
    // Iconos
    REGUMACRO(LPWSTR, TD_INFORMATION_ICON)
    REGUMACRO(LPWSTR, TD_WARNING_ICON)
    REGUMACRO(LPWSTR, TD_ERROR_ICON)
    REGUMACRO(LPWSTR, TD_SHIELD_ICON)

    // Botones
    REGIMACRO(TDCBF_OK_BUTTON)
    REGIMACRO(TDCBF_YES_BUTTON)
    REGIMACRO(TDCBF_NO_BUTTON)
    REGIMACRO(TDCBF_CANCEL_BUTTON)
    REGIMACRO(TDCBF_RETRY_BUTTON)
    REGIMACRO(TDCBF_CLOSE_BUTTON)
}