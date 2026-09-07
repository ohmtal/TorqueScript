//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// Local Variable (%) Power :D ... also added  global ($) but kept the nameing
//-----------------------------------------------------------------------------

#include "console/engineAPI.h"
#include <console/consoleTypes.h>
#include <console/consoleInternal.h>
#include <console/torquescript/ast.h>
#include <console/torquescript/compiler.h>
#include <core/strings/stringUnit.h>


extern  FuncVars gEvalFuncVars;
extern  FuncVars gGlobalScopeFuncVars;

//-----------------------------------------------------------------------------
namespace ElfScript {

    const char* getConsoleValueTypeName(S32 type) {
        switch (type) {
            case ConsoleValueType::cvFloat:   return "Float";
            case ConsoleValueType::cvInteger: return "Integer";
            // case ConsoleValueType::cvString:  return "String";
            case ConsoleValueType::cvSTEntry: return "String";
            case ConsoleValueType::cvConsoleValueType: return "Console";
            #ifdef ENABLE_CONSOLE_VECTOR
            case ConsoleValueType::cvVector:  return "Vector";
            #endif
            default: return avar("type:%d", type);
        }
    }
    // -----------------------------------------------------------------------------
    inline FuncVars* _getFuncVars() {
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
        return _getFuncVars()->lookupExising(StringTable->insert(variableName));

    }

    //-----------------------------------------------------------------------------
    //
    //  get a local or global variable ConsoleValue
    //  i also added global but keep the name getLocalVariable
    //
    bool getLocalVariable(const char* variableName, ConsoleValue*& stack, S32& reg){
        if (!variableName) return false;

        if (variableName[0] == '%') {
            reg = findLocalVarRegisterInCurrentScope(variableName);
            if (reg < 0) return false;
            stack = &Script::gEvalState.currentRegisterArray->values[reg];
            if (!stack ) return false;
            return true;
        }
        else if (variableName[0] == '$') {
            Dictionary::Entry *entry =Con::gGlobalVars.lookup(StringTable->insert(variableName));
            if (!entry) return false;
            stack = &entry->getValue();
            if (!stack ) return false;
            return true;
        }


        return false;
    }

    ConsoleValue* getLocalVariable(const char* variableName ){
        ConsoleValue* stack = nullptr; S32 reg = -1;
        if (!getLocalVariable(variableName, stack, reg)) {
            return nullptr;
        }
        return stack;
    }

    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    // Was a test - again - when do i learn it does not work with local :P
    bool CreateVar(const char* variableName, ConsoleValue*& stack)
    {
        if (!variableName ) return false;


        if (variableName[0] == '%') {

            // !!!!!!!!!!!!!!!!!!!!!!!!
            Con::errorf("---- ElfScript::CreateVar: Local variables can only be created at compile time! ----");
            return false;
            // !!!!!!!!!!!!!!!!!!!!!!!!

            S32 varRegister = -1;

            // check we are in a function
            Dictionary& stackFrame = Script::gEvalState.getCurrentFrame();
            if (stackFrame.scopeName && stackFrame.scopeNamespace ){
                StringTableEntry functionName = stackFrame.scopeName;
                StringTableEntry namespaceName = stackFrame.scopeNamespace->mName;

                StringTableEntry varToLookup = StringTable->insert(variableName);
                varRegister =  ((CodeBlock*)stackFrame.module)->variableRegisterTable.lookup(namespaceName, functionName, varToLookup);

                // does not exists try to add and lookup again
                if (varRegister < 0) {
                    ((CodeBlock*)stackFrame.module)->variableRegisterTable.add(functionName, namespaceName,  varToLookup);
                    varRegister =  ((CodeBlock*)stackFrame.module)->variableRegisterTable.lookup(namespaceName, functionName, varToLookup);
                }

            } else {
                // 2. we should be in global scope
                // return _getFuncVars()->lookupExising(StringTable->insert(variableName));

            }

            // check again
            if (varRegister < 0) return false;
            stack = &Script::gEvalState.currentRegisterArray->values[varRegister];
            if (!stack ) {
                Con::errorf("ElfScriot::CreateVar failed to create variable:%s", variableName);
                return false;
            }
            return true;


            // This is tricky
            return false;

        } else if (variableName[0] == '$') {
            // This add does also check it exits !
            Dictionary::Entry *entry = Con::gGlobalVars.add(StringTable->insert(variableName));
            if (!entry) return false;
            stack = &entry->getValue();
            if (!stack ) return false;
            return true;
        }

        return false;
    }
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
    bool setLocalFloat(const char* variableName, F64 value) {
        ConsoleValue* stack = getLocalVariable(variableName);
        if (stack == nullptr) return false;
        stack->setFloat(value);
        return true;
    }
    //-----------------------------------------------------------------------------
    bool setLocalInt(const char* variableName, S64 value) {
        ConsoleValue* stack = getLocalVariable(variableName);
        if (stack == nullptr) return false;
        stack->setInt(value);
        return true;
    }
    //-----------------------------------------------------------------------------
    bool setLocalString(const char* variableName, const char* value) {
        ConsoleValue* stack = getLocalVariable(variableName);
        if (stack == nullptr) return false;
        stack->setString(value);
        return true;
    }
    //-----------------------------------------------------------------------------
    F64 getLocalFloat(const char* variableName) {
        ConsoleValue* stack = getLocalVariable(variableName);
        if (stack == nullptr) return 0.f;
        return stack->getFloat();
    }
    //-----------------------------------------------------------------------------
    S64 getLocalInt(const char* variableName) {
        ConsoleValue* stack = getLocalVariable(variableName);
        if (stack == nullptr) return 0;
        return stack->getInt();
    }
    //-----------------------------------------------------------------------------
    const char* getLocalString(const char* variableName) {
        ConsoleValue* stack = getLocalVariable(variableName);
        if (stack == nullptr) return "";
        return stack->getString();
    }

