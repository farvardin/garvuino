#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <math.h>
#include <err.h>

#include "stuff.h"

#define SETLO(v,x) v = ((v) & 0xf0) | (x)
#define SETHI(v,x) v = ((v) & 0x0f) | ((x) << 4)

int songx, songy, songoffs, songlen = 1;
int trackx, tracky, trackoffs, tracklen = TRACKLEN;
int instrx, instry, instroffs;
int currtrack = 1, currinstr = 1;
int currtab = 0;
int octave = 4;

char filename[1024];

char *notenames[] = {"C-", "C#", "D-", "D#", "E-", "F-", "F#", "G-", "G#", "A-", "A#", "H-"};

char *validcmds = "0dfijlmtvw~+=";

/*char *keymap[2] = {
	";oqejkixdbhmwnvsz",
	"'2,3.p5y6f7gc9r0l/="
};*/

char *keymap[2] = {
	"zsxdcvgbhnjm,l.;/",
	"q2w3er5t6y7ui9o0p"
};

struct instrline {
	u8			cmd;
	u8			param;
};

struct instrument {
	int			length;
	struct instrline	line[256];
};

struct songline {
	u8			track[4];
	u8			transp[4];
};

struct instrument instrument[256], iclip;
struct track track[256], tclip;
struct songline song[256];

enum {
	PM_IDLE,
	PM_PLAY,
	PM_EDIT
};
int playmode = PM_IDLE;

int hexdigit(char c) {
	if(c >= '0' && c <= '9') return c - '0';
	if(c >= 'a' && c <= 'f') return c - 'a' + 10;
	return -1;
}

int freqkey(int c) {
	char *s;
	int f = -1;

	if(c == '-' || c == KEY_DC) return 0;
	if(c > 0 && c < 256) {
		s = strchr(keymap[0], c);
		if(s) {
			f = (s - (keymap[0])) + octave * 12 + 1;
		} else {
			s = strchr(keymap[1], c);
			if(s) {
				f = (s - (keymap[1])) + octave * 12 + 12 + 1;
			}
		}
	}
	if(f > 12 * 9 + 1) return -1;
	return f;
}

void readsong(int pos, int ch, u8 *dest) {
	dest[0] = song[pos].track[ch];
	dest[1] = song[pos].transp[ch];
}

void readtrack(int num, int pos, struct trackline *tl) {
	tl->note = track[num].line[pos].note;
	tl->instr = track[num].line[pos].instr;
	tl->cmd[0] = track[num].line[pos].cmd[0];
	tl->cmd[1] = track[num].line[pos].cmd[1];
	tl->param[0] = track[num].line[pos].param[0];
	tl->param[1] = track[num].line[pos].param[1];
}

void readinstr(int num, int pos, u8 *il) {
	if(pos >= instrument[num].length) {
		il[0] = 0;
		il[1] = 0;
	} else {
		il[0] = instrument[num].line[pos].cmd;
		il[1] = instrument[num].line[pos].param;
	}
}

void savefile(char *fname) {
	FILE *f;
	int i, j;

	f = fopen(fname, "w");
	if(!f) {
		fprintf(stderr, "save error!\n");
		return;
	}

	fprintf(f, "musicchip tune\n");
	fprintf(f, "version 1\n");
	fprintf(f, "\n");
	for(i = 0; i < songlen; i++) {
		fprintf(f, "songline %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
			i,
			song[i].track[0],
			song[i].transp[0],
			song[i].track[1],
			song[i].transp[1],
			song[i].track[2],
			song[i].transp[2],
			song[i].track[3],
			song[i].transp[3]);
	}
	fprintf(f, "\n");
	for(i = 1; i < 256; i++) {
		for(j = 0; j < tracklen; j++) {
			struct trackline *tl = &track[i].line[j];

			if(tl->note || tl->instr || tl->cmd[0] || tl->cmd[1]) {
				fprintf(f, "trackline %02x %02x %02x %02x %02x %02x %02x %02x\n",
					i,
					j,
					tl->note,
					tl->instr,
					tl->cmd[0],
					tl->param[0],
					tl->cmd[1],
					tl->param[1]);
			}
		}
	}
	fprintf(f, "\n");
	for(i = 1; i < 256; i++) {
		if(instrument[i].length > 1) {
			for(j = 0; j < instrument[i].length; j++) {
				fprintf(f, "instrumentline %02x %02x %02x %02x\n",
					i,
					j,
					instrument[i].line[j].cmd,
					instrument[i].line[j].param);
			}
		}
	}

	fclose(f);
}

