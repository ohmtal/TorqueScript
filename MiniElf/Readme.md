# MiniElf

The really minimalistic Demo how to add ElfScript to your project. 

I only contains the CMakeLists.txt and a small main.cpp - which could be even
smaller if i did not overwrite the argument parser and added two example bindings.

In source you need up to three includes: 

```
#include "main/engineGlue.h"    // << to init/shutdown
#include "console/script.h"     // << to load a script
#include "console/engineAPI.h"  // << to bind functions
```

Before you can use it - this init call:

```
engineGlue::init(nullptr ); // nullptr => we use default logger
```

And at the end this shutdown call for cleanup and garbage collection:

```
 engineGlue::shutDown();
```

Thats it - basically ;) 

For more enhanced Implementations I did some projects: Raylib-ElfScript, BaseElf,
CrazyElf, ElfFlux and OhmFlux/ElfTest - so far. 


