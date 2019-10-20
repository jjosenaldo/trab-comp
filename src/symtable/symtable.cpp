#include <iostream>
#include "symtable.hpp"
#include "type.hpp"

using std::cout;
using std::endl;

Symtable::Symtable(){
    this->parent = nullptr;
}

Symtable::Symtable(Symtable* parent){
    this->parent = parent;
}

Symtable* Symtable::getParent(){
    return this->parent;
}

void Symtable::insert(string id, TableContent content){
    this->table[id] = content;
}

TableContent Symtable::get(string id){
    if(this->table.find(id) != this->table.end())
        return this->table[id];
    else{
        TableContent tcReturn;
        tcReturn.tag = TCNOCONTENT;
        return tcReturn;
    }
}

void Symtable::print(){
    cout << "{" << endl;
    for(auto& entry : this->table){
        cout << entry.first << " : ";
        printTableContent(entry.second);
        cout << endl;
    }
    cout << "}";
}

void SymtablePool::insert(string className, Symtable* table){
    this->pool[className] = table;
}

Symtable* SymtablePool::get(string className){
    auto it = this->pool.find(className);

    if(it == this->pool.end())
        return nullptr;
    
    else
        return it->second;
}

void SymtablePool::print(){
    for(auto it : this->pool){
        cout << it.first << " : ";
        it.second->print();
        cout << endl;
    }
}

void printTableContent(TableContent content){
    if(content.tag == TCTYPE){
        printType(content.type);
    } else if(content.tag == TCSYMTABLE){
        content.symtable->print();
    }
}