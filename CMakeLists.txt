cmake_minimum_required(VERSION 3.12)

project(blake-stone LANGUAGES CXX)

list(APPEND SOURCES
        src/3d_act1.cpp
    src/3d_act2.cpp
    src/3d_agent.cpp
    src/3d_agent.hpp
    src/3d_debug.cpp
    src/3d_def.hpp
    src/3d_draw.cpp
    src/3d_draw2.cpp
    src/3d_game.cpp
    src/3d_inter.cpp
    src/3d_main.cpp
    src/3d_menu.cpp
    src/3d_menu.hpp
    src/3d_msgs.cpp
    src/3d_play.cpp
    src/3d_scale.cpp
    src/3d_state.cpp
    src/3d_state.hpp
    src/an_codes.hpp
        src/audio.hpp
        src/audiobs1.hpp
        src/audiofir.hpp
    src/audiovsi.hpp
    src/C0.ASM
    src/D3_ASM.ASM
    src/D3_DASM2.ASM
    src/D3_DR_A.ASM
    src/D3_DR_A2.ASM
    src/GFXE_BS1.H
    src/GFXE_FIR.H
    src/GFXE_VSI.H
    src/GFXV_BS1.H
    src/GFXV_FIR.H
    src/GFXV_SOD.EQU
        src/gfxv_vsi.hpp
    src/GFXV_WL1.EQU
    src/GFXV_WL6.EQU
    src/H_LDIV.ASM
    src/id_ca.cpp
    src/id_ca.hpp
    src/ID_HEAD.H
    src/id_heads.hpp
    src/id_in.cpp
    src/id_in.hpp
    src/id_mm.cpp
    src/ID_MM.H
    src/id_pm.cpp
    src/id_pm.hpp
    src/id_sd.cpp
    src/ID_SD.EQU
    src/id_sd.hpp
    src/ID_SD_A.ASM
    src/ID_SDD.C
    src/ID_US.H
    src/id_us_1.cpp
    src/ID_US_A.ASM
    src/id_vh.cpp
    src/id_vh.hpp
    src/ID_VH_A.ASM
        src/id_vl.cpp
    src/ID_VL.EQU
        src/id_vl.hpp
    src/ID_VL_A.ASM
    src/JABHACK.ASM
    src/JAM_IO.H
    src/jm_cio.cpp
    src/jm_cio.hpp
    src/jm_debug.cpp
    src/jm_debug.hpp
    src/jm_error.hpp
    src/JM_FREE.C
    src/jm_io.cpp
    src/jm_io.hpp
    src/jm_lzh.cpp
    src/jm_lzh.hpp
    src/jm_tp.cpp
    src/jm_tp.hpp
    src/jm_vl.cpp
    src/jm_vl.hpp
    src/MAPSBS1.H
    src/mapsvsi.hpp
    src/MARKHACK.ASM
    src/movie.cpp
    src/movie.hpp
    src/MUNGE.C
    src/RND.ASM
    src/SCALE.ASM
    src/WAITVBL.ASM
)

add_executable(blake-stone)
target_compile_features(blake-stone PRIVATE cxx_std_20)
target_compile_options(blake-stone PRIVATE 
    -Wall -Wextra -Wpedantic -Wunreachable-code -Wcast-align
    -Wno-implicit-fallthrough -Wno-ignored-qualifiers -Wredundant-decls
    -Woverloaded-virtual -Wreorder)
target_sources(blake-stone PRIVATE ${SOURCES})