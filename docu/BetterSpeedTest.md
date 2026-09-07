# ElfScript Simple Speed Test:


## Goal: get the (close to or better) speed as lua (vanilla - without jit) on "localvar test"
- Lua 5.5.1: 1.243u 0.002s 0:01.25 99.2%     0+0k 0+0io 0pf+0w
- Elfscript 0.7c very close : **1.354u** 0.002s 0:01.36 99.2%     0+0k 0+0io 0pf+0w
- Elfscript 0.6f close to lua but still about 200ms:  **1.434u** 0.006s 0:01.45 98.6%     0+0k 0+0io 0pf+0w
- Elfscript 0.6f outperform PHP 8: 2.242u 0.013s 0:02.31 97.4%     0+0k 18792+0io 97pf+0w
- ElfScript 0.4 outperform python 3: 15.768u 0.005s 0:15.83 99.5%    0+0k 0+0io 0pf+0w. 
- ElfScript 0.? outperform OGE3D (my Torque3D based on 3.10) : 33.268u 0.299s 0:33.61 99.8%  0+0k 0+24io 0pf+0w


## Version 0.7g - stability tests for first release - 
 - Added Array object. A flat array holding ConsoleValues - This is now my universal
 Swiss Army Knife :D ! I added a constructor in lexer to do `%foo = [1,2,3];` with the 
 limitation: I can not add ConsoleVector's in this constructor. But `%foo.push_back({1,2,3,4});` 
 works fine. I removed my new explodeToObject function and added fromString method to Array which makes more 
 sense. With this Object I was able to implement bindings like SDL_RenderPoints without fallback
 to String - which make the call useless. This is now a easy to use alternaive to PointStorageObject.
 While PointStorageObject is still a very fast way to work with a lot of Points/Rects/Colors .. everything
 requires a lot of 2..4 float/integer values. 
 - Implemented direct handling getter/setter for ConsoleValue (used by ValueVector)
 - Added **Neural Network** bindings using genann - demo added to CrazyElf.
 - SDL3 got a **"melody maker"** - demo added to CrazyElf.
 - SDL3 got some more bindings like Clipboard text handling. 
 - Added some methods to Simset/Group to make it more like an "std::"Vector object 
 - **Set Fieldcache default off** - marked as experimental again! When working on scripts I 
 had a strange behavoiur - cache failed and dynamic  fields of just created objects  where 
 not written / could not be read. So it's fast, but not production ready. 
 Unforually i can not reproduce it in a test enviroment, so far. This is also bad
 for Components speed since it's embedded into the FieldCache - but not effected.
 Maybe i put it in parts so i can enable Components cache without object field 
 cache. **2026-09-04:** I think i fixed this, skiping objects without ID (new Object ...)
 on save. 
 - Fixed math inline commands to use getFloat instead of getFastFloat. 
 - Added inline commands to docu generator (stub file)
 - SDL3 Colors (baseElf) changed to ConsoleVector
 - Short if: Vector (PoD) parser: true ? {0} : {0} || true ? {0} : expr || true ? expr : {0}
 - MiniElf Demo - showcase how to embed it - without anything else. Lib failed - need some
 research why. 
 
## Version 0.7f

 - added ConsoleVector to union << 24 byte instead of 32
 - print and printf do not need to pop.
 - ELF_ENABLE_FIELDCACHE default ON again 
 - function toObject => `%obj = toObject({"Hello", "World", 10.0});` create an object
 with fields TypeString v0 = "Hello", TypeString v1="World", TypeF64 v2= 10.0;
 - function toArray => `$foo = {"Hello", "World", 10.0}; echo(toArray("$foo"));`
 clear $foo, create variables TypeString $foo0 = "Hello", TypeString $foo1="World", TypeF64 $foo2= 10.0;
 and return the count of variables;
 - Both new function also works with a string like: "Hello World 10.0";
 - Auto set type on Dynamic Fields:
    - `$foo = {1,2,3}; %obj.foo = $foo;vardumpField("%obj.foo");` set an TypeVector
    - `%obj.addField("myFloat", "TypeF32", 0); %obj.myFloat[0]= 4;` myFloat[0] copy the type from myFloat. 
 
## Version 0.7e

- Fieldcache cleaned and fixed but still default off in cmake.

***Results with FieldCache on -Ofast***
| Script | time |
| --- | --- |
| test_localvar.elf     | 1.388u 0.003s 0:01.39 99.2%     0+0k 0+0io 0pf+0w |
| test_global.elf       | 3.004u 0.001s 0:03.01 99.6%     0+0k 0+0io 0pf+0w |
| test_static.elf       | 4.499u 0.004s 0:04.51 99.5%     0+0k 0+0io 0pf+0w |
| test_dynamic.elf      | 3.844u 0.006s 0:03.86 99.4%     0+0k 0+0io 0pf+0w |
| test_vector_components.elf    | 4.100u 0.003s 0:04.11 99.7%     0+0k 0+0io 0pf+0w |

***Results with FieldCache on***
| Script | time |
| --- | --- |
| test_localvar.elf     | 1.412u 0.003s 0:01.42 99.2%     0+0k 0+0io 0pf+0w |
| test_global.elf       | 2.975u 0.003s 0:02.98 99.6%     0+0k 0+0io 0pf+0w |
| test_static.elf       | 4.621u 0.002s 0:04.63 99.7%     0+0k 0+0io 0pf+0w |
| test_dynamic.elf      | 4.007u 0.004s 0:04.02 99.5%     0+0k 0+0io 0pf+0w |
| test_vector_components.elf    | 4.135u 0.002s 0:04.14 99.7%     0+0k 0+0io 0pf+0w |


***Results with FieldCache off***
| Script | time |
| --- | --- |
| test_localvar.elf     | 1.420u 0.003s 0:01.43 99.3%     0+0k 0+0io 0pf+0w |
| test_global.elf       | 2.960u 0.007s 0:02.99 98.9%     0+0k 8+0io 0pf+0w |
| test_static.elf       | 9.044u 0.001s 0:09.07 99.6%     0+0k 0+0io 0pf+0w |
| test_dynamic.elf      | 11.402u 0.004s 0:11.44 99.6%    0+0k 0+0io 0pf+0w |
| test_vector_components.elf    | 6.108u 0.007s 0:06.14 99.3%     0+0k 0+0io 0pf+0w |

## Version 0.7 d

