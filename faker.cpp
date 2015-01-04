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

/*
 * CVC4
 * ----
 * isblue: STRING -> BOOLEAN
 * ASSERT isblue("sky")
 * QUERY EXISTS x, isblue(x) 
 */
void testBoundPredicate() {
    ExprManager em;
    SmtEngine smt(&em);

    /* ***************************** rapidnet: make isblue function ****************** */

    vector<Variable::TypeCode> types_rapidnet;
    types_rapidnet.push_back(Variable::STRING);
    PredicateSchema isblue_rapidnet =  PredicateSchema("isblue", types_rapidnet);

    /* ************************ rapidnet: exists x, isblue(x) ************************ */

    //make bound var
    Variable x = Variable(Variable::STRING, true);
    vector<Variable*> boundVarList;
    boundVarList.push_back(&x);

    // make the formula isblue(x)
    // x is a bound variable
    vector<Term*> args;
    args.push_back(&x);

    PredicateInstance isblue_x_rapidnet = PredicateInstance(&isblue_rapidnet, args);

    //make it quantifier
    Quantifier exists_x__isblue_x_rapidnet = Quantifier(Quantifier::EXISTS, boundVarList, &isblue_x_rapidnet);

    /* ************************ rapidnet: isblue("sky") ********************* */

    StringVal sky_string = StringVal("sky");
    vector<Term*> args_sky_rapidnet;
    args_sky_rapidnet.push_back(&sky_string);

    PredicateInstance isblue_sky_rapidnet = PredicateInstance(&isblue_rapidnet, args_sky_rapidnet);


    /* ******************************* CVC4 ******************************** */

    Expr isblue_sky_cvc4 = parseFormula(&em, &isblue_sky_rapidnet);


    Expr exists_x__isblue_x_cvc4 = parseFormula(&em, &exists_x__isblue_x_rapidnet);

    /* **************************** CHECK SMT ******************************** */

    std::cout << "\nSky not blue: "  << exists_x__isblue_x_cvc4 << smt.query(exists_x__isblue_x_cvc4) << std::endl;

    smt.assertFormula(isblue_sky_cvc4);

    std::cout  << "\nSky is blue: " << exists_x__isblue_x_cvc4 << smt.query(exists_x__isblue_x_cvc4) << std::endl;

    clearAllVariables();
}

/*
 * QUERY (FORALL (x:INT): (EXISTS (y:INT): x+y = 4));
 */
void testArithmeticNestedQuantifier() {
    ExprManager em;
    SmtEngine smt(&em);

    /* rapidnet */
    IntVal four = IntVal(4);
    Variable x = Variable(Variable::INT, true);
    Variable y = Variable(Variable::INT, true);

    vector<Variable*> boundVarList_exists;
    boundVarList_exists.push_back(&y);

    vector<Variable*> boundVarList_forall;
    boundVarList_forall.push_back(&x);

    Arithmetic x_plus_y = Arithmetic(Arithmetic::PLUS, &x, &y);
    Constraint x_plus_y_eq_4 = Constraint(Constraint::EQ, &x_plus_y, &four);
    Quantifier exists_y__x_plus_y_eq_4 = Quantifier(Quantifier::EXISTS, boundVarList_exists, &x_plus_y_eq_4);

    Quantifier forall_x__exists_y__x_plus_y_eq_4 = Quantifier(Quantifier::FORALL, boundVarList_forall, &exists_y__x_plus_y_eq_4);

    /* CVC4 */
    Expr forall_x__exists_y__x_plus_y_eq_4__cvc4 = parseFormula(&em, &forall_x__exists_y__x_plus_y_eq_4);

    /* **************************** CHECK SMT ******************************** */

    std::cout << "\n" << forall_x__exists_y__x_plus_y_eq_4__cvc4 << " is: " << smt.query(forall_x__exists_y__x_plus_y_eq_4__cvc4) << std::endl;

    clearAllVariables();
}

int main() {
    testIntegersArithmetic();
    testVariables();
    testBoundVariables();
    testBoundPredicate();
    testArithmeticNestedQuantifier();
    return 0;
}








