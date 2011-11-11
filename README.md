## Launchpad Projects

This are simple Launchpad projects to get stuff going. I think it's somewhat difficult to get functional code in the internet, so I want to do my part by creating simple projects using various techniques.
These projects are tested on the mspgcc uniarch releases. With this guide and a $4.30 Launchpad, there's no excuse not to try out embedded coding!

Sergio Campama 2011 
http://kaipi.me

Made for the TI MSP430 LaunchPad 
http://ti.com/launchpadwiki

Released under the Beerware License 
http://en.wikipedia.org/wiki/Beerware

----

### Dependencies

To compile the binaries, I assume that mspgcc uniarch is installed. If it's not, check out the last part of this README. It's not trivial.

To install the binaries on the Launchpad I use `mspdebug`, which is fairly easy to install, and can be found in [here](http://mspdebug.sourceforge.net/). The current working version is 0.18.

For the build system, I use Rake, so you'll need ruby and rake installed. After you have that, you can call `rake` and the project will compile into the final binary. The cool thing about it is that it automatically checks for all *.c files inside the 'src' folder, so if you want to create your own apps, just add the C files and it'll work. Also when compiling, all the 'src' subfolders are added to the header includes, so you can just '#include "path/to/anywhere.h"' and it'll also work. Installation into the Launchpad is as easy as `rake install`. More info can be found running `rake help`

----

### Licence

Released under the [Beerware License](http://en.wikipedia.org/wiki/Beerware). That means you can
do whatever the heck you want with it. If you find it useful and you run into me someday, you're
welcome to buy me a beer if you'd like.

----

### MSPGCC 'Uniarch'

Uniarch is an initiative to unify all the work left behind by mspgcc3 and mspgcc4, while also separating the boundaries between the compiler and TI header files. This 'branch' of mspgcc is the most advanced one, having over 300 supported MSP430 devices.

These instructions are highly based on [this article](http://sourceforge.net/apps/mediawiki/mspgcc/index.php?title=Install:fromsource) with some minor modifications. So, in our home, we will create a workspace to build our binaries

	cd
	mkdir -p msp430
	cd msp430

We will need the following dependencies to build mspgcc. I don't really know which ones are really needed, but with this ones it works, and I'm lazy.

	sudo apt-get install patch ncurses-dev build-essential bison flex libgmp3-dev libmpfr-dev libmpc-dev texinfo zlib1g-dev
	
For some reason, my machine (Ubuntu Server 11.04) didn't recognize mpfr, gmp and mpc, so these are instructions on building them manually, which will install them on `/usr/local/`

For GMP:

	wget ftp://ftp.gmplib.org/pub/gmp-5.0.2/gmp-5.0.2.tar.bz2
	tar xvfj gmp-5.0.2.tar.bz2
	mkdir gmp
	cd gmp
	../gmp-5.0.2/configure
	make
	sudo make install
	cd ..

For MPFR:

	wget http://www.mpfr.org/mpfr-current/mpfr-3.1.0.tar.bz2
	tar xvfj mpfr-3.1.0.tar.bz2
	mkdir mpfr
	cd mpfr
	../mpfr-3.1.0/configure
	make
	sudo make install
	cd ..
	
For MPC:

	wget http://www.multiprecision.org/mpc/download/mpc-0.9.tar.gz
	tar xvfz mpc-0.9.tar.gz
	mkdir mpv
	cd mpv
	../mpc-0.9/configure
	make
	sudo make install
	cd ..

After installing those libs, add these lines to your `~/.bashrc` file, and source it. Again, this worked for me, but maybe you won't need it. 
	
	#Adding /usr/local/lib to LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

So, now that we have that running, we'll fetch all the latest files from the mspgcc project. As of this writing, the latest release was 20110716, with Long Term Support

	wget http://sourceforge.net/projects/mspgcc/files/mspgcc/mspgcc-20110716.tar.bz2
	tar xvfj mspgcc-20110716.tar.bz2
	
Since this release is a LTS version, there will be many patches fixing various bugs. We will now download all the latests patches into a folder called `patches`

	mkdir patches
	cd patches
	
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-binutils-2.21.1-20110716-sf3143071.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-binutils-2.21.1-20110716-sf3386145.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-binutils-2.21.1-20110716-sf3379341.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-binutils-2.21.1-20110716-sf3400711.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-binutils-2.21.1-20110716-sf3400750.patch

	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-gcc-4.5.3-20110706-sf3370978.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-gcc-4.5.3-20110706-sf3390964.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-gcc-4.5.3-20110706-sf3394176.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-gcc-4.5.3-20110706-sf3396639.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-gcc-4.5.3-20110706-sf3409864.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-gcc-4.5.3-20110706-sf3417263.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-gcc-4.5.3-20110706-sf3431602.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-gcc-4.5.3-20110706-sf3433730.patch

	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-libc-20110612-sf3387164.patch
	wget http://sourceforge.net/projects/mspgcc/files/Patches/LTS/20110716/msp430-libc-20110612-sf3402836.patch
	
	cd ..

Now we will need to download, patch, compile and install the specific versions of binutils, gcc and gdb that will be used in mspgcc.
The bin directory will reside in /usr/local/bin for this instructions. 

	wget http://ftpmirror.gnu.org/binutils/binutils-2.21.1a.tar.bz2
	tar xvfj binutils-2.21.1a.tar.bz2
	cd binutils-2.21.1
	#Bring it to mspgcc
	patch -p1 < ../mspgcc-20110716/msp430-binutils-2.21.1-20110716.patch
	#Apply all the patches
	patch -p1 < ../patches/msp430-binutils-2.21.1-20110716-sf3143071.patch
	patch -p1 < ../patches/msp430-binutils-2.21.1-20110716-sf3379341.patch
	patch -p1 < ../patches/msp430-binutils-2.21.1-20110716-sf3386145.patch
	patch -p1 < ../patches/msp430-binutils-2.21.1-20110716-sf3400711.patch
	patch -p1 < ../patches/msp430-binutils-2.21.1-20110716-sf3400750.patch
	cd ..
	mkdir -p BUILD/binutils
	cd BUILD/binutils
	../../binutils-2.21.1/configure --target=msp430 --program-prefix="msp430-" --with-mpfr-include=/usr/local/include -with-mpfr-lib=/usr/local/lib --with-gmp-include=/usr/local/include -with-gmp-lib=/usr/local/lib --with-mpc-include=/usr/local/include -with-mpc-lib=/usr/local/lib
	make
	sudo make install
	cd ../..

That should have the mspgcc binutils running. Now to gcc...

	wget wget http://ftpmirror.gnu.org/gcc/gcc-4.5.3/gcc-core-4.5.3.tar.bz2
	tar xvfj gcc-core-4.5.3.tar.bz2
	cd gcc-4.5.3
	#Update to mspgcc
	patch -p1 < ../mspgcc-20110716/msp430-gcc-4.5.3-20110706.patch
	#Apply all the patches
	patch -p1 < ../patches/msp430-gcc-4.5.3-20110706-sf3370978.patch
	patch -p1 < ../patches/msp430-gcc-4.5.3-20110706-sf3390964.patch
	patch -p1 < ../patches/msp430-gcc-4.5.3-20110706-sf3394176.patch
	patch -p1 < ../patches/msp430-gcc-4.5.3-20110706-sf3396639.patch
	patch -p1 < ../patches/msp430-gcc-4.5.3-20110706-sf3409864.patch
	patch -p1 < ../patches/msp430-gcc-4.5.3-20110706-sf3417263.patch
	patch -p1 < ../patches/msp430-gcc-4.5.3-20110706-sf3431602.patch
	patch -p1 < ../patches/msp430-gcc-4.5.3-20110706-sf3433730.patch
	cd ..
	mkdir -p BUILD/gcc
	cd BUILD/gcc
	../../gcc-4.5.3/configure --target=msp430 --enable-languages=c --program-prefix="msp430-" --with-mpfr-include=/usr/local/include -with-mpfr-lib=/usr/local/lib --with-gmp-include=/usr/local/include -with-gmp-lib=/usr/local/lib --with-mpc-include=/usr/local/include -with-mpc-lib=/usr/local/lib
	make
	sudo make install
	cd ../..

Done with gcc, on to gdb

	wget http://ftpmirror.gnu.org/gdb/gdb-7.2a.tar.bz2
	tar xvfj gdb-7.2a.tar.bz2
	cd gdb-7.2
	#Update to mspgcc
	patch -p1 < ../mspgcc-20110716/msp430-gdb-7.2-20110103.patch
	#No patches for this one
	cd ..
	mkdir -p BUILD/gdb
	cd BUILD/gdb
	../../gdb-7.2/configure --target=msp430 --program-prefix="msp430-" --with-mpfr-include=/usr/local/include -with-mpfr-lib=/usr/local/lib --with-gmp-include=/usr/local/include -with-gmp-lib=/usr/local/lib --with-mpc-include=/usr/local/include -with-mpc-lib=/usr/local/lib
	make
	sudo make install
	cd ../..

Perfect, so all that's left is the microcontroller definitions that are part of msp430mcu and libc that is part of msp430-libc. This files are not downloaded yet (they're not inside the mspgcc package downloaded before, just version references which may be outdated)

	wget http://sourceforge.net/projects/mspgcc/files/msp430mcu/msp430mcu-20111107.tar.bz2
	tar xvfj msp430mcu-20111107.tar.bz2
	cd msp430mcu-20111107
	sudo MSP430MCU_ROOT=`pwd` ./scripts/install.sh /usr/local/ 
	cd ..

Now we need to download, patch, compile and install libc for mspgcc.

	wget http://sourceforge.net/projects/mspgcc/files/msp430-libc/msp430-libc-20110612.tar.bz2
	tar xvfj msp430-libc-20110612.tar.bz2
	cd msp430-libc-20110612
	patch -p1 < ../patches/msp430-libc-20110612-sf3387164.patch
	patch -p1 < ../patches/msp430-libc-20110612-sf3402836.patch
	cd src
	make
	sudo make PREFIX=/usr/local install
	cd ../..

And that's it! You can compile the projects now. 

Now, to install `mspdebug`, follow this instructions

	wget http://sourceforge.net/projects/mspdebug/files/mspdebug-0.18.tar.gz
	tar xvfz mspdebug-0.18.tar.gz
	cd mspdebug-0.18
	#make doesn't work for me unless I specify the WITHOUT_READLINE part
	make WITHOUT_READLINE=1
	sudo make install
	cd ..

If your still having problems, maybe this links can help you. (For now there's only one, but as they appear on the interwebs I will post them here)
	[JustinTech](http://justinstech.org/2011/06/msp430-uniarch-mspgcc-or-mspgcc4/)

###MSPDEBUG USB permissions

Now, having mspdebug installed with the instructions from before, connect your Launchpad to Ubuntu and run `mspdebug rf2500`. If it doesn't work, don't panic, follow the next steps (based on this [article](http://karuppuswamy.com/wordpress/2010/10/07/debugging-ez430-chronos-with-mspdebug-tool-in-ubuntu-linux/).).

First, as root, add `ATTRS{idVendor}=="0451", ATTRS{idProduct}=="f432", MODE="0660", GROUP="plugdev"` to `/etc/udev/rules.d/71-persistent-msp430.rules`. Then run this commands:

	sudo addgroup YOUR_USERNAME plugdev
	sudo restart udev
	
Now unplug and replug the Launchpad and try again and it should work.