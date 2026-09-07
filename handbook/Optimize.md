# ElfScript Syntax Handbook - Speed optimization tips

[Back to Main](./Main.md)


## local variables

Using local variables is the fastest way to work with data. But if you need to 
access it globally the global variable is also not soo bad. 

If you do a loop with lot of calculations you can map the data to a local 
variable and write it back to the global after you are done. 

Simple Example:

```
$GlobalValue = 789456.0;
function looper() {
    %myValue = $GlobalValue;
    for (%i = 0; %i < 789123; %i++){
        %myValue += 5.0;
        %myValue /= 2.0;
    }
    $GlobalValue = %myValue;
}
looper();echo($GlobalValue SPC " is 5"); 
```

Here i expect not a big difference but if you use this method with Object fields
it can matter. 



## Named Objects vs Object-ID

When you create a object with `new SimObject(foo);` you can access the object
with `foo.myMethod();`. When you have to use this a lot it's better to set : 
`%fooObj = foo.getID();` and then work with the id `%fooObj.myMethod();`. In 
Methods you use %this which is a local variable anyway.
 


## Dynamic fields: 

You can set a Type on Dynamic Fields. 

Common Types are: 

- TypeS64 for integer
- TypeF64 for float 
- TypeVector for an 4 point vector

if you set nothing it's set as string. 

When you do calculations on that field TypeF32 is much faster then you did not 
define it == string. 

Example:

```
$posObj = new ScriptObject() {
    TypeF64 x = 0.0;
    TypeF64 y = 0.0;
};
```
    
You can also add a new typed field with:

```
$posObj.addField("z", "TypeF64", 0.0);
$posObj.dumpFields(); 
```

I use $posObj because this works also if you test this scripts in the console 
using BaseElf.

You can also use `$posObj.setFieldType("m0", "TypeF64");` but then the field
m0 must exists. Example: `$posObj.m0=0; $posObj.setFieldType("m0", "TypeF64");`.

## foreach / for in Range

Foreach on SimSets, Arrays or Strings: 

```
    // dummy simgroup : 
    %mySimGroup  = new SimSet();
    foreach (%i in range 5) %mySimGroup.add( new SimObject());

    // for "slow" style
    %count = %mySimGroup.getCount();
    for (%i = 0; %i < %count; %i++ ) {
        %obj = %mySimGroup.getObject(%i);
        echo("for id:" SPC %obj.getId());
    }
    // fast foreach:
    foreach(%obj in %mySimGroup)  echo("foreach id:" SPC %obj.getId());

    %mySimGroup.delete();
```

Foreach on space separated strings (words) 
(since ElfScript 0.7 you can also use foreach only for strings):

```
    // for "slow" style
    %myString = "one two three";
    echo("WORD LOOP on ", %myString);
    %count = getWordCount(%myString);
    for (%i = 0; %i < %count; %i++ ) {
        %word = getword( %myString, %i);
        echo("Word is: ", %word);
    }
    
    // fast foreach:
    foreach$(%word in %myString) echo("foreach Word is: ", %word);
```

foreach on integer:

```
    // for "slow" style
    for (%i = 0; %i < 5; %i++ ) {
        echo("i is: ", %i);
    }

    // fast foreach or for:
    foreach (%i in range 0..5)  echo("foreach: i is: ", %i);
    for (%i in range 5)  echo("for range: i is: ", %i);
    
```


## PointStorageObject

Since we have no real structs in ElfScript. I added an object where you can work 
with lot's of 2D-/3D-Points or Rectangles. 

***The base fields:***

```
  float x = "0"
  float y = "0"
  float z = "0"
  float w = "0"
  float height = "0" //alias to .z
  float width = "0"  //aliad to .w
```

But this is only one Point/Rectangle. But i need much more. So lets say i need
1000 i do:

```
$pointStorage=new PointStorageObject() { storageSize = 1000; }
/* or if you need 10k later: */
$pointStorage.storageSize = 10000;
```

There are multiple ways to work with it:

