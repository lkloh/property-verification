property-verification
=====================
     
MacOS
-----

Download gmp-6.0.0 and put it somewhere where you can find the folder. It can't be installed properly yet on Mac for some reason (let me know if you can!)

To run the codes, put `rapidnet` somewhere on your folder. cd into `property-verification`. To run `test-program.cc`, type

    g++ -I/Users/lkloh/Downloads/cvc4-1.4/src/smt linear_arith.cc -o linear_arith -lcvc4
    
Resources
---------
* [CVC4 C++ Library - examples](http://church.cims.nyu.edu/wiki/Tutorials#linear_arith)
* [API Documentation](http://cvc4.cs.nyu.edu/doc/index.html)

Installing CVC4
---------------

Install the dependency [gmplib](https://gmplib.org/) with superuser privilege for EVERY command, or there may be trouble. 

Install cvc4. First get the ANTLR3 library by running

    sh contrib/get-antlr-3.4
    
Follow the instructions that appear and the run 

    ./configure --with-antlr-dir=/Users/lkloh/property-verification/antlr-3.4 ANTLR=/Users/lkloh/property-verification/antlr-3.4/bin/antlr3
    
    sudo make 
    
    sudo make check
    
    sudo make install

Running the examples
--------------------

Inside cvc4-1.4, run 

    sudo make examples
    
Find the executables in

    cvc4-1.4/builds/examples/api
    
Only works on my current environment for the time being, till I fix this. If you really want to try it now, run

     sudo make clean
     
and follow the cvc4 instructions within the folder.

Run everything with superuser privilege, or trouble may occur. 


Install CVC4
------------

Run testing

    g++ -std=c++11 testing.cpp -o testing -lcvc4

Run linear_arith.cpp

    g++ -std=c++11 linear_arith.cpp -o linear_arith -lcvc4

6) Using the data-structures
----------------------------

`-I`: indicates the location of the header files

`-l`: indicates we want to use the CVC4 libraries

`-L`: where the implementation of the header files can be found

    Run g++ -I<path-to-rapidnet>/src/rapidnet-compiler -<path-to-rapidnet>/lkloh/rapidnet/src/rapidnet-compiler <file-using-data-structures>.cc -o <file-using-data-structures> -lcvc4

Example: I clone this folder to my home directory, with path `/home/lkloh`. To run the file `hellpeworld.cpp` which sources the data structure files from rapidnet, I cd into the location where `helloworld.cpp` may be found, and type:

    g++ -I/home/lkloh/rapidnet/src/rapidnet-compiler -L/home/lkloh/rapidnet/src/rapidnet-compiler helloworld.cpp -o helloworld -lcvc4

7) Remove DS_Store before pushing.
----------------------------------

At the top level of the repository, type

    find . -name .DS_Store -print0 | xargs -0 git rm -f --ignore-unmatch
    
As suggested [here](http://stackoverflow.com/questions/107701/how-can-i-remove-ds-store-files-from-a-git-repository).
