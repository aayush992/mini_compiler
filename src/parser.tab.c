/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir.h"

// Phase 3 Will add code generation functionality

#define TABLE_SIZE 100
#define MAX_NAME 100

// Symbol types
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_VOID
} SymbolType;

typedef struct Symbol {
    char* name;
    int type;
    int size;           // For arrays
    int* int_array;     // For int arrays
    float* float_array; // For float arrays
    char** str_array;   // For string arrays
    int int_val;
    float float_val;
    char* str_val;
    int is_array;
    struct Symbol* next;  // For hash table collision handling
} Symbol;

typedef struct {
    int int_val;
    float float_val;
    char* str_val;
    int is_float;
    int is_bool;
    int is_array;
    int array_index;
    char* temp_var;
} ExprValue;

// Hash table implementation
Symbol* symbol_table[TABLE_SIZE];
int temp_var_count = 0;
int label_count = 0;

void yyerror(const char *s);
int yylex(void);

// Hash function
unsigned int hash(const char* name) {
    unsigned int hash_val = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash_val = hash_val * 31 + name[i];
    }
    return hash_val % TABLE_SIZE;
}

// Insert symbol into hash table
Symbol* insert_symbol(char* name, int type, int size) {
    unsigned int index = hash(name);
    
    // Check if symbol already exists
    Symbol* current = symbol_table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            yyerror("Symbol already declared");
            return NULL;
        }
        current = current->next;
    }
    
    // Create new symbol
    Symbol* sym = (Symbol*)malloc(sizeof(Symbol));
    if (!sym) {
        yyerror("Memory allocation failed");
        return NULL;
    }
    
    sym->name = strdup(name);
    sym->type = type;
    sym->is_array = (size > 0);
    sym->size = size;
    sym->next = NULL;
    
    if(sym->is_array) {
        if(type == TYPE_INT) {
            sym->int_array = (int*)calloc(size, sizeof(int));
        } else if(type == TYPE_FLOAT) {
            sym->float_array = (float*)calloc(size, sizeof(float));
        } else if(type == TYPE_STRING) {
            sym->str_array = (char**)calloc(size, sizeof(char*));
        }
    }
    
    // Insert at the beginning of the list
    sym->next = symbol_table[index];
    symbol_table[index] = sym;
    
    return sym;
}

// Look up symbol in hash table
Symbol* lookup_symbol(const char* name) {
    unsigned int index = hash(name);
    Symbol* current = symbol_table[index];
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

char* new_temp() {
    char* temp = (char*)malloc(10);
    sprintf(temp, "t%d", temp_var_count++);
    return temp;
}

char* new_label() {
    char* label = (char*)malloc(10);
    sprintf(label, "L%d", label_count++);
    return label;
}

void init_symbol_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        symbol_table[i] = NULL;
    }
}

void cleanup_symbol_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol* current = symbol_table[i];
        while (current != NULL) {
            Symbol* to_delete = current;
            current = current->next;
            
            free(to_delete->name);
            if(to_delete->is_array) {
                if(to_delete->type == TYPE_INT)
                    free(to_delete->int_array);
                else if(to_delete->type == TYPE_FLOAT)
                    free(to_delete->float_array);
                else if(to_delete->type == TYPE_STRING)
                    free(to_delete->str_array);
            }
            free(to_delete);
        }
        symbol_table[i] = NULL;
    }
}

// Global IR program
IRProgram* program;
IRFunction* current_function;
BasicBlock* current_block;

// Function declarations
void add_symbol(const char* name, SymbolType type);

