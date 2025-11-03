#ifndef LR_PLATFORM_H
#define LR_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define LR_COMMAND_OUTPUT_BUFFER_SIZE 2048
int RunCommandAndGetOutput(char *OutputBuffer, char *Command[])
{
    int Result = 0;
    
    int HandlesLink[2] = {};
    int WaitStatus = 0;
    pid_t ChildPID = 0;
    int Ret = 0;
    
    char *FilePath = Command[0];
    int AccessMode = F_OK | X_OK;
    Ret = access(FilePath, AccessMode);
    
    if(Ret == 0)
    {
        Ret = pipe(HandlesLink);
        if(Ret != -1)
        {
            ChildPID = fork();
            if(ChildPID != -1)
            {
                if(ChildPID == 0)
                {
                    dup2(HandlesLink[1], STDOUT_FILENO);
                    execve(Command[0], Command, 0);
                }
                else
                {
                    wait(&WaitStatus);
                    
                    Result = read(HandlesLink[0], OutputBuffer, LR_COMMAND_OUTPUT_BUFFER_SIZE);
                    if(Result == -1)
                    {
                        Result = 0;
                    }
                }
                
            }
            else
            {
                // TODO: Logging
            }
        }
        else
        {
            // TODO: Logging
        }
    }
    else
    {
        // TODO: Logging
    }
    
    return Result;
}

void RunCommand(char *Command[])
{
    int WaitStatus = 0;
    
    pid_t ChildPid = fork();
    if(ChildPid != -1)
    {
        if(ChildPid == 0)
        {
            execve(Command[0], Command, 0);
        }
        else
        {
            wait(&WaitStatus);
        }
    }
    else
    {
        // TODO: Logging
    }
}

void RunCommandInPATH(char *Command[])
{
#define NameBufferSize 1024
    char NameBuffer[NameBufferSize] = {0};
    char *ExecutableFound = 0;
    char **VarAt = __environ;
    char Search[] = "PATH=";
    int MatchedSearch = false;
    
    while(*VarAt && !MatchedSearch)
    {
        MatchedSearch = true;
        
        for(unsigned int At = 0;
            (At < ArrayCount(Search)) && (VarAt[At]);
            At++)
        {
            if(Search[At] != VarAt[0][At])
            {
                MatchedSearch = false;
            }
        }
        
        VarAt++;
    }
    
    if(MatchedSearch)
    {
        VarAt--;
        char *Scan = VarAt[0];
        while(*Scan && *Scan != '=') Scan++;
        Scan++;
        
        while((!ExecutableFound) && 
              (*Scan) && (Scan != VarAt[1]))
        {
            int Len = 0;
            while(Scan[Len] && Scan[Len] != ':' && 
                  (Scan+Len != VarAt[1])) Len++;
            
            for(unsigned int At = 0; At < NameBufferSize; At++)
            {
                NameBuffer[At] = 0;
            }
            int At;
            for(At = 0; At < Len; At++)
            {
                NameBuffer[At] = Scan[At];
            }
            NameBuffer[At++] = '/';
            
            for(char *CharAt = Command[0];
                *CharAt;
                CharAt++)
            {
                NameBuffer[At++] = *CharAt;
            }
            NameBuffer[At] = 0;
            
            int AccessMode = F_OK | X_OK;
            int Ret = access(NameBuffer, AccessMode);
            if(Ret == 0)
            {
                ExecutableFound = (char *)NameBuffer;
            }
            
            Scan += Len + 1;
        }
        
        if(ExecutableFound)
        {
            Command[0] = NameBuffer;
            RunCommand(Command);
        }
    }
}

#define RunSimpleCommandAndGetOutput(Buffer, ...) \
RunCommandAndGetOutput((Buffer), (char *[]){__VA_ARGS__, 0})
#define RunSimpleCommand(...)       RunCommand((char *[]){__VA_ARGS__, 0})
#define RunSimpleCommandInPATH(...) RunCommandInPATH((char *[]){__VA_ARGS__, 0})
    
#ifdef __cplusplus
}
#endif

#endif //LR_PLATFORM_H
