import sys

header_template = """
#pragma once

#include "PARENTNAME.h"

class CLASSNAME : public PARENTNAME {
  COMPONENT_CHILD_CLASS(CLASSNAME);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init();
};
"""

class_template = """
#include "CLASSNAME.h"

void CLASSNAME::AddLuaFunctions(lua_State* L)
{
  PARENTNAME::AddLuaFunctions(L);
}

int CLASSNAME::Lua_Create(lua_State* L) {

  auto LOWER_CLASSNAME = create_new_ews_object<CLASSNAME>(L);

  LOWER_CLASSNAME->init();

  return 1;
}

void CLASSNAME::init() {
}
"""

def create_class(classname, parent):
  global header_template
  global class_template

  header = header_template
  header = header.replace("LOWER_CLASSNAME", classname.lower()).replace("CLASSNAME", classname).replace("PARENTNAME", parent)

  clazz = class_template
  clazz = clazz.replace("LOWER_CLASSNAME", classname.lower()).replace("CLASSNAME", classname).replace("PARENTNAME", parent)

  header_out = open(f"{classname}.h", 'w')
  class_out = open(f"{classname}.cpp", 'w')
  header_out.write(header)
  class_out.write(clazz)
  header_out.close()
  class_out.close()

  print(f"Created class for {classname}")

if not len(sys.argv) < 3:
  classname_ = sys.argv[1]
  parentname_ = sys.argv[2]
  create_class(classname_, parentname_)
