#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <sys/types.h>

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

    // TODO: Add more token types

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

    /* TODO: Add more rules.
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

Token tokens[32];
int   nr_token;

#define EXPRDBG_LOG_ENABLE

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
                Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
                    i,
                    rules[i].regex,
                    position,
                    substr_len,
                    substr_len,
                    substr_start);
#endif

                position += substr_len;

                /* TODO: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array `tokens'. For certain types
                 * of tokens, some extra actions should be performed.
                 */
                /*
                                switch (rules[i].token_type)
                                {
                                    default: TODO();
                                }
                */

                if (rules[i].token_type == TK_NOTYPE) break;

                if (nr_token >= 32) {
                    Log("too many tokens");
                    return false;
                }

                tokens[nr_token].type          = rules[i].token_type;
                if (substr_len > 3) substr_len = 31;
                strncpy(tokens[nr_token].str, substr_start, substr_len);
                tokens[nr_token].str[substr_len] = '\0';
                nr_token++;

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

uint32_t expr(char* e, bool* success)
{
    if (!make_token(e)) {
        *success = false;
        return 0;
    }

    /* TODO: Insert codes to evaluate the expression. */
    // TODO();

    return 0;
}
