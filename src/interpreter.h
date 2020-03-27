#pragma once

#include "AST.h"
#include <functional>

class Value {
public:
    virtual ~Value() {}
};

using valp = std::shared_ptr<Value>;

class ValueInt : public Value {
public:
    ValueInt(int v) : value(v) {}
    int value;
};

class ValueFloat : public Value {
public:
    ValueFloat(float v) : value(v) {}
    float value;
};

class ValueString : public Value {
public:
    ValueString(std::string v) : value(v) {}
    std::string value;
};

class ValueList : public Value {
public:
    ValueList(std::vector<valp> e) : elements(e) {}
    std::vector<valp> elements;
};

class ValueFunction : public Value {
public:
    ExpFunc f;
    ValueFunction(ExpFunc f) : f(f) {}
};

class ValueFunctionNative : public Value {
public:
    ValueFunctionNative(std::function<valp(std::vector<valp>)> func) : func(func) {}
    std::function<valp(std::vector<valp>)> func;
};

class Interpreter {

    std::map<std::string, valp> symbols;
    std::map<std::string, bool> pub;

    valp eval(expp e);
    valp eval(ExpInt e);
    valp eval(ExpVar e);
    valp eval(ExpCall e);
    valp eval(ExpList e);
    valp eval(ExpFunc e);
    valp eval(ExpTernary e);

    valp call(ValueFunction f, std::vector<valp> args);

public:
    Interpreter(File f);

    valp get(std::string name);
    valp call(std::string name, std::vector<valp> args);

};
