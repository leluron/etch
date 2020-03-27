
// Generated from Etch.g4 by ANTLR 4.8

#pragma once

#include "parser/EtchBaseVisitor.h"
#include "AST.h"

using namespace std;

/**
 * This class provides an empty implementation of EtchVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ASTGen : public EtchBaseVisitor {
public:

    virtual antlrcpp::Any visitFile(EtchParser::FileContext *ctx) override {
        map<string, Global> gs;
        for (auto g : ctx->globals()) {
            gs.insert(visit(g).as<pair<string, Global>>());
        }
        return File{gs};
    }

    virtual antlrcpp::Any visitScope(EtchParser::ScopeContext *ctx) override {
        return ctx->sc->getText() == "public";
    }

    virtual antlrcpp::Any visitGlobals(EtchParser::GlobalsContext *ctx) override {
        return pair<string, Global>(ctx->ID()->getText(), {
            ctx->scope()?visit(ctx->scope()).as<bool>():false,
            visit(ctx->exp())
        });
    }

    virtual antlrcpp::Any visitStat(EtchParser::StatContext *ctx) override {
        vector<string> ids;
        for (auto i : ctx->ID()) ids.push_back(i->getText());
        return Stat{ids, visit(ctx->explist())};
    }

    virtual antlrcpp::Any visitRetstat(EtchParser::RetstatContext *ctx) override {
        return RetStat{visit(ctx->explist())};
    }

    virtual antlrcpp::Any visitMapdef(EtchParser::MapdefContext *ctx) override {
        vector<pair<expp, expp>> fields;
        for (int i=0;i<ctx->exp().size();i+=2) {
            fields.push_back(pair<expp, expp>(visit(ctx->exp(i)), visit(ctx->exp(i+1))));
        }
        return expp(new ExpMap(fields));
    }

    virtual antlrcpp::Any visitFunccallexp(EtchParser::FunccallexpContext *ctx) override {
        return expp (new ExpCall(visit(ctx->exp()), visit(ctx->explist())));
    }

    expp call(string name, expl args) {
        return expp(new ExpCall(expp(new ExpVar(name)), args));
    }

    expp unop(antlr4::Token *op, expp e1) {
        return call(op->getText(), {e1});
    }

    expp binop(antlr4::Token *op, expp e1, expp e2) {
        return call(op->getText(), {e1, e2});
    }

    expp binop(string name, expp e1, expp e2) {
        return call(name, {e1, e2});
    }

    virtual antlrcpp::Any visitAndexp(EtchParser::AndexpContext *ctx) override {
        return binop(ctx->op, visit(ctx->exp(0)), visit(ctx->exp(1)));
    }

    virtual antlrcpp::Any visitListexp(EtchParser::ListexpContext *ctx) override {
        return expp(new ExpList(visit(ctx->explist()).as<expl>()));
    }

    virtual antlrcpp::Any visitOrexp(EtchParser::OrexpContext *ctx) override {
        return binop(ctx->op, visit(ctx->exp(0)), visit(ctx->exp(1)));
    }

    virtual antlrcpp::Any visitAdditiveexp(EtchParser::AdditiveexpContext *ctx) override {
        return binop(ctx->op, visit(ctx->exp(0)), visit(ctx->exp(1)));
    }

    virtual antlrcpp::Any visitFloatexp(EtchParser::FloatexpContext *ctx) override {
        stringstream ss;
        ss << ctx->FLOAT();
        float v;
        ss >> v;
        return expp(new ExpFloat(v));
    }

    virtual antlrcpp::Any visitRelationexp(EtchParser::RelationexpContext *ctx) override {
        return binop(ctx->op, visit(ctx->exp(0)), visit(ctx->exp(1)));
    }

    virtual antlrcpp::Any visitUnaryexp(EtchParser::UnaryexpContext *ctx) override {
        return unop(ctx->op, visit(ctx->exp()));
    }

    virtual antlrcpp::Any visitMultiplicativeexp(EtchParser::MultiplicativeexpContext *ctx) override {
        return binop(ctx->op, visit(ctx->exp(0)), visit(ctx->exp(1)));
    }

    virtual antlrcpp::Any visitTrueexp(EtchParser::TrueexpContext *ctx) override {
        return expp(new ExpInt(1));
    }

    virtual antlrcpp::Any visitIdexp(EtchParser::IdexpContext *ctx) override {
        return expp(new ExpVar(ctx->ID()->getText()));
    }

    virtual antlrcpp::Any visitFunctiondef(EtchParser::FunctiondefContext *ctx) override {
        vector<string> argNames;
        for (auto a : ctx->ID()) argNames.push_back(a->getText());
        vector<Stat> stats;
        for (auto s : ctx->stat()) stats.push_back(visit(s));
        return expp(new ExpFunc(argNames, stats, visit(ctx->retstat())));
    }

    virtual antlrcpp::Any visitComparisonexp(EtchParser::ComparisonexpContext *ctx) override {
        return binop(ctx->op, visit(ctx->exp(0)), visit(ctx->exp(1)));
    }

    virtual antlrcpp::Any visitParenexp(EtchParser::ParenexpContext *ctx) override {
        return visit(ctx->exp());
    }

    virtual antlrcpp::Any visitFalseexp(EtchParser::FalseexpContext *ctx) override {
        return expp(new ExpInt(0));
    }

    virtual antlrcpp::Any visitStringexp(EtchParser::StringexpContext *ctx) override {
        return expp(new ExpString(ctx->STRING()->getText()));
    }

    virtual antlrcpp::Any visitTernaryexp(EtchParser::TernaryexpContext *ctx) override {
        return expp(new ExpTernary(visit(ctx->exp(0)), visit(ctx->exp(1)), visit(ctx->exp(2))));
    }

    virtual antlrcpp::Any visitIntexp(EtchParser::IntexpContext *ctx) override {
        return visit(ctx->inte());
    }

    virtual antlrcpp::Any visitIndexexp(EtchParser::IndexexpContext *ctx) override {
        return binop("index", visit(ctx->exp(0)), visit(ctx->exp(1)));
    }

    virtual antlrcpp::Any visitInte(EtchParser::InteContext *ctx) override {
        stringstream ss;
        ss << (ctx->INT()?ctx->INT()->getText():ctx->HEX()->getText());
        int v;
        ss >> v;
        return expp(new ExpInt(v));
    }

    virtual antlrcpp::Any visitExplist(EtchParser::ExplistContext *ctx) override {
        expl l;
        if (ctx) for (auto e : ctx->exp()) l.push_back(visit(e));
        return l;
    }
};