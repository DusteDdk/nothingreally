#include "fillist.hpp"

int main() {

    Fillist w("./", "something", "exe");

    // Very top.
    Fillist::Pos top = w;

    w.debugBegin();

    // Add line, capture position of it.
    Fillist::Pos first = w.line("Hello World");
    w.line("Line 2")
    .line("Line 3");

    first().line("Line 0.5");

    w.line("Line 0.7");

    first().line("After Hello World");
    (--first++)().line("Before Hello World");

    first().line("Line 0.3");


    w.at(w.last()).line("Last line.");
    w.before(w.first()).line("First line (for now)");

    top().line("Absolute top.");

    std::cout <<std::endl << "Result:" << std::endl;
    std::cout << w.render() << std::endl;


    return 0;
}