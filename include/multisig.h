#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<stdio.h>

#ifndef _MULTISIG_H
#define _MULTISIG_H
#include<stdlib.h>
using namespace std;

class Multisig{
	private:
		int required_pubkey_num;
		vector<string> pubkeys;
		int total_pubkey_num;
		string check_sum;
	public:
		Multisig(){};
		Multisig(int required,vector<string> v,int total){
			if(required<=0 || total<=0){
				cout<<"required or total number shouldn't less than zero"<<endl;
				return;
			}
			if(v.size() != total){
				cout<<"pubkey's number not equal"<<endl;				
				return;
			}
			if(required > total){
				cout<<"require shouldn't larger than total"<<endl;				
				return;
			}
			this->required_pubkey_num = required;
			for(int i=0; i<v.size(); i++){
				this->pubkeys.push_back(v[i]);
			}
			this->total_pubkey_num = total;
			this->calculate_checksum();
		}
		Multisig(queue<unsigned char> *v){
			// for(queue<unsigned char>::iterator it = v->begin(); it != v->end(); it++){  // to be deleted
			// 	printf("%04x\t",*it);
			// 	// cout<<ios::hex<<result[i]<<endl;
			// }
			// cout<<endl;
			// int int_size = (unsigned int) v->front();
			// cout<<"v->back is:"<<(unsigned int)v->front()<<endl;
			// v->pop();
			int required = (int) v->front();
			v->pop();
			int total = (int) v->front();
			v->pop();

			vector<string> keys;
			for(int i=0; i<total; i++){
				int string_size = (int) v->front();
				v->pop();
				string s;
				for(int j=0; j<string_size; j++){
					unsigned char ch1 = 0xf0;
					unsigned char ch2 = 0x0f;
					unsigned char c1 = (v->front()&ch1)>>4;
					unsigned char c2 = (v->front()&ch2);
					if(c1>=0x0a){
						c1 = 'a' + (c1-0x0a);
					} else {
						c1 =  '0' + c1;
					}
					if(c2>=0x0a){
						c2 = 'a' + (c2-0x0a);
					} else {
						c2 = '0' + c2;
					}
					//char c = (char) v->front();
					//printf("%02x %02x ", c1,c2);
					v->pop();
					s+=c1;
					s+=c2;
				}
				keys.push_back(s);
			}
			// cout<<"int size is:"<<int_size<<endl;
			cout<<endl<<endl<<"after transform:"<<endl;
			cout<<"required:"<<required<<endl;
			cout<<"total:"<<total<<endl;
			cout<<"keys:"<<endl;
			for(int i=0;i<keys.size();i++){
				cout<<keys[i]<<endl;
			}

			// OUTPUT
			// total:3
			// keys:
			// 022f568c7bdc30a66c98664041728302e00a8a115f30f104cc7f7ae72e54db7d38
			// 030be53a6d71af0427f2765f3d524452c87d674b21f0eb922a04b1f5e2e380e605
			// 021e01f704366d7f92364d651c15f388f2fc63d052f7d73f2abacd82abe8aee239
		}
		~Multisig(){};
		
		void calculate_checksum(){
			this->check_sum = "";  // to be finished
		}

		void to_byte(queue<unsigned char> *result){
			// result->push(sizeof(int));  // output 4
			result->push(this->required_pubkey_num);  // 4 bytes
			result->push(this->total_pubkey_num);  // 4 bytes

			for(int i=0; i<this->pubkeys.size(); i++){
				result->push( (unsigned char) this->pubkeys[i].size()/2 );
				for(int j=0; j<this->pubkeys[i].size(); j+=2){
					char ch1 = this->pubkeys[i][j];
					char ch2 = this->pubkeys[i][j+1];
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
					result->push((unsigned char) c);
				}
			}
			queue<unsigned char> tmp(*result);
			cout<<endl<<endl<<"pubkeys to byte result is:"<<endl;
				while(!tmp.empty()){
				printf("%02x",tmp.front());
				tmp.pop();
				}
	// cout<<endl;
			cout<<endl;
		};

		void print(){
			cout<<"required_pubkey_num:"<<this->required_pubkey_num<<endl;
			cout<<"pubkeys:"<<endl;
			for(int i=0; i<this->pubkeys.size(); i++){
				cout<<this->pubkeys[i]<<endl;
			}
			cout<<"total_pubkey_num:"<<this->total_pubkey_num<<endl;
		}
};
#endif
