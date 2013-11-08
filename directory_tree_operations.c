/**
 * Reads source and destination directories (first and second arguments)
 * and for each regular source file creates a destination symlink and
 * for each source folder outputs its file entries on screen
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Not enough arguments\n");
		printf("Program should be called: \"prog_name src_dir dest_dir\"\n");
		exit(1);
	}

	// Open source directory
	DIR *src_dir = opendir(argv[1]);
	if (!src_dir) {
		printf("Source directory could not be opened\n");
		exit(1);
	}

	// Create destination directory if it doesn't exist
	DIR *dest_dir = opendir(argv[2]);
	if (!dest_dir) {
		if (mkdir(argv[2], 0744) == -1) {
			printf("Destination directory could not be created\n");
			exit(1);
		}
	}
	closedir(dest_dir);

	struct dirent *d;
	struct stat *s = malloc(sizeof(struct stat));

	// Read source directory
	while ((d = readdir(src_dir))) {
		// Skip current and parent folder
		if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) {
			continue;
		}

		char src_path[100] = "";
		strcat(src_path, argv[1]);
		strcat(src_path, "/");
		strcat(src_path, d->d_name);

		// Fetch file information
		if (stat(src_path, s) == -1) {
			printf("Information for file \"%s\" could not be fetched\n", src_path);
			continue;
		}

		if (S_ISREG(s->st_mode)) {
			// If regular file, symlink it
			char dest_path[100] = "";
			strcat(dest_path, argv[2]);
			strcat(dest_path, "/");
			strcat(dest_path, d->d_name);

			if (symlink(src_path, dest_path) == -1) {
				if (errno == EEXIST) {
					printf("Symlink for file \"%s\" already exists\n", src_path);
				} else {
					printf("Symlink for file \"%s\" could not be created\n", src_path);
				}
				continue;
			}

			printf("Symlink for file \"%s\" has been created\n", src_path);
		} else if (S_ISDIR(s->st_mode)) {
			// If folder, print files
			DIR *sub_dir = opendir(src_path);
			if (!sub_dir) {
				printf("Subdirectory \"%s\" could not be opened\n", src_path);
				continue;
			}

			struct dirent *dd;

			printf("Files inside \"%s\" directory:\n", src_path);
			while ((dd = readdir(sub_dir))) {
				// Skip current and parent folder
				if (strcmp(dd->d_name, ".") == 0 || strcmp(dd->d_name, "..") == 0) {
					continue;
				}
				printf("    %s\n", dd->d_name);
			}
			printf("-------------\n");

			closedir(sub_dir);
		}
	}

	closedir(src_dir);
}