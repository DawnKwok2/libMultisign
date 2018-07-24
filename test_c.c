#include "include/c_multisig.h"
#include "include/cplus_multisig.h"
#include "include/c_sign.h"
#include "include/cplus_sign.h"
#include <stdio.h>

void test_multisig_c(){
//int main(){
    printf("I am test_multisig_c. Calling C++...\n");
    test_multisig();
    //return 0;
}

void test_sign_c(){
//int main(){
    printf("I am test_sign_c. Calling C++...\n");
    test_sign();
    //return 0;
}

//int main(){
//    test_multisig_c();
//    test_sign_c();
//    return 0;
//}
