#include <string>
#include <format>
#include <list>
#include <stack>
#include <iostream>

using WriterPos = std::list<std::string>::iterator;

class WPos;

class Writer
{
private:
    std::string basePath;
    std::string baseName;
    std::string extension;

    std::list<std::string> lines;

    WriterPos pos;

public:
    Writer(std::string basePath, std::string baseName, std::string extension) : basePath(basePath), baseName(baseName), extension(extension), pos(lines.begin()) {}

    Writer &line(std::string line)
    {
        pos = lines.insert(std::next(pos), line);
        return *this;
    }

    Writer &replace(WPos &pos, std::string str);

    Writer &append(std::string str)
    {
        *pos += str;
        return *this;
    }

    WriterPos getPos() const
    {
        return pos;
    }

    Writer &setPos(WriterPos p)
    {
        pos = p;
        return *this;
    }

    Writer &before(WPos &c);
    Writer &after(WPos &c);
    Writer &toStart()
    {
        pos = lines.begin();
        return *this;
    }
    Writer &toEnd()
    {
        pos = lines.end();
        return *this;
    }

    Writer &operator=(const WPos &w);

    void dump()
    {

        for (auto &l : lines)
        {
            std::cout << "Line: " << l << std::endl;
        }
    }
};

class WPos
{
public:
    WriterPos p;
    WPos &operator=(const Writer &w)
    {
        p = w.getPos();
        return *this;
    }
    WPos(Writer &writer) : p(writer.getPos()) {};
};

Writer &Writer::before(WPos &c)
{
    setPos(std::prev(c.p));
    return *this;
}

Writer &Writer::after(WPos &c)
{
    setPos(c.p);
    return *this;
}

Writer &Writer::operator=(const WPos &w)
{
    setPos(w.p);
    return *this;
}

Writer &Writer::replace(WPos &wpos, std::string str)
{
    auto prevPos = pos;
    pos = lines.insert(lines.erase(wpos.p), str);
    if (prevPos != wpos.p)
    {
        pos = prevPos;
    }
    return *this;
}