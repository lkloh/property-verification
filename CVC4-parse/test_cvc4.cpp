#include <iostream>
#include <cvc4/cvc4.h> 
#include "rapidnet_compiler/sdn-formula.h"

using namespace std;
using namespace CVC4;

/* CVC
 * ---
 * Given that (x>1) and (y<3), what values of x and y satisfy (x=y)?
 */
void get_SAT_model() {
    ExprManager em;
    SmtEngine smt(&em);

    smt.setOption("produce-models", true);

    // make variables
    Expr one = em.mkConst(Rational(1));
    Expr three = em.mkConst(Rational(3));
    Expr x = em.mkVar("x", em.integerType());
    Expr y = em.mkVar("y", em.integerType());

    // make expressions
    Expr x_gt_1 = em.mkExpr(kind::GT, x, one); // x > 1
    Expr y_lt_2 = em.mkExpr(kind::LT, y, three); // y < 3
    Expr x_eq_y = em.mkExpr(kind::EQUAL, x, y); // y=3

    smt.assertFormula(x_gt_1);
    smt.assertFormula(y_lt_2);
    smt.push();

    // check SAT
    Result result = smt.checkSat(x_eq_y);
    cout << "\nCVC4 reports: " << x_eq_y << " is " << result << "." << endl;

    // If the formula was satisfiable (which it should be here), get the value of x. 
    if (result == Result::SAT) {
      cout << "\nx value: " << smt.getValue(x) << "; y value: " << smt.getValue(y) << std::endl;
    }
}

/*
 * Given that x=1 and y>2, is there any values of x and y that satisfy x=y?
 */
void get_UNSAT_example() {
    ExprManager em;
    SmtEngine smt(&em);

    smt.setOption("produce-models", true);

    // make variables
    Expr one = em.mkConst(Rational(1));
    Expr two = em.mkConst(Rational(2));
    Expr x = em.mkVar("x", em.integerType());
    Expr y = em.mkVar("y", em.integerType());

    //make expressions
    Expr x_equals_one = em.mkExpr(kind::EQUAL, x, one);
    Expr y_GT_two = em.mkExpr(kind::GT, y, two);

    // check smt
    smt.assertFormula(x_equals_one);
    smt.assertFormula(y_GT_two);
    smt.push();

    Expr x_equals_y = em.mkExpr(kind::EQUAL, x, y);
    Result result = smt.checkSat(x_equals_y);
    cout << "\nCVC4 reports: " << x_equals_y << " is " << result << "." << endl;
}

/*
 * (x > y) /\ (y > z) /\ (z > 1) => (y > 1)
 * True, but we only need to know (y > z), (z > 1) to conclude that (y > 1)
 * 
 * UNSAT code only supported in CVC4-1.5 (preleased)
 * December 3, 2014
 */
void get_SATcore_example() {
    ExprManager em;
    SmtEngine smt(&em);

    smt.setOption("produce-models", true);

    //make variables
    Expr one = em.mkConst(Rational(1));
    Expr x = em.mkVar("x", em.integerType());
    Expr y = em.mkVar("y", em.integerType());
    Expr z = em.mkVar("z", em.integerType());

    //make expressions
    Expr x_gt_y = em.mkExpr(kind::GT, x, y);
    Expr y_gt_z = em.mkExpr(kind::GT, y, z);
    Expr z_gt_1 = em.mkExpr(kind::GT, z, one);
    Expr y_gt_1 = em.mkExpr(kind::GT, y, one);

    //check sat
    smt.assertFormula(x_gt_y);
    smt.assertFormula(y_gt_z);
    smt.assertFormula(z_gt_1);
    smt.push();

    Result result = smt.query(y_gt_1);
}

/* forall integer x, x=3
 * invalid, show a counterexample this is false
 */
void get_invalid_countermodel() {
    ExprManager em;
    SmtEngine smt(&em);

    smt.setOption("produce-models", true);

    //make variables
    Expr three = em.mkConst(Rational(3));
    Expr x = em.mkBoundVar("x", em.integerType());
    vector<Expr> var_list;
    var_list.push_back(x);
    Expr bound_var_list = em.mkExpr(kind::BOUND_VAR_LIST, var_list);
    Expr formula = em.mkExpr(kind::EQUAL, x, three);
    Expr forall_x__x_eq_3 = em.mkExpr(kind::FORALL, bound_var_list, formula);

     //check sat
    Result result = smt.query(forall_x__x_eq_3);

    if (result == Result::INVALID) {
      cout << "\nExample x that shows " << forall_x__x_eq_3 << " is invalid: " << smt.getValue(x) << std::endl;
    }
}

/* FUNCTION
 * WordToNum(x): string -> integer
 * WordToNum("seven") = 7
 */
void function_example() {
    ExprManager em;
    SmtEngine smt(&em);

    Type stringToInt = em.mkFunctionType(em.stringType(), em.integerType());
    Expr x = em.mkVar(em.integerType());
    Expr WordToNum = em.mkVar("WordToNum", stringToInt);
    Expr WordToNum_x = em.mkExpr(kind::APPLY_UF, WordToNum, x);

    Expr seven_int = em.mkConst(Rational(7));
    Expr seven_string = em.mkConst(String("seven"));
    Expr WordToNum_seven = em.mkExpr(kind::APPLY_UF, WordToNum, seven_string);

    // insist that WordToNum("seven") = 7
    Expr seven_eq_7 = em.mkExpr(kind::EQUAL, WordToNum_seven, seven_int);
    smt.assertFormula(seven_eq_7);

    // check that WordToNum("seven") != 8
    Expr eight_int = em.mkConst(Rational(8));
    Expr seven_neq_8 = em.mkExpr(kind::EQUAL, WordToNum_seven, eight_int);
    Result result = smt.query(seven_neq_8);

    cout << "\nSince " << seven_eq_7 << ", therefore " << seven_neq_8 << " is: " << result << std::endl;
}

