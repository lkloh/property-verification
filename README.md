property-verification
=====================
     
Install CVC4 on MacOS
---------------------

1. Get [GMP](https://gmplib.org/) and install it. 
2. [Get native CVC4](http://cvc4.cs.nyu.edu/downloads/)
2. [Installing CVC4 to run examples](http://cvc4.cs.nyu.edu/wiki/Building_CVC4_from_source)

To run the codes, put `rapidnet` somewhere on your folder. cd into `property-verification`. To run `test-program.cc`, type

    g++ -std=c++11 [FILE-TO-RUN].cc -L/ -o [FILE-TO-RUN] -lcvc4 -lgmp
   
    
Resources
---------
* [CVC4 C++ Library - examples](http://church.cims.nyu.edu/wiki/Tutorials#linear_arith)
* [API Documentation](http://cvc4.cs.nyu.edu/doc/index.html)
* [Call SMT-LIB using Z3](http://stackoverflow.com/questions/8958524/how-to-invoke-z3-on-an-input-file)

Status
------------
* Most likely deleting the stuff in CVC4-parse soon
* To be done: write SmtLib-parse
* Just copy and paste smt-lib into cvc4, after you startup CVC4 using `cvc4 --lang smt`

