#include <xinu.h>
#include <kv.h>
#include <xmalloc.h>


void kv_test() {

	int succ = kv_init();
	if (succ == 0) {
		printf("kv_init error!!\n");
		return;
	}

	int retval = 0; 
    int set_errors = 0; 
    char *valtmp = NULL; 
    int get_errors = 0; 

    retval = setKVHashTab("ctttjyhm", "rmxrodelucysiaytouifbtryfcdveypsjw"); 
    if (retval > 0) set_errors++; 
 
    retval = setKVHashTab("odzuzdge", "txegveaskmlwxkpnrzmurhsbyrrxqudjgrpxom"); 
    if (retval > 0) set_errors++; 
 
    retval = setKVHashTab("kgjdzmou", "ysrmvfneqeyjllycbxqmtyryhrdibymzok"); 
    if (retval > 0) set_errors++; 
 
    retval = setKVHashTab("ijqoehjq", "gzirqtbalnhriihrbuusfuaqrpkopniecqsqdtzpxqdst"); 
    if (retval > 0) set_errors++; 
 
    retval = setKVHashTab("ynlqrbas", "gyyempdvkcdzonpilwvqfqmpsqpbmmrscawqgbuqpdagmazpvzehkocwuuyoxxrp"); 
    if (retval > 0) set_errors++; 
 
    retval = setKVHashTab("hakqxkkc", "yfcoxahjbzpcyallsgioezlyqnsbadvqxt"); 
    if (retval > 0) set_errors++; 
 
    retval = setKVHashTab("wkgftqxb", "dtpcbbnqzqjhuveyxehkebiawcemoolsl"); 
    if (retval > 0) set_errors++; 
 
    retval = setKVHashTab("mvgjjgrv", "yihhepvzdsrwqpummctenmuwszqwnarxz"); 
    if (retval > 0) set_errors++; 
 
    retval = setKVHashTab("rfhwuoip", "gawhpcruuirnunlpaqfznwiwcreaqirmm"); 
    if (retval > 0) set_errors++; 
 
    retval = setKVHashTab("nvyibnyx", "lkjjggxjzkbowapsrilfgfkdatrycrktrsxbmimueoobazvpacgjmsfvgrgxscpfopkowjzcjxq"); 
    if (retval > 0) set_errors++; 

    valtmp = kv_get("ijqoehjq"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", valtmp);

    valtmp = kv_get("odzuzdge"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"odzuzdge\" value: \"%s\"\n", valtmp);

    valtmp = kv_get("ijqoehjq"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", valtmp);

    valtmp = kv_get("kgjdzmou"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", valtmp);

    valtmp = kv_get("ijqoehjq"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", valtmp);

    valtmp = kv_get("kgjdzmou"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", valtmp);

    valtmp = kv_get("rfhwuoip"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"rfhwuoip\" value: \"%s\"\n", valtmp);

    valtmp = kv_get("kgjdzmou"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", valtmp);

    valtmp = kv_get("hakqxkkc"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"hakqxkkc\" value: \"%s\"\n", valtmp);

    valtmp = kv_get("odzuzdge"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"odzuzdge\" value: \"%s\"\n", valtmp);

	printf("set errors: %d, get errors: %d. \n", set_errors, get_errors); 
}
