#ifndef __DPROL_H__
#define __DPROL_H__

#include <stdio.h>
#include <string.h>

int dprol_key_space = 7;
int dprol_long_key_space = 28;
int dprol_tab_space = 4;

#define DPROL_UNKNOWN_ARG -1

#define DPROL_NULL_KEY "______________DPROL_NULL_KEY____________"
#define DPROL_NO_KEY "_________DPROL_NO_KEY________"

struct dprol_command {
  char *key, *long_key, *argument_description, *command_description;
  int group;
};
size_t dprol_command_len(struct dprol_command* ptr) {
  struct dprol_command* end = ptr;
  while(strcmp(end->key, DPROL_NULL_KEY))
    ++end;
  return end - ptr;
}

struct dprol {
  char* description;
  struct dprol_command *command;
  struct dprol *child_command, *parent;
  char **group_description, **child_command_description;
};

void print_options(struct dprol* dprol) {
  struct dprol_command *command = dprol->command;
  size_t ncommand = dprol_command_len(command);
  for(size_t i = 0; i < ncommand; ++i) {
    if(i == 0 || command[i].group != command[i - 1].group) {
      puts(dprol->group_description[ command[i].group ]);
    }

    if(strcmp(command[i].key, DPROL_NO_KEY) == 0)
      printf("%*s", dprol_key_space, "");
    else {
      static char key_buf[100];
      sprintf(key_buf, "-%s,", command[i].key);
      int key_len = strlen(key_buf);
      int left_space = (dprol_key_space - key_len) >> 1;
      int right_space = dprol_key_space - key_len - left_space;
      printf("%*s%s%*s", left_space, "", key_buf, right_space, "");
    }

    {
      static char long_key_buf[100];
      int len;

      sprintf(long_key_buf, "--%s%n", command[i].long_key, &len);
      if(command[i].argument_description)
        sprintf(long_key_buf + len, "=%s%n", command[i].argument_description, &len);

      if(len < dprol_long_key_space)
        printf("%-*s", dprol_long_key_space, long_key_buf);
      else
        printf("%s%*s", long_key_buf, dprol_tab_space, "");
    }

    printf("%s\n", command[i].command_description);
  }
}

void parseOpt(int argc, char *argv[], int (*parseFunc)(char, void*)) {
}

#endif
