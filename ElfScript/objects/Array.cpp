//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// NOTE: THIS is only really good if: ENABLE_CONSOLE_VALUE_CALLBACK is enabled
// NOTE: Limitation: this does not work : %foo=[1,{12,3}];
// TODO: Save
//-----------------------------------------------------------------------------
#include "Array.h"
#include "console/console.h"
#include <console/consoleTypes.h>
#include <console/localVar.h>

// ----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT(Array);
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
DefineEngineMethod(Array, list, void, (), , "List the values") {
    Con::printSeparator();
    for (S32 i = 0; i < object->mValues.size(); i++) {
        Con::printf("#%04d [type:%8s] [value:%20s]"
        , i, ElfScript::getConsoleValueTypeName(object->mValues[i].type), object->mValues[i].getString());
    }
}
