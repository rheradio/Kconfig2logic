/*
 * Copyright (C) 2002 Roman Zippel <zippel@linux-m68k.org>
 * Released under the terms of the GNU GPL v2.0.
 */

#include <locale.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>

#include "lkc.h"

char *number;

static void conf(struct menu *menu);
static void check_conf(struct menu *menu);
static void xfgets(char *str, int size, FILE *in);

enum input_mode {
	oldaskconfig,
	silentoldconfig,
	oldconfig,
	allnoconfig,
	allyesconfig,
	allmodconfig,
	alldefconfig,
	randconfig,
	defconfig,
	savedefconfig,
	listnewconfig,
	olddefconfig,
} input_mode = oldaskconfig;

static int indent = 1;
static int tty_stdio;
static int valid_stdin = 1;
static int sync_kconfig;
static int conf_cnt;
static char line[PATH_MAX];
static struct menu *rootEntry;

static void print_help(struct menu *menu)
{
	struct gstr help = str_new();

	menu_get_ext_help(menu, &help);

	printf("\n%s\n", str_get(&help));
	str_free(&help);
}

static void strip(char *str)
{
	char *p = str;
	int l;

	while ((isspace(*p)))
		p++;
	l = strlen(p);
	if (p != str)
		memmove(str, p, l + 1);
	if (!l)
		return;
	p = str + l - 1;
	while ((isspace(*p)))
		*p-- = 0;
}

static void check_stdin(void)
{
	if (!valid_stdin) {
		printf(_("aborted!\n\n"));
		printf(_("Console input/output is redirected. "));
		printf(_("Run 'make oldconfig' to update configuration.\n\n"));
		exit(1);
	}
}

static int conf_askvalue(struct symbol *sym, const char *def)
{
	enum symbol_type type = sym_get_type(sym);

	if (!sym_has_value(sym))
		printf(_("(NEW) "));

	line[0] = '\n';
	line[1] = 0;

	if (!sym_is_changable(sym)) {
		printf("%s\n", def);
		line[0] = '\n';
		line[1] = 0;
		return 0;
	}

	switch (input_mode) {
	case oldconfig:
	case silentoldconfig:
		if (sym_has_value(sym)) {
			printf("%s\n", def);
			return 0;
		}
		check_stdin();
		/* fall through */
	case oldaskconfig:
		fflush(stdout);
		xfgets(line, sizeof(line), stdin);
		if (!tty_stdio)
			printf("\n");
		return 1;
	default:
		break;
	}

	switch (type) {
	case S_INT:
	case S_HEX:
	case S_STRING:
		printf("%s\n", def);
		return 1;
	default:
		;
	}
	printf("%s", line);
	return 1;
}

bool isInListName(struct listNames* s, char* name) {
    for(struct listNames *iterator = s;
        iterator; iterator = iterator->next) {
        if (!strcmp(iterator->name, name))
            return true;
    }
    return false;
}
// Adds a name to a listName, only if it is not already there
struct listNames* addToken(struct listNames* list, char* token) {
    struct listNames* start = list;
    if (list == NULL) {
        list = malloc(sizeof(struct listNames));
        list->name = strdup(token);
        list->next = NULL;
        return list;
    }
    while (list->next != NULL) {
        if (!strcmp(list->name, token)) return start;
        list = list->next;
    }
    if (!strcmp(list->name, token)) return start;
    struct listNames *new = malloc(sizeof(struct listNames));
    new->name = strdup(token);
    new->next = NULL;
    list->next = new;
    return start;
}

void addRelatedLiterals(struct listNames** relatedLiterals,
                        struct listNames** relatedSymbols,
                        char* name) {
    struct symbol *sym = sym_find(name);
    if (sym == NULL || sym->type == S_UNKNOWN) {
        *relatedLiterals = addToken(*relatedLiterals, name);
        return;
    }
        
    if (sym->type == S_STRING || sym->type == S_INT || sym->type == S_HEX)
        if (!isInListName(*relatedSymbols, sym->name)) {
            *relatedSymbols = addToken(*relatedSymbols, sym->name);
            for(struct listNames* trav = sym->related; trav; trav = trav->next)
                addRelatedLiterals(relatedLiterals, relatedSymbols, trav->name);
        }
    return;
}

