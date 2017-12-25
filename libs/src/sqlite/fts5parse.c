/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 47 "fts5parse.y"

#include "fts5Int.h"
#include "fts5parse.h"

/*
** Disable all error recovery processing in the parser push-down
** automaton.
*/
#define YYNOERRORRECOVERY 1

/*
** Make yytestcase() the same as testcase()
*/
#define yytestcase(X) testcase(X)

/*
** Indicate that sqlite3ParserFree() will never be called with a null
** pointer.
*/
#define YYPARSEFREENOTNULL 1

/*
** Alternative datatype for the argument to the malloc() routine passed
** into sqlite3ParserAlloc().  The default is size_t.
*/
#define YYMALLOCARGTYPE  u64

#line 56 "fts5parse.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    sqlite3Fts5ParserTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is sqlite3Fts5ParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    sqlite3Fts5ParserARG_SDECL     A static variable declaration for the %extra_argument
**    sqlite3Fts5ParserARG_PDECL     A parameter declaration for the %extra_argument
**    sqlite3Fts5ParserARG_STORE     Code to store %extra_argument into yypParser
**    sqlite3Fts5ParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Minimum value for reduce actions
**    YY_MAX_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 29
#define YYACTIONTYPE unsigned char
#define sqlite3Fts5ParserTOKENTYPE Fts5Token
typedef union {
  int yyinit;
  sqlite3Fts5ParserTOKENTYPE yy0;
  int yy4;
  Fts5ExprPhrase* yy11;
  Fts5ExprNearset* yy14;
  Fts5Colset* yy43;
  Fts5ExprNode* yy54;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define sqlite3Fts5ParserARG_SDECL Fts5Parse *pParse;
#define sqlite3Fts5ParserARG_PDECL ,Fts5Parse *pParse
#define sqlite3Fts5ParserARG_FETCH Fts5Parse *pParse = yypParser->pParse
#define sqlite3Fts5ParserARG_STORE yypParser->pParse = pParse
#define YYNSTATE             35
#define YYNRULE              28
#define YY_MAX_SHIFT         34
#define YY_MIN_SHIFTREDUCE   52
#define YY_MAX_SHIFTREDUCE   79
#define YY_MIN_REDUCE        80
#define YY_MAX_REDUCE        107
#define YY_ERROR_ACTION      108
#define YY_ACCEPT_ACTION     109
#define YY_NO_ACTION         110
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if:
**    (1)  The yy_shift_ofst[S]+X value is out of range, or
**    (2)  yy_lookahead[yy_shift_ofst[S]+X] is not equal to X, or
**    (3)  yy_shift_ofst[S] equal YY_SHIFT_USE_DFLT.
** (Implementation note: YY_SHIFT_USE_DFLT is chosen so that
** YY_SHIFT_USE_DFLT+X will be out of range for all possible lookaheads X.
** Hence only tests (1) and (2) need to be evaluated.)
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (105)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   109,   20,   93,    6,   28,   96,   95,   26,   26,   18,
 /*    10 */    93,    6,   28,   17,   95,   56,   26,   19,   93,    6,
 /*    20 */    28,   14,   95,  105,   26,   89,   93,    6,   28,   25,
 /*    30 */    95,   78,   26,   21,   93,    6,   28,  104,   95,   58,
 /*    40 */    26,   29,   93,    6,   28,   32,   95,   22,   26,   24,
 /*    50 */    16,   23,   11,    1,   14,   13,   24,   16,   31,   11,
 /*    60 */     3,   94,   13,   27,    8,   95,   82,   26,    7,    4,
 /*    70 */     5,    3,    4,    5,    3,   80,    4,    5,    3,   63,
 /*    80 */    33,   34,   62,   12,    2,   83,   13,   10,   12,   71,
 /*    90 */    10,   13,   78,    5,    3,   78,    9,   30,   75,   82,
 /*   100 */    54,   57,   53,   57,   15,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    17,   18,   19,   20,   21,   23,   23,   25,   25,   18,
 /*    10 */    19,   20,   21,    7,   23,    9,   25,   18,   19,   20,
 /*    20 */    21,    9,   23,   27,   25,   18,   19,   20,   21,   25,
 /*    30 */    23,   15,   25,   18,   19,   20,   21,   27,   23,    9,
 /*    40 */    25,   18,   19,   20,   21,   14,   23,   22,   25,    6,
 /*    50 */     7,   22,    9,   10,    9,   12,    6,    7,   13,    9,
 /*    60 */     3,   19,   12,   21,    5,   23,   28,   25,    5,    1,
 /*    70 */     2,    3,    1,    2,    3,    0,    1,    2,    3,   11,
 /*    80 */    25,   26,   11,    9,   10,    5,   12,   10,    9,   11,
 /*    90 */    10,   12,   15,    2,    3,   15,   24,   25,    9,   28,
 /*   100 */     8,    9,    8,    9,    9,
};
#define YY_SHIFT_USE_DFLT (105)
#define YY_SHIFT_COUNT    (34)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (95)
static const unsigned char yy_shift_ofst[] = {
 /*     0 */    43,   43,   43,   43,   43,   43,   50,   74,   79,   45,
 /*    10 */    12,   80,   77,   12,   16,   16,   30,   30,   68,   71,
 /*    20 */    75,   91,   92,   94,    6,   31,   31,   59,   63,   57,
 /*    30 */    31,   89,   95,   31,   78,
};
#define YY_REDUCE_USE_DFLT (-19)
#define YY_REDUCE_COUNT (17)
#define YY_REDUCE_MIN   (-18)
#define YY_REDUCE_MAX   (72)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -17,   -9,   -1,    7,   15,   23,   42,  -18,  -18,   55,
 /*    10 */    72,   -4,   -4,    4,   -4,   10,   25,   29,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   108,  108,  108,  108,  108,  108,   92,  108,  108,  102,
 /*    10 */   108,  107,  107,  108,  107,  107,  108,  108,  108,  108,
 /*    20 */   108,   88,  108,  108,  108,   98,   97,  108,  108,   87,
 /*    30 */   100,  108,  108,  101,  108,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  sqlite3Fts5ParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
  yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void sqlite3Fts5ParserTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "OR",            "AND",           "NOT",         
  "TERM",          "COLON",         "MINUS",         "LCP",         
  "RCP",           "STRING",        "LP",            "RP",          
  "CARET",         "COMMA",         "PLUS",          "STAR",        
  "error",         "input",         "expr",          "cnearset",    
  "exprlist",      "colset",        "colsetlist",    "nearset",     
  "nearphrases",   "phrase",        "neardist_opt",  "star_opt",    
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "input ::= expr",
 /*   1 */ "colset ::= MINUS LCP colsetlist RCP",
 /*   2 */ "colset ::= LCP colsetlist RCP",
 /*   3 */ "colset ::= STRING",
 /*   4 */ "colset ::= MINUS STRING",
 /*   5 */ "colsetlist ::= colsetlist STRING",
 /*   6 */ "colsetlist ::= STRING",
 /*   7 */ "expr ::= expr AND expr",
 /*   8 */ "expr ::= expr OR expr",
 /*   9 */ "expr ::= expr NOT expr",
 /*  10 */ "expr ::= colset COLON LP expr RP",
 /*  11 */ "expr ::= LP expr RP",
 /*  12 */ "expr ::= exprlist",
 /*  13 */ "exprlist ::= cnearset",
 /*  14 */ "exprlist ::= exprlist cnearset",
 /*  15 */ "cnearset ::= nearset",
 /*  16 */ "cnearset ::= colset COLON nearset",
 /*  17 */ "nearset ::= phrase",
 /*  18 */ "nearset ::= CARET phrase",
 /*  19 */ "nearset ::= STRING LP nearphrases neardist_opt RP",
 /*  20 */ "nearphrases ::= phrase",
 /*  21 */ "nearphrases ::= nearphrases phrase",
 /*  22 */ "neardist_opt ::=",
 /*  23 */ "neardist_opt ::= COMMA STRING",
 /*  24 */ "phrase ::= phrase PLUS STRING star_opt",
 /*  25 */ "phrase ::= STRING star_opt",
 /*  26 */ "star_opt ::= STAR",
 /*  27 */ "star_opt ::=",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to sqlite3Fts5ParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void sqlite3Fts5ParserInit(void *yypParser){
  yyParser *pParser = (yyParser*)yypParser;
#ifdef YYTRACKMAXSTACKDEPTH
  pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  pParser->yytos = NULL;
  pParser->yystack = NULL;
  pParser->yystksz = 0;
  if( yyGrowStack(pParser) ){
    pParser->yystack = &pParser->yystk0;
    pParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  pParser->yyerrcnt = -1;
#endif
  pParser->yytos = pParser->yystack;
  pParser->yystack[0].stateno = 0;
  pParser->yystack[0].major = 0;
#if YYSTACKDEPTH>0
  pParser->yystackEnd = &pParser->yystack[YYSTACKDEPTH-1];
#endif
}

#ifndef sqlite3Fts5Parser_ENGINEALWAYSONSTACK
/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to sqlite3Fts5Parser and sqlite3Fts5ParserFree.
*/
void *sqlite3Fts5ParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) sqlite3Fts5ParserInit(pParser);
  return pParser;
}
#endif /* sqlite3Fts5Parser_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  sqlite3Fts5ParserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
    case 17: /* input */
{
#line 83 "fts5parse.y"
 (void)pParse; 
#line 538 "fts5parse.c"
}
      break;
    case 18: /* expr */
    case 19: /* cnearset */
    case 20: /* exprlist */
{
#line 89 "fts5parse.y"
 sqlite3Fts5ParseNodeFree((yypminor->yy54)); 
#line 547 "fts5parse.c"
}
      break;
    case 21: /* colset */
    case 22: /* colsetlist */
{
#line 93 "fts5parse.y"
 sqlite3_free((yypminor->yy43)); 
#line 555 "fts5parse.c"
}
      break;
    case 23: /* nearset */
    case 24: /* nearphrases */
{
#line 148 "fts5parse.y"
 sqlite3Fts5ParseNearsetFree((yypminor->yy14)); 
#line 563 "fts5parse.c"
}
      break;
    case 25: /* phrase */
{
#line 183 "fts5parse.y"
 sqlite3Fts5ParsePhraseFree((yypminor->yy11)); 
#line 570 "fts5parse.c"
}
      break;
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void sqlite3Fts5ParserFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef sqlite3Fts5Parser_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void sqlite3Fts5ParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  sqlite3Fts5ParserFinalize(p);
  (*freeProc)(p);
}
#endif /* sqlite3Fts5Parser_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int sqlite3Fts5ParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   sqlite3Fts5ParserARG_FETCH;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
#line 36 "fts5parse.y"

  sqlite3Fts5ParseError(pParse, "fts5: parser stack overflow");