- [X] SDL3 added EngineUnmarshallData< Point2F >
- [X] SDL3 added  EngineUnmarshallData< RectF >
- [X] Vector Component Fieldcache disabled again: AddressSanitizer: heap-use-after-free
popFrame/pushFrame ... make my cache invalid!! ....added a S32 frameID no idea the cache
is worth to live there !! 

## Version 0.7 c
- [X] Let's see if I can speedup Vector Componets as Fieldcache:
   - only for local and global vars 
   - ok, not as fast as expected, but faster as without =>  3.945u instead of 5.411u
- [X] fixme stack overflow when a new command is used but not consumed!! << added OP_POP_STK

| Script | time |
| --- | --- |
| test_localvar.elf     | 1.354u 0.002s 0:01.36 99.2%     0+0k 0+0io 0pf+0w |
| test_global.elf       | 3.023u 0.004s 0:03.03 99.6%     0+0k 0+0io 0pf+0w |
| test_static.elf       | 4.196u 0.002s 0:04.21 99.5%     0+0k 0+0io 0pf+0w |
| test_dynamic.elf      | 3.923u 0.001s 0:03.93 99.7%     0+0k 0+0io 0pf+0w |
| test_vector_components.elf    | 3.837u 0.005s 0:03.85 99.4%     0+0k 0+0io 0pf+0w |

- localvar is also faster  - nice but not sure why.
- Components are much faster.
- Fields are slightly slower because of my change to virtual overwrite custom types
. This is ok because it speed up in real world applciation. 
    
## Version 0.7 b
- Added [virtual functions](./FastPathCustomFieldTypes.md) to overwrite static fields fastPath 
- Got [ConsoleVector directly accessed](./ImplementConsoleTypeCasters.md) via Console-Functions and -Methods without string convert! - Rock 'n Roll!! 
- this means ConsoleVector is completely implemented as a "nativ type" and i will not remove it from ConsoleValue because of 16 bytes! It's 32 bytes and disable it does not change anything in speed. 
- too bad i dont have a benchmark to test this ... but my ConsoleVector Math functions using reference can be 
changed now!

## Version 0.7 a 

TestResult (best of 3 tests) in /docu/speedtest/elf_v6:

| Script | time |
| --- | --- |
| test_localvar.elf     | 1.418u 0.005s 0:01.42 99.2%     0+0k 0+0io 0pf+0w |
| test_global.elf       | 2.930u 0.002s 0:02.94 99.6%     0+0k 0+0io 0pf+0w |
| test_static.elf       | 4.017u 0.002s 0:04.03 99.5%     0+0k 0+0io 0pf+0w |
| test_dynamic.elf      | 3.804u 0.004s 0:03.81 99.7%     0+0k 0+0io 0pf+0w |
| test_vector_components.elf    | 5.411u 0.002s 0:05.42 99.8%     0+0k 0+0io 0pf+0w |

---

With IPO / -O3 optimized compiled 
| Script | time |
| --- | --- |
| test_localvar.elf     | 1.365u 0.003s 0:01.37 99.2%     0+0k 0+0io 0pf+0w   |
| test_global.elf       | 3.487u 0.006s 0:03.50 99.4%     0+0k 0+0io 0pf+0w  |
| test_static.elf       | 4.629u 0.002s 0:04.64 99.5%     0+0k 0+0io 0pf+0w |
| test_dynamic.elf      | 4.118u 0.002s 0:04.13 99.5%     0+0k 0+0io 0pf+0w |
| test_vector_components.elf    | 5.171u 0.004s 0:05.20 99.4%     0+0k 0+0io 0pf+0w |

Localvar and Vector Componets is faster but global/static/dynamic slower!

---

With -0fast compiled:

| Script | time |
| --- | --- |
| test_localvar.elf     | 1.410u 0.002s 0:01.41 100.0%    0+0k 0+0io 0pf+0w |
| test_global.elf       | 2.947u 0.003s 0:02.96 99.3%     0+0k 0+0io 0pf+0w |
| test_static.elf       | 3.908u 0.004s 0:03.92 99.4%     0+0k 0+0io 0pf+0w |
| test_dynamic.elf      | 3.784u 0.004s 0:03.80 99.4%     0+0k 0+0io 0pf+0w |
| test_vector_components.elf    | 5.699u 0.006s 0:05.73 99.3%     0+0k 0+0io 0pf+0w |

Best results with this only components is slightly slower then -O2 (first one). 
But i would prefer -O2. 

---




**NOTE** Vector Component and Static Field are float. Math.randomF is double. The 
ConsoleValue is also double. When float/double is converted there are rounding errors.

- [X] special OP_CODES for "for in in range VALUE" -- seams not to be faster in -O2
    - IPO active: 1.354u 0.004s 0:01.36 99.2%     0+0k 0+0io 0pf+0w
    - IPO active std out to /dev/null: 1.332u 0.003s 0:01.33 100.0%    0+0k 0+0io 0pf+0w
    - Release -O2: 1.411u 0.005s 0:01.42 99.2%     0+0k 0+0io 0pf+0w
    - while this test is faster with IPO other are slower than -O2
    - so i can say it's not worth to create extra OP_CODES for this, an switch case at runtime is fast enough

- [x] default functions to Bytecode: - lot of work for nearly no speed enhancements 

    Test: is it worth ? 
    - static field test with getRandomF(%i)       : 4.550u 0.002s 0:04.56 99.7%     0+0k 0+0io 0pf+0w 
    - static field test with math.randomf(%i)    : 4.039u 0.004s 0:04.05 99.5%     0+0k 0+0io 0pf+0w 
    - static field test with math.randomf() * %i : 4.080u 0.005s 0:04.10 99.5%     0+0k 0+0io 0pf+0w
    - !!! * %i results in rounding errors !!!

    YES! but using only 3 op codes: OP_INLINE_COMMAND_3P, OP_INLINE_COMMAND_2P and OP_INLINE_COMMAND_1P

    - [x] floor     
    - [x] ceil      
    - [x] fabs       
    - [x] fmod      
    - [x] sin       
    - [x] cos       
    - [x] atan      
    - [x] tanh      
    - [x] sqrt      
    - [x] isZero    
    - [x] min       
    - [x] max       
    - [x] atan     
    - [x] clamp     
    - [x] lerp      
    - [x] smoothstep 
    - [x] pow       
   
    
  

- [X] Add a new Benchmark file and update the tests with the new functions.

