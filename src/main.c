#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <limits.h>
#include <errno.h>
// #include<windows.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while(1){
    printf("$ ");
    char str[100];
    char *cmd[]={"type","exit","echo","pwd"};
    fgets(str,sizeof(str),stdin);
    //The strcsspn function is used to find the number of characters in the given string before the 1st occurrence of a character from the defined set of characters 
    int ind=strcspn(str,"\n");
    str[ind]=0; //setting next line char as 0, so that c recognises this as the end of line
    if(strcmp("exit",str)==0)break;

    else if(strncmp("echo",str,4)==0){
      int len=strlen(str);
      len-=5;
      printf("%.*s\n",len,str+5);
    }

    else if(strncmp("type",str,4)==0){
      int found=0;
      char* cmd_name=str+5;
      for(int i=0;i<(sizeof(cmd)/sizeof(cmd[0]));i++){
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
    // system design-https://www.linkedin.com/posts/ankur-dhawan01_sde1-sde2-sde3-share-7421405307924406272-20tB?utm_source=social_share_send&utm_medium=member_desktop_web&rcm=ACoAAEHM7yIBxhW0OYnKbylC9ltkZ-XZq0uyj0c
    else if(!strncmp("pwd",str,3)){
      printf(getenv("PWD"));
      printf("\n");
    }
    else if(!strncmp("cd",str,2)){
      char* args[20];
      int cnt=0;
      char* tok=strtok(str," ");
      while(tok!=NULL){
        args[cnt++]=tok;
        tok=strtok(NULL," ");
      }
      args[cnt]=NULL;

      char *target = args[1];
      if(!strcmp(target,"~")){
        char* home=getenv("HOME");
        setenv("PWD",home,1);
      }
      else{
        char resolved[PATH_MAX];
        if (realpath(target, resolved) != NULL) {
          if (chdir(resolved) == 0) {
            setenv("PWD", resolved, 1);
            //int setenv(const char *name, const char *value, int overwrite);
            //If overwrite is non-zero (e.g., 1), the variable's value is changed to value even if it already exists.
            //If overwrite is zero (0), the variable's value is not changed if it already exists. 
          } else {
            fprintf(stderr, "cd: %s: %s\n", target, strerror(errno));
            //  strerror(error) function in C maps an integer error code
            //  (typically the value stored in the global variable errno) to a human-readable, locale-dependent error message string. 
          }
        } else {
          fprintf(stderr, "cd: %s: No such file or directory\n", target);
        }

        // char* pwd=getenv("PWD");
        // char* arr[20];cnt=0;
        // tok=strtok(pwd,"/");
        // while(tok!=NULL){
        //   arr[cnt++]=tok;
        //   tok=strtok(NULL," ");
        // }
        // arr[cnt]=NULL;
        
        // char** dir=args[1];
        // char* qr[100];int ind=0;
        // tok=strtok(*args[1],"/");
        // while(tok!=NULL){
        //   qr[ind++]=tok;
        //   tok=strtok(NULL," ");
        // }
        // qr[ind]=NULL;
        // cnt--;//decrementinf cnt to point at last word of the current pwd
        // ind=0;
        // while(qr[ind]!=NULL){
        //   if(!strcmp(qr[ind],"..")){
        //     cnt--;
        //   }
        //   else if(!strcmp(qr[ind],".")){
            
        //   }
        //   else{
        //     cnt++;
        //     arr[cnt]=qr[ind];
        //   }
        //   ind++;
        // }
        // dir=arr;
        // struct stat sb;
        // if(stat(*dir,&sb)==0 && S_ISDIR(sb.st_mode)){
        //   chdir(*dir);//affects only the internal kernel state for your process — it doesn’t touch any environment variables like PWD.
        //   //process is now “in” that directory, but the environment variable $PWD (if we print it using getenv("PWD")) still holds the old path.
        //   //thats y we need to use setenv to change the env var
        //   setenv("PWD", *dir, 1);
          
        // }else{
        //   printf("cd: %s: No such file or directory\n",*dir);
        // }
      }
    }
    else{
      char *args[20];                                               
      int arg_count = 0;                                            
      char *token = strtok(str, " ");                               
      while (token != NULL && arg_count < 20) {                      
       args[arg_count++] = token;                                  
       token = strtok(NULL, " ");                                  
      }                                                             
      args[arg_count] = NULL;

      char* path=getenv("PATH");
      char* path_copy=strdup(path);
      char *dir = strtok(path_copy, ":");                           
      char *program_name = args[0];                                 
      int found = 0;  
      while(dir!=NULL){
        char full_path[256];
        // snprintf(full_path,sizeof(full_path),"%s/%s",dir,filname);
        // if(!access(full_path,X_OK)){
        //   // char* exe[256];
        //   // while(filname!=NULL){
        //   //   snprintf(exe,sizeof(exe),"%s ",filname);
        //   //   filname=strtok(NULL," ");
        //   // }
        //   system(argv[0]);
        // }
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, program_name);                                                       
        if (access(full_path, X_OK) == 0) {                         
          found = 1;                                                
          pid_t pid = fork();                                       
          if (pid == 0) {                                           
            execv(full_path, args);                                 
            exit(1);                                                
          } else if (pid > 0) {                                     
            wait(NULL);                                             
          }                                                         
          break; 
        }    
        dir=strtok(NULL,":");
      }
      if(!found)printf("%s: not found\n",program_name);
    }
  
    
  }return 0;
}