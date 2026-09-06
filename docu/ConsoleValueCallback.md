# Add return value ConsoleValue

Very compilicated ;) I did it for ConsoleVector:

**search for:** VectorCallbackType and VectorCallback and mVecC


## console.h
```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
typedef ConsoleValue(*ConsoleValueCallback)(SimObject *obj, S32 argc, ConsoleValue argv[]);
#endif
```

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
   void addCommand( const char* name,ConsoleValueCallback cb,const char *usage, S32 minArgs, S32 maxArgs, bool isToolOnly = false, ConsoleFunctionHeader* header = NULL );///< @copydoc addCommand( const char *, StringCallback, const char *, S32, S32, bool, ConsoleFunctionHeader* )
#endif
```

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
void addCommand(const char *nameSpace, const char *name, ConsoleValueCallback cb, const char *usage, S32 minArgs, S32 maxArgs, bool toolOnly = false, ConsoleFunctionHeader* header = NULL); ///< @copydoc addCommand( const char*, const char *, StringCallback, const char *, S32, S32, bool, ConsoleFunctionHeader* )
#endif     
```

ConsoleConstructor
```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
      ConsoleValueCallback mValueC;     ///< A function/method that returns a ConsoleVector.
#endif
```

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
   ConsoleConstructor(const char *className, const char *funcName, ConsoleValueCallback bfunc, const char *usage, S32 minArgs, S32 maxArgs, bool isToolOnly, ConsoleFunctionHeader* header );
#endif
```


## console.cpp

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
   mValueC = 0;
#endif
```

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
      else if( walk->mValueC)
        Con::addCommand( walk->mClassName, walk->mFuncName, walk->mValueC, walk->mUsage, walk->mMina, walk->mMaxa, walk->mToolOnly, walk->mHeader);
#endif
```

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
ConsoleConstructor::ConsoleConstructor(const char *className, const char *funcName, ConsoleValueCallback bfunc, const char *usage, S32 minArgs, S32 maxArgs, bool isToolOnly, ConsoleFunctionHeader* header )
{
      init( className, funcName, usage, minArgs, maxArgs, isToolOnly, header );
      mVecC = bfunc;
}
#endif
```

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
void addCommand( const char *nsName, const char *name,ConsoleValueCallback cb, const char *usage, S32 minArgs, S32 maxArgs, bool isToolOnly, ConsoleFunctionHeader* header )
{
      Namespace *ns = lookupNamespace(nsName);
      ns->addCommand( StringTable->insert(name), cb, usage, minArgs, maxArgs, isToolOnly, header );
}
#endif
```

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
void addCommand( const char *name,ConsoleValueCallback cb,const char *usage, S32 minArgs, S32 maxArgs, bool isToolOnly, ConsoleFunctionHeader* header )
{
      Namespace::global()->addCommand( StringTable->insert(name), cb, usage, minArgs, maxArgs, isToolOnly, header );
}
#endif
```

## consoleInternal.h


```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
         ConsoleValueCallback mConsoleValueCallbackFunc;
#endif
```

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
 void addCommand(StringTableEntry name, ConsoleValueCallback, const char *usage, S32 minArgs, S32 maxArgs, bool toolOnly = false, ConsoleFunctionHeader* header = NULL);
#endif
```


## consoleInternal.cpp

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
void Namespace::addCommand(StringTableEntry name, ConsoleValueCallback cb, const char *usage, S32 minArgs, S32 maxArgs, bool isToolOnly , ConsoleFunctionHeader* header)
{
      Entry *ent = createLocalEntry(name);
      trashCache();

      ent->mUsage = usage;
      ent->mHeader = header;
      ent->mMinArgs = minArgs;
      ent->mMaxArgs = maxArgs;
      ent->mToolOnly = isToolOnly;

      ent->mType = Entry::ConsoleValueCallbackType;
      ent->cb.mConsoleValueCallbackFunc = cb;
}
#endif
```

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
      case ConsoleValueCallbackType:
            result.transferFrom(cb.mConsoleValueCallbackFunc(thisObj, argc, argv));
        break;
#endif
```

```
```

```
```

## engineAPI.h

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
inline ConsoleValue _EngineConsoleThunkReturnValue( ConsoleValue value )
{
      return value;
}

#endif
```

<!-- ``` -->
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
template<>
struct _EngineConsoleThunkType< ConsoleValue >
{
      typedef ConsoleValue ReturnType;
      typedef ConsoleValueCallback CallbackType;
};
#endif
```





## SimObject.h

---


struct Entry
```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
      , ConsoleValueCallbackType
#endif
```


# compiledEval...

```
#ifdef ENABLE_CONSOLE_VALUE_CALLBACK
                        case Namespace::Entry::ConsoleValueCallbackType:
                        {
                              ConsoleValue result = nsEntry->cb.mConsoleValueCallbackFunc(thisObject, callArgc, callArgv);
                              gCallStack.popFrame();
                              stack[_STK + 1].copyFrom( result) ;
                              PUSH_STK();
                              break;
                        }
#endif
```


- [X] TEST clean when disabled ENABLE_CONSOLE_VALUE_CALLBACK
      $foo = new ValueVector(); $foo.push_back(47.22); echo($foo.get(0));    

- [X] FIX ENABLE_CONSOLE_VALUE_CALLBACK

/opt/ElfScript/CrazyElf/../ElfScript/console/engineAPI.h:662:44: error: could not convert ‘_EngineConsoleThunkReturnValue<ConsoleValue>(engineAPI::detail::ThunkHelpers<2, ConsoleValue, int>::dispatchHelper<_ValueVectorgetframe, 0>(argc, argv, fn, frame, (* & defaultArgs), (_EngineConsoleThunk<2, ConsoleValue(int)>::SeqType(), _EngineConsoleThunk<2, ConsoleValue(int)>::SeqType())))’ from ‘const char*’ to ‘_EngineConsoleThunk<2, ConsoleValue(int)>::ReturnType’ {aka ‘ConsoleValue’}

=> missing _EngineConsoleThunkReturnValue in engineAPI
