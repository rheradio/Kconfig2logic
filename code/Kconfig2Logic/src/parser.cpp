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
#line 1 "src/parser.ypp"
 /*** C/C++ Declarations ***/

#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <sstream>

#include "varInfo.hpp"
#include "configInfo.hpp"
#include "choiceInfo.hpp"
#include "ifInfo.hpp"
#include "symbolTable.hpp"
#include "driver.hpp"

std::vector<synExp*>      depStack;
std::vector<varInfo*>     configStack;
std::vector<configInfo*>  menuStack;
std::vector<choiceInfo*>  choiceStack;
std::set<std::string>     repeats;
int                       depAdded;
configInfo *coninf = NULL;
choiceInfo *choinf = NULL;

int choiceNameCounter = 0;

void addDepStack(synExp* s) {
    if (s != synTrue)
    depStack.push_back(s);
    if (s->isSymbol()) {
        std::string sym = s->getSymbol();
    }
}

void delDepStack(varInfo* v) {
    int l = v->getDepLength();
    if (l > depStack.size())
    throw std::logic_error("Error managing dependency stack");
    else {
        int dif = depStack.size() - l;
        for(int x = 0; x < dif ; x++)
        depStack.pop_back();
    }
}

void handle_config_start(std::string theConf, kconf::Driver& driver) {
    synExp* dep = synTrue;
    synExp* vis = synTrue;
    if (! configStack.empty()) {
        dep = configStack.back()->getDependsVar();
        vis = configStack.back()->getVisibility();
    }
    coninf = new configInfo();
    coninf->setDepLength(depStack.size());
    coninf->setHasConfig();
    // If there is already a value... we keep it
    // and if there is a type... same thing
    configInfo *c = symbolTable::getSymbol(theConf);
    coninf->setName(theConf);
    if (c != NULL) {
        coninf->setValue(c->getValue());
    }

    // If the config is inside a choice, visibility is not inherited
    // Likewise, dependencies are also not inherited
    
    // We use this info in case the depended config is in a choice to
    // break the group of the depended if necessary
    for(std::string var : dep->giveSymbols()) {
        configInfo *c = symbolTable::getSymbol(var);
        if (c != NULL)
            c->addIsDependedBy(theConf);
        coninf->addDependsOn(var);
    }
    
    if (choiceStack.empty()) {
        coninf->addVisibility(vis->copy_exp());
        coninf->addDepends(dep->copy_exp());
    }
    else {
        coninf->setChoiceMember();
    }
    
    // If the type is unknown we check the symbol table
    // to see if it has been declared with a type before
    // and take it from there
    if (coninf->getVartype() == "") {
        configInfo *ci = symbolTable::getSymbol(theConf);
        if (ci != NULL) {
            coninf->setVartype(ci->getVartype());
        }
    }
    
    if (!menuStack.empty())
        coninf->setMenuName(menuStack.back()->getPrompText());
    
    coninf->setFile(driver.lexer->curfile());
    configStack.push_back(coninf);
}

void handle_config_stmt(kconf::Driver& driver) {   // 1
    coninf->setDependStack(depStack);
    if (!choiceStack.empty()) {
        // Berger says choices are boolean by default.
        // It is not true: linux/drivers/usb/musb/Kconfig
        // There's a choice with no type and all the members
        // are tristate
        
        driver.increaseChoiceMembers();
        // Type compatibility check 1
        if (choiceStack.back()->getVartype() == "boolean" &&
        coninf->getVartype() == "tristate") {
            std::cerr << "The choice is boolean and the member is tristate" << std::endl;
            std::cerr << "The choice:" << std::endl
            << choiceStack.back() << std::endl;
            std::cerr << "The member:" << configStack.back()
            << std::endl;
            exit(-1);
        }
        // If the type is not specified, we inherit it from the choice
        if (coninf->getVartype() == "")
            coninf->setVartype(choiceStack.back()->getVartype());
        
        if (choiceStack.back()->getVartype() == "")
            choiceStack.back()->setVartype("boolean");
        
        //If the choice member depends on the previous choice member...
        // it is not really a choice member so we don't push it as such
        
        //Also, it the choice member does not have a prompt, it is also
        // not a freaking choice member
        if (choiceStack.back()->checkDeps(coninf->getDependsVar()->giveSymbols(), coninf->getName()) && coninf->isPrompt())
            choiceStack.back()->push_symbol(coninf);
        else {
            // Get your attributes back...
            coninf->addVisibility(choiceStack.back()->getVisibility());
            coninf->addDepends(choiceStack.back()->getDependsVar());
        }
        
        // If the choice type is not specified, we take it from THE members:
        // If all boolean, boolean then, if any of them is tristate, then tristate it is
        if (choiceStack.back()->getVartype() == "boolean" &&
        configStack.back()->getVartype() == "tristate")
        choiceStack.back()->setVartype("tristate");
    }
    
    if (coninf->getVartype() == "boolean"
    || coninf->getVartype() == "tristate")
    if (repeats.find(coninf->getName()) == repeats.end()) {
        repeats.insert(coninf->getName());
        for(configInfo* c : menuStack) {
            c->addToMenu(coninf->getName());
        }
        for(choiceInfo* c : choiceStack) {
            c->addToMenu(coninf->getName());
        }
        driver.addToDirFile(coninf);
    }
    
    // configs taken from the environment are added
    // to the symbol table,
    // They HAVE to be translated, because the may be
    // of type string and at the least, the variants
    // need to be exclusive
    driver.increaseConfigs();
    symbolTable::declareSymbol(coninf->getName(), coninf);
    
    // We do not want to include them twice
    if (choiceStack.empty())
        driver.add(coninf);
        
    configInfo::addAlternative(coninf->getName(), coninf);
    // We count how many times a config name depends on each symbol
    // to use it to simplify dependencies later
    for (auto s : coninf->getDependsVar()->giveSymbols())
        configInfo::timesDependsOn[std::pair<std::string, std::string>(coninf->getName(), s)]++;
        
    delDepStack(coninf);
    configStack.pop_back();
    configInfo::useXOR = driver.useXOR;


}

