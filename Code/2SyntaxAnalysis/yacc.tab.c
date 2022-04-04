/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "yacc.y" /* yacc.c:339  */

#include "main.h"
#include "yacc.tab.h"

extern "C"
{
	int yyparse();  //分析器的入口函数
	void yyerror(const char *s);    //检测到语法错误时报告，参数为描述错误的字符串
	extern int yylex(); //在lex.yy.c中定义 yyparse()需要调用该函数
}

extern YYSTYPE yylval;
extern int yylineno;
extern char* yytext;
extern char lineBuffer[500];
extern int yyleng;
extern int yycolumn;
extern string itos(int num);

Token* ParseTreeHead=NULL;
bool haveSemanticError=false;
vector<string> syntaxErrorInformation; //存放语法错误信息

void yyerror(const char *s, YYLTYPE *loc);
void yyerror(const char *s, int line, int col);
void yyerror(const char *s, int startLine, int startCol, int endLine, int endCol);


#line 95 "yacc.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "yacc.tab.h".  */
#ifndef YY_YY_YACC_TAB_H_INCLUDED
# define YY_YY_YACC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PROGRAM = 258,
    CONST = 259,
    VAR = 260,
    ARRAY = 261,
    OF = 262,
    PROCEDURE = 263,
    FUNCTION = 264,
    BEGIN = 265,
    END = 266,
    IF = 267,
    THEN = 268,
    CASE = 269,
    FOR = 270,
    TO = 271,
    DOWNTO = 272,
    DO = 273,
    ELSE = 274,
    REPEAT = 275,
    UNTIL = 276,
    WHILE = 277,
    IDENTIFIER = 278,
    UINUM = 279,
    UFNUM = 280,
    CHAR = 281,
    LETTER = 282,
    INTEGER = 283,
    TYPE = 284,
    ASSIGNOP = 285,
    RELOP = 286,
    ADDOP = 287,
    MULOP = 288,
    NOT = 289,
    RECORD = 290,
    REAL = 291,
    BOOLEAN = 292,
    ADD = 293,
    MUL = 294
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_YACC_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 200 "yacc.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   632

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  195
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  422

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    50,
      45,    46,    41,    38,    48,    39,    44,    42,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    53,    47,
       2,    49,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    51,     2,    52,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    40,    43
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    76,    76,    83,    86,    89,    94,   102,   105,   108,
     111,   114,   119,   126,   129,   134,   139,   144,   149,   152,
     155,   159,   166,   171,   174,   177,   180,   183,   186,   189,
     192,   198,   202,   206,   209,   213,   217,   221,   225,   228,
     231,   240,   245,   247,   250,   253,   258,   265,   270,   273,
     276,   279,   282,   287,   290,   295,   303,   306,   309,   312,
     315,   318,   321,   324,   330,   334,   337,   340,   345,   348,
     352,   357,   360,   363,   368,   374,   377,   380,   385,   390,
     392,   395,   398,   403,   410,   415,   418,   421,   424,   427,
     430,   435,   440,   442,   447,   451,   454,   459,   467,   473,
     476,   479,   482,   485,   488,   491,   494,   497,   500,   505,
     510,   512,   515,   518,   523,   528,   531,   534,   539,   542,
     549,   553,   559,   564,   567,   570,   575,   580,   585,   585,
     593,   598,   603,   606,   609,   616,   619,   626,   629,   632,
     638,   641,   647,   650,   660,   663,   666,   669,   673,   679,
     683,   687,   687,   695,   701,   705,   709,   712,   716,   716,
     724,   729,   729,   739,   739,   747,   752,   755,   760,   763,
     769,   774,   774,   782,   787,   792,   797,   800,   804,   808,
     813,   818,   823,   826,   831,   834,   837,   843,   846,   849,
     854,   857,   863,   866,   869,   872
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM", "CONST", "VAR", "ARRAY", "OF",
  "PROCEDURE", "FUNCTION", "BEGIN", "END", "IF", "THEN", "CASE", "FOR",
  "TO", "DOWNTO", "DO", "ELSE", "REPEAT", "UNTIL", "WHILE", "IDENTIFIER",
  "UINUM", "UFNUM", "CHAR", "LETTER", "INTEGER", "TYPE", "ASSIGNOP",
  "RELOP", "ADDOP", "MULOP", "NOT", "RECORD", "REAL", "BOOLEAN", "'+'",
  "'-'", "ADD", "'*'", "'/'", "MUL", "'.'", "'('", "')'", "';'", "','",
  "'='", "'\\''", "'['", "']'", "':'", "$accept", "programstruct",
  "program_head", "program_body", "idlist", "const_declarations",
  "const_declaration", "const_variable", "type_declarations",
  "type_declaration", "type", "standard_type", "record_body", "periods",
  "period", "var_declarations", "var_declaration",
  "subprogram_declarations", "subprogram_declaration", "subprogram_head",
  "formal_parameter", "parameter_lists", "parameter_list", "var_parameter",
  "value_parameter", "compound_statement", "statement_list", "@1",
  "statement", "variable", "id_varparts", "id_varpart", "@2", "else_part",
  "case_body", "branch_list", "@3", "branch", "@4", "const_list", "@5",
  "updown", "procedure_call", "expression_list", "@6", "expression",
  "simple_expression", "term", "factor", "unsign_const", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,    43,    45,
     293,    42,    47,   294,    46,    40,    41,    59,    44,    61,
      39,    91,    93,    58
};
# endif

#define YYPACT_NINF -238

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-238)))