## Version 0.6f :
- [~] ConsoleVector to pointer - but it's not faster when i remove it ... mhhh 
    - setup pool for Vectors
    - set/get || clean ??
    
    NOTE:   i did this (unfinished) - was a mess. But the biggest issue is it 
            did not speepup the script!!!!! Reverted but the unused ENumTable is
            *disabled*
- [X] So the change is i modified console value data and it seams to give a small kick
  
- [X] removed int math operation from compiler again, did not work as expected.  
  
- [X] Testing
    - [X] HelloElf Tests. 
    - [X] CrazyELF
    - [X] BaseElf
    - [X] raylib-elfScript
    - [X] ElfFlux
    - [X] OhmFlux/ElfTest << old script does not work so well with fieldcache
- [X] Tag 
    - [X] git tag -a 0.6 -m "ElfScript 0.6"; git push --tags
    

## Version 0.6g:
- [X] OP_INLINE_COMMAND (first attempt)
    - randomf
    - print
    - => 1.853u 0.005s 0:01.86 99.4%     0+0k 0+0io 0pf+0w
    
- [X] Bytecode Commands:
    - [X] OP_PRINT
    - [X] OP_MATH_RANDOMF
    - => IP0 1.677u 0.003s 0:01.68 99.4%     0+0k 0+0io 0pf+0w
    - [X] OP_MATH_RANDOMF_1
    - [X] OP_MATH_RANDOMF_2
    - => IP0 1.746u 0.002s 0:01.75 99.4%     0+0k 0+0io 0pf+0w
    - => 1.725u 0.002s 0:01.73 99.4%     0+0k 0+0io 0pf+0w - i lost 100ms again ?!
    
- [X] Activated UINT Math
    - => 1.804u 0.004s 0:01.82 98.9%     0+0k 0+0io 0pf+0w ??? ok .. 

- [X] Short path local var:  *= , += , -= , /= ....
    - [X] *= 
    - [X] += 
    - [X] -= 
    - [X] /=
    - => 1.450u 0.004s 0:01.46 99.3%     0+0k 0+0io 0pf+0w
    - after 0.6f ConsoleValue did not change anything i guess i'am on the end 
      of what i can do. No more ideas at the moment.




## Version 0.6f : 
- splitted the foreach / for range OP codes but i had it fully optimized in 0.6d

## Version 0.6e : 
- removed cvString and only using cvSTEntry

## Version 0.6d : 
- revisit and optimize foreach/for range

### Summery ElfScript 0.6d vs TorqueScript

- Static Float Field:    37 times faster
- Dynamic Float Field:   25 times faster
- Float Component:       19 times faster 
- Local Float Variable:  14 times faster 
- Global Float Variable:  8 times faster 


## Version 0.6c : 
- revisited inline cache. removed components from cache. 
- fastpath setDataField/getDataField/fillFieldCache moved inline to header << over a second faster! 
    - The lesson I learned sometimes setting code to inline can change a lot.

## Version 0.6b : 
- restored StringTable - there is a bug in the new implemenation, i saved it to 
unstable folder
- ENABLE_INLINE_CACHE_LOAD/ENABLE_INLINE_CACHE_SAVE enabled again but disabled
Componentfields

## Version 0.6a : 
- rewrote StringTable but did not really speed up
- added initial inline field cache ... thats an rocket in loops but need more cleanup and testing
- with #define ENABLE_INLINE_CACHE but unstable on real test so ifdef'd!!! 

### docu/speedtest/elf/test_localvar_v6.elf
- 0.6g : 1.447u 0.005s 0:01.46 98.6%     0+0k 0+0io 0pf+0w
- 0.6f : 1.434u 0.006s 0:01.45 98.6%     0+0k 0+0io 0pf+0w
- 0.7a : 1.420u 0.001s 0:01.42 100.0%    0+0k 0+0io 0pf+0w

### docu/speedtest/elf/test_localvar_foreach.elf
- OGE3D* (current 26-08-05) : 33.268u 0.299s 0:33.61 99.8%  0+0k 0+24io 0pf+0w
- 0.6a ENABLE_INLINE_CACHE : 2.855u 0.003s 0:02.87 99.3%     0+0k 0+0io 0pf+0w
- 0.6a : 2.857u 0.004s 0:02.87 99.3%     0+0k 0+0io 0pf+0w
- 0.6b : 2.892u 0.007s 0:02.91 99.3%     0+0k 0+0io 0pf+0w
- 0.6c : 2.754u 0.002s 0:02.76 99.6%     0+0k 0+0io 0pf+0w 
- 0.6e : 2.244u 0.004s 0:02.25 99.5%     0+0k 0+0io 0pf+0w
- 0.6f : 2.107u 0.004s 0:02.12 99.0%     0+0k 0+0io 0pf+0w
- 0.6g : 1.916u 0.003s 0:01.93 98.9%     0+0k 0+0io 0pf+0w
- 0.6f : 1.831u 0.005s 0:01.84 99.4%     0+0k 0+0io 0pf+0w
- 0.7a : 1.797u 0.003s 0:01.80 99.4%     0+0k 0+0io 0pf+0w

### docu/speedtest/elf/test_globalvar_forrange.elf
- OGE3D* (current 26-08-05) : 34.477u 0.299s 0:34.82 99.8%  0+0k 0+56io 0pf+0w
- 0.6a ENABLE_INLINE_CACHE : 4.513u 0.002s 0:04.53 99.5%     0+0k 0+0io 0pf+0w
- 0.6a : 4.571u 0.003s 0:04.59 99.5%     0+0k 0+0io 0pf+0w
- 0.6b : 4.479u 0.002s 0:04.49 99.5%     0+0k 0+0io 0pf+0w
- 0.6c : 4.712u 0.006s 0:04.74 99.3%     0+0k 0+0io 0pf+0w
- 0.6e : 3.450u 0.005s 0:03.47 99.4%     0+0k 0+0io 0pf+0w
- 0.6f : 3.342u 0.004s 0:03.36 99.4%     0+0k 0+0io 0pf+0w
- 0.6g : 3.327u 0.002s 0:03.35 99.1%     0+0k 0+0io 0pf+0w
- 0.6f : 3.366u 0.003s 0:03.38 99.4%     0+0k 0+0io 0pf+0w
- 0.7a : 3.331u 0.005s 0:03.34 99.7%     0+0k 0+0io 0pf+0w

