# I C Whitespace
A C-based interpreter for the esoteric programming language Whitspace. Can be
compiled and run on Windows, Mac, and Linux, then used to interpret and run
programs written in Whitespace.

## Installation
I C Whitespace requires that GCC be installed on the system before compiling.

- [Installing GCC on Windows](http://sourceforge.net/projects/mingw/files/latest/download?source=files)
- [Installing GCC on Mac](https://wiki.helsinki.fi/display/HUGG/Installing+the+GNU+compilers+on+Mac+OS+X)

Once GCC is installed on your system and the latest version of this repo
downloaded, unzip the repo and open the resulting folder using the terminal
or command prompt. Type `make` and your computer do the rest.

If nothing happens except a couple lines appear in the window, awesome. You
should be able to see a new file called `whitespace` in your directory, or
`whitespace.exe` if you are using a Windows machine. Once terminal prompts you
for another command, you're ready to start running your own Whitespace programs.

## Usage
You will use the same terminal/console window that you used to compile I C
Whitespace to run I C Whitspace. Usage for I C Whitespace is as follows:

    whitespace <your whitespace file>

For example, to run a whitespace program stored in file `hello.ws`, type

    whitespace hello.ws

and hit enter/return. I C Whitespace will open your file and attempt to
interpret it. If an error occurs, a message will be displayed in your console
and I C Whitespace will quit. If I C Whitespace successfully compiles your code,
it will attempt to run it. If any runtime errors occur, then a message will be
displayed to the console and I C Whitespace will quit.

If you don't see an error message and are greeted with another prompt, then
congratulations; your code compiles and runs without errors. Good for you!

## References
- [Whitespace website](http://compsoc.dur.ac.uk/whitespace/)
