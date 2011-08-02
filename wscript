import os

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")

  conf.check_cfg(package='taglib', args='--cflags --libs',
  uselib_store='TAGLIB')

def post_build(ctx):
  if not os.path.exists('lib/module.node'):
      os.symlink( '../build/default/module.node', 'lib/module.node')

def build(bld):
  bld.add_post_fun(post_build)
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "module"
  obj.source = "src/taglib.cc"
  obj.uselib = "TAGLIB"

# vim: ft=python sw=2
