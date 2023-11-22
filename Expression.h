#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdio.h>

#include "Dict.h"

typedef enum
{
    PLUS,
    MINUS,
    TIMES,
    DIV
} Operator;

typedef struct Expression_t Expression;

/**
 * @brief Free the expression
 *
 * @param exp the expression
 */
void exprFree(Expression *exp);

/**
 * @brief Create a new expression of symbol type
 *
 * @param var the name of the symbol
 * @return the new expression
 */
Expression *exprSymb(char *var);

/**
 * @brief Create a new expression of number type
 *
 * @param num the number
 * @return the new expression
 */
Expression *exprNum(double num);

/**
 * @brief Create a new expression of operator type
 *
 * @param op the operator (PLUS, MINUS, TIMES or DIV)
 * @param left the left subexpression
 * @param right the right subexpression
 * @return the new expression
 */
Expression *exprOp(Operator op, Expression *left, Expression *right);

/**
 * @brief Print the expression in file fp.
 *
 * @param fp the file in which to print the expression
 * @param exp the expression to be printed
 */
void exprPrint(FILE *fp, Expression *exp);

/**
 * @brief Evaluate the value of the expression using the values of symbol
 *        in the dictionary dict. All symbols are supposed to be defined in
 *        dict.
 *
 * @param exp the expression to be evaluated
 * @param dict the dictionary with the values of all symbols
 * @return the value of the expression
 */
double exprEval(Expression *exp, Dict *dict);

/**
 * @brief Compute a new expression corresponding to the derivative of the
 *        input expression according to var.
 *
 * @param exp the expression to be derivated
 * @param var the variable according to which the derivative needs to be computed
 * @return the new expression
 */

Expression *exprDerivate(Expression *exp, char *var);

#endif
