// A Bison parser, made by GNU Bison 3.4.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.



// First part of user prologue.
#line 1 "../tools/src/synExpParser.ypp"
 /*** C/C++ Declarations ***/

#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <set>


#line 98 "../tools/src/synExpParser.ypp"


#include "synExpDriver.hpp"
#include "synExpScanner.hpp"
#include "varInfo.hpp"


/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex


#line 65 "src/synExpParser.cpp"


#include "synExpParser.hpp"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
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
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 30 "../tools/src/synExpParser.ypp"
namespace kconf {
#line 162 "src/synExpParser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  synExpParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
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
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  synExpParser::synExpParser (class synExpDriver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  synExpParser::~synExpParser ()
  {}

  synExpParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  synExpParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  synExpParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  synExpParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  synExpParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  synExpParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  synExpParser::symbol_number_type
  synExpParser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  synExpParser::stack_symbol_type::stack_symbol_type ()
  {}

  synExpParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 3: // T_MAINMENU
      case 4: // T_MENU
        value.YY_MOVE_OR_COPY< kconf_id* > (YY_MOVE (that.value));
        break;

      case 5: // T_LITERAL
      case 6: // T_SYMBOL
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 26: // seqexp
        value.YY_MOVE_OR_COPY< std::vector<synExp*> > (YY_MOVE (that.value));
        break;

      case 25: // exp
        value.YY_MOVE_OR_COPY< synExp* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  synExpParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 3: // T_MAINMENU
      case 4: // T_MENU
        value.move< kconf_id* > (YY_MOVE (that.value));
        break;

      case 5: // T_LITERAL
      case 6: // T_SYMBOL
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 26: // seqexp
        value.move< std::vector<synExp*> > (YY_MOVE (that.value));
        break;

      case 25: // exp
        value.move< synExp* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  synExpParser::stack_symbol_type&
  synExpParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 3: // T_MAINMENU
      case 4: // T_MENU
        value.move< kconf_id* > (that.value);
        break;

      case 5: // T_LITERAL
      case 6: // T_SYMBOL
        value.move< std::string > (that.value);
        break;

      case 26: // seqexp
        value.move< std::vector<synExp*> > (that.value);
        break;

      case 25: // exp
        value.move< synExp* > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  synExpParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  synExpParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
      case 5: // T_LITERAL
#line 95 "../tools/src/synExpParser.ypp"
        { yyoutput << yysym.value.template as < std::string > (); }
#line 397 "src/synExpParser.cpp"
        break;

      case 6: // T_SYMBOL
#line 95 "../tools/src/synExpParser.ypp"
        { yyoutput << yysym.value.template as < std::string > (); }
#line 403 "src/synExpParser.cpp"
        break;

      default:
        break;
    }
    yyo << ')';
  }
#endif

  void
  synExpParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  synExpParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  synExpParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  synExpParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  synExpParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  synExpParser::debug_level_type
  synExpParser::debug_level () const
  {
    return yydebug_;
  }

  void
  synExpParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  synExpParser::state_type
  synExpParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  synExpParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  synExpParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  synExpParser::operator() ()
  {
    return parse ();
  }

  int
  synExpParser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    // User initialization code.
#line 42 "../tools/src/synExpParser.ypp"
{
    // initialize the initial location object
    yyla.location.begin.filename = yyla.location.end.filename = &driver.streamname;
}

#line 528 "src/synExpParser.cpp"


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 3: // T_MAINMENU
      case 4: // T_MENU
        yylhs.value.emplace< kconf_id* > ();
        break;

      case 5: // T_LITERAL
      case 6: // T_SYMBOL
        yylhs.value.emplace< std::string > ();
        break;

      case 26: // seqexp
        yylhs.value.emplace< std::vector<synExp*> > ();
        break;

      case 25: // exp
        yylhs.value.emplace< synExp* > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 116 "../tools/src/synExpParser.ypp"
    {
    driver.addExp(yystack_[1].value.as < synExp* > ());
}
#line 673 "src/synExpParser.cpp"
    break;

  case 3:
#line 120 "../tools/src/synExpParser.ypp"
    {
    driver.addExp(yystack_[1].value.as < synExp* > ());
}
#line 681 "src/synExpParser.cpp"
    break;

  case 4:
