#include <iostream>
#include <ctype.h>
#include <sstream>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;


#define CTRL_KEY(k) ((k) & 0x1f)
#define TAB_STOP 8
#define QUIT_TIMES 3


enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

enum editorHighlight {
  HL_NORMAL = 0,
  HL_BLINK,
  HL_ACTIVE,
  HL_RESET,
  HL_STUDENT,
  HL_FACULTY,
  HL_TOTAL,
};

typedef struct erow {
  int idx;
  int size;
  int rsize;
  char *chars;
  char *render;
  unsigned char *hl;
  int hl_open_comment;
} erow;

struct editorConfig {
  int cx, cy;
  int rx;
  int rowoff;
  int coloff;
  int screenrows;
  int screencols;
  int numrows;
  erow *row;
  int dirty;
  char *filename;
  char statusmsg[80];
  time_t statusmsg_time;
  struct termios orig_termios;

  int editAccess;
  int edit;

  int row_count;
  int col_count;
  
  vector <char> arr;
  char prev  ;
  char present;
  int modified;

  int activeX;
  int activeY;

  int rc;


  int option ;
  char* logFile; 
};

struct editorConfig E;


#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

/*** prototypes ***/

void editorSetStatusMessage(const char *fmt, ...);
void editorRefreshScreen();
char *editorPrompt(char *prompt, void (*callback)(char *, int));

/*** update main file ***/


void update_total()
{
    fstream file;
    file.open("data.txt");
    string line = "", LINE = "", start ="";

    if(file.is_open())
    {   
        string s;
        int i=0;
        
        while(getline(file,s))
        {   
            if(i++==0)
            {
                line += s + "       \n";
                continue;
            }

            char s1[s.size()+1];
            strcpy(s1, s.c_str());
            char* token = strtok(s1, " "); 

            vector<string> tokens;
            while (token != NULL)
            {
                tokens.push_back(token);
                token = strtok(NULL, " ");
            }     

            tokens.pop_back();  

            if(tokens.back().length() > 1)
            {
              tokens.pop_back();
            }
            
            int totalsum = 0;
            line += " " + tokens[0] + "      ";
            string temp = "";
            for (int y=1;y<tokens.size();y++)
            {   

                line += tokens[y] + "        ";
                // temp += tokens[y] + "        ";
                if(tokens[y] != "-"){
                    totalsum += tokens[y][0] - 48;
                }

                // stringstream geek(tokens[y]);
                // int x = 0;
                // geek >> x;
                // totalsum += x ;
            }

            // editorSetStatusMessage( const_cast<char*>(temp.c_str()));

            
            if(totalsum < 10){
                line += "0" + to_string(totalsum)  + "       \n";
            } 
            else{
                line += to_string(totalsum) + "       \n";
            }

        }
    }    

    ofstream myfile("data.txt");
    myfile << line;
    myfile.close();
        
    cout << line;
    
}

