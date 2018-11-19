#include <xinu.h>
#include <kv.h>


void kv_test() {

	int succ = kv_init();
	if (succ > 0) {
		printf("kv_init error!!\n");
		return;
	}

	int retval = 0; 
	KVNode_t *valtmp = NULL; 
    int set_errors = 0; 
    int get_errors = 0; 
    int del_errors = 0;

    retval = setKVHashTab("ctttjyhm", "rmxrodelucysiaytouifbtryfcdveypsjw"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");
 
    retval = setKVHashTab("odzuzdge", "txegveaskmlwxkpnrzmurhsbyrrxqudjgrpxom"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");
 
    retval = setKVHashTab("kgjdzmou", "ysrmvfneqeyjllycbxqmtyryhrdibymzok"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");
 
    retval = setKVHashTab("ijqoehjq", "gzirqtbalnhriihrbuusfuaqrpkopniecqsqdtzpxqdst"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");
 
    retval = setKVHashTab("ynlqrbas", "gyyempdvkcdzonpilwvqfqmpsqpbmmrscawqgbuqpdagmazpvzehkocwuuyoxxrp"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");
 
    retval = setKVHashTab("hakqxkkc", "yfcoxahjbzpcyallsgioezlyqnsbadvqxt"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");
 
    retval = setKVHashTab("wkgftqxb", "dtpcbbnqzqjhuveyxehkebiawcemoolsl"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");
 
    retval = setKVHashTab("mvgjjgrv", "yihhepvzdsrwqpummctenmuwszqwnarxz"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");
 
    retval = setKVHashTab("rfhwuoip", "gawhpcruuirnunlpaqfznwiwcreaqirmm"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");
 
    retval = setKVHashTab("nvyibnyx", "lkjjggxjzkbowapsrilfgfkdatrycrktrsxbmimueoobazvpacgjmsfvgrgxscpfopkowjzcjxq"); 
    if (retval > 0) set_errors++; 
    else printf("set succeed!\n");

    retval = delKVHashTab("nvyibnyx");
    if (retval > 0) del_errors++;
    else printf("delete \"nvyibnyx\" succeed!\n");

    retval = delKVHashTab("rfhwuoip"); 
    if (retval > 0) del_errors++; 
    else printf("delete \"rfhwuoip\" succeed!\n");

    retval = delKVHashTab("mvgjjgrv"); 
    if (retval > 0) set_errors++; 
    else printf("delete \"mvgjjgrv\" succeed!\n");

    valtmp = getKVHashTab("ijqoehjq"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", valtmp->val);

    valtmp = getKVHashTab("odzuzdge"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"odzuzdge\" value: \"%s\"\n", valtmp->val);

    valtmp = getKVHashTab("ijqoehjq"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", valtmp->val);

    valtmp = getKVHashTab("kgjdzmou"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", valtmp->val);

    valtmp = getKVHashTab("ijqoehjq"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", valtmp->val);

    valtmp = getKVHashTab("kgjdzmou"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", valtmp->val);

    valtmp = getKVHashTab("rfhwuoip"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"rfhwuoip\" value: \"%s\"\n", valtmp->val);

    valtmp = getKVHashTab("kgjdzmou"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", valtmp->val);

    valtmp = getKVHashTab("hakqxkkc"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"hakqxkkc\" value: \"%s\"\n", valtmp->val);

    valtmp = getKVHashTab("odzuzdge"); 
    if (valtmp == NULL) get_errors++; 
    else printf("key: \"odzuzdge\" value: \"%s\"\n", valtmp->val);

	printf("set errors: %d, get errors: %d. \n", set_errors, get_errors); 
}
