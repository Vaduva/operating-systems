/**
 * Copies a given source tree (first argument) to given destination (second argument)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

void copy_tree(char source_tree[], char dest_tree[]) {
	DIR *dir1 = opendir(source_tree);
	struct dirent *d;
	struct stat s;

	do {
		if ((d = readdir(dir1))) {
			// Skip current and parent folder and .DS_Store files
			if (strcmp(d->d_name, ".") == 0 || 
				strcmp(d->d_name, "..") == 0 ||
				strcmp(d->d_name, ".DS_Store") == 0) {
				continue;
			}

			// Build path to source file
			char source_path[100] = "";
			strcat(source_path, source_tree);
			strcat(source_path, "/");
			strcat(source_path, d->d_name);

			// Build path to destination file
			char dest_path[100] = "";
			strcat(dest_path, dest_tree);
			strcat(dest_path, "/");
			strcat(dest_path, d->d_name);
			
			// Fetch information about source file
			if (lstat(source_path, &s) < 0) {
				printf("Error: information from source file \"%s\" could not be fetched\n", source_path);
				continue;
			}

			// Copy tree depending on source file type
			if (S_ISDIR(s.st_mode)) {
				// Create folder
				if (mkdir(dest_path, 0777) != 0) {
					printf("Error: destination folder \"%s\" could not be created\n", dest_path);
					continue;
				}

				// Recursively copy tree
				copy_tree(source_path, dest_path);
			} else if (S_ISREG(s.st_mode)) {
				int source_file = open(source_path, O_RDONLY);
				int dest_file = open(dest_path, O_CREAT | O_WRONLY | O_TRUNC, s.st_mode);

				if (source_file == -1) {
					printf("Error: source file \"%s\" could not be opened for copying\n", source_path);
					continue;
				}
				if (dest_file == -1) {
					printf("Error: destination file \"%s\" could not be created as a copy\n", dest_path);
					continue;
				}

				char byte;
				while (read(source_file, &byte, 1)) {
					if (write(dest_file, &byte, 1) == -1) {
						printf("Error: destination file \"%s\" could not be written\n", dest_path);
						continue;
					}
				}

				close(source_file);
				close(dest_file);
			} else if (S_ISLNK(s.st_mode)) {
				// TODO: symlink to what original symlink pointed to
				if (symlink(source_path, dest_path) != 0) {
					printf("Error: destination symlink \"%s\" could not be created\n", dest_path);
					continue;
				}
			}
		}
	} while (d);

	closedir(dir1);
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Not enough arguments!");
		exit(1);
	}

	copy_tree(argv[1], argv[2]);

	return 0;
}