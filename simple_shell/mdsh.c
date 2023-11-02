#include "main.h"

/**
 * main - tests the custom getline function
 * @argc: counter
 * @argv: vector
 * @env: env
 * Return: 0
 */
int main(__attribute__((unused)) int argc,
		__attribute__((unused)) char *argv[], char *env[])
{
	char **args, *line = NULL;
	size_t len = 0;
	ssize_t n_read = 0;
	int status, retval, active_state = RUNNING;
	pid_t pid;

	while (active_state == RUNNING)
	{
		printf("mdsh$ ");
		fflush(stdout);

		n_read = _getline(&line, &len, STDIN_FILENO);
		if (n_read == -1)
		{
			perror("_getline");
			return (-1);
		}

		if (n_read == 0)
		{
			printf("\n");
			safe_free(line);
			active_state = 0;
			/* most definitely Ctrl+D or Ctrl+C was received */
			return (0);
		}
		if (n_read == 1 && *line == '\0')
			continue; /* skip normal ENTER keys */

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (-1);
		}
		if (!pid && n_read > 0)
		{
			args = _strtok(line, NULL);
			retval = execve(args[0], args, env);
			if (retval == -1)
			{
				perror("execve");
				continue;
			}
			safe_free(args);
		}
		else
			wait(&status); /* wait for the child process */

		safe_free(line);
		len = 0;
	}
	safe_free(line);

	return (0);
}
