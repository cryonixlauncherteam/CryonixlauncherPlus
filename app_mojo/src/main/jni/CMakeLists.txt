
cmake_minimum_required(VERSION 3.22.1)

# ------ MojoExec ------

add_subdirectory(mojoexec)

# ------   GLFW   ------

set(GLFW_BUILD_WAYLAND  OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11      OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_ANDROID  ON  CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS   ON  CACHE BOOL "" FORCE)

add_subdirectory(glfw)

target_compile_options(glfw PRIVATE -Wno-nullability-completeness)

# ------   SDL   ------

set(SDL_SHARED       ON CACHE BOOL "" FORCE)
set(SDL_VIDEO        ON CACHE BOOL "" FORCE)
set(SDL_AUDIO		 ON CACHE BOOL "" FORCE)
set(SDL_STATIC       OFF CACHE BOOL "" FORCE)
set(SDL_CAMERA		 OFF CACHE BOOL "" FORCE)
set(SDL_TESTS		 OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES	 OFF CACHE BOOL "" FORCE)
set(SDL3_MAINPROJECT OFF CACHE BOOL "" FORCE)
add_subdirectory(sdl)
target_link_libraries(SDL3-shared PRIVATE mojoexec)

# ----------------------

project("MojoLauncher")

add_library(pojavexec SHARED
        affinity.c
        jvm_hooks/emui_iterator_fix_hook.c
        jvm_hooks/java_exec_hooks.c
        jvm_hooks/lwjgl_dlopen_hook.c
        jre_launcher/jre_launcher.c
        jre_launcher/native_library_hook.c
        jre_launcher/elf_hinter.c
        jre_launcher/abort_wait.c
        utils.c
        stdio_is.c
        minibridge.c
)

target_include_directories(pojavexec PRIVATE .)
target_link_libraries(pojavexec PRIVATE log mojoexec)

add_library(pojavexec_awt SHARED
		awt/awt.c
		awt/awt_window.c
		awt/awt_util.c
		awt/awt_input.c
		awt/awt_clipboard.c
        awt/awt_mapper.c
)

target_link_libraries(pojavexec_awt PRIVATE android log)

add_library(awt_headless SHARED 
	awt_xawt/dummy.c
)

add_library(awt_xawt SHARED
        awt_xawt/xawt_fake.c
)
target_link_libraries(awt_xawt PRIVATE awt_headless)

# ------ Cryonix Launcher Discord Rich Presence ------
# The Gradle helper extracts the SDK headers/native library from the local AAR.
add_library(cryonix_discord_presence SHARED discord_presence.cpp)
set_target_properties(cryonix_discord_presence PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
target_link_libraries(cryonix_discord_presence PRIVATE log android)

set(CRYONIX_DISCORD_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/discord_sdk")
set(CRYONIX_DISCORD_SDK_HEADER "${CRYONIX_DISCORD_SDK_DIR}/include/discordpp.h")
set(CRYONIX_DISCORD_SDK_LIB "${CRYONIX_DISCORD_SDK_DIR}/${ANDROID_ABI}/libdiscord_partner_sdk.so")
if(EXISTS "${CRYONIX_DISCORD_SDK_HEADER}" AND EXISTS "${CRYONIX_DISCORD_SDK_LIB}")
    add_library(discord_partner_sdk SHARED IMPORTED)
    set_target_properties(discord_partner_sdk PROPERTIES IMPORTED_LOCATION "${CRYONIX_DISCORD_SDK_LIB}")
    target_include_directories(cryonix_discord_presence PRIVATE "${CRYONIX_DISCORD_SDK_DIR}/include")
    target_compile_definitions(cryonix_discord_presence PRIVATE CRYONIX_HAS_DISCORD_SDK=1)
    target_link_libraries(cryonix_discord_presence PRIVATE discord_partner_sdk)
else()
    message(WARNING "Cryonix Discord SDK binary is unavailable for ${ANDROID_ABI}; Rich Presence will be disabled for this ABI.")
endif()