### docu/speedtest/elf/test_dynamic_fields_forrange.elf

- OGE3D* using ScriptObject: 137.226u 0.296s 2:17.73 99.8% 0+0k 0+136io 0pf+0w
- 0.6a ENABLE_INLINE_CACHE: 5.571u 0.004s 0:05.60 99.4%     0+0k 0+0io 0pf+0w
- 0.6a : 14.364u 0.005s 0:14.41 99.6%    0+0k 0+0io 0pf+0w
- 0.6b : 6.268u 0.002s 0:06.29 99.5%     0+0k 0+0io 0pf+0w
- 0.6c : 5.417u 0.003s 0:05.43 99.6%     0+0k 0+0io 0pf+0w
- 0.6e : 4.441u 0.004s 0:04.47 99.3%     0+0k 0+0io 0pf+0w
- 0.6f : 4.208u 0.000s 0:04.22 99.5%     0+0k 0+0io 0pf+0w
- 0.6g : 4.254u 0.002s 0:04.27 99.5%     0+0k 0+0io 0pf+0w
- 0.6f : 4.501u 0.010s 0:04.54 99.3%     0+0k 0+0io 0pf+0w
- 0.7a : 4.121u 0.005s 0:04.14 99.5%     0+0k 0+0io 0pf+0w

### docu/speedtest/elf/test_static_fields_localvar_forrange.elf
- OGE3D* using tom2DSprite: 192.723u 0.302s 3:13.28 99.8% 0+0k 0+248io 0pf+0w
- 0.6a ENABLE_INLINE_CACHE: 6.656u 0.004s 0:06.68 99.5%     0+0k 0+0io 0pf+0w
- 0.6a : 11.753u 0.003s 0:11.79 99.6%    0+0k 0+0io 0pf+0w
- 0.6b : 6.853u 0.009s 0:06.89 99.4%     0+0k 0+0io 0pf+0w
- 0.6c : 5.274u 0.004s 0:05.29 99.6%     0+0k 0+0io 0pf+0w
- 0.6e : 4.634u 0.008s 0:04.67 99.1%     0+0k 0+0io 0pf+0w
- 0.6f : 4.500u 0.005s 0:04.52 99.5%     0+0k 0+0io 0pf+0w
- 0.6g : 4.486u 0.002s 0:04.50 99.5%     0+0k 0+0io 0pf+0w
- 0.6f : 4.718u 0.003s 0:04.73 99.5%     0+0k 0+0io 0pf+0w (slower?!)
- 0.7a : 4.510u 0.003s 0:04.52 99.7%     0+0k 0+0io 0pf+0w

### docu/speedtest/elf/test_vector_components.elf
- OGE3D* (current 26-08-05) :: 140.183u 0.322s 2:20.84 99.7% 0+0k 0+48io 0pf+0w
- 0.6a ENABLE_INLINE_CACHE: 6.815u 0.003s 0:06.84 99.5%     0+0k 0+0io 0pf+0w
- 0.6a : 7.372u 0.004s 0:07.40 99.5%     0+0k 0+0io 0pf+0w
- 0.6b : 7.985u 0.001s 0:08.01 99.6%     0+0k 0+0io 0pf+0w
- 0.6c : 7.312u 0.004s 0:07.34 99.5%     0+0k 0+0io 0pf+0w
- 0.6e : 6.169u 0.006s 0:06.19 99.5%     0+0k 0+0io 0pf+0w
- 0.6f : 6.199u 0.002s 0:06.22 99.5%     0+0k 0+0io 0pf+0w
- 0.6g : 6.434u 0.002s 0:06.45 99.6%     0+0k 0+0io 0pf+0w
- 0.6f : 6.002u 0.002s 0:06.02 99.6%     0+0k 0+0io 0pf+0w
- 0.7a : 6.097u 0.004s 0:06.12 99.5%     0+0k 0+0io 0pf+0w


### docu/speedtest/elf/test_vector_components_global.elf
- 0.6a ENABLE_INLINE_CACHE: 7.474u 0.004s 0:07.51 99.4%     0+0k 0+0io 0pf+0w
- 0.6a : 8.119u 0.005s 0:08.14 99.6%     0+0k 0+0io 0pf+0w
- 0.6b : 8.669u 0.006s 0:08.71 99.4%     0+0k 0+0io 0pf+0w
- 0.6c : 7.636u 0.005s 0:07.66 99.6%     0+0k 0+0io 0pf+0w
- 0.6e : 7.298u 0.003s 0:07.33 99.4%     0+0k 0+0io 0pf+0w
- 0.6f : 7.036u 0.005s 0:07.06 99.5%     0+0k 0+0io 0pf+0w
- 0.6g : 7.286u 0.004s 0:07.31 99.5%     0+0k 0+0io 0pf+0w
- 0.6f : 6.953u 0.003s 0:06.97 99.7%     0+0k 0+0io 0pf+0w
- 0.7a : 7.005u 0.009s 0:07.04 99.4%     0+0k 0+0io 0pf+0w


### docu/speedtest/elf/counter.elf << says nothing, just for fun ;)
- 0.6a ENABLE_INLINE_CACHE: 5.016u 0.001s 0:05.03 99.6%     0+0k 0+0io 0pf+0w
- 0.6a : 5.180u 0.003s 0:05.19 99.8%     0+0k 0+0io 0pf+0w
- 0.6b : 4.877u 0.001s 0:04.89 99.5%     0+0k 0+0io 0pf+0w
- 0.6f : 2.939u 0.003s 0:02.95 99.3%     0+0k 0+0io 0pf+0w
- 0.6g : 2.927u 0.001s 0:02.93 99.6%     0+0k 0+0io 0pf+0w
- 0.6f : 2.936u 0.003s 0:02.95 99.3%     0+0k 0+0io 0pf+0w
- 0.7a : 2.650u 0.002s 0:02.65 100.0%    0+0k 0+0io 0pf+0w


### docu/speedtest/elf/test_for.elf
- 0.6c : 2.084u 0.002s 0:02.09 99.5%     0+0k 0+0io 0pf+0w
- 0.6e : 1.374u 0.007s 0:01.39 98.5%     0+0k 0+0io 0pf+0w
- 0.6f : 1.090u 0.004s 0:01.10 99.0%     0+0k 0+0io 0pf+0w
- 0.6g : 1.318u 0.004s 0:01.34 97.7%     0+0k 0+0io 0pf+0w
- 0.6f : 1.184u 0.002s 0:01.19 99.1%     0+0k 0+0io 0pf+0w
- 0.7a : 0.947u 0.004s 0:00.95 98.9%     0+0k 0+0io 0pf+0w

