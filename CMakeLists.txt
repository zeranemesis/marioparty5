cmake_minimum_required(VERSION 3.25)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# obtain revision info from git
find_package(Git)
if (GIT_FOUND)
    # make sure version information gets re-run when the current Git HEAD changes
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse --git-path HEAD
            OUTPUT_VARIABLE mp5native_git_head_filename
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${mp5native_git_head_filename}")

    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse --symbolic-full-name HEAD
            OUTPUT_VARIABLE mp5native_git_head_symbolic
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT_EXECUTABLE} rev-parse --git-path ${mp5native_git_head_symbolic}
            OUTPUT_VARIABLE mp5native_git_head_symbolic_filename
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${mp5native_git_head_symbolic_filename}")

    # defines MP5_NATIVE_WC_REVISION
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
            OUTPUT_VARIABLE MP5_NATIVE_WC_REVISION
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    # defines MP5_NATIVE_WC_DESCRIBE
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} describe --tags --long --dirty --match "v*"
            OUTPUT_VARIABLE MP5_NATIVE_WC_DESCRIBE
            OUTPUT_STRIP_TRAILING_WHITESPACE)

    # remove the git hash, then collapse a clean "-0" suffix only
    string(REGEX REPLACE "-[^-]+(-dirty|)$" "\\1" MP5_NATIVE_WC_DESCRIBE "${MP5_NATIVE_WC_DESCRIBE}")
    string(REGEX REPLACE "-0$" "" MP5_NATIVE_WC_DESCRIBE "${MP5_NATIVE_WC_DESCRIBE}")

    # defines MP5_NATIVE_WC_BRANCH
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
            OUTPUT_VARIABLE MP5_NATIVE_WC_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    # defines MP5_NATIVE_WC_DATE
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} log -1 --format=%ad
            OUTPUT_VARIABLE MP5_NATIVE_WC_DATE
            OUTPUT_STRIP_TRAILING_WHITESPACE)
else ()
    message(STATUS "Unable to find git, commit information will not be available")
endif ()

if (MP5_NATIVE_WC_DESCRIBE MATCHES "^v([0-9]+)\\.([0-9]+)\\.([0-9]+)(-([0-9]+).*)?$")
    set(MP5_NATIVE_SHORT_VERSION_STRING "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}")
    if (CMAKE_MATCH_5)
        set(MP5_NATIVE_VERSION_STRING "${MP5_NATIVE_SHORT_VERSION_STRING}.${CMAKE_MATCH_5}")
    else ()
        set(MP5_NATIVE_VERSION_STRING "${MP5_NATIVE_SHORT_VERSION_STRING}.0")
    endif ()
else ()
    set(MP5_NATIVE_WC_DESCRIBE "UNKNOWN-VERSION")
    set(MP5_NATIVE_VERSION_STRING "0.0.0.0")
    set(MP5_NATIVE_SHORT_VERSION_STRING "0.0.0")
endif ()

# Add version information to CI environment variables
if(DEFINED ENV{GITHUB_ENV})
    file(APPEND "$ENV{GITHUB_ENV}" "MP5_NATIVE_VERSION=${MP5_NATIVE_WC_DESCRIBE}\n")
endif()

