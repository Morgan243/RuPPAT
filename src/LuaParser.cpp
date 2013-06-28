#include "LuaParser.h"

using namespace std;

LuaParser::LuaParser()
{
    pL = initLua();
}

LuaParser::LuaParser(const std::string &script_name)
//{{{
{
    pL = initLua();

    openScript(script_name);
}
//}}}

LuaParser::~LuaParser()
{
    lua_close(pL);
}

lua_State* LuaParser::initLua()
//{{{
{
    //create Lua state
    lua_State *L = luaL_newstate();

    //open all standard Lua libraries
    luaL_openlibs(L);

    return L;
}
//}}}

void LuaParser::openScript(const std::string &script_name)
{
    luaL_dofile(pL, script_name.c_str());
}

void LuaParser::prepPop(const char* name)
//{{{
{
    lua_settop(pL, 0);

    lua_getglobal(pL, name);
}
//}}}

string LuaParser::PopLuaString(const char* name)
//{{{
{
    //set the stack top and get the global by name onto the stack
    prepPop(name);

    if(!lua_isstring(pL,1))
    {
        cout<<"\nInvalid type in PopLuaString!\n";
    }

   string value = lua_tostring(pL, 1);

   lua_pop(pL, 1);

   return value;
}
//}}}
