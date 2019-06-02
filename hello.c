#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
void printPrompt(){
	char hostname[1000];
	char buf[1000];
	char *dir,*t;
	char *b="~";
	struct passwd *info;  
	info=getpwuid(getuid()); 
	getcwd(buf, 1000); 
	gethostname(hostname,1000);
	t=buf+(int) strlen(info->pw_dir);
	dir=malloc(1000);
	strcpy(dir,t);
	printf("%s@%s:~%s$ ",info->pw_name,hostname,dir);
	free(dir);

}
int 
main (int argc, char **argv)
{
	printPrompt();
	return 0;
}