void save_table(int a, int b)
{
    fstream file1;
    fstream file2;
    file1.open("data.txt");
    file2.open(E.filename);
    string line = "", LINE = "", start ="";

    if(a==1)
    {   
        // just catch that LINE from updated file 
        if(file2.is_open())
        {
            string s;
            int i=0;
            while(getline(file2,s)) 
            { 
                if(i==1)
                  LINE += s + "\n"; 
                i++;  
            }
        }

        if(file1.is_open())
        {
            string s;
            int i=0;
            while(getline(file1,s))
            {
                if(i==b)
                   line += LINE ; 
                else
                   line += s +"\n"; 
                i++;  
            }
        }

        ofstream myfile("data.txt");
	    myfile << line;
	    myfile.close();
        //cout << line;

    }

    else if(a==0)
    {   
        vector<string> newcol;
        if(file2.is_open()) // Collecting updated values in a vector
        {   
            string s;
            int i=0;
            
            while(getline(file2,s))
            {    

                char s1[s.size()+1];
                strcpy(s1, s.c_str());
                char* token = strtok(s1, " "); 

                vector<string> tokens;
                while (token != NULL)
                {
                    tokens.push_back(token);
                    token = strtok(NULL, " ");
                }

                if(tokens.size()>1)
                newcol.push_back(tokens[1]);
            }
        }


        if(file1.is_open())
        {
            string s;
            int i=0,flag=0;
            while(getline(file1,s)) 
            {   
                if(i==0)
                  line += s +"\n" ; 

                else
                {   
                    char s1[s.size()+1];
                    strcpy(s1, s.c_str());
                    char* token = strtok(s1, " ");

                    vector<string> tokens;
                    while (token != NULL)
                    {
                        tokens.push_back(token);
                        token = strtok(NULL, " ");
                    }
                    
                    line += " " + tokens[0] + "      " ;
                    
                    int j=1;
                    while(j<tokens.size())
                    {
                        if(j==b){
                          if(j==1){
                              line += newcol[flag++] ;

                          }
                          else{
                             line += "        " + newcol[flag++] ;

                          }
                        }
                        else  {
                            if(j==1){
                              line += tokens[j];
                          }
                          else{
                             line += "        " + tokens[j];
                          }
                        }
                        j++;
                    } 
                    line += "\n";

                }  
                i++;  
            }
        }        
    }

    ofstream myfile("data.txt");
    myfile << line;
    myfile.close();
    
    update_total();
        
    //cout << line;
}


/*** terminal ***/

void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}

// To open the terminal in raw mode 
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = E.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


int editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }

  if (c == '\x1b') {
    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
            case '1': return HOME_KEY;
            case '3': return DEL_KEY;
            case '4': return END_KEY;
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;
            case '7': return HOME_KEY;
            case '8': return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
          case 'H': return HOME_KEY;
          case 'F': return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H': return HOME_KEY;
        case 'F': return END_KEY;
      }
    }

    return '\x1b';
  } else {
    return c;
  }
}


int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;

  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  buf[i] = '\0';

  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

  return 0;
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    return getCursorPosition(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

void editorUpdateHighlight(erow *row) {
  row->hl = (unsigned char*) realloc(row->hl, row->rsize);
  memset(row->hl, HL_NORMAL, row->rsize);  

  if(E.row_count == 1){
    // row->hl = (unsigned char*) realloc(row->hl, row->rsize);
    memset(row->hl, HL_FACULTY, row->rsize);

    if(E.option != 2){
          memset(row->hl, HL_FACULTY, row->rsize-7);
          memset(&row->hl[row->rsize-6], HL_TOTAL, 6);
          memset(&row->hl[row->rsize-1], HL_RESET, 1);
    }


  }
  else {
    memset(row->hl, HL_STUDENT, 9);
  }

if(E.edit){
      memset(&row->hl[E.activeX-3], HL_ACTIVE, 8);
      memset(&row->hl[E.activeX+6], HL_RESET, 1);
    }



}

int editorHighlightColor(int hl) {
  switch (hl) {
    case HL_ACTIVE: return 44;
    case HL_TOTAL : return 43;
    case HL_RESET : return 0;
    case HL_BLINK : return 44;
    case HL_STUDENT : return 32;
    case HL_FACULTY: return 33;
    default: return 37;

  }
}


/*** row operations ***/

int editorRowCxToRx(erow *row, int cx) {
  int rx = 0;
  int j;
  for (j = 0; j < cx; j++) {
    if (row->chars[j] == '\t')
      rx += (TAB_STOP - 1) - (rx % TAB_STOP);
    rx++;
  }
  return rx;
}

int editorRowRxToCx(erow *row, int rx) {
  int cur_rx = 0;
  int cx;
  for (cx = 0; cx < row->size; cx++) {
    if (row->chars[cx] == '\t')
      cur_rx += (TAB_STOP - 1) - (cur_rx % TAB_STOP);
    cur_rx++;

    if (cur_rx > rx) return cx;
  }
  return cx;
}

void editorUpdateRow(erow *row) {
  int tabs = 0;
  int j;
  for (j = 0; j < row->size; j++)
    if (row->chars[j] == '\t') tabs++;

  free(row->render);
  row->render = (char *) malloc(row->size + tabs*(TAB_STOP - 1) + 4);

  int idx = 0;
  for (j = 0; j < row->size; j++) {
   
    if (row->chars[j] == '\t') {
      row->render[idx++] = ' ';

    while (idx % TAB_STOP != 0) row->render[idx++] = ' ';
    } else {
      row->render[idx++] = row->chars[j];
    }
  }
  row->render[idx] = '\0';
  row->rsize = idx;

  editorUpdateHighlight(row);
}

void editorInsertRow(int at, char *s, size_t len) {
  if (at < 0 || at > E.numrows) return;

  E.row = (erow *) realloc(E.row, sizeof(erow) * (E.numrows + 1));
  memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numrows - at));
  for (int j = at + 1; j <= E.numrows; j++) E.row[j].idx++;

  E.row[at].idx = at;

  E.row[at].size = len;

  
  E.row[at].chars = (char *) malloc(len + 1);
  memcpy(E.row[at].chars, s, len);
  E.row[at].chars[len] = '\0';

  if(E.option == 2){
    for(int i = 11 ; i< len ; i++){
        if(s[i] > 47 && s[i] < 58)
          E.arr.push_back(s[i]);
        else if(s[i] == 45){
          E.arr.push_back(s[i]);
        }
    }
  }


  E.row[at].rsize = 0;
  E.row[at].render = NULL;
  E.row[at].hl = NULL;
  E.row[at].hl_open_comment = 0;
  editorUpdateRow(&E.row[at]);

  E.numrows++;
  E.dirty++;
}