### Crazy Elf: assets/modules/StarField.elf 
- 0.6a : ~ 1300 FPS (GPU: 67%) falling down to 1000 FPS (GPU: 50%) Notebook handbreak ? 
- 0.7a : ~ 1500 FPS (GPU: 76%) falling down to 1050 FPS (GPU: 55%)


OGE3D* Release Build - cant test the same scripts since it does not have the Extensions from ElfScript (for range/preprocessor/..).

---

As expected, the local variables followed by the global are the fastest. The  
static fields are many times faster than in in the vanilla torquescript. With 0.4d
it became a real rocket. Only the Dynamic Fields are lame ducs, but i started
to change this.


### Version 0.5d (RelWithDebug) : 
- 0.5d is slightly slower than 0.5c - i added step to for in range which cause a different stop statement which is safer anyway


### Version 0.5c (RelWithDebug) : 
- added foreach(%i in first..last) and foreach( %i in range start..stop) => 2.69! 

### Version 0.5a (RelWithDebug) : 
- opMinusMinus also got direct register write like opPlusPlus
- OP_SAVE_LOCAL_VAR_FLT/OP_SAVE_LOCAL_VAR_UINT speed up
- removed cleanupdata when int/float is set (good idea?) ==> No memleak!


### Version 0.4h🚀 (RelWithDebug) : Direct Threading finished ... let see - compiled with -O2.


### Testenviorment: 

- ThinkPad T570
- DE: Xfce4 4.20
- WM: Xfwm4 (X11)
- CPU: Intel(R) Core(TM) i5-7200U (4) @ 3.10 GHz
- GPU: Intel HD Graphics 620 @ 1.00 GHz [Integrated]
- Memory: 1.04 GiB / 7.49 GiB (14%)

    

***OGE3D: time ./OhmtalGame_Linux.bin -dedicated -game speedtest***


## Local variable:

- Version 0.4a (RelWithDebug) : 15.913u 0.198s 0:16.16 99.6%    0+0k 0+0io 0pf+0w
- OGE3D (current 26-08-05) : 33.268u 0.299s 0:33.61 99.8%  0+0k 0+24io 0pf+0w
- Version 0.4d 🚀 (RelWithDebug) : 5.199u 0.000s 0:05.20 99.8%     0+0k 0+0io 0pf+0w
- Version 0.4f (RelWithDebug) : 4.509u 0.003s 0:04.52 99.5%     0+0k 0+0io 0pf+0w
- Version 0.4g (RelWithDebug) : 4.753u 0.006s 0:04.77 99.5%     0+0k 0+0io 0pf+0w
- Version 0.4h (RelWithDebug) : 4.345u 0.004s 0:04.36 99.5%     0+0k 0+0io 0pf+0w
- Version 0.5a (RelWithDebug) : 
    - 3.988u 0.010s 0:04.02 99.2%     0+0k 0+0io 0pf+0w
    - 3.664u 0.002s 0:03.68 99.4%     0+0k 0+0io 0pf+0w
    - Invalid ::: 3.252u 0.004s 0:03.27 99.3%     0+0k 0+0io 0pf+0w
- Version 0.5c (using foreach range)): 2.696u 0.003s 0:02.70 99.6%     0+0k 0+0io 0pf+0w
- Version 0.5d (using for range)): 2.876u 0.002s 0:02.89 99.3%     0+0k 0+0io 0pf+0w

- Version 0.6a (for) : 3.750u 0.002s 0:03.77 99.4%     0+0k 0+0io 0pf+0w
- Version 0.6a (for range) : 2.868u 0.003s 0:02.88 99.3%     0+0k 0+0io 0pf+0w


**Mission impossible: Lua 5.5: 1.215u 0.002s 0:01.22 99.1%     0+0k 24+0io 1pf+0w**

**for OGE3D i had to replace the #define's with global variables!, 200 ms get lost at startup (console)**



```
#define JLOOPS 25
#define ILOOPS 1000000

%sum = 0;
for (%j = 0; %j < JLOOPS; %j++) {
    for (%i = 0; %i < ILOOPS; %i++) {
        %sum ++;
    }
    echo("SUM (++) IS:" SPC %sum);
    for (%i = 0; %i < ILOOPS; %i++) {
        %sum --;
    }
    echo("SUM (--) IS:" SPC %sum);
    %sum = 66;
    echo("set Sum to 66 == " SPC %sum * 1);
    for (%i = 0; %i < ILOOPS; %i++) {
        %sum *= %i + 1;
        %sum /= %i + 1;
    }
    echo("SUM (*/ %i+1) IS:" SPC %sum);
    for (%i = 0; %i < ILOOPS / 2.0; %i++) {
        %ran = getRandomF(%i);
        %sum -= %ran;
        %sum += %ran;
    }
    echo("SUM (rand +-) IS:" SPC %sum);
}
%sum -= 66.0 ;
echo("---------------------");
echo("---------------------");
echo("Final sum should be 0 == ", %sum);
echo("---------------------");
echo("---------------------");

```

## Global variable :

- Version 0.4a (RelWithDebug) :17.481u 0.182s 0:17.68 99.8%    0+0k 0+0io 0pf+0w
- OGE3D (current 26-08-05) : 34.477u 0.299s 0:34.82 99.8%  0+0k 0+56io 0pf+0w
- Version 0.4d 🚀 (RelWithDebug) : 6.622u 0.013s 0:06.64 99.8%     0+0k 0+0io 0pf+0w
- Version 0.4f 🚀 (RelWithDebug) : 6.345u 0.003s 0:06.35 99.8%     0+0k 0+0io 0pf+0w
- Version 0.4g🚀 (RelWithDebug) : 6.618u 0.009s 0:06.63 99.6%     0+0k 0+0io 0pf+0w
- Version 0.4h🚀 (RelWithDebug) : 6.059u 0.003s 0:06.08 99.5%     0+0k 8+0io 0pf+0w
- Version 0.5d (using for range): 4.350u 0.004s 0:04.37 99.5%     0+0k 0+0io 0pf+0w

- Version 0.6a (using for range): 4.812u 0.007s 0:04.84 99.3%     0+0k 0+0io 0pf+0w