#define YYTABLE_NINF -151

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     395,   453,    23,   168,   466,   499,    33,    -7,   499,    62,
     127,  -238,   332,   156,   198,   385,   568,   382,    50,   145,
    -238,   568,   175,   175,   388,  -238,  -238,  -238,   216,    56,
     180,   475,   247,   175,   432,  -238,   250,   491,   491,   111,
     320,   476,  -238,  -238,   442,   522,   325,   128,   366,   366,
     369,   336,   423,  -238,   336,   423,    15,   291,   418,   305,
     493,   457,  -238,  -238,  -238,   495,   561,   486,  -238,  -238,
    -238,  -238,   474,   478,   107,   573,   502,   529,   515,   518,
     528,   306,   116,    19,  -238,  -238,   175,  -238,  -238,  -238,
    -238,  -238,   323,  -238,   540,   113,   366,   381,   175,    22,
    -238,   175,   530,   443,   433,   436,   544,   307,   488,  -238,
     491,  -238,  -238,  -238,  -238,  -238,  -238,   541,   491,   491,
     491,   185,   316,  -238,  -238,  -238,  -238,  -238,  -238,  -238,
    -238,   491,   579,   211,   491,   190,   205,   366,   366,   366,
    -238,   323,  -238,   392,   539,    16,    27,  -238,   204,   225,
      26,    42,   487,   487,   570,   544,   487,   506,  -238,   268,
    -238,   564,  -238,  -238,  -238,   233,  -238,  -238,  -238,  -238,
    -238,  -238,  -238,  -238,   238,    52,  -238,  -238,   437,    58,
     543,    61,  -238,  -238,  -238,  -238,  -238,   366,   366,   366,
     384,    31,   346,   -18,   549,   326,   308,   545,   122,    37,
     552,  -238,  -238,   510,   510,   510,   487,   572,  -238,   322,
     112,   567,  -238,  -238,   140,    25,   229,    21,   487,   -11,
    -238,  -238,   544,   487,   557,   313,   491,   236,   595,   596,
     597,   341,   343,  -238,  -238,  -238,   323,  -238,   441,    43,
     137,  -238,  -238,  -238,   559,   282,  -238,   535,  -238,  -238,
    -238,  -238,   535,   535,   535,   399,    92,  -238,   567,   567,
     117,   556,   544,   544,   487,   243,   277,   491,   491,   487,
     487,   487,   487,   544,   544,   147,  -238,   584,   487,  -238,
    -238,  -238,   491,   491,   280,  -238,  -238,  -238,   366,   366,
     366,   366,   366,   387,  -238,  -238,   580,   413,    31,  -238,
     376,  -238,    31,  -238,   563,   565,   566,   569,   571,   309,
    -238,   172,  -238,  -238,  -238,   589,   589,   582,   510,   567,
     510,  -238,  -238,   600,   574,  -238,   430,   206,   463,   367,
    -238,  -238,  -238,  -238,  -238,  -238,   487,  -238,   439,   544,
    -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,   116,
    -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,  -238,
    -238,  -238,  -238,  -238,  -238,  -238,   544,  -238,  -238,   567,
    -238,  -238,   491,   491,   544,  -238,  -238,  -238,  -238,   487,
     487,   487,  -238,  -238,   311,  -238,  -238,  -238,  -238,   599,
     601,   425,   487,   575,   544,   491,   491,   491,   544,   544,
     544,   544,   110,   487,  -238,   315,   132,   139,  -238,  -238,
    -238,  -238,   487,   163,   491,   491,   544,  -238,  -238,  -238,
    -238,  -238
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       3,     0,     0,     0,     0,     4,     5,     2,     0,     0,
       0,     0,     0,     0,     0,    92,    18,     0,     0,     0,
      19,     0,    92,    16,     0,     0,     0,     0,     0,     0,
       0,     0,    43,    44,    45,    41,     0,     0,     0,     0,
       0,     0,    33,    38,    39,     0,     0,     0,    28,    29,
      30,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,     0,    67,    64,    69,    65,    66,    50,
      53,    51,    52,    47,     0,     0,     0,     0,     0,     0,
      81,    82,     0,     0,     0,     0,   147,     0,     0,    13,
       0,    31,    34,    35,    32,    36,    37,     0,     0,     0,
       0,     0,     0,    12,     7,    15,     8,     9,    10,    11,
       6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    84,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,   147,     0,   168,   133,     0,
     130,     0,   132,    93,    91,     0,    94,    23,    40,    24,
      25,    26,    27,    21,     0,     0,    71,    56,     0,     0,
       0,     0,    57,    54,    48,    49,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     150,   194,   193,     0,     0,     0,     0,     0,   185,     0,
     175,   176,   183,   184,     0,     0,     0,     0,     0,   148,
     127,   126,   147,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    86,    87,    88,    83,     0,     0,
       0,   115,   118,   119,     0,     0,   105,     0,   106,   107,
     108,    98,     0,     0,     0,     0,     0,   191,   177,   178,
       0,     0,   147,   147,     0,     0,     0,   157,   157,     0,
       0,     0,     0,   147,   147,     0,   173,     0,     0,   149,
     128,   131,     0,     0,     0,    72,    73,    70,     0,     0,
       0,     0,     0,     0,   121,   120,   125,     0,     0,   111,
       0,   112,   113,   109,     0,     0,     0,     0,     0,     0,
     188,     0,   190,   189,   195,   155,   155,   174,     0,   179,
       0,   181,   165,     0,   156,   160,     0,     0,     0,     0,
     142,   141,   140,   139,   170,   169,     0,   153,     0,   147,
      75,    76,    77,    74,    58,    59,    60,    61,    62,    63,
      55,   123,   124,   122,   116,   117,   114,    99,   100,   101,
     102,   103,   104,    97,   187,   186,   147,   135,   134,   180,
     182,   137,     0,     0,   147,   138,   136,   166,   167,     0,
       0,     0,   171,   151,     0,   154,   158,   163,   161,     0,
       0,     0,     0,     0,   147,     0,     0,     0,   147,   147,
     147,   147,     0,     0,   129,     0,     0,     0,   144,   145,
     146,   143,     0,     0,     0,     0,   147,   172,   152,   159,
     164,   162
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -238,  -238,  -238,    17,   -13,  -238,  -238,   -37,   607,   605,
     -36,  -205,   536,   408,   242,   602,   546,   583,  -238,  -238,
     397,   435,   130,  -238,   386,   224,  -143,  -238,  -197,  -103,
    -238,  -238,  -238,   312,   361,   235,  -238,   -82,  -238,   191,
    -238,   302,  -238,  -237,  -238,  -148,   368,  -198,  -188,  -238
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,   147,    56,     8,    19,   322,    16,    30,
      89,    90,   132,   175,   176,    35,   133,    60,   107,   108,
     193,   240,   241,   242,   243,   158,   159,   339,   160,   208,
     219,   279,   393,   367,   323,   324,   395,   325,   397,   326,
     396,   379,   162,   275,   392,   276,   210,   211,   212,   213
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      68,    69,    71,   161,   209,   214,   258,   259,   217,    44,
      45,    47,   216,    91,    93,   257,    96,   189,     7,   311,
     134,    13,   273,   144,     9,   280,   269,   191,   191,   246,
     -80,   -80,   -80,   277,    17,   247,   238,    20,   254,   274,
     278,   338,   304,   198,   296,    43,    10,   305,   306,   307,
     309,    38,   161,   226,    43,   270,    18,    49,   260,   226,
     140,   142,   231,    77,    77,   315,   316,   319,    97,   190,
     135,   192,   192,   167,  -110,   281,   332,   333,   321,  -110,
    -110,   169,   170,   171,   173,   143,   145,   192,   143,   145,
     255,    77,   353,   310,   174,  -110,   297,   174,   174,    39,
     227,   184,   185,   186,   228,    50,   227,    22,   120,   227,
     229,   412,    70,   232,   138,   200,   201,   202,   312,   161,
     369,   328,   329,   330,   331,   166,   203,   238,    23,    80,
     204,   205,   370,   415,    62,    63,    64,   206,   298,    43,
     416,   267,   207,   264,   265,    43,    40,   268,   334,    65,
      66,   233,   234,   235,   237,   402,   121,    26,   336,   161,
     161,    67,   139,   313,   418,   145,   413,   131,    11,   385,
     161,   161,    24,   364,    81,   253,    77,   388,   239,   239,
     373,    51,   239,   299,   300,   239,   172,   373,   382,   174,
     174,   180,    41,   335,   174,   336,   384,   404,    43,    28,
      27,   408,   409,   410,   411,   191,   182,   375,    62,    63,
      64,   336,    98,    62,    63,    64,   183,   376,   365,   421,
     336,    29,   -68,    65,    66,   239,   195,    52,    65,    66,
     271,   389,   390,   391,    14,    67,   161,   286,   -42,   224,
      67,   -42,   -42,   -42,   318,   340,   341,   343,    54,   192,
     272,  -110,   344,   345,   346,   347,   348,   350,   178,    62,
      63,    64,    15,   161,   417,    48,   200,   201,   202,   220,
     192,   161,  -110,    61,    65,    66,   222,   203,   320,   221,
     -96,   342,   225,   302,   109,   239,    67,   239,   206,   239,
     386,   161,    98,   207,    55,   161,   161,   161,   161,   123,
     200,   201,   202,    62,    63,    64,   101,   129,   163,   250,
     362,   203,   394,   161,   283,   222,   414,     5,    65,    66,
       6,   -20,   206,   262,   -20,   -20,   -20,   207,   303,   300,
      67,   238,   419,    14,   -69,   263,   387,   -42,    99,   146,
     -42,   -42,   -42,    72,   292,   -20,    43,   244,   291,    43,
     293,   238,   102,   130,   164,   251,   363,   284,   222,    94,
     -14,    15,   372,   -14,    62,    63,    64,    82,   380,    43,
      92,    79,    83,   249,   131,    83,    25,   355,   420,    65,
      66,   238,   141,   377,   378,   236,    31,    83,   349,    46,
      83,    67,    84,    83,    85,    84,     1,    85,     2,    43,
     308,    86,    87,    88,    86,    87,    88,    84,    29,    85,
      84,    43,    85,    84,   352,    85,    86,    87,    88,    86,
      87,    88,    86,    87,    88,    84,   400,    85,   354,    36,
     356,    37,   354,    58,   148,    87,    88,   150,   144,    84,
      77,    85,   294,   401,     5,   187,    95,     6,   -20,    87,
      88,   -20,   -20,   -20,     5,    43,   149,     6,   -20,   151,
      43,   -20,   -20,   -20,    43,   100,   146,    12,   285,   287,
       6,   -20,   -20,   285,   -20,   -20,   -20,    73,   373,   377,
     378,   -17,   -20,   374,   -17,   -17,   -17,   336,    76,   165,
      77,   383,     6,   -20,   103,   -20,   -20,   -20,   -20,    74,
      14,   104,   105,   106,   -42,   -17,   110,   -42,   -42,   -42,
     200,   201,   202,   117,    62,    63,    64,   -20,   111,   112,
     113,   203,    53,   118,    48,   204,   205,   119,    15,    65,
      66,   144,   206,   200,   201,   202,  -150,   207,   -78,   -78,
     -78,    67,   179,   181,   203,   194,   196,   197,   199,   124,
    -150,   218,   125,    43,   106,   206,   152,  -150,   153,   154,
     207,    84,   126,    85,   155,   127,   156,   157,    78,    33,
      77,    87,    88,    34,   122,   128,   -79,   -79,   -79,    57,
      59,   104,   105,   106,   114,   115,   116,   406,   407,   137,
     177,   168,   188,   215,   223,   230,   248,   256,   252,   261,
     266,   282,   288,   289,   290,   301,   314,   337,   366,   351,
     357,   371,   358,   359,   265,    21,   360,   398,   361,   399,
      32,   372,   136,    42,   295,    75,   403,   245,   368,   327,
     405,   381,   317
};

