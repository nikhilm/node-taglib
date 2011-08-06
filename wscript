import Options
import os
from os import popen, unlink, symlink
from os.path import exists

srcdir = "."
blddir = "build"
VERSION = "0.0.1"

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
  obj.source = "src/tag.cc src/taglib.cc"
  obj.uselib = "TAGLIB"

def shutdown():
  # HACK to get binding.node out of build directory.
  # better way to do this?
  if Options.commands['clean']:
    if exists('lib/module.node'): unlink('lib/module.node')
  else:
    if exists('build/default/lib/module.node') and not exists('lib/module.node'):
      symlink('build/default/lib/module.node', 'lib/module.node')
# vim: ft=python sw=2
