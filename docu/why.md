# Why ?

***The Story why i did it. by XXTH 2026-08-25***

When I started using TorqueScript over 20 years ago ( GarageGame's Torque Game Engine 1.4)
I did hate it (because of all the parse errors) until I started to love it ;)
I had some experience in the "goto" Basic (Apple / Atari XL), Pascal, PHP , 
C but it was my first interpreted language (after php) which I can alter
while it's running. 

### Why it's hot
Changing variables at runtime or write a function while it's running to fix a 
bug is more than a hot reload it's hot writing while running. Which I used a lot 
in my game Auteria while the server with players is active running. 

### Ohmflux 
I worked a lot on my Ohmflux project(s) and missed a scripting language. I 
started to embed lua which is not bad with sol2, but i dont like the lua syntax.
Than i tried the C-like chaiScript which is easy to embed but since i work on a 
slow laptop the linking times was too long for me. I went back to lua, but as I 
said: for me with it's the syntax - i don't feel at home. V8 was too much work 
to embed and so my old idea to add TorqueScript got hot again. 

### KorkScript
Some years (?) ago I first tried KorkScript but it was not usable. Some month ago
I saw James had made some progress and I started to test it. He rewote the parser
and VM (impressive work :)) and i think he plugged in the SimObject (World) after 
or meanwhile that. So i start to bind it to Ohmflux and it works not bad. There
was some rough-edged but as he write on his github page that it's not finished.

### Porting from Torque3D
After some time using KorkScript i missed the new stuff from Torque3D I worked 
with, when porting parts of my source from Auteria/TGE to Torque3D. I had 
started extracting TorqueScript some times over the years but now i became 
**subborn** and did not stop until it started to show up a heartbeat. 

### Elfscript was born
I had created some Ohmflux bindings with KorkScript and so i ported this to 
the stand alone TorqueScript. I thought about a name and since Kork is in use 
*hehe* I remembered the Elf which was in TGE 1.5 - I think. Finding a name for 
a project is sometimes more difficult than writing it - So I decided to call it 
ElfScript which is a mythical creature and the number eleven in german. 

### raylib-ElfScript
I was curious about raylib and had played a bit with it. Since there are so many
bindings to it, I thought - Hey! raylib-ElfScript. Porting it was not a easy 
task - i did not want to add everything as a class and wanted to port it 1:1 
when possible. I invented my manager to map the pointers to id's and for a soft 
garbagecollection.

### So slow 
When porting Demos to the just created raylib bindings I thought - hey yes 
TorqueScript is not the fastest but I love it. But why not start trying to make 
it better. So i took lua as a benchmark and started to find out what I can do. 
The biggest problem of Torque3D is, it does a lot of number to string and then 
back to number and maybe again back to a string again conversations. So this was 
my first part to apply levers. 

### Many construction sites
In compare to TGE's TorqueScript the Torque3D Version is more modern and 
enhanced. But most of the code is still like it was in "Ur" TGE TorqueScript 
version. 
So i realized i had to look at:
- local variables
- global variables
- static fields <<< this was the slowest part of all
- dynamic fields 

Looking at the OP-Codes of the virual machine most times it call load string and
save string. The ConsoleValue which hold the data is good but it does not 
use it potential. I don't know how many nights i have spend to get it in the 
state it's now is. 

### Version 0.6 close to lua performace in my "local var test"
I just tagged the ElfScript Version 0.6 which I proudly can say it's now in a 
different league than it was when I started working on it. It's much faster, beat 
many interpreted languages in speed and is close to the best of the rest: Lua.
Sure the performance of lua-jit or V8 (jit) is impossible to reach by a 
interpreted language. But for a neat, really easy to embed scripting language, 
which is in syntax close to C, php or JavaScript - it's not bad. 

### Version 0.7 performace close to lua in  "local var test" 
About 100ms - i would say: Mission completed :)
