#pragma once

#include <string>
#include <format>
#include <list>
#include <stack>
#include <iostream>
#include <vector>

#include <cstdint>

class Fillist
{

private:
    std::string basePath;
    std::string baseName;
    std::string extension;

    enum class CmdType
    {
        Line,
        IndentIn,
        IndentOut,
        ScopeIn,
        ScopeOut,
        BeginQuote,
        EndQuote,
    };
    std::string cmdTypeStr(CmdType t);

    struct Cmd
    {
        CmdType type;
        std::string payload;
        int_fast32_t count;
    };

    std::list<Fillist::Cmd> cmds;
    std::list<Fillist::Cmd>::iterator pos;

    void setIndent(int_fast32_t n);

    struct
    {
        bool isDebug=false;
        std::string pad = "";
        int indent = 0;
        bool isQuoting = false;
        std::vector<std::string> debug{};
    } state;

    void addCmd(CmdType t, std::string s, int_fast32_t c);

public:
    Fillist(std::string basePath, std::string baseName, std::string extension);

    struct Pos
    {
    private:
        Fillist &parent;
        std::list<Fillist::Cmd>::iterator pos;

    public:
        Pos(Fillist &p);
        Fillist &operator()();
        Pos &operator=(const Fillist &p);
        Pos &operator--();
        Pos operator--(int);
        Pos &operator++();
        Pos operator++(int);

        friend class Fillist;
    };
    friend struct Pos;

    Pos first();
    Pos last();

    Fillist &before(Pos p);
    Fillist &at(Pos p);


    std::string render();
    Fillist &debugBegin();
    Fillist &debugEnd();
    Fillist &line(std::string line);
};
