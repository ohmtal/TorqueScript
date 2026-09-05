#include "console/engineAPI.h"
#include <console/consoleTypes.h>
#include <console/consoleInternal.h>
#include <console/torquescript/ast.h>
#include <console/torquescript/compiler.h>
#include <console/localVar.h>
#include <core/strings/stringUnit.h>

#include <objects/Array.h>

///////////////////////////////////////////////////////
// some testfunction .......

// Script::gEvalState.setCurVarNameCreate(var);

// toObject moved to console functions
// FIXME moved to localvar :D
// DefineEngineFunction(toArray,S32, (const char* varName),
//     ,"Convert an string content of varname to an set typed of variables with varName[0..count] fields\n"
//      "tab separated (default) or space separated"
//      "@return the count of variables"
// ) {
//
//     const char* text = ElfScript::getLocalString(varName);
//
//     if (!text || text[0] == '\0') {
//         Con::errorf("Variable %s is empty and cant be converted to object", varName);
//         return 0;
//     }
//
//     const char* set = "\t\n";
//     // we try to separate by tabs to keep stuff like "Hello World" TAB "tom"
//     U32 count =  StringUnit::getUnitCount( text, "\t\n" );
//     // we only got no or one token - switch to space / tab separated
//     if (count < 2) {
//         count =  StringUnit::getUnitCount( text, " \t\n" );
//         set = " \t\n";
//     }
//
//     // nothing - is empty ?
//     if (count < 1) {
//         Con::errorf("Variable %s is empty and cant be converted to object", varName);
//         return 0;
//     }
//     char buff[256];
//     StringTableEntry fieldNameEntry = nullptr;
//     for (U32 i = 0; i < count; i++) {
//         const char * token = StringUnit::getUnit( text, i, set );
//
//         dSprintf(buff,256,"%s%d",varName, i); //mhh or as array ?
//         fieldNameEntry = StringTable->insert( buff );
//
//         Script::gEvalState.setCurVarNameCreate(fieldNameEntry);
//
//         ConsoleValue* stack = ElfScript::getLocalVariable(fieldNameEntry);
//         if (!stack) return 0;
//         if (isInt(token)) stack->setInt(dAtol(token));
//         else if (isFloat(token)) stack->setFloat(dAtod(token));
//         else stack->setString(token);
//     }
//     // clean original content:
//     ElfScript::setLocalString(varName, "");
//
//     return count;
// }


// ----------------------------------
DefineEngineFunction(Test_FindObject, S32, (const char* objectname), , "")
{
    // only named and by ID (as string) work here ?!!?!?!!
    // fixed $ now also works ... !!!
    SimObject* obj =  Sim::findObject(objectname);
   if (obj) return obj->getId();
   return 0;
}
DefineEngineFunction(Test_FindObjectByVariable, S32, (const char* VariableName), , "")
{
    // new ElfScript  function
    SimObject* obj =  Con::getObjectByNameWithToken(VariableName);
    if (obj) return obj->getId();
    return 0;
}

DefineEngineFunction(setFloatVariable, void, (const char* VariableName, F32 value), , "")
{
    // ok i only fixed $ objects now also work but nothing else ....
    // maybe later i started and cancled
    Con::setFloatVariable(VariableName, value);
}

// ================================================================================================
// Playing with local vars .... moved to console/localVar.cpp
// ================================================================================================
    extern  FuncVars gEvalFuncVars;
    extern  FuncVars gGlobalScopeFuncVars;

namespace ElfScriptDEBUG {

