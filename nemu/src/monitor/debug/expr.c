#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <sys/types.h>

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

/*
typedef enum
{
    TK_NOTYPE = 256,
    TK_EQ

    // DONE: Add more token types

} TokenType;
*/

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
    {"0x[0-9a-fA-F]+", TK_HEXNUM},      // hex number
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

// #define EXPRDBG_LOG_ENABLE

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

                /* DONE: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array `tokens'. For certain types
                 * of tokens, some extra actions should be performed.
                 */
                /*
                                switch (rules[i].token_type)
                                {
                                    default:
                                }
                */

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

// Expr -> LogicalOrExpr
static uint32_t eval_expr(int* pos);
// LogicalOrExpr -> LogicalAndExpr | LogicalOrExpr TK_OR LogicalAndExpr
static uint32_t eval_logical_or_expr(int* pos);
// LogicalAndExpr -> EqualityExpr | LogicalAndExpr TK_AND EqualityExpr
static uint32_t eval_logical_and_expr(int* pos);
// EqualityExpr -> RelationalExpr | EqualityExpr TK_EQ RelationalExpr | EqualityExpr TK_NEQ RelationalExpr
static uint32_t eval_equality_expr(int* pos);
// RelationalExpr -> AddSubExpr
static uint32_t eval_relational_expr(int* pos);
// AddSubExpr -> MulDivExpr | AddSubExpr TK_ADD MulDivExpr | AddSubExpr TK_SUB MulDivExpr
static uint32_t eval_add_sub_expr(int* pos);
// MulDivExpr -> UnaryExpr | MulDivExpr TK_STAR UnaryExpr | MulDivExpr TK_DIV UnaryExpr
static uint32_t eval_mul_div_expr(int* pos);
// UnaryExpr -> Factor | TK_ADD UnaryExpr | TK_SUB UnaryExpr | TK_NOT UnaryExpr
static uint32_t eval_unary_expr(int* pos);
// Factor -> Operand | (Expr) | TK_STAR Expr
static uint32_t eval_factor(int* pos);
// Operand -> TK_DECNUM | TK_HEXNUM | TK_REG | TK_VAR
static uint32_t eval_operand(int* pos);
// UnaryOp -> TK_NOT
// static uint32_t eval_unary_op(int* pos);

uint32_t expr(char* e, bool* success)
{
    if (!make_token(e)) {
        *success = false;
        return 0;
    }

    /* TODO: Insert codes to evaluate the expression. */
    // TODO();
    int      pos = 0;
    uint32_t val = eval_expr(&pos);

    *success = true;
    return val;
}

inline bool check_token(int pos, TokenType type) { return pos < nr_token && tokens[pos].type == type; }

static uint32_t eval_expr(int* pos) { return eval_logical_or_expr(pos); }

static uint32_t eval_logical_or_expr(int* pos)
{
    uint32_t val = eval_logical_and_expr(pos);

    while (*pos < nr_token && tokens[*pos].type == TK_OR) {
        ++(*pos);
        uint32_t rval = eval_logical_and_expr(pos);
        val           = val || rval;
    }

    return val;
}

static uint32_t eval_logical_and_expr(int* pos)
{
    uint32_t val = eval_equality_expr(pos);

    while (*pos < nr_token && tokens[*pos].type == TK_AND) {
        ++(*pos);
        uint32_t rval = eval_equality_expr(pos);
        val           = val && rval;
    }

    return val;
}

static uint32_t eval_equality_expr(int* pos)
{
    uint32_t val = eval_relational_expr(pos);

    while (*pos < nr_token) {
        if (check_token(*pos, TK_EQ)) {
            ++(*pos);
            uint32_t rval = eval_relational_expr(pos);
            val           = (val == rval);
        }
        else if (check_token(*pos, TK_NEQ))
        {
            ++(*pos);
            uint32_t rval = eval_relational_expr(pos);
            val           = (val != rval);
        }
        else
            break;
    }

    return val;
}

