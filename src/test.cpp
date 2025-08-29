#include "filist.hpp"

int main() {

    Writer w("./", "something", "exe");

    

    WPos first = w.line("Line 1.0");
    w.line("Line 2.0").line("Line 3.0");
    WPos four = w.line("Line 4.0");

//    w = first;
    w.before(first).line("Line 0.9");
    WPos oneHalf = w.after(first).line("Line 1.5");
    w.after(first).line("Line 1.4");
    w.after(four);
    w.replace(oneHalf, "Fisk");
    w.line("fisk");

    w=oneHalf;
    w.append("a");
    w.append("b");
    w.append("c");
    w.append("d");

    w.dump();

    return 0;
}