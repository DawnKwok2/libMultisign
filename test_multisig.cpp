#include "include/cplus_multisig.h"
#include "include/multisig.h"

//int main(int argc, char** argv){
//	cout << "I am C++." << endl;
void test_multisig(){
	vector<string> v;
	cout << "I am C++. Executing..." << endl;	v.push_back("022f568c7bdc30a66c98664041728302e00a8a115f30f104cc7f7ae72e54db7d38");
	v.push_back("030be53a6d71af0427f2765f3d524452c87d674b21f0eb922a04b1f5e2e380e605");
	v.push_back("021e01f704366d7f92364d651c15f388f2fc63d052f7d73f2abacd82abe8aee239");
	Multisig multisig(1,v,3);
	queue<unsigned char> result;
	multisig.to_byte(&result);
	Multisig m(&result);
	//return 0;
}
