set(SOURCES
        src/Max31865.cpp
        src/numerics.cpp)

set(INCLUDES
        include)

set(REQUIRES
        driver)

idf_component_register(
        SRCS         ${SOURCES}
        INCLUDE_DIRS ${INCLUDES}
        REQUIRES     ${REQUIRES})
