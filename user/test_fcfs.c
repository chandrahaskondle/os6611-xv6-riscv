#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("\n!!Usage Error: enter 4 processes order: test_fcfs [function_name] [function_name]\n\n");
		exit(-1);
	}

	for (int i = 1; i < argc; i++)
	{

		if (strcmp(argv[i], "uniq_kernel") == 0)
		{
			int pid1 = fork();
			if (pid1 == 0)
			{
				int fd = open("example.txt", 0);
				char buffer[2048];
				int bytescount = read(fd, buffer, 2048);
				uniq("", bytescount, buffer);
				close(fd);

				exit(0);
			}
		}
		else if(strcmp(argv[i], "head_kernel") == 0)
		{
			int pid2 = fork();
			if (pid2 == 0)
			{
				int fd = open("abc.txt", 0);
				char buffer[2048];
				int bytescount = read(fd, buffer, 2048);
				head(14, bytescount, buffer);
				close(fd);

				exit(0);
			}
		}
		else if(strcmp(argv[i], "uniq_user") == 0)
		{
			int pid3 = fork();
			if (pid3 == 0)
			{
				char* arguments[3] = {"uniq", "example.txt", 0};
				exec("uniq", arguments);
				exit(0);
			}
		}
		else if(strcmp(argv[i], "head_user") == 0)
		{
			int pid4 = fork();
			if (pid4 == 0)
			{
				char* arguments[3] = {"head", "abc.txt", 0};
				exec("head", arguments);
				exit(0);
			}
		}
	}

	for (int i = 1; i < argc; i++)
	{
		wait(0);
	}
	ps();

	return 0;
}
