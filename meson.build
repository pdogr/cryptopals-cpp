project('cryptopals-cpp',
	'cpp',
      version : '1.0',
    license : [ 'MIT'],
    meson_version : '>= 0.56.0',
    default_options : [ 'warning_level=3', 'buildtype=debugoptimized', 'cpp_std=c++17' ])

project_sources=[]
project_headers=[]
deps=[
	dependency('boost'),
  dependency('threads')
]

subdir('src')
subdir('include')

inc=[include_directories('include')]
lib=static_library('static',sources: project_sources)

set=get_option('set')
file=get_option('file')

source='src/'+set+'/'+file+'.cpp'

target=executable(
    file,
    source,
    dependencies: deps,
    include_directories:inc,
    link_with: lib)


