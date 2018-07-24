#include "include/cplus_sign.h"
#include "include/sign.h"

//int main(int argc, char** argv){
void test_sign(){
	vector<string> v;
	cout << "I am C++. Executing..." << endl;
	v.push_back("0282252a8de4e360486affc429d4413cd76c90d85e8480981fa7eaac3090f99a66");
	v.push_back("025d3ae5ebf6e5775b1794710013e7bb9a51ee578524bb14cfc111f1e51d2ef6bb");
	v.push_back("022f568c7bdc30a66c98664041728302e00a8a115f30f104cc7f7ae72e54db7d38");

	vector<unsigned char> message = {0x00,0x01,0x02,0x03,0x04};
	queue<unsigned char> messages;
	vector<string> signature;
	signature.push_back("93fbdce99ab4b7cf8dd7578060f7253069aee8dd58b7c606587e87513df713c0");
	signature.push_back("1bd35ca19a2c5f893f2c19d020ada0a37f0bcc2d6be5afd74f5e8b9909c0f75a");

  SignAlgorithm s;
  // s.sign(message,3,11,v);
  // s.generate(3);
	messages = s.sign(message,2,3,v);
	s.verify(messages,signature);
	//return 0;
}