static int conf_string(struct menu *menu)
{
    struct symbol *sym = menu->sym;
    const char *def, *value = NULL;
    
    while (1) {
        printf("%*s%s ", indent - 1, "", _(menu->prompt->text));
        printf("(%s) ", sym->name);
        def = sym_get_string_value(sym);
        if (sym_get_string_value(sym))
            printf("[%s] ", def);
        
        strcpy(line, def);
   
        int counter = 0;
        // We build here a listName of the related symbols. The related symbols
        // are substituted for their related symbols and so on until there are
        // only literals. Watch out for recursion
        struct listNames *literalsRelated = NULL, *symbolsRelated = NULL;
        for(struct listNames* iterator = sym->related; iterator; iterator = iterator->next) {
            addRelatedLiterals(&literalsRelated, &symbolsRelated, iterator->name);
        }
        for(struct listNames* trav = literalsRelated; trav; trav = trav->next) {
            //fprintf(stderr, "Related %s\n", trav->name);
            counter++;
        }

        int r = (rand() % (counter+1));

        if (r == 0) {
            if (sym->type == S_STRING)
            	value = "OTHERVAL";
	    else if (sym->type == S_INT)
            value = "OTHERVAL";
	    else if (sym->type == S_HEX)
            value = "OTHERVAL";
        }	
        else {
            counter = 1;
            struct listNames* iterator = literalsRelated;
            
            while (counter++ < r) iterator = iterator->next;
                value = strdup(iterator->name);
        }
        printf("%s\n", value);
        //if (*value == '\0')
        //    value = "EMPTY";
        if (value && sym_set_string_value(sym, value))
            return 0;
    }
}

static int conf_sym(struct menu *menu)
{
    struct symbol *sym = menu->sym;
    tristate oldval, newval;
    char res[3];
    int possibilities = 0;
    while (1) {
        printf("%*s%s ", indent - 1, "", _(menu->prompt->text));
        if (sym->name)
            printf("(%s) ", sym->name);
        putchar('[');
        oldval = sym_get_tristate_value(sym);
        switch (oldval) {
            case no:
                putchar('N');
                res[possibilities++] = 'n';
                break;
            case mod:
                putchar('M');
                res[possibilities++] = 'm';
                break;
            case yes:
                putchar('Y');
                res[possibilities++] = 'y';
                break;
        }
        if (oldval != no && sym_tristate_within_range(sym, no)) {
            printf("/n");
            res[possibilities++] = 'n';
        }
        if (oldval != mod && sym_tristate_within_range(sym, mod)) {
            printf("/m");
            res[possibilities++] = 'm';
        }
        if (oldval != yes && sym_tristate_within_range(sym, yes)) {
            printf("/y");
            res[possibilities++] = 'y';
            
        }
        if (menu_has_help(menu))
            printf("/?");
        printf("] ");
        //if (!conf_askvalue(sym, sym_get_string_value(sym)))
        //    return 0;
        
        if (possibilities)
            line[0] = res[rand() % possibilities];
        else
            line[0] = res[0];
        
        
        line[1] = 0;
        printf("%s\n",line);
        
        if (!possibilities)
            return 0;
        //strip(line);
        
        switch (line[0]) {
            case 'n':
            case 'N':
                newval = no;
                if (!line[1] || !strcmp(&line[1], "o"))
                    break;
                continue;
            case 'm':
            case 'M':
                newval = mod;
                if (!line[1])
                    break;
                continue;
            case 'y':
            case 'Y':
                newval = yes;
                if (!line[1] || !strcmp(&line[1], "es"))
                    break;
                continue;
            case 0:
                newval = oldval;
                break;
            case '?':
                goto help;
            default:
                continue;
        }
        if (sym_set_tristate_value(sym, newval))
            return 0;
    help:
        print_help(menu);
    }
}

