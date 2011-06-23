## Launchpad Projects

This are simple Launchpad projects to get stuff starting. I think it's somewhat difficult to get functional code in the internet, so I want to do my part by creating simple projects using various techniques.

Sergio Campama 2011 
http://kaipi.me

Made for the TI MSP430 LaunchPad 
http://ti.com/launchpadwiki

Released under the Beerware License 
http://en.wikipedia.org/wiki/Beerware

----

### Dependencies

To compile the binaries, I assume that mspgcc is installed. If it's not, check out the last part of this README. It's not trivial.

To install the binaries on the Launchpad I use `mspdebug`, which is fairly easy to install, and can be found in [here](http://mspdebug.sourceforge.net/)

For the build system, I use Rake, so you'll need ruby and rake installed. After you have that, you can call `rake` and the project will compile into the final binary. The cool thing about it is that it automatically checks for all *.c files inside the 'src' folder, so if you want to create your own apps, just add the files and it'll work. Also, when compiling, all the 'src' subfolders are added to the header includes, so you can just '#include "anywhere.h"' and it'll also work. Just be sure to not conflict with system header files. Instructions on how to install the binary on the Launchpad can be found running `rake install`. I plan on fixing this to actually install it, but I still have to figure out how to do it without entering the mspdebug console.

----

### Licence

Released under the [Beerware License](http://en.wikipedia.org/wiki/Beerware). That means you can
do whatever the heck you want with it. If you find it useful and you run into me someday, you're
welcome to buy me a beer if you'd like.

----

### MSPGCC install instructions

This was tested on a new install of Ubuntu Server 11.04 64 bits. It's a server because I run it on a VMWare Virtual Machine, and I only connect to it via `ssh` and Mac's 'Connect to...' system. So basically I develop on the comfort of my mac, while having Ubuntu's flexibility for compiling.

This are the instructions for building MSPGCC 3, not 4, which is apparently a little more advanced, but this works for me, so this is what I use. If it works, don't fix it.

####gcc-3.4 installation

First, we need gcc-3.4 because this will compile mspgcc's tools. To do this run the following commands

`cd`

`mkdir DELEME`

`cd DELEME`

`wget http://ge.archive.ubuntu.com/ubuntu/pool/universe/g/gcc-3.4/libstdc++6-dev_3.4.6-6ubuntu5_amd64.deb`

`wget http://ge.archive.ubuntu.com/ubuntu/pool/universe/g/gcc-3.4/cpp-3.4_3.4.6-6ubuntu5_amd64.deb`

`wget http://ge.archive.ubuntu.com/ubuntu/pool/universe/g/gcc-3.4/gcc-3.4_3.4.6-6ubuntu5_amd64.deb`

`wget http://ge.archive.ubuntu.com/ubuntu/pool/universe/g/gcc-3.4/gcc-3.4-base_3.4.6-6ubuntu5_amd64.deb`

`wget http://ge.archive.ubuntu.com/ubuntu/pool/universe/g/gcc-3.4/g++-3.4_3.4.6-6ubuntu5_amd64.deb`

`sudo dpkg -i *.deb`

`cd ..`

`rm -rf DELEME`

With this, you SHOULD have a working gcc-3.4. Maybe, if you're as unlucky as me, you'll have a non working version of gcc-3.4, which can be tested with this code

`cd`

`mkdir DELEME`

`cd DELEME`

`echo "int main(){return 0;}" >> temp.c`

`gcc-3.4 temp.c`

`cd ..`

`rm -rf DELEME`

If you have a '-lgcc_s' not found error, then you'll have to run this others commands too.

`sudo rm -rf /usr/lib/gcc/x86_64-linux-gnu/3.4.6/libgcc_s.so`

`sudo ln /lib/x86_64-linux-gnu/libgcc_s.so.1 /usr/lib/gcc/x86_64-linux-gnu/3.4.6/libgcc_s.so`

####mspgcc actual building

So now we will be building mspgcc from scratch. First, we will need some dependencies

`sudo apt-get install cvs patch ncurses-dev`

Now, we'll get the source code and compile it

`cvs -d:pserver:anonymous@mspgcc.cvs.sourceforge.net:/cvsroot/mspgcc login`

`cvs -z3 -d:pserver:anonymous@mspgcc.cvs.sourceforge.net:/cvsroot/mspgcc co -P .`

`cd packaging`

`make folders`

`CC=gcc-3.4 make build`

This last step can take some time, and it won't show any output in the mean time. But don't worry, it will end. After it's finished, we need to make the new binaries accessible, so we'll add the following lines at the end of `~/.bashrc`

```#MSPGCC binaries
export PATH=$PATH:/opt/mspgcc/bin
```

Finally, to be able to use mspgcc without restarting the terminal, we source that last file

`source ~/.bashrc`

Now you should have `msp430-gcc` ready to go.