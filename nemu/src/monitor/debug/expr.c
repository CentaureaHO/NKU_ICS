#include "nemu.h"
#include "monitor/expr.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <sys/types.h>
#include <stdlib.h>

#define REPLACE_REG_TO_PTR
// #define EXPRDBG_LOG_ENABLE
#define PRINT_AST

#ifndef X86_REGS
#define X86_REGS \
    X(eax, 32)   \
    X(ecx, 32)   \
    X(edx, 32)   \
    X(ebx, 32)   \
    X(esp, 32)   \
    X(ebp, 32)   \
    X(esi, 32)   \
    X(edi, 32)   \
    X(eip, 32)   \
    X(ax, 16)    \
    X(cx, 16)    \
    X(dx, 16)    \
    X(bx, 16)    \
    X(sp, 16)    \
    X(bp, 16)    \
    X(si, 16)    \
    X(di, 16)    \
    X(al, 8)     \
    X(cl, 8)     \
    X(dl, 8)     \
    X(bl, 8)     \
    X(ah, 8)     \
    X(ch, 8)     \
    X(dh, 8)     \
    X(bh, 8)
#endif

#define MAX_TOKEN_LEN 32

#define TOKENS                         \
    X(TK_NOTYPE, void, 0)              \
    X(TK_DECNUM, decimal number, 1)    \
    X(TK_HEXNUM, hex number, 2)        \
    X(TK_REG, register, 3)             \
    X(TK_LPARAN, left parantheses, 4)  \
    X(TK_RPARAN, right parantheses, 5) \
    X(TK_ADD, add, 6)                  \
    X(TK_SUB, sub, 7)                  \
    X(TK_DIV, div, 8)                  \
    X(TK_EQ, equal, 9)                 \
    X(TK_NEQ, not equal, 10)           \
    X(TK_AND, and, 11)                 \
    X(TK_OR, or, 12)                   \
    X(TK_NOT, not, 13)                 \
    X(TK_STAR, star, 14)               \
    X(TK_VAR, variable, 15)

typedef enum {
#define X(cn, name, id) cn = id,
    TOKENS
#undef X
} TokenType;

const char* token2str(TokenType type)
{
    switch (type)
    {
#define X(cn, name, id) \
    case cn: return #name;
        TOKENS
#undef X
        default: return "unknown";
    }

    return "unknown";
}

static struct rule
{
    char*     regex;
    TokenType token_type;
} rules[] = {

    /* DONE: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +", TK_NOTYPE},  // spaces
    /*
    {"\\+", '+'},       // plus
    {"==", TK_EQ}       // equal
    */
    {"0[xX][0-9a-fA-F]+", TK_HEXNUM},   // hex number
    {"[0-9]+", TK_DECNUM},              // decimal number
    {"\\$[a-zA-Z]+", TK_REG},           // register
    {"\\(", TK_LPARAN},                 // left parantheses
    {"\\)", TK_RPARAN},                 // right parantheses
    {"\\+", TK_ADD},                    // add
    {"-", TK_SUB},                      // sub
    {"\\*", TK_STAR},                   // star
    {"/", TK_DIV},                      // div
    {"==", TK_EQ},                      // equal
    {"!=", TK_NEQ},                     // not equal
    {"&&", TK_AND},                     // and
    {"\\|\\|", TK_OR},                  // or
    {"!", TK_NOT},                      // not
    {"[a-zA-Z_][a-zA-Z0-9_]*", TK_VAR}  // variable
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
    int  i;
    char error_msg[128];
    int  ret;

    for (i = 0; i < NR_REGEX; i++) {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0) {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token
{
    TokenType type;
    char      str[32];
} Token;

Token tokens[MAX_TOKEN_LEN];
int   nr_token;

static bool make_token(char* e)
{
    int        position = 0;
    int        i;
    regmatch_t pmatch;

    nr_token = 0;

#ifdef EXPRDBG_LOG_ENABLE
    char matched_str[32];
#endif

    while (e[position] != '\0') {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i++) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                char* substr_start = e + position;
                int   substr_len   = pmatch.rm_eo;

#ifdef EXPRDBG_LOG_ENABLE
                strncpy(matched_str, substr_start, substr_len);
                matched_str[substr_len] = '\0';
                Log("match substr \"%s\" as token %s", matched_str, token2str(rules[i].token_type));
#else
/*
Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
    i,
    rules[i].regex,
    position,
    substr_len,
    substr_len,
    substr_start);
*/
#endif

                position += substr_len;

                if (rules[i].token_type == TK_NOTYPE) break;

                if (nr_token >= MAX_TOKEN_LEN) {
                    Log("too many tokens");
                    return false;
                }

                tokens[nr_token].type           = rules[i].token_type;
                if (substr_len > 31) substr_len = 31;
                strncpy(tokens[nr_token].str, substr_start, substr_len);
                tokens[nr_token].str[substr_len] = '\0';
                ++nr_token;

                break;
            }
        }

        if (i == NR_REGEX) {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }

    return true;
}