    // -----------------------------------------------------------------------------
    FuncVars* getFuncVars() {
         return Compiler::gIsEvalCompile ? &gEvalFuncVars : &gGlobalScopeFuncVars;
    }
    // -----------------------------------------------------------------------------
    S32 findLocalVarRegisterInCurrentScope(const char* variableName)  {
        // sanity
        if (!variableName || variableName[0] != '%') return -1;

        // check we are in a function
        Dictionary& stackFrame = Script::gEvalState.getCurrentFrame();
        if (stackFrame.scopeName && stackFrame.scopeNamespace ){
            StringTableEntry functionName = stackFrame.scopeName;
            StringTableEntry namespaceName = stackFrame.scopeNamespace->mName;

            StringTableEntry varToLookup = StringTable->insert(variableName);
            return ((CodeBlock*)stackFrame.module)->variableRegisterTable.lookup(namespaceName, functionName, varToLookup);
        }

        // 2. we should be in global scope
        return getFuncVars()->lookupExising(StringTable->insert(variableName));

    }
    // -----------------------------------------------------------------------------
    const char* getConsoleValueTypeName(S32 type) {
        switch (type) {
            case ConsoleValueType::cvFloat:   return "Float";
            case ConsoleValueType::cvInteger: return "Integer";
            // // // case ConsoleValueType::cvString:  return "String";
            case ConsoleValueType::cvSTEntry: return "String";
    #ifdef ENABLE_CONSOLE_VECTOR
            case ConsoleValueType::cvVector:  return "Vector";
    #endif
            default: return "other";
        }
    }
    // -----------------------------------------------------------------------------
    void varDumpLocals(const char* variableName)
    {
        if (!variableName) return;

        if (variableName[0] != '%') {
            Con::errorf("Sorry [%s] in no local variable %s", variableName);
            return;
        }

        S32 reg = findLocalVarRegisterInCurrentScope(variableName);

        if (reg < 0) {
            Con::printf("%s not found.", variableName);
            return ;
        }


        ConsoleValue& localVal = Script::gEvalState.currentRegisterArray->values[reg];
        Con::printf("Variable: %10s [reg:%2d] [type:%8s] [value:%20s]"
        , variableName, reg, getConsoleValueTypeName(localVal.type), localVal.getString());
    }
    // -----------------------------------------------------------------------------
    void dumpAllLocalVariables() {
        // 1. gEvalFuncVars
        Con::printSeparator();
        Con::printf("       ------------------- GlobalScope -------------------");
        getFuncVars()->listExising();

        Con::printSeparator();
        Con::printf("       ------------------- LocalScope -------------------");
        CompilerLocalVariableToRegisterMappingTable* tbl = &Compiler::getFunctionVariableMappingTable();
        if (!tbl) {
            Con::errorf("no CompilerLocalVariableToRegisterMappingTable found ");
            return ;
        }

        for (auto& [funcName, maptbl] : tbl->localVarToRegister) {
            Con::printf("%s, count ", funcName, maptbl.varList.size());
            for (S32 i = 0 ; i < maptbl.varList.size(); i++)
                Con::printf("   - %s", maptbl.varList[i]);
            // Con::printf("%s: reg:%d currentType: %d", key, val.reg ,(S32)val.currentType);
        }



        Con::printSeparator();
    }
    // -----------------------------------------------------------------------------
}
// ================================================================================================
/*
DefineEngineFunction(dumpLocals, void, (),,"") {
    ElfScriptDEBUG:: dumpAllLocalVariables();
}
// -----------------------------------------------------------------------------

DefineEngineFunction(whereAmI, void,(),,"look up the function where i'am called from") {
    Dictionary& stackFrame = Script::gEvalState.getCurrentFrame();
    if (!stackFrame.scopeName || !stackFrame.scopeNamespace ){
        Con::printf("Global scope i guess ...");
        return;
    }
    StringTableEntry functionName = stackFrame.scopeName;
    StringTableEntry namespaceName = stackFrame.scopeNamespace->mName;

    Con::printSeparator();
    Con::printf("you are in function:  [%s::%s] ",
        namespaceName ? namespaceName : ""
        , functionName ? functionName :"unknown");

    // StringTableEntry varToLookup = StringTable->insert(variableName);
    // S32 registerId = ((CodeBlock*)stackFrame.module)->variableRegisterTable.lookup(namespaceName, functionName, varToLookup);
}*/


// DefineEngineFunction(varDump, void, (const char* variableName), , "local variable dump, you cant use this on console!")
// {
//     ElfScriptDEBUG::varDumpLocals(variableName);
// }