#line 240 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_NUMBER = 4,                     /* NUMBER  */
  YYSYMBOL_FLOAT_LITERAL = 5,              /* FLOAT_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 6,             /* STRING_LITERAL  */
  YYSYMBOL_BOOL_LITERAL = 7,               /* BOOL_LITERAL  */
  YYSYMBOL_INT = 8,                        /* INT  */
  YYSYMBOL_FLOAT = 9,                      /* FLOAT  */
  YYSYMBOL_IF = 10,                        /* IF  */
  YYSYMBOL_ELSE = 11,                      /* ELSE  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_WHILE = 13,                     /* WHILE  */
  YYSYMBOL_FOR = 14,                       /* FOR  */
  YYSYMBOL_PRINT = 15,                     /* PRINT  */
  YYSYMBOL_INPUT = 16,                     /* INPUT  */
  YYSYMBOL_17_ = 17,                       /* '+'  */
  YYSYMBOL_18_ = 18,                       /* '-'  */
  YYSYMBOL_19_ = 19,                       /* '*'  */
  YYSYMBOL_20_ = 20,                       /* '/'  */
  YYSYMBOL_21_ = 21,                       /* '%'  */
  YYSYMBOL_22_ = 22,                       /* '='  */
  YYSYMBOL_23_ = 23,                       /* '<'  */
  YYSYMBOL_24_ = 24,                       /* '>'  */
  YYSYMBOL_25_ = 25,                       /* '('  */
  YYSYMBOL_26_ = 26,                       /* ')'  */
  YYSYMBOL_27_ = 27,                       /* '{'  */
  YYSYMBOL_28_ = 28,                       /* '}'  */
  YYSYMBOL_29_ = 29,                       /* ';'  */
  YYSYMBOL_30_ = 30,                       /* ','  */
  YYSYMBOL_31_ = 31,                       /* '['  */
  YYSYMBOL_32_ = 32,                       /* ']'  */
  YYSYMBOL_AND = 33,                       /* AND  */
  YYSYMBOL_OR = 34,                        /* OR  */
  YYSYMBOL_NOT = 35,                       /* NOT  */
  YYSYMBOL_EQ = 36,                        /* EQ  */
  YYSYMBOL_NEQ = 37,                       /* NEQ  */
  YYSYMBOL_LEQ = 38,                       /* LEQ  */
  YYSYMBOL_GEQ = 39,                       /* GEQ  */
  YYSYMBOL_TRUE = 40,                      /* TRUE  */
  YYSYMBOL_FALSE = 41,                     /* FALSE  */
  YYSYMBOL_STRING = 42,                    /* STRING  */
  YYSYMBOL_BOOL = 43,                      /* BOOL  */
  YYSYMBOL_VOID = 44,                      /* VOID  */
  YYSYMBOL_UMINUS = 45,                    /* UMINUS  */
  YYSYMBOL_IFX = 46,                       /* IFX  */
  YYSYMBOL_YYACCEPT = 47,                  /* $accept  */
  YYSYMBOL_program = 48,                   /* program  */
  YYSYMBOL_49_1 = 49,                      /* $@1  */
  YYSYMBOL_statement_list = 50,            /* statement_list  */
  YYSYMBOL_statement = 51,                 /* statement  */
  YYSYMBOL_declaration = 52,               /* declaration  */
  YYSYMBOL_assignment = 53,                /* assignment  */
  YYSYMBOL_array_assignment = 54,          /* array_assignment  */
  YYSYMBOL_array_expr = 55,                /* array_expr  */
  YYSYMBOL_print_statement = 56,           /* print_statement  */
  YYSYMBOL_input_statement = 57,           /* input_statement  */
  YYSYMBOL_if_statement = 58,              /* if_statement  */
  YYSYMBOL_while_loop = 59,                /* while_loop  */
  YYSYMBOL_for_loop = 60,                  /* for_loop  */
  YYSYMBOL_return_statement = 61,          /* return_statement  */
  YYSYMBOL_condition = 62,                 /* condition  */
  YYSYMBOL_expr = 63,                      /* expr  */
  YYSYMBOL_bool_expr = 64                  /* bool_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   244

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  56
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  142

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    21,     2,     2,
      25,    26,    19,    17,    30,    18,     2,    20,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    29,
      23,    22,    24,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,    32,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,    28,     2,     2,     2,     2,
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
      15,    16,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   215,   215,   215,   219,   219,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   235,   250,   258,   274,
     282,   297,   305,   320,   332,   344,   362,   369,   374,   384,
     396,   399,   404,   409,   414,   422,   423,   424,   428,   440,
     452,   464,   476,   486,   492,   498,   514,   517,   526,   534,
     542,   550,   558,   564,   570,   576,   582
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER", "NUMBER",
  "FLOAT_LITERAL", "STRING_LITERAL", "BOOL_LITERAL", "INT", "FLOAT", "IF",
  "ELSE", "RETURN", "WHILE", "FOR", "PRINT", "INPUT", "'+'", "'-'", "'*'",
  "'/'", "'%'", "'='", "'<'", "'>'", "'('", "')'", "'{'", "'}'", "';'",
  "','", "'['", "']'", "AND", "OR", "NOT", "EQ", "NEQ", "LEQ", "GEQ",
  "TRUE", "FALSE", "STRING", "BOOL", "VOID", "UMINUS", "IFX", "$accept",
  "program", "$@1", "statement_list", "statement", "declaration",
  "assignment", "array_assignment", "array_expr", "print_statement",
  "input_statement", "if_statement", "while_loop", "for_loop",
  "return_statement", "condition", "expr", "bool_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-39)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-28)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -39,    10,   168,   -39,   -19,     8,    20,     4,   116,    15,
      21,   182,    27,   168,    47,    50,   168,   -39,   -16,    18,
      22,    39,    41,   -39,   -39,   -39,    54,   116,   116,    -5,
       5,   116,    42,   -39,   -39,   -39,   -39,   220,   116,    73,
      55,   220,    46,     6,    44,    67,   -39,   -39,   -39,   -39,
     -39,   -39,   -39,   220,   195,   116,    87,   116,    88,    68,
     184,   116,   116,   116,   116,   116,   116,    70,    79,    78,
     116,   -39,   116,   104,   193,    93,   220,    84,   220,    96,
      98,   116,   116,   116,   204,   172,   172,   -39,   -39,   -39,
     103,   116,   212,   220,    99,   -39,   193,   193,   128,   -27,
     116,   -39,   -39,   168,   220,   220,   220,   -39,   168,   108,
     -39,   -39,    48,   -39,   116,   116,   116,   116,   193,   193,
     220,    29,    90,    73,   -39,   220,   220,   220,   220,   -39,
     110,   133,   -39,   125,   131,   143,   168,   168,   126,   147,
     -39,   -39
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     3,     4,     0,     0,
       0,     0,     0,     9,    10,    11,     0,     0,     0,     0,
       0,     0,    45,    43,    44,    47,    46,    34,     0,     0,
      47,    26,    28,     0,     0,     0,     5,     6,     7,     8,
      13,    14,    12,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    15,     0,     0,     0,     0,    16,     0,    18,     0,
       0,     0,     0,     0,     0,    38,    39,    40,    41,    42,
       0,     0,     0,    20,     0,    55,     0,     0,     0,    22,
       0,    17,    19,     0,    36,    35,    37,    25,     0,     0,
      29,    21,     0,    54,     0,     0,     0,     0,     0,     0,
      24,     0,     0,     0,    56,    48,    49,    50,    51,    52,
      53,    30,    32,     0,     0,     0,     0,     0,     0,     0,
      31,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -39,   -39,   -39,   -13,   -12,   -39,   -38,   -39,   -39,   -39,
     -39,   -39,   -39,   -39,   -39,   -36,    -3,   -32
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    16,    17,    18,    19,    20,    36,    21,
      22,    23,    24,    25,    26,    59,    98,    99
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,    69,    67,    27,    46,    37,   118,   119,    41,     4,
       3,    29,    28,    47,     5,     6,     7,    55,     8,     9,
      10,    11,    12,    30,    53,    54,    56,    57,    60,    31,
      42,    46,     4,    13,    71,    60,    58,     5,     6,     7,
      38,     8,     9,    10,    11,    12,    39,    48,    14,    15,
      44,    49,    76,    45,    78,   109,    13,   131,    84,    85,
      86,    87,    88,    89,   112,   113,    72,    92,    50,    93,
      51,    14,    15,    61,   124,    73,    68,    70,   104,   105,
     106,   118,   119,    52,   -27,   133,   129,   130,    60,    74,
     121,    77,    79,     4,    80,   122,    90,   120,     5,     6,
       7,    27,     8,     9,    10,    11,    12,    91,    94,    46,
      46,   125,   126,   127,   128,   100,   101,    13,   132,    32,
      33,    34,    35,   138,   139,   103,    46,    46,   102,     4,
     108,   111,    14,    15,     5,     6,     7,   123,     8,     9,
      10,    11,    12,   118,   134,    62,    63,    64,    65,    66,
       4,   135,     0,    13,   140,     5,     6,     7,   136,     8,
       9,    10,    11,    12,   114,   115,   116,   117,    14,    15,
     137,     4,     0,     0,    13,   141,     5,     6,     7,     0,
       8,     9,    10,    11,    12,    32,    33,    34,    40,    14,
      15,    64,    65,    66,     0,    13,    32,    33,    34,    35,
      95,    62,    63,    64,    65,    66,     0,    81,    82,     0,
      14,    15,    62,    63,    64,    65,    66,     0,    96,     0,
      83,    62,    63,    64,    65,    66,     0,    75,    97,    62,
      63,    64,    65,    66,     0,     0,   107,    62,    63,    64,
      65,    66,     0,     0,   110
};

static const yytype_int16 yycheck[] =
{
      13,    39,    38,    22,    16,     8,    33,    34,    11,     3,
       0,     3,    31,    29,     8,     9,    10,    22,    12,    13,
      14,    15,    16,     3,    27,    28,    31,    22,    31,    25,
       3,    43,     3,    27,    28,    38,    31,     8,     9,    10,
      25,    12,    13,    14,    15,    16,    25,    29,    42,    43,
       3,    29,    55,     3,    57,    91,    27,    28,    61,    62,
      63,    64,    65,    66,    96,    97,    22,    70,    29,    72,
      29,    42,    43,    31,    26,    31,     3,    31,    81,    82,
      83,    33,    34,    29,    29,   123,   118,   119,    91,    22,
     103,     4,     4,     3,    26,   108,    26,   100,     8,     9,
      10,    22,    12,    13,    14,    15,    16,    29,     4,   121,
     122,   114,   115,   116,   117,    22,    32,    27,    28,     3,
       4,     5,     6,   136,   137,    27,   138,   139,    32,     3,
      27,    32,    42,    43,     8,     9,    10,    29,    12,    13,
      14,    15,    16,    33,    11,    17,    18,    19,    20,    21,
       3,    26,    -1,    27,    28,     8,     9,    10,    27,    12,
      13,    14,    15,    16,    36,    37,    38,    39,    42,    43,
      27,     3,    -1,    -1,    27,    28,     8,     9,    10,    -1,
      12,    13,    14,    15,    16,     3,     4,     5,     6,    42,
      43,    19,    20,    21,    -1,    27,     3,     4,     5,     6,
       7,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      42,    43,    17,    18,    19,    20,    21,    -1,    25,    -1,
      36,    17,    18,    19,    20,    21,    -1,    32,    35,    17,
      18,    19,    20,    21,    -1,    -1,    32,    17,    18,    19,
      20,    21,    -1,    -1,    32
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    48,    49,     0,     3,     8,     9,    10,    12,    13,
      14,    15,    16,    27,    42,    43,    50,    51,    52,    53,
      54,    56,    57,    58,    59,    60,    61,    22,    31,     3,
       3,    25,     3,     4,     5,     6,    55,    63,    25,    25,
       6,    63,     3,    50,     3,     3,    51,    29,    29,    29,
      29,    29,    29,    63,    63,    22,    31,    22,    31,    62,
      63,    31,    17,    18,    19,    20,    21,    62,     3,    53,
      31,    28,    22,    31,    22,    32,    63,     4,    63,     4,
      26,    23,    24,    36,    63,    63,    63,    63,    63,    63,
      26,    29,    63,    63,     4,     7,    25,    35,    63,    64,
      22,    32,    32,    27,    63,    63,    63,    32,    27,    62,
      32,    32,    64,    64,    36,    37,    38,    39,    33,    34,
      63,    50,    50,    29,    26,    63,    63,    63,    63,    64,
      64,    28,    28,    53,    11,    26,    27,    27,    50,    50,
      28,    28
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    47,    49,    48,    50,    50,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    52,    52,    52,    52,
      52,    52,    52,    53,    54,    55,    56,    56,    57,    57,
      58,    58,    59,    60,    61,    62,    62,    62,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    64,    64,
      64,    64,    64,    64,    64,    64,    64
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     2,     2,     2,     2,     1,
       1,     1,     2,     2,     2,     3,     4,     5,     4,     5,
       4,     5,     4,     3,     6,     4,     2,     2,     2,     5,
       7,    11,     7,    11,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     2,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 215 "parser.y"
    { init_symbol_table(); }
#line 1395 "parser.tab.c"
    break;

  case 3: /* program: $@1 statement_list  */
