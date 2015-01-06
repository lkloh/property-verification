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
 * (4+3)-(2+1) = 4 
 */
void arithmetic__4_plus_3__minus__2_plus_1__equals__4() {
    ExprManager em;
    SmtEngine smt(&em);

    /* RAPIDNET */
    IntVal one = IntVal(1);
    IntVal two = IntVal(2);
    IntVal three = IntVal(3);
    IntVal four = IntVal(4);

    Arithmetic four_plus_three = Arithmetic(Arithmetic::PLUS, &four, &three); 
    Arithmetic two_plus_one = Arithmetic(Arithmetic::PLUS, &two, &one);
    Arithmetic four_plus_three__minus__two_plus_one =  Arithmetic(Arithmetic::MINUS, &four_plus_three, &two_plus_one);
    Constraint equal_sides = Constraint(Constraint::EQ, &four_plus_three__minus__two_plus_one, &four);

    /* CVC4 */
    Expr equal_sides_cvc4 = parseFormula(&em, &equal_sides);

    /* CHECKING PARSING */
    std::cout << "\nTest" << equal_sides_cvc4 << " is: " << smt.query(equal_sides_cvc4) << std::endl;

    clearAllVariables();
}

/* ADAM is the ancestor of everyone 
 * Ancestor("LilyPotter", "HarryPotter") means LilyPotter is an ancestor of HarryPotter
 */
void quantifier__predicate__ancestor() {
    ExprManager em;
    SmtEngine smt(&em);

    /* ***************************** rapidnet: make Ancestor(x,y) ****************** */

    vector<Variable::TypeCode> types_rapidnet;
    types_rapidnet.push_back(Variable::STRING);
    types_rapidnet.push_back(Variable::STRING);
    PredicateSchema ancestor_rapidnet =  PredicateSchema("Ancestor", types_rapidnet);

    /* ********************** rapidnet: forall x, Ancestor("Adam",x) ***************** */

    //make bound var
    StringVal ADAM = StringVal("Adam");
    Variable str1 = Variable(Variable::STRING, true);
    vector<Variable*> boundVarList;
    boundVarList.push_back(&str1);

    // make the formula ancestor("Adam", x)
    // x is a bound variable
    vector<Term*> args;
    args.push_back(&ADAM);
    args.push_back(&str1);

    PredicateInstance ancestor_adam_x = PredicateInstance(&ancestor_rapidnet, args);

    //make it quantifier
    Quantifier forall_x__ancestor_ADAM_x = Quantifier(Quantifier::FORALL, boundVarList, &ancestor_adam_x);

    /* ********************** rapidnet: ancestor("Adam","Obama") ********************* */

    StringVal OBAMA = StringVal("Obama");
    vector<Term*> args_obama_rapidnet;
    args_obama_rapidnet.push_back(&ADAM);
    args_obama_rapidnet.push_back(&OBAMA);

    PredicateInstance ancestor_adam_obama = PredicateInstance(&ancestor_rapidnet, args_obama_rapidnet);

    /* ******************************* CVC4 ******************************** */

    Expr ancestor_obama_cvc4 = parseFormula(&em, &ancestor_adam_obama);
    Expr forall_x__ancestor_ADAM_x_cvc4 = parseFormula(&em, &forall_x__ancestor_ADAM_x);

    /* **************************** CHECK SMT ******************************** */

    smt.assertFormula(forall_x__ancestor_ADAM_x_cvc4);
    std::cout << "\nSince " << forall_x__ancestor_ADAM_x_cvc4 << " hence "<<  ancestor_obama_cvc4 << " is: " << smt.query(ancestor_obama_cvc4) << std::endl;

    clearAllVariables();
}

/*
 * Function symbols testing
 * 
 */
void quantifier__function_child_younger_than_mother() {
    ExprManager em;
    SmtEngine smt(&em);

    /* ---------------------------- RAPIDNET ------------------------------ */
    //mother
    vector<Variable::TypeCode> domain_types;
    domain_types.push_back(Variable::STRING);
    FunctionSchema mother_schema = FunctionSchema("mother", domain_types, Variable::STRING);

    //
    vector<Term*> args;
    StringVal MaliaObama = StringVal("MaliaObama");
    args.push_back(&MaliaObama);
    UserFunction mother_malia = UserFunction(&mother_schema, args);

    StringVal MichelleObama = StringVal("MichelleObama");
    StringVal BarbaraBush = StringVal("BarbaraBush");

    Constraint michelle_is_mother_of_malia = Constraint(Constraint::EQ, &mother_malia, &MichelleObama);
    Constraint barbara_is_mother_of_malia = Constraint(Constraint::EQ, &mother_malia, &BarbaraBush);

    /* ------------------------------ CVC4 ------------------------------ */
    Expr michelle_is_mother_of_malia_cvc4 = parseFormula(&em, &michelle_is_mother_of_malia);
    Expr barbara_is_mother_of_malia_cvc4 = parseFormula(&em, &barbara_is_mother_of_malia);

    smt.assertFormula(michelle_is_mother_of_malia_cvc4);
    std::cout << "\nSince " << michelle_is_mother_of_malia_cvc4 << ", hence " << barbara_is_mother_of_malia_cvc4 << " is: " << smt.query(barbara_is_mother_of_malia_cvc4) << std::endl;

    clearAllVariables();
}

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








