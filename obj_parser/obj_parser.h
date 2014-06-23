/* A Bison parser, made by GNU Bison 2.7.1.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2013 Free Software Foundation, Inc.
   
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

/**
 ** \file obj_parser.h
 ** Define the utility::parser class.
 */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef YY_YY_OBJ_PARSER_H_INCLUDED
# define YY_YY_OBJ_PARSER_H_INCLUDED

/* "%code requires" blocks.  */
/* Line 33 of lalr1.cc  */
#line 13 "obj_parser.y"

    #include "obj_file.h"
	namespace utility
	{
		class obj_reader;
	}


/* Line 33 of lalr1.cc  */
#line 56 "obj_parser.h"


#include <string>
#include <iostream>
#include "stack.hh"
#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Line 33 of lalr1.cc  */
#line 4 "obj_parser.y"
namespace utility {
/* Line 33 of lalr1.cc  */
#line 73 "obj_parser.h"

  /// A Bison parser.
  class obj_parser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {
/* Line 33 of lalr1.cc  */
#line 28 "obj_parser.y"

    int ival;
    float fval;
    std::string *sval;
    obj::vertex *v;
    obj::texture *vt;
    obj::normal *vn;
    obj::face *f;
    obj::object *obj;
    obj::ivertex *iv;
    obj::material *mtl;
	obj::vec3 *color;
	obj::tex_map *tex;


/* Line 33 of lalr1.cc  */
#line 101 "obj_parser.h"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
     INTEGER = 258,
     FLOAT = 259,
     STRING = 260,
     FILENAME = 261,
     IDENTIFIER = 262,
     PATH = 263,
     VERTEX = 264,
     TEXTURE = 265,
     NORMAL = 266,
     SPACE_VERTEX = 267,
     DEGREE = 268,
     CURVE_SURFACE_TYPE = 269,
     BASIS_MATRIX = 270,
     STEP_SIZE = 271,
     POINT = 272,
     LINE = 273,
     FACE = 274,
     CURVE = 275,
     CURVE_2D = 276,
     SURFACE = 277,
     PARAMETER = 278,
     OUTER_TRIMMING_LOOP = 279,
     INNER_TRIMMING_LOOP = 280,
     SPECIAL_CURVE = 281,
     SPECIAL_POINT = 282,
     END = 283,
     CONNECT = 284,
     GROUP_NAME = 285,
     SMOOTHING_GROUP = 286,
     MERGING_GROUP = 287,
     OBJECT_NAME = 288,
     BEVEL_INTERPOLATION = 289,
     COLOR_INTERPOLATION = 290,
     DISSOLVE_INTERPOLATION = 291,
     LOD = 292,
     MATERIAL_NAME = 293,
     MATERIAL_LIBRARY = 294,
     SHADOW_CASTING = 295,
     RAY_TRACING = 296,
     CURVE_APPROX_TECH = 297,
     SURFACE_APPROX_TECH = 298,
     COMMENT = 299,
     NEWMTL = 300,
     KA = 301,
     KD = 302,
     KS = 303,
     KE = 304,
     ILLUM = 305,
     NS = 306,
     MAP_BUMP = 307,
     BUMP = 308,
     D = 309,
     MAP_KA = 310,
     MAP_KD = 311,
     MAP_KS = 312,
     MAP_NS = 313,
     MAP_OPACITY = 314,
     MAP_D = 315,
     DISP = 316,
     DECAL = 317,
     REFL = 318,
     BLENDU = 319,
     BLENDV = 320,
     BOOST = 321,
     MM = 322,
     O = 323,
     SCALE = 324,
     TURBULENCE = 325,
     TEX_RES = 326,
     CLAMP = 327,
     BUMP_MUL = 328,
     IMFCHAN = 329,
     TYPE = 330,
     ON = 331,
     OFF = 332,
     TF = 333,
     TR = 334,
     NI = 335,
     SLASH = 336,
     BACK_SLASH = 337
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    obj_parser (utility::obj_reader &reader_yyarg);
    virtual ~obj_parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// This class is not copyable.
    obj_parser (const obj_parser&);
    obj_parser& operator= (const obj_parser&);

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    /// Internal symbol numbers.
    typedef unsigned char token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const signed char yypact_[];
    static const signed char yypact_ninf_;

    /// For a state, default reduction number.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned char yydefact_[];

    static const signed char yypgoto_[];
    static const signed char yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const unsigned char yytable_[];
    static const signed char yytable_ninf_;

    static const signed char yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned char yystos_[];

    /// For a rule, its LHS.
    static const unsigned char yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[]; 

#if YYDEBUG
    /// For a symbol, its name in clear.
    static const char* const yytname_[];

    /// A type to store symbol numbers and -1.
    typedef signed char rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned char yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    ///                     If null, do not display the symbol, just free it.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

    /* User arguments.  */
    utility::obj_reader &reader;
  };
/* Line 33 of lalr1.cc  */
#line 4 "obj_parser.y"
} // utility
/* Line 33 of lalr1.cc  */
#line 372 "obj_parser.h"



#endif /* !YY_YY_OBJ_PARSER_H_INCLUDED  */
