# meson.build in src

# Include Glad headers and source file
glad_src = files('../../glad/src/glad.c')
glad_include = include_directories('../../glad/include')

executable(
  'main', 'example1.cpp', 
  sources: glad_src,
  dependencies: [glfw_dep, gl_dep],
  include_directories: glad_include
)