static ASTNode* make_op_node(OperatorType op, ASTNode* left, ASTNode* right)
{
    ASTNode* node       = (ASTNode*)malloc(sizeof(ASTNode));
    node->type          = AST_OPERATOR;
    node->data.op    = op;
    node->data.left  = left;
    node->data.right = right;
    return node;
}

static ASTNode* make_number_node(uint32_t val)
{
    ASTNode* node  = (ASTNode*)malloc(sizeof(ASTNode));
    node->type     = AST_NUMBER;
    node->data.val = val;
    return node;
}

#ifdef REPLACE_REG_TO_PTR

static ASTNode* make_pointer_node(void* ptr, uint8_t bits)
{
    ASTNode* node            = (ASTNode*)malloc(sizeof(ASTNode));
    node->type               = AST_POINTER;
    node->data.ptr       = ptr;
    node->data.bit_width = bits;
    return node;
}

#else

static ASTNode* make_register_node(const char* reg)
{
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type    = AST_REGISTER;
    strncpy(node->data.reg_name, reg, 7);
    node->data.reg_name[7] = '\0';
    return node;
}

#endif

static ASTNode* make_variable_node(const char* var)
{
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type    = AST_VARIABLE;
    strncpy(node->data.var_name, var, 31);
    node->data.var_name[31] = '\0';
    return node;
}

static ASTNode* build_expr(int* pos);
static ASTNode* build_logical_or_expr(int* pos);
static ASTNode* build_logical_and_expr(int* pos);
static ASTNode* build_equality_expr(int* pos);
static ASTNode* build_relational_expr(int* pos);
static ASTNode* build_add_sub_expr(int* pos);
static ASTNode* build_mul_div_expr(int* pos);
static ASTNode* build_unary_expr(int* pos);
static ASTNode* build_factor(int* pos);
static ASTNode* build_operand(int* pos);

static bool ast_has_error = false;

#ifdef PRINT_AST
void print_ast(ASTNode* node, int depth)
{
    if (node == NULL) return;

    for (int i = 0; i < depth; ++i) printf("  ");
    switch (node->type)
    {
        case AST_OPERATOR:
            switch (node->data.op)
            {
                case OP_ADD: printf("ADD\n"); break;
                case OP_SUB: printf("SUB\n"); break;
                case OP_MUL: printf("MUL\n"); break;
                case OP_DIV: printf("DIV\n"); break;
                case OP_EQ: printf("EQ\n"); break;
                case OP_NEQ: printf("NEQ\n"); break;
                case OP_AND: printf("AND\n"); break;
                case OP_OR: printf("OR\n"); break;
                case OP_NOT: printf("NOT\n"); break;
                case OP_DEREF: printf("DEREF\n"); break;
                default: printf("UNKNOWN OP\n"); break;
            }
            print_ast(node->data.left, depth + 1);
            print_ast(node->data.right, depth + 1);
            break;
        case AST_NUMBER: printf("NUMBER: %u\n", node->data.val); break;
        case AST_POINTER: printf("POINTER: %p\n", node->data.ptr); break;
        case AST_REGISTER: printf("REGISTER: %s\n", node->data.reg_name); break;
        case AST_VARIABLE: printf("VARIABLE: %s\n", node->data.var_name); break;
        default: printf("UNKNOWN NODE\n"); break;
    }
}
#endif

