require"lwindows"
function CreateWindow(...)
    return CreateWindowEx(nil, ...)
end
MessageBox = MessageBoxEx
function GetModuleHandle(mod)
    local addr = Val2Addr(nil)
    local hmod = Addr2Struct(addr, "HMODULE")
    GetModuleHandleEx(nil, mod, hmod)
    return hmod
end
Sleep = SleepEx
DrawText = DrawTextEx
function CreateDirectory(...)
    return CreateDirectoryEx(nil, ...)
end