synExp* handle_equality(synExp* dollar1, synExp* dollar3) {
    synExp *alt1 = NULL;
    synExp *alt3 = NULL;
    if (dollar1->get_type() == synExp_String) {
        if (dollar1->get_string() == "y") alt1 = synTrue;
        if (dollar1->get_string() == "m") alt1 = synModule;
        if (dollar1->get_string() == "n") alt1 = synFalse;
    }
    if (dollar3->get_type() == synExp_String) {
        if (dollar3->get_string() == "y") alt3 = synTrue;
        if (dollar3->get_string() == "m") alt3 = synModule;
        if (dollar3->get_string() == "n") alt3 = synFalse;
    }
    // If it's two string literals (or env variable) we just decide here
    // [Right now only in use to check $ARCH]
    if (dollar1->get_type() == synExp_String && dollar3->get_type() == synExp_String) {
    if (dollar1->get_string() == dollar3->get_string())
       return synTrue;
    else
       return synFalse;
    }
    
    // Equality between different types
    if (dollar1->get_type() == synExp_Symbol && alt3 != NULL)
       return makeEqual(dollar1, alt3);
    else
    if (dollar3->get_type() == synExp_Symbol && alt1 != NULL)
       return  makeEqual(alt1, dollar3);
    
    // A string symbol compared to a value
    std::string theString;
    synExp*  theSymbol = NULL;
    bool found = false;
    if (dollar1->get_type() == synExp_Symbol &&
        dollar3->get_type() == synExp_String) {
        theSymbol = dollar1;
        theString = dollar3->get_string();
        found     = true;
    }
    else
    if (dollar1->get_type() == synExp_String &&
        dollar3->get_type() == synExp_Symbol) {
        theString = dollar1->get_string();
        theSymbol = dollar3;
        found     = true;
    }
    if (found) {
        configInfo *v = symbolTable::getSymbol(theSymbol->getSymbol());
        if (v == NULL) {
            configInfo *c = new configInfo();
            c->setName(theSymbol->getSymbol());
            symbolTable::addSymbol(theSymbol->getSymbol(), c);
            v = symbolTable::getSymbol(theSymbol->getSymbol());
        }
        
        v->addValue(theString);
        return new synSymbol(v->buildNameValue(theString));
    }
    found = false;
    synExp *theUnknown = NULL;
    // One unknown and one symbol
    if (dollar1->isSymbol() &&
        dollar3->isUnknown()) {
            theSymbol  = dollar1;
            theUnknown = dollar3;
            found = true;
    }
    else
    if (dollar3->isSymbol() &&
        dollar1->isUnknown()) {
            theSymbol  = dollar3;
            theUnknown = dollar1;
            found = true;
    }
    if (found) {
        synExp* theKnown = NULL;
        configInfo *v = symbolTable::getSymbol(theSymbol->getSymbol());
        std::string type = v->getVartype();
        if (type == "string" || type == "int" || type == "hex") {
            theKnown = new synString(theUnknown->get_string());
            theUnknown->destroy();
            v->addValue(theKnown->get_string());
            return new synSymbol(v->buildNameValue(theKnown->get_string()));
        }
        else
        if (type == "boolean" || type == "tristate") {
            // It was a symbol with no definition
            theKnown = synFalse;
            theUnknown->destroy();
            return synFalse;
        }
        else {
            std::logic_error("Unknown type "+type+" for symbol "+theSymbol->getSymbol());
        }
    }
    // Two symbols of type string checked for equality
    if (dollar1->get_type() == synExp_Symbol &&
    dollar3->get_type() == synExp_Symbol) {
        varInfo *v1 =
        symbolTable::getSymbol(dollar1->get_string());
        varInfo *v3 =
        symbolTable::getSymbol(dollar3->get_string());
        if (v1 != NULL && v3 != NULL)
        if (v1->getVartype() == "string" && v3->getVartype() == "string")
           throw std::logic_error("Equality between string symbols not implemented");
    }
    return makeEqual(dollar1->copy_exp(), dollar3->copy_exp());
}

void handle_default(kconf::Driver& driver, kconf_id* def, synExp* exp, synExp *ifExp) {
    // For def_tristate and def_bool (type and default at the same time)
    if (exp->isUnknown()) {
        if (coninf->getVartype() == "string" ||
            coninf->getVartype() == "int"    ||
            coninf->getVartype() == "hex") {
            std::string st = exp->get_string();
            exp->destroy();
            exp = new synString(st);
        }
        else
        if (coninf->getVartype() == "boolean" || coninf->getVartype() == "tristate") {
                exp->destroy();
                exp = synFalse;
        }
        else throw std::logic_error("Unknown type *"+coninf->getVartype()+"* for symbol "+coninf->getName());
    }
     // Sometimes, there is a string with a default "Whatever" and no explicit type given
     if (coninf->getVartype() == "" && exp->get_type() == synExp_String)
         coninf->setVartype("string");
         
     if (def->type2string() != "unknown") {
         coninf->setVartype(def->type2string());
         driver.increaseCounters(def->type2string());
     }
     // We stack them up in reverse order to simplify later processing
     if (coninf->getVartype() == "boolean" || coninf->getVartype() == "tristate") {
         if (exp->get_type() == synExp_String) {
             if (exp->get_string() == "y")
                 coninf->push_default(ifExp, synTrue);
             else
             if (exp->get_string() == "n")
                 coninf->push_default(ifExp, synFalse);
             else
             if (exp->get_string() == "m")
                 coninf->push_default(ifExp, synModule);
                 else {
                     std::cerr << "Default string value " << exp->get_string() << " for logic config" << std::endl;
                     throw(std::logic_error("Error"));
                 }
         
         }
         else {
             coninf->push_default(ifExp, exp);
         }
     }
     else {
         coninf->push_default(ifExp, exp);
     }
}

void handle_choice(kconf::Driver& driver, std::string cho) {
    synExp* dep = synTrue;
       synExp* vis = synTrue;
       if (! configStack.empty()) {
           dep = configStack.back()->getDependsVar();
           vis = configStack.back()->getVisibility();
       }
       
       choinf = new choiceInfo();
       choinf->setDepLength(depStack.size());
       choinf->addDepends(dep->copy_exp());
       choinf->addVisibility(vis);
       

       std::ostringstream ost;
       ost << cho << "choice_" << choiceNameCounter++;
       choinf->setName(ost.str());
       choiceInfo::useXOR = driver.useXOR;
       choinf->setFile(driver.lexer->curfile());
       choiceStack.push_back(choinf);
       configStack.push_back(choinf);
       // timesDependsOn should be in varInfo, not in configInfo
       // because choices also need to simplify their dependencies
       for (auto s : choinf->getDependsVar()->giveSymbols())
           configInfo::timesDependsOn[std::pair<std::string, std::string>(choinf->getName(), s)]++;
}

