add_library(qyamlcpp STATIC
   collection.h
   comment.h
   emitter.h
   node.h node.cpp
   parse.h parse.cpp
   )
target_link_libraries(qyamlcpp yamlcpp Qt5::Core Qt5::Gui)
target_include_directories(qyamlcpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

