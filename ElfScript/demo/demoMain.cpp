//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// ElfScript demo main file
//-----------------------------------------------------------------------------
#include <cstdio>
// glue it together ;)
#include "main/engineGlue.h"

#include "console/console.h"
#include "console/script.h"
#include "console/engineAPI.h"

#if defined(__unix__)
#include "addons/shellConsole/POSIXStdConsole.h"
#include <platform/platformVolume.h>
#endif

// register enum >>>>
#include "console/consoleExtras.h"
// <<<<

// for timer
#include <chrono>

// // #include "math/mMathConsoleVector.h"


enum MyEnum {
    None = 0,
    One,
    Two,
    Three
};

bool gShutDownRequest = false;
U64 gFrameTime = 0;

String gScriptFile = "ElfScript/demo/test.elf";
// ----------------------------------------------------------------------------
DefineEngineFunction(getFrameTime, S32, (), , "get the time in ms the last loop did need to finish")
{
    return (S32)gFrameTime;
}

// ----------------------------------------------------------------------------
int argParser(int argc, char* argv[]) {
    Con::setIntVariable("$Main::argc", argc);
    for (S32 i = 0; i < argc; i++)
        Con::setVariable(avar("$Main::argv%d", i), argv[i]);


    gShutDownRequest = true; //default no loop!
    String argStr;
    // argv[0] is program name
    for (int i = 1; i < argc; ++i) {
        if (!argv[i]) continue;
        argStr = argv[i];

        if (argStr.equal("--loop")) {
            gShutDownRequest = false;
            continue;
        }

        // filename test
        if (argStr.equal("--script")) {
            if (i + 1 < argc) {
                gScriptFile = argv[++i];
                Con::infof("Script File: %s", gScriptFile.c_str());
            } else {
                Con::errorf("--script but no file parameter usage: --script myFile.elf");
                return 1;
            }
            continue;
        }

    } //for ...
    return 0;
}
// ----------------------------------------------------------------------------
void MyLogger(U32 level, const char *consoleLine) {
#if defined(__unix__)
    // we use console
    if (!stdConsole || !stdConsole->isEnabled()) dPrintf("%s\n", consoleLine);
#else
    switch (level) {
        case 1: dPrintf("[warn] %s\n",  consoleLine); break;
        case 2: dPrintf("[error] %s\n",  consoleLine); break;
        default: dPrintf("%s\n",  consoleLine); break;
    }
#endif
}

struct StrTesto {

    union
    {
        struct
        {
            F64   f;
            S64   i;
            char* s;
        } val;                // 8 Byte

        struct
        {
            void* dataPtr;
            EnumTable* enumTable;
        } ptrval;
    };
};

class ConValue2 {

union
{
    F64   f;
    S64   i;
    char* s;
    void* dataPtr;
};
 S32 type;
 char padding[4];
};

struct alignas(16) ConsoleValueTest {
      union {
            F64 f;
            S64 i;
            void* ptr;
      } val;                  // 8 Byte

      S32 type;               // 4 byte
      unsigned char flags;    // 1 byte
      char padding[3];        // 4 byte
};

DefineEngineFunction(testo, F32, (), ,"test for anything i like ;)"){
    Con::printSeparator();

    Con::printf("Size of SimObject: %d", sizeof("SimObject"));

    Con::printSeparator();
    Con::printf("test size: %d cur size: %d curWithoutVec %d",
                sizeof(ConsoleValueTest),
                sizeof(ConsoleValue),
                sizeof(ConValue2));
    Con::printf("console vector: %d  Test.val: %d", sizeof(ConsoleVector), sizeof(ConsoleValueTest::val));
    Con::printf("size of char *: %d, void*: %d", sizeof(char*),sizeof(void*));
    Con::printf("size of StrTesto: %d, va/ptrval: %d/%d", sizeof(StrTesto),sizeof(StrTesto::val), sizeof(StrTesto::ptrval));
    Con::printSeparator();

    return 0.f;
}
// ----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    printf("Startup ....\n");


    engineGlue::init(MyLogger);
    int ret = argParser(argc, argv);
    if (ret != 0) return ret;

    Con::addVariable("ShutDownRequest", TypeBool, &gShutDownRequest, "");

// moved to addons/shellConsole
#if defined(__unix__)
    // console test:

    StdConsole::create();
    stdConsole->enable(!gShutDownRequest);
    stdConsole->enableInput(!gShutDownRequest);
#endif


    // register enum Test >>
    Con::registerEnumS32<MyEnum>("$MyEnum::", true);
    Con::setScriptConstant("_LEFT_", 1); //real constant using preprocessor
    Con::setScriptConstant("_RIGHT_", 2); //real constant using preprocessor
    // <<<<<


    // filesystem not implemented   Con::setLogMode(0);
    std::string code= R"(
        echo("EnumTest ..................");
        echo($MyEnum::None SPC $MyEnum::One SPC $MyEnum::Two);
        echo("LEFT/RIGHT" SPC _LEFT_ SPC _RIGHT_);
        echo("......................");
        helloWorld("tom");
        error("This is NOT a error!");
        warn("This is NOT a warning!");
        echo( 5 + 5 );

        function FOO::bar(%this) {
            echo(%this.getClassName());
        }
        $foo = new ScriptObject() { class = "FOO"; };
        $foo.userValue = 4711;
        $foo.bar();

        echo("mSin(3.14) =" SPC mSin(3.14));

        schedule(0, 0, "echo", "hello scheduler");

        // -------------------------
        // ... overwrite quit ...
        function quit() {
            $ShutDownRequest = true;
        }
        // -------------------------
        echo("---------------------------------");
        $foo = new ScriptObject();
        $foo.point = {11, 666};
        echo("POINT #1:" SPC $foo.point.x SPC $foo.point.y); // 11 666
        $foo.point.y -=  655; //<<< this is ok
        echo("POINT #2:" SPC $foo.point.x SPC $foo.point.y); // 11 11

        $foo.p[1] = {11, 666};
        echo("fake point #1:" SPC $foo.p[1].x SPC $foo.p[1].y); // 11 666
        $foo.p[1].y -= 655; //<<< fail!!!
        echo("fake point #2:" SPC $foo.p[1].x SPC $foo.p[1].y); // 11 666 !

        $bar[1] = {11, 666};
        $bar[1].y -= 655;
        echo("bar fake var:" SPC $bar[1].x SPC $bar[1].y); // 11 11 !


        $foo.p2 = "11 666";
        echo("fake point #1:" SPC $foo.p2.x SPC $foo.p2.y); // 11 666
        $foo.p2.y -= 655; //<<< fail!!!
        echo("fake point #2:" SPC $foo.p2.x SPC $foo.p2.y); // 11 666 !


    )";
    // if (gShutDownRequest) Con::evaluate(code.c_str(), false, "");


    if (!gScriptFile.isEmpty())   Con::executeFile(gScriptFile);

    bool doMainLoop = Con::isFunction("MainLoop");
    auto start = std::chrono::steady_clock::now();

    while (!gShutDownRequest) {
          // --------- advance time for scheduler this should be placed in the main loop
          engineGlue::process(gFrameTime);

          #if defined(__unix__)
          stdConsole->process();
          #endif

          if (doMainLoop) Con::executef("MainLoop");

          Platform::sleep(16);

          auto end = std::chrono::steady_clock::now();

          auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
          gFrameTime = duration.count();
          start = end;

    }

    // -------- finallize
    engineGlue::shutDown();

    return 0;
}