void loadfile(char *fname) {
	FILE *f;
	char buf[1024];
	int cmd[3];
	int i1, i2, trk[4], transp[4], param[3], note, instr;
	int i;

	snprintf(filename, sizeof(filename), "%s", fname);

	f = fopen(fname, "r");
	if(!f) {
		return;
	}

	songlen = 1;
	while(!feof(f) && fgets(buf, sizeof(buf), f)) {
		if(9 == sscanf(buf, "songline %x %x %x %x %x %x %x %x %x",
			&i1,
			&trk[0],
			&transp[0],
			&trk[1],
			&transp[1],
			&trk[2],
			&transp[2],
			&trk[3],
			&transp[3])) {

			for(i = 0; i < 4; i++) {
				song[i1].track[i] = trk[i];
				song[i1].transp[i] = transp[i];
			}
			if(songlen <= i1) songlen = i1 + 1;
		} else if(8 == sscanf(buf, "trackline %x %x %x %x %x %x %x %x",
			&i1,
			&i2,
			&note,
			&instr,
			&cmd[0],
			&param[0],
			&cmd[1],
			&param[1])) {

			track[i1].line[i2].note = note;
			track[i1].line[i2].instr = instr;
			for(i = 0; i < 2; i++) {
				track[i1].line[i2].cmd[i] = cmd[i];
				track[i1].line[i2].param[i] = param[i];
			}
		} else if(4 == sscanf(buf, "instrumentline %x %x %x %x",
			&i1,
			&i2,
			&cmd[0],
			&param[0])) {

			instrument[i1].line[i2].cmd = cmd[0];
			instrument[i1].line[i2].param = param[0];
			if(instrument[i1].length <= i2) instrument[i1].length = i2 + 1;
		}
	}

	fclose(f);
}

void exitgui() {
	endwin();
}

void initgui() {
	int i;

	initscr();
	noecho();
	keypad(stdscr, TRUE);
	//cbreak();
	//nodelay(stdscr, TRUE);
	//halfdelay(1);
	raw();

	for(i = 1; i < 256; i++) {
		instrument[i].length = 1;
		instrument[i].line[0].cmd = '0';
		instrument[i].line[0].param = 0;
	}

	atexit(exitgui);
}

void drawsonged(int x, int y, int height) {
	int i, j;
	char buf[1024];

	if(songy < songoffs) songoffs = songy;
	if(songy >= songoffs + height) songoffs = songy - height + 1;

	for(i = 0; i < songlen; i++) {
		if(i >= songoffs && i - songoffs < height) {
			move(y + i - songoffs, x + 0);
			if(i == songy) attrset(A_BOLD);
			snprintf(buf, sizeof(buf), "%02x: ", i);
			addstr(buf);
			for(j = 0; j < 4; j++) {
				snprintf(buf, sizeof(buf), "%02x:%02x", song[i].track[j], song[i].transp[j]);
				addstr(buf);
				if(j != 3) addch(' ');
			}
			attrset(A_NORMAL);
			if(playsong && songpos == (i + 1)) addch('*');
		}
	}
}

void drawtracked(int x, int y, int height) {
	int i, j;
	char buf[1024];

	if(tracky < trackoffs) trackoffs = tracky;
	if(tracky >= trackoffs + height) trackoffs = tracky - height + 1;

	for(i = 0; i < tracklen; i++) {
		if(i >= trackoffs && i - trackoffs < height) {
			move(y + i - trackoffs, x + 0);
			if(i == tracky) attrset(A_BOLD);
			snprintf(buf, sizeof(buf), "%02x: ", i);
			addstr(buf);
			if(track[currtrack].line[i].note) {
				snprintf(buf, sizeof(buf), "%s%d",
					notenames[(track[currtrack].line[i].note - 1) % 12],
					(track[currtrack].line[i].note - 1) / 12);
			} else {
				snprintf(buf, sizeof(buf), "---");
			}
			addstr(buf);
			snprintf(buf, sizeof(buf), " %02x", track[currtrack].line[i].instr);
			addstr(buf);
			for(j = 0; j < 2; j++) {
				if(track[currtrack].line[i].cmd[j]) {
					snprintf(buf, sizeof(buf), " %c%02x",
						track[currtrack].line[i].cmd[j],
						track[currtrack].line[i].param[j]);
				} else {
					snprintf(buf, sizeof(buf), " ...");
				}
				addstr(buf);
			}
			attrset(A_NORMAL);
			if(playtrack && ((i + 1) % tracklen) == trackpos) {
				addch('*');
			}
		}
	}
}

