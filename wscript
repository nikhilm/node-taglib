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
  if not os.path.exists('lib/taglib_binding.node'):
      os.symlink( '../build/Release/taglib_binding.node', 'lib/taglib_binding.node')

def build(bld):
  bld.add_post_fun(post_build)
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "taglib_binding"
  obj.source = "src/tag.cc src/taglib.cc"
  obj.uselib = "TAGLIB"

def shutdown():
  # HACK to get binding.node out of build directory.
  # better way to do this?
  if Options.commands['clean']:
    if exists('lib/taglib_binding.node'): unlink('lib/taglib_binding.node')
  else:
    if exists('build/Release/lib/taglib_binding.node') and not exists('lib/taglib_binding.node'):
      symlink('build/Release/lib/taglib_binding.node', 'lib/taglib_binding.node')
# vim: ft=python sw=2
