require"luibexwin"
function CreateWindow(...)
    return CreateWindowEx(nil, ...)
end
MessageBox = MessageBoxEx
function GetModuleHandle(mod)
    local hmodaddr = Val2Addr(nil, 8, true)
    GetModuleHandleEx(nil, mod, hmodaddr)
    local hmod_ptr = Addr2Val(hmodaddr, 0)
    local hmod = Num2Addr(hmod_ptr)
    return hmod
end
local ValueTypes = {
    integer = 0,
    boolean = 1,
    double = 2,
    float = 3,
    userdata = 4,
    string = 5,
    void = 6
}
function Addr2Val1(ptr, retType, argTypes, ...)
    local retTypeN = ValueTypes[retType]
    local argTypesN = {}
    for i,v in ipairs(argTypes) do
        argTypesN[i] = ValueTypes[v]
    end
    return Addr2Val(ptr, retTypeN, argTypesN, ...)
end
Sleep = SleepEx
DrawText = DrawTextEx