void drawinstred(int x, int y, int height) {
	int i;
	char buf[1024];

	if(instry >= instrument[currinstr].length) instry = instrument[currinstr].length - 1;

	if(instry < instroffs) instroffs = instry;
	if(instry >= instroffs + height) instroffs = instry - height + 1;

	for(i = 0; i < instrument[currinstr].length; i++) {
		if(i >= instroffs && i - instroffs < height) {
			move(y + i - instroffs, x + 0);
			if(i == instry) attrset(A_BOLD);
			snprintf(buf, sizeof(buf), "%02x: %c ", i, instrument[currinstr].line[i].cmd);
			addstr(buf);
			if(instrument[currinstr].line[i].cmd == '+' || instrument[currinstr].line[i].cmd == '=') {
				if(instrument[currinstr].line[i].param) {
					snprintf(buf, sizeof(buf), "%s%d",
						notenames[(instrument[currinstr].line[i].param - 1) % 12],
						(instrument[currinstr].line[i].param - 1) / 12);
				} else {
					snprintf(buf, sizeof(buf), "---");
				}
			} else {
				snprintf(buf, sizeof(buf), "%02x", instrument[currinstr].line[i].param);
			}
			addstr(buf);
			attrset(A_NORMAL);
		}
	}
}

void drawmodeinfo(int x, int y) {
	switch(playmode) {
		case PM_IDLE:
			if(currtab == 2) {
				mvaddstr(y, x, "PLAY         IDLE space-> EDIT");
			} else {
				mvaddstr(y, x, "PLAY <-enter IDLE space-> EDIT");
			}
			attrset(A_REVERSE);
			mvaddstr(y, x + 13, "IDLE");
			attrset(A_NORMAL);
			break;
		case PM_PLAY:
			mvaddstr(y, x, "PLAY space-> IDLE         EDIT");
			attrset(A_REVERSE);
			mvaddstr(y, x + 0, "PLAY");
			attrset(A_NORMAL);
			break;
		case PM_EDIT:
			mvaddstr(y, x, "PLAY         IDLE <-space EDIT");
			attrset(A_REVERSE);
			mvaddstr(y, x + 26, "EDIT");
			attrset(A_NORMAL);
			break;
	}
}

void optimize() {
	u8 used[256], replace[256];
	int i, j;

	memset(used, 0, sizeof(used));
	for(i = 0; i < songlen; i++) {
		for(j = 0; j < 4; j++) {
			used[song[i].track[j]] = 1;
		}
	}

	j = 1;
	replace[0] = 0;
	for(i = 1; i < 256; i++) {
		if(used[i]) {
			replace[i] = j;
			j++;
		} else {
			replace[i] = 0;
		}
	}

	for(i = 1; i < 256; i++) {
		if(replace[i] && replace[i] != i) {
			memcpy(&track[replace[i]], &track[i], sizeof(struct track));
		}
	}

	for(i = 0; i < songlen; i++) {
		for(j = 0; j < 4; j++) {
			song[i].track[j] = replace[song[i].track[j]];
		}
	}

	for(i = 1; i < 256; i++) {
		u8 last = 255;

		for(j = 0; j < tracklen; j++) {
			if(track[i].line[j].instr) {
				if(track[i].line[j].instr == last) {
					track[i].line[j].instr = 0;
				} else {
					last = track[i].line[j].instr;
				}
			}
		}
	}
}

static FILE *exportfile = 0;
static int exportbits = 0;
static int exportcount = 0;
static int exportseek = 0;

void putbit(int x) {
	if(x) {
		exportbits |= (1 << exportcount);
	}
	exportcount++;
	if(exportcount == 8) {
		if(exportfile) {
			fprintf(exportfile, "\t.byte\t0x%02x\n", exportbits);
		}
		exportseek++;
		exportbits = 0;
		exportcount = 0;
	}
}

void exportchunk(int data, int bits) {
	int i;

	for(i = 0; i < bits; i++) {
		putbit(!!(data & (1 << i)));
	}
}

