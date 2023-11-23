// See full repo including dev notes at https://github.com/Romadelf/INFO0952-1_TP3-Ex3 (access will be made public after late due date)

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
            fprintf(fp, "%.6g", exp->value.num);
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

Expression* exprDerivate(Expression* exp, char* var)
{
    // To be filled in.
}
