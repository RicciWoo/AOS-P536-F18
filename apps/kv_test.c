#include <xinu.h>
#include <kv.h>
#include <xmalloc.h>


void kv_test() {

	int succ = kv_init();
	if (succ == 0) {
		printf("kv_init error!!\n");
		return;
	}

	int retval; 
    int set_errors = 0; 
    char* valtmp=NULL; 
    int get_errors=0; 

    retval=kv_set("ctttjyhm", "rmxrodelucysiaytouifbtryfcdveypsjw"); 
     if(retval>0) set_errors++; 
 
    retval=kv_set("odzuzdge", "txegveaskmlwxkpnrzmurhsbyrrxqudjgrpxom"); 
     if(retval>0) set_errors++; 
 
    retval=kv_set("kgjdzmou", "ysrmvfneqeyjllycbxqmtyryhrdibymzok"); 
     if(retval>0) set_errors++; 
 
    retval=kv_set("ijqoehjq", "gzirqtbalnhriihrbuusfuaqrpkopniecqsqdtzpxqdst"); 
     if(retval>0) set_errors++; 
 
    retval=kv_set("ynlqrbas", "gyyempdvkcdzonpilwvqfqmpsqpbmmrscawqgbuqpdagmazpvzehkocwuuyoxxrp"); 
     if(retval>0) set_errors++; 
 
    retval=kv_set("hakqxkkc", "yfcoxahjbzpcyallsgioezlyqnsbadvqxt"); 
     if(retval>0) set_errors++; 
 
    retval=kv_set("wkgftqxb", "dtpcbbnqzqjhuveyxehkebiawcemoolsl"); 
     if(retval>0) set_errors++; 
 
    retval=kv_set("mvgjjgrv", "yihhepvzdsrwqpummctenmuwszqwnarxz"); 
     if(retval>0) set_errors++; 
 
    retval=kv_set("rfhwuoip", "gawhpcruuirnunlpaqfznwiwcreaqirmm"); 
     if(retval>0) set_errors++; 
 
    retval=kv_set("nvyibnyx", "lkjjggxjzkbowapsrilfgfkdatrycrktrsxbmimueoobazvpacgjmsfvgrgxscpfopkowjzcjxq"); 
     if(retval>0) set_errors++; 

	char *val = kv_get("ctttjyhm");

	printf("the value with key \"ctttjyhm\" is: %s\n", val);

	printf("%d, %d \n",set_errors, get_errors); 
}
