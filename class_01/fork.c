#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main ()
{
	pid_t pid;
	/* fork a child process */
	pid = fork();

	if ( pid < 0 ) { /* error occured */
		fprintf ( stderr, "Fork failed" );
		return 1;
	} else if ( pid == 0 ) { /* child process */
		execlp ( "/bin/ls", "ls", NULL );
	} else { /* parent process */
		wait ( NULL );
		printf ( "Child Completed" );
	}

	return 0;
}

