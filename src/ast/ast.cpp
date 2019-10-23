#include <iostream>
#include "ast.hpp"
#include "error.hpp"

using std::cout;
using std::endl;

Program::Program(deque<ClassDeclaration*>* decls){
    if(decls != nullptr)
        this->declarations = decls;
    else
        this->declarations = new deque<ClassDeclaration*>;
}

void Program::print(){
    for(auto decl : *this->declarations){   
        decl->print();
        cout << endl;
    }
}

void Program::addClassDecl(ClassDeclaration* decl){
    this->declarations->push_back(decl);
}

void Program::addClassDeclAtFront(ClassDeclaration* decl){
    this->declarations->push_front(decl);
}

deque<ClassDeclaration*>* Program::getDecls(){
    return declarations;
}

// TODO: the class declarations should be in a hash map
ClassDeclaration* Program::getClassDecl(string className){
    for(auto decl : *declarations)
        if(decl->getName() == className)
            return decl;
    return nullptr;
}

Parameter::Parameter(Type* type, string name){
    this->type = type;
    this->name = name;
}

void Parameter::print(){
    printType(this->type);
    cout << " " << name;
}

Type* Parameter::getType(){
    return type;
}

string Parameter::getName(){
    return name;
}

Method::Method(string id, Type* returnType, Block* stmt) : Method(id, returnType, new deque<Parameter*>(), stmt){}

Method::Method(string id, Type* returnType, deque<Parameter*>* parameters, Block* stmt){
    this->id = id;
    this->returnType = returnType;
    this->statement = stmt;
    this->parameters = parameters;
}

string Method::getId(){
    return this->id;
}

Type* Method::getReturnType(){
    return this->returnType;
}

MethodType* Method::getType(){
    auto header = new vector<Type*>;
    header->push_back(returnType);

    if(parameters != nullptr) for(auto param : *parameters) header->push_back(param->getType());

    return new MethodType(header);
}

deque<Parameter*>* Method::getParameters(){
    return this->parameters;
}

Block* Method::getStatement(){
    return this->statement;
}

void Method::print(){
    printType(this->returnType);
    cout << " " << id << "(";

    if(this->parameters != nullptr){
        for(auto param : *this->parameters){
            param->print();
            cout << ", ";
        }
    }
    
    cout << ") ";

    if(this->statement != nullptr)
        this->statement->print();
    else
        cout << "{ }";
}

void Method::addParam(Parameter* param){
    this->parameters->push_back(param);
}

bool Method::processHeader(string className, ClassSymtable* root, ClassSymtablePool* pool){
    string methodName = id;

    if(root->get(methodName).tag != TCNOCONTENT){
        multipleMethodError(className, methodName);
        return false;
    }

    if(returnType->kind == TypeClass & pool->get(returnType->getClassName()) == nullptr){
        classNotDefinedError(returnType->getClassName());
        return false;
    }

    // Adds the method name
    TableContent tcMethodType;
    tcMethodType.tag = TCTYPE;
    tcMethodType.type = this->getType();
    root->insert(methodName, tcMethodType);

    Symtable* methodTable = new Symtable(className, root);

    // Adds the method params
    auto params = parameters;
    if(params != nullptr)
        for(auto param : *params){
            if(methodTable->get(param->getName()).tag != TCNOCONTENT){
                multiplyDefinedParamError(param->getName(), methodName, className);
                delete methodTable;
                return false;
            }

            methodTable->insert(param->getName(), tableContentFromType(param->getType()));
        }

    root->insertMethodTable(methodName, methodTable);
    return true;
}

Field::Field(Type* type, string name, Expression* initValue){
    this->type = type;
    this->name = name;
    this->initValue = initValue;
}

string Field::getName(){
    return name;
}

Type* Field::getType(){
    return type;
}

void Field::print(){
    printType(this->type);
    cout << " " << this->name;

    if(this->initValue != nullptr){
        cout << " = ";
        this->initValue->print();
    }

    cout << ";";
}

bool Field::process(string className, ClassSymtable* root){
    if(root->get(name).tag != TCNOCONTENT){
        multiplyDefinedFieldError(name, className);
        return false;
    }

    TableContent tc;
    tc.tag = TCTYPE;
    tc.type = type;
    root->insert(name, tc);
    return true;
}

ClassDeclaration::ClassDeclaration(string name) : ClassDeclaration(name, "") {}

ClassDeclaration::ClassDeclaration(string name, string parent){
    this->name = name;
    this->parent = parent;
    this->methods = new vector<Method*>;
    this->fields = new vector<Field*>;
}

