import Options
import os
from os import popen, unlink, symlink
from os.path import exists

srcdir = "."
blddir = "build"

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")

  conf.check_cfg(path='taglib-config', package='', args='--cflags --libs',
  uselib_store='TAGLIB')

def post_build(ctx):
  base_path = ctx.srcnode.abspath(ctx.get_env())
  if not os.path.exists('taglib.node'):
      os.symlink(os.path.join(base_path, 'taglib.node'), 'taglib.node')

def build(bld):
  bld.add_post_fun(post_build)
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "taglib"
  obj.source = "src/tag.cc src/taglib.cc"
  obj.uselib = "TAGLIB"

def clean(ctx):
  if os.path.exists('taglib.node'):
    os.unlink('taglib.node')
