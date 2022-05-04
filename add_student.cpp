#include<iostream>
#include <bits/stdc++.h>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;
#include "Marksheet.cpp"


void add_student(string x)
{
    FILE *fp = fopen("data.txt", "r");
    if (!fp) {
        cout << "Unable to open file" << endl;
        die("fopen");
        exit(-1);
    };

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    string s ;
    string data = "";
    int line_no = 0;
    while ((linelen = getline(&line, &linecap, fp)) != -1) {
      while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
        linelen--;
        s = string(line, line + linelen);

        if(linelen > 0){
            data += s +"\n" ;
          line_no++;   
        }   
    }
    free(line);
    fclose(fp);

    data += " " + x + "      -        -        -        -        00       ";

    int len = data.length();
    int fd1;
    char buf[len+1];
    strcpy(buf, data.c_str());
 

    fd1 = open("data.txt", O_RDWR | O_CREAT, 0644);
    if (fd1 == -1 || fd1 == 0) {
        perror("error adding user to datafile : ");
        exit(-1);
    }

    write(fd1, buf, strlen(buf));
    close(fd1);

}

int main()
{
    add_student("Cs19bb00");
}