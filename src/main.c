#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1){
    printf("$ ");
    char cmd[100];
    fgets(cmd,sizeof(cmd),stdin);
    cmd[strcspn(cmd,"\n")]=0;
    if(strcmp("exit",cmd)==0)break;
    else if(strncmp("echo",cmd,4)==0){
      int len=strlen(cmd);
      len-=5;
      printf("%.*s\n",len,cmd+5);
    }
    else printf("%s: command not found\n",cmd);
  }
  

  return 0;
}
