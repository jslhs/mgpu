/* A Bison parser, made by GNU Bison 2.7.1.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
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


/* First part of user declarations.  */

/* Line 283 of lalr1.cc  */
#line 38 "obj_parser.cpp"


#include "obj_parser.h"

/* User implementation prologue.  */

/* Line 289 of lalr1.cc  */
#line 46 "obj_parser.cpp"
/* Unqualified %code blocks.  */
/* Line 290 of lalr1.cc  */
#line 22 "obj_parser.y"

    #include "obj_reader.h"
	static int yylex(utility::obj_parser::semantic_type *yylval, utility::obj_parser::location_type *yyloc, utility::obj_reader &reader);


/* Line 290 of lalr1.cc  */
#line 56 "obj_parser.cpp"


# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location) YYUSE(Type)
# define YY_REDUCE_PRINT(Rule)        static_cast<void>(0)
# define YY_STACK_PRINT()             static_cast<void>(0)

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

/* Line 357 of lalr1.cc  */
#line 4 "obj_parser.y"
namespace utility {
/* Line 357 of lalr1.cc  */
#line 152 "obj_parser.cpp"

  /// Build a parser object.
  obj_parser::obj_parser (utility::obj_reader &reader_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      reader (reader_yyarg)
  {
  }

  obj_parser::~obj_parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  obj_parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    std::ostream& yyo = debug_stream ();
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    YYUSE (yytype);
  }


  void
  obj_parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  obj_parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    YYUSE (yytype);
  }

  void
  obj_parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  obj_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  obj_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  obj_parser::debug_level_type
  obj_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  obj_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  obj_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  obj_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  obj_parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    // State.
    int yyn;
    int yylen = 0;
    int yystate = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    static semantic_type yyval_default;
    semantic_type yylval = yyval_default;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_.clear ();
    yysemantic_stack_.clear ();
    yylocation_stack_.clear ();
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
        YYCDEBUG << "Reading a token: ";
        yychar = yylex (&yylval, &yylloc, reader);
      }

    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yy_table_value_is_error_ (yyn))
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    // Compute the default @$.
    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
          case 6:
