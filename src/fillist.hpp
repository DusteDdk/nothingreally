#pragma once

#include <string>
#include <format>
#include <list>
#include <stack>
#include <iostream>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>

class Fillist
{

private:
    std::string basePath;
    std::string baseName;
    std::string extension;

    enum class CmdType
    {
        Insert,
        IndentMore,
        IndentLess,
        ScopeBegin,
        ScopeEnd,
        QuoteBegin,
        QuoteEnd,
    };
    std::string cmdTypeStr(CmdType t);

    struct Scope
    {
        std::string tag;
        std::string begin;
        std::string end;
    };

    struct Cmd
    {
        CmdType type;
        std::string line;
    };

    std::list<Fillist::Cmd> cmds;
    std::list<Fillist::Cmd>::iterator pos;

    void setIndent(int n);

    std::string pad = "";
    int indent = 0;
    bool isQuoting = false;
    bool isVerbose = false;


    void addCmd(CmdType t, std::string line);

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
    Fillist &verbose(bool verbose);

    Fillist &scopeBegin(std::string beginStr="{");
    Fillist &scopeEnd(std::string endStr="}");

    Fillist &indentMore();
    Fillist &indentLess();

    Fillist &line(std::string line);

    Fillist &quotedLine(std::string line, std::string quoteChar="\"");

    Fillist &append(std::string str);

    Fillist &appendQuoted(std::string str, std::string quoteChar="\"");

    template <class... Args>
    Fillist& line(std::string_view fmt, Args&&... args) {
        return line( std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)) );
    }

    template <class... Args>
    Fillist& append(std::string_view fmt, Args&&... args) {
        return append( std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)) );
    }
};
