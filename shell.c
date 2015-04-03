#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/types.h>

int
main(int argc, char *argv[])
{


 char command[100];
 int pid, status;
    while(1){	
	printf(">>");
	scanf("%[^\n]",command);
	pid = fork();
	if (pid!=0) {
		int l;
		waitpid(pid, &status, WUNTRACED);
		getchar();
	}
	else {
		int i = strlen(command);
		int count = 0;
		int index = 0;
		char arg[64][64];
		int j;
		int p = 0;
		int state = 0;
		int space = 0;
		for(j=0;j<i;j++)
		{
			if(command[j]==' ')
			{
				if(space != 0)
				{
					arg[count][index]='\0';
					count ++;
				}
				index = 0;
				space = 0;
				continue;
			}
			else
			{
				if(command[j]=='<')
				{
					if(space == 1)
					{
						arg[count][index]='\0';
						count++;
					}
					state = 1;
					break;
				}
				if(command[j]=='>')
				{
					if(space == 1)
					{
						arg[count][index]='\0';
						count++;
					}
					state = -1;
					if(command[j+1]=='>')
					{
						state = -2;
						j++;
					}
					break;
				}
				space = 1;
				arg[count][index++]=command[j];
			}
		}
		if(state == 0)
		{
			arg[count][index++]='\0';
			count = count + 1;
		}
		else if(state == 1)
		{
			char in[100];
			j++;
			while(command[j]==' ')
				j++;
			int inde = 0;
			while(command[j]!=' ' && command[j]!='\0')
			{
				in[inde++] = command[j];
				j++;
			}
			in[inde++]='\0';
			int input = open(in,O_RDONLY);
			dup2(input,0);
			close(input);
			while(command[j]==' ')
				j++;
			if(command[j]=='>')
			{
				state = -1;
				if(command[j+1]=='>')
				{
					j++;
					state = -2;
				}
			}
				
		}
		if(state == -1)
		{
			char out[100];
			j++;
			while(command[j]==' ')
				j++;
			int ende = 0;
			while(command[j]!=' ' && command[j]!='\0')
			{
				out[ende++] = command[j];
				j++;
			}
			out[ende++]='\0';
			int output;
			output = open(out, O_WRONLY | O_CREAT, S_IRWXU);
			dup2(output,1);
			close(output);
		}
		if(state == -2)
		{
			char out[100];
			j++;
			while(command[j]==' ')
				j++;
			int ende = 0;
			while(command[j]!=' ' && command[j]!='\0')
			{
				out[ende++] = command[j];
				j++;
			}
			out[ende++]='\0';
			int output;
			output = open(out,O_APPEND | O_WRONLY | O_CREAT, S_IRWXU);
			dup2(output,1);
			close(output);
		}
		char * a[count];
		for(j=0;j<count;j++)
		{
			a[j]=strdup(arg[j]);
		}
		a[j]=NULL;
        execvp(a[0],a);  
	}
   }
}
