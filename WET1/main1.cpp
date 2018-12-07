/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Winter 2018-2019                                     */
/* Homework : Wet 1                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main1.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

//Use VERBOSE for more detailed output
//#define VERBOSE
#define TIMING

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "library1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The command's strings */
typedef enum {
    NONE_CMD = -2,
    COMMENT_CMD = -1,
    INIT_CMD = 0,
    ADDWITHTIMING_CMD = 1,
    ADD_CMD = 2,
    FINDWITHTIMING_CMD = 3,
    FIND_CMD = 4,
    DELETEWITHTIMING_CMD = 5,
    DELETE_CMD = 6,
    QUIT_CMD = 7
} commandType;

static const int numActions = 8;
static const char *commandStr[] = {
        "init",
        "addWithTiming",
        "add",
        "findWithTiming",
        "find",
        "deleteWithTiming",
        "delete",
        "quit" };

static const char* ReturnValToStr(int val) {
    switch (val) {
        case SUCCESS:
            return "SUCCESS";
        case ALLOCATION_ERROR:
            return "ALLOCATION_ERROR";
        case FAILURE:
            return "FAILURE";
        case INVALID_INPUT:
            return "INVALID_INPUT";
        default:
            return "";
    }
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
    error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString,ErrorParams) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString, ErrorParams); return error; }

#define GET_TIME clock()
static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {

    char buffer[MAX_STRING_INPUT_SIZE];

    // Reading commands
    while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
        fflush(stdout);
        if (parser(buffer) == error)
            break;
    };
    return 0;
}

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
                                const char** const command_arg) {
    if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
        return (NONE_CMD);
    if (StrCmp("#", command)) {
        if (strlen(command) > 1)
            printf("%s", command);
        return (COMMENT_CMD);
    };
    for (int index = 0; index < numActions; index++) {
        if (StrCmp(commandStr[index], command)) {
            *command_arg = command + strlen(commandStr[index]) + 1;
            return ((commandType)index);
        };
    };
    return (NONE_CMD);
}

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnAdd(void* DS, const char* const command);
static errorType OnAddWithTiming(void* DS, const char* const command);
static errorType OnFind(void* DS, const char* const command);
static errorType OnFindWithTiming(void* DS, const char* const command);
static errorType OnDelete(void* DS, const char* const command);
static errorType OnDeleteWithTiming(void* DS, const char* const command);
static errorType OnQuit(void** DS, const char* const command);

/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/

static errorType parser(const char* const command) {
    static void *DS = NULL; /* The general data structure */
    const char* command_args = NULL;
    errorType rtn_val = error;

    commandType command_val = CheckCommand(command, &command_args);

    switch (command_val) {

        case (INIT_CMD):
            rtn_val = OnInit(&DS, command_args);
            break;
        case (ADD_CMD):
            rtn_val = OnAdd(DS, command_args);
            break;
        case (ADDWITHTIMING_CMD):
            rtn_val = OnAddWithTiming(DS, command_args);
            break;
        case (FIND_CMD):
            rtn_val = OnFind(DS, command_args);
            break;
        case (FINDWITHTIMING_CMD):
            rtn_val = OnFindWithTiming(DS, command_args);
            break;
        case (DELETE_CMD):
            rtn_val = OnDelete(DS, command_args);
            break;
        case (DELETEWITHTIMING_CMD):
            rtn_val = OnDeleteWithTiming(DS, command_args);
            break;
        case (QUIT_CMD):
            rtn_val = OnQuit(&DS, command_args);
            break;

        case (COMMENT_CMD):
            rtn_val = error_free;
            break;
        case (NONE_CMD):
            rtn_val = error;
            break;
        default:
            assert(false);
            break;
    };
    return (rtn_val);
}

/***************************************************************************/
/* OnInit                                                                  */
/***************************************************************************/
static errorType OnInit(void** DS, const char* const command) {
    if (isInit) {
#ifdef VERBOSE
        printf("init was already called.\n");
#endif
        return (error_free);
    };
    isInit = true;

    *DS = Init();
    if (*DS == NULL) {
#ifdef VERBOSE
        printf("init failed.\n");
#endif
        return error;
    };
#ifdef VERBOSE
    printf("init done.\n");
#endif

    return error_free;
}


/***************************************************************************/
/* OnAdd                                                                   */
/***************************************************************************/
static errorType OnAdd(void* DS, const char* const command) {
    int s,t,v;
    void *p;
    ValidateRead(sscanf(command, "%d %d %d", &s, &t, &v), 3, "%s failed.\n", commandStr[ADD_CMD]);
    for(int i = s;i <= t; i++) {
        StatusType res = Add(DS, i, &v, &p);
#ifdef VERBOSE
        printf("%s: %s\n", commandStr[ADD_CMD], ReturnValToStr(res));
#endif
    }
    return error_free;
}

