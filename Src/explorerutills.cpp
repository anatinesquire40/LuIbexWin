#define REGNAME "openfnahookregs"
struct luacustdatadef {
    lua_State* L;
    int funcRef;
    int custDataRef;
};
static UINT_PTR Trampoline_OpenFNAHook(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LPOPENFILENAMEA ofn = nullptr;
    if (uMsg == WM_INITDIALOG) {
        ofn = (LPOPENFILENAMEA)lParam;
        SetWindowLongPtr(hdlg, GWLP_USERDATA, (LONG_PTR)ofn);
    }
    else {
        ofn = (LPOPENFILENAMEA)GetWindowLongPtr(hdlg, GWLP_USERDATA);
    }
	luacustdatadef* dataRef = (luacustdatadef*)(ofn->lCustData);
    lua_State* L = dataRef->L;
    lua_rawgeti(L, LUA_REGISTRYINDEX, dataRef->funcRef);
    pushWindowStruct(L, HWND, hdlg);
    lua_pushinteger(L, uMsg);
    lua_pushinteger(L, wParam);
    lua_pushinteger(L, lParam);
    if (lua_pcall(L, 4, 1, 0) != LUA_OK) {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "print");
        lua_pushfstring(L, "Error en Lua OpenFNA Hook: %s\n", err);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
        return 0;
    }
    UINT_PTR result = (UINT_PTR)luaL_checkinteger(L, -1);
    lua_pop(L, 1);
    return result;
}
static std::vector<std::unique_ptr<luacustdatadef>> luacustdatarefs;
static std::vector<std::string> storage;
static void cleanupRefs(lua_State* L, luacustdatadef* dataRef)
{
    
    lua_pushstring(L, REGNAME);
    lua_rawget(L, LUA_REGISTRYINDEX);
    // Limpia el vector lua
    for (size_t i = 1; i <= lua_rawlen(L, -1); ++i) {
        lua_geti(L, -1, (lua_Integer)i);
        if (lua_isinteger(L, -1)) {
            lua_Integer refIndex = lua_tointeger(L, -1);
            if (refIndex == dataRef->funcRef || refIndex == dataRef->custDataRef) {
                lua_pushnil(L);
                lua_seti(L, -3, (lua_Integer)i);
            }
        }
        lua_pop(L, 1);
	}
    // Limpia el vector global
    for (auto it = luacustdatarefs.begin(); it != luacustdatarefs.end(); ++it) {
        if (it->get() == dataRef) {
            luacustdatarefs.erase(it);
            break;
        }
    }
}

