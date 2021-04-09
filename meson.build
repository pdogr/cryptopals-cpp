project('cryptopals-cpp',
	'cpp',
      version : '1.0',
    license : [ 'MIT'],
    meson_version : '>= 0.56.0',
    default_options : [ 'warning_level=3', 'buildtype=debugoptimized', 'cpp_std=c++17' ])

project_sources=[]
project_headers=[]
inc=[include_directories('include')]
deps=[
	dependency('boost'),
]

subdir('src')
subdir('include')

set=get_option('set')
file=get_option('file')

project_sources+=[
  'src/'+set+'/'+file
]

target=executable(meson.project_name(),
      project_sources,
    dependencies: deps,include_directories:inc )