- Direct read/write to storage: 
    - setPoint (U32 index, F32 x, F32 y, F32 z, F32 w) - set the points for storage index
    - getPosVec(index) :: return a space separated string with all four elements at index
    - setPointVec(index,x,y,z,w): set the elements at index 
    - getPoint2Vec(index) :: return a space separated string "x y"
    - setPoint2Vec(index,x,y) :: set x and y at index
    - getPointX(index)/getPointY(index)/getPointZ(index)/getPointW(index) - getting one value at index
    - same for setPointX(Y/Z/W)(index,value)

- Push / Pull 
    - storePoint(index) : copy the base x,y,z,w to storage at index
    - fetchPoint(index) : set the base x,y,z,w from the data at storage index
        
From StarField Example ( "....." = other code) :

```
#define STAR_COUNT 420
.....

function StarField::OnAdd(%this) {
.....
    $starPoints = new PointStorageObject(starPoints) {
        storageSize = STAR_COUNT;
    };
.....
    for ( %i = 0; %i < STAR_COUNT; %i++)
    {
        $starPoints.setPoint(%i, GetRandom($minH , $maxH),GetRandom($minW , $maxW), 1.0 );
    }
......
}
```

Here i create the PointStorageObject with 420 Points. Then I randomize the 420 points.

``` 
......
function StarField::Render(%this) {

......
    %LocalPointStorage = $pointStorage; // slightly faster
    for ( %i = 0; %i < STAR_COUNT; %i++)
    {
        %LocalPointStorage.fetchPoint(%i); // This writes from the point storage to .x,.y,.z

        // >>>>>>>>>>>>>>>> update point
         %LocalPointStorage.z -= %dt * %speed;

        // Calculate the screen position
        %invZ = 1.0 /  %LocalPointStorage.z;
        %screenPos_X = $hW + (%LocalPointStorage.x * %invZ);
        %screenPos_Y = $hH + (%LocalPointStorage.y * %invZ);

        if ( %LocalPointStorage.z < 0.0 || %screenPos_X < 0.0 || %screenPos_Y < 0.0
             || %screenPos_X > $screenWidth || %screenPos_Y > $screenHeight)
        {

            // %LocalPointStorage.setPos(GetRandomValue($minH, $maxH), GetRandomValue($minH, $maxH), 1.0);
            %LocalPointStorage.setPos(GetRandom($minH, $maxH), GetRandom($minH, $maxH), 1.0);
            %screenPos_X = $hW +  %LocalPointStorage.x;
            %screenPos_Y = $hH +  %LocalPointStorage.y;
        }
        // <<<<<<<<<<<<<<<<<<<<<
        if (%drawLines)
        {
            %t = mClampF(%LocalPointStorage.z + %startDiv , 0.0 , 1.0 );

            if ( (%t - %LocalPointStorage.z) > 1e-3)
            {
                %invT = 1.0 / %t;
                %startX = $hW + ( %LocalPointStorage.x * %invT);
                %startY = $hH + (  %LocalPointStorage.y * %invT);

                // DrawLine(%startX, %startY, %screenPos_X, %screenPos_Y, RAYWHITE);
                SDL_RenderLine(%renderer,%startX, %startY, %screenPos_X, %screenPos_Y );
            }
        }
        else
        {
            %radius = mLerp(3.0, 1.0, %LocalPointStorage.z);
            // DrawCircle(%screenPos_X, %screenPos_Y, %radius, RAYWHITE);
            SDL_RenderRectF(%renderer, %screenPos_X, %screenPos_Y, %radius,%radius, true);
        }

        %LocalPointStorage.storePoint(%i); //this update the point storage from .x,.y,.z
    }
......
}

```

In the Loop i do `%LocalPointStorage.fetchPoint(%i);` now i can work with the base
fields of the PointStorage(*1) .x,.y.z. At the end i write it back to the
index %i: `%LocalPointStorage.storePoint(%i);`.


(*1) transfer to local variables with getX/Y/Z and a setPoint to writeback is 
maybe faster. Did not test it so far. 

[The full StarField Script (BaseElf Version)](https://github.com/ohmtal/ElfScript/blob/main/BaseElf/assets/modules/StarField.elf) 

[SpeedTest i did to optimize the code](https://github.com/ohmtal/ElfScript/blob/main/docu/BetterSpeedTest.md)

---

[Back to Main](./Main.md)