message(STATUS "MP5 Native version set to ${MP5_NATIVE_WC_DESCRIBE}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
project(mp5native LANGUAGES C CXX VERSION ${MP5_NATIVE_VERSION_STRING})
if (APPLE)
    enable_language(OBJC)
endif ()
if(APPLE AND NOT CMAKE_OSX_SYSROOT)
    # If the Xcode SDK is lagging behind system version, CMake needs this done first
    execute_process(COMMAND xcrun --sdk macosx --show-sdk-path
            OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
            OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

# Aurora doesn't have prebuilts for x86_64 macOS and arm64 Windows yet
if (APPLE AND CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
    set(AURORA_DAWN_PROVIDER "vendor" CACHE STRING "Dawn provider" FORCE)
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_cmake")

include(FetchContent)

FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
        URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

# GP5E01_00, the only version the decompilation targets
set(VERSION 0)
set(MP5_NATIVE_BUNDLE_NAME Mp5Native)
set(MP5_NATIVE_BUNDLE_IDENTIFIER io.github.zeranemesis.mp5native)
set(MP5_NATIVE_COMPANY_NAME "zeranemesis")
set(MP5_NATIVE_FILE_DESCRIPTION "MP5 Native")
set(MP5_NATIVE_PRODUCT_NAME "Mp5Native")
set(MP5_NATIVE_COPYRIGHT "Copyright (C) MP5 Native contributors")

option(USE_ASAN "Enable address sanitization" OFF)
option(SKIP_HU_ALLOC "Skip the game's built in allocator" OFF)
option(RUNTIME_SANITIZE "Runtime (stack) sanitization" OFF)

set(USE_ASAN OFF CACHE BOOL "Disable ASan" FORCE)
set(AURORA_ENABLE_DVD ON)
set(AURORA_ENABLE_RMLUI ON CACHE BOOL "Enable RmlUi UI support" FORCE)
set(RUNTIME_SANITIZE OFF)
set(SKIP_HU_ALLOC OFF CACHE BOOL "Skip the built in allocator" FORCE)

# Set build type to RelWithDebInfo if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif ()

if ((CMAKE_SYSTEM_NAME STREQUAL Linux) OR APPLE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_BUILD_RPATH "$ORIGIN")
elseif (MSVC)
    add_compile_options(/bigobj)
endif ()

# Settings shared by dol and every REL module
function(mp5native_game_target target)
    target_include_directories(${target} PRIVATE include src extern extern/aurora/include extern/aurora/include/dolphin ${CMAKE_BINARY_DIR})
    # SDK headers Aurora does not provide (THP file format...) come from the
    # decompilation, after Aurora's, so Aurora always wins when both exist
    target_include_directories(${target} AFTER PRIVATE libs/dolphin/include)
    target_compile_definitions(${target} PRIVATE TARGET_PC NON_MATCHING OPTIMIZED_TEXTURE_LOADING BYTESWAPPING VERSION=${VERSION})
    if (MSVC)
        target_compile_definitions(${target} PRIVATE _USE_MATH_DEFINES _CRT_SECURE_NO_WARNINGS)
        # A pointer stored in a 32-bit integer (or the reverse) works on GameCube
        # and corrupts memory on a 64-bit host: make it an error, not a warning.
        target_compile_options(${target} PRIVATE /we4311 /we4312)
    else ()
        target_compile_options(${target} PRIVATE -Werror=pointer-to-int-cast -Werror=int-to-pointer-cast)
    endif ()
endfunction()

add_subdirectory(extern/aurora EXCLUDE_FROM_ALL)
add_subdirectory(extern/musyx EXCLUDE_FROM_ALL)
# Mario Party 5 links MusyX 2.0.4 (configure.py). Every target that includes a
# MusyX header must see the same version as the library itself.
target_compile_definitions(musyx PUBLIC MUSY_VERSION_MAJOR=2 MUSY_VERSION_MINOR=0 MUSY_VERSION_PATCH=4)
if (MSVC)
    target_compile_definitions(musyx PRIVATE _USE_MATH_DEFINES)
endif ()

include(extern/aurora/cmake/AuroraCopyRuntimeDLLs.cmake)

configure_file(${CMAKE_SOURCE_DIR}/version.h.in ${CMAKE_BINARY_DIR}/mp5native_version.h)
include(files.cmake)

source_group("Game" FILES ${GAME_FILES})
source_group("Port" FILES ${PORT_FILES})

add_library(dol SHARED ${GAME_FILES} ${PORT_FILES} extern/libco/libco.c)
set_target_properties(dol PROPERTIES C_VISIBILITY_PRESET default CXX_VISIBILITY_PRESET default)
mp5native_game_target(dol)
target_compile_definitions(dol PRIVATE TARGET_DOL)
# RELs link against dol like a GameCube module links against main.dol: every
# function is exported, and shared data is marked SHARED_SYM (dllexport/dllimport).
set_target_properties(dol PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_link_libraries(dol PUBLIC aurora::core aurora::card aurora::dvd aurora::gx aurora::mtx aurora::os aurora::si aurora::vi aurora::pad musyx absl::flat_hash_map nlohmann_json::nlohmann_json fmt::fmt rmlui_debugger)
# TODO it doesn't work on Linux yet
if (WIN32)
    if(USE_ASAN)
        target_compile_definitions(dol PRIVATE USE_ASAN)
        set_source_files_properties(${GAME_FILES} PROPERTIES COMPILE_OPTIONS "-fsanitize=address")
    endif()
    if(SKIP_HU_ALLOC)
        target_compile_definitions(dol PRIVATE SKIP_HU_ALLOC)
    endif()
endif()
if (MSVC)
    # target_compile_options(dol PRIVATE "/WX")
    target_compile_options(dol PRIVATE "/wd4244") # implicit narrowing conversion
    target_compile_options(dol PRIVATE "/wd4101") # unreferenced local variable
    target_compile_options(dol PRIVATE "/wd4102") # unreferenced label
    target_compile_options(dol PRIVATE "/wd4305") # truncation
    target_compile_options(dol PRIVATE "/wd4018") # signed/unsigned mismatch
    target_compile_options(dol PRIVATE "/wd4996") # unsafe sprintf, etc
    target_compile_options(dol PRIVATE "/wd4146") # unary minus operator applied to unsigned type, result still unsigned
    target_compile_options(dol PRIVATE "/wd4552") # result of expression not used
    target_compile_options(dol PRIVATE "/wd4553") # result of expression not used: == vs =
    target_compile_options(dol PRIVATE "/wd4113") # function pointer parameter list mismatch
    # target_compile_options(dol PRIVATE "/Zi")
    #    target_compile_options(dol PRIVATE "/Ob0")
    target_compile_options(dol PRIVATE "/Od")
    if(NOT USE_ASAN AND RUNTIME_SANITIZE)
        target_compile_options(dol PRIVATE "/GS")
        target_compile_options(dol PRIVATE "/RTC1")  # incompatible with asan
    endif()
else ()
    # target_compile_options(dol PRIVATE "-fvisibility=hidden")
    # target_link_options(dol PRIVATE "LINKER:--version-script=${CMAKE_CURRENT_SOURCE_DIR}/dol.map")
endif ()

if(ANDROID)
    add_library(mp5native SHARED src/port/entry.cpp)
    set_target_properties(mp5native PROPERTIES OUTPUT_NAME main)
else ()
    add_executable(mp5native src/port/entry.cpp)
endif ()
target_compile_definitions(mp5native PRIVATE TARGET_PC NON_MATCHING VERSION=${VERSION})
target_include_directories(mp5native PRIVATE include ${CMAKE_BINARY_DIR})
target_link_libraries(mp5native PRIVATE dol aurora::main)
aurora_copy_runtime_dlls(mp5native dol)

list(APPEND BINARY_TARGETS mp5native dol)

set(REL_TARGETS "")
foreach (rel_file ${REL_FILES})
    get_filename_component(rel_dir ${rel_file} DIRECTORY)
    get_filename_component(rel_target ${rel_dir} NAME)
    list(APPEND REL_TARGETS ${rel_target})
endforeach ()
list(REMOVE_DUPLICATES REL_TARGETS)

# Add RELs
foreach (dir ${REL_TARGETS})
    if (IS_DIRECTORY ${CMAKE_SOURCE_DIR}/src/REL/${dir})
        set(REL_SOURCE_FILES ${REL_FILES})
        list(FILTER REL_SOURCE_FILES INCLUDE REGEX "^src/REL/${dir}/.*\\.c$")

        if (REL_SOURCE_FILES)
            # every REL module needs its own copy of these (see each file's comment):
            # empty _ctors/_dtors, and the progressive-scan render modes Aurora
            # declares but does not define
            add_library(${dir} SHARED ${REL_SOURCE_FILES} src/port/rel_ctors.c src/port/gx_render_modes.c)
            list(APPEND BINARY_TARGETS ${dir})
            if (USE_ASAN)
                set_source_files_properties(${REL_SOURCE_FILES} PROPERTIES COMPILE_OPTIONS "-fsanitize=address")
                target_compile_definitions(${dir} PRIVATE USE_ASAN)
            endif()
            mp5native_game_target(${dir})
            # the overlay table names modules in lower case ("dll/bootdll.rel")
            string(TOLOWER ${dir} rel_output_name)
            set_target_properties(${dir} PROPERTIES OUTPUT_NAME ${rel_output_name})
            if (MSVC)
                set_target_properties(${dir} PROPERTIES LINK_FLAGS "/EXPORT:ObjectSetup")
                # target_compile_options(${dir} PRIVATE "/WX")
                target_compile_options(${dir} PRIVATE "/wd4244") # implicit narrowing conversion
                target_compile_options(${dir} PRIVATE "/wd4101") # unreferenced local variable
                target_compile_options(${dir} PRIVATE "/wd4102") # unreferenced label
                target_compile_options(${dir} PRIVATE "/wd4305") # truncation
                target_compile_options(${dir} PRIVATE "/wd4018") # signed/unsigned mismatch
                target_compile_options(${dir} PRIVATE "/wd4996") # unsafe sprintf, etc
                target_compile_options(${dir} PRIVATE "/wd4146") # unary minus operator applied to unsigned type, result still unsigned
                target_compile_options(${dir} PRIVATE "/wd4552") # result of expression not used
                target_compile_options(${dir} PRIVATE "/wd4553") # result of expression not used: == vs =
                target_compile_options(${dir} PRIVATE "/wd4113") # function pointer parameter list mismatch
                target_compile_options(${dir} PRIVATE "/wd4068") # unknown pragma
                # target_compile_options(${dir} PRIVATE "/Zi")
                #                target_compile_options(${dir} PRIVATE "/Ob0")
                target_compile_options(${dir} PRIVATE "/Od")
                if(NOT USE_ASAN AND RUNTIME_SANITIZE)
                    target_compile_options(${dir} PRIVATE "/GS")
                    target_compile_options(${dir} PRIVATE "/RTC1")  # incompatible with asan
                endif()
            elseif (NOT WIN32)
                # target_compile_options(${dir} PRIVATE "-fvisibility=hidden")
                if (APPLE)
                    target_link_options(${dir} PRIVATE "LINKER:-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/rel_symbols.txt")
                else()
                    target_link_options(${dir} PRIVATE "LINKER:--version-script=${CMAKE_CURRENT_SOURCE_DIR}/rel.map")
                endif()
                set_target_properties(${dir} PROPERTIES PREFIX "")
            endif ()
            target_link_libraries(${dir} PRIVATE dol musyx aurora::mtx aurora::gx)
            add_dependencies(mp5native ${dir})
        endif ()
    endif ()
endforeach ()

if (ANDROID)
    # SDLActivity loads SDL_main via dlsym on Android. Since aurora::main is a static
    # archive, force an undefined reference so the linker keeps the SDL_main object.
    target_link_options(mp5native PRIVATE "-Wl,-u,SDL_main")
endif ()

if (NOT APPLE)
    add_custom_command(TARGET mp5native POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:mp5native>/res"
            COMMENT "Copying resources"
    )
endif ()
#
if (WIN32)
    set(MP5_NATIVE_WINDOWS_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/windows)
    set(MP5_NATIVE_WINDOWS_ICON_PNG ${CMAKE_CURRENT_SOURCE_DIR}/res/icon.png)
    set(MP5_NATIVE_WINDOWS_ICON_ICO ${CMAKE_CURRENT_BINARY_DIR}/mp5native.ico)
    set(MP5_NATIVE_WINDOWS_RC ${CMAKE_CURRENT_BINARY_DIR}/mp5native.rc)
    set(MP5_NATIVE_WINDOWS_MANIFEST ${CMAKE_CURRENT_BINARY_DIR}/mp5native.manifest)

    add_custom_command(
            OUTPUT ${MP5_NATIVE_WINDOWS_ICON_ICO}
            COMMAND powershell -ExecutionPolicy Bypass -File
            ${MP5_NATIVE_WINDOWS_RESOURCE_DIR}/Create-IcoFromPng.ps1
            -InputPng ${MP5_NATIVE_WINDOWS_ICON_PNG}
            -OutputIco ${MP5_NATIVE_WINDOWS_ICON_ICO}
            DEPENDS ${MP5_NATIVE_WINDOWS_ICON_PNG} ${MP5_NATIVE_WINDOWS_RESOURCE_DIR}/Create-IcoFromPng.ps1
            VERBATIM
            COMMENT "Generating Windows icon"
    )

    configure_file(${MP5_NATIVE_WINDOWS_RESOURCE_DIR}/mp5native.manifest.in ${MP5_NATIVE_WINDOWS_MANIFEST} @ONLY)
    configure_file(${MP5_NATIVE_WINDOWS_RESOURCE_DIR}/mp5native.rc.in ${MP5_NATIVE_WINDOWS_RC} @ONLY)

    target_sources(mp5native PRIVATE ${MP5_NATIVE_WINDOWS_ICON_ICO} ${MP5_NATIVE_WINDOWS_RC})
    #    set_target_properties(mp5native PROPERTIES WIN32_EXECUTABLE TRUE)

    if (MSVC)
        target_link_options(mp5native PRIVATE /MANIFEST:NO)
    endif ()
endif ()

if (APPLE)
    set(MP5_NATIVE_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/macos)
    set(MP5_NATIVE_INFO_PLIST ${MP5_NATIVE_RESOURCE_DIR}/Info.plist.in)
    file(GLOB_RECURSE MP5_NATIVE_RESOURCE_FILES
            "${MP5_NATIVE_RESOURCE_DIR}/Assets.car"
            "${MP5_NATIVE_RESOURCE_DIR}/Base.lproj/*"
            "${MP5_NATIVE_RESOURCE_DIR}/Mp5Native.icns")
    file(GLOB_RECURSE MP5_NATIVE_APP_RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/res/*")
    target_sources(mp5native PRIVATE ${MP5_NATIVE_RESOURCE_FILES})
    target_sources(mp5native PRIVATE ${MP5_NATIVE_APP_RESOURCE_FILES})
    foreach (FILE ${MP5_NATIVE_RESOURCE_FILES})
        file(RELATIVE_PATH NEW_FILE "${MP5_NATIVE_RESOURCE_DIR}" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}")
    endforeach ()
    foreach (FILE ${MP5_NATIVE_APP_RESOURCE_FILES})
        file(RELATIVE_PATH NEW_FILE "${CMAKE_CURRENT_SOURCE_DIR}" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}")
    endforeach ()
    set_target_properties(
            mp5native PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_NAME ${MP5_NATIVE_BUNDLE_NAME}
            MACOSX_BUNDLE_GUI_IDENTIFIER ${MP5_NATIVE_BUNDLE_IDENTIFIER}
            MACOSX_BUNDLE_BUNDLE_VERSION ${MP5_NATIVE_VERSION_STRING}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${MP5_NATIVE_SHORT_VERSION_STRING}
            MACOSX_BUNDLE_INFO_PLIST ${MP5_NATIVE_INFO_PLIST}
            OUTPUT_NAME Mp5Native
            XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
            XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "YES"
    )
endif ()



# Packaging logic
function(get_target_output_name target result_var)
    get_target_property(output_name ${target} OUTPUT_NAME)
    if (NOT output_name)
        set(${result_var} "${target}" PARENT_SCOPE)
    else ()
        set(${result_var} "${output_name}" PARENT_SCOPE)
    endif ()
endfunction()

function(get_target_prefix target result_var)
    set(${result_var} "" PARENT_SCOPE)
    if (APPLE)
        # Have to recreate some bundle logic here, since CMake can't tell us
        get_target_property(is_bundle ${target} MACOSX_BUNDLE)
        if (is_bundle)
            get_target_output_name(${target} output_name)
            if (CMAKE_SYSTEM_NAME STREQUAL Darwin)
                set(${result_var} "${output_name}.app/Contents/MacOS/" PARENT_SCOPE)
            else ()
                set(${result_var} "${output_name}.app/" PARENT_SCOPE)
            endif ()
        endif ()
    endif ()
endfunction()

set(EXTRA_TARGETS "")
if (TARGET crashpad_handler)
    list(APPEND EXTRA_TARGETS crashpad_handler)
endif ()

install(TARGETS ${BINARY_TARGETS} ${EXTRA_TARGETS} DESTINATION ${CMAKE_INSTALL_PREFIX})
aurora_install_runtime_dlls(mp5native ${CMAKE_INSTALL_PREFIX})

if (NOT APPLE)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_INSTALL_PREFIX})
endif ()

# TODO uncomment
if (CMAKE_BUILD_TYPE STREQUAL Debug) #OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
    set(DEBUG_FILES_LIST "")
    foreach (target IN LISTS BINARY_TARGETS EXTRA_TARGETS)
        get_target_output_name(${target} output_name)
        if (WIN32)
            install(FILES $<TARGET_PDB_FILE:${target}> DESTINATION ${CMAKE_INSTALL_PREFIX} OPTIONAL)
        elseif (APPLE)
            get_target_prefix(${target} target_prefix)
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND rm -fr \"$<TARGET_FILE_NAME:${target}>.dSYM\")")
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND dsymutil \"${target_prefix}$<TARGET_FILE_NAME:${target}>\")")
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND strip -S \"${target_prefix}$<TARGET_FILE_NAME:${target}>\")")
            if (NOT target_prefix STREQUAL "")
                install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND mv \"${target_prefix}$<TARGET_FILE_NAME:${target}>.dSYM\" .)")
            endif ()
        elseif (UNIX)
            get_target_prefix(${target} target_prefix)
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND objcopy --only-keep-debug \"${target_prefix}$<TARGET_FILE_NAME:${target}>\" \"${target_prefix}$<TARGET_FILE_NAME:${target}>.dbg\")")
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND objcopy --strip-debug --add-gnu-debuglink=$<TARGET_FILE_NAME:${target}>.dbg \"${target_prefix}$<TARGET_FILE_NAME:${target}>\")")
        endif ()
        list(APPEND DEBUG_FILES_LIST "${output_name}")
    endforeach ()
    # This is a terrible hack to only run this on CI
    # until I turn this into a script or something
    if(DEFINED ENV{GITHUB_ENV})
        if (WIN32)
            list(TRANSFORM DEBUG_FILES_LIST APPEND ".pdb")
            list(JOIN DEBUG_FILES_LIST " " DEBUG_FILES)
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND 7z a -t7z \"${CMAKE_INSTALL_PREFIX}/debug.7z\" ${DEBUG_FILES})")
        elseif (APPLE)
            list(TRANSFORM DEBUG_FILES_LIST APPEND ".dSYM")
            list(JOIN DEBUG_FILES_LIST " " DEBUG_FILES)
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND tar acfv \"${CMAKE_INSTALL_PREFIX}/debug.tar.xz\" ${DEBUG_FILES})")
        elseif (UNIX)
            list(TRANSFORM DEBUG_FILES_LIST APPEND ".dbg")
            list(JOIN DEBUG_FILES_LIST " " DEBUG_FILES)
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND tar -I \"xz -9 -T0\" -cvf \"${CMAKE_INSTALL_PREFIX}/debug.tar.xz\" ${DEBUG_FILES})")
        endif ()
    endif ()
endif ()
foreach (target IN LISTS BINARY_TARGETS)
    get_target_prefix(${target} target_prefix)
    foreach (extra_target IN LISTS EXTRA_TARGETS)
        get_target_prefix(${extra_target} extra_prefix)
        if (NOT "${target_prefix}" STREQUAL "${extra_prefix}")
            # Copy extra target to target prefix
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND cp \"${extra_prefix}$<TARGET_FILE_NAME:${extra_target}>\" \"${target_prefix}$<TARGET_FILE_NAME:${extra_target}>\")")
        endif ()
    endforeach ()
endforeach ()