#line 217 "parser.y"
    { cleanup_symbol_table(); }
#line 1401 "parser.tab.c"
    break;

  case 16: /* declaration: INT IDENTIFIER '=' expr  */
#line 235 "parser.y"
                              {
        if ((yyvsp[0].exprval).is_array) {
            yyerror("Cannot assign array to scalar");
        } else if ((yyvsp[0].exprval).is_bool) {
            yyerror("Cannot assign boolean to int");
        } else {
            Symbol* sym = insert_symbol((yyvsp[-2].strval), TYPE_INT, 0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                sym->int_val = (yyvsp[0].exprval).int_val;
                printf("%s = %s\n", (yyvsp[-2].strval), (yyvsp[0].exprval).temp_var);
            }
        }
    }
#line 1421 "parser.tab.c"
    break;

  case 17: /* declaration: INT IDENTIFIER '[' NUMBER ']'  */
#line 250 "parser.y"
                                    {
        Symbol* sym = insert_symbol((yyvsp[-3].strval), TYPE_INT, (yyvsp[-1].intval));
        if (!sym) {
            yyerror("Redeclaration of variable");
        } else {
            printf("Array %s[%d] declared\n", (yyvsp[-3].strval), (yyvsp[-1].intval));
        }
    }
#line 1434 "parser.tab.c"
    break;

  case 18: /* declaration: FLOAT IDENTIFIER '=' expr  */