static const yytype_uint16 yycheck[] =
{
      37,    38,    39,   106,   152,   153,   204,   205,   156,    22,
      23,    24,   155,    49,    50,   203,     1,     1,     1,   256,
       1,     4,     1,     1,     1,   222,     1,     1,     1,    47,
       8,     9,    10,    44,     1,    53,     5,    44,     1,    18,
      51,   278,   247,     1,     1,    23,    23,   252,   253,   254,
     255,     1,   155,     1,    23,    30,    23,     1,   206,     1,
      96,    97,     1,    48,    48,   262,   263,   265,    53,    53,
      51,    45,    45,   110,    47,   223,   273,   274,   266,    53,
      53,   118,   119,   120,   121,    98,    99,    45,   101,   102,
      53,    48,   297,     1,   131,    53,    53,   134,   135,    49,
      48,   137,   138,   139,    52,    49,    48,    45,     1,    48,
      52,     1,     1,    52,     1,    23,    24,    25,     1,   222,
     318,   269,   270,   271,   272,   108,    34,     5,     1,     1,
      38,    39,   320,     1,    23,    24,    25,    45,     1,    23,
       1,     1,    50,    31,    32,    23,     1,     7,     1,    38,
      39,   187,   188,   189,   190,   392,    49,     1,    48,   262,
     263,    50,    49,    46,     1,   178,   403,    51,     0,   366,
     273,   274,    45,     1,    46,    53,    48,   374,   191,   192,
      48,     1,   195,    46,    47,   198,     1,    48,   336,   226,
     227,     1,    47,    46,   231,    48,   339,   394,    23,     1,
      44,   398,   399,   400,   401,     1,     1,     1,    23,    24,
      25,    48,     1,    23,    24,    25,    11,    11,    46,   416,
      48,    23,    11,    38,    39,   238,     1,    47,    38,    39,
       1,   379,   380,   381,     1,    50,   339,     1,     5,     1,
      50,     8,     9,    10,     1,   282,   283,   284,     1,    45,
      21,    47,   288,   289,   290,   291,   292,   293,    47,    23,
      24,    25,    29,   366,   412,    49,    23,    24,    25,     1,
      45,   374,    47,    23,    38,    39,    47,    34,     1,    11,
      47,     1,    44,     1,    60,   298,    50,   300,    45,   302,
     372,   394,     1,    50,    47,   398,   399,   400,   401,    75,
      23,    24,    25,    23,    24,    25,     1,     1,     1,     1,
       1,    34,     1,   416,     1,    47,     1,     1,    38,    39,
       4,     5,    45,     1,     8,     9,    10,    50,    46,    47,
      50,     5,   414,     1,    11,    13,   373,     5,    47,    23,
       8,     9,    10,    23,     1,    29,    23,     1,     7,    23,
       7,     5,    47,    47,    47,    47,    47,    44,    47,    23,
      44,    29,    47,    47,    23,    24,    25,     1,     1,    23,
       1,    46,     6,    47,    51,     6,    44,     1,   415,    38,
      39,     5,     1,    16,    17,     1,     1,     6,     1,     1,
       6,    50,    26,     6,    28,    26,     1,    28,     3,    23,
       1,    35,    36,    37,    35,    36,    37,    26,    23,    28,
      26,    23,    28,    26,     1,    28,    35,    36,    37,    35,
      36,    37,    35,    36,    37,    26,     1,    28,   298,    47,
     300,    49,   302,     1,     1,    36,    37,     1,     1,    26,
      48,    28,     1,    18,     1,    53,    23,     4,     5,    36,
      37,     8,     9,    10,     1,    23,    23,     4,     5,    23,
      23,     8,     9,    10,    23,    47,    23,     1,   226,   227,
       4,     5,    29,   231,     8,     9,    10,     1,    48,    16,
      17,     5,    29,    53,     8,     9,    10,    48,    46,     1,
      48,    52,     4,     5,     1,    29,     8,     9,    10,    23,
       1,     8,     9,    10,     5,    29,    49,     8,     9,    10,
      23,    24,    25,    27,    23,    24,    25,    29,    23,    24,
      25,    34,    47,    49,    49,    38,    39,    49,    29,    38,
      39,     1,    45,    23,    24,    25,    30,    50,     8,     9,
      10,    50,   134,   135,    34,   148,   149,   150,   151,    47,
      44,    45,    23,    23,    10,    45,    12,    51,    14,    15,
      50,    26,    47,    28,    20,    47,    22,    23,    46,     1,
      48,    36,    37,     5,     1,    47,     8,     9,    10,    33,
      34,     8,     9,    10,    23,    24,    25,   396,   397,    49,
      11,    50,    53,    23,    30,    52,    47,    45,    53,    27,
      33,    44,     7,     7,     7,    46,    50,    23,    19,    29,
      47,    11,    47,    47,    32,     8,    47,    18,    47,    18,
      15,    47,    86,    21,   238,    42,    51,   192,   316,   268,
     395,   329,   264
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,    55,    56,     1,     4,    57,    59,     1,
      23,     0,     1,    57,     1,    29,    62,     1,    23,    60,
      44,    62,    45,     1,    45,    44,     1,    44,     1,    23,
      63,     1,    63,     1,     5,    69,    47,    49,     1,    49,
       1,    47,    69,    23,    58,    58,     1,    58,    49,     1,
      49,     1,    47,    47,     1,    47,    58,    70,     1,    70,
      71,    23,    23,    24,    25,    38,    39,    50,    61,    61,
       1,    61,    23,     1,    23,    71,    46,    48,    46,    46,
       1,    46,     1,     6,    26,    28,    35,    36,    37,    64,
      65,    64,     1,    64,    23,    23,     1,    53,     1,    47,
      47,     1,    47,     1,     8,     9,    10,    72,    73,    79,
      49,    23,    24,    25,    23,    24,    25,    27,    49,    49,
       1,    49,     1,    79,    47,    23,    47,    47,    47,     1,
      47,    51,    66,    70,     1,    51,    66,    49,     1,    49,
      64,     1,    64,    58,     1,    58,    23,    57,     1,    23,
       1,    23,    12,    14,    15,    20,    22,    23,    79,    80,
      82,    83,    96,     1,    47,     1,    57,    61,    50,    61,
      61,    61,     1,    61,    61,    67,    68,    11,    47,    67,
       1,    67,     1,    11,    64,    64,    64,    53,    53,     1,
      53,     1,    45,    74,    74,     1,    74,    74,     1,    74,
      23,    24,    25,    34,    38,    39,    45,    50,    83,    99,
     100,   101,   102,   103,    99,    23,    80,    99,    45,    84,
       1,    11,    47,    30,     1,    44,     1,    48,    52,    52,
      52,     1,    52,    64,    64,    64,     1,    64,     5,    58,
      75,    76,    77,    78,     1,    75,    47,    53,    47,    47,
       1,    47,    53,    53,     1,    53,    45,   102,   101,   101,
      99,    27,     1,    13,    31,    32,    33,     1,     7,     1,
      30,     1,    21,     1,    18,    97,    99,    44,    51,    85,
      82,    99,    44,     1,    44,    68,     1,    68,     7,     7,
       7,     7,     1,     7,     1,    78,     1,    53,     1,    46,
      47,    46,     1,    46,    65,    65,    65,    65,     1,    65,
       1,    97,     1,    46,    50,    82,    82,   100,     1,   101,
       1,   102,    61,    88,    89,    91,    93,    88,    99,    99,
      99,    99,    82,    82,     1,    46,    48,    23,    97,    81,
      61,    61,     1,    61,    64,    64,    64,    64,    64,     1,
      64,    29,     1,    65,    76,     1,    76,    47,    47,    47,
      47,    47,     1,    47,     1,    46,    19,    87,    87,   101,
     102,    11,    47,    48,    53,     1,    11,    16,    17,    95,
       1,    95,    99,    52,    80,    82,    91,    61,    82,    99,
      99,    99,    98,    86,     1,    90,    94,    92,    18,    18,
       1,    18,    97,    51,    82,    89,    93,    93,    82,    82,
      82,    82,     1,    97,     1,     1,     1,    99,     1,    91,
      61,    82
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    55,    55,    55,    55,    56,    56,    56,    56,
      56,    56,    57,    57,    57,    58,    58,    59,    59,    59,
      59,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    62,    62,    62,    62,    62,    63,    63,    63,    63,
      63,    63,    63,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    65,    65,    65,    65,    66,    66,
      67,    67,    67,    67,    68,    68,    68,    68,    69,    69,
      69,    69,    69,    70,    70,    70,    70,    70,    70,    70,
      70,    71,    71,    71,    72,    72,    72,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    74,
      74,    74,    74,    74,    75,    75,    75,    75,    76,    76,
      77,    77,    78,    78,    78,    78,    79,    79,    81,    80,
      80,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    83,    84,
      84,    86,    85,    85,    87,    87,    88,    88,    90,    89,
      89,    92,    91,    94,    93,    93,    95,    95,    96,    96,
      96,    98,    97,    97,    99,    99,   100,   100,   100,   100,
     100,   101,   101,   101,   102,   102,   102,   102,   102,   102,
     102,   102,   103,   103,   103,   103
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     3,     3,     6,     6,     6,     6,
       6,     6,     5,     5,     5,     3,     1,     3,     3,     3,
       0,     5,     3,     5,     5,     5,     5,     5,     3,     3,
       3,     2,     2,     1,     2,     2,     2,     2,     1,     1,
       3,     3,     0,     3,     3,     3,     5,     3,     5,     5,
       3,     3,     3,     1,     3,     6,     3,     3,     6,     6,
       6,     6,     6,     6,     1,     1,     1,     1,     1,     0,
       3,     1,     3,     3,     4,     4,     4,     4,     3,     0,
       3,     3,     3,     5,     3,     5,     5,     5,     5,     3,
       3,     3,     0,     3,     2,     2,     2,     6,     4,     6,
       6,     6,     6,     6,     6,     4,     4,     4,     4,     3,
       0,     3,     3,     3,     3,     1,     3,     3,     1,     1,
       2,     2,     3,     3,     3,     2,     3,     3,     0,     7,
       1,     3,     1,     1,     5,     5,     5,     5,     5,     4,
       4,     4,     4,     8,     8,     8,     8,     0,     2,     2,
       0,     0,     7,     2,     2,     0,     1,     0,     0,     7,
       1,     0,     7,     0,     7,     1,     1,     1,     1,     4,
       4,     0,     7,     1,     3,     1,     1,     2,     2,     3,
       4,     3,     4,     1,     1,     1,     4,     4,     3,     3,
       3,     2,     1,     1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 76 "yacc.y" /* yacc.c:1646  */
    {
                        ParseTreeHead=(yyval)=new Token("programstruct");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                        if(yylex())     yyerror("redundant content at the end",(yylsp[0]).last_line,(yylsp[0]).last_column+1);
                        YYACCEPT;
                }
#line 1709 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 83 "yacc.y" /* yacc.c:1646  */
    {//error: program_head识别出错
                        ParseTreeHead=(yyval)=new Token("programstruct");
                        yyerror("fatal error in program_head, maybe missing keyword \"PROGRAM\"",(yylsp[-2]).first_line,(yylsp[-2]).first_column,(yylsp[-2]).last_line,(yylsp[-2]).last_column);
                }
#line 1718 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 86 "yacc.y" /* yacc.c:1646  */
    {//error: program_body识别出错
                       ParseTreeHead=(yyval)=new Token("programstruct");
                        yyerror("fatal error in program body.");
                }
#line 1727 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 89 "yacc.y" /* yacc.c:1646  */
    {//error: 末尾缺少'.'
                        ParseTreeHead=(yyval)=new Token("programstruct");
                        yyerror("missing '.' here",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 1736 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 94 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("program_head");
                        (yyval)->children.push_back((yyvsp[-5]));
                        (yyval)->children.push_back((yyvsp[-4]));
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 1750 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 102 "yacc.y" /* yacc.c:1646  */
    {//error:POGRAM后缺少主程序名
                        (yyval)=new Token("program_head");
                        yyerror("missing PROGRAM name here",(yylsp[-5]).last_line,(yylsp[-5]).last_column+1);
                }
#line 1759 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 105 "yacc.y" /* yacc.c:1646  */
    {//error: 缺少左括号'('
                        (yyval)=new Token("program_head");
                        yyerror("missing '(' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column-1);
                }
#line 1768 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 108 "yacc.y" /* yacc.c:1646  */
    {//error: idlist识别失败
                        (yyval)=new Token("program_head");
                        yyerror("program identifier missing or illegal",(yylsp[-2]).first_line,(yylsp[-2]).first_column,(yylsp[-2]).last_line,(yylsp[-2]).last_column);
                }
#line 1777 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 111 "yacc.y" /* yacc.c:1646  */
    {//error:缺少右括号')'
                        (yyval)=new Token("program_head");
                        yyerror("missing '(' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column+1);
                }
#line 1786 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 114 "yacc.y" /* yacc.c:1646  */
    {//error: 缺少分号';'
                        (yyval)=new Token("program_head");
                        yyerror("missing ';' here.",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 1795 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 119 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("program_body");
                        (yyval)->children.push_back((yyvsp[-4]));
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 1808 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 126 "yacc.y" /* yacc.c:1646  */
    {//error:const_declarations识别失败
                        (yyval)=new Token("program_body");
                        yyerror("fatal error in const_declarations, maybe missing keyword \"CONST\"",(yylsp[-4]).first_line,(yylsp[-4]).first_column-1);
                }
#line 1817 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 129 "yacc.y" /* yacc.c:1646  */
    {error:compound_statement识别失败
                        (yyval)=new Token("program_body");
                        yyerror("fatal error in compound_statement, maybe missing keyword \"begin\"",(yylsp[0]).first_line,(yylsp[0]).first_column-1);
                }
#line 1826 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 134 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("idlist");
			            (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 1837 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 139 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("idlist");
			            (yyval)->children.push_back((yyvsp[0]));
                }
#line 1846 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 144 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_declarations");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));                
                }
#line 1857 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 149 "yacc.y" /* yacc.c:1646  */
    {//error: const_declarations识别失败
                        (yyval)=new Token("const_declarations");
                        yyerror("fatal error in const_declaration, identifier may be illegal",(yylsp[-1]).first_line,(yylsp[-1]).first_column,(yylsp[-1]),last_line,(yylsp[-1]).last_column);
                }
#line 1866 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 152 "yacc.y" /* yacc.c:1646  */
    {//error: 缺少分号';'
                        (yyval)=new Token("const_declarations");
                        yyerror("missing ';' here.",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 1875 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 155 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_declarations");
                }
#line 1883 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 159 "yacc.y" /* yacc.c:1646  */
    {    //产生式1
                        (yyval)=new Token("const_declaration");
                        (yyval)->children.push_back((yyvsp[-4]));
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 1896 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 166 "yacc.y" /* yacc.c:1646  */
    {    //产生式2
                        (yyval)=new Token("const_declaration");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 1907 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 171 "yacc.y" /* yacc.c:1646  */
    {     //error: const_declaration识别失败（产生式1）
                        (yyval)=new Token("const_declaration");
                        yyerror("fatal error in const_declaration, identifier missing or illegal.",(yylsp[-4]).first_line,(yylsp[-4]).first_column,(yylsp[-4]).last_line,(yylsp[-4]).last_column);
                }
#line 1916 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 174 "yacc.y" /* yacc.c:1646  */
    {//error: 缺少分号（产生式1）
                        (yyval)=new Token("const_declaration");
                        yyerror("missing ';' here.",(yylsp[-4]).last_line,(yylsp[-4]).last_column+1);
                }
#line 1925 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 177 "yacc.y" /* yacc.c:1646  */
    {      //error: 标识符识别失败（产生式1）
                        (yyval)=new Token("const_declaration");
                        yyerror("identifier is illegal or missing .",(yylsp[-2]).first_line,(yylsp[-2]).first_column,(yylsp[-2]).last_line,(yylsp[-2]).last_column);
                }
#line 1934 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 180 "yacc.y" /* yacc.c:1646  */
    {   //error: 缺少等号'+'（产生式1）
                        (yyval)=new Token("const_declaration");
                        yyerror("missing '=' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column+1);
                }
#line 1943 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 183 "yacc.y" /* yacc.c:1646  */
    {       //error: 无法识别const_variable（产生式1）
                        (yyval)=new Token("const_declaration");
                        yyerror("fatal error in const_variable",(yylsp[0]).first_line,(yylsp[0]).first_column,(yylsp[0]).last_line,(yylsp[0]).last_column);
                }
#line 1952 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 186 "yacc.y" /* yacc.c:1646  */
    {     //error: 标识符无法识别或缺失（产生式2）
                        (yyval)=new Token("const_declaration");
                        yyerror("identifier is illegal or missing.",(yylsp[-1]).last_line,(yylsp[-1]).last_column-1);
                }
#line 1961 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 189 "yacc.y" /* yacc.c:1646  */
    {  //error:缺少等号'='（产生式2）
                        (yyval)=new Token("const_declaration");
                        yyerror("missing '=' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column+1);
                }
#line 1970 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 192 "yacc.y" /* yacc.c:1646  */
    {     //error: const_variable识别失败（产生式2）
                        (yyval)=new Token("const_declaration");
                        yyerror("fatal error in const_variable",(yylsp[0]).first_line,(yylsp[0]).first_column.(yylsp[0]).last_line.(yylsp[0]).last_column);
                }
#line 1979 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 198 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 1989 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 202 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 1999 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 206 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2008 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 209 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2018 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 213 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2028 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 217 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2038 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 221 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2048 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 225 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2057 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 228 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2066 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 231 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("const_variable");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2077 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 240 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("type_declarations");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2088 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 245 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("type_declarations");
                }
#line 2096 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 247 "yacc.y" /* yacc.c:1646  */
    {       //error: 缺少关键字TYPE
                        (yyval)=new Token("type_declarations");
                        yyerror("missing keyword \"TYPE\"",(yylsp[-1]).first_line,(yylsp[-1]).last_column-1);
                }
#line 2105 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 250 "yacc.y" /* yacc.c:1646  */
    {      //error: type_declaration识别失败
                        (yyval)=new Token("type_declarations");
                        yyerror("fatal error in type_declaration, check whether identifiers is legal.",(yylsp[-1]).first_line,(yylsp[-1]).first_column,(yylsp[-1]).last_line,(yylsp[-1]).last_column);
                }
#line 2114 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 253 "yacc.y" /* yacc.c:1646  */
    {      //error: 缺少分号';'
                        (yyval)=new Token("type_declarations");
                        yyerror("missing ';' here.",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 2123 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 258 "yacc.y" /* yacc.c:1646  */
    {     //产生式1
                        (yyval)=new Token("type_declaration");
                        (yyval)->children.push_back((yyvsp[-4]));
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2136 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 265 "yacc.y" /* yacc.c:1646  */
    {      //产生式2
                        (yyval)=new Token("type_declaration");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2147 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 270 "yacc.y" /* yacc.c:1646  */
    {  //error: 缺少分号';'（产生式1）
                        (yyval)=new Token("type_declaration");
                        yyerror("missing ';' here.",(yylsp[-4]).last_line,(yylsp[-4]).last_column+1);
                }
#line 2156 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 273 "yacc.y" /* yacc.c:1646  */
    {   //error: 缺少等号'='（产生式1）
                        (yyval)=new Token("type_declaration");
                        yyerror("missing '=' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column+1);
                }
#line 2165 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 276 "yacc.y" /* yacc.c:1646  */
    {    //error:标识符无法识别或缺失（产生式2）
                        (yyval)=new Token("type_declaration");
                        yyerror("identifier is illegal or missing.",(yylsp[-2]).first_line,(yylsp[-2]).first_column,(yylsp[-2]).last_line,(yylsp[-2]).last_column);
                }
#line 2174 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 279 "yacc.y" /* yacc.c:1646  */
    {    //error:缺少等号'='（产生式2）
                        (yyval)=new Token("type_declaration");
                        yyerror("missing '=' here.",(yylsp[-2]).last_line,(yylsp[-2]),last_column+1);
                }
#line 2183 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 282 "yacc.y" /* yacc.c:1646  */
    {     //error: type识别失败
                        (yyval)=new Token("type_declaration");
                        yyerror("fatal error in type",(yylsp[0]).first_line,(yylsp[0]).first_column,(yylsp[0]).last_line,(yylsp[0]),last_column);
                }
#line 2192 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 287 "yacc.y" /* yacc.c:1646  */
    {   //产生式1
                        (yyval)=new Token("type");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2201 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 290 "yacc.y" /* yacc.c:1646  */
    {    //产生式2
                        (yyval)=new Token("type");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2212 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 295 "yacc.y" /* yacc.c:1646  */
    {  //产生式3
                        (yyval)=new Token("type");
                        (yyval)->children.push_back((yyvsp[-5]));
                        (yyval)->children.push_back((yyvsp[-4]));
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2226 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 303 "yacc.y" /* yacc.c:1646  */
    {    //error: 缺少关键字"RECORD" （产生式2）
                        (yyval)=new Token("type");
                        yyerror("missing keyword \"RECORD\"",(yylsp[-1]).last_line,(yylsp[-1]).last_column-1);
                }
#line 2235 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 306 "yacc.y" /* yacc.c:1646  */
    {     //error: 缺少关键字"END" （产生式2）
                        (yyval)=new Token("type");
                        yyerror("missing keyword \"END\"",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 2244 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 309 "yacc.y" /* yacc.c:1646  */
    {   //error: 缺少关键字“ARRAY” （产生式3）
                        (yyval)=new Token("type");
                        yyerror("missing keyword \"ARRAY\"",(yylsp[-4]).last_line,(yylsp[-4]).last_column-1);
                }
#line 2253 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 312 "yacc.y" /* yacc.c:1646  */
    {   //error:缺少左括号'[' （产生式3）
                        (yyval)=new Token("type");
                        yyerror("missing '[' here.",(yylsp[-4]).first_line,(yylsp[-4]).first_column-1);
                }
#line 2262 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 315 "yacc.y" /* yacc.c:1646  */
    {      //error:periods识别失败 （产生式3）
                        (yyval)=new Token("type");
                        yyerror("fatal error in periods",(yylsp[-3]).first_line,(yylsp[-3]).first_column,(yylsp[-3]).last_line,(yylsp[-3]).last_column);
                }
#line 2271 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 318 "yacc.y" /* yacc.c:1646  */
    {      //error:缺少右括号'[' （产生式3）
                        (yyval)=new Token("type");
                        yyerror("missing ']' here.",(yylsp[-3]).last_line,(yylsp[-3]),last_column+1);
                }
#line 2280 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 321 "yacc.y" /* yacc.c:1646  */
    {    //error: 缺少关键字“OF” （产生式3）
                        (yyval)=new Token("type");
                        yyerror(missing keyword \"OF\".",(yylsp[-2]).last_line,(yylsp[-2]).last_column+1);
                }
#line 2289 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 324 "yacc.y" /* yacc.c:1646  */
    {       //error: type识别失败 （产生式3）
                        (yyval)=new Token("type");
                        yyerror("fatal error in type",(yylsp[0]).first_line,(yylsp[0]).first_column,(yylsp[0]).last_line,(yylsp[0]).last_column);
                }
#line 2298 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 330 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("standard_type");
                        (yyval)->children.push_back((yyvsp[0]));

                }
#line 2308 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 334 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("standard_type");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2317 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 337 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("standard_type");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2326 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 340 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("standard_type");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2335 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 345 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("record_body");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2344 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 348 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("record_body");
                }
#line 2352 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 352 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("periods");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2363 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 357 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("periods");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2372 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 360 "yacc.y" /* yacc.c:1646  */
    {     //error: 缺少','
                        (yyval)=new Token("periods");
                        yyerror("missing ',' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column+1);
                }
#line 2381 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 363 "yacc.y" /* yacc.c:1646  */
    {          //error: period识别失败
                        (yyval)=new Token("periods");
                        yyerror("fatal error in period",(yylsp[0]).first_line,(yylsp[0]).first_column,(yylsp[0]),last_line,(yylsp[0]),last_column);
                }
#line 2390 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 368 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("period");
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2402 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 374 "yacc.y" /* yacc.c:1646  */
    {   //缺少'.'
                        (yyval)=new Token("period");
                        yyerror("missing '.' here.",(yylsp[-3]).last_line,(yylsp[-3]).last_column++1);
                }
#line 2411 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 377 "yacc.y" /* yacc.c:1646  */
    {     //error: 缺少'.'
                        (yyval)=new Token("period");
                        yyerror("missing '.' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column++1);
                }
#line 2420 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 380 "yacc.y" /* yacc.c:1646  */
    {      //error: const_variable识别失败
                        (yyval)=new Token("period");
                        yyerror("fatal error in const_variable",(yylsp[0]).first_line,(yylsp[0]).first_column,(yylsp[0]).last_line,(yylsp[0]).last_column);
                }
#line 2429 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 385 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("var_ declarations");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2440 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 390 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("var_ declarations");
                }
#line 2448 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 392 "yacc.y" /* yacc.c:1646  */
    {     //error: 缺少关键字"VAR"
                        (yyval)=new Token("var_ declarations");
                        yyerror("missing keyword \"VAR\"",(yylsp[-1]).last_line,(yylsp[-1]).last_column-1);
                }
#line 2457 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 395 "yacc.y" /* yacc.c:1646  */
    {       //error: var_declaration识别失败
                        (yyval)=new Token("var_ declarations");
                        yyerror("fatal error in var_declaration",(yylsp[-1]).first_line,(yylsp[-1]).first_column,(yylsp[-1]).last_line,(yylsp[-1]).last_column);
                }
#line 2466 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 398 "yacc.y" /* yacc.c:1646  */
    {   //error: 缺少分号';'
                        (yyval)=new Token("var_ declarations");
                        yyerror("missing ';' here.",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 2475 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 403 "yacc.y" /* yacc.c:1646  */
    {  //产生式1
                        (yyval)=new Token("var_declaration");
                        (yyval)->children.push_back((yyvsp[-4]));
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2488 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 410 "yacc.y" /* yacc.c:1646  */
    {    //产生式2
                        (yyval)=new Token("var_declaration");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2499 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 415 "yacc.y" /* yacc.c:1646  */
    {    //error: 缺少分号';' （产生式1）
                        (yyval)=new Token("var_declaration");
                        yyerror("missing ':' here. ",(yylsp[-4]).last_line,(yylsp[-4]).last_column+1);
                }
#line 2508 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 418 "yacc.y" /* yacc.c:1646  */
    {      //error: 无法识别idlist （产生式1）
                        (yyval)=new Token("var_declaration");
                        yyerror("fatal error in idlist, identifier may be ilegal or missing",(yylsp[-2]).first_line,(yylsp[-2]).first_column,(yylsp[-2]).last_line,(yylsp[-2]).last_column);
                }
#line 2517 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 421 "yacc.y" /* yacc.c:1646  */
    {      //error:缺少冒号':' （产生式1）
                        (yyval)=new Token("var_declaration");
                        yyerror("missing ':' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column+1);
                }
#line 2526 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 424 "yacc.y" /* yacc.c:1646  */
    {      //error: 无法识别type （产生式1）
                        (yyval)=new Token("var_declaration");
                        yyerror("fatal error in type.",(yylsp[0]).first_line,(yylsp[0]).first_column,(yylsp[0]).last_line,(yylsp[0]).last_column);
                }
#line 2535 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 427 "yacc.y" /* yacc.c:1646  */
    {    //error: 缺少冒号 （产生式2）
                        (yyval)=new Token("var_declaration");
                        yyerror("missing ':' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column+1);
                }
#line 2544 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 430 "yacc.y" /* yacc.c:1646  */
    {     //error: type识别失败
                        (yyval)=new Token("var_declaration");
                        yyerror("fatal error in type",(yylsp[0]).first_line,(yylsp[0]).first_column,(yylsp[0]).last_line,(yylsp[0]).last_column);
                }
#line 2553 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 435 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("subprogram_declarations");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2564 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 440 "yacc.y" /* yacc.c:1646  */
    { 
                        (yyval)=new Token("subprogram_declarations");
                }
#line 2572 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 442 "yacc.y" /* yacc.c:1646  */
    { //error: 缺少分号';'
                        (yyval)=new Token("subprogram_declarations");
                        yyerror("miising ';' here.",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 2581 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 447 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("subprogram_declaration");
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2591 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 451 "yacc.y" /* yacc.c:1646  */
    {     //error: subprogram_head识别失败
                        (yyval)=new Token("subprogram_declaration");
                        yyerror("fatal error in subprogram_head.",(yylsp[-1]).first_line,(yylsp[-1]).first_column,(yylsp[-1]).last_line,(yylsp[-1]).last_column
                }
#line 2600 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 454 "yacc.y" /* yacc.c:1646  */
    {     //error: program_body识别失败
                        (yyval)=new Token("subprogram_declaration");
                        yyerror("fatal error in program_body.",(yylsp[0]).first_line,(yylsp[0]).first_column,(yylsp[0]).last_line,(yylsp[0]).last_column);
                }
#line 2609 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 459 "yacc.y" /* yacc.c:1646  */
    {   //产生式1
                        (yyval)=new Token("subprogram_head");
                        (yyval)->children.push_back((yyvsp[-5]));
                        (yyval)->children.push_back((yyvsp[-4]));
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2623 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 467 "yacc.y" /* yacc.c:1646  */
    {   //产生式2
                        (yyval)=new Token("subprogram_head");
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2635 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 473 "yacc.y" /* yacc.c:1646  */
    {   //error: 缺少关键字“FUNCTOPN"（产生式1）
                        (yyval)=new Token("subprogram_head");
                        yyerror("missing keyword /"FUNCTION/"",(yylsp[-4]).last_line,(yylsp[-4]).last_column-1);
                }
#line 2644 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 476 "yacc.y" /* yacc.c:1646  */
    {   //error: "IDENTIFIER"非法或缺失 （产生式1）
                        (yyval)=new Token("subprogram_head");
                        yyerror("identifier is ilegal or miising.",(yylsp[-4]).first_line,(yylsp[-4]).first_column,(yylsp[-4]).last_line,(yylsp[-4]).last_column);
                }
#line 2653 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 479 "yacc.y" /* yacc.c:1646  */
    {   //error: formal_parameter识别失败 （产生式1）
                        (yyval)=new Token("subprogram_head");
                        yyerror("fatal error in formal_parameter.",(yylsp[-3]).first_line,(yylsp[-3]).first_column,(yylsp[-3]).last_line,(yylsp[-3]).last_column);
                }
#line 2662 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 482 "yacc.y" /* yacc.c:1646  */
    {   //error: 缺少冒号 ':'  （产生式1）
                        (yyval)=new Token("subprogram_head");
                        yyerror("missing ':' here",(yylsp[-3]).last_line,(yylsp[-3]).last_column+1);
                }
#line 2671 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 485 "yacc.y" /* yacc.c:1646  */
    {   //error: standard_type识别失败 （产生式1）
                        (yyval)=new Token("subprogram_head");
                        yyerror("fatal error in standard_type.",(yylsp[-1]).first_line,(yylsp[-1]).first_column,(yylsp[-1]).last_line,(yylsp[-1]).last_column);
                }
#line 2680 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 488 "yacc.y" /* yacc.c:1646  */
    {   //error: 缺少分号';' （产生式1）
                        (yyval)=new Token("subprogram_head");
                        yyerror("missing ';' here.",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 2689 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 491 "yacc.y" /* yacc.c:1646  */
    {     //error: 缺少关键字"PROCEDURE" （产生式2）
                        (yyval)=new Token("subprogram_head");
                        yyerror("missing keyword /"PROCEDURE/" here.",(yylsp[-2]).first_line,(yylsp[-2]).first_column-1);
                }
#line 2698 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 494 "yacc.y" /* yacc.c:1646  */
    {     //error: "IDENTIFIER"非法或缺失 （产生式2）
                        (yyval)=new Token("subprogram_head");
                        yyerror("identifier is illegal or missing.",(yylsp[-2]).first_line,(yylsp[-2]).first_column,(yylsp[-2]).last_line,(yylsp[-2]).last_column);
                }
#line 2707 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 497 "yacc.y" /* yacc.c:1646  */
    {     //error:  formal_parameter识别失败  （产生式2）
                        (yyval)=new Token("subprogram_head");
                        yyerror("fatal error in formal_parameter.",(yylsp[-1]).first_line,(yylsp[-1]),first_column,(yylsp[-1]).last_line,(yylsp[-1]).last_column);
                }
#line 2716 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 500 "yacc.y" /* yacc.c:1646  */
    {     //error: 缺少分号';'    （产生式2）
                        (yyval)=new Token("subprogram_head");
                        yyerror("missing ';' here.",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 2725 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 505 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("formal_parameter");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2736 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 510 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("formal_parameter");
                }
#line 2744 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 512 "yacc.y" /* yacc.c:1646  */
    {       //error: 缺少左括号'('
                        (yyval)=new Token("formal_parameter");
                        yyerror("missing '(' here.",(yylsp[-1]).first_line,(yylsp[-1]).first_column-1);
                }
#line 2753 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 515 "yacc.y" /* yacc.c:1646  */
    {      //error: 无法识别 parameter_lists
                        (yyval)=new Token("formal_parameter");
                        yyerror("fatal error in parameter_lists.",(yylsp[-1]).first_line,(yylsp[-1]).first_column,(yylsp[-1]).last_line,(yylsp[-1]).last_column);
                }
#line 2762 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 518 "yacc.y" /* yacc.c:1646  */
    {       //error:缺少右括号')'
                        (yyval)=new Token("formal_parameter");
                        yyerror("missing ')' here.",(yylsp[-1]).last_line,(yylsp[-1]).last_column+1);
                }
#line 2771 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 523 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("parameter_lists");
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2782 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 528 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("parameter_lists");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2791 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 531 "yacc.y" /* yacc.c:1646  */
    {   //error: 缺少分号';'
                        (yyval)=new Token("parameter_lists");
                        yyerror("missing ';' here.",(yylsp[-2]).last_line,(yylsp[-2]).last_column+1);
                }
#line 2800 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 534 "yacc.y" /* yacc.c:1646  */
    {   //error: parameter_list识别失败
                        (yyval)=new Token("parameter_lists");
                        yyerror("fatal error in parameter_list.",(yylsp[0]).first_line,(yylsp[0]).first_column,(yylsp[0]).last_line,(yylsp[0]).last_column);
                }
#line 2809 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 539 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("parameter_list");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2818 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 542 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval)=new Token("parameter_list");
                        (yyval)->children.push_back((yyvsp[0]));
                }
#line 2827 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 549 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("var_parameter");
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 2837 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 553 "yacc.y" /* yacc.c:1646  */
    { //不完整的引用参数列表
                    (yyval) = new Token("var_parameter");
                    yyerror("incomplete refereced parameter list", &(yyloc));
                }
#line 2846 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 559 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("value_parameter");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 2857 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 564 "yacc.y" /* yacc.c:1646  */
    { //缺少冒号
                    (yyval) = new Token("value_parameter");
                    yyerror("missing a colon ':' here", (yylsp[-2]).first_line, (yylsp[-2]).last_column+1);
                }
#line 2866 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 567 "yacc.y" /* yacc.c:1646  */
    {  //缺少基本类型关键字
                    (yyval) = new Token("value_parameter");
                    yyerror("missing a base type keyword here", (yylsp[-1]).last_line, (yylsp[-1]).last_column+1);
                }
#line 2875 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 570 "yacc.y" /* yacc.c:1646  */
    { //缺少基本类型关键字
                    (yyval) = new Token("value_parameter");
                    yyerror("missing a base type keyword here", (yylsp[-1]).last_line, (yylsp[-1]).last_column+1);
				}
#line 2884 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 575 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("compound_statement");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 2895 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 580 "yacc.y" /* yacc.c:1646  */
    { //ERROR 缺少END关键字 checked
                    (yyval) = new Token("compound_statement");
                    yyerror("missing keyword \"end\"", (yylsp[-1]).last_line, (yylsp[-1]).last_column+1);
				}
#line 2904 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 585 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement_list");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 2915 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 590 "yacc.y" /* yacc.c:1646  */
    {  //缺少分号
                    (yyval) = new Token("statement_list");
                    yyerror("missing a semicolon ';' here", (yylsp[-6]).last_line, (yylsp[-6]).last_column+1);
                }
#line 2924 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 593 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement_list");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 2933 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 598 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 2944 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 603 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 2953 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 606 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 2962 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 609 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
                    (yyval)->children.push_back((yyvsp[-4]));
                    (yyval)->children.push_back((yyvsp[-3]));
                    (yyval)->children.push_back((yyvsp[-2]));                        
                    (yyval)->children.push_back((yyvsp[-1]));                        
                    (yyval)->children.push_back((yyvsp[0]));                        
                }
#line 2975 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 616 "yacc.y" /* yacc.c:1646  */
    {  //缺少then关键字
                    (yyval) = new Token("statement");
                    yyerror("missing keyword \"then\"", (yylsp[-3]).last_line, (yylsp[-3]).last_column+1);
                }
#line 2984 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 619 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
                    (yyval)->children.push_back((yyvsp[-4]));
                    (yyval)->children.push_back((yyvsp[-3]));
                    (yyval)->children.push_back((yyvsp[-2]));                        
                    (yyval)->children.push_back((yyvsp[-1]));                        
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 2997 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 626 "yacc.y" /* yacc.c:1646  */
    {   //缺少of关键字
                    (yyval) = new Token("statement");
                    yyerror("missing keyword \"of\"", (yylsp[-3]).last_line, (yylsp[-3]).last_column+1);
                }
#line 3006 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 629 "yacc.y" /* yacc.c:1646  */
    {    //缺少end关键字
                    (yyval) = new Token("statement");
                    yyerror("missing keyword \"end\"", (yylsp[-1]).last_line, (yylsp[-1]).last_column+1);
                }
#line 3015 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 632 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
                    (yyval)->children.push_back((yyvsp[-3]));
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));                        
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3027 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 638 "yacc.y" /* yacc.c:1646  */
    { //缺少关键字do
                    (yyval) = new Token("statement");
				    yyerror("missing keywrod \"do\"", (yylsp[-2]).last_line, (yylsp[-2]).last_column+1);
			    }
#line 3036 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 641 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
                    (yyval)->children.push_back((yyvsp[-3]));
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));                        
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3048 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 647 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
				    yyerror("missing keywrod \"until\"", (yylsp[-2]).last_line, (yylsp[-2]).last_column+1);
                }
#line 3057 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 650 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
                    (yyval)->children.push_back((yyvsp[-7]));
                    (yyval)->children.push_back((yyvsp[-6]));
                    (yyval)->children.push_back((yyvsp[-5]));                        
                    (yyval)->children.push_back((yyvsp[-4]));                        
                    (yyval)->children.push_back((yyvsp[-3]));                        
                    (yyval)->children.push_back((yyvsp[-2]));                        
                    (yyval)->children.push_back((yyvsp[-1]));                        
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3073 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 660 "yacc.y" /* yacc.c:1646  */
    { //缺少赋值号
                    (yyval) = new Token("statement");
                    yyerror("missing assignop \":=\"", (yylsp[-6]).last_line, (yylsp[-6]).last_column+1);
                }
#line 3082 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 663 "yacc.y" /* yacc.c:1646  */
    {  //缺少to 或 downto
                    (yyval) = new Token("statement");
                    yyerror("missing keyword \"to\" or \"downto\"", (yylsp[-6]).last_line, (yylsp[-6]).last_column+1);
                }
#line 3091 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 666 "yacc.y" /* yacc.c:1646  */
    { //缺少关键字do
                    (yyval) = new Token("statement");
				    yyerror("missing keywrod \"do\"", (yylsp[-2]).last_line, (yylsp[-4]).last_column+1);
			    }
#line 3100 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 669 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("statement");
                }
#line 3108 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 673 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("variable");
                    (yyval)->children.push_back((yyvsp[-1]));                        
                    (yyval)->children.push_back((yyvsp[0]));                        
                }
#line 3118 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 679 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("variable");
                    (yyval)->children.push_back((yyvsp[-1]));                        
                    (yyval)->children.push_back((yyvsp[0])); 
                }
#line 3128 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 683 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("variable");
                }
#line 3136 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 687 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("id_varpart");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3147 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 692 "yacc.y" /* yacc.c:1646  */
    {   //缺少右大括号
                    (yyval) = new Token("id_varpart");
                    yyerror("missing a right bracket ']' here", (yylsp[-5]).last_line, (yylsp[-5]).last_column+1)
                }
#line 3156 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 695 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("id_varpart");
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3166 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 701 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("else_part");
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3176 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 705 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("else_part");
                }
#line 3184 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 709 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("case_body");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3193 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 712 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("case_body");
                }
#line 3201 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 716 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("branch_list");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3212 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 721 "yacc.y" /* yacc.c:1646  */
    {    //缺少分号
                    (yyval) = new Token("branch_list");
                    yyerror("missing a semicolon ';' here", (yylsp[-6]).first_line, (yylsp[-6]).last_column+1);
                }
#line 3221 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 724 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("branch_list");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3230 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 729 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("branch");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3241 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 734 "yacc.y" /* yacc.c:1646  */
    {  //缺少冒号
                    (yyval) = new Token("branch");
                    yyerror("missing a colon ':' here", (yylsp[-6]).first_line, (yylsp[-6]).last_column+1);
                }
#line 3250 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 739 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("const_list");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3261 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 744 "yacc.y" /* yacc.c:1646  */
    { //缺少逗号
                    (yyval) = new Token("const_list");
                    yyerror("missing a comma ',' here", (yylsp[-6]).first_line, (yylsp[-6]).last_column+1);
                }
#line 3270 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 747 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("const_list");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3279 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 752 "yacc.y" /* yacc.c:1646  */
    {
                (yyval) = new Token("updown");
                (yyval)->children.push_back((yyvsp[0]));
            }
#line 3288 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 755 "yacc.y" /* yacc.c:1646  */
    {
                (yyval) = new Token("updown");
                (yyval)->children.push_back((yyvsp[0]));
            }
#line 3297 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 760 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval) = new Token("procedure_call")
                        (yyval)->children.push_back((yyvsp[0]));
                    }
#line 3306 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 763 "yacc.y" /* yacc.c:1646  */
    {
                        (yyval) = new Token("procedure_call")
                        (yyval)->children.push_back((yyvsp[-3]));
                        (yyval)->children.push_back((yyvsp[-2]));
                        (yyval)->children.push_back((yyvsp[-1]));
                        (yyval)->children.push_back((yyvsp[0]));
                    }
#line 3318 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 769 "yacc.y" /* yacc.c:1646  */
    {  //缺少右括号
                        (yyval) = new Token("procedure_call")
                        yyerror("missing a right bracket ')' here", (yylsp[-1]).last_line, (yylsp[-1]).last_column+1)
                    }
#line 3327 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 774 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("expression_list");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3338 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 779 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("expression_list");
                    yyerror("missing a comma ',' here", (yylsp[-6]).last_line, (yylsp[-6]).last_column+1)
                }
#line 3347 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 782 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("expression_list");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3356 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 787 "yacc.y" /* yacc.c:1646  */
    {
                (yyval) = new Token("expression");
                (yyval)->children.push_back((yyvsp[-2]));
                (yyval)->children.push_back((yyvsp[-1]));
                (yyval)->children.push_back((yyvsp[0]));
            }
#line 3367 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 792 "yacc.y" /* yacc.c:1646  */
    {
                (yyval) = new Token("expression");
                (yyval)->children.push_back((yyvsp[0]));
            }
#line 3376 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 797 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("simple_expression");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3385 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 800 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("simple_expression");
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3395 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 804 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("simple_expression");
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3405 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 808 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("simple_expression");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3416 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 813 "yacc.y" /* yacc.c:1646  */
    {  //ADDOP后缺少操作数
                    (yyval) = new Token("simple_expression");
                    yyerror("missing operand",(yylsp[-2]).last_line, (yylsp[-2]).last_column+1);
				}
#line 3425 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 818 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("term");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3436 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 823 "yacc.y" /* yacc.c:1646  */
    {  //MULOP后缺少操作数 a*-b 
                    (yyval) = new Token("term");
                    yyerror("missing operand",(yylsp[-2]).last_line, (yylsp[-2]).last_column+1);
                }
#line 3445 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 826 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("term");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3454 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 831 "yacc.y" /* yacc.c:1646  */
    {
                (yyval) = new Token("factor");
                (yyval)->children.push_back((yyvsp[0]));
            }
#line 3463 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 834 "yacc.y" /* yacc.c:1646  */
    {
                (yyval) = new Token("factor");
                (yyval)->children.push_back((yyvsp[0]));
            }
#line 3472 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 837 "yacc.y" /* yacc.c:1646  */
    {
                (yyval) = new Token("factor");
                (yyval)->children.push_back((yyvsp[-3]));
                (yyval)->children.push_back((yyvsp[-2]));
                (yyval)->children.push_back((yyvsp[-1]));
                (yyval)->children.push_back((yyvsp[0]));
            }
#line 3484 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 843 "yacc.y" /* yacc.c:1646  */
    {  //缺少右括号
                (yyval) = new Token("factor");
                yyerror("missing a right bracket ')' here", (yylsp[-1]).last_line, (yylsp[-1]).last_column+1)
            }
#line 3493 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 846 "yacc.y" /* yacc.c:1646  */
    { //函数调用的表达式列表缺失
                (yyval) = new Token("factor");
                yyerror("missing actual parameter list of function call", (yylsp[-1]).last_line, (yylsp[-1]).last_column+1);
            }
#line 3502 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 849 "yacc.y" /* yacc.c:1646  */
    {
                (yyval) = new Token("factor");
                (yyval)->children.push_back((yyvsp[-2]));
                (yyval)->children.push_back((yyvsp[-1]));
                (yyval)->children.push_back((yyvsp[0]));
            }
#line 3513 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 854 "yacc.y" /* yacc.c:1646  */
    {  //缺少右括号
                (yyval) = new Token("factor");
                yyerror("missing a right bracket ')' here", (yylsp[-1]).last_line, (yylsp[-1]).last_column+1)
            }
#line 3522 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 857 "yacc.y" /* yacc.c:1646  */
    {
                (yyval) = new Token("factor");
                (yyval)->children.push_back((yyvsp[-1]));
                (yyval)->children.push_back((yyvsp[0]));
            }
#line 3532 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 863 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("unsign_const");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3541 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 866 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("unsign_const");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3550 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 869 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("unsign_const");
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3559 "yacc.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 872 "yacc.y" /* yacc.c:1646  */
    {
                    (yyval) = new Token("unsign_const");
                    (yyval)->children.push_back((yyvsp[-2]));
                    (yyval)->children.push_back((yyvsp[-1]));
                    (yyval)->children.push_back((yyvsp[0]));
                }
#line 3570 "yacc.tab.c" /* yacc.c:1646  */
    break;


#line 3574 "yacc.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 879 "yacc.y" /* yacc.c:1906  */



void yyerror(const char *s){
	haveSemanticError = true;//错误标志，含有语法错误
	string errorInformation;//定义错误信息
	errorInformation += string(s);//添加错误信息
	errorInformation += ", location: " + itos(yylineno-1) + "." + itos(yycolumn-yyleng);//添加错误位置
	syntaxErrorInformation.push_back(errorInformation);//存放错误信息
}

void yyerror(const char *s, YYLTYPE *loc){//处理单个字符的错误
	haveSemanticError = true;
	string errorInformation;
	errorInformation = "syntax error, " + string(s) + ", location: " + itos(loc->first_line) + "." + itos(loc->first_column) + "-" + itos(loc->last_line) + "." + itos(loc->last_column);
	syntaxErrorInformation.push_back(errorInformation);
}

void yyerror(const char *s, int line, int col){//处理一行以内的错误
	haveSemanticError = true;
	string errorInformation;
	errorInformation = "syntax error, " + string(s) + ", location: " + itos(line) + "." + itos(col);
	syntaxErrorInformation.push_back(errorInformation);
}

void yyerror(const char *s, int startLine, int startCol, int endLine, int endCol){//处理涉及多行的错误
	haveSemanticError = true;
	string errorInformation;
	errorInformation = "syntax error, " + string(s) + ", location: " + itos(startLine) + "." + itos(startCol) + "-" + itos(endLine) + "." + itos(endCol);
	syntaxErrorInformation.push_back(errorInformation);
}

int main()
{
    printf("-----begin parsing\n");  
    yyparse();//使yacc开始读取输入和解析，它会调用lex的yylex()读取记号  
    puts("-----end parsing"); 
}