void editorFreeRow(erow *row) {
  free(row->render);
  free(row->chars);
  free(row->hl);
}

void editorDelRow(int at) {
  if (at < 0 || at >= E.numrows) return;
  editorFreeRow(&E.row[at]);
  memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
  for (int j = at; j < E.numrows - 1; j++) E.row[j].idx--;
  E.numrows--;
  E.dirty++;
}

void editorRowInsertChar(erow *row, int at, int c) {
  if (at < 0 || at > row->size) at = row->size;
  row->chars = (char *) realloc(row->chars, row->size + 2);
  memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
  row->size++;
  row->chars[at] = c;
  editorUpdateRow(row);
  E.dirty++;
}

void editorRowAppendString(erow *row, char *s, size_t len) {
  row->chars = (char * ) realloc(row->chars, row->size + len + 1);
  memcpy(&row->chars[row->size], s, len);
  row->size += len;
  row->chars[row->size] = '\0';
  editorUpdateRow(row);
  E.dirty++;
}

void editorRowDelChar(erow *row, int at) {
  if (at < 0 || at >= row->size) return;
  memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
  row->size--;
  editorUpdateRow(row);
  E.dirty++;
}

/*** editor operations ***/

void editorInsertChar(int c) {
  if (E.cy == E.numrows) {
    editorInsertRow(E.numrows, (char*)"", 0);
  }
  editorRowInsertChar(&E.row[E.cy], E.cx, c);
  E.cx++;
}

void editorInsertNewline() {
  if (E.cx == 0) {
    editorInsertRow(E.cy, (char*)"", 0);
  } else {
    erow *row = &E.row[E.cy];
    editorInsertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
    row = &E.row[E.cy];
    row->size = E.cx;
    row->chars[row->size] = '\0';
    editorUpdateRow(row);
  }
  E.cy++;
  E.cx = 0;
}

void editorDelChar() {
  if (E.cy == E.numrows) return;
  if (E.cx == 0 && E.cy == 0) return;

  erow *row = &E.row[E.cy];
  if (E.cx > 0) {
    editorRowDelChar(row, E.cx - 1);
    E.cx--;
  } else {
    E.cx = E.row[E.cy - 1].size;
    editorRowAppendString(&E.row[E.cy - 1], row->chars, row->size);
    editorDelRow(E.cy);
    E.cy--;
  }
}

/*** file i/o ***/

char *editorRowsToString(int *buflen) {
  int totlen = 0;
  int j;
  for (j = 0; j < E.numrows; j++)
    totlen += E.row[j].size + 1;
  *buflen = totlen;

  char *buf = (char *) malloc(totlen);
  char *p = buf;
  for (j = 0; j < E.numrows; j++) {
    memcpy(p, E.row[j].chars, E.row[j].size);
    p += E.row[j].size;
    *p = '\n';
    p++;
  }

  return buf;
}