```
#define JLOOPS 25
#define ILOOPS 1000000

$sum = 0;
for (%j = 0; %j < JLOOPS; %j++) {
    for (%i = 0; %i < ILOOPS; %i++) {
        $sum ++;
    }
    echo("SUM (++) IS:" SPC $sum);
    for (%i = 0; %i < ILOOPS; %i++) {
        $sum --;
    }
    echo("SUM (--) IS:" SPC $sum);
    $sum = 66;
    echo("set Sum to 66 == " SPC $sum * 1);
    for (%i = 0; %i < ILOOPS; %i++) {
        $sum *= %i + 1;
        $sum /= %i + 1;
    }
    echo("SUM (*/ %i+1) IS:" SPC $sum);
    for (%i = 0; %i < ILOOPS / 2.0; %i++) {
        %ran = getRandomF(%i);
        $sum -= %ran;
        $sum += %ran;
    }
    echo("SUM (rand +-) IS:" SPC $sum);
}
$sum -= 66.0 ;
echo("---------------------");
echo("---------------------");
echo("Final sum should be 0 == ", $sum);
echo("---------------------");
echo("---------------------");

```

## Static Float Field

- Version 0.4a (RelWithDebug) : 29.871u 0.159s 0:30.06 99.8%    0+0k 0+0io 0pf+0w
- OGE3D using tom2DSprite: 192.723u 0.302s 3:13.28 99.8% 0+0k 0+248io 0pf+0w
- Version 0.4c 🚀 (RelWithDebug) : 31.224u 0.193s 0:31.45 99.8%    0+0k 0+0io 0pf+0w
- Version 0.4d 🚀 (RelWithDebug) : 21.784u 0.009s 0:21.82 99.8%    0+0k 0+0io 0pf+0w
- Version 0.4f 🚀 (RelWithDebug) : 21.170u 0.003s 0:21.20 99.8%    0+0k 0+0io 0pf+0w
- Version 0.4g 🚀 (RelWithDebug) : 21.010u 0.009s 0:21.06 99.7%    0+0k 0+0io 0pf+0w
- Version 0.4h🚀 (RelWithDebug) : 19.916u 0.002s 0:19.97 99.6%    0+0k 8+0io 0pf+0w

- Version 0.6a (RelWithDebug) : 12.193u 0.003s 0:12.23 99.6%    0+0k 0+0io 0pf+0w



Here 0.4c is slower than 0.4a (no idea why) - but 0.4b was at the same speed so rocket
change did not cause it. Use local var is now the fastest. global is slightly 
slower than local but still faster than named. 




**OGE3D :: it's slower than a dynamic field ? - here ElfScript fastpath really shine :)**

```
#define JLOOPS 25
#define ILOOPS 1000000

new PointStorageObject(sto);

sto.x = 0;
for (%j = 0; %j < JLOOPS; %j++) {
    for (%i = 0; %i < ILOOPS; %i++) {
        sto.x ++;
    }
    echo("SUM (++) IS:" SPC sto.x);
    for (%i = 0; %i < ILOOPS; %i++) {
        sto.x --;
    }
    echo("SUM (--) IS:" SPC sto.x);
    sto.x = 66;
    echo("set Sum to 66 == " SPC sto.x * 1);
    for (%i = 0; %i < ILOOPS; %i++) {
        sto.x *= %i + 1;
        sto.x /= %i + 1;
    }
    echo("SUM (*/ %i+1) IS:" SPC sto.x);
    for (%i = 0; %i < ILOOPS / 2.0; %i++) {
        %ran = getRandomF(%i);
        sto.x -= %ran;
        sto.x += %ran;
    }
    echo("SUM (rand +-) IS:" SPC sto.x);
}
sto.x -= 66.0 ;
echo("---------------------");
echo("---------------------");
echo("Final sum should be 0 == ", sto.x);
echo("---------------------");
echo("---------------------");
```

## Static Float Field but with objectID (local var) 

- Version 0.4a (RelWithDebug) : 34.982u 1.909s 0:36.97 99.7%    0+0k 0+0io 0pf+0w
- Version 0.4c 🚀 (RelWithDebug) : 24.884u 0.202s 0:25.12 99.8%    0+0k 0+0io 0pf+0w
- Version 0.4d 🚀 (RelWithDebug) : 13.967u 0.003s 0:13.99 99.7%    0+0k 0+0io 0pf+0w
- Version 0.4f 🚀 (RelWithDebug) : 14.015u 0.009s 0:14.04 99.7%    0+0k 0+0io 0pf+0w
- Version 0.4g 🚀 (RelWithDebug) : 14.773u 0.006s 0:14.80 99.7%    0+0k 0+0io 0pf+0w
- Version 0.4h🚀 (RelWithDebug) : 13.582u 0.002s 0:13.62 99.7%    0+0k 0+0io 0pf+0w
- Version 0.5d (using for range): 12.359u 0.007s 0:12.40 99.5%    0+0k 0+0io 0pf+0w

- Version 0.6a (using for range): **6.578u** 0.000s 0:06.60 99.5%     0+0k 0+0io 0pf+0w



### So why was this slower (same with a global var):

- 1. Object is created and %stoObj is set to unsigned int (OP_SAVEVAR_UINT). fine
- 2. `%stoObj.x = 0;` It need to lookup the object and does OP_LOADIMMED_STR => OP_SETCURVAR => OP_LOADVAR_STR => OP_SETCUROBJECT
So it need to convert the integer to string every time we use %stoObj.xxx.  
SimObject* findObject(const char* name) is used  in OP_SETCUROBJECT. But there are 
alternativ: SimObject* findObject(ConsoleValue*); or SimObject* findObject(const char* name);
- 3. Concept to proof: OP_LOADIMMED_STR => 

Example call :
``` 
% sto.x = 1;
0: OP_LOADIMMED_STR stk=+1 str=1
2: OP_LOADIMMED_IDENT stk=+1 str=sto
5: OP_SETCUROBJECT stk=0
6: OP_SETCURFIELD stk=0 field=x
9: OP_POP_STK stk=-1
10: OP_SAVEFIELD_FASTPATH stk=-1 (curCodeIP: 69)
11: OP_POP_STK stk=-1
12: OP_RETURN_VOID stk=0
% $sto.y = 1;
0: OP_LOADIMMED_STR stk=+1 str=1  << This saves the value "1" to the Stack
2: OP_SETCURVAR stk=0 var=$sto    << loopup variable
5: OP_LOADVAR_STR stk=+1          << set the content of $sto to the stack 
6: OP_SETCUROBJECT stk=0          << now it uses the slow int=>string and the slow findObject(const char*)
7: OP_SETCURFIELD stk=0 field=y
10: OP_POP_STK stk=-1
11: OP_SAVEFIELD_FASTPATH stk=-1 (curCodeIP: 69)
12: OP_POP_STK stk=-1
13: OP_RETURN_VOID stk=0 
```

