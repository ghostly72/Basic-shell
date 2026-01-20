#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        printf("%s: not found\n",cmd_name);
      }
    }

    else printf("%s: command not found\n",str);
  }
  

  return 0;
}
