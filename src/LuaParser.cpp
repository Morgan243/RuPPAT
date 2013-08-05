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

string LuaParser::PopLuaTableStringValue(const char* table_name, const char* key)
//{{{
{
    //reset stack pointer and push the table name
    prepPop(table_name);

    //check it is actually a table
    if(!lua_istable(pL,1))
    {
        cout<<"\nError getting "<< table_name<<" from lua script!\n";
    }

    //indicate the key in the table
    lua_pushstring(pL, key);

    //tell lua to pop key off the stack, grab the value for the key
    //and place the value on the stack.
    //using -2 because the table is second from the top now on stack
    lua_gettable(pL, -2);

    //now the value corresponding to the key in the table is on top of the stack
    //check that the value is what is expected
    if(!lua_isstring(pL, -1))
    {
        cout<<"\nError getting value for "<< key <<" from table "<< table_name<<"!\n";
    }

    //get the value off the stack
    string value = lua_tostring(pL, -1);

    lua_pop(pL, 1);

    return value;
}
//}}}

string LuaParser::PopLuaTableStringValue(const char* table_name, const int key)
//{{{
{
    //reset stack pointer and push the table name
    prepPop(table_name);

    //check it is actually a table
    if(!lua_istable(pL,1))
    {
        cout<<"\nError getting "<< table_name<<" from lua script!\n";
    }

    //indicate the key in the table
    lua_pushnumber(pL,key);

    //tell lua to pop key off the stack, grab the value for the key
    //and place the value on the stack.
    //using -2 because the table is second from the top now on stack
    lua_gettable(pL, -2);

    //now the value corresponding to the key in the table is on top of the stack
    //check that the value is what is expected
    if(!lua_isstring(pL, -1))
    {
        cout<<"\nError getting value for "<< key <<" from table "<< table_name<<"!\n";
    }

    //get the value off the stack
    string value = lua_tostring(pL, -1);

    lua_pop(pL, 1);

    return value;
}
//}}}

int LuaParser::PopLuaTableIntegerValue(const char* table_name, const char* key)
//{{{
{
    //reset stack pointer and push the table name
    prepPop(table_name);

    //check it is actually a table
    if(!lua_istable(pL,1))
    {
        cout<<"\nError getting "<< table_name<<" from lua script!\n";
    }

    //indicate the key in the table
    lua_pushstring(pL, key);

    //tell lua to pop key off the stack, grab the value for the key
    //and place the value on the stack.
    //using -2 because the table is second from the top now on stack
    lua_gettable(pL, -2);

    //now the value corresponding to the key in the table is on top of the stack
    //check that the value is what is expected
    if(!lua_isnumber(pL, -1))
    {
        cout<<"\nError getting value for "<< key <<" from table "<< table_name<<"!\n";
    }

    //get the value off the stack
    int value = (int)lua_tonumber(pL, -1);

    lua_pop(pL, 1);

    return value;
}
//}}}

int LuaParser::PopLuaTableIntegerValue(const char* table_name, const int key)
//{{{
{
    //reset stack pointer and push the table name
    prepPop(table_name);

    //check it is actually a table
    if(!lua_istable(pL,1))
    {
        cout<<"\nError getting "<< table_name<<" from lua script!\n";
    }

    //indicate the key in the table
    lua_pushnumber(pL, key);

    //tell lua to pop key off the stack, grab the value for the key
    //and place the value on the stack.
    //using -2 because the table is second from the top now on stack
    lua_gettable(pL, -2);

    //now the value corresponding to the key in the table is on top of the stack
    //check that the value is what is expected
    if(!lua_isnumber(pL, -1))
    {
        cout<<"\nError getting value for "<< key <<" from table "<< table_name<<"!\n";
    }

    //get the value off the stack
    int value = (int)lua_tonumber(pL, -1);

    lua_pop(pL, 1);

    return value;
}
//}}}

string LuaParser::GetStringFromField(string field)
{
    string ret_str = "";

    lua_getfield(this->pL, -1, field.c_str());

    ret_str = lua_tostring(this->pL, -1);

    lua_pop(this->pL, 1);

    return ret_str;
}

string LuaParser::GetStringFromField(int index)
{
//{{{
    string ret_str = "";

    lua_rawgeti(this->pL, -1, index);

    ret_str = lua_tostring(this->pL, -1);

    lua_pop(this->pL, 1);

    return ret_str;
//}}}
}

int LuaParser::GetIntegerFromField(string field)
{
//{{{
    int ret_int = 0;

    lua_getfield(this->pL, -1, field.c_str());

    ret_int = lua_tonumber(this->pL, -1);

    lua_pop(this->pL, 1);

    return ret_int;
//}}}
}

int LuaParser::GetIntegerFromField(int index)
{
//{{{
    int ret_int = 0;

    lua_rawgeti(this->pL, -1, index);

    ret_int = lua_tonumber(this->pL, -1);

    lua_pop(this->pL, 1);

    return ret_int;
//}}}
}

lua_State* LuaParser::GetState()
{
    return pL;
}

