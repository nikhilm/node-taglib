import Options
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

  tagconf = conf.find_program('taglib-config', var='TAGLIB_CONFIG', mandatory=True);

  taglibdir = popen("%s --libs"%tagconf).readline().strip()
  conf.env.append_value("LIBPATH_TAGLIB", taglibdir);
  conf.env.append_value("LIB_TAGLIB", "tag");

  taglibinc = popen("%s --cflags"%tagconf).readline().strip()[2:] # [2:] to remove the -I
  conf.env.append_value("CPPPATH_TAGLIB", taglibinc);

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "taglib"
  obj.source = "taglib.cc"
  obj.uselib = "TAGLIB"

def shutdown():
  # HACK to get binding.node out of build directory.
  # better way to do this?
  if Options.commands['clean']:
    if exists('taglib.node'): unlink('taglib.node')
  else:
    if exists('build/default/taglib.node') and not exists('taglib.node'):
      symlink('build/default/taglib.node', 'taglib.node')