void editorOpen(char *filename) {
  
  free(E.filename);
  E.filename = strdup(filename);

  //editorSelectSyntaxHighlight();

  FILE *fp = fopen(filename, "r");
  if (!fp) die("fopen");

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  

  while ((linelen = getline(&line, &linecap, fp)) != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\n' ||
                           line[linelen - 1] == '\r'))
      linelen--;
    E.row_count++;
    editorInsertRow(E.numrows, line, linelen);  
    
  }
  free(line);
  fclose(fp);
  E.dirty = 0;
}

void editorSave() {
  if (E.filename == NULL) {
    E.filename = editorPrompt((char*)"Save as: %s (ESC to cancel)", NULL);
    if (E.filename == NULL) {
      editorSetStatusMessage("Save aborted");
      return;
    }
    // editorSelectSyntaxHighlight();
  }

  int len;
  char *buf = editorRowsToString(&len);

  int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        E.dirty = 0;
        save_table(0, E.rc);
        return;
      }
    }
    close(fd);
  }

  free(buf);
  editorSetStatusMessage("Can't save! I/O error: %s", strerror(errno));
}

/*** append buffer ***/

struct abuf {
  char *b;
  int len;
};

#define ABUF_INIT {NULL, 0}

void abAppend(struct abuf *ab, const char *s, int len) {
  char *n =  (char *) realloc(ab->b, ab->len + len);

  if (n == NULL) return;
  memcpy(&n[ab->len], s, len);
  ab->b = n;
  ab->len += len;
}

void abFree(struct abuf *ab) {
  free(ab->b);
}

/*** output ***/

void editorScroll() {
  E.rx = 0;
  if (E.cy < E.numrows) {
    E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
  }

  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
  if (E.cy >= E.rowoff + E.screenrows) {
    E.rowoff = E.cy - E.screenrows + 1;
  }
  if (E.rx < E.coloff) {
    E.coloff = E.rx;
  }
  if (E.rx >= E.coloff + E.screencols) {
    E.coloff = E.rx - E.screencols + 1;
  }
}

void editorDrawRows(struct abuf *ab) {
  int y;
  for (y = 0; y < E.screenrows; y++) {
    int filerow = y + E.rowoff;
    if (filerow >= E.numrows) {
      if (E.numrows == 0 && y == E.screenrows / 3) {
        char welcome[80];
        int welcomelen = snprintf(welcome, sizeof(welcome),
          "Amigo Editor");
        if (welcomelen > E.screencols) welcomelen = E.screencols;
        int padding = (E.screencols - welcomelen) / 2;
        if (padding) {
          abAppend(ab, "~", 1);
          padding--;
        }
        while (padding--) abAppend(ab, " ", 1);
        abAppend(ab, welcome, welcomelen);
      } else {
        abAppend(ab, "~", 1);
      }
    } else {
      int len = E.row[filerow].rsize - E.coloff;
      if (len < 0) len = 0;
      if (len > E.screencols) len = E.screencols;
      char *c = &E.row[filerow].render[E.coloff];
      unsigned char *hl = &E.row[filerow].hl[E.coloff];
      int current_color = -1;
      int j;
      for (j = 0; j < len; j++) {
        if (iscntrl(c[j])) {
          char sym = (c[j] <= 26) ? '@' + c[j] : '?';
          abAppend(ab, "\x1b[7m", 4);
          abAppend(ab, &sym, 1);
          abAppend(ab, "\x1b[m", 3);
          if (current_color != -1) {
            char buf[16];
            int clen = snprintf(buf, sizeof(buf), "\x1b[%dm", current_color);
            abAppend(ab, buf, clen);
          }
        } else if (hl[j] == HL_NORMAL) {
          if (current_color != -1) {
            abAppend(ab, "\x1b[39m", 5);
            current_color = -1;
          }
          abAppend(ab, &c[j], 1);
        } else {
          int color = editorHighlightColor(hl[j]);
          if (color != current_color) {
            current_color = color;
            char buf[16];
            int clen = snprintf(buf, sizeof(buf), "\x1b[%dm", color);
            abAppend(ab, buf, clen);
          }
          abAppend(ab, &c[j], 1);
        }
      }
      abAppend(ab, "\x1b[39m", 5);
    }

    abAppend(ab, "\x1b[K", 3);
    abAppend(ab, "\r\n", 2);
  }
}

