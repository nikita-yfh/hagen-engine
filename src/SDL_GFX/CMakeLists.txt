set(HEADERS SDL2_gfxPrimitives.h  SDL2_imageFilter.h  SDL2_rotozoom.h)
set(SOURCES SDL2_gfxPrimitives.c  SDL2_imageFilter.c  SDL2_rotozoom.c)
add_library(gfx SHARED ${SOURCES} ${HEADERS})