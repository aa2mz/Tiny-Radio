# Software

## Building software
Software is built with the Arduino IDE.
Copy all the files from the core directory
and one of the .ino files from the examples directory
into a directory on your development machine.
The IDE will require that the name of your code directory
be the same as the base part of the name of your .ino file.

## All the core files are .h files ???
Yes. I have had problems linking large projects that contain many .cpp files
so I include all the modules by placing them in .h files.
The effect is the the application looks like one big source file to the linker
but it still allows me to develop and maintain them as individual modules.

## Model
Major interfaces are defined as virtual classes that other classes know how to communicate with.
This allows subsitution of services and implementation at comlile time and sometimes run-time. 
For example, there is a virtual class named "RadioClocks" which class Si5351Clocks implements.
If, in the future, someone implements a clock class from those AD-things and that classs= implements the
RadioClocks interface, then nothing else in the application need to be changed to use the new clock chipps.

Another interface class, CatRadio, exist between the different serial protocols and the class Radio.
You can totally throw out my Radio implemenation and if yours implements the "CatRadio" interface
then it will work with the other parts of the app. 
This would be the way to go if you were wanted to use the ICom CI-V protocol in your own applications.
But...

## Coperative Multitasking

The application consists of many stateful tasks that are run by the "tinytasker.h" module.
You will see that many classes inherit from ***"Taskable". ***

Taskable routines check for any input or state changes that they can use, use it, then return.
A simple state variable is inherited from Taskable, though many tasks store  additional data in class variable.

The advantage of using tasks is that tasks don't need to know where they came from or that chore comes next.
Think of tasks as being many small arduino programs that each have they own setup() and loop() calls.
They do what they have to do and don't mind other tasks are doing.

### Taskable class initializer
The class initializer of, optionally, and init() method performs once-per-power-up actions.

### int setup(int ms=4)
A setup routine is used restart a task timing on the fly.
Timing tells the TinyTasker how often to call the task's "loop" method.
For things like checking the state of buttons, the time interval isn't likely to change.
You will see and goood example of changing the call0back time in the keyer as it times CW elements.

There is no predefined use of the return value; however you may use it for your own purposes.

For most tasks, a four millisecond period is fast enough. 
Tasks that interact with humans input can usually occur on a 20ms or 40ms interval.

### int loop(int arg=0)

After at least the number of milliseconds specified in setup, the default is four, the classes'
loop() methord is called. The class will do whatever can be done at that moment and then 
store any state information needed for the next time it is called. 
The Keyer class and the serial protocal classes rely heavily on stored state information
and you will see big switch(state) statements the act on the stored state.

There is no predefined use of the return value; however you may use it for your own purposes.

## building an application

### nothing new
"new" for classes is not generally used on Arduino as it has poor memory managment.
Instead, a global instance usually created by giving it a name after the class definition.
Pointers to classes can still be passed to other classes; 
you will see this done as a "radio" is passed to as serial protocall as part of the application startup.

### the actual Arduino setup() routine

You will see startup and EProm recovery code in setup() then task initialization.
Most start up parameters are read from EProm.
Then the tasks are added to the TinyTasker incedence using "tinyTasker.add()"
That's pretty much it.
Using TinyTasker actually made the application very simple, see the
Arduino loop() in any example .ino file to see how easy.