int alignbyte() {
	if(exportcount) {
		if(exportfile) {
			fprintf(exportfile, "\t.byte\t0x%02x\n", exportbits);
		}
		exportseek++;
		exportbits = 0;
		exportcount = 0;
	}
	if(exportfile) fprintf(exportfile, "\n");
	return exportseek;
}

int packcmd(u8 ch) {
	if(!ch) return 0;
	if(strchr(validcmds, ch)) {
		return strchr(validcmds, ch) - validcmds;
	}
	return 0;
}

void exportdata(FILE *f, int maxtrack, int *resources) {
	int i, j;
	int nres = 0;

	exportfile = f;
	exportbits = 0;
	exportcount = 0;
	exportseek = 0;

	for(i = 0; i < 16 + maxtrack; i++) {
		exportchunk(resources[i], 13);
	}

	resources[nres++] = alignbyte();

	for(i = 0; i < songlen; i++) {
		for(j = 0; j < 4; j++) {
			if(song[i].transp[j]) {
				exportchunk(1, 1);
				exportchunk(song[i].track[j], 6);
				exportchunk(song[i].transp[j], 4);
			} else {
				exportchunk(0, 1);
				exportchunk(song[i].track[j], 6);
			}
		}
	}

	for(i = 1; i < 16; i++) {
		resources[nres++] = alignbyte();

		if(instrument[i].length > 1) {
			for(j = 0; j < instrument[i].length; j++) {
				exportchunk(packcmd(instrument[i].line[j].cmd), 8);
				exportchunk(instrument[i].line[j].param, 8);
			}
		}

		exportchunk(0, 8);
	}

	for(i = 1; i <= maxtrack; i++) {
		resources[nres++] = alignbyte();

		for(j = 0; j < tracklen; j++) {
			u8 cmd = packcmd(track[i].line[j].cmd[0]);

			exportchunk(!!track[i].line[j].note, 1);
			exportchunk(!!track[i].line[j].instr, 1);
			exportchunk(!!cmd, 1);

			if(track[i].line[j].note) {
				exportchunk(track[i].line[j].note, 7);
			}

			if(track[i].line[j].instr) {
				exportchunk(track[i].line[j].instr, 4);
			}

			if(cmd) {
				exportchunk(cmd, 4);
				exportchunk(track[i].line[j].param[0], 8);
			}
		}
	}
}

void export() {
	FILE *f = fopen("exported.s", "w");
	FILE *hf = fopen("exported.h", "w");
	int i, j;
	int maxtrack = 0;
	int resources[256];

	exportfile = 0;
	exportbits = 0;
	exportcount = 0;
	exportseek = 0;

	for(i = 0; i < songlen; i++) {
		for(j = 0; j < 4; j++) {
			if(maxtrack < song[i].track[j]) maxtrack = song[i].track[j];
		}
	}

	fprintf(f, "\t.global\tsongdata\n\n");

	fprintf(hf, "#define MAXTRACK\t0x%02x\n", maxtrack);
	fprintf(hf, "#define SONGLEN\t\t0x%02x\n", songlen);

	fprintf(f, "songdata:\n");

	exportdata(0, maxtrack, resources);

	fprintf(f, "# ");
	for(i = 0; i < 16 + maxtrack; i++) {
		fprintf(f, "%04x ", resources[i]);
	}
	fprintf(f, "\n");

	exportdata(f, maxtrack, resources);

	fclose(f);
	fclose(hf);
}

