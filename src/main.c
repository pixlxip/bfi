#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

int main (int argc, char *argv[]) {
	int tapeSize = 30000; // 30,000
	int cursor = 0;
	int filepos = 0;
	int useGetch = 0;
	int useTime = 0;
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
	// get configs from command line
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && argv[i][1] != '-') {
			for (int j = 1; j < strlen(argv[i]); j++) {
				if (argv[i][j] == 'g') useGetch = 1;
				else if (argv[i][j] == 't') useTime = 1;
				else printf ("Warning: '-%c': unknown flag. Ignoring.\n", argv[i][j]);
			}
			continue;
		}
		FILE *fileCheck;
		if (fileCheck = fopen(argv[i], "r")) { bfFile = argv[i]; fclose (fileCheck); }
		else if (atoi(argv[i])) tapeSize = atoi(argv[i]);
		else { printf ("Warning: '%s': unknown argument. Ignoring.\n", argv[i]); }
	}
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
		if (c == '>') right();
		else if (c == '<') left();
		else if (c == '+') plus();
		else if (c == '-') minus();
		else if (c == '.') print();
		else if (c == ',') input();
		else if (c == '[') startLoop();
		else if (c == ']') endLoop();
		else if (c == '*' && useTime == 1) sleep();
	}
	// printf("%s", filestr);
	int i = 0;
	while (filepos < strlen(filestr) && i < 1000) {
		intrchar (filestr[filepos]);
		// printf("%c", filestr[filepos]);
		i++;
	}
}
