#include "interpreter.h"

using namespace std;

valp Interpreter::get(string name) {
    auto it = symbols.find(name);
    if (it == symbols.end()) throw runtime_error("Can't find variable");
    if (!pub[name]) throw runtime_error("Variable is private");
    return it->second;
}

valp Interpreter::call(string name, vector<valp> args) {
    auto f = *dynamic_pointer_cast<ValueFunction>(get(name));
    return call(f, args);
}

valp Interpreter::eval(ExpInt e) {
    return valp(new ValueInt(e.value));
}

valp Interpreter::eval(ExpList e) {
    vector<valp> l;
    for (auto e0 : e.l) {
        l.push_back(eval(e0));
    }
    return valp(new ValueList(l));
}

valp Interpreter::eval(ExpVar e) {
    auto it = symbols.find(e.name);
    if (it == symbols.end()) throw runtime_error("Can't find var");
    return it->second;
}

valp Interpreter::eval(ExpFunc e) {
    return valp(new ValueFunction(e));
}

valp Interpreter::eval(ExpCall e) {
    vector<valp> args;
    for (auto e0 : e.args) args.push_back(eval(e0));
    auto f = eval(e.func);
    if (auto f0 = dynamic_pointer_cast<ValueFunction>(f))
        return call(*f0, args);
    else if (auto f0 = dynamic_pointer_cast<ValueFunctionNative>(f))
        return (f0->func)(args);
    else throw runtime_error("Can't call a non-function");
}

valp Interpreter::call(ValueFunction f, vector<valp> args) {
    map<string, valp> backup = symbols;
    if (args.size() != f.f.argNames.size()) throw runtime_error("mismatched arguments");
    for (int i=0;i<args.size();i++) {
        symbols[f.f.argNames[i]] = args[i];
    }
    for (auto s : f.f.stats) {
        symbols[s.vars[0]] = eval(s.values[0]);
    }
    valp ret = eval(f.f.ret.ret[0]);
    symbols = backup;
    return ret;
}

valp Interpreter::eval(ExpTernary e) {
    auto c = *dynamic_pointer_cast<ValueInt>(eval(e.cond));
    if (c.value) return eval(e.then);
    else return eval(e.els);
}

#define D(a) if (auto e = dynamic_pointer_cast<a>(eb)) return eval(*e);

valp Interpreter::eval(expp eb) {
    D(ExpInt);
    D(ExpVar);
    D(ExpCall)
    D(ExpList);
    D(ExpFunc);
    D(ExpTernary);
    throw runtime_error("Not supported yet");
}

valp mul(valp l, valp r) {
    auto l0 = *dynamic_pointer_cast<ValueInt>(l);
    auto r0 = *dynamic_pointer_cast<ValueInt>(r);
    return valp(new ValueInt(l0.value*r0.value));
}

valp add(valp l, valp r) {
    auto l0 = *dynamic_pointer_cast<ValueInt>(l);
    auto r0 = *dynamic_pointer_cast<ValueInt>(r);
    return valp(new ValueInt(l0.value+r0.value));
}

valp mod(valp l, valp r) {
    auto l0 = *dynamic_pointer_cast<ValueInt>(l);
    auto r0 = *dynamic_pointer_cast<ValueInt>(r);
    return valp(new ValueInt(l0.value%r0.value));
}

valp div(valp l, valp r) {
    auto l0 = *dynamic_pointer_cast<ValueInt>(l);
    auto r0 = *dynamic_pointer_cast<ValueInt>(r);
    return valp(new ValueInt(l0.value/r0.value));
}

valp neq(valp l, valp r) {
    auto l0 = *dynamic_pointer_cast<ValueInt>(l);
    auto r0 = *dynamic_pointer_cast<ValueInt>(r);
    return valp(new ValueInt(l0.value!=r0.value));
}

valp eq(valp l, valp r) {
    auto l0 = *dynamic_pointer_cast<ValueInt>(l);
    auto r0 = *dynamic_pointer_cast<ValueInt>(r);
    return valp(new ValueInt(l0.value==r0.value));
}

valp last(valp l) {
    auto l0 = *dynamic_pointer_cast<ValueList>(l);
    return l0.elements[l0.elements.size()-1];
}

valp append(valp l, valp r) {
    auto l0 = *dynamic_pointer_cast<ValueList>(l);
    vector<valp> elements = l0.elements;
    elements.push_back(r);
    return valp(new ValueList(elements));
}

Interpreter::Interpreter(File f) {
    for (auto g : f.globals) {
        symbols[g.first] = eval(g.second.e);
        pub[g.first] = g.second.pub;
    }

    symbols["*"] = valp(new ValueFunctionNative(
        [](auto a) {return mul(a[0], a[1]);}));

    symbols["+"] = valp(new ValueFunctionNative(
        [](auto a) {return add(a[0], a[1]);}));

    symbols["%"] = valp(new ValueFunctionNative(
        [](auto a) {return mod(a[0], a[1]);}));

    symbols["/"] = valp(new ValueFunctionNative(
        [](auto a) {return div(a[0], a[1]);}));

    symbols["!="] = valp(new ValueFunctionNative(
        [](auto a) {return neq(a[0], a[1]);}));

    symbols["=="] = valp(new ValueFunctionNative(
        [](auto a) {return eq(a[0], a[1]);}));

    symbols["last"] = valp(new ValueFunctionNative(
        [](auto a) {return last(a[0]);}));

    symbols["append"] = valp(new ValueFunctionNative(
        [](auto a) {return append(a[0], a[1]);}));

}