#line 258 "parser.y"
                                {
        if ((yyvsp[0].exprval).is_array) {
            yyerror("Cannot assign array to scalar");
        } else if ((yyvsp[0].exprval).is_bool) {
            yyerror("Cannot assign boolean to float");
        } else {
            float val = (yyvsp[0].exprval).is_float ? (yyvsp[0].exprval).float_val : (float)(yyvsp[0].exprval).int_val;
            Symbol* sym = insert_symbol((yyvsp[-2].strval), TYPE_FLOAT, 0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                sym->float_val = val;
                printf("%s = %s\n", (yyvsp[-2].strval), (yyvsp[0].exprval).temp_var);
            }
        }
    }
#line 1455 "parser.tab.c"
    break;

  case 19: /* declaration: FLOAT IDENTIFIER '[' NUMBER ']'  */
#line 274 "parser.y"
                                      {
        Symbol* sym = insert_symbol((yyvsp[-3].strval), TYPE_FLOAT, (yyvsp[-1].intval));
        if (!sym) {
            yyerror("Redeclaration of variable");
        } else {
            printf("Array %s[%d] declared\n", (yyvsp[-3].strval), (yyvsp[-1].intval));
        }
    }
#line 1468 "parser.tab.c"
    break;

  case 20: /* declaration: STRING IDENTIFIER '=' expr  */
