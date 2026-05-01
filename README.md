# LuIbexWin

**LuIbexWin** is a powerful Lua module that bridges Lua scripting with the Windows API (Win32), enabling developers to create native Windows applications, automate system tasks, and access low-level Windows functionality directly from Lua. It provides safe memory management, window procedure handling, and seamless integration with Win32 functions, making it ideal for GUI development, system utilities, and advanced scripting scenarios.

## What is LuIbexWin For?

LuIbexWin serves as a comprehensive wrapper for the Windows API, allowing Lua developers to:
- Create and manage Windows GUI applications with native performance
- Interact with system resources like processes, files, and memory
- Handle Windows messages and events through Lua functions
- Perform low-level operations safely without risking crashes
- Build cross-platform Lua applications with Windows-specific features
- Automate Windows tasks and integrate with existing Win32 codebases

This module is particularly useful for developers who want to leverage Lua's simplicity and speed for Windows development, avoiding the complexities of C++ while maintaining full access to system capabilities.

### Compatibility
- Lua 5.3 and newer
- 64-bit Windows only
- MSVC compiler required

---

## Prerequisites

Before compiling and using `LuIbexWin`, make sure you have:

1. **CMake ≥ 3.16**
2. **Visual Studio** with C++ and MASM support (as `LuIbexWin` only supports `MSVC`)
3. **Lua** installation (5.3 or newer) and in x64
4. Sufficient disk space (as `LuIbexWin` exposes many functions and complex structures)

---

## Lua Setup

You can see that the lua folder is empty, you need to put the includes and the library that the module requires.
1. Includes
Create a folder in the lua folder called `include`
In the Lua Source folder go to `src\` and copy: `lua.h, lua.hpp, lauxlib.h, luaconf.h` and `lualib.h` to `include`
2. Library
Create a folder in the lua folder called `lib`
In the build of the Lua Source you will find `luaXX.lib`, copy it to `lib` and rename to `lua.lib`
---

## Library Compilation

1. Open a terminal in the project directory.
2. Create a build directory and navigate to it:

   ```bat
   mkdir build
   cd build
   ```
3. Generate build files with CMake:

   ```bat
   cmake ..
   ```
   if you want to configure as static build just use:
   ```bat
   cmake -DBUILD_STATIC=ON ..
   ```
4. Compile the library:

   ```bat
   cmake --build . --config Release
   ```
   This will produce `luibexwin.dll` ready to use.

---

## Installation

To make Lua use `LuIbexWin` correctly:

1. **Lua Modules:**
   Copy the `ibexwin` directory (containing `h.lua` and other scripts) into your Lua modules folder (`package.path`).

2. **Native Modules:**
   Copy `luibexwin.dll` into your Lua native libraries folder (`package.cpath`).

3. Make sure `package.path` and `package.cpath` include these directories so that `require "ibexwin.h"` work properly.

---

## Prebuilt Binaries

Precompiled binaries for LuIbexWin are available here:

👉 https://sourceforge.net/projects/luibexwin-binaries

---

## Usage

To use LuIbexWin in your Lua scripts:

```lua
require "ibexwin.h"
```

### Main Functions

- `ToWindowProc(hWnd, lfnc)`: Converts a Lua function into a Win32 WNDPROC
- `Num2Addr(int)`: Converts integer to userdata pointer
- `Addr2Num(userdata)`: Converts pointer to integer
- `Val2Addr(...)`: Creates memory blocks from Lua values
- `Addr2Val(...)`: Reads memory or converts native functions
- `CopyAddr(src, size, switch)`: Copies memory blocks
- `WriteAddr(dest, src, size)`: Writes to memory
- `LoopMessages()`: Processes Windows message queue

### Type Aliases
- `i/integer`, `d/double`, `f/float`, `s/string`, `u/userdata`, `b/boolean`, `p/lightuserdata`, `v/void`

### Common Constants
- Window styles: `WS_OVERLAPPEDWINDOW`, `WS_CHILD`, `WS_VISIBLE`
- Message box types: `MB_OK`, `MB_YESNO`
- Colors: `COLOR_WINDOW`, `COLOR_BTNFACE`
- Messages: `WM_DESTROY`, `WM_COMMAND`

---

## Examples

### Example 1: Calling a Windows API Function

```lua
require "ibexwin.h"

-- Get kernel32.dll handle
local k32 = GetModuleHandle("kernel32.dll")
-- Get address of GetTickCount
local GetTickCount_addr = GetProcAddress(k32, "GetTickCount")
-- Convert to Lua callable function
local gettick = Addr2Val(Num2Addr(GetTickCount_addr), "integer", {})
-- Call it
print("Ticks:", gettick())

-- Get computer name
local nameBuf, bufsz = Val2Addr(nil, 256)
local sizeRef = Val2Addr(bufsz)
local getComputerName = Addr2Val(
    Num2Addr(GetProcAddress(k32, "GetComputerNameA")),
    "boolean",
    {"string", "lightuserdata"}
)
if getComputerName(nameBuf, sizeRef) then
    print("PC:", Addr2Val(nameBuf, "string"))
end
```

### Example 2: Creating a Simple Window

```lua
require "ibexwin.h"

local function onButtonClick(hwnd)
    MessageBox(hwnd, "Hello world!", "My first window", MB_OK)
end

local winProc = function(hWnd, uMsg, wParam, lParam)
    if uMsg == WM_DESTROY then
        PostQuitMessage(0)
        return 0
    elseif uMsg == WM_COMMAND then
        if LOWORD(wParam) == 1001 then
            onButtonClick(hWnd)
        end
    end
    return DefWindowProc(hWnd, uMsg, wParam, lParam)
end

local CLASS_NAME = "LuaClass"
local wc = {
    style = CS_HREDRAW | CS_VREDRAW,
    lpfnWndProc = ToWindowProc(nil, winProc),  -- Convert Lua function to WNDPROC
    cbClsExtra = 0,
    cbWndExtra = 0,
    hInstance = GetModuleHandle(nil),
    hIcon = nil,
    hCursor = LoadCursor(nil, IDC_ARROW),
    hbrBackground = COLOR_WINDOW + 1,
    lpszMenuName = nil,
    lpszClassName = CLASS_NAME,
    hIconSm = nil
}
RegisterClassEx(wc)

local hwnd = CreateWindow(
    CLASS_NAME,
    "Hello World from Lua",
    WS_OVERLAPPEDWINDOW,
    100, 100, 400, 300,
    nil, nil,
    wc.hInstance
)

local hButton = CreateWindow(
    "BUTTON",
    "Click me",
    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
    50, 50, 100, 30,
    hwnd,
    1001,
    wc.hInstance
)

ShowWindow(hwnd, SW_SHOW)
UpdateWindow(hwnd)
LoopMessages()
```

This creates a window with a clickable button. LuIbexWin handles message routing and memory management automatically.

---

## Advanced Tips and Safety

- LuIbexWin uses SEH (`__try/__except`) to prevent crashes from memory faults
- Keep closures alive while windows are active to avoid invalid WNDPROC
- Use `Val2Addr` for passing values to Win32 functions
- Limit arguments to 16 for `Addr2Val` native functions
- Return `DefWindowProc` for unhandled messages
- Messages are routed by HWND and class name automatically
- No need to specify `sizeof` for standard structs like RECT, POINT

For more details, see the full documentation in the wiki.