/* Line 664 of lalr1.cc  */
#line 174 "obj_parser.y"
    { reader.parse_mtl(*(yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 7:
/* Line 664 of lalr1.cc  */
#line 175 "obj_parser.y"
    { reader.set_mtl_name((yysemantic_stack_[(1) - (1)].sval)); }
    break;

  case 8:
/* Line 664 of lalr1.cc  */
#line 176 "obj_parser.y"
    { reader.add((yysemantic_stack_[(1) - (1)].v)); }
    break;

  case 9:
/* Line 664 of lalr1.cc  */
#line 177 "obj_parser.y"
    { reader.add((yysemantic_stack_[(1) - (1)].vn)); }
    break;

  case 10:
/* Line 664 of lalr1.cc  */
#line 178 "obj_parser.y"
    { reader.add((yysemantic_stack_[(1) - (1)].vt)); }
    break;

  case 11:
/* Line 664 of lalr1.cc  */
#line 179 "obj_parser.y"
    { reader.add((yysemantic_stack_[(1) - (1)].f)); }
    break;

  case 12:
/* Line 664 of lalr1.cc  */
#line 180 "obj_parser.y"
    { reader.add((yysemantic_stack_[(1) - (1)].obj)); }
    break;

  case 14:
/* Line 664 of lalr1.cc  */
#line 182 "obj_parser.y"
    { reader.add((yysemantic_stack_[(1) - (1)].obj)); }
    break;

  case 15:
/* Line 664 of lalr1.cc  */
#line 185 "obj_parser.y"
    { reader.add((yysemantic_stack_[(1) - (1)].mtl)); }
    break;

  case 16:
/* Line 664 of lalr1.cc  */
#line 186 "obj_parser.y"
    { reader.set_mtl_Ka((yysemantic_stack_[(1) - (1)].color)); }
    break;

  case 17:
/* Line 664 of lalr1.cc  */
#line 187 "obj_parser.y"
    { reader.set_mtl_Kd((yysemantic_stack_[(1) - (1)].color)); }
    break;

  case 18:
/* Line 664 of lalr1.cc  */
#line 188 "obj_parser.y"
    { reader.set_mtl_Ks((yysemantic_stack_[(1) - (1)].color)); }
    break;

  case 19:
/* Line 664 of lalr1.cc  */
#line 189 "obj_parser.y"
    { reader.set_mtl_Ns((yysemantic_stack_[(1) - (1)].fval)); }
    break;

  case 20:
/* Line 664 of lalr1.cc  */
#line 190 "obj_parser.y"
    { reader.set_mtl_Ke((yysemantic_stack_[(1) - (1)].color)); }
    break;

  case 21:
/* Line 664 of lalr1.cc  */
#line 191 "obj_parser.y"
    { reader.set_mtl_d((yysemantic_stack_[(1) - (1)].fval)); }
    break;

  case 22:
/* Line 664 of lalr1.cc  */
#line 192 "obj_parser.y"
    { reader.set_mtl_illum((yysemantic_stack_[(1) - (1)].ival)); }
    break;

  case 23:
/* Line 664 of lalr1.cc  */
#line 193 "obj_parser.y"
    { reader.set_mtl_map_Ka((yysemantic_stack_[(1) - (1)].tex)); }
    break;

  case 24:
/* Line 664 of lalr1.cc  */
#line 194 "obj_parser.y"
    { reader.set_mtl_map_Kd((yysemantic_stack_[(1) - (1)].tex)); }
    break;

  case 25:
/* Line 664 of lalr1.cc  */
#line 195 "obj_parser.y"
    { reader.set_mtl_map_Ks((yysemantic_stack_[(1) - (1)].tex)); }
    break;

  case 26:
/* Line 664 of lalr1.cc  */
#line 196 "obj_parser.y"
    { reader.set_mtl_map_Ns((yysemantic_stack_[(1) - (1)].tex)); }
    break;

  case 27:
/* Line 664 of lalr1.cc  */
#line 197 "obj_parser.y"
    { reader.set_mtl_map_bump((yysemantic_stack_[(1) - (1)].tex)); }
    break;

  case 28:
/* Line 664 of lalr1.cc  */
#line 198 "obj_parser.y"
    { reader.set_mtl_map_d((yysemantic_stack_[(1) - (1)].tex)); }
    break;

  case 29:
/* Line 664 of lalr1.cc  */
#line 199 "obj_parser.y"
    { reader.set_mtl_disp((yysemantic_stack_[(1) - (1)].tex)); }
    break;

  case 30:
/* Line 664 of lalr1.cc  */
#line 200 "obj_parser.y"
    { reader.set_mtl_decal((yysemantic_stack_[(1) - (1)].tex)); }
    break;

  case 31:
/* Line 664 of lalr1.cc  */
#line 201 "obj_parser.y"
    { reader.set_mtl_refl((yysemantic_stack_[(1) - (1)].tex)); }
    break;

  case 32:
/* Line 664 of lalr1.cc  */
#line 202 "obj_parser.y"
    { reader.set_mtl_Tf((yysemantic_stack_[(1) - (1)].color)); }
    break;

  case 33:
/* Line 664 of lalr1.cc  */
#line 203 "obj_parser.y"
    { reader.set_mtl_Ni((yysemantic_stack_[(1) - (1)].fval)); }
    break;

  case 35:
/* Line 664 of lalr1.cc  */
#line 208 "obj_parser.y"
    { (yyval.sval) = (yysemantic_stack_[(2) - (2)].sval); }
    break;

  case 36:
/* Line 664 of lalr1.cc  */
#line 212 "obj_parser.y"
    { (yyval.v) = new obj::vertex((yysemantic_stack_[(5) - (2)].fval), (yysemantic_stack_[(5) - (3)].fval), (yysemantic_stack_[(5) - (4)].fval), (yysemantic_stack_[(5) - (5)].fval)); }
    break;

  case 37:
/* Line 664 of lalr1.cc  */
#line 213 "obj_parser.y"
    { (yyval.v) = new obj::vertex((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 38:
/* Line 664 of lalr1.cc  */
#line 217 "obj_parser.y"
    { (yyval.vn) = new obj::normal((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 39:
/* Line 664 of lalr1.cc  */
#line 221 "obj_parser.y"
    { (yyval.vt) = new obj::texture((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 40:
/* Line 664 of lalr1.cc  */
#line 222 "obj_parser.y"
    { (yyval.vt) = new obj::texture((yysemantic_stack_[(3) - (2)].fval), (yysemantic_stack_[(3) - (3)].fval)); }
    break;

  case 41:
/* Line 664 of lalr1.cc  */
#line 223 "obj_parser.y"
    { (yyval.vt) = new obj::texture((yysemantic_stack_[(2) - (2)].fval)); }
    break;

  case 42:
/* Line 664 of lalr1.cc  */
#line 227 "obj_parser.y"
    { (yyval.obj) = new obj::object((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 43:
/* Line 664 of lalr1.cc  */
#line 231 "obj_parser.y"
    { /*std::cout << "smoothing group: " << $2 << std::endl;*/ }
    break;

  case 44:
/* Line 664 of lalr1.cc  */
#line 232 "obj_parser.y"
    { /*std::cout << "smoothing group: " << $2 << std::endl;*/ }
    break;

  case 45:
/* Line 664 of lalr1.cc  */
#line 236 "obj_parser.y"
    { (yyval.iv) = new obj::ivertex((yysemantic_stack_[(5) - (1)].ival), (yysemantic_stack_[(5) - (3)].ival), (yysemantic_stack_[(5) - (5)].ival)); }
    break;

  case 46:
/* Line 664 of lalr1.cc  */
#line 237 "obj_parser.y"
    { (yyval.iv) = new obj::ivertex((yysemantic_stack_[(4) - (1)].ival), (yysemantic_stack_[(4) - (4)].ival)); }
    break;

  case 47:
/* Line 664 of lalr1.cc  */
#line 241 "obj_parser.y"
    { (yyval.f) = new obj::face((yysemantic_stack_[(5) - (2)].iv), (yysemantic_stack_[(5) - (3)].iv), (yysemantic_stack_[(5) - (4)].iv), (yysemantic_stack_[(5) - (5)].iv)); }
    break;

  case 48:
/* Line 664 of lalr1.cc  */
#line 242 "obj_parser.y"
    { (yyval.f) = new obj::face((yysemantic_stack_[(4) - (2)].iv), (yysemantic_stack_[(4) - (3)].iv), (yysemantic_stack_[(4) - (4)].iv)); }
    break;

  case 49:
/* Line 664 of lalr1.cc  */
#line 245 "obj_parser.y"
    { (yyval.obj) = new obj::object((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 50:
/* Line 664 of lalr1.cc  */
#line 249 "obj_parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].ival); }
    break;

  case 51:
/* Line 664 of lalr1.cc  */
#line 250 "obj_parser.y"
    { (yyval.fval) = (yysemantic_stack_[(1) - (1)].fval); }
    break;

  case 52:
/* Line 664 of lalr1.cc  */
#line 253 "obj_parser.y"
    { (yyval.sval) = (yysemantic_stack_[(1) - (1)].sval); }
    break;

  case 53:
/* Line 664 of lalr1.cc  */
#line 254 "obj_parser.y"
    { (yyval.sval) = (yysemantic_stack_[(1) - (1)].sval); }
    break;

  case 54:
/* Line 664 of lalr1.cc  */
#line 257 "obj_parser.y"
    { (yyval.mtl) = new obj::material((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 56:
/* Line 664 of lalr1.cc  */
#line 261 "obj_parser.y"
    { (yyval.color) = new obj::vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 58:
/* Line 664 of lalr1.cc  */
#line 265 "obj_parser.y"
    { (yyval.color) = new obj::vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 60:
/* Line 664 of lalr1.cc  */
#line 269 "obj_parser.y"
    { (yyval.color) = new obj::vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 62:
/* Line 664 of lalr1.cc  */
#line 273 "obj_parser.y"
    { (yyval.color) = new obj::vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 63:
/* Line 664 of lalr1.cc  */
#line 276 "obj_parser.y"
    { (yyval.fval) = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 64:
/* Line 664 of lalr1.cc  */
#line 277 "obj_parser.y"
    { (yyval.fval) = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 66:
/* Line 664 of lalr1.cc  */
#line 281 "obj_parser.y"
    { (yyval.fval) = (yysemantic_stack_[(2) - (2)].fval); }
    break;

  case 68:
/* Line 664 of lalr1.cc  */
#line 285 "obj_parser.y"
    { (yyval.ival) = (yysemantic_stack_[(2) - (2)].ival); }
    break;

  case 70:
/* Line 664 of lalr1.cc  */
#line 289 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 72:
/* Line 664 of lalr1.cc  */
#line 293 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 74:
/* Line 664 of lalr1.cc  */
#line 297 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 76:
/* Line 664 of lalr1.cc  */
#line 301 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 79:
/* Line 664 of lalr1.cc  */
#line 306 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 80:
/* Line 664 of lalr1.cc  */
#line 307 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 83:
/* Line 664 of lalr1.cc  */
#line 312 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 84:
/* Line 664 of lalr1.cc  */
#line 313 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 86:
/* Line 664 of lalr1.cc  */
#line 317 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 88:
/* Line 664 of lalr1.cc  */
#line 321 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 90:
/* Line 664 of lalr1.cc  */
#line 325 "obj_parser.y"
    { (yyval.tex) = new obj::tex_map((yysemantic_stack_[(2) - (2)].sval)); }
    break;

  case 91:
/* Line 664 of lalr1.cc  */
#line 327 "obj_parser.y"
    { (yyval.color) = new obj::vec3((yysemantic_stack_[(4) - (2)].fval), (yysemantic_stack_[(4) - (3)].fval), (yysemantic_stack_[(4) - (4)].fval)); }
    break;

  case 93:
/* Line 664 of lalr1.cc  */
#line 331 "obj_parser.y"
    { (yyval.fval) = (yysemantic_stack_[(2) - (2)].fval); }
    break;


/* Line 664 of lalr1.cc  */
#line 824 "obj_parser.cpp"
      default:
        break;
      }

    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */
        if (yychar <= yyeof_)
          {
            /* Return failure if at end of input.  */
            if (yychar == yyeof_)
              YYABORT;
          }
        else
          {
            yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
            yychar = yyempty_;
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
    if (false)
      goto yyerrorlab;

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (!yy_pact_value_is_default_ (yyn))
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	  YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystate_stack_.height ())
      {
        yydestruct_ ("Cleanup: popping",
                     yystos_[yystate_stack_[0]],
                     &yysemantic_stack_[0],
                     &yylocation_stack_[0]);
        yypop_ ();
      }

    return yyresult;
    }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (yychar != yyempty_)
          {
            /* Make sure we have latest lookahead translation.  See
               comments at user semantic actions for why this is
               necessary.  */
            yytoken = yytranslate_ (yychar);
            yydestruct_ (YY_NULL, yytoken, &yylval, &yylloc);
          }

        while (1 < yystate_stack_.height ())
          {
            yydestruct_ (YY_NULL,
                         yystos_[yystate_stack_[0]],
                         &yysemantic_stack_[0],
                         &yylocation_stack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  // Generate an error message.
  std::string
  obj_parser::yysyntax_error_ (int, int)
  {
    return YY_("syntax error");
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char obj_parser::yypact_ninf_ = -69;
  const signed char
  obj_parser::yypact_[] =
  {
       105,     2,     2,     2,    11,    -3,     5,     9,    10,    13,
     -69,    14,     2,     2,     2,     2,    17,     2,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,    19,    23,   105,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,     2,     2,     2,
     -58,    11,   -69,   -69,   -69,   -69,   -69,   -69,   -69,     2,
       2,     2,     2,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,     2,
     -69,   -69,   -69,   -69,   -69,     2,     2,     2,    -1,    11,
       2,     2,     2,     2,     2,     2,   -69,   -69,   -57,    24,
      11,   -69,   -69,   -69,   -69,   -69,   -69,    28,   -69,   -69,
     -69
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  obj_parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     0,    55,    57,    59,    61,    67,    65,    77,    78,
       0,    69,    71,    73,    75,    82,    81,    87,    85,    89,
       0,     0,    92,    94,     0,     2,     4,     7,     8,     9,
      10,    12,    13,    11,    14,    15,    16,    17,    18,    20,
      21,    19,    22,    23,    24,    25,    26,    27,    28,    30,
      29,    31,    32,    33,    34,    50,    51,     0,    41,     0,
       0,     0,    42,    44,    43,    49,    35,     6,    54,     0,
       0,     0,     0,    68,    66,    52,    53,    79,    80,    63,
      70,    72,    74,    76,    84,    83,    88,    86,    90,     0,
      64,    93,    95,     1,     3,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,    37,    39,    38,     0,     0,
      48,    56,    58,    60,    62,    91,    36,     0,    46,    47,
      45
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  obj_parser::yypgoto_[] =
  {
       -69,   -69,   -69,    -9,   -69,   -69,   -69,   -69,   -69,   -69,
     -68,   -69,   -69,    -2,    21,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  obj_parser::yydefgoto_[] =
  {
        -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      71,    43,    44,    67,    87,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char obj_parser::yytable_ninf_ = -1;
  const unsigned char
  obj_parser::yytable_[] =
  {
        68,    69,   118,   109,    72,    65,    66,    85,    73,    86,
      79,    80,    81,    82,    70,    84,    75,    76,    89,    77,
      83,    78,   102,   103,   108,   127,   104,   128,    99,   100,
     101,   130,     0,     0,     0,     0,     0,     0,     0,     0,
      88,   120,    90,    91,    92,    93,    94,    95,    96,    97,
      98,     0,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   105,   106,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,   111,   112,
     113,   119,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   114,     0,     0,
       0,     0,     0,   115,   116,   117,     0,     0,   121,   122,
     123,   124,   125,   126,     1,     2,     3,     0,     0,     0,
       0,     0,     0,     0,     4,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     0,     7,     0,
       0,     0,     0,     8,     9,     0,     0,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    30,    31,    32,    33
  };

  /* YYCHECK.  */
  const signed char
  obj_parser::yycheck_[] =
  {
         2,     3,     3,    71,     7,     3,     4,     6,     3,     8,
      12,    13,    14,    15,     3,    17,     7,     7,    20,     6,
       3,     7,     3,     0,    82,    82,    35,     3,    30,    31,
      32,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,   109,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    82,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,    -1,    -1,   110,   111,
     112,   113,   114,   115,     9,    10,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    81
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  obj_parser::yystos_[] =
  {
         0,     9,    10,    11,    19,    30,    31,    33,    38,    39,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      78,    79,    80,    81,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    95,    96,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,     3,     4,    97,    97,    97,
       3,    94,     7,     3,    77,     7,     7,     6,     7,    97,
      97,    97,    97,     3,    97,     6,     8,    98,    98,    97,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    97,
      97,    97,     3,     0,    87,    97,    97,    97,    82,    94,
      97,    97,    97,    97,    97,    97,    97,    97,     3,    82,
      94,    97,    97,    97,    97,    97,    97,    82,     3,    94,
       3
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  obj_parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  obj_parser::yyr1_[] =
  {
         0,    84,    85,    86,    86,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    88,    89,    89,    90,    91,
      91,    91,    92,    93,    93,    94,    94,    95,    95,    96,
      97,    97,    98,    98,    99,   100,   100,   101,   101,   102,
     102,   103,   103,   104,   104,   105,   105,   106,   106,   107,
     107,   108,   108,   109,   109,   110,   110,   111,   111,   111,
     111,   112,   112,   112,   112,   113,   113,   114,   114,   115,
     115,   116,   117,   117,   118,   118
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  obj_parser::yyr2_[] =
  {
         0,     2,     1,     2,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     5,     4,     4,     4,
       3,     2,     2,     2,     2,     5,     4,     5,     4,     2,
       1,     1,     1,     1,     2,     1,     4,     1,     4,     1,
       4,     1,     4,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     2,     1,     2,     1,     1,     2,
       2,     1,     1,     2,     2,     1,     2,     1,     2,     1,
       2,     4,     1,     2,     1,     2
  };

#if YYDEBUG
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const obj_parser::yytname_[] =
  {
    "$end", "error", "$undefined", "INTEGER", "FLOAT", "STRING", "FILENAME",
  "IDENTIFIER", "PATH", "VERTEX", "TEXTURE", "NORMAL", "SPACE_VERTEX",
  "DEGREE", "CURVE_SURFACE_TYPE", "BASIS_MATRIX", "STEP_SIZE", "POINT",
  "LINE", "FACE", "CURVE", "CURVE_2D", "SURFACE", "PARAMETER",
  "OUTER_TRIMMING_LOOP", "INNER_TRIMMING_LOOP", "SPECIAL_CURVE",
  "SPECIAL_POINT", "END", "CONNECT", "GROUP_NAME", "SMOOTHING_GROUP",
  "MERGING_GROUP", "OBJECT_NAME", "BEVEL_INTERPOLATION",
  "COLOR_INTERPOLATION", "DISSOLVE_INTERPOLATION", "LOD", "MATERIAL_NAME",
  "MATERIAL_LIBRARY", "SHADOW_CASTING", "RAY_TRACING", "CURVE_APPROX_TECH",
  "SURFACE_APPROX_TECH", "COMMENT", "NEWMTL", "KA", "KD", "KS", "KE",
  "ILLUM", "NS", "MAP_BUMP", "BUMP", "D", "MAP_KA", "MAP_KD", "MAP_KS",
  "MAP_NS", "MAP_OPACITY", "MAP_D", "DISP", "DECAL", "REFL", "BLENDU",
  "BLENDV", "BOOST", "MM", "O", "SCALE", "TURBULENCE", "TEX_RES", "CLAMP",
  "BUMP_MUL", "IMFCHAN", "TYPE", "ON", "OFF", "TF", "TR", "NI", "KM",
  "SLASH", "BACK_SLASH", "$accept", "obj_file", "obj_file_lines",
  "obj_file_line", "material_line", "vertex_line", "normal_line",
  "texture_line", "group_line", "smooth_group_line", "vertex_group",
  "face_line", "object_line", "real", "path", "newmtl_line",
  "ambient_line", "diffuse_line", "specular_line", "env_color_line",
  "dissolve_line", "specular_n_line", "illum_line", "ambient_map_line",
  "diffuse_map_line", "specular_map_line", "specular_n_map_line",
  "bump_map_line", "dissolve_map_line", "decal_map_line", "disp_map_line",
  "refl_map_line", "trans_filter_line", "optical_density_line", "km_line", YY_NULL
  };


  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const obj_parser::rhs_number_type
  obj_parser::yyrhs_[] =
  {
        85,     0,    -1,    86,    -1,    86,    87,    -1,    87,    -1,
      44,    -1,    39,     6,    -1,    88,    -1,    89,    -1,    90,
      -1,    91,    -1,    95,    -1,    92,    -1,    93,    -1,    96,
      -1,    99,    -1,   100,    -1,   101,    -1,   102,    -1,   105,
      -1,   103,    -1,   104,    -1,   106,    -1,   107,    -1,   108,
      -1,   109,    -1,   110,    -1,   111,    -1,   112,    -1,   114,
      -1,   113,    -1,   115,    -1,   116,    -1,   117,    -1,   118,
      -1,    38,     7,    -1,     9,    97,    97,    97,    97,    -1,
       9,    97,    97,    97,    -1,    11,    97,    97,    97,    -1,
      10,    97,    97,    97,    -1,    10,    97,    97,    -1,    10,
      97,    -1,    30,     7,    -1,    31,    77,    -1,    31,     3,
      -1,     3,    82,     3,    82,     3,    -1,     3,    82,    82,
       3,    -1,    19,    94,    94,    94,    94,    -1,    19,    94,
      94,    94,    -1,    33,     7,    -1,     3,    -1,     4,    -1,
       6,    -1,     8,    -1,    45,     7,    -1,    46,    -1,    46,
      97,    97,    97,    -1,    47,    -1,    47,    97,    97,    97,
      -1,    48,    -1,    48,    97,    97,    97,    -1,    49,    -1,
      49,    97,    97,    97,    -1,    54,    97,    -1,    79,    97,
      -1,    51,    -1,    51,    97,    -1,    50,    -1,    50,     3,
      -1,    55,    -1,    55,    98,    -1,    56,    -1,    56,    98,
      -1,    57,    -1,    57,    98,    -1,    58,    -1,    58,    98,
      -1,    52,    -1,    53,    -1,    52,    98,    -1,    53,    98,
      -1,    60,    -1,    59,    -1,    60,    98,    -1,    59,    98,
      -1,    62,    -1,    62,    98,    -1,    61,    -1,    61,    98,
      -1,    63,    -1,    63,    98,    -1,    78,    97,    97,    97,
      -1,    80,    -1,    80,    97,    -1,    81,    -1,    81,     3,
      -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  obj_parser::yyprhs_[] =
  {
         0,     0,     3,     5,     8,    10,    12,    15,    17,    19,
      21,    23,    25,    27,    29,    31,    33,    35,    37,    39,
      41,    43,    45,    47,    49,    51,    53,    55,    57,    59,
      61,    63,    65,    67,    69,    71,    74,    80,    85,    90,
      95,    99,   102,   105,   108,   111,   117,   122,   128,   133,
     136,   138,   140,   142,   144,   147,   149,   154,   156,   161,
     163,   168,   170,   175,   178,   181,   183,   186,   188,   191,
     193,   196,   198,   201,   203,   206,   208,   211,   213,   215,
     218,   221,   223,   225,   228,   231,   233,   236,   238,   241,
     243,   246,   251,   253,   256,   258
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  obj_parser::yyrline_[] =
  {
         0,   164,   164,   168,   169,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   208,   212,   213,   217,   221,
     222,   223,   227,   231,   232,   236,   237,   241,   242,   245,
     249,   250,   253,   254,   257,   260,   261,   264,   265,   268,
     269,   272,   273,   276,   277,   280,   281,   284,   285,   288,
     289,   292,   293,   296,   297,   300,   301,   304,   305,   306,
     307,   310,   311,   312,   313,   316,   317,   320,   321,   324,
     325,   327,   330,   331,   334,   335
  };

  // Print the state stack on the debug stream.
  void
  obj_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  obj_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  obj_parser::token_number_type
  obj_parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int obj_parser::yyeof_ = 0;
  const int obj_parser::yylast_ = 186;
  const int obj_parser::yynnts_ = 35;
  const int obj_parser::yyempty_ = -2;
  const int obj_parser::yyfinal_ = 103;
  const int obj_parser::yyterror_ = 1;
  const int obj_parser::yyerrcode_ = 256;
  const int obj_parser::yyntokens_ = 84;

  const unsigned int obj_parser::yyuser_token_number_max_ = 338;
  const obj_parser::token_number_type obj_parser::yyundef_token_ = 2;

/* Line 1135 of lalr1.cc  */
#line 4 "obj_parser.y"
} // utility
/* Line 1135 of lalr1.cc  */
#line 1414 "obj_parser.cpp"
/* Line 1136 of lalr1.cc  */
#line 337 "obj_parser.y"



void utility::obj_parser::error(const utility::obj_parser::location_type &loc, const std::string &msg)
{
	reader.error(loc, msg);
}

static int yylex(utility::obj_parser::semantic_type *yylval, utility::obj_parser::location_type *yyloc, utility::obj_reader &reader)
{
	return reader.scanner().yylex(yylval, yyloc);
}

