cmake_minimum_required(VERSION 3.16)
project(SolarBox2D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
#  SECTION 0: AUTOMATIC REPAIR (The Magic Fix)
#  This block runs before building. It steals working libraries from your
#  compiler and forcibly replaces the broken ones in SFML.
# ==============================================================================
if(WIN32)
    set(MSYS_LIB "C:/msys64/ucrt64/lib")
    set(SFML_EXTLIBS "${CMAKE_CURRENT_SOURCE_DIR}/sfml/extlibs/libs-mingw/x64")

    message(STATUS "--- AUTOMATION: Patching SFML Audio Libraries ---")
    
    # 1. Copy FLAC, OGG, VORBIS (Direct overwrite)
    # We look for the files in MSYS2 and copy them to SFML folder
    file(GLOB FIX_LIBS 
        "${MSYS_LIB}/libflac.a"
        "${MSYS_LIB}/libogg.a"
        "${MSYS_LIB}/libvorbis.a"
        "${MSYS_LIB}/libvorbisfile.a"
        "${MSYS_LIB}/libvorbisenc.a"
    )
    
    if(FIX_LIBS)
        file(COPY ${FIX_LIBS} DESTINATION "${SFML_EXTLIBS}")
        message(STATUS "Fixed: FLAC, OGG, and Vorbis libraries updated from System.")
    else()
        message(WARNING "Could not find MSYS2 libraries at ${MSYS_LIB}. Audio linking might fail.")
    endif()

    # 2. Fix OpenAL (Name mismatch fix)
    # MSYS2 calls it 'libopenal.a', SFML wants 'libopenal32.a'
    if(EXISTS "${MSYS_LIB}/libopenal.a")
        configure_file("${MSYS_LIB}/libopenal.a" "${SFML_EXTLIBS}/libopenal32.a" COPYONLY)
        message(STATUS "Fixed: OpenAL library updated and renamed.")
    endif()
endif()

# ==============================================================================
#  SECTION 1: BUILD CONFIGURATION
# ==============================================================================
# Enable Audio now that we have fixed the libraries!
set(SFML_BUILD_AUDIO    ON  CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS ON  CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW   ON  CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK  OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC_TAGFILE OFF CACHE BOOL "" FORCE)

# Build Dependencies Locally
add_subdirectory(sfml)
add_subdirectory(box2d)

# ==============================================================================
#  SECTION 2: GAME SETUP
# ==============================================================================
add_executable(solar 
    src/main.cpp
    src/Game.cpp
    src/PhysicsWorld.cpp
    src/InputHandler.cpp
    src/Sun.cpp
    src/Planet.cpp
    # Headers (Optional, for IDE visibility)
    src/PlanetType.hpp
    src/PlanetShaders.hpp
    src/Game.hpp
    src/PhysicsWorld.hpp
    src/InputHandler.hpp
    src/Sun.hpp
    src/Planet.hpp
    src/CelestialBody.hpp
    src/Config.hpp
    src/GameResources.hpp
    src/GuiLayer.hpp
    src/GravitySystem.hpp
    src/ContactListener.hpp
    src/StarBackground.hpp
    src/Utils.hpp
)

target_link_libraries(solar PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio 
    box2d
)

# ==============================================================================
#  SECTION 3: DEPLOYMENT AUTOMATION (Copying DLLs)
# ==============================================================================
if(WIN32)
    # Detect Architecture for OpenAL DLL
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_DIR "x64")
    else()
        set(ARCH_DIR "x86")
    endif()
    
    set(COMPILER_PATH "C:/msys64/ucrt64/bin")

    add_custom_command(TARGET solar POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        
        # 1. Game DLLs (SFML + Box2D)
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE:sfml-audio>
        $<TARGET_FILE:box2d>
        
        # 2. OpenAL DLL (From the source folder we just patched)
        "${CMAKE_CURRENT_SOURCE_DIR}/sfml/extlibs/bin/${ARCH_DIR}/openal32.dll"

        # 3. Compiler Runtime DLLs (Math, C++, Threads)
        "${COMPILER_PATH}/libgcc_s_seh-1.dll"
        "${COMPILER_PATH}/libstdc++-6.dll"
        "${COMPILER_PATH}/libwinpthread-1.dll"

        # 4. Music File (Checks root folder)
        # "${CMAKE_CURRENT_SOURCE_DIR}/music.ogg" 

        # Destination: Next to the EXE
        $<TARGET_FILE_DIR:solar>
        
        COMMENT "Auto-Deploy: Copying patched libs, DLLs, and runtimes..."
    )
endif()