/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf -a -C -E -g -k '1,3,$' -P  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif


#include "parser.hpp"
#include "kconf_id.hpp"
struct kconf_id;
/* maximum key range = 71, duplicates = 0 */

class Perfect_Hash
{
private:
  static inline unsigned int kconf_id_hash (const char *str, size_t len);
public:
  static const struct kconf_id *kconf_id_lookup (const char *str, size_t len);
};

inline unsigned int
Perfect_Hash::kconf_id_hash (const char *str, size_t len)
{
  static const unsigned char asso_values[] =
    {
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73,  0, 73, 73, 73, 73, 73, 73,
      73, 73, 73,  0,  0, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 25, 25,
       0,  0,  0,  5,  0,  0, 73, 73,  5,  0,
      10,  5, 45, 73, 20, 20,  0, 15, 15, 73,
      20, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73
    };
  unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[static_cast<unsigned char>(str[2])];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[static_cast<unsigned char>(str[0])];
        break;
    }
  return hval + asso_values[static_cast<unsigned char>(str[len - 1])];
}

struct kconf_id_strings_t
  {
    char kconf_id_strings_str2[sizeof("if")];
    char kconf_id_strings_str3[sizeof("int")];
    char kconf_id_strings_str5[sizeof("endif")];
    char kconf_id_strings_str6[sizeof("INSERT")];
    char kconf_id_strings_str7[sizeof("default")];
    char kconf_id_strings_str8[sizeof("tristate")];
    char kconf_id_strings_str9[sizeof("endchoice")];
    char kconf_id_strings_str12[sizeof("def_tristate")];
    char kconf_id_strings_str13[sizeof("def_bool")];
    char kconf_id_strings_str14[sizeof("defconfig_list")];
    char kconf_id_strings_str17[sizeof("on")];
    char kconf_id_strings_str18[sizeof("optional")];
    char kconf_id_strings_str21[sizeof("option")];
    char kconf_id_strings_str22[sizeof("endmenu")];
    char kconf_id_strings_str23[sizeof("mainmenu")];
    char kconf_id_strings_str25[sizeof("menuconfig")];
    char kconf_id_strings_str27[sizeof("modules")];
    char kconf_id_strings_str29[sizeof("menu")];
    char kconf_id_strings_str31[sizeof("select")];
    char kconf_id_strings_str32[sizeof("comment")];
    char kconf_id_strings_str33[sizeof("env")];
    char kconf_id_strings_str35[sizeof("range")];
    char kconf_id_strings_str36[sizeof("choice")];
    char kconf_id_strings_str39[sizeof("bool")];
    char kconf_id_strings_str41[sizeof("source")];
    char kconf_id_strings_str42[sizeof("visible")];
    char kconf_id_strings_str43[sizeof("hex")];
    char kconf_id_strings_str46[sizeof("config")];
    char kconf_id_strings_str47[sizeof("boolean")];
    char kconf_id_strings_str51[sizeof("string")];
    char kconf_id_strings_str54[sizeof("help")];
    char kconf_id_strings_str56[sizeof("prompt")];
    char kconf_id_strings_str72[sizeof("depends")];
  };
static const struct kconf_id_strings_t kconf_id_strings_contents =
  {
    "if",
    "int",
    "endif",
    "INSERT",
    "default",
    "tristate",
    "endchoice",
    "def_tristate",
    "def_bool",
    "defconfig_list",
    "on",
    "optional",
    "option",
    "endmenu",
    "mainmenu",
    "menuconfig",
    "modules",
    "menu",
    "select",
    "comment",
    "env",
    "range",
    "choice",
    "bool",
    "source",
    "visible",
    "hex",
    "config",
    "boolean",
    "string",
    "help",
    "prompt",
    "depends"
  };
#define kconf_id_strings ((const char *) &kconf_id_strings_contents)
const struct kconf_id *
Perfect_Hash::kconf_id_lookup (const char *str, size_t len)
{
  enum
    {
      TOTAL_KEYWORDS = 33,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 14,
      MIN_HASH_VALUE = 2,
      MAX_HASH_VALUE = 72
    };

  static const struct kconf_id wordlist[] =
    {
      {-1}, {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str2,             token::T_IF,                    TF_COMMAND|TF_PARAM},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str3,            token::T_TYPE,                  TF_COMMAND, S_INT},
      {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str5,          token::T_ENDIF,                 TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str6,		token::T_INSERT,		TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str7,        token::T_DEFAULT,               TF_COMMAND, S_UNKNOWN},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str8,       token::T_TYPE,                  TF_COMMAND, S_TRISTATE},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str9,      token::T_ENDCHOICE,             TF_COMMAND},
      {-1}, {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str12,	token::T_DEFAULT,               TF_COMMAND, S_TRISTATE},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str13,       token::T_DEFAULT,               TF_COMMAND, S_BOOLEAN},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str14,	token::T_OPT_DEFCONFIG_LIST,    TF_OPTION},
      {-1}, {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str17,             token::T_ON,                    TF_PARAM},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str18,       token::T_OPTIONAL,              TF_COMMAND},
      {-1}, {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str21,         token::T_OPTION,                TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str22,        token::T_ENDMENU,               TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str23,       token::T_MAINMENU,              TF_COMMAND},
      {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str25,     token::T_MENUCONFIG,            TF_COMMAND},
      {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str27,        token::T_OPT_MODULES,           TF_OPTION},
      {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str29,           token::T_MENU,                  TF_COMMAND},
      {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str31,         token::T_SELECT,                TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str32,        token::T_COMMENT,               TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str33,            token::T_OPT_ENV,               TF_OPTION},
      {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str35,          token::T_RANGE,                 TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str36,         token::T_CHOICE,                TF_COMMAND},
      {-1}, {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str39,           token::T_TYPE,                  TF_COMMAND, S_BOOLEAN},
      {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str41,         token::T_SOURCE,                TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str42,        token::T_VISIBLE,               TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str43,            token::T_TYPE,                  TF_COMMAND, S_HEX},
      {-1}, {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str46,         token::T_CONFIG,                TF_COMMAND},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str47,        token::T_TYPE,                  TF_COMMAND, S_BOOLEAN},
      {-1}, {-1}, {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str51,         token::T_TYPE,                  TF_COMMAND, S_STRING},
      {-1}, {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str54,           token::T_HELP,                  TF_COMMAND},
      {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str56,         token::T_PROMPT,                TF_COMMAND},
      {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
      {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
      {(int)(size_t)&((struct kconf_id_strings_t *)0)->kconf_id_strings_str72,        token::T_DEPENDS,               TF_COMMAND}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = kconf_id_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          int o = wordlist[key].name;
          if (o >= 0)
            {
              const char *s = o + kconf_id_strings;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
                return &wordlist[key];
            }
        }
    }
  return 0;
}



