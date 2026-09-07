# ElfScript Syntax Handbook - Variables and Fields

@ElfScript 0.7

[Back to Main](./Main.md)

## 1.) Common Notes

- We have global and local variables. 
- There are also static and dynamic fields in Objects. 
- You do not need to initialize a Variable 
- The common types are float, int and string. All Structs(Vectors) like a Point are stored in a String.

## 2.) Local Variable

A local variable is only vaild in a local scope. It start with a **%**.

`%a = 5;` We set local variable %a to a integer 5
`%b = 5.55;` We set local variable %b to a float 5.55
`%c = "foo";` We set local variable %c to a string "foo" 
`%color = "255 0 0 255";` We set %color to a String which represent a red color. 
`%color = {255, 0, 0, 255};` Same as before but better readable. (*ElfScript only)

## 3.) Global Variables 

A global Variable can be accessed everywhere in your scripts and start with a "$";
The definition is the same as a local variable. 

## Local / Global scope showcase:

```
print("----------------- Local / Global var scope showcase ----------------- ");
%localVar = "LocalVar";
$GlobalVar = "GlobalVar";

function showLocal() {
    print("inside func %localvar==",%localvar, ", $GlobalVar==",$GlobalVar); //local should be empty
}

print("outside func %localvar==",%localvar, ", $GlobalVar==",$GlobalVar); //local should be filled

showLocal();
```

## 4.) Fields

A Field is a property of a Object. We have two types of fields:

- static field: defined in the C code with a strict type 
- dynamic fields: added dynamicly they are stored as strings in C so the access 
is slower than static but the dynamic fields feature is very useful. 

I will talk about Objects later but for fields i add an example:

```
$MyObject = new ScriptObject();
$MyObject.customValue = 5; //bad is casted as string
$MyObject.addField("CustomFloatValue", "TypeF64", 5.0); // good casted as float, Note: Typename as String
```
We created an basic ScriptObject. Because it's basic we have no useful static fields
here so i add a dynamic field called "customValue". Fields are separated from the 
Object with a dot ".". 

See also [Objects](./Objects.md)


## 5.) Init of a Variable 

As said before we set not type and do have to initialize a variable or a field.
When i use `echo($foo);` it will print an empty string also if i never had used 
it before and `echo($foo++);` will print "1" also if it was empty == 0 before.
This can be a bit dangerous if you misstype a variable name. 

## 6.) Types

Numbers [TypeS64] or [TypeF64]:
```
123     (Integer)
1.234   (floating point)
1234e-3 (scientific notation)
0xc001  (hexadecimal)
```

String [TypeString]:
```
"Hello World" (normal string)
```

ConsoleVector [TypeVector] (4 Float's )
```
%myVec = { 10.0, 20.09 }; // (Vector first 2 values filled => 10.0, 20.09, 0.0, 0.0)
```

Array Construtor:
```
%myArray = [10.0, "Hello", 50]; // Array Object with 3 Values 
```


Boolean: 
```
true  == 1
false == 0
```

String Operators: 
```
@       (concatenates two strings)
TAB     (concatenation with tab)
SPC     (concatenation with space)
NL      (newline)
```

Note: **SPC** is the most use operator. If you want to concat strings you write:
`$myString = 100 * 5 SPC "tons";` $myString is filled with: "105 tons";


Escape Sequences:
```
\n          (newline)
\r          (carriage return)
\t          (tab)
\xhh        (two digit hex value ASCII code)
\"          (quotation mark)
\\          (backslash)
```

---

[Operators and Control Statements](./Operators.md) 

[Back to Main](./Main.md)