void handleinput() {
	int c, x;
	
	if((c = getch()) != ERR) switch(c) {
		case 10:
		case 13:
			if(currtab != 2) {
				playmode = PM_PLAY;
				if(currtab == 1) {
					startplaytrack(currtrack);
				} else if(currtab == 0) {
					startplaysong(songy);
				}
			}
			break;
		case ' ':
			silence();
			if(playmode == PM_IDLE) {
				playmode = PM_EDIT;
			} else {
				playmode = PM_IDLE;
			}
			break;
		case 9:
			currtab++;
			currtab %= 3;
			break;
		case 'E' - '@':
			erase();
			refresh();
			endwin();
			exit(0);
			break;
		case 'W' - '@':
			savefile(filename);
			break;
		case '<':
			if(octave) octave--;
			break;
		case '>':
			if(octave < 8) octave++;
			break;
		case '[':
			if(currinstr > 1) currinstr--;
			break;
		case ']':
			if(currinstr < 255) currinstr++;
			break;
		case '{':
			if(currtrack > 1) currtrack--;
			break;
		case '}':
			if(currtrack < 255) currtrack++;
			break;
		case '`':
			if(currtab == 0) {
				int t = song[songy].track[songx / 4];
				if(t) currtrack = t;
				currtab = 1;
			} else if(currtab == 1) {
				currtab = 0;
			}
			break;
		case '#':
			optimize();
			break;
		case '%':
			export();
			break;
		case KEY_LEFT:
			switch(currtab) {
				case 0:
					if(songx) songx--;
					break;
				case 1:
					if(trackx) trackx--;
					break;
				case 2:
					if(instrx) instrx--;
					break;
			}
			break;
		case KEY_RIGHT:
			switch(currtab) {
				case 0:
					if(songx < 15) songx++;
					break;
				case 1:
					if(trackx < 8) trackx++;
					break;
				case 2:
					if(instrx < 2) instrx++;
					break;
			}
			break;
		case KEY_UP:
			switch(currtab) {
				case 0:
					if(songy) songy--;
					break;
				case 1:
					if(tracky) {
						tracky--;
					} else {
						tracky = tracklen - 1;
					}
					break;
				case 2:
					if(instry) instry--;
					break;
			}
			break;
		case KEY_DOWN:
			switch(currtab) {
				case 0:
					if(songy < songlen - 1) songy++;
					break;
				case 1:
					if(tracky < tracklen - 1) {
						tracky++;
					} else {
						tracky = 0;
					}
					break;
				case 2:
					if(instry < instrument[currinstr].length - 1) instry++;
					break;
			}
			break;
		case 'C':
			if(currtab == 2) {
				memcpy(&iclip, &instrument[currinstr], sizeof(struct instrument));
			} else if(currtab == 1) {
				memcpy(&tclip, &track[currtrack], sizeof(struct track));
			}
			break;
		case 'V':
			if(currtab == 2) {
				memcpy(&instrument[currinstr], &iclip, sizeof(struct instrument));
			} else if(currtab == 1) {
				memcpy(&track[currtrack], &tclip, sizeof(struct track));
			}
			break;
		default:
			if(playmode == PM_EDIT) {
				x = hexdigit(c);
				if(x >= 0) {
					if(currtab == 2
					&& instrx > 0
					&& instrument[currinstr].line[instry].cmd != '+'
					&& instrument[currinstr].line[instry].cmd != '=') {
						switch(instrx) {
							case 1: SETHI(instrument[currinstr].line[instry].param, x); break;
							case 2: SETLO(instrument[currinstr].line[instry].param, x); break;
						}
					}
					if(currtab == 1 && trackx > 0) {
						switch(trackx) {
							case 1: SETHI(track[currtrack].line[tracky].instr, x); break;
							case 2: SETLO(track[currtrack].line[tracky].instr, x); break;
							case 4: if(track[currtrack].line[tracky].cmd[0])
								SETHI(track[currtrack].line[tracky].param[0], x); break;
							case 5: if(track[currtrack].line[tracky].cmd[0])
								SETLO(track[currtrack].line[tracky].param[0], x); break;
							case 7: if(track[currtrack].line[tracky].cmd[1])
								SETHI(track[currtrack].line[tracky].param[1], x); break;
							case 8: if(track[currtrack].line[tracky].cmd[1])
								SETLO(track[currtrack].line[tracky].param[1], x); break;
						}
					}
					if(currtab == 0) {
						switch(songx & 3) {
							case 0: SETHI(song[songy].track[songx / 4], x); break;
							case 1: SETLO(song[songy].track[songx / 4], x); break;
							case 2: SETHI(song[songy].transp[songx / 4], x); break;
							case 3: SETLO(song[songy].transp[songx / 4], x); break;
						}
					}
				}
				x = freqkey(c);
				if(x >= 0) {
					if(currtab == 2
					&& instrx
					&& (instrument[currinstr].line[instry].cmd == '+' || instrument[currinstr].line[instry].cmd == '=')) {
						instrument[currinstr].line[instry].param = x;
					}
					if(currtab == 1 && !trackx) {
						track[currtrack].line[tracky].note = x;
						if(x) {
							track[currtrack].line[tracky].instr = currinstr;
						} else {
							track[currtrack].line[tracky].instr = 0;
						}
						tracky++;
						tracky %= tracklen;
						if(x) iedplonk(x, currinstr);
					}
				}
				if(currtab == 2 && instrx == 0) {
					if(strchr(validcmds, c)) {
						instrument[currinstr].line[instry].cmd = c;
					}
				}
				if(currtab == 1 && (trackx == 3 || trackx == 6 || trackx == 9)) {
					if(strchr(validcmds, c)) {
						if(c == '.' || c == '0') c = 0;
						track[currtrack].line[tracky].cmd[(trackx - 3) / 3] = c;
					}
				}
				if(c == 'A') {
					if(currtab == 2) {
						struct instrument *in = &instrument[currinstr];

						if(in->length < 256) {
							memmove(&in->line[instry + 2], &in->line[instry + 1], sizeof(struct instrline) * (in->length - instry - 1));
							instry++;
							in->length++;
							in->line[instry].cmd = '0';
							in->line[instry].param = 0;
						}
					} else if(currtab == 0) {
						if(songlen < 256) {
							memmove(&song[songy + 2], &song[songy + 1], sizeof(struct songline) * (songlen - songy - 1));
							songy++;
							songlen++;
							memset(&song[songy], 0, sizeof(struct songline));
						}
					}
				} else if(c == 'I') {
					if(currtab == 2) {
						struct instrument *in = &instrument[currinstr];

						if(in->length < 256) {
							memmove(&in->line[instry + 1], &in->line[instry + 0], sizeof(struct instrline) * (in->length - instry));
							in->length++;
							in->line[instry].cmd = '0';
							in->line[instry].param = 0;
						}
					} else if(currtab == 0) {
						if(songlen < 256) {
							memmove(&song[songy + 1], &song[songy + 0], sizeof(struct songline) * (songlen - songy));
							songlen++;
							memset(&song[songy], 0, sizeof(struct songline));
						}
					}
				} else if(c == 'D') {
					if(currtab == 2) {
						struct instrument *in = &instrument[currinstr];

						if(in->length > 1) {
							memmove(&in->line[instry + 0], &in->line[instry + 1], sizeof(struct instrline) * (in->length - instry - 1));
							in->length--;
							if(instry >= in->length) instry = in->length - 1;
						}
					} else if(currtab == 0) {
						if(songlen > 1) {
							memmove(&song[songy + 0], &song[songy + 1], sizeof(struct songline) * (songlen - songy - 1));
							songlen--;
							if(songy >= songlen) songy = songlen - 1;
						}
					}
				}
			} else if(playmode == PM_IDLE) {
				x = freqkey(c);

				if(x > 0) {
					iedplonk(x, currinstr);
				}
			}
			break;
	}
}

