#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

string WebSearch(string query) {
  int i=0;
  for(i; i<query.size(); i++) {
  if(query[i]==' ') { //Khoang trang (space) thay bang ky tu "+"
  query=query.replace(query.find(" ",0), 1, "+"); 
  }
  else if(query[i]=='+') { //Ky tu "+" thay bang "%2B"
  query=query.replace(query.find("+",0), 1, "%2B");
  }
  }
  string url="http://www.google.com/search?q=";
  url.insert(url.size(), query);
  return url.c_str(); 
}
int main()
{   
    string s;
    getline(cin,s);
    s=WebSearch(s);
    ShellExecute(NULL, "open", s.c_str(), NULL, NULL, SW_SHOWNORMAL); 
    return 0;
}  