static uint32_t eval_relational_expr(int* pos)
{
    // 此文法来自于上一学期我的编译原理作业
    // 其中显然会有关系运算符
    // 虽然此处实际没有关系运算符，但既然已经有这个文法了，那就带上了
    return eval_add_sub_expr(pos);
}

static uint32_t eval_add_sub_expr(int* pos)
{
    uint32_t val = eval_mul_div_expr(pos);

    while (*pos < nr_token) {
        if (check_token(*pos, TK_ADD)) {
            ++(*pos);
            uint32_t rval = eval_mul_div_expr(pos);
            val += rval;
        }
        else if (check_token(*pos, TK_SUB))
        {
            ++(*pos);
            uint32_t rval = eval_mul_div_expr(pos);
            val -= rval;
        }
        else
            break;
    }

    return val;
}

static uint32_t eval_mul_div_expr(int* pos)
{
    uint32_t val = eval_unary_expr(pos);

    while (*pos < nr_token) {
        if (check_token(*pos, TK_STAR)) {
            ++(*pos);
            uint32_t rval = eval_unary_expr(pos);
            val *= rval;
        }
        else if (check_token(*pos, TK_DIV))
        {
            ++(*pos);
            uint32_t rval = eval_unary_expr(pos);
            if (rval == 0) {
                Log("divided by zero");
                return 0;
            }
            val /= rval;
        }
        else
            break;
    }

    return val;
}

static uint32_t eval_unary_expr(int* pos)
{
    if (check_token(*pos, TK_ADD)) {
        ++(*pos);
        return eval_unary_expr(pos);
    }
    else if (check_token(*pos, TK_SUB))
    {
        ++(*pos);
        return -eval_unary_expr(pos);
    }
    else if (check_token(*pos, TK_NOT))
    {
        ++(*pos);
        return !eval_unary_expr(pos);
    }
    else
        return eval_factor(pos);
}

static uint32_t eval_factor(int* pos)
{
    if (check_token(*pos, TK_STAR)) {
        ++(*pos);

        // return vaddr_read(eval_expr(pos), 4);
        uint32_t addr = eval_expr(pos);
        uint32_t val  = vaddr_read(addr, 4);
        Log("read memory at 0x%08x: 0x%08x", addr, val);
        return val;
    }

    if (check_token(*pos, TK_LPARAN)) {
        ++(*pos);
        uint32_t val = eval_expr(pos);
        if (*pos < nr_token && check_token(*pos, TK_RPARAN)) {
            ++(*pos);
            return val;
        }
        else
        {
            Log("missing right parantheses");
            return 0;
        }
    }

    return eval_operand(pos);
}

static uint32_t eval_operand(int* pos)
{
    if (check_token(*pos, TK_DECNUM)) {
        uint32_t val = 0;
        sscanf(tokens[*pos].str, "%u", &val);
        ++(*pos);
        return val;
    }
    else if (check_token(*pos, TK_HEXNUM))
    {
        uint32_t val = 0;
        sscanf(tokens[*pos].str, "%x", &val);
        ++(*pos);
        return val;
    }
    else if (check_token(*pos, TK_REG))
    {
        char* reg_name = tokens[*pos].str + 1;
        ++(*pos);

        if (strcmp(reg_name, "eax") == 0) return cpu.eax;
        if (strcmp(reg_name, "ecx") == 0) return cpu.ecx;
        if (strcmp(reg_name, "edx") == 0) return cpu.edx;
        if (strcmp(reg_name, "ebx") == 0) return cpu.ebx;
        if (strcmp(reg_name, "esp") == 0) return cpu.esp;
        if (strcmp(reg_name, "ebp") == 0) return cpu.ebp;
        if (strcmp(reg_name, "esi") == 0) return cpu.esi;
        if (strcmp(reg_name, "edi") == 0) return cpu.edi;

        Log("unknown register %s", reg_name);
        return 0;
    }
    else if (check_token(*pos, TK_VAR))
    {
        TODO();
        return 0;
    }

    Log("unexpected token %s", tokens[*pos].str);
    return 0;
}