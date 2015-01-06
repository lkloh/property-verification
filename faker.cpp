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
#include "rapidnet_compiler/sdn-formula.cc"
#include "rapidnet_compiler/sdn-formula-to-cvc4.h"

using namespace std;
using namespace CVC4;

















/*
 * Function
 * --------
 * mother(x): returns the mother of x
 *
 * Predicate
 * ---------
 * YOUNGER(x,y): x is younger than y
 *
 * ASSERT forall x, YOUNGER(x, mother(x))
 * (everyone is younger than his/her mother)
 * 
 * QUERY YOUNGER("MaliaObama", mother("MaliaObama"))? //should be true
 */
void nested_function_check() {
    ExprManager em;
    SmtEngine smt(&em);

    std::cout << "\nNested Function Check" << std::endl;

    /* ---------------------------- RAPIDNET ------------------------------ */
    //bound vars
    Variable child = Variable(Variable::STRING, true); 

    //mother schema
    vector<Variable::TypeCode> mother_domain_types;
    mother_domain_types.push_back(Variable::STRING);
    FunctionSchema mother_schema = FunctionSchema("mother", mother_domain_types, Variable::STRING);

    //mother UserFunction
    vector<Term*> mother_args;
    mother_args.push_back(&child);
    UserFunction mother_function = UserFunction(&mother_schema, mother_args);

    //YOUNGER schema
    vector<Variable::TypeCode> younger_domain_types;
    younger_domain_types.push_back(Variable::STRING);
    younger_domain_types.push_back(Variable::STRING);
    PredicateSchema YOUNGER_schema = PredicateSchema("YOUNGER", younger_domain_types);

    //Younger terms
    vector<Term*> YOUNGER_args;
    YOUNGER_args.push_back(&child);
    YOUNGER_args.push_back(&mother_function);
    PredicateInstance YOUNGER_x_motherx = PredicateInstance(&YOUNGER_schema, YOUNGER_args);

    // forall x, YOUNGER(x, mother(x))
    vector<Variable*> quantifier_bound_vars;
    quantifier_bound_vars.push_back(&child);
    Quantifier forall_x__YOUNGER_x_motherx = Quantifier(Quantifier::FORALL, quantifier_bound_vars, &YOUNGER_x_motherx);

    /* ---------------------------- RAPIDNET ------------------------------ */

    StringVal MaliaObama = StringVal("MaliaObama");

    // mother("MaliaObama")
    vector<Term*> malia_schema_args;
    malia_schema_args.push_back(&MaliaObama);
    UserFunction mother_malia = UserFunction(&mother_schema, malia_schema_args);

    vector<Term*> YOUNGER_malia_args;
    YOUNGER_malia_args.push_back(&MaliaObama);
    YOUNGER_malia_args.push_back(&mother_malia);
    PredicateInstance malia_younger_than_her_mother = PredicateInstance(&YOUNGER_schema, YOUNGER_malia_args);

    /* ---------------------------- CVC4 ------------------------------ */

    Expr forall_x__YOUNGER_x_motherx_cvc4 = parseFormula(&em, &forall_x__YOUNGER_x_motherx);
    Expr malia_younger_than_her_mother_cvc4 = parseFormula(&em, &malia_younger_than_her_mother);

    smt.assertFormula(forall_x__YOUNGER_x_motherx_cvc4);
    std::cout << "Since "<< forall_x__YOUNGER_x_motherx_cvc4 << " therefore " << malia_younger_than_her_mother_cvc4 << " is: " << smt.query(malia_younger_than_her_mother_cvc4) << std::endl;

    clearAllVariables();
}

int main() {
    testBoundVariables();
    testBoundPredicate();
    testArithmeticNestedQuantifier();
    connective__x_gt_y__AND__y_gt_z__IMPLIES__x_gt_z();
    arithmetic__4_plus_3__minus__2_plus_1__equals__4();
    quantifier__predicate__ancestor();
    quantifier__function_child_younger_than_mother();
    nested_function_check(); 
    return 0;
}