#line 282 "parser.y"
                                 {
        if ((yyvsp[0].exprval).is_array) {
            yyerror("Cannot assign array to scalar");
        } else if (!(yyvsp[0].exprval).str_val) {
            yyerror("Cannot assign non-string to string");
        } else {
            Symbol* sym = insert_symbol((yyvsp[-2].strval), TYPE_STRING, 0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                sym->str_val = strdup((yyvsp[0].exprval).str_val);
                printf("%s = %s\n", (yyvsp[-2].strval), (yyvsp[0].exprval).temp_var);
            }
        }
    }
#line 1488 "parser.tab.c"
    break;

  case 21: /* declaration: STRING IDENTIFIER '[' NUMBER ']'  */
#line 297 "parser.y"
                                       {
        Symbol* sym = insert_symbol((yyvsp[-3].strval), TYPE_STRING, (yyvsp[-1].intval));
        if (!sym) {
            yyerror("Redeclaration of variable");
        } else {
            printf("Array %s[%d] declared\n", (yyvsp[-3].strval), (yyvsp[-1].intval));
        }
    }
#line 1501 "parser.tab.c"
    break;

  case 22: /* declaration: BOOL IDENTIFIER '=' bool_expr  */
#line 305 "parser.y"
                                    {
        if ((yyvsp[0].exprval).is_array) {
            yyerror("Cannot assign array to scalar");
        } else {
            Symbol* sym = insert_symbol((yyvsp[-2].strval), TYPE_BOOL, 0);
            if (!sym) {
                yyerror("Redeclaration of variable");
            } else {
                sym->int_val = (yyvsp[0].exprval).int_val;
                printf("%s = %s\n", (yyvsp[-2].strval), (yyvsp[0].exprval).temp_var);
            }
        }
    }
#line 1519 "parser.tab.c"
    break;

  case 23: /* assignment: IDENTIFIER '=' expr  */
#line 320 "parser.y"
                        {
        Symbol* sym = lookup_symbol((yyvsp[-2].strval));
        if (!sym) {
            yyerror("Undefined variable");
        } else if (sym->is_array) {
            yyerror("Cannot assign to array variable directly");
        } else {
            printf("%s = %s\n", (yyvsp[-2].strval), (yyvsp[0].exprval).temp_var);
        }
    }
#line 1534 "parser.tab.c"
    break;

  case 24: /* array_assignment: IDENTIFIER '[' expr ']' '=' expr  */
#line 332 "parser.y"
                                     {
        Symbol* sym = lookup_symbol((yyvsp[-5].strval));
        if (!sym) {
            yyerror("Undefined variable");
        } else if (!(yyvsp[-3].exprval).is_array && (yyvsp[-3].exprval).int_val >= 0 && (yyvsp[-3].exprval).int_val < sym->size) {
            printf("%s[%s] = %s\n", (yyvsp[-5].strval), (yyvsp[-3].exprval).temp_var, (yyvsp[0].exprval).temp_var);
        } else {
            yyerror("Array index out of bounds");
        }
    }
#line 1549 "parser.tab.c"
    break;

  case 25: /* array_expr: IDENTIFIER '[' expr ']'  */
#line 344 "parser.y"
                            {
        Symbol* sym = lookup_symbol((yyvsp[-3].strval));
        if (!sym) {
            yyerror("Undefined variable");
            (yyval.exprval).int_val = 0;
            (yyval.exprval).float_val = 0.0;
            (yyval.exprval).is_float = 0;
        } else if (!(yyvsp[-1].exprval).is_array && (yyvsp[-1].exprval).int_val >= 0 && (yyvsp[-1].exprval).int_val < sym->size) {
            (yyval.exprval).is_array = 1;
            (yyval.exprval).array_index = (yyvsp[-1].exprval).int_val;
            (yyval.exprval).temp_var = new_temp();
            printf("%s = %s[%s]\n", (yyval.exprval).temp_var, (yyvsp[-3].strval), (yyvsp[-1].exprval).temp_var);
        } else {
            yyerror("Array index out of bounds");
        }
    }
#line 1570 "parser.tab.c"
    break;

  case 26: /* print_statement: PRINT expr  */
#line 362 "parser.y"
               {
        if ((yyvsp[0].exprval).is_array) {
            yyerror("Cannot print array directly");
        } else {
            printf("print %s\n", (yyvsp[0].exprval).temp_var);
        }
    }