DefineEngineFunction(setLocalFloatVariable, void, (const char* VariableName, F32 value), , "local float test!")
{
    if (VariableName[0] != '%') {
        Con::errorf("must be a local Variable!");
        return;
    }

    S32 reg = ElfScriptDEBUG::findLocalVarRegisterInCurrentScope(VariableName);

    if (reg < 0) {
        Con::errorf("I can only set the value on a existing local variable!");
        return ;
    }

    Con::printf(" --- Found register %d for %s", reg, VariableName);

    // is 0 valid ??
    if (reg > 0 ) {
        ConsoleValue& localVal = Script::gEvalState.currentRegisterArray->values[reg];
        Con::printf(" --- %s type is (%d) %s", VariableName, localVal.type, ElfScript::getConsoleValueTypeName(localVal.type));
        localVal.setFloat(value);
        Con::printf(" --- type after setFloat: (%d) %s", localVal.type, ElfScript::getConsoleValueTypeName(localVal.type));
    }

}

///////////////////////////////////////////////////////
    // class EmptyObject : public SimObject
    // {
    //     typedef SimObject Parent;
    //
    // public:
    //     DECLARE_CONOBJECT(EmptyObject);
    // };
    // IMPLEMENT_CONOBJECT(EmptyObject);
//--------------

struct Point2F{
    F32 x = 0;
    F32 y = 0;
};

IMPLEMENT_ENGINE_TYPE_TRAITS(Vector<F32>);

class TestObj: public SimObject
{
    typedef SimObject Parent;
public:
    DECLARE_CONOBJECT(TestObj);
    F32 mX, mY, mZ;
    F64 mDouble;
    S32 mIndex;
    bool mBool;
    StringTableEntry mTestName;
    S32 mSlots[3];
    S8 mS8 = 0;
    S8 mU8 = 0;

    U32 mU32 = 0;
    U64 mU64 = 0;
    S64 mS64 = 0;

    /*
     * Storing Point in a vector Vector implementation
     *
     * property pointSize set/get the size of this vector to speed up access
     *          i fo not use push at mPoints this should be pretty fast
     * setPoint store x,y at a index in this vector
     * pushPoint set mX and mY to a point stored in mPoints(shoudnt it by pop? )
     */
    Vector<Point2F> mPoints;

    TestObj() {
        mX = mY = mZ = 0.f;
        mDouble = 0.f;
        mIndex = 0;
        mBool = false;
        mTestName = StringTable->insert("NoName");
        for (S32 i = 0; i < 3; i++) mSlots[i]=0;
    }

    static bool _setPointSize(void* obj,const char* , const char* data) {
        TestObj* object = static_cast<TestObj*>(obj);
        if (!object || !data) {
            Con::errorf("Failed to set pointSize!");
            return false;
        }
        S32 size = dAtoi(data);
        if ( size >= 0 ) {
            object->mPoints.setSize(size);
            dMemset(object->mPoints.address(), 0, object->mPoints.size() * sizeof(Point2F));
            Con::printf("New point size is: %d", size);
            //NOT! return true;
        }
        return false;
    }
    static const char *_getPointSize(void* obj, const char* data) {
        TestObj* object = static_cast<TestObj*>(obj);
        if (!object) return "";
        return Con::getIntArg(object->mPoints.size());
    }


    static void initPersistFields()
    {
        Parent::initPersistFields();
        addField("testS8", TypeS8, Offset(mS8, TestObj));
        addField("testU8", TypeU8, Offset(mU8, TestObj));
        addField("testU32", TypeU32, Offset(mU8, TestObj));
        addField("testU64", TypeU64, Offset(mU8, TestObj));
        addField("testS64", TypeS64, Offset(mU8, TestObj));
        addField("x",     TypeF32,     Offset(mX, TestObj));
        addField("y",     TypeF32,     Offset(mY, TestObj));
        addField("z",     TypeF32,     Offset(mZ, TestObj));
        addField("testFloat", TypeF64,   Offset(mDouble, TestObj));
        addField("testInt",  TypeS32,    Offset(mIndex, TestObj));
        addField("testBool", TypeBool,   Offset(mBool, TestObj));
        addField("testString", TypeString, Offset(mTestName, TestObj));
        addField("testSlot", TypeS32, Offset(mSlots, TestObj), 3); //WARNING count is not validated!!!
        //----
        addProtectedField("pointSize", TypeS32, 0, &_setPointSize,&_getPointSize, "Set the point size (element count).");


    }