static int conf_choice(struct menu *menu)
{
    struct symbol *sym, *def_sym;
    struct menu *child;
    bool is_new;
    
    sym = menu->sym;
    is_new = !sym_has_value(sym);
    if (sym_is_changable(sym)) {
        conf_sym(menu);
        sym_calc_value(sym);
        switch (sym_get_tristate_value(sym)) {
            case no:
                return 1;
            case mod:
                return 0;
            case yes:
                break;
        }
    } else {
        switch (sym_get_tristate_value(sym)) {
            case no:
                return 1;
            case mod:
                printf("%*s%s\n", indent - 1, "", _(menu_get_prompt(menu)));
                return 0;
            case yes:
                break;
        }
    }
    
    while (1) {
        int cnt, def;
        
        printf("%*s%s\n", indent - 1, "", _(menu_get_prompt(menu)));
        def_sym = sym_get_choice_value(sym);
        cnt = def = 0;
        line[0] = 0;
        for (child = menu->list; child; child = child->next) {
            if (!menu_is_visible(child))
                continue;
            if (!child->sym) {
                printf("%*c %s\n", indent, '*', _(menu_get_prompt(child)));
                continue;
            }
            cnt++;
            if (child->sym == def_sym) {
                def = cnt;
                printf("%*c", indent, '>');
            } else
                printf("%*c", indent, ' ');
            printf(" %d. %s", cnt, _(menu_get_prompt(child)));
            if (child->sym->name)
                printf(" (%s)", child->sym->name);
            if (!sym_has_value(child->sym))
                printf(_(" (NEW)"));
            printf("\n");
        }
        printf(_("%*schoice"), indent - 1, "");
        if (cnt == 1) {
            printf("[1]: 1\n");
            goto conf_childs;
        }
        printf("[1-%d", cnt);
        if (menu_has_help(menu))
            printf("?");
        printf("]: ");
        switch (input_mode) {
            case oldconfig:
            case silentoldconfig:
                if (!is_new) {
                    cnt = def;
                    printf("%d\n", cnt);
                    break;
                }
                check_stdin();
                /* fall through */
            case oldaskconfig:
                fflush(stdout);
                if (cnt)
                    cnt =  1 + (rand() % cnt);
                else
                    cnt = 1;
                printf("%d\n",cnt);
                break;
            default:
                break;
        }
        
    conf_childs:
        for (child = menu->list; child; child = child->next) {
            if (!child->sym || !menu_is_visible(child))
                continue;
            if (!--cnt)
                break;
        }
        if (!child)
            continue;
        if (line[0] && line[strlen(line) - 1] == '?') {
            print_help(child);
            continue;
        }
        sym_set_choice_value(sym, child->sym);
        for (child = child->list; child; child = child->next) {
            indent += 2;
            conf(child);
            indent -= 2;
        }
        return 1;
    }
}


static void conf(struct menu *menu)
{
	struct symbol *sym;
	struct property *prop;
	struct menu *child;

	if (!menu_is_visible(menu))
		return;

	sym = menu->sym;
	prop = menu->prompt;
	if (prop) {
		const char *prompt;

		switch (prop->type) {
		case P_MENU:
			if ((input_mode == silentoldconfig ||
			     input_mode == listnewconfig ||
			     input_mode == olddefconfig) &&
			    rootEntry != menu) {
				check_conf(menu);
				return;
			}
			/* fall through */
		case P_COMMENT:
			prompt = menu_get_prompt(menu);
			if (prompt)
				printf("%*c\n%*c %s\n%*c\n",
					indent, '*',
					indent, '*', _(prompt),
					indent, '*');
		default:
			;
		}
	}

	if (!sym)
		goto conf_childs;

	if (sym_is_choice(sym)) {
		conf_choice(menu);
		if (sym->curr.tri != mod)
			return;
		goto conf_childs;
	}

	switch (sym->type) {
	case S_INT:
	case S_HEX:
	case S_STRING:
		conf_string(menu);
		break;
	default:
		conf_sym(menu);
		break;
	}

conf_childs:
	if (sym)
		indent += 2;
	for (child = menu->list; child; child = child->next)
		conf(child);
	if (sym)
		indent -= 2;
}

