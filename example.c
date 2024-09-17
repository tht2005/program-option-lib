#include "dprol.h"

#include <stdio.h>

struct info_t {
  int a, b, c;
};

void printHelp(struct dprol* dprol) {
  printf("Help:\n"); 
  dprol_print_subcommand(dprol);
}

int parseFunc(int op, char *val, struct parse_data_t* parseInfo, void* infoPtr) {
  struct info_t *data = (struct info_t *)infoPtr;
  switch(op) {
    case 1:
      dprol_print_version();
      exit(0);
    case 2:
      printHelp(parseInfo->dprol);
      exit(0);
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
  { "e", "execute", "COMMAND", "execute a `.wgetrc'-style command" },

  { DPROL_NO_KEY, DPROL_GROUP_DESCRIPTION, 0, "\nLogging and input file:" },
  { "o", "output-file", "FILE", "log messages to FILE" },
  { "a", "append-output", "FILE", "append messages to FILE" },
  { "d", "debug", 0, "print lots of debug information" },
  { "q", "quiet", 0, "quiet (no output)" },
  { "v", "verbose", 0, "be verbose (default)" },
  { "nv", "no-verbose", 0, "turn off verbose, without being quiet" },
  { DPROL_NO_KEY, "report-speed", "TYPE", "output bandwidth as TYPE, TYPE can be bits" },

  { DPROL_NO_KEY, DPROL_GROUP_DESCRIPTION, 0, "\nDownload:" },
  { "t", "tries", "NUMBER", "number of trials before failed" },
  { DPROL_NO_KEY, "speed-limit", "NUMBER", "limit download spped" },

  { DPROL_NULL_KEY }
};

struct dprol_child wget_subcommand[] = {
  { DPROL_GROUP_DESCRIPTION, "\nSecure things" },
  { "rsa", "Encode data", 0, 0 },
  { "genprime", "Generate a prime", 0, 0 },

  { DPROL_GROUP_DESCRIPTION, "\nBigint operator" },
  { "add", "Add two bigint", 0, 0 },
  { "mul", "Multiply two bigint", 0, 0 },

  { NULL }
};

struct dprol dprol_wget = {
  "A program to fetch data from web servers.",
  wget_option,
  wget_subcommand
};

  if(argc == 1) {
    printHelp(&dprol_wget);
    return;
  }
  //dprol_run_subcommand(argc, argv, &dprol_wget, "./subprogram_dir");
  struct info_t obj;
  dprol_parse_opt(argc, argv, &dprol_wget, parseFunc, (void*)&obj);
}

int main(int argc, char *argv[]) {
  program_option_init(argc, argv);
  return 0;
}

