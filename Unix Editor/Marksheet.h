#ifndef Marksheet_H
#define Marksheet_H

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


using namespace std;

#define CTRL_KEY(k) ((k) & 0x1f)
#define TAB_STOP 8
#define QUIT_TIMES 3


void Dean();
void Student(int arg1);
void Faculty(string arg1, string arg2, int rc);

#endif