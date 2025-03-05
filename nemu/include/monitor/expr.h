#ifndef __EXPR_H__
#define __EXPR_H__

#include "common.h"

typedef enum { OP_NONE, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_EQ, OP_NEQ, OP_AND, OP_OR, OP_NOT, OP_DEREF } OperatorType;

typedef enum { AST_OPERATOR, AST_NUMBER, AST_REGISTER, AST_VARIABLE, AST_POINTER } ASTNodeType;

typedef struct ASTNode
{
    ASTNodeType type;
    union
    {
        struct
        {
            OperatorType    op;
            struct ASTNode* left;
            struct ASTNode* right;
        };
        uint32_t val;
        char     reg_name[8];
        char     var_name[32];
        // void* ptr;
        struct
        {
            void*   ptr;
            uint8_t bit_width;
        };
    } data;
} ASTNode;

ASTNode* build_ast(char* expr, bool* success);

uint32_t eval_ast(ASTNode* node);

void free_ast(ASTNode* node);

uint32_t expr(char*, bool*);

#endif