void editorDrawNavBar(struct abuf *ab) {
  abAppend(ab, "\x1b[7m", 4);
  char status[80];
  int len = snprintf(status, sizeof(status), " Marksheet %s",
    E.dirty ? "(modified)" : "");

  if (len > E.screencols) len = E.screencols;
  abAppend(ab, status, len);
  while (len < E.screencols) {
      abAppend(ab, " ", 1);
      len++;
  }
  abAppend(ab, "\x1b[m", 3);
  abAppend(ab, "\r\n", 2);
}

void editorDrawMessageBar(struct abuf *ab) {
  abAppend(ab, "\x1b[K", 3);
  int msglen = strlen(E.statusmsg);
  if (msglen > E.screencols) msglen = E.screencols;
  if (msglen && time(NULL) - E.statusmsg_time < 5)
    abAppend(ab, E.statusmsg, msglen);
}

void editorRefreshScreen() {
  
  editorScroll();

  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);

  editorDrawNavBar(&ab);
  editorDrawRows(&ab);
  editorDrawMessageBar(&ab);

  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 2,
                                            (E.rx - E.coloff) + 1);
  abAppend(&ab, buf, strlen(buf));

  abAppend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

void editorSetStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
  va_end(ap);
  E.statusmsg_time = time(NULL);
}

/*** input ***/

char *editorPrompt(char *prompt, void (*callback)(char *, int)) {
  size_t bufsize = 128;
  char *buf = (char *)malloc(bufsize);

  size_t buflen = 0;
  buf[0] = '\0';

  while (1) {
    editorSetStatusMessage(prompt, buf);
    editorRefreshScreen();

    int c = editorReadKey();
    if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
      if (buflen != 0) buf[--buflen] = '\0';
    } else if (c == '\x1b') {
      editorSetStatusMessage("");
      if (callback) callback(buf, c);
      free(buf);
      return NULL;
    } else if (c == '\r') {
      if (buflen != 0) {
        editorSetStatusMessage("");
        if (callback) callback(buf, c);
        return buf;
      }
    } else if (!iscntrl(c) && c < 128) {
      if (buflen == bufsize - 1) {
        bufsize *= 2;
        buf = (char *) realloc(buf, bufsize);
      }
      buf[buflen++] = c;
      buf[buflen] = '\0';
    }

    if (callback) callback(buf, c);
  }
}

void editorMoveCursor(int key) {
  erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  E.activeX = 0;
  E.activeY = E.cy;
  editorUpdateHighlight(&E.row[E.cy]);


  switch (key) {
    case ARROW_LEFT:
      if (E.cx != 15 ) {
        E.cx = E.cx-9;
      }
      else if (E.cy > 1) {
        E.cy--;
        E.cx = 42;
      }
      break;
    case ARROW_RIGHT:
      if (row && E.cx < 42 ) {
       E.cx = E.cx+ 9;
      } else if (row && E.cx == 42) {
        if(E.cy+1 >= E.numrows) 
          break;
        E.cy++;
        E.cx = 15;
      }
      break;
    case ARROW_UP:
      if (E.cy != 1) {
        E.cy--;
        if(E.cx < 15)
          E.cx = 15;
      }
      break;
    case ARROW_DOWN:
      if (E.cy+1 < E.numrows) {
            E.cy++;
      }
      break;
  }

  row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  int rowlen = row ? row->size : 15;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}