#line 1582 "parser.tab.c"
    break;

  case 27: /* print_statement: PRINT STRING_LITERAL  */
#line 369 "parser.y"
                           {
        printf("print \"%s\"\n", (yyvsp[0].strval));
    }
#line 1590 "parser.tab.c"
    break;

  case 28: /* input_statement: INPUT IDENTIFIER  */
#line 374 "parser.y"
                     {
        Symbol* sym = lookup_symbol((yyvsp[0].strval));
        if (!sym) {
            yyerror("Undefined variable");
        } else if (sym->is_array) {
            yyerror("Cannot input to array variable directly");
        } else {
            printf("input %s\n", (yyvsp[0].strval));
        }
    }
#line 1605 "parser.tab.c"
    break;

  case 29: /* input_statement: INPUT IDENTIFIER '[' expr ']'  */
#line 384 "parser.y"
                                    {
        Symbol* sym = lookup_symbol((yyvsp[-3].strval));
        if (!sym) {
            yyerror("Undefined variable");
        } else if (!(yyvsp[-1].exprval).is_array && (yyvsp[-1].exprval).int_val >= 0 && (yyvsp[-1].exprval).int_val < sym->size) {
            printf("input %s[%s]\n", (yyvsp[-3].strval), (yyvsp[-1].exprval).temp_var);
        } else {
            yyerror("Array index out of bounds");
        }
    }
#line 1620 "parser.tab.c"
    break;

  case 30: /* if_statement: IF '(' condition ')' '{' statement_list '}'  */
#line 396 "parser.y"
                                                {
        printf("If condition true\n");
    }
#line 1628 "parser.tab.c"
    break;

  case 31: /* if_statement: IF '(' condition ')' '{' statement_list '}' ELSE '{' statement_list '}'  */
#line 399 "parser.y"
                                                                              {
        printf("If-Else executed\n");
    }
#line 1636 "parser.tab.c"
    break;

  case 32: /* while_loop: WHILE '(' condition ')' '{' statement_list '}'  */
#line 404 "parser.y"
                                                   {
        printf("While loop executed\n");
    }
#line 1644 "parser.tab.c"
    break;

  case 33: /* for_loop: FOR '(' assignment ';' condition ';' assignment ')' '{' statement_list '}'  */
#line 409 "parser.y"
                                                                               {
        printf("For loop executed\n");
    }
#line 1652 "parser.tab.c"
    break;

  case 34: /* return_statement: RETURN expr  */
#line 414 "parser.y"
                {
        if((yyvsp[0].exprval).is_float)
            printf("Return %.2f\n", (yyvsp[0].exprval).float_val);
        else
            printf("Return %d\n", (yyvsp[0].exprval).int_val);
    }
#line 1663 "parser.tab.c"
    break;

  case 35: /* condition: expr '>' expr  */
