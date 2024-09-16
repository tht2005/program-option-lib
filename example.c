#include "./include/dprol.h"

#include <stdio.h>

struct dprol_command command[] = {
  { "V", "version", 0, "display the version of Wget and exit", 0 },
  { "h", "help", 0, "print this help", 0 },
  { "b", "background", 0, "go to background after startup", 0 },
  { "e", "execute", "COMMAND", "execute a `.wgetrc'-style command", 0 },

  { "o", "output-file", "FILE", "log messages to FILE", 1 },
  { "a", "append-output", "FILE", "append messages to FILE", 1 },
  { "d", "debug", 0, "print lots of debug information", 1 },
  { "q", "quiet", 0, "quiet (no output)", 1 },
  { "v", "verbose", 0, "be verbose (default)", 1 },
  { "nv", "no-verbose", 0, "turn off verbose, without being quiet", 1 },
  { DPROL_NO_KEY, "report-speed", "TYPE", "output bandwidth as TYPE, TYPE can be bits", 1 },

  { "t", "tries", "NUMBER", "number of trials before failed", 2 },
  { DPROL_NO_KEY, "speed-limit", "NUMBER", "limit download spped", 2 },

  { DPROL_NULL_KEY }
};
char *group_description[] = {
  "\nStartup:",
  "\nLogging and input file:",
  "\nDownloads:",
  ""
};

struct dprol a = { "A program", command, 0, 0, group_description };

int main(int argc, char *argv[]) {
  print_options(&a);
  return 0;
}

