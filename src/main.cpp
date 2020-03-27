#include <iostream>

#include <antlr4-runtime/antlr4-runtime.h>
#include "parser/EtchLexer.h"
#include "parser/EtchParser.h"

#include "ASTGen.h"
#include "interpreter.h"

using namespace std;
using namespace antlr4;

int main() {
    ifstream stream;
    stream.open("test.etch");

    ANTLRInputStream input(stream);
    EtchLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    EtchParser parser(&tokens);

    EtchParser::FileContext* tree = parser.file();

    Interpreter interpreter(ASTGen().visitFile(tree).as<File>());

    valp a = interpreter.call("main", {valp(new ValueInt(28294))});

    auto li = *dynamic_pointer_cast<ValueList>(a);

    for (auto e : li.elements) {
        auto e0 = *dynamic_pointer_cast<ValueInt>(e);
        cout << e0.value << endl;
    }

}