static void table2lpopenfilenamea(lua_State* L, int index, LPOPENFILENAMEA lp)
{
    ZeroMemory(lp, sizeof(OPENFILENAMEA));
    lp->lStructSize = sizeof(OPENFILENAMEA);

    if (!lua_istable(L, index)) return;
    index = lua_absindex(L, index);

    auto getStringField = [&](const char* field, bool obl = false) -> const char* {
        lua_getfield(L, index, field);
        const char* str = lua_tostring(L, -1);
		if (obl && !str) luaL_checktype(L, -1, LUA_TSTRING);
		lua_pop(L, 1);
        return str;
        };

    // Strings
    lp->lpstrFilter = (LPSTR)getStringField("lpstrFilter");
    lp->lpstrFile = (LPSTR)getStringField("lpstrFile", true);
    lp->lpstrFileTitle = (LPSTR)getStringField("lpstrFileTitle");
    lp->lpstrInitialDir = getStringField("lpstrInitialDir");
    lp->lpstrTitle = getStringField("lpstrTitle");
    lp->lpstrDefExt = getStringField("lpstrDefExt");
    lp->lpTemplateName = getStringField("lpstrTemplateName");

    // hwndOwner
    lua_getfield(L, index, "hwndOwner");
    lp->hwndOwner = lua_isuserdata(L, -1) ? *(HWND*)lua_touserdata(L, -1) : NULL;
    lua_pop(L, 1);

    // hInstance
    lua_getfield(L, index, "hInstance");
    lp->hInstance = lua_isuserdata(L, -1) ? *(HINSTANCE*)lua_touserdata(L, -1) : GetModuleHandle(NULL);
    lua_pop(L, 1);

    // nMaxFile
    lua_getfield(L, index, "nMaxFile");
    lp->nMaxFile = lua_isinteger(L, -1) ? (DWORD)lua_tointeger(L, -1) : MAX_PATH;
    lua_pop(L, 1);

    // nMaxFileTitle
    lua_getfield(L, index, "nMaxFileTitle");
    lp->nMaxFileTitle = lua_isinteger(L, -1) ? (DWORD)lua_tointeger(L, -1) : 0;
    lua_pop(L, 1);

    // Flags
    lua_getfield(L, index, "Flags");
    lp->Flags = lua_isinteger(L, -1) ? (DWORD)lua_tointeger(L, -1) : 0;
    lua_pop(L, 1);

    // Hook + lCustData
    lua_getfield(L, index, "lpfnHook");
    bool hasFunc = lua_isfunction(L, -1);
    auto dataRef = std::make_unique<luacustdatadef>();
    dataRef->L = L;
    dataRef->funcRef = 0;
    dataRef->custDataRef = 0;

    if (hasFunc) {
        dataRef->funcRef = addLWinProc(L, -1, REGNAME);
        lp->lpfnHook = Trampoline_OpenFNAHook;
    }
    else if (lua_isuserdata(L, -1)) {
        lp->lpfnHook = *(LPOFNHOOKPROC*)lua_touserdata(L, -1);
    }
    else {
        lp->lpfnHook = NULL;
    }
    lua_pop(L, 1);

    lua_getfield(L, index, "lCustData");
    if (lua_istable(L, -1)) {
        dataRef->custDataRef = addLWinProc(L, -1, REGNAME);
    }

    if (hasFunc) {
        luacustdatarefs.push_back(std::move(dataRef));
        lp->lCustData = (LPARAM)luacustdatarefs.back().get();
    }
    else if (lua_isuserdata(L, -1)) {
        lp->lCustData = (LPARAM)lua_touserdata(L, -1);
    }
    else {
        lp->lCustData = NULL;
    }
    lua_pop(L, 1);
}


// -------------------------------------------------
// GetOpenFileName
// -------------------------------------------------
Lua_Function(GetOpenFileName)
{
    OPENFILENAMEA ofn;
    table2lpopenfilenamea(L, 1, &ofn); // Tabla de opciones en stack[1]

    BOOL result = ::GetOpenFileNameA(&ofn);
    cleanupRefs(L, (luacustdatadef*)ofn.lCustData);
    lua_pushboolean(L, result);
    return 1;
}

// -------------------------------------------------
// GetSaveFileName
// -------------------------------------------------
Lua_Function(GetSaveFileName)
{
    OPENFILENAMEA ofn;
    table2lpopenfilenamea(L, 1, &ofn); // Tabla de opciones en stack[1]

    BOOL result = ::GetSaveFileNameA(&ofn);
    cleanupRefs(L, (luacustdatadef*)ofn.lCustData);
	lua_pushboolean(L, result);
    return 1;
}

// -------------------------------------------------
// Callback de BROWSEINFOA para Lua
// -------------------------------------------------
static int Trampoline_BrowseFNA(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM data)
{
    luacustdatadef* cust = (luacustdatadef*)data;
    if (!cust || !cust->L) return 0;

    lua_State* L = cust->L;
    lua_rawgeti(L, LUA_REGISTRYINDEX, cust->funcRef);

    // Pasamos los parámetros del callback a Lua
    lua_pushinteger(L, uMsg);
    lua_pushinteger(L, lParam);

    if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "print");
        lua_pushfstring(L, "Error en Lua SHBrowseForFolder Hook: %s\n", err);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
        return 0;
    }

    UINT_PTR result = (UINT_PTR)luaL_checkinteger(L, -1);
    lua_pop(L, 1);
    return result;
}

