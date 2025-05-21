# meson.build in src

# Include Glad headers and source file
glad_src = files('../../glad/src/glad.c')
glad_include = include_directories('../../glad/include')
stb_include = include_directories('../../stb')

executable(
  'main', 'exercise.cpp', 
  sources: glad_src,
  dependencies: [glfw_dep, gl_dep],
  include_directories: [glad_include, stb_include]
)
