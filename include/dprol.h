#ifndef __DPROL_H__
#define __DPROL_H__

#include <stdio.h>
#include <string.h>

char *dprol_usage=0, *dprol_bug=0, *version=0;

int dprol_key_space = 7;
int dprol_long_key_space = 28;
int dprol_tab_space = 4;

int dprol_subcommand_tab = 4;
int dprol_subcommand_space = 15;

#define DPROL_UNKNOWN_ARG -1
#define DPROL_SKIP_ARG 1
#define DPROL_OK_ARG 0

#define DPROL_NULL_KEY "______________DPROL_NULL_KEY____________"
#define DPROL_NO_KEY "_________DPROL_NO_KEY________"
#define DPROL_GROUP_DESCRIPTION "_________DPROL_GROUP_DES______________"

struct dprol_option {
  char *key, *long_key, *argument_description, *option_description;
  int group;
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

void print_subcommand(struct dprol* dprol) {
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

void print_options(struct dprol* dprol) {
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

/*
void parseOpt(int argc, char *argv[], int (*parseFunc)(char, void*)) {
}
*/

#endif