// -------------------------------------------------
// Convierte tabla Lua a BROWSEINFOA
// -------------------------------------------------
static void table2browseinfoa(lua_State* L, int index, LPBROWSEINFOA bi)
{
    ZeroMemory(bi, sizeof(BROWSEINFOA));
    if (!lua_istable(L, index)) return;
    index = lua_absindex(L, index);

    auto getStringField = [&](const char* field) -> const char* {
        lua_getfield(L, index, field);
        const char* str = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        return str;
        };

    // hwndOwner
    lua_getfield(L, index, "hwndOwner");
    bi->hwndOwner = lua_isuserdata(L, -1) ? *(HWND*)lua_touserdata(L, -1) : NULL;
    lua_pop(L, 1);

    // pidlRoot (no muy común, pero soportado)
    lua_getfield(L, index, "pidlRoot");
    bi->pidlRoot = lua_isuserdata(L, -1) ? (PCIDLIST_ABSOLUTE)lua_touserdata(L, -1) : NULL;
    lua_pop(L, 1);

    // lpszTitle
    bi->lpszTitle = getStringField("lpszTitle");

    // ulFlags
    lua_getfield(L, index, "ulFlags");
    bi->ulFlags = lua_isinteger(L, -1) ? (UINT)lua_tointeger(L, -1) : 0;
    lua_pop(L, 1);

    // lpfn callback y lParam (data)
    lua_getfield(L, index, "lpfn");
    bool hasFunc = lua_isfunction(L, -1);

    auto dataRef = std::make_unique<luacustdatadef>();
    dataRef->L = L;
    dataRef->funcRef = 0;
    dataRef->custDataRef = 0;

    if (hasFunc) {
        dataRef->funcRef = addLWinProc(L, -1, REGNAME);
        bi->lpfn = Trampoline_BrowseFNA;
    }
    else if (lua_isuserdata(L, -1)) {
        bi->lpfn = *(BFFCALLBACK*)lua_touserdata(L, -1);
    }
    else {
        bi->lpfn = nullptr;
    }
    lua_pop(L, 1);

    lua_getfield(L, index, "lParam");
    if (lua_istable(L, -1)) {
        dataRef->custDataRef = addLWinProc(L, -1, REGNAME);
    }

    if (hasFunc) {
        luacustdatarefs.push_back(std::move(dataRef));
        bi->lParam = (LPARAM)luacustdatarefs.back().get();
    }
    else if (lua_isuserdata(L, -1)) {
        bi->lParam = (LPARAM)lua_touserdata(L, -1);
    }
    else {
        bi->lParam = 0;
    }
    lua_pop(L, 1);

    // pszDisplayName (salida opcional)
    lua_getfield(L, index, "pszDisplayName");
    if (lua_isstring(L, -1)) {
        bi->pszDisplayName = (LPSTR)lua_tostring(L, -1);
    }
    else {
        bi->pszDisplayName = nullptr;
    }
    lua_pop(L, 1);
}


