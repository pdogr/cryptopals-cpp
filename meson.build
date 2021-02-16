project('cryptopals-cpp',
	'cpp',
      version : '1.0',
    license : [ 'MIT'],
    meson_version : '>= 0.56.0',
    default_options : [ 'warning_level=3', 'buildtype=debugoptimized', 'cpp_std=c++17' ])
inc=include_directories('include')
sources=[]
build_args=[]

deps=[
	dependency('boost'),
]

sources+=run_command('all.sh','src').stdout().strip().split('\n')

set=get_option('set')
file=get_option('file')

sources+=[
  'src/'+set+'/'+file
]

target=executable(meson.project_name(),
      sources,
    dependencies: deps,
    include_directories: inc)



