Lua_Function(LoadCursor)
{
    HINSTANCE hInstance = luaL_wingetbyudata(L, 1, HINSTANCE);

    LPCSTR lpCursorName = luaL_wingetbycheckudata(L, 2, LPCSTR);
    HCURSOR cursor = LoadCursorA(hInstance, lpCursorName);
    pushWindowStruct(L, HCURSOR, cursor);
    return 1;
}
void register_cursor(lua_State* L)
{
        REGUMACRO(LPSTR, IDC_ARROW)         // 32512
        REGUMACRO(LPSTR, IDC_IBEAM)         // 32513
        REGUMACRO(LPSTR, IDC_WAIT)          // 32514
        REGUMACRO(LPSTR, IDC_CROSS)         // 32515
        REGUMACRO(LPSTR, IDC_UPARROW)       // 32516
        REGUMACRO(LPSTR, IDC_SIZE)          // 32640
        REGUMACRO(LPSTR, IDC_ICON)          // 32641
        REGUMACRO(LPSTR, IDC_SIZENWSE)      // 32642
        REGUMACRO(LPSTR, IDC_SIZENESW)      // 32643
        REGUMACRO(LPSTR, IDC_SIZEWE)        // 32644
        REGUMACRO(LPSTR, IDC_SIZENS)        // 32645
        REGUMACRO(LPSTR, IDC_SIZEALL)       // 32646
        REGUMACRO(LPSTR, IDC_NO)            // 32648
#if(WINVER >= 0x0500)
        REGUMACRO(LPSTR, IDC_HAND)          // 32649
#endif

        REGUMACRO(LPSTR, IDC_APPSTARTING)   // 32650

#if(WINVER >= 0x0400)
        REGUMACRO(LPSTR, IDC_HELP)          // 32651
#endif

#if(WINVER >= 0x0606)
        REGUMACRO(LPSTR, IDC_PIN)           // 32671
        REGUMACRO(LPSTR, IDC_PERSON)        // 32672
#endif
}
