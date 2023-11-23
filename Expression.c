// See full repo including dev notes and bonus branch at https://github.com/Romadelf/INFO0952-1_TP3-Ex3 (access will be made public after late due date)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Dict.h"
#include "Expression.h"

//--------------------------------------------------------------------------
// Do not change code between these lines, or our tests might fail.

typedef enum
{
    NUMBER, OPERATOR, SYMBOL
} ExprType;

struct Expression_t
{
    ExprType type;
    union
    {
        double num;
        char* symb;
        Operator op;
    } value;
    Expression* left;
    Expression* right;
};

// Constructeurs

Expression* exprSymb(char* var)
{
    Expression* exp = malloc(sizeof(Expression));
    if (exp == NULL)
    {
        fprintf(stderr, "exprSymb: could not allocate memory.\n");
        exit(1);
    }
    exp->type = SYMBOL;
    exp->value.symb = var;
    exp->left = NULL;
    exp->right = NULL;
    return exp;
}

Expression* exprNum(double num)
{
    Expression* exp = malloc(sizeof(Expression));
    if (exp == NULL)
    {
        fprintf(stderr, "exprNum: could not allocate memory.\n");
        exit(1);
    }
    exp->type = NUMBER;
    exp->value.num = num;
    exp->left = NULL;
    exp->right = NULL;
    return exp;
}

Expression* exprOp(Operator op, Expression* left, Expression* right)
{
    if (op != PLUS && op != MINUS && op != TIMES && op != DIV)
    {
        fprintf(stderr, "expOp: unknown operator.\n");
        exit(1);
    }

    Expression* exp = malloc(sizeof(Expression));
    if (exp == NULL)
    {
        fprintf(stderr, "exprOp: could not allocate memory.\n");
        exit(1);
    }
    exp->type = OPERATOR;
    exp->value.op = op;
    exp->left = left;
    exp->right = right;
    return exp;
}

void exprFree(Expression* exp)
{
    if (exp->type == OPERATOR)
    {
        if (exp->left != NULL)
            exprFree(exp->left);
        if (exp->right != NULL)
            exprFree(exp->right);
    }
    free(exp);
}

static Expression* exprCopy(Expression* exp)
{
    if (exp == NULL)
    {
        fprintf(stderr, "exprCopy: malformed expression.\n");
        exit(1);
    }
    if (exp->type == NUMBER)
        return exprNum(exp->value.num);
    else if (exp->type == SYMBOL)
        return exprSymb(exp->value.symb);
    else if (exp->type == OPERATOR)
        return exprOp(exp->value.op, exprCopy(exp->left), exprCopy(exp->right));
    else
    {
        fprintf(stderr, "exprCopy: unknown node type (%d).\n", exp->type);
        exit(1);
    }
}

//--------------------------------------------------------------------------

// Only enter your code below. You can add additional functions as needed, (but declare
// them as static).

void exprPrint(FILE* fp, Expression* exp)
{
    switch (exp->type)
    {
        case NUMBER:
        {
            fprintf(fp, "%f", exp->value.num);
            break;
        }
        case SYMBOL:
        {
            fprintf(fp, "%s", exp->value.symb);
            break;
        }
        case OPERATOR:
        {
            char* sign;
            switch (exp->value.op)
            {
                case PLUS:
                {
                    sign = "+";
                    break;
                }
                case MINUS:
                {
                    sign = "-";
                    break;
                }
                case TIMES:
                {
                    sign = "*";
                    break;
                }
                case DIV:
                {
                    sign = "/";
                    break;
                }
                default:
                {
                    fprintf(stderr, "exprPrint: unknown operator.\n");
                    exit(2);
                }
            }
            fprintf(fp, "(");
            exprPrint(fp, exp->left);
            fprintf(fp, "%s", sign);
            exprPrint(fp, exp->right);
            fprintf(fp, ")");
            break;
        }
        default:
        {
            fprintf(stderr, "exprPrint: unknown type.\n");
            exit(2);
        }
    }
}

double exprEval(Expression* exp, Dict* dict)
{
    switch (exp->type)
    {
        case NUMBER:
        {
            return exp->value.num;
        }
        case SYMBOL:
        {
            return *((double*)dictSearch(dict, exp->value.symb));
        }
        case OPERATOR:
        {
            double left = exprEval(exp->left, dict);
            double right = exprEval(exp->right, dict);
            switch (exp->value.op)
            {
                case PLUS:
                {
                    return left + right;
                }
                case MINUS:
                {
                    return left - right;
                }
                case TIMES:
                {
                    return left * right;
                }
                case DIV:
                {
                    return left / right;
                }
                default:
                {
                    fprintf(stderr, "exprEval: unknown operator.\n");
                    exit(2);
                }
            }
            break;
        }
        default:
        {
            fprintf(stderr, "exprEval: unknown type.\n");
            exit(2);
        }
    }
}

