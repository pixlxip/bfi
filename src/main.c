#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

int main (int argc, char *argv[]) {
	int tapeSize = 30000; // 30,000
	int cursor   = 0;
	int filepos  = 0;
	int useGetch = 0;
	int useTime  = 0;
	char* readFile (char* name) {
		FILE* file = fopen(name, "rb");
		if (!file) {
			printf("No file provided! Exiting.\n");
			exit(1);
		}
		fseek(file, 0, SEEK_END);
		int length = ftell(file);
		fseek(file, 0, SEEK_SET);
		char* data = malloc(length);
		fread(data, 1, length, file);
		fclose(file);
		return data;
	}
	char * bfFile;
	// default configs from bf.conf
	/* FILE * confFile;
	if (fopen(confFile)) {
		fclose(confFile);
		char * confStr = readFile("bf.conf");
		int spaces = 0;
		for (int i = 0; i < strlen(confStr); i++) {
			if (confStr[i] == ' ') spaces++;
		}
		char * options[spaces];
		for (int i = 0; i < strlen(confStr); i++) {
			if (confStr[i] != ' ') options;
		}
	} */
	void config (int optAmount, char * options[]) {
		const char * getchArg = "--getch";
		const char * timeArg  = "--time";
		for (int i = 1; i < optAmount; i++) {
			FILE *fileCheck;
			if (fileCheck = fopen(options[i], "r")) { bfFile = options[i]; 
			                                          fclose (fileCheck); }
			else if (options[i][0] == '-' && options[i][1] != '-') {
				for (int j = 1; j < strlen(options[i]); j++) {
					if (options[i][j] == 'g') useGetch = 1;
					else if (options[i][j] == 't') useTime = 1;
					else printf ("Warning: '-%c': unknown flag. Ignoring.\n", 
					             options[i][j]);
				}
				continue;
			}
			else if (options[i] == getchArg) usegetch = 1;
			else if (options[i] == timeArg) usetime = 1;
			else if (atoi(options[i])) tapeSize = atoi(options[i]);
			else { printf ("Warning: '%s': unknown argument. Ignoring.\n", options[i]); }
		}
	}
	config (argc, argv);
	int tape[tapeSize];
	memset (tape, 0, tapeSize);
	char * filestr = readFile (bfFile);
	void right () {
		if (cursor == tapeSize - 1) cursor = 0;
		else cursor++;
		filepos++;
	}
	void left () {
		if (cursor == 0) cursor = tapeSize - 1;
		else cursor--;
		filepos++;
	}
	void plus () {
		tape[cursor]++;
		filepos++;
	}
	void minus () {
		tape[cursor]--;
		filepos++;
	}
	void print () {
		printf("%c", (char) tape[cursor]);
		filepos++;
	}
	void input () {
		char scanned;
		if (!useGetch) scanf("%c", &scanned);
		else scanned = getch();
		tape[cursor] = (int) scanned;
		filepos++;
	}
	void startLoop () {
		if (tape[cursor] == 0) {
			// [: +, ]: -, if it finds a ] and the sum is 0, it found the closing ].
			int cpos = filepos;
			int sum = 1;
			int eof = 0;
			// printf("%c", filestr[cpos]);
			while (1) {
				if ( filestr[cpos] == '[' ) sum++;
				else if ( filestr[cpos] == ']' ) sum--;
				else if ( filestr[cpos] == '\0' ) eof++;
				// printf("%i: %c\n", cpos, filestr[cpos]);
				if ((sum == 0 && filestr[cpos] == ']') || eof == 1) break;	
				cpos++;
			}
			if (eof == 1) {
				printf ("Character %i: End of file reached without closing ]\n", cursor);
				exit(1);
			}
			else filepos = cpos + 1;
		} else {
			filepos++;
		}
	}
	void endLoop () {
		if (tape[cursor] != 0) {
			// ]: +, [: -, if it finds a [ and the sum is 0, it found the opening [.
			int cpos = filepos;
			int sum = 1;
			int eof = 0;
			// printf("%c", filestr[cpos]);
			while (1) {
				if ( filestr[cpos] == ']' ) sum++;
				else if ( filestr[cpos] == '[' ) sum--;
				else if ( cpos == 0 ) eof++;
				// printf("%i: %c\n", cpos, filestr[cpos]);
				if ((sum == 0 && filestr[cpos] == '[') || eof == 1) break;	
				cpos--;
			}
			if (eof == 1) {
				printf ("Character %i: Start of file reached without opening [\n", cursor);
				exit(1);
			}
			else filepos = cpos;
		} else {
			filepos++;
		}

	}
	void sleep () {
		usleep(tape[cursor] * 1000);
	}
	void intrchar (char c) {
		switch (c) {
			case '>':
				right();
				break;
			case '<':
				left();
				break;

			case '+':
				plus();
				break;

			case '-':
				minus();
				break;

			case '.':
				print();
				break;

			case ',':
				input();
				break;

			case '[':
				startLoop();
				break;

			case ']':
				endLoop();
				break;
			case '*':
				if (useTime) {
					sleep();
				}
				break;
		}
	}
	// printf("%s", filestr);
	int i = 0;
	while (filepos < strlen(filestr) && i < 1000) {
		intrchar (filestr[filepos]);
		// printf("%c", filestr[filepos]);
		i++;
	}
}