void editorProcessKeypress() {
  static int quit_times = QUIT_TIMES;

  int c = editorReadKey();

  switch(E.option){
    case 1 :
      switch (c){
            case CTRL_KEY('q'):
            case CTRL_KEY('a'):
              write(STDOUT_FILENO, "\x1b[2J", 4);
              write(STDOUT_FILENO, "\x1b[H", 3);
              exit(0);
              break;
            case PAGE_UP:
            case PAGE_DOWN:
              {
                if (c == PAGE_UP) {
                  E.cy = E.rowoff;
                } else if (c == PAGE_DOWN) {
                  E.cy = E.rowoff + E.screenrows - 1;
                  if (E.cy > E.numrows) E.cy = E.numrows;
                }

                int times = E.screenrows;
                while (times--)
                  editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
              }
              break;

            case ARROW_UP:
            case ARROW_DOWN:
            case ARROW_LEFT:
            case ARROW_RIGHT:
              editorRefreshScreen();
              editorMoveCursor(c);
              break;
      }
      break ;
    
    case 2 : 
      switch (c) {
        case '\r':
    
          if(E.edit == 0){
            editorSetStatusMessage("\u001b[32mPlease Enter the Marks\u001b[0m or \u001b[31mPress Esc to return back\u001b[0m");
            E.edit = 1;
            E.activeX = E.cx;

            E.prev = E.arr[E.cy];
            E.present = ' ';

            editorUpdateHighlight(&E.row[E.cy]);
            E.cx++;
            editorDelChar();
            editorInsertChar(' ');
            E.modified = 0;

          }
          else{
            if(E.modified){
                E.cx--;
                E.edit = 0;
                E.activeX = 0;
                E.arr[E.cy] = E.present;
                editorUpdateHighlight(&E.row[E.cy]);
                editorSetStatusMessage("\u001b[32mMarks are updated\u001b[0m");
                editorSave();
                
            }
            else{
              editorSetStatusMessage("\u001b[31mPlease Enter valid marks\u001b[0m");
              E.present = ' ';
            }
          }
          break;
          
        case '\033' :
            if(E.edit == 1)
            {
              editorDelChar();
              editorInsertChar(E.prev);
              E.modified = 0;
              E.edit =0;
              editorUpdateHighlight(&E.row[E.cy]);
              E.cx--;
              editorSave();
            }
            editorSetStatusMessage(" ");
            break;

        case CTRL_KEY('q'):
        case CTRL_KEY('a'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
        case PAGE_UP:
        case PAGE_DOWN:
            {
              if (c == PAGE_UP) {
                E.cy = E.rowoff;
              } else if (c == PAGE_DOWN) {
                E.cy = E.rowoff + E.screenrows - 1;
                if (E.cy > E.numrows) E.cy = E.numrows;
              }

              int times = E.screenrows;
              while (times--)
                editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
            }
            break;

        case ARROW_UP:
        case ARROW_DOWN:
            if(E.edit == 0){
              E.edit = 0;
              editorRefreshScreen();
              editorMoveCursor(c);
            }
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9' :
        case '-' :
          if(E.edit){

            editorSetStatusMessage("Press \u001b[32mEnter\u001b[0m to save the Marks or Press \u001b[31mEsc\u001b[0m to return back");
            E.present = c;
            editorDelChar();
            editorInsertChar(c);
            E.modified = 1;
          }

      }
      break;

    case 3 : 
      switch (c){
        case CTRL_KEY('q'):
        case CTRL_KEY('a'):
              write(STDOUT_FILENO, "\x1b[2J", 4);
              write(STDOUT_FILENO, "\x1b[H", 3);
              exit(0);
              break;

        case ARROW_LEFT:
        case ARROW_RIGHT:
              editorRefreshScreen();
              editorMoveCursor(c);
              break;
        
      }
      break;

    default :
      switch(c){
            case CTRL_KEY('q'):
            case CTRL_KEY('a'):
              write(STDOUT_FILENO, "\x1b[2J", 4);
              write(STDOUT_FILENO, "\x1b[H", 3);
              exit(0);
              break;
      }
      break;


  }

  int a = 0;

  if(a){

      switch (c) {

    case HOME_KEY:
      E.cx = 7;
      break;

    case END_KEY:
      if (E.cy < E.numrows)
        E.cx = 19;
      break;

    case '\033' :
      
      if(E.edit == 1)
      {
        editorDelChar();
        editorInsertChar(E.prev);
        E.modified = 0;
        E.edit =0;
        editorUpdateHighlight(&E.row[E.cy]);
        E.cx--;
        editorSave();
      }
      editorSetStatusMessage(" ");
      break;

    case PAGE_UP:
    case PAGE_DOWN:
      {
        if (c == PAGE_UP) {
          E.cy = E.rowoff;
        } else if (c == PAGE_DOWN) {
          E.cy = E.rowoff + E.screenrows - 1;
          if (E.cy > E.numrows) E.cy = E.numrows;
        }

        int times = E.screenrows;
        while (times--)
          editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      if(E.edit == 0){
        E.edit = 0;
        editorRefreshScreen();
        editorMoveCursor(c);
      }
      break;

    // case CTRL_KEY('l'):
    // case '\x1b':
    //   break;
          
    

    default:
      break;
  }

  }
  // else{
  //     switch (c) {

  //           case CTRL_KEY('q'):

  //             write(STDOUT_FILENO, "\x1b[2J", 4);
  //             write(STDOUT_FILENO, "\x1b[H", 3);
  //             exit(0);
  //             break;
          
  //           case ARROW_UP:
  //           case ARROW_DOWN:
  //           case ARROW_LEFT:
  //           case ARROW_RIGHT:
  //             editorMoveCursor(c);
  //             break;
  //     }
  // }

  

  quit_times = QUIT_TIMES;
}

/*** init ***/

void initEditor() {
  E.cx = 15;
  E.cy = 1;
  E.rx = 15;
  E.rowoff = 0;
  E.coloff = 0;
  E.numrows = 0;
  E.row = NULL;
  E.dirty = 0;
  E.filename = NULL;
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;

  E.row_count = 0;
  E.col_count= 0;
  E.present = '-';

  E.editAccess = 0;
  E.edit = 0;
  E.prev = '-';
  E.modified = 0;

  E.activeX = 0;
  E.activeY = 0;

  E.option = 0;

  E.rc = -1;

  if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
  E.screenrows -= 2;
}

void Dean() {


      enableRawMode();
      initEditor();

      E.option = 1;

      string arg1 = "data.txt";
      char* c = const_cast<char*>(arg1.c_str());

      editorOpen(c);
      
      editorSetStatusMessage("\u001b[31mHELP\u001b[0m : Ctrl- q = quit");

      while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
        editorSetStatusMessage("\u001b[31mHELP\u001b[0m : Ctrl- q = quit");

      }


}

void present_row(int n)
{
    string line = "";
    fstream file; 

	file.open("data.txt"); 
	if (file.is_open())
	{
		string s, str="" ;   
        int i=0;   
		while(getline(file, s))
		{ 
            if( i != 0 )
            {   
                if(i==n)
                str += s + "\n";

                // if(i==n)
                //     str += s + "\n";
                // else
                //     str += " Student" + to_string(i) + "\n";
            }
            else
            {
                str += s + "\n";
            }
            i++;
        }
        string str1 = "Student" + to_string(n) + ".txt";
        ofstream myfile(str1);
	    myfile << str;
	    myfile.close();
    }    
}


void Student(int arg1) {

    string str1 , str2;
    str1 = "Student" + to_string(arg1) + ".txt";
    present_row(arg1);
      enableRawMode();
      initEditor();

      E.option = 3;

      char* c = const_cast<char*>(str1.c_str());

      editorOpen(c);
      
      editorSetStatusMessage("\u001b[31mHELP\u001b[0m : Ctrl- q = quit");

      while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
        // editorSetStatusMessage("\u001b[31mHELP\u001b[0m : Ctrl- q = quit");

      }


}

