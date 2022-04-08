#include<iostream>
#include<unistd.h>

using namespace std;

int main()
{
  string ans;
  ans = getuid();
  cout << ans << endl;
  return 0;
}
