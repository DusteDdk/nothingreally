#include "fillist.hpp"

std::string Fillist::cmdTypeStr(CmdType t)
{
    switch (t)
    {
    case CmdType::Line:
        return "Line";
    case CmdType::IndentIn:
        return "IndentIn";
    case CmdType::IndentOut:
        return "IndentOut";
    case CmdType::ScopeIn:
        return "ScopeIn";
    case CmdType::ScopeOut:
        return "ScopeOut";
    case CmdType::BeginQuote:
        return "BeginQuote";
    case CmdType::EndQuote:
        return "EndQuote";
    case CmdType::DebugOn:
        return "DebugOn";
    case CmdType::DebugOff:
        return "DebugOff";
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

// prefix --, returns new value
Fillist::Pos &Fillist::Pos::operator--()
{
    pos = std::prev(pos);
    return *this;
}

// postfix --, returns previous value
Fillist::Pos Fillist::Pos::operator--(int)
{
    Pos tmp = *this; // copy current state
    --(*this);       // reuse prefix
    return tmp;      // return old state
}

Fillist::Pos &Fillist::Pos::operator++()
{
    pos = std::next(pos);
    return *this;
}

Fillist::Pos Fillist::Pos::operator++(int)
{
    Pos tmp = *this; // copy current state
    ++(*this);       // reuse prefix
    return tmp;      // return old state
}

Fillist::Fillist(std::string basePath, std::string baseName, std::string extension)
    : basePath(basePath), baseName(baseName), extension(extension), pos(cmds.begin()) {};

void Fillist::addCmd(CmdType t, std::string s, int_fast32_t c)
{
    pos = cmds.insert(std::next(pos), {.type = t, .payload = s, .count = c});
}

void Fillist::setIndent(int_fast32_t n)
{
    state.indent += n;
    state.pad = std::string(state.indent * 4, ' ');
}


Fillist &Fillist::first() {
    // inserting advances the iterator, so it wraps around to the first element.
    pos=cmds.end();
    return *this;
}
Fillist &Fillist::last() {
    pos=std::prev( cmds.end() );
    return *this;
}


std::string Fillist::render()
{
    std::string str = "";

    int lineNumber = 0;
    int cmdNumber = 0;

    bool isDbg = false;
    for (Cmd &c : cmds)
    {

        if (c.type == CmdType::DebugOn)
        {
            isDbg = true;
        }

        if (isDbg)
        {
            str += std::format("<CMD:{} Type: {}: LineNum: {} Payload: {}", cmdNumber, cmdTypeStr(c.type), lineNumber, c.payload);
        }

        cmdNumber++;

        switch (c.type)
        {
        case CmdType::Line:
            lineNumber++;
            str += std::format("{}{}\n", state.pad, c.payload, "\n");
            break;
        case CmdType::IndentIn:
            setIndent(1);
            break;
        case CmdType::IndentOut:
            setIndent(-1);
            break;
        case CmdType::ScopeIn:
            setIndent(1);
            break;
        case CmdType::ScopeOut:
            setIndent(-1);
            break;
        case CmdType::BeginQuote:
            state.isQuoting = true;
            break;
        case CmdType::EndQuote:
            state.isQuoting = false;
            break;
        case CmdType::DebugOff:
            isDbg = false;
        case CmdType::DebugOn:
            str += "\n";
            break;
        }
    }

    return str;
}

Fillist &Fillist::debugBegin(std::string tag)
{
    addCmd(CmdType::DebugOn, tag, 0);
    return *this;
}
Fillist &Fillist::debugEnd()
{
    addCmd(CmdType::DebugOff, "", 0);
    return *this;
}

Fillist &Fillist::line(std::string line)
{
    addCmd(CmdType::Line, line, 1);
    return *this;
}