// A Bison parser, made by GNU Bison 3.4.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file src/parser.hpp
 ** Define the kconf::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

#ifndef YY_YY_SRC_PARSER_HPP_INCLUDED
# define YY_YY_SRC_PARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 491 "src/parser.ypp"

    #include "varInfo.hpp"
    #include "synExp.hpp"
    
    struct kconf_id;


#line 56 "src/parser.hpp"


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"

#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 465 "src/parser.ypp"
namespace kconf {
#line 172 "src/parser.hpp"




  /// A Bison parser.
  class Parser
  {
  public:
#ifndef YYSTYPE
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class semantic_type
  {
  public:
    /// Type of *this.
    typedef semantic_type self_type;

    /// Empty construction.
    semantic_type () YY_NOEXCEPT
      : yybuffer_ ()
    {}

    /// Construct and fill.
    template <typename T>
    semantic_type (YY_RVREF (T) t)
    {
      YYASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {}

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
    /// Prohibit blind copies.
    self_type& operator= (const self_type&);
    semantic_type (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // T_MAINMENU
      // T_MENU
      // T_ENDMENU
      // T_SOURCE
      // T_CHOICE
      // T_ENDCHOICE
      // T_COMMENT
      // T_CONFIG
      // T_MENUCONFIG
      // T_HELP
      // T_IF
      // T_ENDIF
      // T_DEPENDS
      // T_OPTIONAL
      // T_PROMPT
      // T_TYPE
      // T_DEFAULT
      // T_SELECT
      // T_RANGE
      // T_VISIBLE
      // T_OPTION
      // T_ON
      // T_OPT_DEFCONFIG_LIST
      // T_OPT_MODULES
      // T_OPT_ENV
      // T_INSERT
      char dummy1[sizeof (kconf_id*)];

      // T_HELPTEXT
      // T_WORD
      // T_WORD_QUOTE
      // symbol_option_arg
      // prompt
      // word_opt
      char dummy2[sizeof (std::string)];

      // depends_list
      // depends
      // visibility_list
      // visible
      // if_expr
      // expr
      // symbol
      char dummy3[sizeof (synExp*)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[size];
    } yybuffer_;
  };

#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        T_EOF = 0,
        T_MAINMENU = 258,
        T_MENU = 259,
        T_ENDMENU = 260,
        T_SOURCE = 261,
        T_CHOICE = 262,
        T_ENDCHOICE = 263,
        T_COMMENT = 264,
        T_CONFIG = 265,
        T_MENUCONFIG = 266,
        T_HELP = 267,
        T_IF = 268,
        T_ENDIF = 269,
        T_DEPENDS = 270,
        T_OPTIONAL = 271,
        T_PROMPT = 272,
        T_TYPE = 273,
        T_DEFAULT = 274,
        T_SELECT = 275,
        T_RANGE = 276,
        T_VISIBLE = 277,
        T_OPTION = 278,
        T_ON = 279,
        T_OPT_DEFCONFIG_LIST = 280,
        T_OPT_MODULES = 281,
        T_OPT_ENV = 282,
        T_INSERT = 283,
        T_HELPTEXT = 284,
        T_WORD = 285,
        T_WORD_QUOTE = 286,
        T_UNEQUAL = 287,
        T_CLOSE_PAREN = 288,
        T_OPEN_PAREN = 289,
        T_EOL = 290,
        T_EOP = 291,
        T_OR = 292,
        T_AND = 293,
        T_EQUAL = 294,
        T_NOT = 295
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that);
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, kconf_id*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const kconf_id*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, synExp*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const synExp*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
      {
        // User destructor.
        symbol_number_type yytype = this->type_get ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yytype)
        {
       default:
          break;
        }

        // Type destructor.
switch (yytype)
    {
      case 3: // T_MAINMENU
      case 4: // T_MENU
      case 5: // T_ENDMENU
      case 6: // T_SOURCE
      case 7: // T_CHOICE
      case 8: // T_ENDCHOICE
      case 9: // T_COMMENT
      case 10: // T_CONFIG
      case 11: // T_MENUCONFIG
      case 12: // T_HELP
      case 13: // T_IF
      case 14: // T_ENDIF
      case 15: // T_DEPENDS
      case 16: // T_OPTIONAL
      case 17: // T_PROMPT
      case 18: // T_TYPE
      case 19: // T_DEFAULT
      case 20: // T_SELECT
      case 21: // T_RANGE
      case 22: // T_VISIBLE
      case 23: // T_OPTION
      case 24: // T_ON
      case 25: // T_OPT_DEFCONFIG_LIST
      case 26: // T_OPT_MODULES
      case 27: // T_OPT_ENV
      case 28: // T_INSERT
        value.template destroy< kconf_id* > ();
        break;

      case 29: // T_HELPTEXT
      case 30: // T_WORD
      case 31: // T_WORD_QUOTE
      case 58: // symbol_option_arg
      case 86: // prompt
      case 92: // word_opt
        value.template destroy< std::string > ();
        break;

      case 81: // depends_list
      case 82: // depends
      case 83: // visibility_list
      case 84: // visible
      case 89: // if_expr
      case 90: // expr
      case 91: // symbol
        value.template destroy< synExp* > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_type (by_type&& that);
#endif

      /// Copy constructor.
      by_type (const by_type& that);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The token.
      token_type token () const YY_NOEXCEPT;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_type>
    {
      /// Superclass.
      typedef basic_symbol<by_type> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type(token_type (tok), std::move (l))
      {
        YYASSERT (tok == token::T_EOF || tok == token::T_UNEQUAL || tok == token::T_CLOSE_PAREN || tok == token::T_OPEN_PAREN || tok == token::T_EOL || tok == token::T_EOP || tok == token::T_OR || tok == token::T_AND || tok == token::T_EQUAL || tok == token::T_NOT);
      }
#else
      symbol_type (int tok, const location_type& l)
        : super_type(token_type (tok), l)
      {
        YYASSERT (tok == token::T_EOF || tok == token::T_UNEQUAL || tok == token::T_CLOSE_PAREN || tok == token::T_OPEN_PAREN || tok == token::T_EOL || tok == token::T_EOP || tok == token::T_OR || tok == token::T_AND || tok == token::T_EQUAL || tok == token::T_NOT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, kconf_id* v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YYASSERT (tok == token::T_MAINMENU || tok == token::T_MENU || tok == token::T_ENDMENU || tok == token::T_SOURCE || tok == token::T_CHOICE || tok == token::T_ENDCHOICE || tok == token::T_COMMENT || tok == token::T_CONFIG || tok == token::T_MENUCONFIG || tok == token::T_HELP || tok == token::T_IF || tok == token::T_ENDIF || tok == token::T_DEPENDS || tok == token::T_OPTIONAL || tok == token::T_PROMPT || tok == token::T_TYPE || tok == token::T_DEFAULT || tok == token::T_SELECT || tok == token::T_RANGE || tok == token::T_VISIBLE || tok == token::T_OPTION || tok == token::T_ON || tok == token::T_OPT_DEFCONFIG_LIST || tok == token::T_OPT_MODULES || tok == token::T_OPT_ENV || tok == token::T_INSERT);
      }
#else
      symbol_type (int tok, const kconf_id*& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YYASSERT (tok == token::T_MAINMENU || tok == token::T_MENU || tok == token::T_ENDMENU || tok == token::T_SOURCE || tok == token::T_CHOICE || tok == token::T_ENDCHOICE || tok == token::T_COMMENT || tok == token::T_CONFIG || tok == token::T_MENUCONFIG || tok == token::T_HELP || tok == token::T_IF || tok == token::T_ENDIF || tok == token::T_DEPENDS || tok == token::T_OPTIONAL || tok == token::T_PROMPT || tok == token::T_TYPE || tok == token::T_DEFAULT || tok == token::T_SELECT || tok == token::T_RANGE || tok == token::T_VISIBLE || tok == token::T_OPTION || tok == token::T_ON || tok == token::T_OPT_DEFCONFIG_LIST || tok == token::T_OPT_MODULES || tok == token::T_OPT_ENV || tok == token::T_INSERT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YYASSERT (tok == token::T_HELPTEXT || tok == token::T_WORD || tok == token::T_WORD_QUOTE);
      }
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YYASSERT (tok == token::T_HELPTEXT || tok == token::T_WORD || tok == token::T_WORD_QUOTE);
      }
#endif
    };

    /// Build a parser object.
    Parser (class Driver& driver_yyarg);
    virtual ~Parser ();

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    // Implementation of make_symbol for each symbol type.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_EOF (location_type l)
      {
        return symbol_type (token::T_EOF, std::move (l));
      }
#else
      static
      symbol_type
      make_T_EOF (const location_type& l)
      {
        return symbol_type (token::T_EOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_MAINMENU (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_MAINMENU, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_MAINMENU (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_MAINMENU, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_MENU (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_MENU, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_MENU (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_MENU, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ENDMENU (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_ENDMENU, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_ENDMENU (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_ENDMENU, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SOURCE (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_SOURCE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_SOURCE (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_SOURCE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CHOICE (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_CHOICE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_CHOICE (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_CHOICE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ENDCHOICE (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_ENDCHOICE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_ENDCHOICE (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_ENDCHOICE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_COMMENT (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_COMMENT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_COMMENT (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_COMMENT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CONFIG (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_CONFIG, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_CONFIG (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_CONFIG, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_MENUCONFIG (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_MENUCONFIG, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_MENUCONFIG (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_MENUCONFIG, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_HELP (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_HELP, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_HELP (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_HELP, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_IF (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_IF, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_IF (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_IF, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ENDIF (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_ENDIF, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_ENDIF (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_ENDIF, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_DEPENDS (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_DEPENDS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_DEPENDS (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_DEPENDS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OPTIONAL (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_OPTIONAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_OPTIONAL (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_OPTIONAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_PROMPT (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_PROMPT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_PROMPT (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_PROMPT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_TYPE (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_TYPE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_TYPE (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_TYPE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_DEFAULT (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_DEFAULT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_DEFAULT (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_DEFAULT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_SELECT (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_SELECT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_SELECT (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_SELECT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_RANGE (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_RANGE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_RANGE (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_RANGE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_VISIBLE (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_VISIBLE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_VISIBLE (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_VISIBLE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OPTION (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_OPTION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_OPTION (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_OPTION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_ON (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_ON, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_ON (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_ON, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OPT_DEFCONFIG_LIST (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_OPT_DEFCONFIG_LIST, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_OPT_DEFCONFIG_LIST (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_OPT_DEFCONFIG_LIST, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OPT_MODULES (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_OPT_MODULES, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_OPT_MODULES (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_OPT_MODULES, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OPT_ENV (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_OPT_ENV, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_OPT_ENV (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_OPT_ENV, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_INSERT (kconf_id* v, location_type l)
      {
        return symbol_type (token::T_INSERT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_INSERT (const kconf_id*& v, const location_type& l)
      {
        return symbol_type (token::T_INSERT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_HELPTEXT (std::string v, location_type l)
      {
        return symbol_type (token::T_HELPTEXT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_HELPTEXT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_HELPTEXT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_WORD (std::string v, location_type l)
      {
        return symbol_type (token::T_WORD, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_WORD (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_WORD, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_WORD_QUOTE (std::string v, location_type l)
      {
        return symbol_type (token::T_WORD_QUOTE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_T_WORD_QUOTE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::T_WORD_QUOTE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_UNEQUAL (location_type l)
      {
        return symbol_type (token::T_UNEQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_T_UNEQUAL (const location_type& l)
      {
        return symbol_type (token::T_UNEQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_CLOSE_PAREN (location_type l)
      {
        return symbol_type (token::T_CLOSE_PAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_CLOSE_PAREN (const location_type& l)
      {
        return symbol_type (token::T_CLOSE_PAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OPEN_PAREN (location_type l)
      {
        return symbol_type (token::T_OPEN_PAREN, std::move (l));
      }
#else
      static
      symbol_type
      make_T_OPEN_PAREN (const location_type& l)
      {
        return symbol_type (token::T_OPEN_PAREN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_EOL (location_type l)
      {
        return symbol_type (token::T_EOL, std::move (l));
      }
#else
      static
      symbol_type
      make_T_EOL (const location_type& l)
      {
        return symbol_type (token::T_EOL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_EOP (location_type l)
      {
        return symbol_type (token::T_EOP, std::move (l));
      }
#else
      static
      symbol_type
      make_T_EOP (const location_type& l)
      {
        return symbol_type (token::T_EOP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_OR (location_type l)
      {
        return symbol_type (token::T_OR, std::move (l));
      }
#else
      static
      symbol_type
      make_T_OR (const location_type& l)
      {
        return symbol_type (token::T_OR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_AND (location_type l)
      {
        return symbol_type (token::T_AND, std::move (l));
      }
#else
      static
      symbol_type
      make_T_AND (const location_type& l)
      {
        return symbol_type (token::T_AND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_EQUAL (location_type l)
      {
        return symbol_type (token::T_EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_T_EQUAL (const location_type& l)
      {
        return symbol_type (token::T_EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_NOT (location_type l)
      {
        return symbol_type (token::T_NOT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_NOT (const location_type& l)
      {
        return symbol_type (token::T_NOT, l);
      }
#endif


  private:
    /// This class is not copyable.
    Parser (const Parser&);
    Parser& operator= (const Parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const short yytable_[];

  static const short yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::reverse_iterator iterator;
      typedef typename S::const_reverse_iterator const_iterator;
      typedef typename S::size_type size_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (size_type i)
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (int i)
      {
        return operator[] (size_type (i));
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (size_type i) const
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (int i) const
      {
        return operator[] (size_type (i));
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (int n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      size_type
      size () const YY_NOEXCEPT
      {
        return seq_.size ();
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.rbegin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.rend ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, int range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (int i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        int range_;
      };

    private:
      stack (const stack&);
      stack& operator= (const stack&);
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 325,     ///< Last index in yytable_.
      yynnts_ = 52,  ///< Number of nonterminal symbols.
      yyfinal_ = 11, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 41  ///< Number of tokens.
    };


    // User arguments.
    class Driver& driver;
  };

  inline
  Parser::token_number_type
  Parser::yytranslate_ (token_type t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
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
      35,    36,    37,    38,    39,    40
    };
    const unsigned user_token_number_max_ = 295;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
    , location (std::move (that.location))
  {
    switch (this->type_get ())
    {
      case 3: // T_MAINMENU
      case 4: // T_MENU
      case 5: // T_ENDMENU
      case 6: // T_SOURCE
      case 7: // T_CHOICE
      case 8: // T_ENDCHOICE
      case 9: // T_COMMENT
      case 10: // T_CONFIG
      case 11: // T_MENUCONFIG
      case 12: // T_HELP
      case 13: // T_IF
      case 14: // T_ENDIF
      case 15: // T_DEPENDS
      case 16: // T_OPTIONAL
      case 17: // T_PROMPT
      case 18: // T_TYPE
      case 19: // T_DEFAULT
      case 20: // T_SELECT
      case 21: // T_RANGE
      case 22: // T_VISIBLE
      case 23: // T_OPTION
      case 24: // T_ON
      case 25: // T_OPT_DEFCONFIG_LIST
      case 26: // T_OPT_MODULES
      case 27: // T_OPT_ENV
      case 28: // T_INSERT
        value.move< kconf_id* > (std::move (that.value));
        break;

      case 29: // T_HELPTEXT
      case 30: // T_WORD
      case 31: // T_WORD_QUOTE
      case 58: // symbol_option_arg
      case 86: // prompt
      case 92: // word_opt
        value.move< std::string > (std::move (that.value));
        break;

      case 81: // depends_list
      case 82: // depends
      case 83: // visibility_list
      case 84: // visible
      case 89: // if_expr
      case 90: // expr
      case 91: // symbol
        value.move< synExp* > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->type_get ())
    {
      case 3: // T_MAINMENU
      case 4: // T_MENU
      case 5: // T_ENDMENU
      case 6: // T_SOURCE
      case 7: // T_CHOICE
      case 8: // T_ENDCHOICE
      case 9: // T_COMMENT
      case 10: // T_CONFIG
      case 11: // T_MENUCONFIG
      case 12: // T_HELP
      case 13: // T_IF
      case 14: // T_ENDIF
      case 15: // T_DEPENDS
      case 16: // T_OPTIONAL
      case 17: // T_PROMPT
      case 18: // T_TYPE
      case 19: // T_DEFAULT
      case 20: // T_SELECT
      case 21: // T_RANGE
      case 22: // T_VISIBLE
      case 23: // T_OPTION
      case 24: // T_ON
      case 25: // T_OPT_DEFCONFIG_LIST
      case 26: // T_OPT_MODULES
      case 27: // T_OPT_ENV
      case 28: // T_INSERT
        value.copy< kconf_id* > (YY_MOVE (that.value));
        break;

      case 29: // T_HELPTEXT
      case 30: // T_WORD
      case 31: // T_WORD_QUOTE
      case 58: // symbol_option_arg
      case 86: // prompt
      case 92: // word_opt
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case 81: // depends_list
      case 82: // depends
      case 83: // visibility_list
      case 84: // visible
      case 89: // if_expr
      case 90: // expr
      case 91: // symbol
        value.copy< synExp* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 3: // T_MAINMENU
      case 4: // T_MENU
      case 5: // T_ENDMENU
      case 6: // T_SOURCE
      case 7: // T_CHOICE
      case 8: // T_ENDCHOICE
      case 9: // T_COMMENT
      case 10: // T_CONFIG
      case 11: // T_MENUCONFIG
      case 12: // T_HELP
      case 13: // T_IF
      case 14: // T_ENDIF
      case 15: // T_DEPENDS
      case 16: // T_OPTIONAL
      case 17: // T_PROMPT
      case 18: // T_TYPE
      case 19: // T_DEFAULT
      case 20: // T_SELECT
      case 21: // T_RANGE
      case 22: // T_VISIBLE
      case 23: // T_OPTION
      case 24: // T_ON
      case 25: // T_OPT_DEFCONFIG_LIST
      case 26: // T_OPT_MODULES
      case 27: // T_OPT_ENV
      case 28: // T_INSERT
        value.move< kconf_id* > (YY_MOVE (s.value));
        break;

      case 29: // T_HELPTEXT
      case 30: // T_WORD
      case 31: // T_WORD_QUOTE
      case 58: // symbol_option_arg
      case 86: // prompt
      case 92: // word_opt
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case 81: // depends_list
      case 82: // depends
      case 83: // visibility_list
      case 84: // visible
      case 89: // if_expr
      case 90: // expr
      case 91: // symbol
        value.move< synExp* > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_type.
  inline
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  Parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  inline
  Parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  Parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }

  inline
  Parser::token_type
  Parser::by_type::token () const YY_NOEXCEPT
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
    };
    return token_type (yytoken_number_[type]);
  }

#line 465 "src/parser.ypp"
} // kconf
#line 2017 "src/parser.hpp"





#endif // !YY_YY_SRC_PARSER_HPP_INCLUDED
