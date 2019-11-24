#include "value.hpp"
#include <iostream>
using std::to_string;

IntValue::IntValue(int v){
    value = v;
    className = "IntValue";
}

IntValue::IntValue() : IntValue(0){}

string IntValue::toString(){
    return to_string(value);
}

int IntValue::getInt() const {
    return this->value;
}

BoolValue::BoolValue() : BoolValue(false){}

BoolValue::BoolValue(bool v){
    value = v;
    className = "BoolValue";
}

string BoolValue::toString(){
    return value ? "true" :  "false";
}

bool BoolValue::getBool() const {
    return this->value;
}

StringValue::StringValue() : StringValue(""){}

StringValue::StringValue(string v){
    this->value = v;
    className = "StringValue";
}

string StringValue::getString() const{
    return value;
}

string StringValue::toString(){
    return getString();
}

ArrayValue::ArrayValue(Value** v, int n){
    value = v;
    this->n = n;
    className = "ArrayValue";
}

Value** ArrayValue::getArray() const{
    return value;
}

string ArrayValue::toString(){
    // TODO
    return "";
}

ArrayValue::ArrayValue(int* dims, int n, EnumValue ev) : ArrayValue(dims,1,n,ev){}

// TODO: are we allowed to use loops here? if we are not, then we should use
// goto's or something similar
ArrayValue::ArrayValue(int* dims, int i, int n, EnumValue ev){
    // TODO: put this constructor in a separate recursive method so that this assignment
    // doesn't get executed every single time
    className = "ArrayValue";

    this->value = new Value*[dims[i-1]];
    if(i == n){
        for(int j = 0; j < dims[i-1]; ++j){ 
            // TODO: ClassValue
            switch(ev){
                case EnumValue::EV_IntValue:
                    this->value[j] = new IntValue();
                    break;
                case EnumValue::EV_StringValue:
                    this->value[j] = new StringValue();
                    break;
                case EnumValue::EV_BoolValue:
                    this->value[j] = new BoolValue();
                    break;
            }
        }
    } else{
        for(int j = 0; j < dims[i-1]; ++j) {
            this->value[j] = new ArrayValue(dims, i+1,n,ev);
        }
    }
}

ClassValue::ClassValue(string className){
    this->className = className;
}


// int operators
IntValue* operator-(const IntValue& v1, const IntValue& v2) {
    return new IntValue(v1.value - v2.value);
}

IntValue* operator-(const IntValue& v1) {
    return new IntValue(-v1.value);
}

IntValue* operator*(const IntValue& v1, const IntValue& v2) {
    return new IntValue(v1.value * v2.value);
}

IntValue* operator/(const IntValue& v1, const IntValue& v2) {
    return new IntValue(v1.value / v2.value);
}

IntValue* operator%(const IntValue& v1, const IntValue& v2) {
    return new IntValue(v1.value % v2.value);
}

// int and String operators
template<typename Val>
Val* operator+(const Val& v1, const Val& v2) {
    return new Val(v1.value + v2.value);
}

template<typename Val>
BoolValue* operator<(const Val& v1, const Val& v2) {
    return new BoolValue(v1.value < v2.value);
}

template<typename Val>
BoolValue* operator<=(const Val& v1, const Val& v2) {
    return new BoolValue(v1.value <= v2.value);
}

template<typename Val>
BoolValue* operator>(const Val& v1, const Val& v2) {
    return new BoolValue(v1.value > v2.value);
}

template<typename Val>
BoolValue* operator>=(const Val& v1, const Val& v2) {
    return new BoolValue(v1.value >= v2.value);
}

// boolean operators
BoolValue* operator!(const BoolValue& v1) {
    return new BoolValue(!v1.value);
}

BoolValue* operator||(const BoolValue& v1, const BoolValue& v2) {
    return new BoolValue(v1.value || v2.value);
}

BoolValue* operator&&(const BoolValue& v1, const BoolValue& v2) {
    return new BoolValue(v1.value && v2.value);
}

// array operators
Value* Value::operator[](const int i){
    ArrayValue* arr = dynamic_cast<ArrayValue*>(this);
    return (arr->value)[i];
}

// all types operators
template<typename Val>
BoolValue* operator==(const Val& v1, const Val& v2) {
    return new BoolValue(v1.value == v2.value);
}

template<typename Val>
BoolValue* operator!=(const Val& v1, const Val& v2) {
    return new BoolValue(v1.value != v2.value);
}