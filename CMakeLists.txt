cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(ZGloomVita)
include("${VITASDK}/share/vita.cmake" REQUIRED)

# ----------------------------------------------------------------------------------
# Toolchain fine-tuning for PS Vita (Cortex-A9/NEON)
# Toggle with -DVITA_PERF_TWEAKS=ON/OFF (default: ON)
# ----------------------------------------------------------------------------------
option(VITA_PERF_TWEAKS "Enable PS Vita performance-oriented compiler & linker flags" ON)
if(VITA_PERF_TWEAKS)
  # CPU/Vector
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -mcpu=cortex-a9 -mtune=cortex-a9 -mfpu=neon -mfloat-abi=hard -fstrict-aliasing")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a9 -mtune=cortex-a9 -mfpu=neon -mfloat-abi=hard -fstrict-aliasing")

  # Optimize+LTO (already used elsewhere but keep here to ensure consistency)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O3 -ffast-math -flto")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -flto")

  # Function/data sectioning for dead code removal at link time
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

  # Define NDEBUG for release-ish builds (asserts off)
  add_definitions(-DNDEBUG)

  # Linker garbage collection & as-needed to prune unused objects/stubs
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -Wl,--as-needed")
endif()


set(VITA_APP_NAME "ZGloomVita")
set(VITA_TITLEID  "JETSGLOOM")
set(VITA_VERSION  "02.60")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive")

include_directories(
#  common
	include_directories(${VITASDK}/arm-vita-eabi/include/SDL2)
)

add_executable(${PROJECT_NAME}
#	src/logger.h
#	src/logger.cpp
	src/input.h
	src/input.cpp
	src/script.h
	src/script.cpp
	src/monsterlogic.h
	src/monsterlogic.cpp
	src/soundhandler.h
	src/soundhandler.cpp
	src/config.h
	src/config.cpp
	src/titlescreen.h
	src/titlescreen.cpp
	src/hud.h
	src/hud.cpp
	src/iffhandler.h
	src/iffhandler.cpp
	src/font.h
	src/font.cpp
	src/menuscreen.cpp
	src/menuscreen.h
	src/gamelogic.cpp
	src/gamelogic.h
	src/binresources.h
	src/binresource.cpp
	src/gloommap.h
	src/gloommap.cpp
	src/objectgraphics.h
	src/objectgraphics.cpp
	src/gloommaths.h
	src/gloommaths.cpp
	src/crmfile.h
	src/quick.h
	src/quick.cpp
	src/decrunchmania.h
	src/decrunchmania.cpp
	src/renderer.h
	src/renderer.cpp
	src/zgloom.cpp

)

target_link_libraries(${PROJECT_NAME}
  SDL2_mixer
  modplug
  opusfile
  opus
  ogg
  SDL2
  vita2d
  mikmod
  mpg123
  FLAC
  vorbisfile
  vorbis
  ogg
  xmp
  SceDisplay_stub
  SceCtrl_stub
  SceAudio_stub
  SceAudioIn_stub
  SceSysmodule_stub
  SceGxm_stub
  SceCommonDialog_stub
  SceAppMgr_stub
  SceTouch_stub
  SceHid_stub
  SceMotion_stub
  SceAppUtil_stub
  SceLibKernel_stub
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/manual/001.png sce_sys/manual/001.png
  FILE sce_sys/manual/002.png sce_sys/manual/002.png
  FILE sce_sys/manual/003.png sce_sys/manual/003.png
  FILE sce_sys/manual/004.png sce_sys/manual/004.png
  FILE sce_sys/manual/005.png sce_sys/manual/005.png
  FILE sce_sys/manual/006.png sce_sys/manual/006.png
  FILE sce_sys/manual/007.png sce_sys/manual/007.png
  FILE sce_sys/manual/008.png sce_sys/manual/008.png
  FILE sce_sys/manual/009.png sce_sys/manual/009.png
  FILE sce_sys/manual/010.png sce_sys/manual/010.png
  FILE sce_sys/manual/011.png sce_sys/manual/011.png
  FILE sce_sys/manual/012.png sce_sys/manual/012.png
  FILE sce_sys/param.sfo sce_sys/param.sfo
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/pic0.png sce_sys/pic0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/gloomdeluxe.png sce_sys/livearea/contents/gloomdeluxe.png
  FILE sce_sys/livearea/contents/gloom3.png sce_sys/livearea/contents/gloom3.png
  FILE sce_sys/livearea/contents/zombiemassacre.png sce_sys/livearea/contents/zombiemassacre.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)

# Append these lines to your CMakeLists.txt (or replace flags/targets accordingly)
# --- Performance flags (VitaSDK, GCC) ---
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ffast-math -flto -fno-exceptions -fomit-frame-pointer -ftree-vectorize -funsafe-math-optimizations")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -flto -fno-exceptions -fomit-frame-pointer -ftree-vectorize -funsafe-math-optimizations -std=gnu++11")

# Enable NEON if available on toolchain
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon -mfloat-abi=hard")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon -mfloat-abi=hard")

# -----------------------------------------------------------
# Vita Performance Pack Sources (LensFlare / RendererHooks)
# -----------------------------------------------------------
set(ZGLOOM_VITA_ADDON_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/ConfigOverlays.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/MenuAdapters.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/weaponfix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vita/VignetteOverlay.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/vita/FilmGrainOverlay.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/vita/ScanlineOverlay.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/vita/FrameLimiter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vita/Profiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vita/LensFlare.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vita/RendererHooks.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/vita/VignetteOverlay.cpp
)

# !! Namen exakt wie bei add_executable oben !!
target_sources(${PROJECT_NAME} PRIVATE ${ZGLOOM_VITA_ADDON_SOURCES})

# Added by patch: streaming-texture bridge
set(EXTRA_VITA_SRC ${EXTRA_VITA_SRC} src/vita/renderer_sdlbridge.cpp)
