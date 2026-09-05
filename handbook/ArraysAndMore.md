# ElfScript Syntax Handbook - Array,  Sets, Key Value 

@ElfScript 0.7

[Back to Main](./Main.md)


## ConsoleVector (TypeVector) 

ConsoleVector is can be primary used as a storage for Points, Rects, Colors. 
It's a four point float structure. 

Example setter: `%vec = {10,20,30,40};`

Example getter single value (x component): `%x = %vec.x;`

Valid components are: **x, y, z, w** or **r, g, b, a** or **x, y, width, height** 

## Array 

An array is a flat list of values. When you use the following Constructor an 
Array Object is created and should be deleted when not longer used:

Example constructor: `%arr = [10,20,30,40,50,60];`

Example getter of first value: `%value = %arr.at(0);` 

I also added the Array Object to foreach so you can use `foreach(%value in %arr) {}`

The Array Object loosly follow the std::vector methods:

- getter:
    - at(index)
    - first()
    - last()
    - pop_front() like first but delete first
    - pop_back() like last but delete last

- setter
    - push_back(value) 
    - set(index, value)
    - insert(index, value)

- modifier
    - clear()
    - reserve(count)
    - fill(value)
    - erase(index)

- debug
    - list()
    
- SimObject
    - delete() delete the object and clean up data
    
    
# Key Value Table 

Every Object have a build in key value table called "dynamic fields".
When defining the object the Fieldtype should be set, else it defaults to string. 

The Types are:

- TypeF64       64bit float (double)  - TypeF32 is an alias
- TypeS64       64bit signed integer (long) - TypeS32 is an alias
- TypeVector    A ConsoleVector (4 points)
- TypeBool      A alias to TypeS64
- TypeString    default type  - a string. 

Example
```
%obj = new Object() {
    TypeF64  myFloat = 0.0;
    TypeS64  myInt   = 0;
    TypeVector myVec = {1,2,3,4};
    TypeString myStr = "Hello World";
};

printf("f:%f, i:%d, v:%s, s:%s", %obj.myFloat, %obj.myInt, %obj.myVec, %obj.myStr);

```

# Lists (SimSet, SimGroup or the alias Group)

All of this lists can be used to hold Objects. 

A Group is a Simset which does delete it's objects (members) when it is deleted
so a Object can only be member of one Group. 

So you can add a Object's to this Set-Types and work with it like an array. 






[Optimize for Speed](./Optimize.md) 

[Back to Main](./Main.md)
