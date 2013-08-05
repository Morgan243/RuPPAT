#ifndef LUA_PARSER_H
#define LUA_PARSER_H
#include <string>
#include <iostream>

//Lua Scripting support
extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#pragma comment(lib, "lua.lib")
#pragma comment(lib, "lualib.lib")

class LuaParser
{
    private:
        lua_State *pL;

        lua_State* initLua();

        void prepPop(const char* name);

    public:
        LuaParser();
        LuaParser(const std::string &script_name);
        ~LuaParser();

        void openScript(const std::string &script_name);

        template <class T>
            T PopLuaNumber(const char* name);

        std::string PopLuaString(const char* name);

        std::string PopLuaTableStringValue(const char* table_name, const char* key);
        std::string PopLuaTableStringValue(const char* table_name, const int key);

        int PopLuaTableIntegerValue(const char* table_name, const char* key);
        int PopLuaTableIntegerValue(const char* table_name, const int key);

        std::string GetStringFromField(std::string field);
        std::string GetStringFromField(int index);

        int GetIntegerFromField(std::string field);
        int GetIntegerFromField(int index);
        
        lua_State* GetState();
};

template <class T>
T LuaParser::PopLuaNumber(const char* name)
//{{{{
{
    //set the stack top and get the global by name onto the stack
    prepPop(name);

    if(!lua_isnumber(pL,1))
    {
        std::cout<<"\nInvalid type in PopLuaNumber!\n";
    }

    //grab value and cast to the correct type
    T value = (T)lua_tonumber(pL,1);

    //remove from lua stack
    lua_pop(pL, 1);

    return value;
}
//}}}
#endif
