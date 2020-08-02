/* This program will take in command line arguments.
 * The arguments are treated as file names,
 * and the program will display the information of
 * the files if they are found in the current directory.
 *
 * If no argument is provided, the program will display
 * the file information of all the files in the current directory.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

#define BUF_SIZE 128	// Buffer size

// Define functions
void displayFileInfo(char *fileName);
char *getFileType(mode_t mode);
char *getAccessPermission(mode_t mode, char *permission);
char *formatDate(time_t time, char *buf);

// Main function accepts command line arguments and display relevant information
int main(int argc, char *argv[])
{
	DIR *dir;		// Directory
	struct dirent *ent;	// Structure to store directory entries
	char* found[argc];	// Array to store file names with information displayed
	int count = 0;		// Counter to count number of file names found

	// Open current directory
	if((dir = opendir(".")) == NULL)
	{
		printf("Unable to open current directory\n\n");
		exit(1);
	}

	// If no argument is provided
	if(argc == 1)
	{
		while((ent = readdir(dir)) != NULL)
		{
			// Ignore previous directory and current directory file
			if(strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, ".") != 0)
			{
				// Display information for all files in current directory
				displayFileInfo(ent->d_name);
			}
		}
	}
	else
	{
		while((ent = readdir(dir)) != NULL)
		{
			for(int i=1; i<argc; i++)
			{
				// Display only files with the same name as argument
				if(strcmp(ent->d_name, argv[i]) == 0)
				{
					displayFileInfo(argv[i]);

					// Add arguments displayed into array
					found[count] = argv[i];
					count++;
				}
			}
		}

		// Display file names that were not found
		for(int i=1; i<argc; i++)
		{
			int counter = 0;  // Counter to count number of times file is found

			for(int j=0; j<count; j++)
			{
				// If argument is in 'found' array
				if(strcmp(argv[i], found[j]) == 0)
				{
					counter++;
				}
			}

			// Display not found message if counter is 0
			if(counter == 0)
			{
				printf("File %s not found.\n\n", argv[i]);
			}
		}
	}

	closedir(dir);  // Close directory
	exit(0);
}

// Function to obtain and display file information
void displayFileInfo(char *fileName)
{
	struct stat fileInfo;		// Information of file
	struct passwd *pw;		// Password file of file
	struct group *gr;		// Group file of file
	char permission[BUF_SIZE] = "";	// Permission of file
	char aTime[BUF_SIZE];		// Last access time of file
	char mTime[BUF_SIZE];		// Last modification time of file
	char cTime[BUF_SIZE];		// Last time file status changed

	stat(fileName, &fileInfo);	// Getting information of file
	pw = getpwuid(fileInfo.st_uid);	// Getting password file
	gr = getgrgid(fileInfo.st_gid);	// Getting group file

	// Displaying information of file
	printf("File name: \t\t\t%s\n", fileName);
	printf("User name of the owner owner: \t%s\n", pw->pw_name);
	printf("Group name of the group owner: \t%s\n", gr->gr_name);
	printf("Type of file: \t\t\t%s\n", getFileType(fileInfo.st_mode));
	printf("Full access permissions: \t%s\n", getAccessPermission(fileInfo.st_mode, permission));
	printf("Size of file: \t\t\t%lld bytes\n", (long long)fileInfo.st_size);
	printf("I-node number: \t\t\t%ld\n", (long)fileInfo.st_ino);
	printf("Major device number: \t\t%lx\n", (long)major(fileInfo.st_dev));
	printf("Minor device number: \t\t%lx\n", (long)minor(fileInfo.st_dev));
	printf("Number of links: \t\t%ld\n", (long)fileInfo.st_nlink);
	printf("Last access time: \t\t%s\n", formatDate(fileInfo.st_atime, aTime));
	printf("Last modification time: \t%s\n", formatDate(fileInfo.st_mtime, mTime));
	printf("Last time file status changed: \t%s\n", formatDate(fileInfo.st_ctime, cTime));
	printf("\n");
}

// Function to obtain file type
char *getFileType(mode_t mode)
{
	char *type;

	if(S_ISREG(mode))
	{
		type = "regular file";
	}
	else if(S_ISDIR(mode))
	{
		type = "directory";
	}
	else if(S_ISCHR(mode))
	{
		type = "character special";
	}
	else if(S_ISBLK(mode))
	{
		type = "block special";
	}
	else if(S_ISFIFO(mode))
	{
		type = "fifo";
	}
	else if(S_ISLNK(mode))
	{
		type = "symbolic link";
	}
	else if(S_ISSOCK(mode))
	{
		type = "socket";
	}
	else
	{
		type = "**unknown file type";
	}

	return type;
}

// Function to obtain access permission of file
char *getAccessPermission(mode_t mode, char *permission)
{
	if(mode & S_IRUSR)
	{
		strcat(permission, "r");
	}
	else
	{
		strcat(permission, "-");
	}
	if(mode & S_IWUSR)
	{
		strcat(permission, "w");
	}
	else
	{
		strcat(permission, "-");
	}
	if(mode & S_IXUSR)
	{
		strcat(permission, "x");
	}
	else
	{
		strcat(permission, "-");
	}
	if(mode & S_IRGRP)
	{
		strcat(permission, "r");
	}
	else
	{
		strcat(permission, "-");
	}
	if(mode & S_IWGRP)
	{
		strcat(permission, "w");
	}
	else
	{
		strcat(permission, "-");
	}
	if(mode & S_IXGRP)
	{
		strcat(permission, "x");
	}
	else
	{
		strcat(permission, "-");
	}
	if(mode & S_IROTH)
	{
		strcat(permission, "r");
	}
	else
	{
		strcat(permission, "-");
	}
	if(mode & S_IWOTH)
	{
		strcat(permission, "w");
	}
	else
	{
		strcat(permission, "-");
	}
	if(mode & S_IXOTH)
	{
		strcat(permission, "x");
	}
	else
	{
		strcat(permission, "-");
	}

	return permission;
}

// Function to convert time to 'ls' time format
char *formatDate(time_t time, char *buf)
{
	strftime(buf, BUF_SIZE, "%b %d %R", localtime(&time));
	return buf;
}
