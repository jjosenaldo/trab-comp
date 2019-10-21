#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <deque>
#include <unordered_map>
#include <string>
#include "operator.hpp"
#include "symtable.hpp"

using std::deque;
using std::unordered_map;
using std::string;

union AtomExpValue{
    bool boolval;   
    int intval;
    char* strval;
};

bool predefinedId(char* id);

#include "type.hpp"

class Expression{
    protected:
        Type* type;

    public:
        virtual ~Expression() {}
        Type* getType();
        virtual void print() = 0;
        virtual bool process(Symtable* environment, ClassSymtablePool* pool) = 0;
        virtual string toString() {return "";} // TODO
};

// Example: 1+1
class BinExpression : public Expression{
    private:
        Expression* first;
        Expression* second;
        BinOperator op;
    
    public:
        BinExpression(Expression* first, Expression* second, BinOperator op);
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

// Example: !a
class UnExpression : public Expression{
    private:
        Expression* first;
        UnOperator op;

    public:
        UnExpression(Expression* first, UnOperator op);
        bool process(Symtable* environment, ClassSymtablePool* pool);

        void print();
};

// Example: false
//,         52
//          "oi"
//          null
// The type is set when the parse tree is built
class AtomExpression : public Expression{
    private:
        AtomExpValue val;

    public:
        AtomExpression(Type* type);
        AtomExpression(AtomExpValue val, Type* type);
        AtomExpValue getVal();
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

class ObjExpression : public Expression{};

// Example: new int
// The type is set when the parse tree is built
class ArrayDeclExpression : public ObjExpression{
    private:
        Type* type;
    public:
        ArrayDeclExpression(Type* type);
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

// Example: new Cat()
class NewObjExpression : public ObjExpression{
    private:
        string id;
    
    public:
        NewObjExpression(string id);
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

// Its type is not known when the tree is being built
class IdExpression : public ObjExpression{
    private:
        string id;
    
    public:
        IdExpression(string id);
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

// Its type is not known when the tree is being built
class FieldAccessExpression : public ObjExpression{
    private:
        string id;
    
    public:
        FieldAccessExpression(string id);
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

// Its type is not known when the tree is being built
class ThisExpression : public ObjExpression{
    public:
        void print();
        bool process(Symtable* environment, ClassSymtablePool* pool);
};

// Its type is not known when the tree is being built
class MethodCallExpression : public ObjExpression{
    private:
        Expression* left;
        string method;
        deque<Expression*>* arguments;

    public:
        MethodCallExpression(Expression* left, string method, deque<Expression*>* args);
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

// Example: (a)
class ParenExpression : public ObjExpression{
    private:
        Expression* first;

    public:
        ParenExpression(Expression* first);
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

class LitArrayExpression : public ObjExpression{
    private:
        deque<Expression*>* expressions;
    public:
        LitArrayExpression(deque<Expression*>* exprs);
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

class ArrayAccessExpression : public Expression{
    private:
        ObjExpression* left;
        deque<Expression*>* dimensions;

    public:
        ArrayAccessExpression(ObjExpression* left, deque<Expression*>* dimensions);
        bool process(Symtable* environment, ClassSymtablePool* pool);
        void print();
};

#endif