#line 124 "../tools/src/synExpParser.ypp"
    {
    
}
#line 689 "src/synExpParser.cpp"
    break;

  case 5:
#line 127 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synCompound(synAnd, yystack_[2].value.as < synExp* > (), yystack_[0].value.as < synExp* > ());
}
#line 697 "src/synExpParser.cpp"
    break;

  case 6:
#line 131 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synCompound(synOr, yystack_[2].value.as < synExp* > (), yystack_[0].value.as < synExp* > ());
}
#line 705 "src/synExpParser.cpp"
    break;

  case 7:
#line 135 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synCompound(synNot, yystack_[0].value.as < synExp* > ());
}
#line 713 "src/synExpParser.cpp"
    break;

  case 8:
#line 139 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synXOR(yystack_[1].value.as < std::vector<synExp*> > ());
}
#line 721 "src/synExpParser.cpp"
    break;

  case 9:
#line 143 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < std::vector<synExp*> > ().push_back(yystack_[0].value.as < synExp* > ());
}
#line 729 "src/synExpParser.cpp"
    break;

  case 10:
#line 147 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < std::vector<synExp*> > () = yystack_[1].value.as < std::vector<synExp*> > ();
    yylhs.value.as < std::vector<synExp*> > ().push_back(yystack_[0].value.as < synExp* > ());
}
#line 738 "src/synExpParser.cpp"
    break;

  case 11:
#line 152 "../tools/src/synExpParser.ypp"
    {
}
#line 745 "src/synExpParser.cpp"
    break;

  case 12:
#line 155 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synCompound(synIfThenElse, yystack_[4].value.as < synExp* > (), yystack_[2].value.as < synExp* > (), yystack_[0].value.as < synExp* > ());
}
#line 753 "src/synExpParser.cpp"
    break;

  case 13:
#line 159 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synCompound(synIfThenElse, yystack_[2].value.as < synExp* > (), yystack_[0].value.as < synExp* > (), synTrue);
}
#line 761 "src/synExpParser.cpp"
    break;

  case 14:
#line 163 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = yystack_[1].value.as < synExp* > ();
}
#line 769 "src/synExpParser.cpp"
    break;

  case 15:
#line 167 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synCompound(synImplies, yystack_[2].value.as < synExp* > (), yystack_[0].value.as < synExp* > ());
}
#line 777 "src/synExpParser.cpp"
    break;

  case 16:
#line 171 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synCompound(synEqual, yystack_[2].value.as < synExp* > (), yystack_[0].value.as < synExp* > ());
}
#line 785 "src/synExpParser.cpp"
    break;

  case 17:
#line 175 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = synTrue;
}
#line 793 "src/synExpParser.cpp"
    break;

  case 18:
#line 179 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = synFalse;
}
#line 801 "src/synExpParser.cpp"
    break;

  case 19:
#line 183 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = synModule;
}
#line 809 "src/synExpParser.cpp"
    break;

  case 20:
#line 187 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synString(yystack_[0].value.as < std::string > ());
}
#line 817 "src/synExpParser.cpp"
    break;

  case 21:
#line 191 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = new synSymbol(yystack_[0].value.as < std::string > ());
}
#line 825 "src/synExpParser.cpp"
    break;

  case 22:
#line 195 "../tools/src/synExpParser.ypp"
    {
    yylhs.value.as < synExp* > () = synFalse;
}
#line 833 "src/synExpParser.cpp"
    break;


