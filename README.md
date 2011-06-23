## Launchpad Projects

This are simple Launchpad projects to get stuff starting. I think it's somewhat difficult to get functional code in the internet, so I want to do my part by creating simple projects using various techniques.

----

### Dependencies

To compile the binaries, I assume that mspgcc is installed. If it's not, check out the last part of this README. It's not trivial.

To install the binaries in the Launchpad I use `mspdebug`, which is fairly easy to install, and can be found in [here](http://mspdebug.sourceforge.net/)

For the build system, I use Rake, so you'll need ruby and rake installed. After you have that, you can call `rake` and the project will compile into the final binary. The cool thing about it is that it automatically checks for all *.c files inside the 'src' folder, so if you want to create your own apps, just add the files and it'll work. Also, when compiling, all the 'src' subfolders are added to the header includes, so you can just '#include "anywhere.h"' and it'll also work. Just be sure to not conflict with system header files. Instructions on how to install the binary on the Launchpad can be found running `rake install`. I plan on fixing this to actually install it, but I still have to figure out how to do it without entering the mspdebug console.

----

### Licence

Released under the [Beerware License](http://en.wikipedia.org/wiki/Beerware). That means you can
do whatever the heck you want with it. If you find it useful and you run into me someday, you're
welcome to buy me a beer if you'd like.

----

### MSPGCC install instructions

This was tested on a new install of Ubuntu Server 11.04 64 bits. It's a server because I run it on a VMWare Virtual Machine, and I only connect to it via `ssh` and Mac's 'Connect to...' system. So basically I develop on the comfort of my mac, while having Ubuntu's flexibility for compiling

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