    void setPos(F32 x, F32 y, F32 z) {
        mX = x;
        mY = y;
        mZ = z;
    }
};

IMPLEMENT_CONOBJECT(TestObj);

DefineEngineMethod(TestObj, getPos, String, (), , "get the position") {
    // we do nothing special here
    StringBuilder str;
    str.format("%g %g %g", object->mX, object->mY, object->mZ);
    return Con::getStringArg(str.end());
}

DefineEngineMethod(TestObj, getX, F32, (), , "") {
  return object->mX;
}
DefineEngineMethod(TestObj, getY, F32, (), , "") {
    return object->mY;
}
DefineEngineMethod(TestObj, getZ, F32, (), , "") {
    return object->mZ;
}

DefineEngineMethod(TestObj, walk, void, (F32 x, F32 y, F32 z), , "Set point where to walk to") {
    // we do nothing special here
    object->setPos(x,y,z);
}

DefineEngineMethod(TestObj, setPoint, bool, (S32 index, F32 x, F32 y), , "Set point at index") {
    if (index < 0 || index >= object->mPoints.size()) return false;
    object->mPoints[index] = { x, y};
    return true;
}



DefineEngineMethod(TestObj, getPoint, Vector<F32>, (S32 index), , "Get point at index and return it as vector<F32> basically a string, slow shit ;)") {
    if (index < 0 || index >= object->mPoints.size()) return {};
    Vector<F32> tmpVector;
    tmpVector.clear();
    tmpVector.push_back(object->mPoints[index].x);
    tmpVector.push_back(object->mPoints[index].y);
    return tmpVector;
}

DefineEngineMethod(TestObj, pushPoint, bool, (S32 index), , "push point at index to x,y") {
    if (index < 0 || index >= object->mPoints.size()) return false;

    object->mX =  object->mPoints[index].x;
    object->mY = object->mPoints[index].y;
    return true;
}

// this is executed at the same speed as .pushPoint. so the VM does NOT cache %this object in functions
// Console value need a pointer field
DefineEngineFunction(Test_PushPoint, bool, (S32 pId, S32 index), , "same as pushpoint but as function") {
    TestObj* object = dynamic_cast<TestObj*>(Sim::findObject(pId));
    if (!object || index < 0 || index >= object->mPoints.size()) return false;
    object->mX =  object->mPoints[index].x;
    object->mY = object->mPoints[index].y;
    return true;
}

// -----------------------------------------
// $t = new TestObj(); $t.showVec();
DefineEngineMethod(TestObj, showVec, void, (ConsoleVector vec),,"") {
    Con::printf("VEC: %f, %f, %f, %f", vec.points[0], vec.points[1], vec.points[2], vec.points[3]);
}

DefineEngineMethod(TestObj, getVec, ConsoleVector, (),,"") {
    return ConsoleVector({1.1,2.2,3.3,4.4});
}

// damit i did not know that: ... how cool is that! -  using the pointer to id translator
DefineEngineMethod(TestObj, toArray, Array*, (bool testNull),(false),"") {
    if (testNull) return nullptr;
    Array* result = new Array();
    result->registerObject();
    ConsoleValue tempValue;
    tempValue.setString("Hello Array!");
    result->mValues.reserve(100);
    result->mValues.fill(tempValue);
    return result;
}

DefineEngineMethod(TestObj, fromArray, void, (Array* array),,"") {
    if (array) Con::printf("We got an array object!");
    else Con::printf("We got an NULLPTR instead an array object!");

}


