/*********************                                                        */
/*! \file linear_arith.cpp
 ** \verbatim
 ** Original author: Tim King
 ** Major contributors: none
 ** Minor contributors (to current version): Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2014  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A simple demonstration of the linear arithmetic capabilities of CVC4
 **
 ** A simple demonstration of the linear arithmetic solving capabilities and
 ** the push pop of CVC4. This also gives an example option.
 **/

#include <iostream>
#include <cvc4/cvc4.h> 
#include "rapidnet_compiler/sdn-formula.h"
#include "rapidnet_compiler/sdn-formula-to-cvc4.h"

using namespace std;
using namespace CVC4;

/* 
 * check that 
 * 3+4 = 7 is VALID 
 */
void testIntegersArithmetic() {
    ExprManager em;
    SmtEngine smt(&em);

    /* RAPIDNET */
    IntVal three = IntVal(3);
    IntVal four = IntVal(4);
    IntVal seven = IntVal(7);

    Arithmetic three_plus_four_rapidnet = Arithmetic(Arithmetic::PLUS, &three, &four); 
    Constraint three_plus_four_equals_seven_rapidnet = Constraint(Constraint::EQ, &three_plus_four_rapidnet, &seven);

    /* CVC4*/
    Expr three_plus_four_cvc4 = parseTerm(&em, &three_plus_four_rapidnet);
    Expr three_plus_four_equals_seven_cvc4 = parseFormula(&em, &three_plus_four_equals_seven_rapidnet); 

    /* CHECKING PARSING */
    std::cout << "\n" << three_plus_four_equals_seven_cvc4 << " is: " << smt.query(three_plus_four_equals_seven_cvc4) << std::endl;

    clearAllVariables();
}



int main() {
    testIntegersArithmetic();
    return 0;
}








