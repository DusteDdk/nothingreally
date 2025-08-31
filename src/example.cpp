#include "fillist.hpp"

int main() {

    Fillist w("./", "something", "exe");

    // Very top.
    Fillist::Pos top = w;

    w.verbose(true);

    // Add line, capture position of it.
    Fillist::Pos first = w.line("Hello World");
    w.indentMore();
    w.line("Line 2")
    .line("Line 3");


    
    Fillist::Pos scope = w.scopeBegin();

    first().line("Line 0.5");

    scope().line("Line 0.7");
    w.scopeBegin();
    w.line("Line 0.8").scopeEnd();
    
    w.scopeEnd();

    first().line("After Hello World");
    (--first++)().line("Before Hello World");

    first().line("Line 0.3");


    w.at(w.last()).quotedLine("Last line.");
    w.before(w.first()).line("First line").appendQuoted(" for now");

    top().line("Absolute top.").append("..");

    auto endSoFar=w.last();
    endSoFar().indentLess().line("This is {} formatted line", "a").append(", it is {} fancy {}", "very", "indeed");

    std::cout <<std::endl << "Result:" << std::endl;
    std::cout << w.render() << std::endl;


    return 0;
}