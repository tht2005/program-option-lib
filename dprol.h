#ifndef __DPROL_H__
#define __DPROL_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *dprol_bug=0, *dprol_version=0;

int dprol_key_space = 7;
int dprol_long_key_space = 28;
int dprol_tab_space = 4;

int dprol_subcommand_tab = 4;
int dprol_subcommand_space = 15;

#define DPROL_PARSE_UNKNOWN_ARG -500
#define DPROL_PARSE_OK -69
#define DPROL_PARSE_ERROR -68
#define DPROL_PARSE_COMMAND -67


#define DPROL_NULL_KEY "______________DPROL_NULL_KEY____________"
#define DPROL_NO_KEY "_________DPROL_NO_KEY________"
#define DPROL_NO_LONG_KEY "_________DPROL_NO_LONG_KEY________"
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
  char *name, *description, *bin_path;
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
        sprintf(long_key_buf + len, "%s%n", option[i].argument_description, &len);

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

void dprol_run_subcommand(int argc, char *argv[], struct dprol_child* child) {
    static char buf[1000];
    int cur = 0, tmp;
    sprintf(buf + cur, "%s %n", child->bin_path, &tmp), cur += tmp;
    for(int i = 2; i < argc; ++i) {
      sprintf(buf + cur, "%s %n", argv[i], &tmp), cur += tmp;
    }
    system(buf);
    exit(0);
}

void dprol_redirect_subcommand(int argc, char *argv[], struct dprol* dprol) {
  assert(argc > 1);
  struct dprol_child *child = dprol->child;
  size_t nchild = dprol_child_len(child);
  for(size_t i = 0; i < nchild; ++i) {
    if(strcmp(child[i].name, DPROL_GROUP_DESCRIPTION) == 0) {
      continue;
    }
    if(strcmp(argv[1], child[i].name) == 0) {
      dprol_run_subcommand(argc, argv, child + i);
    }
  }
  static char buf[1000];
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
  return DPROL_PARSE_UNKNOWN_ARG;
}
size_t dprol_match_key(struct dprol* dprol, char* str) {
  struct dprol_option* option = dprol->option;
  size_t noption = dprol_option_len(option);
  for(size_t i = 0; i < noption; ++i)
    if(strcmp(option[i].key, str) == 0)
      return i;
  return DPROL_PARSE_UNKNOWN_ARG;
}

int dprol_get_opt(struct dprol* dprol, char* arg) {
  size_t arg_len = strlen(arg);
  size_t pref_minus = 0;
  while(pref_minus < arg_len && arg[pref_minus] == '-')
    ++pref_minus;
  if(pref_minus == 0)
    return DPROL_PARSE_COMMAND;
  if(pref_minus == 1)
    return dprol_match_key(dprol, arg + 1);
  if(pref_minus == 2)
    return dprol_match_long_key(dprol, arg + 2);
  return DPROL_PARSE_UNKNOWN_ARG;
}

void dprol_unknown_option(char* opt) {
  printf("Unknown option: \"%s\".\n", opt);
  exit(1);
}
void dprol_parse_error(char* opt) {
  printf("Invalid argument for option %s.\n", opt);
  exit(1);
}

void dprol_parse_opt(int argc, char *argv[], struct dprol* dprol, int (*parseFunc)(int, char*, struct parse_data_t*, void*), void* infoPtr) {
  struct parse_data_t parseData = { argc, 1, argv, dprol };
  int *cur_arg = &parseData.cur_arg;
  for(; *cur_arg < parseData.argc; ) {
    int op = dprol_get_opt(dprol, parseData.argv[*cur_arg]);
    if(op == DPROL_PARSE_UNKNOWN_ARG)
      dprol_unknown_option(parseData.argv[*cur_arg]);

    static char optionStr[1000];
    memcpy(optionStr, argv[*cur_arg], strlen(argv[*cur_arg]) * sizeof(char));

    ++(*cur_arg);

    char* value = 0;
    if(op != DPROL_PARSE_COMMAND && dprol->option[op].argument_description) {
      if(*cur_arg >= parseData.argc)
        dprol_parse_error(optionStr);
      value = parseData.argv[ (*cur_arg)++ ];
    }

    int ret = parseFunc(op, value, &parseData, infoPtr);
    if(ret == DPROL_PARSE_ERROR)
      dprol_parse_error(optionStr);
  }
}

char *dprol_program_name_format(char* name) {
  char *ptr = name + strlen(name);
  while(ptr != name && *ptr != '/')
    --ptr;
  if(*ptr == '/')
    ++ptr;
  return ptr;
}

void dprol_print_usage(char* prog_name, struct dprol* dprol, int itemEachLine) {
  assert(itemEachLine > 0);

  static char prefix[1000];
  sprintf(prefix, "Usage: %s", dprol_program_name_format(prog_name));
  int prefix_len = strlen(prefix);
  int cnt = 0;

  struct dprol_child* child = dprol->child;
  size_t nchild = dprol_child_len(child);

  if(nchild > 0) {
    static const char *str[] = { "<command>", "[<args>]" };
    const size_t nstr = sizeof(str) / sizeof(*str);
    for(size_t i = 0; i < nstr; ++i) {
      if(cnt % itemEachLine == 0) {
        if(cnt == 0)
          printf("%s", prefix);
        else
          printf("%*s", prefix_len, "");
      }

      printf(" %s", str[i]);

      if(++cnt % itemEachLine == 0)
        putchar('\n');
    }
  }

  struct dprol_option* option = dprol->option;
  size_t noption = dprol_option_len(option);

  for(size_t i = 0; i < noption; ++i) {
    if(strcmp(option[i].long_key, DPROL_GROUP_DESCRIPTION) == 0)
      continue;

    if(cnt % itemEachLine == 0) {
      if(cnt == 0)
        printf("%s", prefix);
      else
        printf("%*s", prefix_len, "");
    }

    static char buf[1000];
    int tmp;
    char *ptr = buf;

    if(strcmp(option[i].key, DPROL_NO_KEY) != 0) {
      sprintf(ptr, "-%s%n", option[i].key, &tmp), ptr += tmp;
      if(option[i].argument_description)
        sprintf(ptr, "%s%n", option[i].argument_description, &tmp), ptr += tmp;
    }
    if(strcmp(option[i].long_key, DPROL_NO_LONG_KEY) != 0) {
      if(ptr != buf)
        sprintf(ptr, " | %n", &tmp), ptr += tmp;
      sprintf(ptr, "--%s%n", option[i].long_key, &tmp), ptr += tmp;
      if(option[i].argument_description)
        sprintf(ptr, "%s%n", option[i].argument_description, &tmp), ptr += tmp;
    }

    printf(" [%s]", buf);

    if(++cnt % itemEachLine == 0)
      putchar('\n');
  }
  
  if(cnt % itemEachLine)
    putchar('\n');
}

#endif