#line 752 "fts5parse.c"
/******** End %stack_overflow code ********************************************/
   sqlite3Fts5ParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  sqlite3Fts5ParserTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>yypParser->yystackEnd ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  { 17, -1 },
  { 21, -4 },
  { 21, -3 },
  { 21, -1 },
  { 21, -2 },
  { 22, -2 },
  { 22, -1 },
  { 18, -3 },
  { 18, -3 },
  { 18, -3 },
  { 18, -5 },
  { 18, -3 },
  { 18, -1 },
  { 20, -1 },
  { 20, -2 },
  { 19, -1 },
  { 19, -3 },
  { 23, -1 },
  { 23, -2 },
  { 23, -5 },
  { 24, -1 },
  { 24, -2 },
  { 26, 0 },
  { 26, -2 },
  { 25, -4 },
  { 25, -2 },
  { 27, -1 },
  { 27, 0 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
**
** The yyLookahead and yyLookaheadToken parameters provide reduce actions
** access to the lookahead token (if any).  The yyLookahead will be YYNOCODE
** if the lookahead token has already been consumed.  As this procedure is
** only called from one place, optimizing compilers will in-line it, which
** means that the extra parameters have no performance impact.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno,       /* Number of the rule by which to reduce */
  int yyLookahead,             /* Lookahead token, or YYNOCODE if none */
  sqlite3Fts5ParserTOKENTYPE yyLookaheadToken  /* Value of the lookahead token */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  sqlite3Fts5ParserARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=yypParser->yystackEnd ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* input ::= expr */
#line 82 "fts5parse.y"
{ sqlite3Fts5ParseFinished(pParse, yymsp[0].minor.yy54); }
#line 928 "fts5parse.c"
        break;
      case 1: /* colset ::= MINUS LCP colsetlist RCP */
#line 97 "fts5parse.y"
{ 
    yymsp[-3].minor.yy43 = sqlite3Fts5ParseColsetInvert(pParse, yymsp[-1].minor.yy43);
}
#line 935 "fts5parse.c"
        break;
      case 2: /* colset ::= LCP colsetlist RCP */
#line 100 "fts5parse.y"
{ yymsp[-2].minor.yy43 = yymsp[-1].minor.yy43; }
#line 940 "fts5parse.c"
        break;
      case 3: /* colset ::= STRING */
#line 101 "fts5parse.y"
{
  yylhsminor.yy43 = sqlite3Fts5ParseColset(pParse, 0, &yymsp[0].minor.yy0);
}
#line 947 "fts5parse.c"
  yymsp[0].minor.yy43 = yylhsminor.yy43;
        break;
      case 4: /* colset ::= MINUS STRING */
#line 104 "fts5parse.y"
{
  yymsp[-1].minor.yy43 = sqlite3Fts5ParseColset(pParse, 0, &yymsp[0].minor.yy0);
  yymsp[-1].minor.yy43 = sqlite3Fts5ParseColsetInvert(pParse, yymsp[-1].minor.yy43);
}
#line 956 "fts5parse.c"
        break;
      case 5: /* colsetlist ::= colsetlist STRING */
#line 109 "fts5parse.y"
{ 
  yylhsminor.yy43 = sqlite3Fts5ParseColset(pParse, yymsp[-1].minor.yy43, &yymsp[0].minor.yy0); }
#line 962 "fts5parse.c"
  yymsp[-1].minor.yy43 = yylhsminor.yy43;
        break;
      case 6: /* colsetlist ::= STRING */
#line 111 "fts5parse.y"
{ 
  yylhsminor.yy43 = sqlite3Fts5ParseColset(pParse, 0, &yymsp[0].minor.yy0); 
}
#line 970 "fts5parse.c"
  yymsp[0].minor.yy43 = yylhsminor.yy43;
        break;
      case 7: /* expr ::= expr AND expr */
#line 115 "fts5parse.y"
{
  yylhsminor.yy54 = sqlite3Fts5ParseNode(pParse, FTS5_AND, yymsp[-2].minor.yy54, yymsp[0].minor.yy54, 0);
}
#line 978 "fts5parse.c"
  yymsp[-2].minor.yy54 = yylhsminor.yy54;
        break;
      case 8: /* expr ::= expr OR expr */
#line 118 "fts5parse.y"
{
  yylhsminor.yy54 = sqlite3Fts5ParseNode(pParse, FTS5_OR, yymsp[-2].minor.yy54, yymsp[0].minor.yy54, 0);
}
#line 986 "fts5parse.c"
  yymsp[-2].minor.yy54 = yylhsminor.yy54;
        break;
      case 9: /* expr ::= expr NOT expr */
#line 121 "fts5parse.y"
{
  yylhsminor.yy54 = sqlite3Fts5ParseNode(pParse, FTS5_NOT, yymsp[-2].minor.yy54, yymsp[0].minor.yy54, 0);
}
#line 994 "fts5parse.c"
  yymsp[-2].minor.yy54 = yylhsminor.yy54;
        break;
      case 10: /* expr ::= colset COLON LP expr RP */
#line 125 "fts5parse.y"
{
  sqlite3Fts5ParseSetColset(pParse, yymsp[-1].minor.yy54, yymsp[-4].minor.yy43);
  yylhsminor.yy54 = yymsp[-1].minor.yy54;
}
#line 1003 "fts5parse.c"
  yymsp[-4].minor.yy54 = yylhsminor.yy54;
        break;
      case 11: /* expr ::= LP expr RP */
#line 129 "fts5parse.y"
{yymsp[-2].minor.yy54 = yymsp[-1].minor.yy54;}
#line 1009 "fts5parse.c"
        break;
      case 12: /* expr ::= exprlist */
      case 13: /* exprlist ::= cnearset */ yytestcase(yyruleno==13);
#line 130 "fts5parse.y"
{yylhsminor.yy54 = yymsp[0].minor.yy54;}
#line 1015 "fts5parse.c"
  yymsp[0].minor.yy54 = yylhsminor.yy54;
        break;
      case 14: /* exprlist ::= exprlist cnearset */
#line 133 "fts5parse.y"
{
  yylhsminor.yy54 = sqlite3Fts5ParseImplicitAnd(pParse, yymsp[-1].minor.yy54, yymsp[0].minor.yy54);
}
#line 1023 "fts5parse.c"
  yymsp[-1].minor.yy54 = yylhsminor.yy54;
        break;
      case 15: /* cnearset ::= nearset */
#line 137 "fts5parse.y"
{ 
  yylhsminor.yy54 = sqlite3Fts5ParseNode(pParse, FTS5_STRING, 0, 0, yymsp[0].minor.yy14); 
}
#line 1031 "fts5parse.c"
  yymsp[0].minor.yy54 = yylhsminor.yy54;
        break;
      case 16: /* cnearset ::= colset COLON nearset */
#line 140 "fts5parse.y"
{ 
  yylhsminor.yy54 = sqlite3Fts5ParseNode(pParse, FTS5_STRING, 0, 0, yymsp[0].minor.yy14); 
  sqlite3Fts5ParseSetColset(pParse, yylhsminor.yy54, yymsp[-2].minor.yy43);
}
#line 1040 "fts5parse.c"
  yymsp[-2].minor.yy54 = yylhsminor.yy54;
        break;
      case 17: /* nearset ::= phrase */
#line 151 "fts5parse.y"
{ yylhsminor.yy14 = sqlite3Fts5ParseNearset(pParse, 0, yymsp[0].minor.yy11); }
#line 1046 "fts5parse.c"
  yymsp[0].minor.yy14 = yylhsminor.yy14;
        break;
      case 18: /* nearset ::= CARET phrase */
#line 152 "fts5parse.y"
{ 
  sqlite3Fts5ParseSetCaret(yymsp[0].minor.yy11);
  yymsp[-1].minor.yy14 = sqlite3Fts5ParseNearset(pParse, 0, yymsp[0].minor.yy11); 
}
#line 1055 "fts5parse.c"
        break;
      case 19: /* nearset ::= STRING LP nearphrases neardist_opt RP */
#line 156 "fts5parse.y"
{
  sqlite3Fts5ParseNear(pParse, &yymsp[-4].minor.yy0);
  sqlite3Fts5ParseSetDistance(pParse, yymsp[-2].minor.yy14, &yymsp[-1].minor.yy0);
  yylhsminor.yy14 = yymsp[-2].minor.yy14;
}
#line 1064 "fts5parse.c"
  yymsp[-4].minor.yy14 = yylhsminor.yy14;
        break;
      case 20: /* nearphrases ::= phrase */
#line 162 "fts5parse.y"
{ 
  yylhsminor.yy14 = sqlite3Fts5ParseNearset(pParse, 0, yymsp[0].minor.yy11); 
}
#line 1072 "fts5parse.c"
  yymsp[0].minor.yy14 = yylhsminor.yy14;
        break;
      case 21: /* nearphrases ::= nearphrases phrase */
#line 165 "fts5parse.y"
{
  yylhsminor.yy14 = sqlite3Fts5ParseNearset(pParse, yymsp[-1].minor.yy14, yymsp[0].minor.yy11);
}
#line 1080 "fts5parse.c"
  yymsp[-1].minor.yy14 = yylhsminor.yy14;
        break;
      case 22: /* neardist_opt ::= */
#line 172 "fts5parse.y"
{ yymsp[1].minor.yy0.p = 0; yymsp[1].minor.yy0.n = 0; }
#line 1086 "fts5parse.c"
        break;
      case 23: /* neardist_opt ::= COMMA STRING */
#line 173 "fts5parse.y"
{ yymsp[-1].minor.yy0 = yymsp[0].minor.yy0; }
#line 1091 "fts5parse.c"
        break;
      case 24: /* phrase ::= phrase PLUS STRING star_opt */
#line 185 "fts5parse.y"
{ 
  yylhsminor.yy11 = sqlite3Fts5ParseTerm(pParse, yymsp[-3].minor.yy11, &yymsp[-1].minor.yy0, yymsp[0].minor.yy4);
}
#line 1098 "fts5parse.c"
  yymsp[-3].minor.yy11 = yylhsminor.yy11;
        break;
      case 25: /* phrase ::= STRING star_opt */
#line 188 "fts5parse.y"
{ 
  yylhsminor.yy11 = sqlite3Fts5ParseTerm(pParse, 0, &yymsp[-1].minor.yy0, yymsp[0].minor.yy4);
}
#line 1106 "fts5parse.c"
  yymsp[-1].minor.yy11 = yylhsminor.yy11;
        break;
      case 26: /* star_opt ::= STAR */
#line 196 "fts5parse.y"
{ yymsp[0].minor.yy4 = 1; }
#line 1112 "fts5parse.c"
        break;
      case 27: /* star_opt ::= */
#line 197 "fts5parse.y"
{ yymsp[1].minor.yy4 = 0; }
#line 1117 "fts5parse.c"
        break;
      default:
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[yysize].stateno,(YYCODETYPE)yygoto);

  /* There are no SHIFTREDUCE actions on nonterminals because the table
  ** generator has simplified them to pure REDUCE actions. */
  assert( !(yyact>YY_MAX_SHIFT && yyact<=YY_MAX_SHIFTREDUCE) );

  /* It is not possible for a REDUCE to be followed by an error */
  assert( yyact!=YY_ERROR_ACTION );

  if( yyact==YY_ACCEPT_ACTION ){
    yypParser->yytos += yysize;
    yy_accept(yypParser);
  }else{
    yymsp += yysize+1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  sqlite3Fts5ParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  sqlite3Fts5ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  sqlite3Fts5ParserTOKENTYPE yyminor         /* The minor type of the error token */
){
  sqlite3Fts5ParserARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 30 "fts5parse.y"

  UNUSED_PARAM(yymajor); /* Silence a compiler warning */
  sqlite3Fts5ParseError(
    pParse, "fts5: syntax error near \"%.*s\"",TOKEN.n,TOKEN.p
  );
#line 1186 "fts5parse.c"
/************ End %syntax_error code ******************************************/
  sqlite3Fts5ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  sqlite3Fts5ParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  sqlite3Fts5ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "sqlite3Fts5ParserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void sqlite3Fts5Parser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  sqlite3Fts5ParserTOKENTYPE yyminor       /* The value for the token */
  sqlite3Fts5ParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  sqlite3Fts5ParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE,yymajor,yyminor);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
