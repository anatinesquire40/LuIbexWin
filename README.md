# Lua Windows

**Lua Windows (`lwindows`)** is a specialized module that allows Lua to interact directly with the Windows API (Win32).  
It provides access to native functions, structures, messages, and system utilities, making Lua a powerful tool for developing applications and scripts that require deep control of the Windows environment.

### Compatibility
Lua 5.3 and newer.

---

## Prerequisites

Before compiling and using `lwindows`, make sure you have:

1. **CMake ≥ 3.16**  
2. **Visual Studio** with C++ and MASM support  
3. **Lua** installation (5.3 or newer)  
4. Sufficient disk space (as `lwindows` exposes many functions and complex structures)

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
4. Compile the library:

   ```bat
   cmake --build . --config Release
   ```

   This will produce `lwindows.dll` ready to use.

---

## Installation

To make Lua use `lwindows` correctly:

1. **Lua Modules:**
   Copy the `windows` directory (containing `h.lua` and other scripts) into your Lua modules folder (`package.path`).

2. **Native Modules:**
   Copy `lwindows.dll` into your Lua native libraries folder (`package.cpath`).

3. Make sure `package.path` and `package.cpath` include these directories so that `require "windows.h"` and `require "lwindows"` work properly.
