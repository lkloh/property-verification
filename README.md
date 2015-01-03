property-verification
=====================
     
Install CVC4 on MacOS
---------------------

1. Get [GMP](https://gmplib.org/) and install it. 
2. [Get native CVC4](http://cvc4.cs.nyu.edu/downloads/)
2. [Installing CVC4 to run examples](http://cvc4.cs.nyu.edu/wiki/Building_CVC4_from_source)

To run the codes, put `rapidnet` somewhere on your folder. cd into `property-verification`. To run `test-program.cc`, type

    g++ -std=c++11 -I[path-to-rapidnet-folder]/rapidnet/src/rapidnet-compiler/sdn-formula.h -L[path-to-rapidnet-folder]/rapidnet/src/rapidnet-compiler [FILE-TO-RUN].cc -L/ -o [FILE-TO-RUN] -lcvc4
    
On my environment, that would be
    
    g++ -std=c++11 -I/Users/lkloh/rapidnet/src/rapidnet-compiler/sdn-formula.h -L/Users/lkloh/rapidnet/src/rapidnet-compiler linear_arith.cpp -o linear_arith -lcvc4
    
for example
    
Resources
---------
* [CVC4 C++ Library - examples](http://church.cims.nyu.edu/wiki/Tutorials#linear_arith)
* [API Documentation](http://cvc4.cs.nyu.edu/doc/index.html)



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


Running examples in property-verification
-----------------------------------------

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
