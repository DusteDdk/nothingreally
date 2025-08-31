#include "fillist.hpp"

std::string Fillist::cmdTypeStr(CmdType t)
{
    switch (t)
    {
    case CmdType::Insert:
        return "Insert";
    case CmdType::IndentMore:
        return "IndentMore";
    case CmdType::IndentLess:
        return "IndentLess";
    case CmdType::ScopeBegin:
        return "ScopeBegin";
    case CmdType::ScopeEnd:
        return "ScopeEnd";
    case CmdType::QuoteBegin:
        return "QuoteBegin";
    case CmdType::QuoteEnd:
        return "QuoteEnd";
    }
}

Fillist::Pos::Pos(Fillist &p) : parent(p), pos(parent.pos) {}

Fillist::Pos &Fillist::Pos::operator=(const Fillist &p)
{
    parent = p;
    pos = parent.pos;
    return *this;
}

Fillist &Fillist::Pos::operator()()
{
    parent.pos = pos;
    return parent;
}

Fillist::Pos &Fillist::Pos::operator--()
{
    pos = std::prev(pos);
    return *this;
}

Fillist::Pos Fillist::Pos::operator--(int)
{
    Pos tmp = *this;
    --(*this);
    return tmp;
}

Fillist::Pos &Fillist::Pos::operator++()
{
    pos = std::next(pos);
    return *this;
}

Fillist::Pos Fillist::Pos::operator++(int)
{
    Pos tmp = *this;
    ++(*this);
    return tmp;
}

Fillist::Fillist(std::string basePath, std::string baseName, std::string extension)
    : basePath(basePath), baseName(baseName), extension(extension), pos(cmds.begin()) {};

void Fillist::addCmd(CmdType t, std::string line)
{
    pos = std::next(pos);
    if (isVerbose)
    {
        int nPos = 0;
        for (auto iter = cmds.begin(); iter != cmds.end(); iter++)
        {
            if (iter == pos)
            {
                break;
            }
            nPos++;
        }
        std::cout << format("[{}] {} @ {}/{} ({})", baseName, cmdTypeStr(t), nPos, cmds.size(), line) << std::endl;
    }

    pos = cmds.insert(pos, {.type = t, .line = line});
}

void Fillist::setIndent(int n)
{
    indent += n;
    if(indent < 0) {
        indent=0;
        std::cout << "err, negative indent"  << std::endl;

    }
    pad = std::string(indent * 4, ' ');
}

Fillist::Pos Fillist::first()
{
    Pos tmp(*this);
    tmp.pos = cmds.begin();
    return tmp;
}
Fillist::Pos Fillist::last()
{
    Pos tmp(*this);
    tmp.pos = std::prev(cmds.end());
    return tmp;
}

Fillist &Fillist::before(Pos p)
{
    pos = std::prev(p.pos);
    return *this;
}

Fillist &Fillist::at(Pos p)
{
    pos = p.pos;
    return *this;
}

std::string Fillist::render()
{
    if(isVerbose) {
        std::cout << "Render begin" << std::endl;
    }
    std::string str = "";

    for (Cmd &c : cmds)
    {
        if(isVerbose) {
            std::cout << "Cmd:" << cmdTypeStr(c.type) << " : " << c.line << std::endl;
        }
        switch (c.type)
        {
        case CmdType::Insert:
            str += std::format("{}{}\n", pad, c.line);
            break;
        case CmdType::IndentMore:
            setIndent(1);
            break;
        case CmdType::IndentLess:
            setIndent(-1);
            break;
        case CmdType::ScopeBegin:
            str += std::format("{}{}\n", pad, c.line);
            setIndent(1);
            break;
        case CmdType::ScopeEnd:
            setIndent(-1);
            str += std::format("{}{}\n", pad, c.line);
            break;
        case CmdType::QuoteBegin:
            isQuoting = true;
            break;
        case CmdType::QuoteEnd:
            isQuoting = false;
            break;
        }
    }

    if(isVerbose) {
        std::cout << "Render end" << std::endl;
    }

    return str;
}

Fillist &Fillist::verbose(bool verbose)
{
    isVerbose = verbose;
    return *this;
}


Fillist &Fillist::scopeBegin(std::string beginStr)
{
    addCmd(CmdType::ScopeBegin, beginStr);
    return *this;
}

Fillist &Fillist::scopeEnd(std::string endStr)
{
    addCmd(CmdType::ScopeEnd, endStr);
    return *this;
}

Fillist &Fillist::indentMore() {
    addCmd(CmdType::IndentMore, "");
    return *this;
}
Fillist &Fillist::indentLess() {
    addCmd(CmdType::IndentLess, "");
    return *this;
}


Fillist &Fillist::line(std::string line)
{
    addCmd(CmdType::Insert, line);
    return *this;
}

Fillist &Fillist::quotedLine(std::string str, std::string quoteChar) {
    return line( std::format("{}{}{}",quoteChar, str, quoteChar) );
}

Fillist &Fillist::append(std::string str) {
    (*pos).line += str;
    return *this;
}

Fillist &Fillist::appendQuoted(std::string str, std::string quoteChar) {
    return append( std::format("{}{}{}",quoteChar, str, quoteChar) );
}


