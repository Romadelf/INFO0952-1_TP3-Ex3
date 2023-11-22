#include <stdio.h>
#include <stdlib.h>

#include "Dict.h"
#include "Expression.h"

int main(void)
{

    // Create the dictionary
    Dict* dict = dictCreate(1000);

    // Add the symbols
    double x = 1.0;
    double y = 2.0;
    double z = 3.0;
    double w = 4.0;
    dictInsert(dict, "x", &x);
    dictInsert(dict, "y", &y);
    dictInsert(dict, "z", &z);
    dictInsert(dict, "w", &w);

    // Create the expression
    // ((2 * (y - x)) + (z*z)*(x/(w+x)))

    Expression* expr = exprOp(
            PLUS,
            exprOp(TIMES, exprNum(2),
                   exprOp(MINUS, exprSymb("y"), exprSymb("x"))),
            exprOp(TIMES,
                   exprOp(TIMES, exprSymb("z"), exprSymb("z")),
                   exprOp(DIV, exprSymb("x"),
                          exprOp(PLUS, exprSymb("w"), exprSymb("x")))));

    printf("x=%.2f, y=%.2f, z=%.2f, w=%.2f\n\n", x, y, z, w);

    printf("Expression initiale:\n");
    exprPrint(stdout, expr);
    printf("\n");

    printf("Valeur: %f (expected: %f)\n", exprEval(expr, dict),
           2 * (y - x) + z * z * (x / (w + x)));

    printf("\nDérivée selon x:\n");
    Expression* exprderiv_x = exprDerivate(expr, "x");
    exprPrint(stdout, exprderiv_x);
    printf("\n");
    printf("Valeur: %f (expected: %f)\n", exprEval(exprderiv_x, dict),
           -2 + z * z * w / ((w + x) * (w + x)));

    printf("\nDérivée selon y:\n");
    Expression* exprderiv_y = exprDerivate(expr, "y");
    exprPrint(stdout, exprderiv_y);
    printf("\n");
    printf("Valeur: %f (expected: %f)\n", exprEval(exprderiv_y, dict), 2.0);

    printf("\nDérivée selon z:\n");
    Expression* exprderiv_z = exprDerivate(expr, "z");
    exprPrint(stdout, exprderiv_z);
    printf("\n");
    printf("Valeur: %f (expected: %f)\n", exprEval(exprderiv_z, dict),
           2 * z * x / (w + x));

    printf("\nDérivée selon w:\n");
    Expression* exprderiv_w = exprDerivate(expr, "w");
    exprPrint(stdout, exprderiv_w);
    printf("\n");
    printf("Valeur: %f (expected: %f)\n", exprEval(exprderiv_w, dict),
           -z * z * x / ((w + x) * (w + x)));

    exprFree(expr);
    exprFree(exprderiv_x);
    exprFree(exprderiv_y);
    exprFree(exprderiv_z);
    exprFree(exprderiv_w);

    dictFree(dict);
}
