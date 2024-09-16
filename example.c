#include "./include/dprol.h"

#include <stdio.h>

void program_option_init(int argc, char *argv[]) {

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
  { "t", "tries", "NUMBER", "number of trials before failed", 2 },
  { DPROL_NO_KEY, "speed-limit", "NUMBER", "limit download spped", 2 },

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
  "A program", wget_option,
  wget_subcommand
};


  print_subcommand(&dprol_wget);
  print_options(&dprol_wget);
}

int main(int argc, char *argv[]) {
  program_option_init(argc, argv);
  return 0;
}

