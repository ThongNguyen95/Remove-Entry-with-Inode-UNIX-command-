#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>

int checkInput(char * input);
void getEntry(int inode);
int removeEntry(char * filename, int inode);

int main(int ac, char * av[]) {
	
	if (ac != 2) {
		printf("SYNOPSIS: rbi INODENUMBER\n");
	} else {
		if (checkInput(av[1]) == 0)
			printf("Error: inode number is not integer!\n");
		else
			getEntry(atoi(av[1]));
	}
	return 0;
}

int checkInput(char * input) {
	int i;
	for (i = 0; i < strlen(input); i++){
		if (isdigit(input[i]) == 0)
			return 0;
	}
	return 1;
}

void getEntry(int inode) {
	DIR * ptr;
	struct dirent * direntst;
	int count = 0;
	ptr = opendir(".");
	if (ptr == NULL)
		printf("Failed to open directory!\n");
	else {
		while((direntst = readdir(ptr)) != NULL)
			count += removeEntry(direntst->d_name, inode);
		if (count == 0)
			printf("Failed to remove directory entry.\n");
		closedir(ptr);
	}
}

int removeEntry(char * filename, int inode) {
	struct stat info;
	if (stat(filename, &info) != -1) {
		if (info.st_ino == inode) {
			if ((info.st_mode & 0170000) == 0040000) {	//directory
				if (rmdir(filename) != -1)
					return 1;
			}
			else if ((info.st_mode & 0170000) == 0100000) { //regular file
				if (unlink(filename) != -1)
					return 1;
			}
		}
	}
	return 0;
}