static void check_conf(struct menu *menu)
{
	struct symbol *sym;
	struct menu *child;

	if (!menu_is_visible(menu))
		return;

	sym = menu->sym;
	if (sym && !sym_has_value(sym)) {
		if (sym_is_changable(sym) ||
		    (sym_is_choice(sym) && sym_get_tristate_value(sym) == yes)) {
			if (input_mode == listnewconfig) {
				if (sym->name && !sym_is_choice_value(sym)) {
					printf("%s%s\n", CONFIG_, sym->name);
				}
			} else if (input_mode != olddefconfig) {
				if (!conf_cnt++)
					printf(_("*\n* Restart config...\n*\n"));
				rootEntry = menu_get_parent_menu(menu);
				conf(rootEntry);
			}
		}
	}

	for (child = menu->list; child; child = child->next)
		check_conf(child);
}

static struct option long_opts[] = {
	{"oldaskconfig",    no_argument,       NULL, oldaskconfig},
	{"oldconfig",       no_argument,       NULL, oldconfig},
	{"silentoldconfig", no_argument,       NULL, silentoldconfig},
	{"defconfig",       optional_argument, NULL, defconfig},
	{"savedefconfig",   required_argument, NULL, savedefconfig},
	{"allnoconfig",     no_argument,       NULL, allnoconfig},
	{"allyesconfig",    no_argument,       NULL, allyesconfig},
	{"allmodconfig",    no_argument,       NULL, allmodconfig},
	{"alldefconfig",    no_argument,       NULL, alldefconfig},
	{"randconfig",      no_argument,       NULL, randconfig},
	{"listnewconfig",   no_argument,       NULL, listnewconfig},
	{"olddefconfig",    no_argument,       NULL, olddefconfig},
	/*
	 * oldnoconfig is an alias of olddefconfig, because people already
	 * are dependent on its behavior(sets new symbols to their default
	 * value but not 'n') with the counter-intuitive name.
	 */
	{"oldnoconfig",     no_argument,       NULL, olddefconfig},
	{NULL, 0, NULL, 0}
};

static void conf_usage(const char *progname)
{

	printf("Usage: %s [-s] [option] <kconfig-file>\n", progname);
	printf("[option] is _one_ of the following:\n");
	printf("  --listnewconfig         List new options\n");
	printf("  --oldaskconfig          Start a new configuration using a line-oriented program\n");
	printf("  --oldconfig             Update a configuration using a provided .config as base\n");
	printf("  --silentoldconfig       Same as oldconfig, but quietly, additionally update deps\n");
	printf("  --olddefconfig          Same as silentoldconfig but sets new symbols to their default value\n");
	printf("  --oldnoconfig           An alias of olddefconfig\n");
	printf("  --defconfig <file>      New config with default defined in <file>\n");
	printf("  --savedefconfig <file>  Save the minimal current configuration to <file>\n");
	printf("  --allnoconfig           New config where all options are answered with no\n");
	printf("  --allyesconfig          New config where all options are answered with yes\n");
	printf("  --allmodconfig          New config where all options are answered with mod\n");
	printf("  --alldefconfig          New config with all symbols set to default\n");
	printf("  --randconfig            New config with random answer to all options\n");
}
// DFA 30/10/2019 To keep all symbols and all related symbols
struct allSymbols *total_symbol_hash[SYMBOL_HASHSIZE];
// END DFA

struct allSymbols *allSymbolsFind(const char *name)
{
    int hash = 0;

    if (!name)
        return NULL;

    hash = strhash(name) % SYMBOL_HASHSIZE;
    struct allSymbols *as;
    for (as = total_symbol_hash[hash]; as; as = as->next) {
        if (as && !strcmp(as->symName, name))
                break;
    }

    return as;
}




