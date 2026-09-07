# ElfScript

***~~Probably~~ The [fasted](docu/BetterSpeedTest.md) TorqueScript on Earth ;)*** 

Easy to embed fast scripting language with a amazing - hot reload - and - 
modify while running - capabilities. 

Since Version 0.6 i'am very close to the speed of intepreted lua (not jit). So
it's one of the fastest easy to bind interpreted Script-Language available. 

Don't miss my [Why?](docu/why.md) Story where I explain why I work on this project. 

Started writing a 📔[HandBook for ElfScript Scripting](./handbook/Main.md)

## Folder: ElfScript

The folder where the beast lives: 

- [ElfScript](./ElfScript/)

## Folder: BaseElf
![ScreenShots](./BaseElf/ScreenShotsCombined.png)

Located in Folder [BaseElf](./BaseElf): 

A neat basic Game Engine using [BaseFlux](https://github.com/ohmtal/BaseFlux/) as 
base for SDL3/ImGui/ResourceManager and ElfScript. It's also an enhanced example 
how to embed ElfScript.

This is a nice place to learn ElfScript (aka TorqueScipt ). 

## Folder: CrazyElf

A neat SDL3-ElfScript implemtation based on my [SDL3 addon](./ElfScript/addons/SDL3/). 
SDL3 Bindings are close to the C function calls except Audio is wrapped and PollEvents is in C++ Code only.

Located in Folder [CrazyElf](./CrazyElf): 


## Folder: obsolete 

- TorqueScript : My first working Version. I will not change this anymore. I work on the code in the ElfScript folder.
- experimental : Unfinished non functional attempt to make it much smaller. 

## Example / TestBed Application using OhmFlux:

- ~~added math (using also Ohmflux functions)~~
- added Platform functions (not complete)
- added some classes to test Sprite/Texture/Label/Font/Audio instance ....

[Ohmflux ElfTest](https://github.com/ohmtal/OhmFlux/tree/main/ElfTest)

## Raylib Bindings (raylib-elfscript):

- Using the raylib commands but in three Main-Callbacks:
    - function MainInit() { return true;}
    - function MainUpdate() {}
    - function MainShutDown() {}

[raylib-elfscript](https://github.com/ohmtal/raylib-elfscript)


### Summery ElfScript 0.6 vs TorqueScript

- Static Float Field:    37 times faster
- Dynamic Float Field:   25 times faster
- Float Component:       19 times faster 
- Local Float Variable:  14 times faster 
- Global Float Variable:  8 times faster 


## Notable changes:

Based on the Torque3D (4.x) source code this is my version of TorqueScript without Torque3D. 

- **ElfScript 0.7:** added an Array object which can be used in script with this constuctor: `%arr = [ 1 , 2 ,3 ];`. It's methods names are loosly follow the naming of std::vector methods.
- **ElfScript 0.7:** lexer/vm direct implemented math.* (math.randomf/math.sin/...) and print/printf/sprintf commands 
- **ElfScript 0.7:** lexer is not longer case sensitive so you can use: for (%i IN Range 10) instead of everthing is lowercase
- **ElfScript 0.6:** altered ConsoleValue. it's now  16 byte or 32 byte if ConsoleVector is enabled (on TODO list). Allocated Strings are removed for good.
- **ElfScript 0.6:** some often used SimObject methods are moved to inline (good performance boost)
- **ElfScript 0.6:** Inline Field cache.
- **ElfScript 0.6:** assign operators (--,+=,*=,/=,-=) with fast direct register writing like ++ was
- 🚀 **ElfScript:** Foreach for integers: `foreach(%i in 1..3)` iterate from 1 to 3 (including the 1 and the 3 like pascal `for 1 to 3`), also added `foreach(%i in range 1..3)` iterate from 1..2. Also added same as for. [Operators](https://github.com/ohmtal/ElfScript/blob/main/handbook/Operators.md)
- 🚀 **ElfScript:** Byte Code handling replaced the for/case monster in compiledEval with **direct threading**.
- 🚀 **ElfScript:** Dynamic Fields can be int, float or string which give a very good performace boost. Also if you setup them with TypeF32 for example. The type is really set to float and not only cosmetic. 
- 🚀 **ElfScript:** Added fastpath for static float fields setDataField which is 28 times faster than before. (ELFSCRIPT_FASTPATH_FLD)
- 🚀 **ElfScript:** Variable FastPath: OP_LOADVAR_STR and OP_LOAD_LOCAL_VAR_STR lookup the variable type to return not always string which make the previous setting to float/int useless. 
- 🚀 **ElfScript (V0.4c):** Second rocket stage ignited: OP_SAVE_LOCAL_VAR_STR and OP_SAVEVAR_STR. When i started with ElfScript (V0.0) the starfield demo in raylib-elfscript perfomed at 60fps. I optimzed the script as good as possible and got 75 fps. In Version 0.3 it was raised to about 500fps. After this last rocket change - which was quite easy - it's now raised from 550 to 900 fps (you should know i work on a 9 years old thinkpad with intel gpu) in BaseElf Starfield - did not check raylib so far. Holy cow ! 
- 🌶️ **ElfScript:** Console Vector: **Foo.MyVector = { 50.1, 78.5 };**. Foo.MyVector= { $a * 5, $b * 4}; Is stored in the console value and used when you access it over component system like Foo.MyVector.x. 
- 🚀 **ElfScript:** Added #define with code preprocessor for byte code fast constant handling (ELFSCRIPT_PREPROCESSOR)
- 🤘 Added **ImGui** bindings to [ElfScript](https://github.com/ohmtal/ElfScript/tree/main/ElfScript/addons/ImGui). Demo: [BaseElf](./BaseElf)
- 🤘 Added **SDL3** Input (keyboard/mouse) handling and binding with events and polling to [ElfScript](https://github.com/ohmtal/ElfScript/tree/main/ElfScript/addons/SDL3). Demo: [BaseElf](./BaseElf)
- 😍 **ElfScript:** Added some handy console functions but this is my favorite: formatString(string format, ...) where you are able to add up to 31 parameter to really format a string :)
- **ElfScript:** Added Con::ConsoleDocForStub default false to make the classes/function dumps better human readable but kept the code when it's exported for an parser.
- **ElfScript:** Added IMPLEMENT_ENGINE_TYPE_TRAITS for non PoD console types (C linkage incompatible warning)
- ~~**ElfScript:** ELFSCRIPT_STRICT_SLOT_TYPE saving the name to type in the VM - not sure it this is faster or slower with the lookup overhead. **Not recommended** because it save the field name and then all fields with the same name are casted to the type of the first field it found.~~
- Made it standalone
- ~~Added optional GarabageCollectionSet (ELFSCRIPT_GARBAGECOLLECTION) **not recommended** it slowdown the delete process when using lots of objects~~
- ~~Experimental: **Really not recommended:**  ELFSCRIPT_CALLFUNC_CACHED a attempt to speed up but ended up in instable calls.~~
- EngineGlue for init/process/shutdown
- Ripped out some stuff i dont need like Taml
- Fixed some memory leaks :)
- Added auto enum binding as constants
- Added new Log functions
- Fixed Emscripten and Android Build  (Android problem: file loading does not work in apk - i added overwrite exec - example in OhmFlux/ElfTest)
- Replaced Math with Light Version (based on Torque2D) since the Types like Point3F are removed
- Since i also removed nearly all platform code - network is not implemented, because i need to add threading/mutex again in order to get it working. 



## Windows 
Just tested BaseElf on Windows. All Projects using ElfScript needs:

```
# Windows - winVolume :: need to set on all builds 
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/Zc:wchar_t-")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        NOMINMAX 
        UNICODE 
        _UNICODE
    )
endif()
```

And if you want the binary in the base directory:

```
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"

    # -- for windows: 
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}"
)
```

## Syntax examples:

```
// Hello World example:
echo("Hello World");

// Variables:
$value = 5; //global Variable
%value = 5; //local Variable - inside function

// Objects:
$fooObj = new SimObject(Foo) {
    TypeF32 myValue = 1.0; // dynamic field can be defined in script 
    class = "FooClass"; // define a class name which can be used by different objects 
}; 
echo(Foo.myValue); // gives 1.0
echo($fooObj.myValue); // gives 1.0
function FooClass::print(%this) { // adding a custom method 
    // %this is a local variable which holds the SimObject-ID    
    echo(%this.myValue);
}
$fooObj.print();
echo($fooObj SPC Foo.getId()); //print SPC (space separated) object id of the foo object 
// You can also add a new dynamic field with assigning a value:
$fooObj.name = "Tom"; //bad idea overwrites object name
$fooObj.playerName = "Tom"; //a fields which is not defined by engine
echo(Tom.playerName);  // since i renamed it with .name= Foo is gone and Tom is here ;) 
$fooObj.dumpFields(); //list all fields of the object
$fooObj.dump(); //list all fields and methods of the object 
```


## Script related links

- [Torque3D](https://github.com/TorqueGameEngines/Torque3D)
- [KorkScript embeded TorqueScript](https://github.com/jamesu/korkscript/)
- [OGE3D enhanced Torque3D 3.x](https://github.com/ohmtal/OGE3D)
