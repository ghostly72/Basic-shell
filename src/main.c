#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
// #include<windows.h>


int parse_args(char *str, int start, char *args[], int max_args) {
    int pos = start;
    int argc = 0;
    int i = 0;
    char tok[512];
    int in_single = 0, in_double = 0;

    while (str[pos] != 0) {
        char c = str[pos];

        if (c == '\'' && !in_double) {
            in_single = !in_single; // toggle single-quote mode
        }
        else if (c == '"' && !in_single) {
            in_double = !in_double; // toggle double-quote mode
        }
        else if (c == '\\') {
            if (in_single) {
                // Inside single quotes → take backslash literally
                tok[i++] = '\\';
            } else {
                // Outside or inside double quotes
                // pos++;
                // if (str[pos] == 0) break;
                // // if (in_double && (str[pos] == '"' || str[pos] == '\\'))
                // //     tok[i++] = str[pos];
                // // else
                //     tok[i++] = str[pos];

                 pos++;
                if (str[pos] == 0) break;
                // inside double quotes → only escape \ and "
                if (in_double && (str[pos] == '"' || str[pos] == '\\'))
                    tok[i++] = str[pos];
                else if (!in_single)
                    tok[i++] = str[pos];
                else
                    tok[i++] = '\\';
            }
        }
        //else if (c == '\\') {
        //     pos++;
        //     if (str[pos] == 0) break;
        //     // inside double quotes → only escape \ and "
        //     if (in_double && (str[pos] == '"' || str[pos] == '\\'))
        //         tok[i++] = str[pos];
        //     else if (!in_single)
        //         tok[i++] = str[pos];
        //     else
        //         tok[i++] = '\\'; // literal backslash in single quotes
        // }
        else if (isspace(c) && !in_single && !in_double) {
            if (i > 0) {
                tok[i] = '\0';
                args[argc++] = strdup(tok);
                i = 0;
                if (argc >= max_args - 1) break;
            }
            while (isspace(str[pos + 1])) pos++;
        }
        else {
            tok[i++] = c;
        }

        pos++;
    }

    if (i > 0) {
        tok[i] = '\0';
        args[argc++] = strdup(tok);
    }

    args[argc] = NULL;
    return argc;
}


// int parse_args(char *str, int start, char *args[], int max_args,char ch) {
//     int pos = start;
//     int flag = 0;
//     int argc = 0;
//     char tok[256];
//     int i = 0;

//     while (str[pos] != 0) {
//         char c = str[pos];

//         if ( ch == '\'' || ch == '"') {
//             if(c == ch)flag = !flag;
//             else{
//               pos++;
//               while(str[pos+1]!=c){
//                 tok[i++]=str[pos++];
//               }
//               tok[i++]=pos;
//             }
//         } 
//         else if (c == '\\' && !flag) {
//             // backslash outside quotes → escape next char
//             pos++;
//             if (str[pos] != 0) {
//                 tok[i++] = str[pos];
//             }
//         }
//         else if (isspace(c) && !flag) {
//             if (i > 0) {
//                 tok[i] = 0;
//                 args[argc++] = strdup(tok);
//                 i = 0;
//                 // if (argc >= max_args - 1) break;
//             }
//             while (isspace(str[pos + 1])) pos++;  // collapse spaces
//         } 
//         else {
//             if(ch=='"' && flag && c=='\\' )pos++;
//             tok[i++] = str[pos];
//         }
//         pos++;
//     }

//     if (i > 0) {
//         tok[i] = 0;
//         args[argc++] = strdup(tok);
//     }

