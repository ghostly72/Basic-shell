#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1){
    printf("$ ");
    char str[100];
    char *cmd[]={"type","exit","echo"};
    fgets(str,sizeof(str),stdin);
    str[strcspn(str,"\n")]=0;
    if(strcmp("exit",str)==0)break;

    else if(strncmp("echo",str,4)==0){
      int len=strlen(str);
      len-=5;
      printf("%.*s\n",len,str+5);
    }

    else if(strncmp("type",str,4)==0){
      int found=0;
      char* cmd_name=str+5;
      for(int i=0;i<3;i++){
        if(strcmp(cmd_name,cmd[i])==0){
          printf("%s is a shell builtin\n",cmd_name);
          found=1;break;
        }
      }
      if(!found){
        char *path_env = getenv("PATH"); 
        if (path_env) {
          char *path_copy = strdup(path_env); //strdup uses dynamic memory allocation like calloc and malloc, thats y we need to free this memory block at the end
          char *dir = strtok(path_copy, ":"); 
          while (dir != NULL) { 
            char full_path[256]; 
            snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd_name); 
            if (access(full_path, X_OK) == 0) { 
              printf("%s is %s\n", cmd_name, full_path); 
              found = 1; 
              break; 
            } 
            dir = strtok(NULL, ":");
          } 
          free(path_copy);
          if(!found) printf("%s: not found\n",cmd_name);
        }
      }
      // else printf("%s: not found\n",str);
    }
    else printf("%s: not found\n",str);
}
  

  return 0;
}