static bool strEquals(char* str1, char* str2)
{
    return strcmp(str1, str2) == 0;
}

static bool areIndependent(Expression* exp, char* var)
{
    switch (exp->type)
    {
        case NUMBER:
        {
            return true;
        }
        case SYMBOL:
        {
            return !strEquals(var, exp->value.symb);
        }
        case OPERATOR:
        {
            return areIndependent(exp->left, var)
                    && areIndependent(exp->right, var);
        }
        default:
        {
            fprintf(stderr, "areIndependent: unknown type.\n");
            exit(2);
        }
    }
}

Expression* exprDerivate(Expression* exp, char* var)
{
    switch (exp->type)
    {
        case NUMBER:
        {
            return exprNum(0);
        }
        case SYMBOL:
        {
            return exprNum(strEquals(var, exp->value.symb));
        }
        case OPERATOR:
        {
            if (areIndependent(exp, var))
                return exprNum(0);

            switch (exp->value.op)
            {
                case PLUS:
                {
                    if (areIndependent(exp->left, var))
                        return exprDerivate(exp->right, var);

                    if (areIndependent(exp->right, var))
                        return exprDerivate(exp->left, var);

                    return exprOp(PLUS, //@formatter:off
                                  exprDerivate(exp->left, var),
                                  exprDerivate(exp->right, var)); //@formatter:on
                }
                case MINUS:
                {
                    if (areIndependent(exp->left, var)) //@formatter:off
                        return exprOp(MINUS,
                                      exprNum(0),
                                      exprDerivate(exp->right, var)); //@formatter:on

                    if (areIndependent(exp->right, var))
                        return exprDerivate(exp->left, var);

                    return exprOp(MINUS, //@formatter:off
                                  exprDerivate(exp->left, var),
                                  exprDerivate(exp->right, var)); //@formatter:on
                }
                case TIMES:
                {
                    if (areIndependent(exp->left, var))
                        return exprOp(TIMES, //@formatter:off
                                      exprCopy(exp->left),
                                      exprDerivate(exp->right, var)); //@formatter:on

                    if (areIndependent(exp->right, var))
                        return exprOp(TIMES, //@formatter:off
                                      exprCopy(exp->right),
                                      exprDerivate(exp->left, var)); //@formatter:on

                    return exprOp(PLUS, //@formatter:off
                                  exprOp(TIMES,
                                         exprDerivate(exp->left, var),
                                         exprCopy(exp->right)),
                                  exprOp(TIMES,
                                         exprCopy(exp->left),
                                         exprDerivate(exp->right, var))); //@formatter:on
                }
                case DIV:
                {
                    if (areIndependent(exp->left, var))
                        return exprOp(TIMES, //@formatter:off
                                      exprOp(MINUS,
                                             exprNum(0),
                                             exprCopy(exp->left)),
                                      exprOp(DIV,
                                             exprDerivate(exp->right, var),
                                             exprOp(TIMES,
                                                    exprCopy(exp->right),
                                                    exprCopy(exp->right)))); //@formatter:on

                    if (areIndependent(exp->right, var))
                    {
                        Expression* altExp = exprOp(TIMES, //@formatter:off
                                                    exprOp(DIV,
                                                           exprNum(1),
                                                           exprCopy(exp->right)),
                                                    exprCopy(exp->left)); //@formatter:on
                        Expression* derivate = exprDerivate(altExp, var);
                        exprFree(altExp);
                        return derivate;
                    }

                    return exprOp(DIV, //@formatter:off
                                  exprOp(MINUS,
                                         exprOp(TIMES,
                                                exprDerivate(exp->left, var),
                                                exprCopy(exp->right)),
                                         exprOp(TIMES,
                                                exprCopy(exp->left),
                                                exprDerivate(exp->right, var))),
                                  exprOp(TIMES,
                                         exprCopy(exp->right),
                                         exprCopy(exp->right))); //@formatter:on
                }
                default:
                {
                    fprintf(stderr, "exprDerivate: unknown operator.\n");
                    exit(2);
                }
            }
        }
        default:
        {
            fprintf(stderr, "exprDerivate: unknown type.\n");
            exit(2);
        }
    }
}
