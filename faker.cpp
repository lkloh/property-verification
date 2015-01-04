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

/* check that 
 * (x=y) and (y=4)
 * IMPLIES (x=4)
 */
void testVariables() {
    ExprManager em;
    SmtEngine smt(&em);

    /* RAPIDNET */
    Variable x_rapidnet = Variable(Variable::INT, false);
    Variable y_rapidnet = Variable(Variable::INT, false);
    IntVal four_rapidnet = IntVal(4);

    Constraint x_equals_y_rapidnet = Constraint(Constraint::EQ, &x_rapidnet, &y_rapidnet);
    Constraint y_equals_4_rapidnet = Constraint(Constraint::EQ, &y_rapidnet, &four_rapidnet);
    Constraint x_equals_4_rapidnet = Constraint(Constraint::EQ, &x_rapidnet, &four_rapidnet);

    /* CVC4 */

    //declare variables
    Expr x_equals_y_cvc4 = parseFormula(&em, &x_equals_y_rapidnet);
    Expr y_equals_4_cvc4 = parseFormula(&em, &y_equals_4_rapidnet);
    Expr x_equals_4_cvc4 = parseFormula(&em, &x_equals_4_rapidnet);

    smt.assertFormula(y_equals_4_cvc4);
    smt.assertFormula(x_equals_4_cvc4);
    smt.push();

    /* CHECKING PARSING */
    std::cout << "\n" << y_equals_4_cvc4 << " and " << x_equals_4_cvc4 << " implies that: " << x_equals_4_cvc4 << " is: " << smt.query(x_equals_4_cvc4) << std::endl;

    clearAllVariables();
}

/*
 * FORALL x, (x=3)
 * Should be INVALID
 */
void testBoundVariables() {
    ExprManager em;
    SmtEngine smt(&em);

    /* rapidnet */

    IntVal three = IntVal(3);
    Variable x = Variable(Variable::INT, true);

    vector<Variable*> boundVarList;
    boundVarList.push_back(&x);

    Constraint x_equals_3 = Constraint(Constraint::EQ, &x, &three);

    Quantifier forall_x__x_equals_3_rapidnet = Quantifier(Quantifier::FORALL, boundVarList, &x_equals_3);

    /* CVC4 */
    Expr forall_x__x_equals_3_cvc4 = parseFormula(&em, &forall_x__x_equals_3_rapidnet);

    /* check smt */
    std::cout << "\nTest " << forall_x__x_equals_3_cvc4 << " is: "<< smt.query(forall_x__x_equals_3_cvc4) << std::endl;

    clearAllVariables();
}


int main() {
    testIntegersArithmetic();
    testVariables();
    testBoundVariables();
    return 0;
}








