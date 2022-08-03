set(includedirs
    sol2/include
    lua
    luacpp/Source
)

set(requires
    Watchy
)

set(privrequires
)

FILE(GLOB LuaSources lua/*.c)

set(srcs 
    main.cpp
    EventLoop.cpp
    Display.cpp
    Database.cpp
    Watchy.cpp

    lua/onelua.c
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ${includedirs} REQUIRES ${requires} PRIV_REQUIRES ${privrequires})

target_compile_options(${COMPONENT_TARGET} PUBLIC
)