void ClassDeclaration::print(){
    cout << "class " << this->name << " {" << endl;

    for(auto field : *this->fields){
        field->print();
        cout << endl;
    }
    for(auto method : *this->methods){
        method->print();
        cout << endl;
    }
    cout << "}";
}

void ClassDeclaration::addMethod(Method* method){
    this->methods->push_back(method);
}

void ClassDeclaration::addField(Field* field){
    this->fields->push_back(field);
}

string ClassDeclaration::getName(){
    return name;
}

string ClassDeclaration::getParent(){
    return parent;
}

vector<Method*>* ClassDeclaration::getMethods(){
    return methods;
}

vector<Field*>* ClassDeclaration::getFields(){
    return fields;
}

// TODO: the methods should be in a hashtable
Method* ClassDeclaration::getMethod(string methodName){
    for(Method* method : *methods) if(method->getId() == methodName) return method;
    return nullptr;
}

bool processesClassInheritanceHierarchy(deque<ClassDeclaration*>* declarations, ClassSymtablePool* pool){
    unordered_map<string, string> unprocClasses;
    unordered_map<string, string> procClasses;

    for(auto decl :*declarations)
        unprocClasses.emplace(decl->getName(), decl->getParent());

    auto itCurrent = unprocClasses.begin();
    string firstInChain = itCurrent->second;
    string parent;

    while(true){
        if(itCurrent->first == itCurrent->second){
            circularInheritanceError(itCurrent->first);
            return false;
        }

        parent = itCurrent->second;
        if(parent == ""){
            procClasses.emplace(itCurrent->first, itCurrent->second);
            unprocClasses.erase(itCurrent);

            if(unprocClasses.empty())
                return true;
            
            itCurrent = unprocClasses.begin();
            firstInChain = itCurrent->second;
            continue;
        }

        auto itParentUnproc = unprocClasses.find(parent);
        if(itParentUnproc != unprocClasses.end()){
            procClasses.emplace(itCurrent->first, itCurrent->second);
            unprocClasses.erase(itCurrent);

            itCurrent = itParentUnproc;
            continue;
        }

        auto itParentProc = procClasses.find(parent);
        if(itParentProc != procClasses.end()){
            if(itParentProc->second == firstInChain){
                circularInheritanceError(firstInChain);
                return false;
            }
            
            procClasses.emplace(itCurrent->first, itCurrent->second);
            unprocClasses.erase(itCurrent);

            if(unprocClasses.empty())
                return true;

            itCurrent = unprocClasses.begin();
            firstInChain = itCurrent->second;
            continue;
        }

        classNotDefinedError(parent);
        return false;
    }

    return true;
}
bool addClassNamesToPool(Program* program, ClassSymtablePool* pool){
    for(auto classDecl : *program->getDecls()){
        string className = classDecl->getName();

        if(pool->get(className) != nullptr) {
            multipleClassError(className);
            return false;
        }

        ClassSymtable* root = new ClassSymtable(className);
        pool->insert(className, root);
    }

    return true;
}

ClassSymtablePool* buildClassSymtablePool(Program* program){
    ClassSymtablePool* pool = new ClassSymtablePool;

    if(!addClassNamesToPool(program, pool)){
        delete pool;
        return nullptr;
    }

    if(!processesClassInheritanceHierarchy(program->getDecls(), pool)) {
        delete pool;
        return nullptr;
    }

    // Processes each class
    for(auto classDecl : *program->getDecls()){
        string className = classDecl->getName();
        ClassSymtable* root = pool->get(className);

        // Processes the fields
        auto fields = classDecl->getFields();
        if(fields != nullptr) 
            for(auto field : *fields)
                if(!field->process(className, root)){
                    delete pool;
                    return nullptr;
                }

        // Processes the method headers
        auto methods = classDecl->getMethods();
        if(methods != nullptr){
            for(auto method : *methods)
                if(!method->processHeader(className, root, pool)){
                    delete pool;
                    return nullptr;
                }
        }
    }

    // Processes each method body
    unordered_map<string, ClassSymtable*>* tablePool = pool->getPool();
    for(auto tablePoolEntry : *tablePool){
        string className = tablePoolEntry.first;
        ClassSymtable* classSymtable = tablePoolEntry.second;
        if(!classSymtable->processMethodBodies(program->getClassDecl(className), pool))  {
            delete pool;
            return nullptr;
        }
    }

    return pool;
}