void WriteLog(char* logFile){
  string str = "";
  str += to_string(E.cy) + '\n';
  str += to_string(E.edit) + '\n';
  str += to_string(E.modified ) + '\n';
  char temp = E.prev - 48 ;
  str += to_string(temp) + '\n';
  char temp2 ;
  if(E.present == ' '){
     str += " \n";
  }
  else{
    temp2= E.present -48;
    str += to_string(temp2) + '\n';

  }
  char* s = const_cast<char*>(str.c_str());
  int len = str.length();
  int fd1;
  char buf[len+1];
  strcpy(buf, str.c_str());

    fd1 = open(logFile, O_RDWR | O_CREAT, 0644);
    if (fd1 == -1) {
        perror(logFile);
        exit(-1);
    }
    write(fd1, buf, strlen(buf)); 
    close(fd1);
}


void present_column(int n)
{

    FILE *fp = fopen("data.txt", "r");
    if (!fp) die("fopen");

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

   	vector <vector<string>> v;
    string s ;
    string data = "";
    int line_no = 1;
    while ((linelen = getline(&line, &linecap, fp)) != -1) {
      while (linelen > 0 && (line[linelen - 1] == '\n' ||
                            line[linelen - 1] == '\r'))
        linelen--;
        s = string(line, line + linelen);

        if(linelen > 0){
              char s1[s.size()+1];
              strcpy(s1, s.c_str());
              char* token = strtok(s1, " ");

              vector<string> tokens;
              while (token != NULL)
              {

                tokens.push_back(string(token));
                token = strtok(NULL, " ");
              }

              if(line_no++ == 1){
                  data += "            Faculty" + to_string(n) + "\n";
                  continue;
              }

			        data += " " + tokens[0] + "      " + tokens[n] + "       " + "\n";
        }      
    }
    free(line);
    fclose(fp);

    cout << "Data : \n" << data ; 

  int len = data.length();
  int fd1;
  char buf[len+1];
  strcpy(buf, data.c_str());

    string str = "Faculty" + to_string(n) + ".txt";
    char* logFile = const_cast<char*>(str.c_str());
 

    fd1 = open(logFile, O_RDWR | O_CREAT, 0644);
    if (fd1 == -1 || fd1 == 0) {
        perror("error creating logfile : ");
        exit(-1);
    }

    write(fd1, buf, strlen(buf));
    close(fd1);
  
}