// -------------------------------------------------
// SHBrowseForFolder
// -------------------------------------------------
Lua_Function(SHBrowseForFolder)
{
    BROWSEINFOA bi{};
    table2browseinfoa(L, 1, &bi);

    PIDLIST_ABSOLUTE pidl = SHBrowseForFolderA(&bi);
    cleanupRefs(L, (luacustdatadef*)bi.lParam);
	pushWindowStruct(L, PIDLIST_ABSOLUTE, pidl);
    return 1;
}
Lua_Function(SHParseDisplayName)
{
    // 1. Recibir todos los parámetros desde Lua
    PCWSTR pszName = (PCWSTR)luaL_checkstring(L, 1);       // nombre/ruta
    LPBC pbc = (LPBC)lua_touserdata(L, 2); // puede ser nil
    ULONG sfgaoIn = (ULONG)luaL_optinteger(L, 3, 0); // atributos de entrada opcionales

    // 2. Preparar salida
    PIDLIST_ABSOLUTE pidl = nullptr;
    ULONG sfgaoOut = 0;
    // 3. Llamada a la API
    HRESULT hr = SHParseDisplayName(pszName, pbc, &pidl, sfgaoIn, &sfgaoOut);
    if (FAILED(hr)) {
        lua_pushnil(L);
        lua_pushinteger(L, hr); // devolver HRESULT si falla
        return 2;
    }

    // 4. Empujar resultados a Lua
    pushWindowStruct(L, PIDLIST_ABSOLUTE, pidl); // PIDLIST_ABSOLUTE como userdata
    lua_pushinteger(L, sfgaoOut);               // atributos de salida

    return 2; // userdata + sfgaoOut
}

Lua_Function(SHGetPathFromIDList)
{
    PCIDLIST_ABSOLUTE pidl = luaL_wingetbycheckudata(L, 1, PIDLIST_ABSOLUTE);
    size_t len;
    LPSTR path = (LPSTR)luaL_checklstring(L, 2, &len);
    BOOL result = SHGetPathFromIDListA(pidl, path);
    lua_pushboolean(L, result);
    return 1;
}
void register_explorerutills(lua_State* L)
{
    REGIMACRO(MAX_PATH)
    REGIMACRO(BIF_RETURNONLYFSDIRS)
    REGIMACRO(BIF_DONTGOBELOWDOMAIN)
    REGIMACRO(BIF_STATUSTEXT)
    REGIMACRO(BIF_RETURNFSANCESTORS)
    REGIMACRO(BIF_EDITBOX)
    REGIMACRO(BIF_VALIDATE)
    REGIMACRO(BIF_NEWDIALOGSTYLE)
    REGIMACRO(BIF_USENEWUI)
    REGIMACRO(BIF_BROWSEINCLUDEFILES)
    REGIMACRO(BIF_BROWSEFORCOMPUTER)
    REGIMACRO(BIF_BROWSEFORPRINTER)
    REGIMACRO(BIF_BROWSEINCLUDEURLS)
    REGIMACRO(BIF_BROWSEFILEJUNCTIONS)
    REGIMACRO(OFN_READONLY)
    REGIMACRO(OFN_OVERWRITEPROMPT)
    REGIMACRO(OFN_HIDEREADONLY)
    REGIMACRO(OFN_NOCHANGEDIR)
    REGIMACRO(OFN_SHOWHELP)
    REGIMACRO(OFN_ENABLEHOOK)
    REGIMACRO(OFN_ENABLETEMPLATE)
    REGIMACRO(OFN_ENABLETEMPLATEHANDLE)
    REGIMACRO(OFN_NOVALIDATE)
    REGIMACRO(OFN_ALLOWMULTISELECT)
    REGIMACRO(OFN_EXTENSIONDIFFERENT)
    REGIMACRO(OFN_PATHMUSTEXIST)
    REGIMACRO(OFN_FILEMUSTEXIST)
    REGIMACRO(OFN_CREATEPROMPT)
    REGIMACRO(OFN_SHAREAWARE)
    REGIMACRO(OFN_NOREADONLYRETURN)
    REGIMACRO(OFN_NOTESTFILECREATE)
    REGIMACRO(OFN_NONETWORKBUTTON)
    REGIMACRO(OFN_NOLONGNAMES)
    REGIMACRO(OFN_EXPLORER)
    REGIMACRO(OFN_NODEREFERENCELINKS)
    REGIMACRO(OFN_LONGNAMES)
    REGIMACRO(OFN_ENABLEINCLUDENOTIFY)
    REGIMACRO(OFN_ENABLESIZING)
    REGIMACRO(OFN_DONTADDTORECENT)
    REGIMACRO(OFN_FORCESHOWHIDDEN)

}