# Root meson.build
project('glfw_test', ['cpp', 'c'])

glfw_dep = dependency('glfw3', required: true)
gl_dep   = dependency('gl',    required: true)

foreach img : ['wall.jpg', 'cobble.png']
  configure_file(
    input : 'src/' + img,
    output : img,
    copy  : true
  )
endforeach

subdir('src')
