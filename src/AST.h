#pragma once

#include <variant>
#include <memory>
#include <map>
#include <string>
#include <vector>

class Exp;

using expp = std::shared_ptr<Exp>;
using expl = std::vector<expp>;

class Stat {
public:
    std::vector<std::string> vars;
    expl values;
};

class RetStat {
public:
    expl ret;
};

class Global {
public:
    bool pub = false;
    expp e;
};

class File {
public:
    std::map<std::string, Global> globals;
};

class Exp {
public:
    virtual ~Exp() {}
};

class ExpInt : public Exp {
public:
    ExpInt(int value) : value(value) {}
    int value;
};

class ExpFloat : public Exp {
public:
    ExpFloat(float value) : value(value) {}
    float value;
};

class ExpString : public Exp {
public:
    ExpString(std::string value) : value(value) {}
    std::string value;
};

class ExpVar : public Exp {
public:
    ExpVar(std::string name) : name(name) {}
    std::string name;
};

class ExpList : public Exp {
public:
    ExpList(expl l) : l(l) {}
    expl l;
};

class ExpFunc : public Exp {
public:
    ExpFunc(
        std::vector<std::string> argNames,
        std::vector<Stat> stats, RetStat ret) :
            argNames(argNames), stats(stats), ret(ret) {}
    std::vector<std::string> argNames;
    std::vector<Stat> stats;
    RetStat ret;
};

class ExpMap : public Exp {
public:
    ExpMap(std::vector<std::pair<expp, expp>> entries) : entries(entries) {}
    std::vector<std::pair<expp, expp>> entries;
};

class ExpCall : public Exp {
public:
    ExpCall(expp func, expl args) : func(func), args(args) {}
    expp func;
    expl args;
};

class ExpTernary : public Exp {
public:
    ExpTernary(expp then, expp cond, expp els) : then(then), cond(cond), els(els) {}
    expp then, cond, els;
};

