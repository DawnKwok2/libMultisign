#ifndef _SIGN_H
#define _SIGN_H

#include "Schnorr.cpp"
#include "multisig.h"
#include "String_to_byte.h"
#include <vector>
#include <queue>

class SignAlgorithm{

  public:
    void generate(int num=1){
      CCurve curve;
      for(int i=0;i<num;i++){
        curve.GenerateKeys();
        // retrieve public keys
        Integer pk1, pk2;
        pk1 = curve.GetPublicKeyX();
        pk2 = curve.GetPublicKeyY();
        // cout << "pk1: " << pk1 << endl;
        // cout << "pk2: " << pk2 << endl;
        std::vector<unsigned char> vchPubKey;
	      if (!curve.GetVchPublicKey(vchPubKey))
		      cout << "GetVchPublicKey failed" << endl;
        printf("\nvchPubkey is:\n");
        for(int i=0;i<vchPubKey.size();i++){
          printf("%02x",vchPubKey[i]);
        }
        printf("\n");

        Integer sk1;
        sk1 = curve.GetSecretKey();
        std::vector<unsigned char> vchSecretKey;
        if (!curve.GetVchSecretKey(vchSecretKey))
          cout << "GetVchSecretKey failed" << endl;
        printf("\nprivkey is:\n");
        for(int i=0;i<vchSecretKey.size();i++){
          printf("%02x",vchSecretKey[i]);
        }
        printf("\n");
      }
    };

    queue<unsigned char> sign(vector<unsigned char> message, int m, int n, vector<string> pubkeys){
      Multisig multisig(m,pubkeys,n);
      queue<unsigned char> result;
      multisig.to_byte(& result);

      // combine message and result
      // while(!message.empty()){
        // message.push_back(result.front()); // should reverse
        // result.pop();
      // }
      for(int i=0;i<message.size();i++){
        result.push(message[i]);        
      }

      cout<<endl<<"add addr to message is:"<<endl;
      // output
      // for(int i=0;i<message.size();i++){
        // printf("%02x",message[i]);
      // }
      queue<unsigned char> tmp(result);
      while(!tmp.empty()){
	    	printf("%02x",tmp.front());
		    tmp.pop();
	    }
      printf("\n");
      return result;
    };

    bool verify(queue<unsigned char> message, vector<string> privateKey){
      // first get the pubkeys
      // message is:
      //02
      //03
      //21
      //022f568c7bdc30a66c98664041728302e00a8a115f30f104cc7f7ae72e54db7d3821030be53a6d71af0427f2765f3d524452c87d674b21f0eb922a04b1f5e2e380e60521021e01f704366d7f92364d651c15f388f2fc63d052f7d73f2abacd82abe8aee239
      //0001020304

      cout<<endl<<"verifying"<<endl;
      // while(!message.empty()){
    	// 	printf("%02x",message.front());
	    // 	message.pop();
	    // }
	    // cout<<endl;

      int required = (int)message.front();  // m
      message.pop();
      cout<<"required is:"<<required<<endl;
      int total = (int)message.front();  // n
      message.pop();
      cout<<"total is:"<<total<<endl;

      if(privateKey.size()==0){
        cout<<"the key pass into is 0, error"<<endl;
        return false;
      }

      if(privateKey.size()!=required){
        cout<<"the key pass into not equals required, error"<<endl;
        return false;
      }

      if(privateKey.size()>total){
        cout<<"the key pass into is greater than total, error"<<endl;
        return false;
      }

      vector<string> pubkey;
      for(int i=0;i<total;i++){  // pubkeys
        int string_size = (int)message.front();
        message.pop();
        string s;
        for(int j=0;j<string_size;j++){  //pubkey
          unsigned char ch1 = 0xf0;
					unsigned char ch2 = 0x0f;
					unsigned char c1 = (message.front()&ch1)>>4;
					unsigned char c2 = (message.front()&ch2);
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
					// printf("%02x %02x ", c1,c2);
					message.pop();
					s+=c1;
					s+=c2;
        }
        pubkey.push_back(s);
      }
      
      if(pubkey.size()!=total){
        cout<<"pubkey size is not equals total, message error"<<endl;
        return false;
      }

      // left is message
      cout<<"pubkeys are:"<<endl;
      for(int i=0;i<total;i++){
        cout<<"key"<<i<<" is:"<<endl;
        cout<<pubkey[i]<<endl;
      }


      int count=0;
      for(int i=0;i<total;i++){
        string s_pub = pubkey[i];
        for(int j=0;j<required;j++){
          string s_priv = privateKey[j];
          if(match(s_pub, s_priv)){
            cout<<"match one!!!"<<endl;
            count++;
            // remove private key

            break;
          }
        }
      }


      if(count==required){
        cout<<"verify pass!!!"<<endl;
        return true;
      } else {
        cout<<"verify not pass!!!"<<endl;
        return false;
      }

      //     return true;
    };

    bool match(string pubKey, string privateKey){
      std::vector<unsigned char> pub,priv;
      String_to_byte(pubKey,pub);
      String_to_byte(privateKey,priv);
      
      // cout<<"pubkey to byte is:"<<endl;
      // for(int i=0;i<pub.size();i++){
      //   printf("%02x ",pub[i]);
      // }

      // cout<<endl<<"private key to byte is:"<<endl;
      // for(int i=0;i<priv.size();i++){
      //   printf("%02x ",priv[i]);
      // }

      CCurve c2;
      c2.GenerateKeys(); // mush have 1
      // std::vector<unsigned char> vchSecretKey;
      if (!c2.SetVchSecretKey(priv))  // must have 2
        cout << "SetVchSecretKey failed" << endl;
      c2.GeneratePublicKey();  // must have 3
      std::vector<unsigned char> vchPubKey;
	    if (!c2.GetVchPublicKey(vchPubKey))
		    cout << "GetVchPublicKey failed" << endl;
      
      // check pubkey and current pubkey
      if(vchPubKey.size() != pub.size() ){
        cout<<"size not equal"<<endl;
        return false;
      }
      for(int i=0;i<pub.size();i++){
        if(vchPubKey[i]!=pub[i]){
          return false;
        }
      }
      return true;
    }



};
#endif