    //-----------------------------------------------------------------------------
    #ifdef ENABLE_CONSOLE_VECTOR
    ConsoleVector getLocalVector(const char* variableName) {
        ConsoleValue* stack = getLocalVariable(variableName);
        if (stack == nullptr) return ConsoleVector{0};
        return stack->getVector();
    }
    //-----------------------------------------------------------------------------
    bool setLocalVector(const char* variableName, ConsoleVector& value) {
        ConsoleValue* stack = getLocalVariable(variableName);
        if (stack == nullptr) return false;
        stack->setVector(value);
        return true;
    }

    #endif
    // -------------------------------------------------------------------------
    //  ~~~~~~~~~ Dump and Debug .... ~~~~~~~~~~
    // -------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    void varDumpGobals(const char* variableName)
    {
        if (!variableName) return;

        if (variableName[0] != '$') {
            Con::errorf("Sorry [%s] in not a global variable %s", variableName);
            return;
        }
        Dictionary::Entry *entry =Con::gGlobalVars.lookup(StringTable->insert(variableName));
        if (!entry) {
            Con::printf("%s not found.", variableName);
            return ;
        }

        ConsoleValue& localVal = entry->getValue();
        Con::printf(" %10s [type:%8s] [value:%20s]"
        , variableName, getConsoleValueTypeName(localVal.type), localVal.getString());

    }
    // -----------------------------------------------------------------------------
    void varDumpLocals(const char* variableName)
    {
        // sanity
        if (!variableName) return;

        if (variableName[0] != '%') {
            Con::errorf("Sorry [%s] in not a local variable %s", variableName);
            return;
        }

        S32 reg = findLocalVarRegisterInCurrentScope(variableName);

        if (reg < 0) {
            Con::printf("%s not found.", variableName);
            return ;
        }


        ConsoleValue& localVal = Script::gEvalState.currentRegisterArray->values[reg];
        Con::printf(" %10s [type:%8s] [value:%20s] [reg:%2d] "
        , variableName, getConsoleValueTypeName(localVal.type), localVal.getString(), reg);
    }
    // -----------------------------------------------------------------------------
    void varDumpDynamicField(const char* variableName) {
        ConsoleValue* cval = nullptr;
        cval = Con::getObjectDynamicFieldConsoleValue(variableName);
        if (cval) {
            Con::printf(" %20s [type:%8s] [value:%20s]"
            , variableName, getConsoleValueTypeName(cval->type), cval->getString());
        }

    }
    // -----------------------------------------------------------------------------
    void dumpAllGlobalVariables() {
        Dictionary::HashTableData* hashTable = Con::gGlobalVars.hashTable;
        if (!hashTable) return;
        Con::printf("       ------------------- Global Variables -------------------");

        for (S32 i = 0; i < hashTable->size; i++)
        {
            Dictionary::Entry *walk = hashTable->data[i];
            while (walk)
            {
                ConsoleValue& localVal = walk->getValue();
                Con::printf(" %30s [type:%8s] [value:%20s]"
                , walk->name, getConsoleValueTypeName(localVal.type), localVal.getString());

                walk = walk->nextEntry;
            }
        }
    }
    // -----------------------------------------------------------------------------
    void dumpAllLocalVariables() {
        // 1. gEvalFuncVars
        Con::printSeparator();
        Con::printf("       ------------------- GlobalScope -------------------");
        _getFuncVars()->listExising();

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
    void varDump(const char* variableName) {
        if ( !variableName || variableName[0] == '\0') return;
        if ( variableName[0] == '%') ElfScript::varDumpLocals(variableName);
        else if ( variableName[0] == '$') ElfScript::varDumpGobals(variableName);
    }

} //namespace ElfScript
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
DefineEngineFunction(varDumpField, void, (const char* variableName), , "local/global variable dump. local only valid in the variables scope")
{
    if ( !variableName || variableName[0] == '\0') return;
    ElfScript::varDumpDynamicField(variableName);
}

DefineEngineFunction(varDump, void, (const char* variableName), , "local/global variable dump. local only valid in the variables scope")
{
    ElfScript::varDump(variableName);
    // if ( !variableName || variableName[0] == '\0') return;
    // if ( variableName[0] == '%') ElfScript::varDumpLocals(variableName);
    // else if ( variableName[0] == '$') ElfScript::varDumpGobals(variableName);


}
// -----------------------------------------------------------------------------
DefineEngineFunction(dumpLocals, void, (),,"dump local and global  variables") {
    ElfScript:: dumpAllLocalVariables();
    ElfScript:: dumpAllGlobalVariables();
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

}
// -----------------------------------------------------------------------------
// see also toObject (previous toArray)
DefineEngineFunction(explodeGlobal,S32, (const char* varName, bool debugOut),(false),
                     "WARNING works only with Global Variable '$' !!!!!!\n"
                     "Convert an string content of varname to an set typed of variables with varName[0..count] fields\n"
                     "tab separated (default) or space separated"
                     "@return the count of variables"
) {

    const char* text = ElfScript::getLocalString(varName);

    if (!text || text[0] == '\0') {
        // Con::errorf("Variable %s is empty and cant be converted to array", varName);
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
        Con::errorf("Variable %s is empty and cant be converted to object", varName);
        return 0;
    }
    char buff[256];
    StringTableEntry fieldNameEntry = nullptr;
    for (U32 i = 0; i < count; i++) {
        const char * token = StringUnit::getUnit( text, i, set );

        dSprintf(buff,256,"%s%d",varName, i); //mhh or as array ?
        fieldNameEntry = StringTable->insert( buff );

        // // Script::gEvalState.setCurVarNameCreate(fieldNameEntry);

        ConsoleValue* stack = nullptr;
        ElfScript::CreateVar(fieldNameEntry, stack);
        if (!stack) return 0;
        if (isInt(token)) stack->setInt(dAtol(token));
        else if (isFloat(token)) stack->setFloat(dAtod(token));
        else stack->setString(token);

        if (debugOut) ElfScript::varDump(fieldNameEntry);
    }
    // clean original content:
    ElfScript::setLocalString(varName, "");



    return count;
}
// ElfScript 0.7f - see also
// FIXME can be removed and Array need a from string ?
// Added to Array fromString
// DefineEngineFunction(explodeToObject,S32, (const char* text),
//                      ,"Convert an string to Object with v[] fields\n"
//                      "tab separated (default) or space separated\n"
//                      "WARNING you need to delete the object after it's not longer needed."
// ) {
//     if (!text || text[0] == '\0') {
//         Con::errorf("Empty text cant be converted to object");
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
//         Con::errorf("Empty text cant be converted to object");
//         return 0;
//     }
//
//     SimObject* obj = new SimObject();
//     char buff[32];
//     StringTableEntry fieldNameEntry = nullptr;
//     for (U32 i = 0; i < count; i++) {
//         const char * token = StringUnit::getUnit( text, i, set );
//
//         dSprintf(buff,32,"v%d", i); //mhh or as array ?
//         fieldNameEntry = StringTable->insert( buff );
//
//         obj->setDataField(fieldNameEntry, nullptr, token);
//         // try to typeCast:
//         if (isInt(token)) obj->setDataFieldType(TypeS64, fieldNameEntry, nullptr );
//         else if (isFloat(token)) obj->setDataFieldType(TypeF64, fieldNameEntry, nullptr );
//     }
//     obj->registerObject();
//     return obj->getId();
// }
// =============================================================================
#ifdef TORQUE_DEBUG
DefineEngineFunction(TEST_VAR_CREATE,void,(),,"must be test inside and outside a function!") {
    ConsoleValue* stack = nullptr;
    Con::printSeparator();
    if (ElfScript::CreateVar("$globalVarTest", stack)) {
        stack->setString("GLOBAL: I was here !");
        ElfScript::varDump("$globalVarTest");
    } else {
    }
    if (ElfScript::CreateVar("%localVarTest", stack)) {
        stack->setString("LOCAL: I was here !");
        ElfScript::varDump("%localVarTest");
    } else {
        Con::errorf("- FAILED TO CREATE LOCAL!");
    }
    Con::printSeparator();
}

#endif
