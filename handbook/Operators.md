# ElfScript Syntax Handbook - Operators and Control Statements

[Back to Main](./Main.md)

## Operators

Most operators are the same as used in C/C++.

 - '+' plus
 - '-' minus 
 - '*' multiplication
 - '/' division
 
 - '$=' equal (string)
 - '$!=' not equal (string)
 - '==' equal (numeric)
 - '!=' not equal (numeric)
 - '<' lower 
 - '>' greater
 - '>=' greater equal (numeric)
 - '<=' lower equal (numeric)

 - '=' assignment
 - '+=', '*=', '-=' and '/=' and some more an be comined. So `$a+=5;` is the same as `$a = $a + 5;`
 - '++' add one. only allowed like $a++ NOT ++$a;
 - '--' decrement one. only allowed like $a-- NOT --$a;
 
 - '%' modulo
 - '|' Bitwise or
 - '^' Bitwise xor
 - '&' Bitwise and
 - '>>' SHR
 - '<<' SHL
 
 - '&&' and in Statements
 - '||' or in Statements
 
 - '//' one line comment 
 - '/* */' inline or multiline comment 

Maybe I missed something ;)
 
## if and switch 

**if** by example:
```
$foo = 1;
/* without brakets at one statement: */
if ($foo == 1) echo("$foo is one!");
if ($foo != 1) echo("$foo is not one!");
if ($foo >= 1) echo("$foo is not one or more!");
/* with brakets: */
if ( $foo <= 1) {
    echo("$foo is less than 1 let increment it");
    $foo++;
}
/* for strings: */
if ($foo $= "1") echo ("$foo is" SPC $foo);

/* short if then */
echo ( $foo == 1 ? "ONE" : "something else");

```

**switch** by example:
```
$foo = 4;

switch ($foo) {
    case 1: echo("$foo is one!");
    case 2: echo("$foo is two!");
    default: echo("$foo is" SPC $foo);
}
``` 
You may notice i did not write a break. In ElfScript you don't need to add a break
at the end of a case statement.

On Strings we need to add a "$" like comparing with if.
```
$foo = "bar";

switch$ ($foo) {
    case "a": echo("$foo is a!");
    case "bar": echo("$foo is bar!");
    default: echo("$foo is" SPC $foo);
}
```

## for,  while and foreach

Example *for*:
```
for (%i = 0; %i < 10; %i ++) {
    echo("%i is:" SPC %i);
}
```

Example *for with range(fast)*:
```
for (%i in range 10) {
    echo("%i is:" SPC %i); //print 0 .. 9 
}
```

```
for (%i in 0..10) {
    echo("%i is:" SPC %i); //print 0..10
}
```

Example *while* with skip (continue) and stop loop (break). Dont forget to increment
before continue ;) 
```
%i = 0;
while ( %i < 20 ) {
    if (%i == 5) { 
        %i++; 
        continue;
    } 
    if (%i == 10) break;  
    echo("%i is:" SPC %i);
    %i++;
}
```

*foreach*: 

1. It's designed for SimSet's and SimGroup's to loop it's members
2. ~~~foreach$~~~  foreach can also be used for words like "A B C".
3. It can be used also for integers: `foreach(%i in 1..3) {}`. 
**Importent** it gives you all numbers: 1,2,3 like pascal: `For i:=1 to 3 do ...`
4. If you want to interate from 1 to 3 and set the maxium you an use `foreach(%i in range 1..3) {}`
5. Like the Set loop it also works for the Array Object

SimSet Example with named Object  and cleanup:
``` 
new SimSet(MySimSet);
MySimSet.add ( new SimObject() { foo = 1;});
MySimSet.add ( new SimObject() { foo = 2;});
MySimSet.add ( new SimObject() { foo = 3;});

foreach(%obj in MySimSet) {
    echo("The foo of object" SPC %obj.getId() SPC "is" SPC %obj.foo);
}

MySimSet.deleteAllObjects();
MySimSet.delete();
```

String Example:

```
%MyString = "Die Kuh lief um den Teich.";
foreach (%word in %MyString) {
    echo("..", %word, "..");
}
echo("Drei mal ganz schnell hintereinander...");
```

Integer Examples:

Two available usages : 
- foreach( %i in first..last )
- foreach( %i in range start..stop )

```
foreach( %i  in 1..3) { print(%i); } /* print: 1 2 3 */
foreach( %i  in range 1..3) { print(%i); } /* print: 1 2 */
foreach( %i  in 3..1) { print(%i); } /* print: 3 2 1 */
foreach( %i  in range 3..1) { print(%i); } /* print: 3 2*/
foreach( %i  in 0..0) { print(%i); } /* print: 0 */
foreach( %i  in range 0..0) { print(%i); } /* no output*/

// all the previous also work with for:
for( %i in range 3) { print(%i); } /* print: 1 2 */
for( %i in range 0..5 step 3) { print(%i); } /* print: 0 3 */
for( %i in range 0..-5 step -3) { print(%i); } /* print: 0 -3 */

for( %i in range 0..-5 step 1) { print(%i); } /* print: ERROR */

```




[Objects and Functions](./Objects.md) 

[Back to Main](./Main.md)
