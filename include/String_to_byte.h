#include<vector>
#include<string>

#ifndef _STRING_TO_BYTE_H
#define _STRING_TO_BYTE_H
using namespace std;
void String_to_byte(string s, vector<unsigned char> &v){
  for(int i=0;i<s.length();i+=2){
    char ch1 = s[i];
    char ch2 = s[i+1];
    int int1,int2;
    if(ch1>='a'){
      int1 = ch1 -'a'+10;
    } else {
      int1 = ch1 -'0';
    }
    if(ch2>='a'){
      int2 = ch2 - 'a' + 10;
    } else {
      int2 = ch2 - '0';
    }
    unsigned char c = int1*16+int2;
    // printf("%02x ",c);
    v.push_back((unsigned char) c);
  }
  // for(int i=0;i<v.size();i++){
  //   printf("%02x ",v[i]);
  // }
  // printf("\n");
}

void String_to_queue(string s, queue<unsigned char> &q){
  for(int i=0;i<s.length();i+=2){
    char ch1 = s[i];
    char ch2 = s[i+1];
    int int1,int2;
    if(ch1>='a'){
      int1 = ch1 -'a'+10;
    } else {
      int1 = ch1 -'0';
    }
    if(ch2>='a'){
      int2 = ch2 - 'a' + 10;
    } else {
      int2 = ch2 - '0';
    }
    unsigned char c = int1*16+int2;
    // printf("%02x ",c);
    q.push((unsigned char) c);
  }
}
#endif