void handle_source(kconf::Driver& driver, std::string filename) {
    driver.streamname = filename;
       // The file name may contain a SYMBOL like "$BR2_EXTERNAL/Config.in"
       std::string symbolName;
       if (driver.streamname.find('$') != std::string::npos) {
           std::string old    = driver.streamname;
           std::size_t dollar = driver.streamname.find('$');
           std::size_t bar    = driver.streamname.rfind('/');
           if (dollar !=  std::string::npos) {
               if (bar != 0)
               symbolName =
               std::string(driver.streamname, dollar+1, bar - dollar-1);
               else
               symbolName =
               std::string(driver.streamname, dollar+1, driver.streamname.length() - dollar-1);
           }
           configInfo *ss = symbolTable::getSymbol(symbolName);
           if (ss == NULL)
             std::cerr << "1 Symbol " << symbolName << " not found in symbol table" << std::endl;

           std::string start;
           if (dollar > 0)
                start  = std::string(driver.streamname,0, dollar -1);
           std::string middle = ss->getValue()->get_string();
           std::string end    =  std::string(driver.streamname, bar, driver.streamname.length() - bar);
           filename = start+middle+end;
       }

       //std::cerr << "Calling checkNextfiles " << filename << std::endl;
       int nfiles = driver.lexer->checkNextFiles(filename);
       driver.increaseFiles(nfiles);
}

synExp* handle_symbol(kconf::Driver& driver, std::string& sym) {
    if (sym == "n" || sym == "N") return synFalse;
    if (sym == "y" || sym == "Y") return synTrue;
    if (sym == "m" || sym == "M") return synModule;
    
    if (driver.configList.find(sym) != driver.configList.end()) {
        return new synSymbol(sym);
    }
    //std::cerr << "Symbol " << sym << " unknown" << std::endl;
    return new synUnknown(sym);
}

void handle_ifblock(kconf::Driver& driver, synExp* dollar2) {
    if (dollar2->isUnknown()) {
        std::string st = dollar2->get_string();
        dollar2->destroy();
        dollar2 = synFalse;
    }
    synExp* dep = synTrue;
    synExp* vis = synTrue;
    if (! configStack.empty()) {
        dep = configStack.back()->getDependsVar();
        vis = configStack.back()->getVisibility();
    }
    ifInfo* ifinf = new ifInfo();
    ifinf->setDepLength(depStack.size());
    ifinf->addDepends(dep->copy_exp());
    ifinf->addDepends(dollar2->copy_exp());
    
    addDepStack(dollar2);
    // 4
    ifinf->setDependStack(depStack);
    
    ifinf->addVisibility(vis->copy_exp());
    ifinf->setGuard(dollar2);
    configStack.push_back(ifinf);
    driver.increaseIfBlocks();
}

#line 549 "src/parser.ypp"


#include "driver.hpp"
#include "scanner.hpp"
#include "varInfo.hpp"


/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */

#undef yylex
#define yylex driver.lexer->lex


#line 501 "src/parser.cpp"


#include "parser.hpp"




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

#line 465 "src/parser.ypp"
namespace kconf {
#line 598 "src/parser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
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
  Parser::Parser (class Driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_number_type
  Parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
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
        value.YY_MOVE_OR_COPY< kconf_id* > (YY_MOVE (that.value));
        break;

      case 29: // T_HELPTEXT
      case 30: // T_WORD
      case 31: // T_WORD_QUOTE
      case 58: // symbol_option_arg
      case 86: // prompt
      case 92: // word_opt
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 81: // depends_list
      case 82: // depends
      case 83: // visibility_list
      case 84: // visible
      case 89: // if_expr
      case 90: // expr
      case 91: // symbol
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

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
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
        value.move< kconf_id* > (YY_MOVE (that.value));
        break;

      case 29: // T_HELPTEXT
      case 30: // T_WORD
      case 31: // T_WORD_QUOTE
      case 58: // symbol_option_arg
      case 86: // prompt
      case 92: // word_opt
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 81: // depends_list
      case 82: // depends
      case 83: // visibility_list
      case 84: // visible
      case 89: // if_expr
      case 90: // expr
      case 91: // symbol
        value.move< synExp* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
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
        value.move< kconf_id* > (that.value);
        break;

      case 29: // T_HELPTEXT
      case 30: // T_WORD
      case 31: // T_WORD_QUOTE
      case 58: // symbol_option_arg
      case 86: // prompt
      case 92: // word_opt
        value.move< std::string > (that.value);
        break;

      case 81: // depends_list
      case 82: // depends
      case 83: // visibility_list
      case 84: // visible
      case 89: // if_expr
      case 90: // expr
      case 91: // symbol
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
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
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
      case 29: // T_HELPTEXT
#line 547 "src/parser.ypp"
        { yyoutput << yysym.value.template as < std::string > (); }
#line 923 "src/parser.cpp"
        break;

      case 30: // T_WORD
#line 547 "src/parser.ypp"
        { yyoutput << yysym.value.template as < std::string > (); }
#line 929 "src/parser.cpp"
        break;

      case 31: // T_WORD_QUOTE
#line 547 "src/parser.ypp"
        { yyoutput << yysym.value.template as < std::string > (); }
#line 935 "src/parser.cpp"
        break;

      case 58: // symbol_option_arg
#line 547 "src/parser.ypp"
        { yyoutput << yysym.value.template as < std::string > (); }
#line 941 "src/parser.cpp"
        break;

      case 86: // prompt
#line 547 "src/parser.ypp"
        { yyoutput << yysym.value.template as < std::string > (); }
#line 947 "src/parser.cpp"
        break;

      case 92: // word_opt
#line 547 "src/parser.ypp"
        { yyoutput << yysym.value.template as < std::string > (); }
#line 953 "src/parser.cpp"
        break;

      default:
        break;
    }
    yyo << ')';
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
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
#line 477 "src/parser.ypp"
{
    // initialize the initial location object
    yyla.location.begin.filename = yyla.location.end.filename = &driver.streamname;
}

#line 1078 "src/parser.cpp"


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
        yylhs.value.emplace< kconf_id* > ();
        break;

      case 29: // T_HELPTEXT
      case 30: // T_WORD
      case 31: // T_WORD_QUOTE
      case 58: // symbol_option_arg
      case 86: // prompt
      case 92: // word_opt
        yylhs.value.emplace< std::string > ();
        break;

      case 81: // depends_list
      case 82: // depends
      case 83: // visibility_list
      case 84: // visible
      case 89: // if_expr
      case 90: // expr
      case 91: // symbol
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
#line 567 "src/parser.ypp"
    {
}
#line 1252 "src/parser.cpp"
    break;

  case 3:
#line 569 "src/parser.ypp"
    {
}
#line 1259 "src/parser.cpp"
    break;

  case 4:
#line 572 "src/parser.ypp"
    {
    driver.firstPass = false;
}
#line 1267 "src/parser.cpp"
    break;