void readAllSymbols() {
    char line[20*1024];
    int hash;
    struct allSymbols *aS = NULL;
    FILE *allSymbols = fopen("symbols", "r");
    if (!allSymbols) {
        fprintf(stderr, "symbols file not found\n");
        exit(-1);
    }
    //fprintf(stderr, "Initializing");
    for(int c = 0; c < SYMBOL_HASHSIZE; c++)
        total_symbol_hash[c] = NULL;
    //fprintf(stderr, " done\n");

    while (fgets(line, 20*1024-1, allSymbols)) {
        aS = malloc(sizeof(struct allSymbols));
        if (aS == NULL) {
            fprintf(stderr, "Malloc failed");
            exit(-1);
        }
        aS->size = 0;
        char *name  = strtok(line, " ");
        aS->symName = strdup(name);
        //fprintf(stderr, "Symbol name %s\n", aS->symName);
        aS->list    = NULL;
        aS->next    = NULL;

        hash = strhash(name) % SYMBOL_HASHSIZE;
        char *token;
        while ((token = strtok(NULL, " "))) {
            if (token[0] != 10) {
                aS->list = addToken(aS->list, token);
                aS->size++;
            }
        }
        if (total_symbol_hash[hash] == NULL) {
            total_symbol_hash[hash] = aS;
        }
        // Collision
        else {
            struct allSymbols *entry = allSymbolsFind(aS->symName);
            if (entry) {
                //fprintf(stderr, "Previously defined %s\n", aS->symName);
                for(struct listNames *list = aS->list; list; list = list->next)
                    entry->list = addToken(entry->list, list->name);
            }
            else {
                //fprintf(stderr, "Collision %s\n", aS->symName);
            struct allSymbols* aSS = NULL;
            for(aSS = total_symbol_hash[hash]; aSS->next; aSS = aSS->next){};
                aSS->next = aS;
            }
        }
    }
}