ASTNode* build_ast(char* e, bool* success)
{
    if (!make_token(e)) {
        *success = false;
        return NULL;
    }

    ast_has_error = false;
    int      pos  = 0;
    ASTNode* root = build_expr(&pos);

    if (pos < nr_token) {
        Log("Syntax error at token %s, at position %d", tokens[pos].str, pos);
        *success = false;
        free_ast(root);
        return NULL;
    }

    if (ast_has_error) {
        *success = false;
        free_ast(root);
        return NULL;
    }

#ifdef PRINT_AST
    Log("AST:");
    print_ast(root, 0);
#endif

    *success = true;
    return root;
}

void free_ast(ASTNode* node)
{
    if (node == NULL) return;

    if (node->type == AST_OPERATOR) {
        free_ast(node->data.left);
        free_ast(node->data.right);
    }

    free(node);
}

uint32_t eval_ast(ASTNode* node)
{
    if (node == NULL) return 0;

    switch (node->type)
    {
        case AST_OPERATOR:
        {
            switch (node->data.op)
            {
                case OP_ADD: return eval_ast(node->data.left) + eval_ast(node->data.right);
                case OP_SUB: return eval_ast(node->data.left) - eval_ast(node->data.right);
                case OP_MUL: return eval_ast(node->data.left) * eval_ast(node->data.right);
                case OP_DIV:
                {
                    uint32_t divisor = eval_ast(node->data.right);
                    if (divisor == 0) {
                        Log("divided by zero");
                        return 0;
                    }
                    return eval_ast(node->data.left) / divisor;
                }
                case OP_EQ: return eval_ast(node->data.left) == eval_ast(node->data.right);
                case OP_NEQ: return eval_ast(node->data.left) != eval_ast(node->data.right);
                case OP_AND: return eval_ast(node->data.left) && eval_ast(node->data.right);
                case OP_OR: return eval_ast(node->data.left) || eval_ast(node->data.right);
                case OP_NOT: return !eval_ast(node->data.right);
                case OP_DEREF:
                {
                    uint32_t addr = eval_ast(node->data.right);

                    if (addr > PMEM_SIZE - 4) {
                        Log("invalid memory address 0x%08x", addr);
                        return 0;
                    }

                    uint32_t val = vaddr_read(addr, 4);
                    Log("read memory at 0x%08x: 0x%08x", addr, val);
                    return val;
                }
                default: Log("unknown operator"); return 0;
            }
        }
        case AST_NUMBER: return node->data.val;
        case AST_POINTER:
            switch (node->data.bit_width)
            {
                case 8: return *(uint8_t*)node->data.ptr;
                case 16: return *(uint16_t*)node->data.ptr;
                case 32: return *(uint32_t*)node->data.ptr;
                default: Log("unknown pointer bits"); return 0;
            }
        case AST_REGISTER: { const char* reg_name = node->data.reg_name;

#define X(name, bits) \
    if (strcmp(reg_name, #name) == 0) return cpu.name;
            X86_REGS
#undef X

            Log("unknown register %s", reg_name);
            return 0;
        }
        case AST_VARIABLE: Log("Variable not supported: %s", node->data.var_name); return 0;
        default: Log("unknown AST node type"); return 0;
    }
}

inline bool check_token(int pos, TokenType type) { return pos < nr_token && tokens[pos].type == type; }

static ASTNode* build_expr(int* pos) { return build_logical_or_expr(pos); }

static ASTNode* build_logical_or_expr(int* pos)
{
    ASTNode* node = build_logical_and_expr(pos);

    while (*pos < nr_token && tokens[*pos].type == TK_OR) {
        ++(*pos);
        ASTNode* right = build_logical_and_expr(pos);
        node           = make_op_node(OP_OR, node, right);
    }

    return node;
}

static ASTNode* build_logical_and_expr(int* pos)
{
    ASTNode* node = build_equality_expr(pos);

    while (*pos < nr_token && tokens[*pos].type == TK_AND) {
        ++(*pos);
        ASTNode* right = build_equality_expr(pos);
        node           = make_op_node(OP_AND, node, right);
    }

    return node;
}

static ASTNode* build_equality_expr(int* pos)
{
    ASTNode* node = build_relational_expr(pos);

    while (*pos < nr_token) {
        if (check_token(*pos, TK_EQ)) {
            ++(*pos);
            ASTNode* right = build_relational_expr(pos);
            node           = make_op_node(OP_EQ, node, right);
        }
        else if (check_token(*pos, TK_NEQ))
        {
            ++(*pos);
            ASTNode* right = build_relational_expr(pos);
            node           = make_op_node(OP_NEQ, node, right);
        }
        else
            break;
    }

    return node;
}

static ASTNode* build_relational_expr(int* pos) { return build_add_sub_expr(pos); }

static ASTNode* build_add_sub_expr(int* pos)
{
    ASTNode* node = build_mul_div_expr(pos);

    while (*pos < nr_token) {
        if (check_token(*pos, TK_ADD)) {
            ++(*pos);
            ASTNode* right = build_mul_div_expr(pos);
            node           = make_op_node(OP_ADD, node, right);
        }
        else if (check_token(*pos, TK_SUB))
        {
            ++(*pos);
            ASTNode* right = build_mul_div_expr(pos);
            node           = make_op_node(OP_SUB, node, right);
        }
        else
            break;
    }

    return node;
}

static ASTNode* build_mul_div_expr(int* pos)
{
    ASTNode* node = build_unary_expr(pos);

    while (*pos < nr_token) {
        if (check_token(*pos, TK_STAR)) {
            ++(*pos);
            ASTNode* right = build_unary_expr(pos);
            node           = make_op_node(OP_MUL, node, right);
        }
        else if (check_token(*pos, TK_DIV))
        {
            ++(*pos);
            ASTNode* right = build_unary_expr(pos);
            node           = make_op_node(OP_DIV, node, right);
        }
        else
            break;
    }

    return node;
}

static ASTNode* build_unary_expr(int* pos)
{
    if (check_token(*pos, TK_ADD)) {
        ++(*pos);
        return build_unary_expr(pos);
    }
    else if (check_token(*pos, TK_SUB))
    {
        ++(*pos);
        ASTNode* expr = build_unary_expr(pos);
        ASTNode* zero = make_number_node(0);
        return make_op_node(OP_SUB, zero, expr);
    }
    else if (check_token(*pos, TK_NOT))
    {
        ++(*pos);
        ASTNode* expr = build_unary_expr(pos);
        return make_op_node(OP_NOT, NULL, expr);
    }
    else
        return build_factor(pos);
}

static ASTNode* build_factor(int* pos)
{
    if (check_token(*pos, TK_STAR)) {
        ++(*pos);
        ASTNode* expr = build_expr(pos);
        return make_op_node(OP_DEREF, NULL, expr);
    }

    if (check_token(*pos, TK_LPARAN)) {
        int pos_backup = *pos;
        ++(*pos);
        ASTNode* expr = build_expr(pos);
        if (*pos < nr_token && check_token(*pos, TK_RPARAN)) {
            ++(*pos);
            return expr;
        }
        else
        {
            Log("missing right parantheses for left parantheses at %d", pos_backup);
            ast_has_error = true;
            return NULL;
        }
    }

    return build_operand(pos);
}

static ASTNode* build_operand(int* pos)
{
    if (check_token(*pos, TK_DECNUM)) {
        uint32_t val = 0;
        sscanf(tokens[*pos].str, "%u", &val);
        ++(*pos);
        return make_number_node(val);
    }
    else if (check_token(*pos, TK_HEXNUM))
    {
        uint32_t val = 0;
        sscanf(tokens[*pos].str, "%x", &val);
        ++(*pos);
        return make_number_node(val);
    }
    else if (check_token(*pos, TK_REG))
    {
        char* reg_name = tokens[*pos].str + 1;
        ++(*pos);

#ifdef REPLACE_REG_TO_PTR
#define X(name, bits) \
    if (strcmp(reg_name, #name) == 0) return make_pointer_node((void*)&cpu.name, bits);
        X86_REGS
#undef X
#else
        return make_register_node(reg_name);
#endif

        Log("unknown register %s", reg_name);
        ast_has_error = true;
        return NULL;
    }
    else if (check_token(*pos, TK_VAR))
    {
        char* var_name = tokens[*pos].str;
        ++(*pos);
        return make_variable_node(var_name);
    }

    Log("unexpected token %s", tokens[*pos].str);
    ast_has_error = true;
    return NULL;
}

uint32_t expr(char* e, bool* success)
{
    ASTNode* root = build_ast(e, success);
    if (!*success) {
        return 0;
    }

    uint32_t result = eval_ast(root);
    free_ast(root);
    return result;
}