//     args[argc] = NULL;
//     return argc;
// }


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

    else if (!strncmp("echo", str, 4)) {
      char *args[20];
      char ch=' ';int st=-1;
      if(str[5]=='\'' || str[5]=='"')ch=str[5];
      // if(str[5]=='\'')ch='\'';
      // else if(str[5]=='"')ch='"';
      // for(int i=0;i<100;i++){
      //   if(str[i]=='\''){ch='\'';st=i;break;}
      //   else if(str[i]=='"'){ch='"';st=i;break;}
      // }
      
      int argc = parse_args(str, 5, args, 20);
      for (int i = 0; i < argc; i++) {
          printf("%s", args[i]);
          if (i < argc - 1) printf(" ");
      }
      printf("\n");
    } 
    else if (!strncmp("cat", str, 3)) {
        char *args[20];
        args[0] = "cat";
        char ch=' ';int st=-1;
        if(str[5]=='\'' || str[5]=='"')ch=str[5];
      // if(str[4]=='\'')ch='\'';
      // else if(str[4]=='\'') ch='"';
      // for(int i=0;i<100;i++){
      //   if(str[i]=='\''){ch='\'';st=i;break;}
      //   else if(str[i]=='"'){ch='"';st=i;break;}
      // }
      
        int argc = parse_args(str, 4, &args[1], 19);
        // If you call parse_args(..., args, ...) then args[0] inside the function writes into args[0] in the caller.
        // If you call parse_args(..., &args[1], ...) then args[0] inside the function writes into args[1] in the caller, args[1] inside writes into args[2] in the caller, and so on.
        // That’s exactly why you sometimes pass &args[1]: you want the parser to fill the array starting at index 1 so that the caller can preset args[0] (for example to the program name "cat").
        args[argc + 1] = NULL;
        pid_t pid = fork();
        if (pid == 0) {
            execvp("cat", args);
            // exec replaces the current process image with a new program.
            // That means:
              // The process ID (PID) stays the same,
              // But the entire code, stack, and memory of your program (the shell child)
              // gets replaced by the new program (cat in this case).
              //so the below error statements r never reached
            perror("execvp failed");
            exit(1);
        } else { 
            wait(NULL);//wait()- it blocks the parent until one of its children finishes.
        }
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
      int argc = 0;

      int pos = 0;
      while (isspace(str[pos])) pos++;

      // Parse executable name
      char prog[256];
      int i = 0;
      if (str[pos] == '\'' || str[pos] == '"') {
          char quote = str[pos++];
          // prog[i++] = str[pos++];
          while (str[pos] != 0 && str[pos] != quote) {
              prog[i++] = str[pos++];
          }
          prog[i] = '\0';
          if (str[pos] == quote) pos++; // skip closing quote
      } else {
          while (str[pos] != 0 && !isspace(str[pos])) {
              prog[i++] = str[pos++];
          }
          prog[i] = '\0';
      }

      args[argc++] = strdup(prog);

      // Parse remaining arguments (can reuse your parse_args here)
      while (isspace(str[pos])) pos++;
      int argstart = pos;
      int extra = parse_args(str, argstart, &args[argc], 19);
      argc += extra;

      args[argc] = NULL;// execv() function in C is used to replace the current process image with a new process image. 
      //It takes two parameters: the path to the executable file and a null-terminated array of string arguments. 
      //that's y we terminate this with null


      // Search PATH
      char *path = getenv("PATH");
      char *copy = strdup(path);//y do we need to make a copy of path??
      char *dir = strtok(copy, ":");
      int found = 0;

      while (dir != NULL) {
          char full_path[512];
          snprintf(full_path, sizeof(full_path), "%s/%s", dir, args[0]);
          if (access(full_path, X_OK) == 0) {
              found = 1;
              pid_t pid = fork();
              if (pid == 0) {
                  execv(full_path, args);// arg[0] is the name of th excutable that we extrcted from insifr the wuotes
                  //in that case, how can we pass the args array simply as an array of argumernts in execv?

                  perror("execv failed");
                  exit(1);
              } else {
                  wait(NULL);
              }
              break;
          }
          dir = strtok(NULL, ":");
      }
      if (!found)
          fprintf(stderr, "%s: command not found\n", args[0]);
      free(copy);

    }
  
    
  }return 0;
}


// char* func(char str[],int ind,int strt){
//     int pos=strt,flag=0;
//     char* args[20];int argc=0;
//     char tok[100];int i=0;
//     while(str[pos]!=0){
//       char c=str[pos];
//       if(c=='\''){
//         flag=!flag;
//       }
//       else if(isspace(c) && !flag){
//         if(i>0){
//           tok[i]=0;
//           args[argc]=strdup(tok);argc+=1;
//           args[argc++]=" ";
//           i=0;
//         }
//         while(pos+1<ind && isspace(str[pos+1]))pos++;
//       }
//       else{
//         tok[i++]=str[pos];
//       }
//       pos++;
//     }
//     if(i>0){
//       tok[i]=0;
//       args[argc++]=strdup(tok);
//     }
//     char res[100];
//     strcpy(res,args[0]);
//     for(int i=1;i<argc;i++){
//       strncat(res,args[i],strlen(args[i]));
//     }
//     return res;
// }
