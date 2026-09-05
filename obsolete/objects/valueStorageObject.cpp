//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// ElfScript StorageObject
// like PointStorageObject but a list of ConsoleValues
//-----------------------------------------------------------------------------
#include "valueStorageObject.h"


IMPLEMENT_CONOBJECT(ValueStorageObject);


//-----------------------------------------------------------------------------
static bool _setStorageSize(void* obj,const char* , const char* data) {
    ValueStorageObject* object = static_cast<ValueStorageObject*>(obj);
    if (!object || !data) {
        Con::errorf("Failed to set pointSize!");
        return false;
    }
    object->setStorageSize(dAtoui(data));
    return false;
}
static const char *_getStorageSize(void* obj, const char* data) {
    ValueStorageObject* object = static_cast<ValueStorageObject*>(obj);
    if (!object) return "";
    return Con::getIntArg(object->mValues.size());
}
//-----------------------------------------------------------------------------
void ValueStorageObject::initPersistFields()
{
    Parent::initPersistFields();

    addProtectedField("storageSize", TypeU32, 0, &_setStorageSize,&_getStorageSize, "Set the storage size (how many Values we can work with) Max:1000000.");
}
//-----------------------------------------------------------------------------
// Methods
//-----------------------------------------------------------------------------
DefineEngineMethod(ValueStorageObject, getFloat, F64, (S32 index), , "get the float (double)value at index") {
    if (!object->isIndexValid(index)) return 0.f;
    return object->mValues[index].getFloat();
}
DefineEngineMethod(ValueStorageObject, setFloat, bool, (S32 index, F64 value), , "set the float(double) value at index") {
    if (!object->isIndexValid(index)) return false;
    object->mValues[index].setFastFloat(value);
    return 0;
}

DefineEngineMethod(ValueStorageObject, getInt, S64, (S32 index), , "get the integer(long) value at index") {
    if (!object->isIndexValid(index)) return 0.f;
    return object->mValues[index].getInt();
}
DefineEngineMethod(ValueStorageObject, setInt, bool, (S32 index, F64 value), , "set the integer(long) value at index") {
    if (!object->isIndexValid(index)) return false;
    object->mValues[index].setFastInt(value);
    return 0;
}

DefineEngineMethod(ValueStorageObject, getString, const char* , (S32 index), , "get the sttring value at index") {
    if (!object->isIndexValid(index)) return "";
    return object->mValues[index].getString();
}
DefineEngineMethod(ValueStorageObject, setString, bool, (S32 index, const char* value), , "set the string value at index") {
    if (!object->isIndexValid(index)) return false;
    object->mValues[index].setString(value);
    return 0;
}
#ifdef ENABLE_CONSOLE_VECTOR
DefineEngineMethod(ValueStorageObject, getVector, ConsoleVector , (S32 index), , "get the Vector value at index") {
    if (!object->isIndexValid(index)) return {0};
    return object->mValues[index].getVector();
}
DefineEngineMethod(ValueStorageObject, setVector, bool, (S32 index, ConsoleVector value), , "set the vector value at index") {
    if (!object->isIndexValid(index)) return false;
    object->mValues[index].setVector(value);
    return 0;
}
#endif
