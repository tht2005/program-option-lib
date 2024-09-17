#ifndef __DPROL_H__
#define __DPROL_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *dprol_usage=0, *dprol_bug=0, *dprol_version=0;

int dprol_key_space = 7;
int dprol_long_key_space = 28;
int dprol_tab_space = 4;

int dprol_subcommand_tab = 4;
int dprol_subcommand_space = 15;

#define DPROL_UNKNOWN_ARG -500
#define DPROL_SKIP_ARG -499

#define DPROL_PARSE_OK -488
#define DPROL_PARSE_ERROR -487

#define DPROL_NULL_KEY "______________DPROL_NULL_KEY____________"
#define DPROL_NO_KEY "_________DPROL_NO_KEY________"
#define DPROL_GROUP_DESCRIPTION "_________DPROL_GROUP_DES______________"

struct dprol_option {
  char *key, *long_key, *argument_description, *option_description;
};
size_t dprol_option_len(struct dprol_option* ptr) {
  struct dprol_option* end = ptr;
  while(strcmp(end->key, DPROL_NULL_KEY))
    ++end;
  return end - ptr;
}

struct dprol_child {
  char *name, *description, *usage;
  struct dprol_option *option;
};
size_t dprol_child_len(struct dprol_child* ptr) {
  struct dprol_child* end = ptr;
  while(end->name)
    ++end;
  return end - ptr;
}

struct dprol {
  char *description;
  struct dprol_option *option;
  struct dprol_child* child;
};

void dprol_print_usage() {
  if(dprol_usage) {
    puts(dprol_usage);
  }
  else {
    puts("Bugs: variable dprol_usage not defined!");
  }
}

void dprol_print_version() {
  if(dprol_version) {
    puts(dprol_version);
  }
  else {
    puts("Bugs: variable dprol_version not defined!");
  }
}

void dprol_print_subcommand(struct dprol* dprol) {
  struct dprol_child *child = dprol->child;
  size_t nchild = dprol_child_len(child);
  for(size_t i = 0; i < nchild; ++i) {
    if(strcmp(child[i].name, DPROL_GROUP_DESCRIPTION) == 0) {
      puts(child[i].description);
      continue;
    }

    printf("%*s%-*s%s\n", dprol_subcommand_tab, "",
        dprol_subcommand_space - dprol_subcommand_tab, child[i].name, child[i].description);
  }
  if(dprol_bug) {
    printf("\n%s\n", dprol_bug);
  }
}

void dprol_print_options(struct dprol* dprol) {
  struct dprol_option *option = dprol->option;
  size_t noption = dprol_option_len(option);
  for(size_t i = 0; i < noption; ++i) {
    if(strcmp(option[i].long_key, DPROL_GROUP_DESCRIPTION) == 0) {
      puts(option[i].option_description);
      continue;
    }

    if(strcmp(option[i].key, DPROL_NO_KEY) == 0)
      printf("%*s", dprol_key_space, "");
    else {
      static char key_buf[100];
      sprintf(key_buf, "-%s,", option[i].key);
      int key_len = strlen(key_buf);
      int left_space = (dprol_key_space - key_len) >> 1;
      int right_space = dprol_key_space - key_len - left_space;
      printf("%*s%s%*s", left_space, "", key_buf, right_space, "");
    }

    {
      static char long_key_buf[100];
      int len;

      sprintf(long_key_buf, "--%s%n", option[i].long_key, &len);
      if(option[i].argument_description)
        sprintf(long_key_buf + len, "=%s%n", option[i].argument_description, &len);

      if(len < dprol_long_key_space)
        printf("%-*s", dprol_long_key_space, long_key_buf);
      else
        printf("%s%*s", long_key_buf, dprol_tab_space, "");
    }

    printf("%s\n", option[i].option_description);
  }
}

void dprol_user_error(int argc, char *argv[], char* msg) {
  printf("%s Type \"%s --help\" for instructions.\n", msg, argv[0]);
  exit(0);
}

void dprol_run_subcommand(int argc, char *argv[], struct dprol* dprol, char* subprogram_path) {
  assert(argc > 1);
  static char buf[1000];
  struct dprol_child *child = dprol->child;
  size_t nchild = dprol_child_len(child);
  for(size_t i = 0; i < nchild; ++i) {
    if(strcmp(child[i].name, DPROL_GROUP_DESCRIPTION) == 0) {
      continue;
    }
    if(strcmp(argv[1], child[i].name) == 0) {
      for(int i = 1, cur = 0, tmp; i < argc; ++i) {
        if(i == 1 && subprogram_path) {
          sprintf(buf + cur, "%s%n", subprogram_path, &tmp);
          cur += tmp;
          if(subprogram_path[strlen(subprogram_path) - 1] != '/') {
            sprintf(buf + cur, "/");
            ++cur;
          }
        }
        sprintf(buf + cur, "%s %n", argv[i], &tmp);
        cur += tmp;
      }
      system(buf);
      return;
    }
  }
  sprintf(buf, "Invalid command \"%s\".", argv[1]);
  dprol_user_error(argc, argv, buf);
}

struct parse_data_t {
  int argc;
  int cur_arg;
  char **argv;
  struct dprol* dprol;
};

size_t dprol_match_long_key(struct dprol* dprol, char* str) {
  struct dprol_option* option = dprol->option;
  size_t noption = dprol_option_len(option);
  for(size_t i = 0; i < noption; ++i)
    if(strcmp(option[i].long_key, str) == 0)
      return i;
  return DPROL_UNKNOWN_ARG;
}
size_t dprol_match_key(struct dprol* dprol, char* str) {
  struct dprol_option* option = dprol->option;
  size_t noption = dprol_option_len(option);
  for(size_t i = 0; i < noption; ++i)
    if(strcmp(option[i].key, str) == 0)
      return i;
  return DPROL_UNKNOWN_ARG;
}

int dprol_get_opt(struct dprol* dprol, char* arg) {
  size_t arg_len = strlen(arg);
  size_t pref_minus = 0;
  while(pref_minus < arg_len && arg[pref_minus] == '-')
    ++pref_minus;
  if(pref_minus == 1)
    return dprol_match_key(dprol, arg + 1);
  if(pref_minus == 2)
    return dprol_match_long_key(dprol, arg + 2);
  return DPROL_UNKNOWN_ARG;
}

void dprol_unknown_option(char* opt) {
  printf("Unknown option: \"%s\".\n", opt);
  exit(1);
}

void dprol_parse_opt(int argc, char *argv[], struct dprol* dprol, int (*parseFunc)(int, char*, struct parse_data_t*, void*), void* infoPtr) {
  struct parse_data_t parseData = { argc, 1, argv, dprol };
  int *cur_arg = &parseData.cur_arg;
  for(; *cur_arg < parseData.argc; ) {
    size_t op = dprol_get_opt(dprol, parseData.argv[*cur_arg]);
    if(op == DPROL_UNKNOWN_ARG)
      dprol_unknown_option(parseData.argv[*cur_arg]);

    ++(*cur_arg);

    char* value = 0;
    if(dprol->option[op].argument_description) {
      if(*cur_arg >= parseData.argc) {
        printf("Expected value after argument %s.\n", parseData.argv[*cur_arg - 1]);
        exit(1);
      }
      value = parseData.argv[*cur_arg];
      ++(*cur_arg);
    }

    parseFunc(op, value, &parseData, infoPtr);
  }
}

#endif
