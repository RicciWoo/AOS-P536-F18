#include <xinu.h>
#include <kv.h>


void kv_test() {

	int succ = kv_init();
	if (succ > 0) {
		printf("kv_init error!!\n");
		return;
	}

	int retval = 0; 
	KVNode_t *kvNode = NULL; 
	LRUNode_t *lruNode = NULL;
    int set_errors = 0; 
    int get_errors = 0; 
    int del_errors = 0;

    // Test for functions for kv Hash table

    retval = setKVHashTab("ctttjyhm", "rmxrodelucysiaytouifbtryfcdveypsjw"); 
    if (retval > 0) set_errors++; 
    else printf("set \"ctttjyhm\" succeed!\n");
 
    retval = setKVHashTab("odzuzdge", "txegveaskmlwxkpnrzmurhsbyrrxqudjgrpxom"); 
    if (retval > 0) set_errors++; 
    else printf("set \"odzuzdge\"succeed!\n");
 
    retval = setKVHashTab("kgjdzmou", "ysrmvfneqeyjllycbxqmtyryhrdibymzok"); 
    if (retval > 0) set_errors++; 
    else printf("set \"kgjdzmou\"succeed!\n");
 
    retval = setKVHashTab("ijqoehjq", "gzirqtbalnhriihrbuusfuaqrpkopniecqsqdtzpxqdst"); 
    if (retval > 0) set_errors++; 
    else printf("set \"ijqoehjq\" succeed!\n");
 
    retval = setKVHashTab("ynlqrbas", "gyyempdvkcdzonpilwvqfqmpsqpbmmrscawqgbuqpdagmazpvzehkocwuuyoxxrp"); 
    if (retval > 0) set_errors++; 
    else printf("set \"ynlqrbas\" succeed!\n");
 
    retval = setKVHashTab("hakqxkkc", "yfcoxahjbzpcyallsgioezlyqnsbadvqxt"); 
    if (retval > 0) set_errors++; 
    else printf("set \"hakqxkkc\" succeed!\n");
 
    retval = setKVHashTab("wkgftqxb", "dtpcbbnqzqjhuveyxehkebiawcemoolsl"); 
    if (retval > 0) set_errors++; 
    else printf("set \"wkgftqxb\" succeed!\n");
 
    retval = setKVHashTab("mvgjjgrv", "yihhepvzdsrwqpummctenmuwszqwnarxz"); 
    if (retval > 0) set_errors++; 
    else printf("set \"mvgjjgrv\" succeed!\n");
 
    retval = setKVHashTab("rfhwuoip", "gawhpcruuirnunlpaqfznwiwcreaqirmm"); 
    if (retval > 0) set_errors++; 
    else printf("set \"rfhwuoip\" succeed!\n");
 
    retval = setKVHashTab("nvyibnyx", "lkjjggxjzkbowapsrilfgfkdatrycrktrsxbmimueoobazvpacgjmsfvgrgxscpfopkowjzcjxq"); 
    if (retval > 0) set_errors++; 
    else printf("set \"rfhwuoip\" succeed!\n");


    retval = delKVHashTab("nvyibnyx");
    if (retval > 0) del_errors++;
    else printf("delete \"nvyibnyx\" succeed!\n");

    retval = delKVHashTab("rfhwuoip"); 
    if (retval > 0) del_errors++; 
    else printf("delete \"rfhwuoip\" succeed!\n");

    retval = delKVHashTab("mvgjjgrv"); 
    if (retval > 0) del_errors++; 
    else printf("delete \"mvgjjgrv\" succeed!\n");

    retval = delKVHashTab("wkgftqxb"); 
    if (retval > 0) del_errors++; 
    else printf("delete \"wkgftqxb\" succeed!\n");

    retval = delKVHashTab("hakqxkkc"); 
    if (retval > 0) del_errors++; 
    else printf("delete \"hakqxkkc\" succeed!\n");
 

    kvNode = getKVHashTab("ijqoehjq"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", kvNode->val);

    kvNode = getKVHashTab("odzuzdge"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"odzuzdge\" value: \"%s\"\n", kvNode->val);

    kvNode = getKVHashTab("ijqoehjq"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", kvNode->val);

    kvNode = getKVHashTab("kgjdzmou"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", kvNode->val);

    kvNode = getKVHashTab("ijqoehjq"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", kvNode->val);

    kvNode = getKVHashTab("kgjdzmou"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", kvNode->val);

    kvNode = getKVHashTab("rfhwuoip"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"rfhwuoip\" value: \"%s\"\n", kvNode->val);

    kvNode = getKVHashTab("kgjdzmou"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", kvNode->val);

    kvNode = getKVHashTab("hakqxkkc"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"hakqxkkc\" value: \"%s\"\n", kvNode->val);

    kvNode = getKVHashTab("odzuzdge"); 
    if (kvNode == NULL) get_errors++; 
    else printf("key: \"odzuzdge\" value: \"%s\"\n", kvNode->val);

	printf("set errors: %d, del errors: %d, get errors: %d. \n", 
		   set_errors, del_errors, get_errors); 

	// test for funtions on LRU hash table

    set_errors = 0; 
    get_errors = 0; 
    del_errors = 0;

    retval = setLRUHashTab("ctttjyhm", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"ctttjyhm\" succeed!\n");
 
    retval = setLRUHashTab("odzuzdge", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"odzuzdge\"succeed!\n");
 
    retval = setLRUHashTab("kgjdzmou", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"kgjdzmou\"succeed!\n");
 
    retval = setLRUHashTab("ijqoehjq", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"ijqoehjq\" succeed!\n");
 
    retval = setLRUHashTab("ynlqrbas", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"ynlqrbas\" succeed!\n");
 
    retval = setLRUHashTab("hakqxkkc", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"hakqxkkc\" succeed!\n");
 
    retval = setLRUHashTab("wkgftqxb", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"wkgftqxb\" succeed!\n");
 
    retval = setLRUHashTab("mvgjjgrv", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"mvgjjgrv\" succeed!\n");
 
    retval = setLRUHashTab("rfhwuoip", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"rfhwuoip\" succeed!\n");
 
    retval = setLRUHashTab("nvyibnyx", NULL); 
    if (retval > 0) set_errors++; 
    else printf("set \"rfhwuoip\" succeed!\n");


    retval = delLRUHashTab("nvyibnyx");
    if (retval > 0) del_errors++;
    else printf("delete \"nvyibnyx\" succeed!\n");

    retval = delLRUHashTab("rfhwuoip"); 
    if (retval > 0) del_errors++; 
    else printf("delete \"rfhwuoip\" succeed!\n");

    retval = delLRUHashTab("mvgjjgrv"); 
    if (retval > 0) del_errors++; 
    else printf("delete \"mvgjjgrv\" succeed!\n");

    retval = delLRUHashTab("wkgftqxb"); 
    if (retval > 0) del_errors++; 
    else printf("delete \"wkgftqxb\" succeed!\n");

    retval = delLRUHashTab("hakqxkkc"); 
    if (retval > 0) del_errors++; 
    else printf("delete \"hakqxkkc\" succeed!\n");
 

    lruNode = getLRUHashTab("ijqoehjq"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", lruNode->prev);

    lruNode = getLRUHashTab("odzuzdge"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"odzuzdge\" value: \"%s\"\n", lruNode->prev);

    lruNode = getLRUHashTab("ijqoehjq"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", lruNode->prev);

    lruNode = getLRUHashTab("kgjdzmou"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", lruNode->prev);

    lruNode = getLRUHashTab("ijqoehjq"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"ijqoehjq\" value: \"%s\"\n", lruNode->prev);

    lruNode = getLRUHashTab("kgjdzmou"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", lruNode->prev);

    lruNode = getLRUHashTab("rfhwuoip"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"rfhwuoip\" value: \"%s\"\n", lruNode->prev);

    lruNode = getLRUHashTab("kgjdzmou"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"kgjdzmou\" value: \"%s\"\n", lruNode->prev);

    lruNode = getLRUHashTab("hakqxkkc"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"hakqxkkc\" value: \"%s\"\n", lruNode->prev);

    lruNode = getLRUHashTab("odzuzdge"); 
    if (lruNode == NULL) get_errors++; 
    else printf("key: \"odzuzdge\" value: \"%s\"\n", lruNode->prev);

	printf("set errors: %d, del errors: %d, get errors: %d. \n", 
		   set_errors, del_errors, get_errors); 
}
