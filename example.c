#include "dprol.h"

#include <stdio.h>

const int USAGE_NITEM = 4;

struct info_t {
  int a, b, c;
};

void printHelp(char* prog_name, struct dprol* dprol) {
  dprol_print_usage(prog_name, dprol, USAGE_NITEM);
  printf("%s", dprol->description);
  //dprol_print_subcommand(dprol);
  dprol_print_options(dprol);
  if(dprol_bug) {
    printf("\n%s\n", dprol_bug);
  }
}

int parseFunc(int op, char *val, struct parse_data_t* parseInfo, void* infoPtr) {
  struct info_t *data = (struct info_t *)infoPtr;
  switch(op) {
    case 1:
      dprol_print_version();
      exit(0);
    case 2:
      printHelp(parseInfo->argv[0], parseInfo->dprol);
      exit(0);
    case 3:
      break;
    case DPROL_PARSE_COMMAND:
      dprol_redirect_subcommand(parseInfo->argc, parseInfo->argv, parseInfo->dprol);
    default:
      return DPROL_PARSE_ERROR;
  }
  return DPROL_PARSE_OK;
}

void program_option_init(int argc, char *argv[]) {
  dprol_version = "dprol 0.0.1";
  dprol_bug = "If you find any bug please open a pull request on tht2005/program-option-lib\nor mail to dangduong31205@gmail.com";

struct dprol_option wget_option[] = {
  { DPROL_NO_KEY, DPROL_GROUP_DESCRIPTION, 0, "\nStartup:" },
  { "V", "version", 0, "display the version of Wget and exit" },
  { "h", "help", 0, "print this help" },
  { "b", "background", 0, "go to background after startup" },
  { "e", "execute", " <command>", "execute a `.wgetrc'-style command" },

  { DPROL_NO_KEY, DPROL_GROUP_DESCRIPTION, 0, "\nLogging and input file:" },
  { "o", "output-file", "=<file>", "log messages to FILE" },
  { "a", "append-output", "=<file>", "append messages to FILE" },
  { "d", "debug", 0, "print lots of debug information" },
  { "q", "quiet", 0, "quiet (no output)" },
  { "v", "verbose", 0, "be verbose (default)" },
  { "nv", "no-verbose", 0, "turn off verbose, without being quiet" },
  { DPROL_NO_KEY, "report-speed", "=<type>", "output bandwidth as TYPE, TYPE can be bits" },

  { DPROL_NO_KEY, DPROL_GROUP_DESCRIPTION, 0, "\nDownload:" },
  { "t", "tries", "=<number>", "number of trials before failed" },
  { DPROL_NO_KEY, "speed-limit", "=<number>", "limit download spped" },

  { DPROL_NO_KEY, DPROL_GROUP_DESCRIPTION, 0, "\nTest:" },
  { DPROL_NO_KEY, "tries", "=<number>", "number of trials before failed" },
  { "tt", DPROL_NO_LONG_KEY, "=<number>", "number of trials before failed" },

  { DPROL_NULL_KEY }
};

struct dprol_child wget_subcommand[] = {
  { DPROL_GROUP_DESCRIPTION, "\nSecure things" },
  { "rsa", "Encode data", "bin/rsa" },
  { "genprime", "Generate a prime", "bin/genprime" },

  { DPROL_GROUP_DESCRIPTION, "\nBigint operator" },
  { "add", "Add two bigint", "bin/add" },
  { "mul", "Multiply two bigint", "bin/mul" },

  { NULL }
};

struct dprol dprol_wget = {
  "\nA program to fetch data from web servers.\n",
  wget_option,
  wget_subcommand
};

  if(argc == 1) {
    printHelp(argv[0], &dprol_wget);
    return;
  }
  struct info_t obj;
  dprol_parse_opt(argc, argv, &dprol_wget, parseFunc, (void*)&obj);
}

int main(int argc, char *argv[]) {
  program_option_init(argc, argv);
  return 0;
}

