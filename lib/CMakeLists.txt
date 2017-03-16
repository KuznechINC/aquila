# Ooura FFT
enable_language(C)
set(Ooura_fft_SOURCES ooura/fft4g.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
add_library(Ooura_fft STATIC ${Ooura_fft_SOURCES})

install(TARGETS Ooura_fft
        EXPORT  AquilaTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )

# UnitTest++
if(Aquila_BUILD_TESTS)
    add_subdirectory(unittestpp)
endif()
