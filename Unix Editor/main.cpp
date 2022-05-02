#include <iostream>
using namespace std;
#include "Marksheet.h"


int main(){

    int option = 2;
    int rc = 0;

    

    // cout << "Press 1 to Dean or Press 2 to faculty or press 3 for Student\n";
    // cin >> edit ;

    switch (option){
        case 1 :
            Dean();
            break;
        case 2 :
            // cout << "Enter Faculty ID : \n";
            // cin >> rc;
            Faculty("Faculty3.log", "Faculty3_Display.txt");
            break;
        case 3 :
            Student("buf.txt");
            break;
        default :
            cout << " Invalid Option Choosen \n";
            break;
    }

}