### ElfScript 0.4c changed  OP_SETCUROBJECT to 🚀 rocket mode ;)
After my change to directly call findObject ===>  24.916u 0.166s 0:25.11 99.8%    0+0k 0+0io 0pf+0w

But i in script i don't get an advantage out of this. No matter how i try. The 
assigned var becomes string 

```
% %sto = $sto;
0: OP_SETCURVAR stk=0 var=$sto
3: OP_LOADVAR_STR stk=+1
4: OP_SAVE_LOCAL_VAR_STR stk=0 reg=2  << 💩  my rocket crash here too. $sto is integer . guess this can be fixed 
6: OP_POP_STK stk=-1
7: OP_RETURN_VOID stk=0

```

**fixed assign from global int var** .. still call OP_SAVE_LOCAL_VAR_STR but check which type the stack
var have and set int/float/string ... my BaseElf starfield changed from named to
local var assigned by the global object var raised up from 550 to 900 fps - holy cow.

---

So next one - DefineEngineMethod( SimObject, getId, S32 ....

let see what the stack says in OP_SAVE_LOCAL_VAR_STR  .. .. let me double check 
but we get an integer --- thats fine!!! 

```
% %sto = sto.getId();                                         
0: OP_PUSH_FRAME stk=0 count=1
2: OP_LOADIMMED_IDENT stk=+1 str=sto
5: OP_PUSH stk=-1
6: OP_CALLFUNC stk=+1 name=getId nspace=(null) callType=MethodCall
12: OP_SAVE_LOCAL_VAR_STR stk=0 reg=2    << 💩  my rocket crash here 
14: OP_POP_STK stk=-1
15: OP_RETURN_VOID stk=0
```

- [X] Fine Fine but to make it complete - i also need to change global vars: `OP_SAVEVAR_STR`

### 🚀 Second rocket stage ignited: OP_SAVE_LOCAL_VAR_STR and OP_SAVEVAR_STR*

### Code:

```
#define JLOOPS 25
#define ILOOPS 1000000

%stoObj = new PointStorageObject();

%stoObj.x = 0;
for (%j = 0; %j < JLOOPS; %j++) {
    for (%i = 0; %i < ILOOPS; %i++) {
        %stoObj.x ++;
    }
    echo("SUM (++) IS:" SPC %stoObj.x);
    for (%i = 0; %i < ILOOPS; %i++) {
        %stoObj.x --;
    }
    echo("SUM (--) IS:" SPC %stoObj.x);
    %stoObj.x = 66;
    echo("set Sum to 66 == " SPC %stoObj.x * 1);
    for (%i = 0; %i < ILOOPS; %i++) {
        %stoObj.x *= %i + 1;
        %stoObj.x /= %i + 1;
    }
    echo("SUM (*/ %i+1) IS:" SPC %stoObj.x);
    for (%i = 0; %i < ILOOPS / 2.0; %i++) {
        %ran = getRandomF(%i);
        %stoObj.x -= %ran;
        %stoObj.x += %ran;
    }
    echo("SUM (rand +-) IS:" SPC %stoObj.x);
}
%stoObj.x -= 66.0 ;
echo("---------------------");
echo("---------------------");
echo("Final sum should be 0 == ", %stoObj.x);
echo("---------------------");
echo("---------------------");

```

# Dynamic (string) Field - named again

- Version 0.4a (RelWithDebug) : 127.546u 2.019s 2:09.75 99.8%   0+0k 0+0io 0pf+0w
- OGE3D using ScriptObject: 137.226u 0.296s 2:17.73 99.8% 0+0k 0+136io 0pf+0w
- Version 0.4c 🚀 (RelWithDebug) : 125.068u 1.963s 2:07.38 99.7%   0+0k 1416+0io 8pf+0w
- Version 0.4d 🚀 (RelWithDebug) : 124.605u 1.840s 2:06.71 99.7%   0+0k 0+0io 0pf+0w
- Version 0.4e 🚀 (RelWithDebug) : 114.157u 1.692s 1:56.06 99.8%   0+0k 0+0io 0pf+0w
- Version 0.4f 🚀 (RelWithDebug) : 22.012u 0.000s 0:22.04 99.8%    0+0k 0+0io 0pf+0w
- Version 0.4g 🚀 (RelWithDebug) : 22.104u 0.000s 0:22.12 99.9%    0+0k 0+0io 0pf+0w
- Version 0.4h🚀 (RelWithDebug) : 21.901u 0.005s 0:21.97 99.6%    0+0k 8+0io 0pf+0w


---
### same test but using local var (docu/speedtest/elf/test_dynamic_fields_localvar.elf)
- Version 0.4h🚀 (RelWithDebug) : 14.863u 0.001s 0:14.90 99.7%    0+0k 0+0io 0pf+0w

### same test but using local var / for range (docu/speedtest/elf/test_dynamic_fields_forrange.elf)
- Version 0.6a (RelWithDebug) : 5.571u 0.004s 0:05.60 99.4%     0+0k 0+0io 0pf+0w


Note: dynX must have TypeF32 set, else it fall back to string.
---

**OGE3D here nearly on same speed, since ElfScript have no Dyanmic Field optimations**

### 0.4c/d This my problem child: 

- Dynamic fields in the object is one of the coolest features in Elf(Torque)Script.
- I could get a better result with local var instead of named now, but is not a 
big difference.
- It store the values in a fast Map as strings but i need the values also as float and int. 
    
- value is stored in SimFieldDictionary: char *value;

    - void SimFieldDictionary::setFieldValue(StringTableEntry slotName, const char *value)
    - SimFieldDictionary::Entry *SimFieldDictionary::addEntry(U32 bucket, StringTableEntry slotName, ConsoleBaseType* type, char* value)
    - const char *SimFieldDictionary::getFieldValue(StringTableEntry slotName)
    - void SimFieldDictionary::printFields(SimObject *obj)
    - Entry() : slotName(StringTable->EmptyString()), value(NULL), next(NULL), type(NULL) {};
    - char *value;

```
% $sto = new PointStorageObject(sto);$Debug::DumpByteCode=1; 
% $sto.self = $sto;
0: OP_SETCURVAR stk=0 var=$sto
3: OP_LOADVAR_STR stk=+1
4: OP_SETCURVAR stk=0 var=$sto
7: OP_LOADVAR_STR stk=+1
8: OP_SETCUROBJECT stk=0
9: OP_SETCURFIELD stk=0 field=self
12: OP_POP_STK stk=-1
13: OP_SAVEFIELD_FASTPATH stk=-1 (curCodeIP: 69)
14: OP_POP_STK stk=-1
15: OP_RETURN_VOID stk=0

```
OP_SAVEFIELD_FASTPATH is fine but curObject->findField(curField); is nullptr because
it's dynamic field also fine. it enter SimObject::setDataField so we are in slow string mode. 
It jumps to DynField.. but now it check for  `if(!array)` only. Not if it's also have 
an entry. 

test_dynamic_fields_localvar.elf 
==> 106.306u 1.783s 1:48.25 99.8%   0+0k 0+0io 0pf+0w


same on OP_LOADFIELD_STR => `if(!array)`

test_dynamic_fields_localvar.elf
==> 106.732u 1.770s 1:48.67 99.8%   0+0k 0+0io 0pf+0w

??? slower ??? should be faster .. maybe my laptop is tired ... 


==> 114.157u 1.692s 1:56.06 99.8%   0+0k 0+0io 0pf+0w
overall 10sec faster ... when i want more i need to add float/int to SimFieldDictionary
or instad of value a ConsoleValue object ?! << this would make sense or not ? 


- [X] replaced const char* value with ConsoleValue mValue  -
    ==> "named" speed test: 114.462u 1.952s 1:56.65 99.7%   0+0k 0+0io 0pf+0w
    thats fine same as before but with ConsoleValue in place 
    
- [X] use types in mValue SAVE:
     bool pushDataField(StringTableEntry slotName, const char *array, ConsoleValue* stackP, S32 type_OP_path);
     
     called from OP_SAVEFIELD_FASTPATH
        
        - implemented status quo
        - wrote new implemenation based on the incomming stack, the data is set.
        - added also ConsoleValueType in SimFieldDictionary::setFieldType (float/int/string)
        - bad: 112.777u 1.854s 1:54.86 99.7%   0+0k 0+0io 0pf+0w

     

- [X] use types in mValue LOAD:
        - implemented fast set of stack 
        - 113.513u 1.814s 1:55.47 99.8%   0+0k 0+0io 0pf+0w


- [X] Find what is missing 
    I made get and set and also added typed but is still slow !!!!
    - also added FLT_/INT_ oto this path 
    - => shit => 115.705u 1.714s 1:57.69 99.7%   0+0k 192+0io 2pf+0w

    Everything seams to work fine but somewhere it gets slow down ....
    i break the test script in parts :
    
        - sto.DynX ++; : 20.060u 0.365s 0:20.46 99.8%    0+0k 0+0io 0pf+0w
        - sto.DynX -- ; : 19.625u 0.398s 0:20.06 99.7%    0+0k 0+0io 0pf+0w
        - */ : 37.276u 0.683s 0:38.00 99.8%    0+0k 0+0io 0pf+0w
        - rand +-: 27.374u 0.296s 0:27.71 99.8%    0+0k 0+0io 0pf+0w
    
    All slow nothing special 
    - setrand only: 10.443u 0.236s 0:10.69 99.8%    0+0k 0+0io 0pf+0w

    👾 LOL! I had threaded on in releasebuild where the changes are not implemented so far
    => 22.338u 0.009s 0:22.39 99.7%    0+0k 0+0io 0pf+0w
    
    🐞 the values are all ZERO ?! << FIXED
    
    22.151u 0.000s 0:22.17 99.9%    0+0k 0+0io 0pf+0w
    
    ==> SAME SPEED AS STATIC FIELDS  - ROCK AND ROLL
    

    
### Version 0.4f:

- [X] lot of testing << some but will be continued
- [X] cleanup compiled eval or completly switch to threaded!

        - [X] make threaded as default add and test the latest changes!!! 
            -  OP_LOADFIELD_UINT
            -  OP_LOADFIELD_FLT
            -  OP_LOADFIELD_STR
            -  OP_SAVEFIELD_FLT:
            -  OP_SAVEFIELD_UINT:
            -  OP_SAVEFIELD_FASTPATH:
        - [~] remove GarbageCollection shit
        - [X] remove fast path ifdefs and make it default 
        - [X] ELFSCRIPT_PREPROCESSOR also as default
        
- [ ] Make a memleak check after that !!!!!!!!!!!

---

## Code: 

```
#define JLOOPS 25
#define ILOOPS 1000000

%sto = new PointStorageObject(){
    TypeF32 DynX = 0; // here is the beef!
};

sto.DynX = 0;
for (%j = 0; %j < JLOOPS; %j++) {
    for (%i = 0; %i < ILOOPS; %i++) {
        sto.DynX ++;
    }
    echo("SUM (++) IS:" SPC sto.DynX);
    for (%i = 0; %i < ILOOPS; %i++) {
        sto.DynX --;
    }
    echo("SUM (--) IS:" SPC sto.DynX);
    sto.DynX = 66;
    echo("set Sum to 66 == " SPC sto.DynX * 1);
    for (%i = 0; %i < ILOOPS; %i++) {
        sto.DynX *= %i + 1;
        sto.DynX /= %i + 1;
    }
    echo("SUM (*/ %i+1) IS:" SPC sto.DynX);
    for (%i = 0; %i < ILOOPS / 2.0; %i++) {
        %ran = getRandomF(%i);
        sto.DynX -= %ran;
        sto.DynX += %ran;
    }
    echo("SUM (rand +-) IS:" SPC sto.DynX);
}
sto.DynX -= 66.0 ;
echo("---------------------");
echo("---------------------");
echo("Final sum should be 0 == ", sto.DynX);
echo("---------------------");
echo("---------------------");
```


Speed template saved here ;)

| Script | time |
| --- | --- |
| test_localvar.elf     |  |
| test_global.elf       |  |
| test_static.elf       |  |
| test_dynamic.elf      |  |
| test_vector_components.elf    |  |
