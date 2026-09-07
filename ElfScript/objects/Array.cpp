//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// NOTE: THIS is only really good if: ENABLE_CONSOLE_VALUE_CALLBACK is enabled
// NOTE: Limitation: this does not work : %foo=[1,{12,3}];
//-----------------------------------------------------------------------------
#include "Array.h"
#include "console/console.h"
#include <console/consoleTypes.h>
#include <console/localVar.h>
#include <core/strings/stringUnit.h>

// ----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Array);
// ----------------------------------------------------------------------------
bool Array::onAdd()  {
    fromFields(true); //check if we have fields to merge
    return Parent::onAdd();
}
// -------------------------------------------------------------------------
void Array::write(Stream& stream, U32 tabStop, U32 flags)
{
    this->toFields();
    Parent::write(stream, tabStop, flags);

}
// -------------------------------------------------------------------------
void Array::toFields()
{
    S32 count = mValues.size();
    if (count < 1) return;


    ConsoleValue v;
    v.setInt(count);
    this->pushDynamicField(StringTable->insert("_populate"), nullptr, &v);

    for(S32 i = 0; i < count; i++) {
        this->pushDynamicField(StringTable->insert(avar("_p%d", i)), nullptr, &this->mValues[i]);
    }

}
// -------------------------------------------------------------------------
void Array::fromFields(bool removeFields)
{
    StringTableEntry fldName = nullptr;

    ConsoleValue v;
    SimFieldDictionary * dict = this->getFieldDictionary();
    v.setInt(0);
    fldName = StringTable->insert("_populate");
    if (!this->stackDynamicField( fldName, nullptr, &v)) return;
    S32 count = v.getInt();
    if ( count  <= 0) return;
    if (removeFields) {
        dict->setFieldValue(fldName, nullptr);
    }
    this->mValues.clear();


    for(S32 i = 0; i < count; i++) {
        fldName = StringTable->insert(avar("_p%d", i));
        if (this->stackDynamicField(fldName, nullptr, &v)) {
            mValues.push_back(v);
            if (removeFields) {
                dict->setFieldValue(fldName, nullptr);
            }
        }
    }

}
// ----------------------------------------------------------------------------
// push String parts to array and return the count of the pushed values
S32 Array::fromString(const char* text, bool doAdd )
{
    if (!text || text[0] == '\0') {
        Con::errorf("Empty text cant be converted to object");
        return 0;
    }

    const char* set = "\t\n";
    // we try to separate by tabs to keep stuff like "Hello World" TAB "tom"
    U32 count =  StringUnit::getUnitCount( text, "\t\n" );
    // we only got no or one token - switch to space / tab separated
    if (count < 2) {
        count =  StringUnit::getUnitCount( text, " \t\n" );
        set = " \t\n";
    }

    // nothing - is empty ?
    if (count < 1) {
        // Con::errorf("Empty text cant be converted to object");
        return 0;
    }

    if (!doAdd) this->mValues.clear();

    for (U32 i = 0; i < count; i++) {
        const char * token = StringUnit::getUnit( text, i, set );

        ConsoleValue value;
        // try to typeCast:
        if (isInt(token)) value.setInt(dAtol(token));
        else if (isFloat(token)) value.setFloat(dAtod(token));
        else value.setString(token);

        this->mValues.push_back(value);
    }

    return count;
}

// ----------------------------------------------------------------------------
DefineEngineMethod(Array, at, ConsoleValue, (S32 index), , "fetch a value at index") {
    if (!object->isIndexValid(index)) return ConsoleValue();
    return object->mValues[index];
}

DefineEngineMethod(Array, first, ConsoleValue, (), , "fetch first ") {
    if (!object->isIndexValid(0)) return ConsoleValue();
    return object->mValues[0];
}
DefineEngineMethod(Array, last, ConsoleValue, (), , "fetch last ") {
    S32 size = object->mValues.size();
    if (size == 0) return ConsoleValue();
    return object->mValues[size - 1];
}

// -----------------------------------------------------------------------------
DefineEngineMethod(Array, size, S32, (), , "get the size") {
    return object->mValues.size();
}

DefineEngineMethod(Array, clear, void, (), , "clear the Vector") {
    object->mValues.clear();
}

DefineEngineMethod(Array, reserve, void, (U32 size), , "reserve elements in the array") {
    object->mValues.reserve(size);
}

DefineEngineMethod(Array, fill, void, (ConsoleValue value), , "fill the array with value") {
    object->mValues.fill(value);
}
// -----------------------------------------------------------------------------
DefineEngineMethod(Array, push_back, void, (ConsoleValue value), , "push back a value") {
    object->mValues.push_back(value);
}

// -----------------------------------------------------------------------------
DefineEngineMethod(Array, fromString, S32, (const char* text, bool doAdd), (true),
                   "add values from a TAB or SPC speparated String\n"
                   "@param doAdd true = add values, false = fill with this and clean before"
                   "@return count of the added values"
) {
   return  object->fromString(text, doAdd);
}
// -----------------------------------------------------------------------------

DefineEngineMethod(Array, set, bool, (S32 index, ConsoleValue value), , "set a value at index") {
    if (!object->isIndexValid(index)) return false;
    object->mValues[index] = value;
    return true;
}


DefineEngineMethod(Array, insert, bool, (S32 position,ConsoleValue value), , "insert a value at index") {
    if (!object->isIndexValid(position)) return false;
    object->mValues.insert(position, value);
    return true;
}
DefineEngineMethod(Array, erase, bool, (S32 position), , "") {
    if (!object->isIndexValid(position)) return false;
    object->mValues.erase(position);
    return true;
}

DefineEngineMethod(Array, pop_front, ConsoleValue, (), , "pop the first and return the removed value") {
    ConsoleValue result;
    if (!object->isIndexValid(0)) return result;
    result = object->mValues[0];
    object->mValues.pop_front();
    return result;
}

DefineEngineMethod(Array, pop_back, ConsoleValue, (), , "pop the last and return the remove value") {
    ConsoleValue result;
    S32 size = object->mValues.size();
    if (size == 0) return result;
    result = object->mValues[size - 1];
    object->mValues.pop_back();
    return result;
}
// -----------------------------------------------------------------------------
DefineEngineMethod(Array, toFields, void, (),
                   ,"Copy Array to dynamic fields fieldnames start with _p"
){
    object->toFields();
}
// -----------------------------------------------------------------------------
DefineEngineMethod(Array, fromFields, void, (bool deleteFieldAfterAdd),(false)
                   ,"fill Array from dynamic fields\n"
                   "fieldnames start with _p and count is in _populate\n"
                   "this is used by save automaticly"

){
    object->fromFields(deleteFieldAfterAdd);
}
// -----------------------------------------------------------------------------
DefineEngineMethod(Array, concat, bool, (Array* other, bool deleteOther),(false) ,
                   "Concat (append) a other array to this one\n"
                   "@param Array* other the other array\n"
                   "@param bool deleteOther delete other object after merge\n"
                   "@return bool !failed"
) {
    if (!other || other->mValues.size() == 0) return false;

    object->mValues.merge(other->mValues);
    if (deleteOther) other->deleteObject();
    return true;
}
// -----------------------------------------------------------------------------
DefineEngineMethod(Array, list, void, (), , "List the values") {
    Con::printSeparator();
    for (S32 i = 0; i < object->mValues.size(); i++) {
        Con::printf("#%04d [type:%8s] [value:%20s]"
        , i, ElfScript::getConsoleValueTypeName(object->mValues[i].type), object->mValues[i].getString());
    }
}
