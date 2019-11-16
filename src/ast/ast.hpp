#ifndef AST_HPP
#define AST_HPP

#include <deque>
#include <string>
#include <vector>
#include "symtable.hpp"
#include "type.hpp"

using std::deque;
using std::string;
using std::vector;

class Block;
class ClassDeclaration;
class Expression;
class Field;
class Parameter;
class StaticVisitor;

class Program{
    private:
        deque<ClassDeclaration*>* declarations;

    public:
        Program(deque<ClassDeclaration*>* decls);

        void addClassDecl(ClassDeclaration* decl);

        void addClassDeclAtFront(ClassDeclaration* decl);

        deque<ClassDeclaration*>* getDecls();

        ClassDeclaration* getClassDecl(string className);

        void print();
};

class Method{
    private:
        string id;
        Type* returnType;
        deque<Parameter*>* parameters;
        Block* statement;

    public:
        Method(string id, Type* returnType, Block* stmt);
        Method(string id, Type* returnType, deque<Parameter*>* parameters, Block* stmt);
        string getId();
        Type* getReturnType();
        MethodType* getType();
        deque<Parameter*>* getParameters();
        Block* getStatement();
        void addParam(Parameter* param);
        void print();

        friend class StaticVisitor;
};

class ClassDeclaration{
    private:
        string name;
        string parent;
        vector<Method*>* methods;
        vector<Field*>* fields;
    public:
        ClassDeclaration(string name);

        ClassDeclaration(string name, string parent);

        void addMethod(Method* method);

        string getName();

        string getParent();

        void addField(Field* field);

        vector<Method*>* getMethods();

        vector<Field*>* getFields();

        Method* getMethod(string methodName);

        void print();
};

class Field{
    private:
        Type* type;
        string name;
        Expression* initValue;

    public:
        Field(Type* type, string name, Expression* initValue);

        string getName();
        bool process(string className, ClassSymtable* root, ClassSymtablePool* pool);
        Type* getType();

        void print();
};

class Parameter{
    private:
        Type* type;
        string name;

    public:
        Parameter(Type* type, string name);

        Type* getType();

        string getName();

        bool process(Symtable* parent, ClassSymtablePool* pool);
        void print();
};

#endif