void drawgui() {
	char buf[1024];
	int lines = LINES, cols = 79;
	int songcols[] = {0, 1, 3, 4, 6, 7, 9, 10, 12, 13, 15, 16, 18, 19, 21, 22};
	int trackcols[] = {0, 4, 5, 7, 8, 9, 11, 12, 13};
	int instrcols[] = {0, 2, 3};

	erase();
	mvaddstr(0, 0, "music chip tracker 0.1 by lft");
	drawmodeinfo(cols - 30, 0);
	snprintf(buf, sizeof(buf), "Octave:   %d <>", octave);
	mvaddstr(2, cols - 14, buf);
	mvaddstr(3, cols - 14, "^W)rite ^E)xit");

	snprintf(buf, sizeof(buf), "^F)ilename:        %s", filename);
	mvaddstr(2, 15, buf);

	mvaddstr(5, 0, "Song");
	drawsonged(0, 6, lines - 12);

	snprintf(buf, sizeof(buf), "Track %02x {}", currtrack);
	mvaddstr(5, 29, buf);
	drawtracked(29, 6, lines - 8);

	snprintf(buf, sizeof(buf), "Instr. %02x []", currinstr);
	mvaddstr(5, 49, buf);
	drawinstred(49, 6, lines - 12);

	switch(currtab) {
		case 0:
			move(6 + songy - songoffs, 0 + 4 + songcols[songx]);
			break;
		case 1:
			move(6 + tracky - trackoffs, 29 + 4 + trackcols[trackx]);
			break;
		case 2:
			move(6 + instry - instroffs, 49 + 4 + instrcols[instrx]);
			break;
	}

	refresh();
}

void guiloop() {
	for(;;) {
		drawgui();
		handleinput();
	}
}
