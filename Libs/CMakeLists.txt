# Include FetchContent module
include(FetchContent)

# Include Raylib
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# Include Box2D
FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v2.4.0
)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "Build Box2D unit tests" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "Build Box2D testbed" FORCE)
FetchContent_MakeAvailable(box2d)

# Include Sol2
FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(sol2)

# Include Lua
FetchContent_Declare(
        lua
        GIT_REPOSITORY https://github.com/lua/lua.git
        GIT_TAG v5.4.4
)
FetchContent_MakeAvailable(lua)
set(LUA_SOURCES
        ${lua_SOURCE_DIR}/lapi.c
        ${lua_SOURCE_DIR}/lauxlib.c
        ${lua_SOURCE_DIR}/lbaselib.c
        ${lua_SOURCE_DIR}/lcode.c
        ${lua_SOURCE_DIR}/lcorolib.c
        ${lua_SOURCE_DIR}/lctype.c
        ${lua_SOURCE_DIR}/ldblib.c
        ${lua_SOURCE_DIR}/ldebug.c
        ${lua_SOURCE_DIR}/ldo.c
        ${lua_SOURCE_DIR}/ldump.c
        ${lua_SOURCE_DIR}/lfunc.c
        ${lua_SOURCE_DIR}/lgc.c
        ${lua_SOURCE_DIR}/linit.c
        ${lua_SOURCE_DIR}/liolib.c
        ${lua_SOURCE_DIR}/llex.c
        ${lua_SOURCE_DIR}/lmathlib.c
        ${lua_SOURCE_DIR}/lmem.c
        ${lua_SOURCE_DIR}/loadlib.c
        ${lua_SOURCE_DIR}/lobject.c
        ${lua_SOURCE_DIR}/lopcodes.c
        ${lua_SOURCE_DIR}/loslib.c
        ${lua_SOURCE_DIR}/lparser.c
        ${lua_SOURCE_DIR}/lstate.c
        ${lua_SOURCE_DIR}/lstring.c
        ${lua_SOURCE_DIR}/lstrlib.c
        ${lua_SOURCE_DIR}/ltable.c
        ${lua_SOURCE_DIR}/ltablib.c
        ${lua_SOURCE_DIR}/ltests.c
        ${lua_SOURCE_DIR}/ltm.c
        ${lua_SOURCE_DIR}/lua.c
        ${lua_SOURCE_DIR}/lundump.c
        ${lua_SOURCE_DIR}/lutf8lib.c
        ${lua_SOURCE_DIR}/lvm.c
        ${lua_SOURCE_DIR}/lzio.c
        ${lua_SOURCE_DIR}/lzio.h
)
add_library(Lua_static STATIC ${LUA_SOURCES})
target_include_directories(Lua_static PUBLIC ${lua_SOURCE_DIR})
