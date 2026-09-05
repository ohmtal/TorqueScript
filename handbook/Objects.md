# ElfScript Syntax Handbook - Objects and Funcions

[Back to Main](./Main.md)

## Functions 

- Functions can be accessed global like the global variables. 
- Functions use local parameter and may return a value or not. 
- The return value can be every type available float/int/bool/string.
- A default parameter is defined with "=".
- When a custom function is called there is no validation of parameters.
- you can use `isFunction("NAME");` to check a function exits

```
function foo(%name, %age, %gender, %active = false) {
    echo(
        "Hello" SPC %name NL "your age is: " SPC %age NL
        "Your gender is " SPC (%gender $= "f" ? "female" : "male")
    );
    if (!active) echo("Too bad you are not active");
    
    return true;
}
foo("lisa", "30", "f");
```




## Objects

- Objects are used a lot to organize the code and to speed up execution. Like you 
create an Object in C++ to represent a Sprite with all it's Properties and methods. 
- The script does not use pointer to objects. It uses ObjectId's (unsigned integer)
- So we can store the reference in every variable 
- Or use a Name. which is set like this `new ScriptObject(MyObject); `
- Initial parameters can be set when constucted. 
- Every Object have a **onAdd** and **onRemove** callback to initalize and cleanup.
- Some function/method calls require a ObjectId - when we use a named object we 
must add the .getId() method => MyObject.getId(). return the ObjectId

- In my example i create a SimGroup called GameGroup add some objects and cleanup 
again. 
- SimSet and SimGroup store a list of Objects but a Object can be only a 
member of one SimGroup. 
- When a SimGroup is deleted all members are also deleted. We use that for easy clean or members. 
- Methods use Namespaces. So if you write a Method it can be 
    - function SimGroup::abc(%this) << can be used by all SimGroup's
    - function GroupClassForAll::def(%this) << used by all which have class set GroupClassForAll
    - function GameGroup::ghi(%this) << used by the object named GameGroup
- Methods first parameter is %this which hold the objectId
- Methods are called with a dot and ()
- Properties are also separated with a dot bit without () on the end.
- Use `isObject([ObjectID])` to check a object exits
- Use `OBJECT.isMethod("foo")` to check a method of a object exits

```
/* i write them before the create */
function GameGroup::onAdd(%this) {
    echo("Hello I'am new here");
}
function GameGroup::onRemove(%this) {
    echo("Bye Bye");
}

new SimGroup(GameGroup) { class = "GroupClassForAll";};
GameGroup.add( new SimObject() { TypeS32 foo = 1; });
GameGroup.add( new SimObject() { TypeS32 foo = 2; });
GameGroup.add( new SimObject() { TypeS32 foo = 3; });
GameGroup.add( new SimObject() { TypeS32 foo = 4; });
/* List the group in console */
GameGroup.listObjects();
/* iterate */
foreach( %obj in GameGroup ) {
    %obj.foo++;
    echo("Object with ID:" SPC %obj.getId() SPC " has foo ==" SPC %obj.foo);
}
/* create a custom method which is assigned to object GameGroup*/
function GameGroup::myCustomMethod(%this) {
    echo("My Id is" SPC %this);
}
GameGroup.myCustomMethod();
/* useful functions */
GameGroup.dump(); /* dump fields and methods */
GameGroup.dumpFields(); /* dump fields */
GameGroup.dumpMethods(); /* dump methods */

/* finally i delete the GameGroup and all it's members */
$saveID = GameGroup.getId();
GameGroup.delete();

echo("Object exits:" SPC isObject($saveID));
```

[Arrays, Lists and Key Value Tables](./ArraysAndMore.md)

[Back to Main](./Main.md)