/***************************************************************************/
/* OnAddWithTiming                                                         */
/***************************************************************************/
static errorType OnAddWithTiming(void* DS, const char* const command) {
    int start_size;
    Size(DS, &start_size);
    int s,t,v;
    void *p;
    ValidateRead(sscanf(command, "%d %d %d", &s, &t, &v), 3, "%s failed.\n", commandStr[ADDWITHTIMING_CMD]);
    auto start = GET_TIME;
    for(int i = s;i <= t; i++) {
        StatusType res = Add(DS, i, &v, &p);
#ifdef VERBOSE
        printf("%s: %s\n", commandStr[ADDWITHTIMING_CMD], ReturnValToStr(res));
#endif
    }
    auto stop = GET_TIME;
    auto diff = stop - start;

    int end_size;
    Size(DS, &end_size);
#ifdef TIMING
    printf("%s %d %d %d\n", commandStr[ADD_CMD], t-s+1,(start_size+end_size)/2, diff);
#endif
    return error_free;
}

/***************************************************************************/
/* OnFind                                                                  */
/***************************************************************************/
static errorType OnFind(void* DS, const char* const command) {
    int s,t;
    void* v;
    ValidateRead(sscanf(command, "%d %d", &s, &t), 2, "%s failed.\n", commandStr[FIND_CMD]);
    for(int i = s;i <= t; i++) {
        StatusType res = Find(DS, i, &v);
#ifdef VERBOSE
        printf("%s: %s\n", commandStr[FIND_CMD], ReturnValToStr(res));
#endif
    }
    return error_free;
}

/***************************************************************************/
/* OnFindWithTiming                                                        */
/***************************************************************************/
static errorType OnFindWithTiming(void* DS, const char* const command) {
    int start_size;
    Size(DS, &start_size);
    int s,t;
    void* v;
    ValidateRead(sscanf(command, "%d %d", &s, &t), 2, "%s failed.\n", commandStr[FINDWITHTIMING_CMD]);
    auto start = GET_TIME;
    for(int i = s;i <= t; i++) {
        StatusType res = Find(DS, i, &v);
#ifdef VERBOSE
        printf("%s: %s\n", commandStr[FINDWITHTIMING_CMD], ReturnValToStr(res));
#endif
    }
    auto stop = GET_TIME;
    auto diff = stop - start;

    int end_size;
    Size(DS, &end_size);
#ifdef TIMING
    printf("%s %d %d %d\n", commandStr[FIND_CMD], t-s+1,(start_size+end_size)/2, diff);
#endif
    return error_free;
}

/***************************************************************************/
/* OnDelete                                                                */
/***************************************************************************/
static errorType OnDelete(void* DS, const char* const command) {
    int s,t;
    ValidateRead(sscanf(command, "%d %d", &s, &t), 2, "%s failed.\n", commandStr[DELETE_CMD]);
    for(int i = s;i <= t; i++) {
        StatusType res = Delete(DS, i);
#ifdef VERBOSE
        printf("%s: %s\n", commandStr[DELETE_CMD], ReturnValToStr(res));
#endif
    }
    return error_free;
}


/***************************************************************************/
/* OnDeleteWithTiming                                                      */
/***************************************************************************/
static errorType OnDeleteWithTiming(void* DS, const char* const command) {
    int start_size;
    Size(DS, &start_size);
    int s,t;
    ValidateRead(sscanf(command, "%d %d", &s, &t), 2, "%s failed.\n", commandStr[DELETEWITHTIMING_CMD]);
    auto start = GET_TIME;
    for(int i = s;i <= t; i++) {
        StatusType res = Delete(DS, i);
#ifdef VERBOSE
        printf("%s: %s\n", commandStr[DELETEWITHTIMING_CMD], ReturnValToStr(res));
#endif
    }
    auto stop = GET_TIME;
    auto diff = stop - start;

    int end_size;
    Size(DS, &end_size);
#ifdef TIMING
    printf("%s %d %d %d\n", commandStr[DELETE_CMD], t-s+1,(start_size+end_size)/2, diff);
#endif
    return error_free;
}

/***************************************************************************/
/* OnQuit                                                                  */
/***************************************************************************/
static errorType OnQuit(void** DS, const char* const command) {
    Quit(DS);
    if (*DS != NULL) {
#ifdef VERBOSE
        printf("Quit failed.\n");
#endif
        return error;
    };

    isInit = false;
#ifdef VERBOSE
    printf("Quit done.\n");
#endif

    return error_free;
}

#ifdef __cplusplus
}
#endif