/* FUNCTION
 * OnlyChildOf: string, string -> string
 *              FATHER, MOTHER    CHILD
 *
 * Example
 * OnlyChildOf("BillClinton","HillaryRodham") = "ChelseaClinton"
 * OnlyChildOf("BillClinton","HillaryRodham") != "MaliaObama"
 */
void dual_function_example() {
    ExprManager em;
    SmtEngine smt(&em);

    vector<Type> schema_domain_types;
    schema_domain_types.push_back(em.stringType());
    schema_domain_types.push_back(em.stringType());
    Type OnlyChildOfType = em.mkFunctionType(schema_domain_types, em.stringType());

    Expr x = em.mkVar(em.stringType());
    Expr y = em.mkVar(em.stringType());
    vector<Expr> OnlyChildOf_vec;
    OnlyChildOf_vec.push_back(x);
    OnlyChildOf_vec.push_back(y);
    Expr OnlyChildOf = em.mkVar("OnlyChildOf", OnlyChildOfType);
    Expr OnlyChildOf_x = em.mkExpr(kind::APPLY_UF, OnlyChildOf, OnlyChildOf_vec);

    Expr BillClintonString = em.mkConst(String("BillClinton"));
    Expr HillaryRodhamString = em.mkConst(String("HillaryRodham"));
    Expr ChelseaClintonString = em.mkConst(String("ChelseaClinton"));
    Expr MaliaObamaString = em.mkConst(String("MaliaObama"));

    // OnlyChildOf("BillClinton","HillaryRodham") = "ChelseaClinton"
    vector<Expr> clinton_array;
    clinton_array.push_back(BillClintonString);
    clinton_array.push_back(HillaryRodhamString);
    Expr OnlyChildOf_Bill_Hillary = em.mkExpr(kind::APPLY_UF, OnlyChildOf, clinton_array);
    Expr OnlyChildOf_Bill_Hillary_is_Chelsea = em.mkExpr(kind::EQUAL, OnlyChildOf_Bill_Hillary, ChelseaClintonString);

    smt.assertFormula(OnlyChildOf_Bill_Hillary_is_Chelsea);

    Expr OnlyChildOf_Bill_Hillary_is_Malia = em.mkExpr(kind::EQUAL, OnlyChildOf_Bill_Hillary, MaliaObamaString);

    Result result = smt.query(OnlyChildOf_Bill_Hillary_is_Malia);
    cout << "\nSince " << OnlyChildOf_Bill_Hillary_is_Chelsea << ", therefore " << OnlyChildOf_Bill_Hillary_is_Malia << " is: " << result << std::endl;
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
void nested_function_predicate() {
    ExprManager em;
    SmtEngine smt(&em);

    /* make mother schema */
    vector<Type> mother_domain_types;
    mother_domain_types.push_back(em.stringType());
    Type mother_f = em.mkFunctionType(mother_domain_types, em.stringType());

    Expr child = em.mkBoundVar("child", em.stringType());

    vector<Expr> mother_schema_args;
    mother_schema_args.push_back(child);
    Expr mother_schema = em.mkVar("mother", mother_f);

    Expr mother_x = em.mkExpr(kind::APPLY_UF, mother_schema, mother_schema_args);

    /* make YOUNGER predicate */
    vector<Type> YOUNGER_domain_types;
    YOUNGER_domain_types.push_back(em.stringType());
    YOUNGER_domain_types.push_back(em.stringType());
    Type YOUNGER_p = em.mkPredicateType(YOUNGER_domain_types);

    vector<Expr> YOUNGER_schema_args;
    YOUNGER_schema_args.push_back(child);
    YOUNGER_schema_args.push_back(mother_x);
    Expr YOUNGER_schema = em.mkVar("YOUNGER", YOUNGER_p);

    Expr YOUNGER_x_motherx = em.mkExpr(kind::APPLY_UF, YOUNGER_schema, YOUNGER_schema_args);

    /* make forall quantifier */
    vector<Expr> bound_vars;
    bound_vars.push_back(child);
    Expr bound_vars_expr = em.mkExpr(kind::BOUND_VAR_LIST, bound_vars);
    Expr forall_x__YOUNGER_x_motherx = em.mkExpr(kind::FORALL, bound_vars_expr, YOUNGER_x_motherx);

    /* the mother of malia obama is older than malia obama */
    Expr MaliaObamaString = em.mkConst(String("MaliaObama"));

    // mother("MaliaObama")
    vector<Expr> malia_mother_args;
    malia_mother_args.push_back(MaliaObamaString);
    Expr mother_malia_function = em.mkExpr(kind::APPLY_UF, mother_schema, malia_mother_args);

    // YOUNGER("MaliaObama", mother("MaliaObama"))
    vector<Expr> malia_args;
    malia_args.push_back(MaliaObamaString);
    malia_args.push_back(mother_malia_function);
    Expr YOUNGER_malia_motherMalia = em.mkExpr(kind::APPLY_UF, YOUNGER_schema, malia_args);

    /* query smt */
    smt.assertFormula(forall_x__YOUNGER_x_motherx);
    std::cout << "\nSince" << forall_x__YOUNGER_x_motherx << ", therefore " << YOUNGER_malia_motherMalia << " is: " << smt.query(YOUNGER_malia_motherMalia) << std::endl;
}


int main() {
    function_example();
    dual_function_example();
	get_SAT_model();
	get_UNSAT_example();
    get_SATcore_example();
    get_invalid_countermodel();
    nested_function_predicate(); 
	return 0;
}