void Faculty(int rc) {

      string str1 ,str2;

      str1 = "";
      str1 += "Faculty" + to_string(rc) + ".log";
      str2 = "";
      str2 += "Faculty" + to_string(rc) + ".txt";

      present_column(rc);


      initEditor();
      E.rc = rc;

      E.option = 2;

      char* c = const_cast<char*>(str2.c_str());
      enableRawMode();
      editorOpen(c);

      char* file = const_cast<char*>(str1.c_str());

      int fd = open(file, O_RDWR , 0644);
      if (fd != -1) {
         
      char buf[100] = {'\0'};

      int rd = read(fd, buf, 100 );
      close(fd);

      if(rd == -1){
        perror("Error! Unable to read  log file");
        exit(1);
      }

      string s = "";
      int index = 2;
      for(int i =0 ; i < 100 && buf[i] != '\0'; i++){
        if(buf[i] == '\n'){
           s += '\0';

          switch (index){
            case 2 :
              E.cy = stoi(s);
              break;
            case 3 :
              E.editAccess = 1;
              E.edit = stoi(s);
              if(E.edit){
                    editorSetStatusMessage("\u001b[32mPlease Enter the Marks\u001b[0m or \u001b[31mPress Esc to return back\u001b[0m");
                    E.edit = 1;
                    E.activeX = E.cx;

                    editorUpdateHighlight(&E.row[E.cy]);
                    E.cx++;
                    editorDelChar();
                    editorInsertChar(' ');
                    E.modified = 0;
              }
              break;
            case 4 : 
              E.modified = stoi(s);
              break;
            case 5 :
              if(E.edit && !E.modified){
                E.prev = s[0];
              }
              break;
            case 6 :
              if(E.edit && E.modified){
                E.present = s[0];
                editorDelChar();
                editorInsertChar(E.present);
              }
              break;
            case 7 :
              break;
            default :
              break;
          }
          s = "";
          index ++;
        }
        else{
          s += buf[i];
        }
      }

      }

      
      editorSetStatusMessage("\u001b[31mHELP\u001b[0m : Ctrl- q = quit, ENTER : Edit , ESC : Retain Previous value");

      while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
        WriteLog(file);
      }
      


}