  case 5:
#line 575 "src/parser.ypp"
    {
}
#line 1274 "src/parser.cpp"
    break;

  case 6:
#line 577 "src/parser.ypp"
    {
}
#line 1281 "src/parser.cpp"
    break;

  case 11:
#line 585 "src/parser.ypp"
    {  }
#line 1287 "src/parser.cpp"
    break;

  case 12:
#line 586 "src/parser.ypp"
    { }
#line 1293 "src/parser.cpp"
    break;

  case 13:
#line 588 "src/parser.ypp"
    {
	
}
#line 1301 "src/parser.cpp"
    break;

  case 14:
#line 591 "src/parser.ypp"
    { }
#line 1307 "src/parser.cpp"
    break;

  case 30:
#line 609 "src/parser.ypp"
    {  }
#line 1313 "src/parser.cpp"
    break;

  case 31:
#line 610 "src/parser.ypp"
    {  }
#line 1319 "src/parser.cpp"
    break;

  case 32:
#line 616 "src/parser.ypp"
    {// Check
    if (!driver.firstPass)
        handle_config_start(yystack_[1].value.as < std::string > (), driver);
    else {
          driver.configList.insert(yystack_[1].value.as < std::string > ());
        }
}
#line 1331 "src/parser.cpp"
    break;

  case 33:
#line 624 "src/parser.ypp"
    {// Check
    if (!driver.firstPass)
        handle_config_stmt(driver);
}
#line 1340 "src/parser.cpp"
    break;

  case 35:
#line 631 "src/parser.ypp"
    {// Check
    if (!driver.firstPass)
        handle_config_start(yystack_[1].value.as < std::string > (), driver);
    else {
        driver.configList.insert(yystack_[1].value.as < std::string > ());
    }
}
#line 1352 "src/parser.cpp"
    break;

  case 36:
#line 639 "src/parser.ypp"
    {// Check
    if (!driver.firstPass)
        handle_config_stmt(driver);
}
#line 1361 "src/parser.cpp"
    break;

  case 40:
#line 649 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        // Not here because this only affects the config
        //addDepStack($2);
        coninf->addInnerDep(yystack_[0].value.as < synExp* > ()->copy_exp());
        coninf->addDepends(yystack_[0].value.as < synExp* > ()->copy_exp());
    }
}
#line 1374 "src/parser.cpp"
    break;

  case 44:
#line 663 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        std::string st = yystack_[2].value.as < kconf_id* > ()->type2string();
        coninf->setVartype(st);
        driver.increaseCounters(st);
    }
}
#line 1386 "src/parser.cpp"
    break;

  case 45:
#line 672 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        coninf->activatePrompt();
        coninf->setPrompText(yystack_[2].value.as < std::string > ());
        coninf->setPromptGuard(yystack_[1].value.as < synExp* > ());
    }
}
#line 1398 "src/parser.cpp"
    break;

  case 46:
#line 681 "src/parser.ypp"
    {
    if (!driver.firstPass)
        handle_default(driver, yystack_[3].value.as < kconf_id* > (), yystack_[2].value.as < synExp* > (), yystack_[1].value.as < synExp* > ());
}
#line 1407 "src/parser.cpp"
    break;

  case 47:
#line 687 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        // Dependencies alone or dependencies and visibility condition?
        coninf->addMapSelect(yystack_[2].value.as < std::string > (), yystack_[1].value.as < synExp* > (), coninf->getDependsVar()->copy_exp());
        driver.addToSelectable(yystack_[2].value.as < std::string > ());
        coninf->setSelects();
        for(configInfo* c : menuStack)
            c->setSelects();
    }
}
#line 1422 "src/parser.cpp"
    break;

  case 48:
#line 698 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        synExp *dollar2 = yystack_[3].value.as < synExp* > ();
        synExp *dollar3 = yystack_[2].value.as < synExp* > ();
        if (dollar2->isUnknown()) {
            std::string st = dollar2->get_string();
            dollar2->destroy();
            dollar2 = new synString(st);
        }
        if (dollar3->isUnknown()) {
            std::string st = dollar3->get_string();
            dollar3->destroy();
            dollar3 = new synString(st);
        }
        coninf->addRange(dollar2, dollar3);
    }
}
#line 1444 "src/parser.cpp"
    break;

  case 51:
#line 723 "src/parser.ypp"
    {
    // We do nothing, this has to be dealt with when calling Kconfig2Logic
    // with the syntax key=value, so the value will get written in the
    // symbol table for key.

    if (!driver.firstPass) {
        coninf->setFromEnvironment();
        configInfo *c = symbolTable::getSymbol(coninf->getName());
        
        // If the key=value was not used we define the symbol
        // to avoid problems later on
        if (c == NULL) {
            if (coninf->getVartype() == "string")
                coninf->setValue(new synString(""));
            else
                if (coninf->getVartype() == "int")
                    coninf->setValue(new synString("0"));
                    else
                    if (coninf->getVartype() == "hex")
                        coninf->setValue(new synString("0x0"));
                        
            symbolTable::addSymbol(coninf->getName(), coninf);
        }
    }
}
#line 1474 "src/parser.cpp"
    break;

  case 52:
#line 750 "src/parser.ypp"
    {  }
#line 1480 "src/parser.cpp"
    break;

  case 53:
#line 751 "src/parser.ypp"
    { if (!driver.firstPass) {yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();}  }
#line 1486 "src/parser.cpp"
    break;

  case 54:
#line 757 "src/parser.ypp"
    {
    if (!driver.firstPass)
        handle_choice(driver, yystack_[1].value.as < std::string > ());

}
#line 1496 "src/parser.cpp"
    break;

  case 55:
#line 764 "src/parser.ypp"
    {
    if (!driver.firstPass)
        choinf->setDependStack(depStack);
}
#line 1505 "src/parser.cpp"
    break;

  case 57:
#line 772 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        driver.increaseChoices();
        driver.addConstraint(choiceStack.back());
        if (choiceStack.back()->getName() == "")
        choiceStack.back()->setName(choiceStack.back()->getPrompText());
        driver.addChoice(choiceStack.back());
        driver.add(choiceStack.back());
        choiceStack.pop_back();
        configStack.pop_back();
    }
}
#line 1522 "src/parser.cpp"
    break;

  case 60:
#line 788 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        // Not here because it only affects the choice
        //addDepStack($2);
        choiceStack.back()->addInnerDep(yystack_[0].value.as < synExp* > ()->copy_exp());
        choiceStack.back()->addDepends(yystack_[0].value.as < synExp* > ()->copy_exp());
    }
}
#line 1535 "src/parser.cpp"
    break;

  case 64:
#line 802 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        choiceStack.back()->activatePrompt();
        choiceStack.back()->setPrompText(yystack_[2].value.as < std::string > ());
        choiceStack.back()->setPromptGuard(yystack_[1].value.as < synExp* > ());
    }
}
#line 1547 "src/parser.cpp"
    break;

  case 65:
#line 811 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        choiceStack.back()->setVartype(yystack_[2].value.as < kconf_id* > ()->type2string());
        driver.increaseCounters(yystack_[2].value.as < kconf_id* > ()->type2string());
    }
}
#line 1558 "src/parser.cpp"
    break;

  case 66:
#line 819 "src/parser.ypp"
    {
    if (!driver.firstPass)
        choiceStack.back()->setOptional();
}
#line 1567 "src/parser.cpp"
    break;

  case 67:
#line 825 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        choiceStack.back()->push_default(yystack_[1].value.as < synExp* > (), new synSymbol(yystack_[2].value.as < std::string > ()));
        // For def_tristate and def_bool (type and default at the same time)
        if (yystack_[3].value.as < kconf_id* > ()->type2string() != "unknown") {
            choiceStack.back()->setVartype(yystack_[3].value.as < kconf_id* > ()->type2string());
            driver.increaseCounters(yystack_[3].value.as < kconf_id* > ()->type2string());
        }
    }
}
#line 1582 "src/parser.cpp"
    break;

  case 70:
#line 841 "src/parser.ypp"
    {
    if (!driver.firstPass)
        handle_ifblock(driver, yystack_[1].value.as < synExp* > ());
}
#line 1591 "src/parser.cpp"
    break;

  case 72:
#line 849 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        delDepStack(configStack.back());
        configStack.pop_back();
    }
}
#line 1602 "src/parser.cpp"
    break;

  case 77:
#line 866 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        // Front-end related. Nothing to see here.
        driver.increaseMainMenus();
        //driver.menuStack.push_back($2);
    }

}
#line 1615 "src/parser.cpp"
    break;

  case 78:
#line 878 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        synExp* dep = synTrue;
        synExp* vis = synTrue;
        if (! configStack.empty()) {
            dep = configStack.back()->getDependsVar();
            vis = configStack.back()->getVisibility();
        }
        configInfo *menuinf = new configInfo();
        menuinf->setDepLength(depStack.size());
        menuinf->setHasConfig();
        menuinf->addDepends(dep->copy_exp());
        menuinf->addVisibility(vis->copy_exp());
        menuinf->setPrompText(yystack_[1].value.as < std::string > ());
        
        configStack.push_back(menuinf);
        menuStack.push_back(menuinf);
        driver.increaseMenus();
    }
}
#line 1640 "src/parser.cpp"
    break;

  case 79:
#line 900 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        configStack.back()->addVisibility(yystack_[1].value.as < synExp* > ());
        // This menu inherits the dependencies of upper menus
        
        addDepStack(yystack_[0].value.as < synExp* > ());
        configStack.back()->setDependStack(depStack);
        configStack.back()->addDepends(yystack_[0].value.as < synExp* > ()->copy_exp());
    }
}
#line 1655 "src/parser.cpp"
    break;

  case 81:
#line 914 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        if (!menuStack.back()->getSelects())
        driver.addMenuBlocks(menuStack.back());
        
        delDepStack(configStack.back());
        menuStack.pop_back();
        configStack.pop_back();
    }
}
#line 1670 "src/parser.cpp"
    break;

  case 86:
#line 933 "src/parser.ypp"
    {
        handle_source(driver, yystack_[1].value.as < std::string > ());
   
}
#line 1679 "src/parser.cpp"
    break;

  case 87:
#line 941 "src/parser.ypp"
    {
	
}
#line 1687 "src/parser.cpp"
    break;

  case 88:
#line 946 "src/parser.ypp"
    {
}
#line 1694 "src/parser.cpp"
    break;

  case 89:
#line 952 "src/parser.ypp"
    {
        driver.lexer->starthelp();
}
#line 1702 "src/parser.cpp"
    break;

  case 90:
#line 957 "src/parser.ypp"
    {
}
#line 1709 "src/parser.cpp"
    break;

  case 91:
#line 963 "src/parser.ypp"
    { yylhs.value.as < synExp* > () = synTrue; }
#line 1715 "src/parser.cpp"
    break;

  case 92:
#line 964 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        yylhs.value.as < synExp* > () = makeAnd(yystack_[1].value.as < synExp* > ()->copy_exp(), yystack_[0].value.as < synExp* > ()->copy_exp());}
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1725 "src/parser.cpp"
    break;

  case 93:
#line 969 "src/parser.ypp"
    {
    if (!driver.firstPass)
        yylhs.value.as < synExp* > () = yystack_[1].value.as < synExp* > ();
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1735 "src/parser.cpp"
    break;

  case 94:
#line 974 "src/parser.ypp"
    { yylhs.value.as < synExp* > () = synFalse;}
#line 1741 "src/parser.cpp"
    break;

  case 95:
#line 978 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        synExp* dollar3 = yystack_[1].value.as < synExp* > ();
        if (dollar3->isUnknown()) {
            dollar3->destroy();
            dollar3 = synFalse;
        }
        yylhs.value.as < synExp* > () = dollar3;
    }
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1757 "src/parser.cpp"
    break;

  case 96:
#line 994 "src/parser.ypp"
    { yylhs.value.as < synExp* > () = synTrue; }
#line 1763 "src/parser.cpp"
    break;

  case 97:
#line 995 "src/parser.ypp"
    {
    if (!driver.firstPass)
        yylhs.value.as < synExp* > () = makeAnd(yystack_[1].value.as < synExp* > ()->copy_exp(), yystack_[0].value.as < synExp* > ()->copy_exp());
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1773 "src/parser.cpp"
    break;

  case 98:
#line 1000 "src/parser.ypp"
    {
    if (!driver.firstPass)
        yylhs.value.as < synExp* > () = yystack_[1].value.as < synExp* > ();
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1783 "src/parser.cpp"
    break;

  case 99:
#line 1008 "src/parser.ypp"
    {
    if (!driver.firstPass)
        yylhs.value.as < synExp* > () = yystack_[0].value.as < synExp* > ();
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1793 "src/parser.cpp"
    break;

  case 100:
#line 1017 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        // If there is no previous prompt declared with T_PROMPT...
        if (configStack.back()->getPrompText() == "")
        configStack.back()->setPromptGuard(synFalse);
    }
}
#line 1805 "src/parser.cpp"
    break;

  case 101:
#line 1025 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        configStack.back()->activatePrompt();
        configStack.back()->setPrompText(yystack_[1].value.as < std::string > ());
        configStack.back()->setPromptGuard(yystack_[0].value.as < synExp* > ());
    }
}
#line 1817 "src/parser.cpp"
    break;

  case 102:
#line 1034 "src/parser.ypp"
    { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();   }
#line 1823 "src/parser.cpp"
    break;

  case 103:
#line 1035 "src/parser.ypp"
    {
    yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
}
#line 1831 "src/parser.cpp"
    break;

  case 104:
#line 1040 "src/parser.ypp"
    {  }
#line 1837 "src/parser.cpp"
    break;

  case 105:
#line 1041 "src/parser.ypp"
    { }
#line 1843 "src/parser.cpp"
    break;

  case 106:
#line 1042 "src/parser.ypp"
    {  }
#line 1849 "src/parser.cpp"
    break;

  case 109:
#line 1050 "src/parser.ypp"
    { yylhs.value.as < synExp* > () = synTrue;   }
#line 1855 "src/parser.cpp"
    break;

  case 110:
#line 1051 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        if (yystack_[0].value.as < synExp* > ()->isUnknown())
            yylhs.value.as < synExp* > () =  synFalse;
        else
            yylhs.value.as < synExp* > () = yystack_[0].value.as < synExp* > ();
    }
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1869 "src/parser.cpp"
    break;

  case 111:
#line 1062 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        yylhs.value.as < synExp* > () = yystack_[0].value.as < synExp* > ()->copy_exp();
    }
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1880 "src/parser.cpp"
    break;

  case 112:
#line 1068 "src/parser.ypp"
    {
    if (!driver.firstPass)
        yylhs.value.as < synExp* > () = handle_equality(yystack_[2].value.as < synExp* > (), yystack_[0].value.as < synExp* > ());
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1890 "src/parser.cpp"
    break;

  case 113:
#line 1073 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        std::cerr << "dolar1 " << yystack_[2].value.as < synExp* > () << " dolar3" << yystack_[0].value.as < synExp* > () << std::endl;
        yylhs.value.as < synExp* > () = makeNot(handle_equality(yystack_[2].value.as < synExp* > (), yystack_[0].value.as < synExp* > ())->copy_exp());
    }
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1902 "src/parser.cpp"
    break;

  case 114:
#line 1080 "src/parser.ypp"
    {
    if (!driver.firstPass)
        yylhs.value.as < synExp* > () =  yystack_[1].value.as < synExp* > ()->copy_exp();
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1912 "src/parser.cpp"
    break;

  case 115:
#line 1085 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        if (yystack_[0].value.as < synExp* > ()->isUnknown())
          yylhs.value.as < synExp* > () = synTrue;
        else
            yylhs.value.as < synExp* > () = makeNot(yystack_[0].value.as < synExp* > ()->copy_exp());
    }
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1926 "src/parser.cpp"
    break;

  case 116:
#line 1094 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        synExp* first  = yystack_[2].value.as < synExp* > ();
        synExp* second = yystack_[0].value.as < synExp* > ();
        if (yystack_[2].value.as < synExp* > ()->isUnknown())
            first = synFalse;
        if (yystack_[0].value.as < synExp* > ()->isUnknown())
            second = synFalse;
        
        yylhs.value.as < synExp* > () = makeOr(first->copy_exp(), second->copy_exp());
    }
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1944 "src/parser.cpp"
    break;

  case 117:
#line 1107 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        synExp* first  = yystack_[2].value.as < synExp* > ();
        synExp* second = yystack_[0].value.as < synExp* > ();
        if (yystack_[2].value.as < synExp* > ()->isUnknown())
            first = synFalse;
        if (yystack_[0].value.as < synExp* > ()->isUnknown())
            second = synFalse;
        
        yylhs.value.as < synExp* > () = makeAnd(first->copy_exp(), second->copy_exp());
    }
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1962 "src/parser.cpp"
    break;

  case 118:
#line 1121 "src/parser.ypp"
    {
    if (!driver.firstPass) {
        yylhs.value.as < synExp* > () = handle_symbol(driver, yystack_[0].value.as < std::string > ());
    }
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1973 "src/parser.cpp"
    break;

  case 119:
#line 1128 "src/parser.ypp"
    {
     if (!driver.firstPass)
        yylhs.value.as < synExp* > () = new synString(yystack_[0].value.as < std::string > ());
    else yylhs.value.as < synExp* > () = synTrue;
}
#line 1983 "src/parser.cpp"
    break;

  case 120:
#line 1135 "src/parser.ypp"
    { yylhs.value.as < std::string > () = ""; }
#line 1989 "src/parser.cpp"
    break;

  case 121:
#line 1136 "src/parser.ypp"
    {
    if (!driver.firstPass)
        yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
    else yylhs.value.as < std::string > () = "";
}
#line 1999 "src/parser.cpp"
    break;


#line 2003 "src/parser.cpp"

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
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
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


  const signed char Parser::yypact_ninf_ = -91;

  const signed char Parser::yytable_ninf_ = -89;

  const short
  Parser::yypact_[] =
  {
      25,    37,   -91,    12,   -21,   150,   -91,    26,   -91,   -91,
     -11,   -91,   -91,    30,    27,    37,    45,    37,    79,    78,
      37,    86,    89,    -9,   114,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,   161,   -91,   172,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,   -91,   189,   -91,   -21,   139,   -91,
     -91,   140,   -91,   141,   -91,   142,   -91,   144,   146,   147,
     -91,   -91,    -9,    -9,    65,    -2,   -91,   148,   149,    35,
     117,    80,   283,   267,   -18,   267,   222,   -11,   -91,   -91,
     -91,   -91,   -91,   -91,    39,   -91,    -9,    -9,   139,    44,
      44,   -91,   -91,   152,   153,   167,    37,    37,    -9,   171,
      44,   -91,   211,   -91,   -91,   -91,   -91,   184,   -91,   -91,
     179,    37,    37,   185,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,   -91,   -91,   -91,   203,   -91,   255,
     -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,    26,
     -91,   180,   -91,   -91,   -91,   -91,   -91,    -9,   203,   186,
     203,    46,   203,    44,   -12,   199,   -91,   -91,   203,   210,
     203,    -9,   -91,   -91,    69,   212,   -91,   -91,   213,   214,
     203,   181,   -91,   -91,   216,   -91,   218,    41,   -91,   -91,
     -91,   -91,   219,    37,   -91,   -91,   -91,   -91,   -91
  };

  const unsigned char
  Parser::yydefact_[] =
  {
       7,     0,   107,     0,     0,     0,     7,     7,   102,   103,
       0,     1,     4,     7,     0,     0,     0,     0,   120,     0,
       0,     0,     0,     0,     0,    15,    19,    16,    17,    21,
      18,    20,    22,    34,     0,    23,     0,     8,    37,    26,
      27,    37,    28,    58,    68,     9,    73,    24,    96,    82,
      10,    29,    91,    25,    11,     0,   108,     0,    77,     3,
      14,     0,   104,     0,   121,     0,   105,     0,     0,     0,
     118,   119,     0,     0,     0,   111,   106,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,    78,    86,
      54,    87,    32,    35,     0,   115,     0,     0,    70,     0,
       0,    12,    13,     0,     0,     0,     0,   100,     0,     0,
       0,    50,     0,    43,    42,    38,    39,     0,    41,    40,
       0,     0,   100,     0,    62,    63,    59,    61,    60,    69,
      57,    56,    74,    76,    72,    75,    71,   109,    98,     0,
      97,    83,    85,    81,    84,    80,    93,    94,    92,     7,
     114,   116,   117,   113,   112,    31,    89,     0,   109,     0,
     109,   109,   109,     0,     0,     0,    90,    66,   109,     0,
     109,     0,    99,     2,     0,     0,    44,   101,     0,     0,
     109,    52,    49,    30,     0,    65,     0,   110,    95,    45,
      46,    47,     0,     0,    51,    64,    67,    48,    53
  };

  const short
  Parser::yypgoto_[] =
  {
     -91,   -91,   198,    -6,   240,   -91,    29,   -75,   -91,   -91,
     -91,   -91,   -91,   226,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -51,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,    63,   -91,   -91,   -91,   -91,   -91,   201,
     195,   -73,   -91,   -91,   162,    -1,    59,    -5,   145,   -70,
     -90,   -91
  };

  const short
  Parser::yydefgoto_[] =
  {
      -1,     3,    13,     4,     5,    36,    37,   114,    38,    39,
      40,    41,    42,    79,   115,   116,   164,   194,    43,    44,
     130,    45,    81,   126,    82,    46,   134,    47,    83,     6,
      48,    49,   143,    50,    85,    51,    52,    53,   117,   118,
      86,   119,    84,   140,   159,   160,    54,     7,   172,    74,
      75,    65
  };

  const short
  Parser::yytable_[] =
  {
      10,    57,    94,    95,   137,    58,   125,    59,   128,   153,
     154,   147,    11,   148,    61,    12,    63,   138,   181,    67,
     163,    70,    71,   182,     2,    72,   151,   152,     1,     1,
      99,    73,   133,     1,   142,   -33,   103,   100,   161,   -33,
     -33,   -33,   -33,   -33,   -33,   -33,   -33,   104,   -33,   -33,
     105,   -33,   106,   107,   108,   109,   110,   -33,   111,   171,
       2,    56,    60,   -33,   147,   112,   148,     8,     9,    98,
     113,   -33,   150,   180,    70,    71,    96,    97,    96,    97,
      62,   103,   149,    96,    97,   -55,   -55,   174,   -55,   -55,
     -55,   -55,   104,   -55,   -55,   105,   120,   121,   122,   123,
       2,   187,    96,    97,   188,   158,    96,    97,   -55,    64,
     112,   129,   132,    66,   141,   124,    68,   -36,   103,    69,
     168,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   -36,   104,
     -36,   -36,   105,   -36,   106,   107,   108,   109,   110,   -36,
     111,   131,   136,   173,   145,   -36,   135,   112,   144,    76,
      -6,    14,   113,   -36,    15,    16,    17,    18,    19,    20,
      21,    22,    77,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    78,    56,    88,    89,    90,    33,    91,
      34,    92,    93,   101,   102,    35,    -6,   155,   156,    -5,
      14,   157,   198,    15,    16,    17,    18,    19,    20,    21,
      22,   162,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,   165,   166,   167,   170,   171,    33,    97,    34,
     193,   176,   -88,   103,    35,    -5,   -88,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,   183,   -88,   -88,   105,   -88,   -88,
     -88,   -88,   -88,   -88,   -88,   185,    55,   189,   190,   191,
     -88,   195,   112,   196,   197,    87,   103,   146,   -88,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,    80,   -79,   -79,
     105,    15,    16,    17,    18,    19,    20,    21,    22,   139,
      23,    24,   127,   -79,   169,   112,     0,     0,    16,    17,
     146,    19,    20,    21,    22,    33,    23,    24,     0,     0,
       0,     0,    35,   175,     0,   177,   178,   179,     0,     0,
       0,    33,     0,   184,     0,   186,     0,     0,    35,     0,
       0,     0,     0,     0,     0,   192
  };

  const short
  Parser::yycheck_[] =
  {
       1,     7,    72,    73,    22,    10,    81,    13,    81,    99,
     100,    86,     0,    86,    15,    36,    17,    35,    30,    20,
     110,    30,    31,    35,    35,    34,    96,    97,     3,     3,
      32,    40,    83,     3,    85,     0,     1,    39,   108,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    13,
      35,    35,    35,    28,   139,    30,   139,    30,    31,    74,
      35,    36,    33,   163,    30,    31,    37,    38,    37,    38,
      35,     1,    87,    37,    38,     5,     6,   157,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      35,   171,    37,    38,    35,   106,    37,    38,    28,    30,
      30,    82,    83,    35,    85,    35,    30,     0,     1,    30,
     121,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    82,    83,   149,    85,    28,    83,    30,    85,    35,
       0,     1,    35,    36,     4,     5,     6,     7,     8,     9,
      10,    11,     1,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,     1,    35,    35,    35,    35,    28,    35,
      30,    35,    35,    35,    35,    35,    36,    35,    35,     0,
       1,    24,   193,     4,     5,     6,     7,     8,     9,    10,
      11,    30,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     1,    29,    35,    30,    13,    28,    38,    30,
      39,    35,     0,     1,    35,    36,     4,     5,     6,     7,
       8,     9,    10,    11,    35,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    35,     6,    35,    35,    35,
      28,    35,    30,    35,    35,    57,     1,    35,    36,     4,
       5,     6,     7,     8,     9,    10,    11,    41,    13,    14,
      15,     4,     5,     6,     7,     8,     9,    10,    11,    84,
      13,    14,    81,    28,   122,    30,    -1,    -1,     5,     6,
      35,     8,     9,    10,    11,    28,    13,    14,    -1,    -1,
      -1,    -1,    35,   158,    -1,   160,   161,   162,    -1,    -1,
      -1,    28,    -1,   168,    -1,   170,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    -1,   180
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,     3,    35,    42,    44,    45,    70,    88,    30,    31,
      86,     0,    36,    43,     1,     4,     5,     6,     7,     8,
       9,    10,    11,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    28,    30,    35,    46,    47,    49,    50,
      51,    52,    53,    59,    60,    62,    66,    68,    71,    72,
      74,    76,    77,    78,    87,    45,    35,    44,    88,    44,
      35,    86,    35,    86,    30,    92,    35,    86,    30,    30,
      30,    31,    34,    40,    90,    91,    35,     1,     1,    54,
      54,    63,    65,    69,    83,    75,    81,    43,    35,    35,
      35,    35,    35,    35,    90,    90,    37,    38,    88,    32,
      39,    35,    35,     1,    12,    15,    17,    18,    19,    20,
      21,    23,    30,    35,    48,    55,    56,    79,    80,    82,
      16,    17,    18,    19,    35,    48,    64,    80,    82,    47,
      61,    87,    47,    62,    67,    74,    87,    22,    35,    81,
      84,    47,    62,    73,    74,    87,    35,    48,    82,    88,
      33,    90,    90,    91,    91,    35,    35,    24,    86,    85,
      86,    90,    30,    91,    57,     1,    29,    35,    86,    85,
      30,    13,    89,    44,    90,    89,    35,    89,    89,    89,
      91,    30,    35,    35,    89,    35,    89,    90,    35,    35,
      35,    35,    89,    39,    58,    35,    35,    35,    86
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    41,    42,    42,    43,    44,    44,    45,    45,    45,
      45,    45,    45,    45,    45,    46,    46,    46,    46,    46,
      46,    46,    46,    47,    47,    47,    47,    47,    47,    47,
      48,    48,    49,    50,    51,    52,    53,    54,    54,    54,
      54,    54,    54,    54,    55,    55,    55,    55,    55,    56,
      57,    57,    58,    58,    59,    60,    61,    62,    63,    63,
      63,    63,    63,    63,    64,    64,    64,    64,    65,    65,
      66,    67,    68,    69,    69,    69,    69,    70,    71,    72,
      73,    74,    75,    75,    75,    75,    76,    77,    78,    79,
      80,    81,    81,    81,    81,    82,    83,    83,    83,    84,
      85,    85,    86,    86,    87,    87,    87,    88,    88,    89,
      89,    90,    90,    90,    90,    90,    90,    90,    91,    91,
      92,    92
  };

  const unsigned char
  Parser::yyr2_[] =
  {
       0,     2,     5,     3,     1,     2,     1,     0,     2,     2,
       2,     2,     4,     4,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     2,     3,     2,     1,     3,     2,     0,     2,     2,
       2,     2,     2,     2,     3,     4,     4,     4,     5,     3,
       0,     3,     0,     2,     3,     2,     1,     3,     0,     2,
       2,     2,     2,     2,     4,     3,     2,     4,     0,     2,
       3,     1,     3,     0,     2,     2,     2,     3,     3,     3,
       1,     3,     0,     2,     2,     2,     3,     3,     2,     2,
       2,     0,     2,     2,     2,     4,     0,     2,     2,     2,
       0,     2,     1,     1,     2,     2,     2,     1,     2,     0,
       2,     1,     3,     3,     3,     2,     3,     3,     1,     1,
       0,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "T_EOF", "error", "$undefined", "T_MAINMENU", "T_MENU", "T_ENDMENU",
  "T_SOURCE", "T_CHOICE", "T_ENDCHOICE", "T_COMMENT", "T_CONFIG",
  "T_MENUCONFIG", "T_HELP", "T_IF", "T_ENDIF", "T_DEPENDS", "T_OPTIONAL",
  "T_PROMPT", "T_TYPE", "T_DEFAULT", "T_SELECT", "T_RANGE", "T_VISIBLE",
  "T_OPTION", "T_ON", "T_OPT_DEFCONFIG_LIST", "T_OPT_MODULES", "T_OPT_ENV",
  "T_INSERT", "T_HELPTEXT", "T_WORD", "T_WORD_QUOTE", "T_UNEQUAL",
  "T_CLOSE_PAREN", "T_OPEN_PAREN", "T_EOL", "T_EOP", "T_OR", "T_AND",
  "T_EQUAL", "T_NOT", "$accept", "input", "pass", "start", "stmt_list",
  "option_name", "common_stmt", "option_error", "config_entry_start",
  "config_stmt", "insert_stmt", "menuconfig_entry_start",
  "menuconfig_stmt", "config_option_list", "config_option",
  "symbol_option", "symbol_option_list", "symbol_option_arg", "choice",
  "choice_entry", "choice_end", "choice_stmt", "choice_option_list",
  "choice_option", "choice_block", "if_entry", "if_end", "if_stmt",
  "if_block", "mainmenu_stmt", "menu", "menu_entry", "menu_end",
  "menu_stmt", "menu_block", "source_stmt", "comment", "comment_stmt",
  "help_start", "help", "depends_list", "depends", "visibility_list",
  "visible", "prompt_stmt_opt", "prompt", "end", "nl", "if_expr", "expr",
  "symbol", "word_opt", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short
  Parser::yyrline_[] =
  {
       0,   567,   567,   569,   572,   575,   577,   580,   582,   583,
     584,   585,   586,   587,   591,   595,   595,   595,   595,   595,
     595,   595,   595,   599,   600,   601,   602,   603,   604,   605,
     609,   610,   616,   624,   629,   631,   639,   644,   646,   647,
     648,   657,   658,   659,   662,   671,   680,   686,   698,   717,
     720,   722,   750,   751,   756,   763,   769,   772,   785,   787,
     788,   796,   797,   798,   801,   810,   818,   824,   836,   838,
     841,   846,   849,   856,   858,   859,   860,   865,   877,   899,
     911,   914,   925,   927,   928,   929,   932,   940,   945,   951,
     956,   963,   964,   969,   974,   977,   994,   995,  1000,  1007,
    1017,  1025,  1034,  1035,  1040,  1041,  1042,  1046,  1047,  1050,
    1051,  1062,  1068,  1073,  1080,  1085,  1094,  1107,  1121,  1128,
    1135,  1136
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
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
  Parser::yy_reduce_print_ (int yyrule)
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


#line 465 "src/parser.ypp"
} // kconf
#line 2566 "src/parser.cpp"

#line 1144 "src/parser.ypp"
 /*** Additional Code ***/
void kconf::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
    exit(-1);
}

