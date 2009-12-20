from os import popen

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