#line 837 "src/synExpParser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
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

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
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


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  synExpParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  synExpParser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char synExpParser::yypact_ninf_ = -15;

  const signed char synExpParser::yytable_ninf_ = -1;

  const signed char
  synExpParser::yypact_[] =
  {
      19,   -15,   -15,   -15,   -15,   -15,   -15,    73,    73,   -15,
     -13,    73,    37,    93,    -8,    87,    73,   -15,   -15,    99,
     -15,    73,    73,    73,    73,    73,   -15,    30,    55,   -15,
      30,   -14,    -1,   -15,    79,   -15,    30,    73,    30
  };

  const unsigned char
  synExpParser::yydefact_[] =
  {
       0,     4,    20,    21,    17,    18,    19,     0,     0,    22,
       0,     0,     0,     0,     0,     0,    11,     7,     1,     0,
       3,     0,     0,     0,     0,     0,    14,     9,     0,     2,
      15,     6,     5,    16,    13,     8,    10,     0,    12
  };

  const signed char
  synExpParser::yypgoto_[] =
  {
     -15,   -15,    -7,   -15
  };

  const signed char
  synExpParser::yydefgoto_[] =
  {
      -1,    12,    13,    28
  };

  const unsigned char
  synExpParser::yytable_[] =
  {
      14,    15,    16,    25,    17,    19,    23,    24,     0,    27,
      21,    22,    23,    24,    30,    31,    32,    33,    34,     1,
      24,    36,     0,     0,     2,     3,     4,     5,     6,     7,
      38,     0,     0,     0,     8,     9,    10,    18,     0,     0,
       0,    11,     2,     3,     4,     5,     6,     7,    21,    22,
      23,    24,     8,     9,    10,     0,     0,     0,     0,    11,
       2,     3,     4,     5,     6,     7,     0,     0,     0,    35,
       8,     9,    10,     0,     0,     0,     0,    11,     2,     3,
       4,     5,     6,     7,     0,     0,     0,     0,     8,     9,
      10,    37,     0,     0,     0,    11,     0,    21,    22,    23,
      24,    26,     0,     0,     0,    21,    22,    23,    24,    20,
       0,    21,    22,    23,    24,    29,     0,    21,    22,    23,
      24
  };

  const signed char
  synExpParser::yycheck_[] =
  {
       7,     8,    15,    11,    11,    12,    20,    21,    -1,    16,
      18,    19,    20,    21,    21,    22,    23,    24,    25,     0,
      21,    28,    -1,    -1,     5,     6,     7,     8,     9,    10,
      37,    -1,    -1,    -1,    15,    16,    17,     0,    -1,    -1,
      -1,    22,     5,     6,     7,     8,     9,    10,    18,    19,
      20,    21,    15,    16,    17,    -1,    -1,    -1,    -1,    22,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    22,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    -1,    15,    16,
      17,    12,    -1,    -1,    -1,    22,    -1,    18,    19,    20,
      21,    14,    -1,    -1,    -1,    18,    19,    20,    21,    16,
      -1,    18,    19,    20,    21,    16,    -1,    18,    19,    20,
      21
  };

  const unsigned char
  synExpParser::yystos_[] =
  {
       0,     0,     5,     6,     7,     8,     9,    10,    15,    16,
      17,    22,    24,    25,    25,    25,    15,    25,     0,    25,
      16,    18,    19,    20,    21,    11,    14,    25,    26,    16,
      25,    25,    25,    25,    25,    14,    25,    12,    25
  };

  const unsigned char
  synExpParser::yyr1_[] =
  {
       0,    23,    24,    24,    24,    25,    25,    25,    25,    26,
      26,    26,    25,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25
  };

  const unsigned char
  synExpParser::yyr2_[] =
  {
       0,     2,     3,     2,     1,     3,     3,     2,     4,     1,
       2,     0,     6,     4,     3,     3,     3,     1,     1,     1,
       1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const synExpParser::yytname_[] =
  {
  "T_EOF", "error", "$undefined", "T_MAINMENU", "T_MENU", "T_LITERAL",
  "T_SYMBOL", "T_TRUE", "T_FALSE", "T_MODULE", "T_IF", "T_THEN", "T_ELSE",
  "T_UNEQUAL", "T_CLOSEPAREN", "T_OPENPAREN", "T_EOL", "T_XOR",
  "T_IMPLIES", "T_OR", "T_AND", "T_EQUAL", "T_NOT", "$accept", "explist",
  "exp", "seqexp", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  synExpParser::yyrline_[] =
  {
       0,   116,   116,   120,   124,   127,   131,   135,   139,   143,
     147,   152,   155,   159,   163,   167,   171,   175,   179,   183,
     187,   191,   195
  };

  // Print the state stack on the debug stream.
  void
  synExpParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  synExpParser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 30 "../tools/src/synExpParser.ypp"
} // kconf
#line 1258 "src/synExpParser.cpp"

#line 198 "../tools/src/synExpParser.ypp"
 /*** Additional Code ***/
bool is_digits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}
void kconf::synExpParser::error(const synExpParser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}