void       traverseAllSymbols() {
    char *filename = "configuration";
    if (number) {
        filename = malloc(sizeof(char)+strlen("configuration.")+strlen(number)+1);
        strcpy(filename, "configuration.");
        strcat(filename, number);
    }
    FILE *configuration = fopen(filename, "w");
    bool environment;
    struct property *prop;
    int i = 0, size;
    const char *val = NULL;
    char *value = NULL, *oldval = NULL, *newval = NULL;
    struct symbol *sym = NULL;
    if (!configuration) {
        fprintf(stderr, "Could not open configuration for writing\n");
        exit(-1);
    }
    for_all_symbols(i, sym) {
        sym_calc_value(sym);
        val = sym_get_string_value(sym);
//        if (val == NULL) {
//            fprintf(configuration, "not %s\n", sym->name);
//            break;
//        }
//        else
            value = strdup(val);
        
        // Change parenthesis and spaces for underscores
        for(int c = 0; c < strlen(value); c++)
            if (value[c] == '(' || value[c] == ')' || value[c] == ' ')
                value[c] = '_';
        if (!sym->name)
            continue;
        // Treat symbol
        switch (sym->type) {
            case S_BOOLEAN:
            case S_TRISTATE:
                if (*value == 'n' || !strcmp(value, "\"n\"")) {
                    fprintf(configuration, "not %s\n", sym->name);
                }
                else
                    if (*value == 'y'|| !strcmp(value, "\"y\"")) {
                        fprintf(configuration, "%s\n", sym->name);
                    }
                    else
                        if (*value == 'm'|| !strcmp(value, "\"m\"")) {
                            fprintf(configuration, "%s=m\n", sym->name);
                        }
                
                break;
            case S_HEX:
                // If the value does not begin with 0x we put it at the
                // beginning. This can happen if there was no prompt
                // and the value was taken as is from the default clause
                size = strlen(value) + 1;
                if (value[0] != '0' ||
                    (value[1] != 'x' && value[1] != 'X')) {
                    size += 2;
                    newval = oldval = xmalloc(size);
                    *oldval++ = '0';
                    *oldval++ = 'x';
                    strcpy(oldval, value);
                    free(value);
                    value = newval;
                }
            case S_STRING:
            case S_INT:
                environment = false;
                for_all_properties(sym, prop, P_ENV)
                if (prop)
                    environment = true;
                
                if ((!(sym->flags & SYMBOL_WRITE) && !environment)) {
                    fprintf(configuration, "not %s\n", sym->name);
                    free(value);
                    break;
                }
                if (!strcmp(value, ""))
                    fprintf(configuration, "%s_EQ_EMPTY\n", sym->name);
                else
                    fprintf(configuration, "%s_EQ_%s\n", sym->name, value);
                free(value);
                break;
            case S_UNKNOWN:
            case S_OTHER:
                break;
        }
    }
}
int main(int ac, char **av)
{
	const char *progname = av[0];
	int opt;
	const char *name, *defconfig_file = NULL /* gcc uninit */;
	struct stat tmpstat;
	struct timeval now;
        unsigned int seed;
        char *seed_env;
   
        /*
         * Use microseconds derived seed,
         * compensate for systems where it may be zero
         */
    	gettimeofday(&now, NULL);
    	seed = (unsigned int)((now.tv_sec + 1) * (now.tv_usec + 1));
  	seed_env = getenv(PRODUCT_ENV"_SEED");
    	if(seed_env && *seed_env ) {
        	char *endp;
        	int tmp = (int)strtol(seed_env, &endp, 0);
        	if (*endp == '\0') {
            	seed = tmp;
        	}
        }
    	fprintf( stderr, PRODUCT_ENV"_SEED=0x%X\n", seed );
    	srand(seed);
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	tty_stdio = isatty(0) && isatty(1) && isatty(2);

	while ((opt = getopt_long(ac, av, "s", long_opts, NULL)) != -1) {
		if (opt == 's') {
			conf_set_message_callback(NULL);
			continue;
		}
		input_mode = (enum input_mode)opt;
		switch (opt) {
		case silentoldconfig:
			sync_kconfig = 1;
			break;
		case defconfig:
		case savedefconfig:
			defconfig_file = optarg;
			break;
		case randconfig:
		{
			struct timeval now;
			unsigned int seed;
			char *seed_env;

			/*
			 * Use microseconds derived seed,
			 * compensate for systems where it may be zero
			 */
			gettimeofday(&now, NULL);
			seed = (unsigned int)((now.tv_sec + 1) * (now.tv_usec + 1));

			seed_env = getenv("KCONFIG_SEED");
			if( seed_env && *seed_env ) {
				char *endp;
				int tmp = (int)strtol(seed_env, &endp, 0);
				if (*endp == '\0') {
					seed = tmp;
				}
			}
			fprintf( stderr, "KCONFIG_SEED=0x%X\n", seed );
            fprintf( stderr, "stderr %p\n", stderr);
			srand(seed);
			break;
		}
		case oldaskconfig:
		case oldconfig:
		case allnoconfig:
		case allyesconfig:
		case allmodconfig:
		case alldefconfig:
		case listnewconfig:
		case olddefconfig:
			break;
		case '?':
			conf_usage(progname);
			exit(1);
			break;
		}
	}
    if (atoi(av[optind]) > 0) {
        number = strdup(av[optind]);
        optind++;
    }
    
	if (ac == optind) {
		printf(_("%s: Kconfig file missing\n"), av[0]);
		conf_usage(progname);
		exit(1);
	}
	name = av[optind];
    // DFA 30/10/2019
    readAllSymbols();
    // END DFA
	conf_parse(name);

	//zconfdump(stdout);
	if (sync_kconfig) {
		name = conf_get_configname();
		if (stat(name, &tmpstat)) {
			fprintf(stderr, _("***\n"
				"*** Configuration file \"%s\" not found!\n"
				"***\n"
				"*** Please run some configurator (e.g. \"make oldconfig\" or\n"
				"*** \"make menuconfig\" or \"make xconfig\").\n"
				"***\n"), name);
			exit(1);
		}
	}

	switch (input_mode) {
	case defconfig:
		if (!defconfig_file)
			defconfig_file = conf_get_default_confname();
		if (conf_read(defconfig_file)) {
			printf(_("***\n"
				"*** Can't find default configuration \"%s\"!\n"
				"***\n"), defconfig_file);
			exit(1);
		}
		break;
	case savedefconfig:
	case silentoldconfig:
	case oldaskconfig:
	case oldconfig:
	case listnewconfig:
	case olddefconfig:
		conf_read(NULL);
		break;
	case allnoconfig:
	case allyesconfig:
	case allmodconfig:
	case alldefconfig:
	case randconfig:
		name = getenv("KCONFIG_ALLCONFIG");
		if (!name)
			break;
		if ((strcmp(name, "") != 0) && (strcmp(name, "1") != 0)) {
			if (conf_read_simple(name, S_DEF_USER)) {
				fprintf(stderr,
					_("*** Can't read seed configuration \"%s\"!\n"),
					name);
				exit(1);
			}
			break;
		}
		switch (input_mode) {
		case allnoconfig:	name = "allno.config"; break;
		case allyesconfig:	name = "allyes.config"; break;
		case allmodconfig:	name = "allmod.config"; break;
		case alldefconfig:	name = "alldef.config"; break;
		case randconfig:	name = "allrandom.config"; break;
		default: break;
		}
		if (conf_read_simple(name, S_DEF_USER) &&
		    conf_read_simple("all.config", S_DEF_USER)) {
			fprintf(stderr,
				_("*** KCONFIG_ALLCONFIG set, but no \"%s\" or \"all.config\" file found\n"),
				name);
			exit(1);
		}
		break;
	default:
		break;
	}

	if (sync_kconfig) {
		if (conf_get_changed()) {
			name = getenv("KCONFIG_NOSILENTUPDATE");
			if (name && *name) {
				fprintf(stderr,
					_("\n*** The configuration requires explicit update.\n\n"));
				return 1;
			}
		}
		valid_stdin = tty_stdio;
	}

	switch (input_mode) {
	case allnoconfig:
		conf_set_all_new_symbols(def_no);
		break;
	case allyesconfig:
		conf_set_all_new_symbols(def_yes);
		break;
	case allmodconfig:
		conf_set_all_new_symbols(def_mod);
		break;
	case alldefconfig:
		conf_set_all_new_symbols(def_default);
		break;
	case randconfig:
		/* Really nothing to do in this loop */
		while (conf_set_all_new_symbols(def_random)) ;
		break;
	case defconfig:
		conf_set_all_new_symbols(def_default);
		break;
	case savedefconfig:
		break;
	case oldaskconfig:
		rootEntry = &rootmenu;
		conf(&rootmenu);
		input_mode = silentoldconfig;
		/* fall through */
	case oldconfig:
	case listnewconfig:
	case olddefconfig:
	case silentoldconfig:
		/* Update until a loop caused no more changes */
		do {
			conf_cnt = 0;
			check_conf(&rootmenu);
		} while (conf_cnt &&
			 (input_mode != listnewconfig &&
			  input_mode != olddefconfig));
		break;
	}

	if (sync_kconfig) {
		/* silentoldconfig is used during the build so we shall update autoconf.
		 * All other commands are only used to generate a config.
		 */
		if (conf_get_changed() && conf_write(NULL)) {
			fprintf(stderr, _("\n*** Error during writing of the configuration.\n\n"));
			exit(1);
		}
		if (conf_write_autoconf()) {
			fprintf(stderr, _("\n*** Error during update of the configuration.\n\n"));
			return 1;
		}
	} else if (input_mode == savedefconfig) {
		if (conf_write_defconfig(defconfig_file)) {
			fprintf(stderr, _("n*** Error while saving defconfig to: %s\n\n"),
				defconfig_file);
			return 1;
		}
	} else if (input_mode != listnewconfig) {
        traverseAllSymbols();
		if (conf_write(NULL)) {
			fprintf(stderr, _("\n*** Error during writing of the configuration.\n\n"));
			exit(1);
		}
	}
	return 0;
}

/*
 * Helper function to facilitate fgets() by Jean Sacren.
 */
void xfgets(char *str, int size, FILE *in)
{
	if (fgets(str, size, in) == NULL)
		fprintf(stderr, "\nError in reading or end of file.\n");
}
