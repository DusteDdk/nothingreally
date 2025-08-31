#include "fillist.hpp"

int main() {

    Fillist w("./", "something", "exe");

    Fillist::Pos first = w.line("Hello World");
    w.line("Line 2")
    .line("Line 3");

    first().line("Line 0.5");

    w.line("Line 0.7");

    (--first)().line("Before HelloWorld");

    w.last().line("VeryEnd");

    w.first().line("VeryBegin");
    std::cout << w.render() << std::endl;


    return 0;
}