#line 422 "parser.y"
                    { (yyval.intval) = 1; printf("if %s > %s\n", (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var); }
#line 1669 "parser.tab.c"
    break;

  case 36: /* condition: expr '<' expr  */
#line 423 "parser.y"
                    { (yyval.intval) = 1; printf("if %s < %s\n", (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var); }
#line 1675 "parser.tab.c"
    break;

  case 37: /* condition: expr EQ expr  */
#line 424 "parser.y"
                   { (yyval.intval) = 1; printf("if %s == %s\n", (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var); }
#line 1681 "parser.tab.c"
    break;

  case 38: /* expr: expr '+' expr  */
#line 428 "parser.y"
                    {
        (yyval.exprval) = (yyvsp[-2].exprval);
        if ((yyvsp[-2].exprval).is_float || (yyvsp[0].exprval).is_float) {
            (yyval.exprval).is_float = 1;
            (yyval.exprval).float_val = ((yyvsp[-2].exprval).is_float ? (yyvsp[-2].exprval).float_val : (yyvsp[-2].exprval).int_val) +
                           ((yyvsp[0].exprval).is_float ? (yyvsp[0].exprval).float_val : (yyvsp[0].exprval).int_val);
        } else {
            (yyval.exprval).int_val = (yyvsp[-2].exprval).int_val + (yyvsp[0].exprval).int_val;
        }
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s + %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1698 "parser.tab.c"
    break;

  case 39: /* expr: expr '-' expr  */
#line 440 "parser.y"
                    {
        (yyval.exprval) = (yyvsp[-2].exprval);
        if ((yyvsp[-2].exprval).is_float || (yyvsp[0].exprval).is_float) {
            (yyval.exprval).is_float = 1;
            (yyval.exprval).float_val = ((yyvsp[-2].exprval).is_float ? (yyvsp[-2].exprval).float_val : (yyvsp[-2].exprval).int_val) -
                           ((yyvsp[0].exprval).is_float ? (yyvsp[0].exprval).float_val : (yyvsp[0].exprval).int_val);
        } else {
            (yyval.exprval).int_val = (yyvsp[-2].exprval).int_val - (yyvsp[0].exprval).int_val;
        }
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s - %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1715 "parser.tab.c"
    break;

  case 40: /* expr: expr '*' expr  */
#line 452 "parser.y"
                    {
        (yyval.exprval) = (yyvsp[-2].exprval);
        if ((yyvsp[-2].exprval).is_float || (yyvsp[0].exprval).is_float) {
            (yyval.exprval).is_float = 1;
            (yyval.exprval).float_val = ((yyvsp[-2].exprval).is_float ? (yyvsp[-2].exprval).float_val : (yyvsp[-2].exprval).int_val) *
                           ((yyvsp[0].exprval).is_float ? (yyvsp[0].exprval).float_val : (yyvsp[0].exprval).int_val);
        } else {
            (yyval.exprval).int_val = (yyvsp[-2].exprval).int_val * (yyvsp[0].exprval).int_val;
        }
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s * %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1732 "parser.tab.c"
    break;

  case 41: /* expr: expr '/' expr  */
#line 464 "parser.y"
                    {
        (yyval.exprval) = (yyvsp[-2].exprval);
        if ((yyvsp[-2].exprval).is_float || (yyvsp[0].exprval).is_float) {
            (yyval.exprval).is_float = 1;
            (yyval.exprval).float_val = ((yyvsp[-2].exprval).is_float ? (yyvsp[-2].exprval).float_val : (yyvsp[-2].exprval).int_val) /
                           ((yyvsp[0].exprval).is_float ? (yyvsp[0].exprval).float_val : (yyvsp[0].exprval).int_val);
        } else {
            (yyval.exprval).int_val = (yyvsp[-2].exprval).int_val / (yyvsp[0].exprval).int_val;
        }
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s / %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1749 "parser.tab.c"
    break;

  case 42: /* expr: expr '%' expr  */
#line 476 "parser.y"
                    {
        if ((yyvsp[-2].exprval).is_float || (yyvsp[0].exprval).is_float) {
            yyerror("Modulo operation not allowed on floating point numbers");
        } else {
            (yyval.exprval).int_val = (yyvsp[-2].exprval).int_val % (yyvsp[0].exprval).int_val;
            (yyval.exprval).is_float = 0;
            (yyval.exprval).temp_var = new_temp();
            printf("%s = %s %% %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
        }
    }
#line 1764 "parser.tab.c"
    break;

  case 43: /* expr: NUMBER  */
#line 486 "parser.y"
             {
        (yyval.exprval).int_val = (yyvsp[0].intval);
        (yyval.exprval).is_float = 0;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %d\n", (yyval.exprval).temp_var, (yyvsp[0].intval));
    }
#line 1775 "parser.tab.c"
    break;

  case 44: /* expr: FLOAT_LITERAL  */
#line 492 "parser.y"
                    {
        (yyval.exprval).float_val = (yyvsp[0].floatval);
        (yyval.exprval).is_float = 1;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %.2f\n", (yyval.exprval).temp_var, (yyvsp[0].floatval));
    }
#line 1786 "parser.tab.c"
    break;

  case 45: /* expr: IDENTIFIER  */
#line 498 "parser.y"
                 {
        Symbol* s = lookup_symbol((yyvsp[0].strval));
        if (!s) {
            yyerror("Undefined variable");
            (yyval.exprval).int_val = 0;
            (yyval.exprval).float_val = 0.0;
            (yyval.exprval).is_float = 0;
        } else if (s->is_array) {
            yyerror("Cannot use array variable directly");
        } else {
            (yyval.exprval).is_float = (s->type == TYPE_FLOAT);
            (yyval.exprval).int_val = s->int_val;
            (yyval.exprval).float_val = s->float_val;
        }
        (yyval.exprval).temp_var = strdup((yyvsp[0].strval));
    }
#line 1807 "parser.tab.c"
    break;

  case 46: /* expr: array_expr  */
#line 514 "parser.y"
                 {
        (yyval.exprval) = (yyvsp[0].exprval);
    }
#line 1815 "parser.tab.c"
    break;

  case 47: /* expr: STRING_LITERAL  */
#line 517 "parser.y"
                     {
        (yyval.exprval).str_val = (yyvsp[0].strval);
        (yyval.exprval).is_float = 0;
        (yyval.exprval).is_bool = 0;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = \"%s\"\n", (yyval.exprval).temp_var, (yyvsp[0].strval));
    }
#line 1827 "parser.tab.c"
    break;

  case 48: /* bool_expr: expr EQ expr  */
#line 526 "parser.y"
                   {
        (yyval.exprval).is_bool = 1;
        (yyval.exprval).int_val = ((yyvsp[-2].exprval).is_float || (yyvsp[0].exprval).is_float) ?
            ((yyvsp[-2].exprval).is_float ? (yyvsp[-2].exprval).float_val : (yyvsp[-2].exprval).int_val) == ((yyvsp[0].exprval).is_float ? (yyvsp[0].exprval).float_val : (yyvsp[0].exprval).int_val) :
            (yyvsp[-2].exprval).int_val == (yyvsp[0].exprval).int_val;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s == %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1840 "parser.tab.c"
    break;

  case 49: /* bool_expr: expr NEQ expr  */
#line 534 "parser.y"
                    {
        (yyval.exprval).is_bool = 1;
        (yyval.exprval).int_val = ((yyvsp[-2].exprval).is_float || (yyvsp[0].exprval).is_float) ?
            ((yyvsp[-2].exprval).is_float ? (yyvsp[-2].exprval).float_val : (yyvsp[-2].exprval).int_val) != ((yyvsp[0].exprval).is_float ? (yyvsp[0].exprval).float_val : (yyvsp[0].exprval).int_val) :
            (yyvsp[-2].exprval).int_val != (yyvsp[0].exprval).int_val;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s != %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1853 "parser.tab.c"
    break;

  case 50: /* bool_expr: expr LEQ expr  */
#line 542 "parser.y"
                    {
        (yyval.exprval).is_bool = 1;
        (yyval.exprval).int_val = ((yyvsp[-2].exprval).is_float || (yyvsp[0].exprval).is_float) ?
            ((yyvsp[-2].exprval).is_float ? (yyvsp[-2].exprval).float_val : (yyvsp[-2].exprval).int_val) <= ((yyvsp[0].exprval).is_float ? (yyvsp[0].exprval).float_val : (yyvsp[0].exprval).int_val) :
            (yyvsp[-2].exprval).int_val <= (yyvsp[0].exprval).int_val;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s <= %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1866 "parser.tab.c"
    break;

  case 51: /* bool_expr: expr GEQ expr  */
#line 550 "parser.y"
                    {
        (yyval.exprval).is_bool = 1;
        (yyval.exprval).int_val = ((yyvsp[-2].exprval).is_float || (yyvsp[0].exprval).is_float) ?
            ((yyvsp[-2].exprval).is_float ? (yyvsp[-2].exprval).float_val : (yyvsp[-2].exprval).int_val) >= ((yyvsp[0].exprval).is_float ? (yyvsp[0].exprval).float_val : (yyvsp[0].exprval).int_val) :
            (yyvsp[-2].exprval).int_val >= (yyvsp[0].exprval).int_val;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s >= %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1879 "parser.tab.c"
    break;

  case 52: /* bool_expr: bool_expr AND bool_expr  */
#line 558 "parser.y"
                              {
        (yyval.exprval).is_bool = 1;
        (yyval.exprval).int_val = (yyvsp[-2].exprval).int_val && (yyvsp[0].exprval).int_val;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s && %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1890 "parser.tab.c"
    break;

  case 53: /* bool_expr: bool_expr OR bool_expr  */
#line 564 "parser.y"
                             {
        (yyval.exprval).is_bool = 1;
        (yyval.exprval).int_val = (yyvsp[-2].exprval).int_val || (yyvsp[0].exprval).int_val;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s || %s\n", (yyval.exprval).temp_var, (yyvsp[-2].exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1901 "parser.tab.c"
    break;

  case 54: /* bool_expr: NOT bool_expr  */
#line 570 "parser.y"
                    {
        (yyval.exprval).is_bool = 1;
        (yyval.exprval).int_val = !(yyvsp[0].exprval).int_val;
        (yyval.exprval).temp_var = new_temp();
        printf("%s = !%s\n", (yyval.exprval).temp_var, (yyvsp[0].exprval).temp_var);
    }
#line 1912 "parser.tab.c"
    break;

  case 55: /* bool_expr: BOOL_LITERAL  */
#line 576 "parser.y"
                   {
        (yyval.exprval).is_bool = 1;
        (yyval.exprval).int_val = (yyvsp[0].intval);
        (yyval.exprval).temp_var = new_temp();
        printf("%s = %s\n", (yyval.exprval).temp_var, (yyvsp[0].intval) ? "true" : "false");
    }
#line 1923 "parser.tab.c"
    break;

  case 56: /* bool_expr: '(' bool_expr ')'  */
#line 582 "parser.y"
                        { (yyval.exprval) = (yyvsp[-1].exprval); }
#line 1929 "parser.tab.c"
    break;


#line 1933 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

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
                      yytoken, &yylval);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 584 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

/* Symbol table functions */
void add_symbol(const char* name, SymbolType type) {
    unsigned int index = hash(name);
    Symbol* sym = (Symbol*)malloc(sizeof(Symbol));
    if (!sym) return;
    
    strncpy(sym->name, name, MAX_NAME - 1);
    sym->name[MAX_NAME - 1] = '\0';
    sym->type = type;
    sym->next = symbol_table[index];
    symbol_table[index] = sym;
} 
