/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "hw2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

long lookup(const char *filename);
void timedl(void);

int *
deletedl_1_svc(char **filename, struct svc_req *rqstp)
{
	static int  result;
    FILE *flog;
        
    if (lookup(*filename) < 0) {
	    printf("File %s does not exist.\n", *filename);
	    result = 2;
	    return &result;
    }

    if(remove(*filename) == -1) {
	    printf("ERROR: deleting file %s.\n", *filename);
	    result = 0;
	    return &result;
    }
    
    timedl();
    printf("delete %s\n", *filename);
    printf("File %s has been deleted.\n", *filename);
    
    flog = fopen("server.log", "a");
    fprintf(flog, "delete %s\n", *filename);
    fprintf(flog, "File %s has been deleted.\n", *filename);
    fclose(flog);

    result = 1;
	return &result;
}

int *
renamedl_1_svc(stringPair *argp, struct svc_req *rqstp)
{
	static int  result;
    char *filename = argp->filename;
    char *str = argp->str;
    FILE *fp, *flog;
    
    if (lookup(filename) < 0) {
	    if ((fp = fopen(filename, "w")) == NULL){
	        printf("ERROR: cannot create file %s.\n", filename);
	        result = 0;
	        return &result;
	    }
	    fclose(fp);
    }
    
   
    if (rename(filename, str) < 0)
    {
	    printf("ERROR: rename file %s.\n", filename);
	    result = 0;
	    return &result;
    }
    
    timedl();
    printf("rename %s %s\n", filename, str);
    printf("File %s has been renamed to %s.\n", filename, str);
    flog = fopen("server.log", "a");
    fprintf(flog, "rename %s %s\n", filename, str);
    fprintf(flog, "File %s has been renamed to %s.\n", filename, str);
    fclose(flog); 

    result = 1;
	return &result;
}


int *
savedl_1_svc(stringPair *argp, struct svc_req *rqstp)
{
	static int  result;
	int size;
    FILE *fp, *flog;
    char *filename = argp->filename;
    char *str = argp->str;

    if (lookup(filename) < 0) {
	    printf("File %s does not exist.\n", filename);
	    result = -1;
	    return &result;
    }

    fp = fopen(filename, "a");
    size = fwrite(str, sizeof(char), strlen(str), fp);
    timedl();
    printf("save %s %s\n", filename, str);
    printf("%d bytes of data have been written to the file %s.\n", size, filename);
    fclose(fp);
    
    flog = fopen("server.log", "a");
    fprintf(flog, "save %s %s\n", filename, str);
    fprintf(flog, "%d bytes of data have been written to the file %s.\n", size, filename);
    fclose(flog);
       
    result = size;
	return &result;
}


str *
headdl_1_svc(stringInt *argp, struct svc_req *rqstp)
{
	static str  result;
    int i = 0;
    FILE *fp, *flog;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char s[2048] = "";

    char *filename = argp->filename;
    int len_toread = argp->len;
	
    flog = fopen("server.log", "a");

    if ((fp = fopen(filename, "r")) == NULL){
	    //Create the file
	    if ((fp = fopen(filename, "w")) == NULL){
	        printf("ERROR: cannot creat file %s.\n", filename);
	        exit(1);
	    }
	    timedl();
	    printf("head %s %d\n", filename, len_toread);
	    printf("File %s has been created.\n", filename);
	    fprintf(flog, "head %s %d\n", filename, len_toread);
	    fprintf(flog, "File %s has been created.\n", filename);
	    fclose(fp);
        fclose(flog);
        result = "2";
        return &result;
    } 
    else {
	    while ((i < len_toread) && ((read = getline(&line, &len, fp)) != -1)) {
	        strcat(s, line);
	        i++;
	        printf("%s", line); 
	    }
    } 
    result = s;
    free(line);
    fclose(fp);
    fclose(flog);
    return &result;
}

str *
treedl_1_svc(void *argp, struct svc_req *rqstp)
{
	static str  result;
    DIR *pdir;
    struct dirent *ent;
    FILE *flog;
    char s[1024] = "", temp[100];
    
    pdir = opendir("."); // Single level file system
    if (pdir == NULL) {
	    printf("ERROR: cannot get current directory.\n");
        return 0;
    }
    timedl();
    flog = fopen("server.log", "a");
    printf("tree\n");
    printf("  .\n");
    strcat(s, "  .\n");
    fprintf(flog, "tree\n");
    fprintf(flog, "  .\n");    
    while((ent = readdir(pdir)) != NULL){
        printf("  |__%s\n", ent->d_name);
        fprintf(flog, "  |__%s\n", ent->d_name);
        sprintf(temp, "  |__%s\n", ent->d_name);
        strcat(s, temp);
    }

    result = s;

    fclose(flog);
    closedir(pdir);
    
	return &result;
}



/* assuming a long integer is enough for the file size
   return -1 on error */
long lookup(const char *filename) {
	int status;
	struct stat statbuf;

	if (stat(filename, &statbuf) == -1) {	
		return -1;
	}
	
	return (long)statbuf.st_size;
}

void timedl(void)
{
    time_t t;
	struct tm *tminfo;
    FILE *flog;
    
    flog = fopen("server.log", "a");

	time(&t);
	tminfo = localtime(&t);
	tminfo->tm_mon += 1;
	printf("[%d/%d %d:%d:%d] ", tminfo->tm_mon, tminfo->tm_mday, tminfo->tm_hour, tminfo->tm_min, tminfo->tm_sec);
	fprintf(flog, "[%d/%d %d:%d:%d] ", tminfo->tm_mon, tminfo->tm_mday, tminfo->tm_hour, tminfo->tm_min, tminfo->tm_sec);
	fclose(flog);
}
