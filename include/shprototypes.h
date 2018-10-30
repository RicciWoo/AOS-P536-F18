/* in file xsh_argecho.c */
extern	shellcmd  xsh_argecho	(int32, char *[]);

/* in file xsh_arp.c */
extern	shellcmd  xsh_arp	(int32, char *[]);

/* in file xsh_bingid.c */
extern	shellcmd  xsh_bingid	(int32, char *[]);

/* in file xsh_cat.c */
extern	shellcmd  xsh_cat	(int32, char *[]);

/* in file xsh_clear.c */
extern	shellcmd  xsh_clear	(int32, char *[]);

/* in file xsh_date.c */
extern	shellcmd  xsh_date	(int32, char *[]);

/* in file xsh_devdump.c */
extern	shellcmd  xsh_devdump	(int32, char *[]);

/* in file xsh_echo.c */
extern	shellcmd  xsh_echo	(int32, char *[]);

/* in file xsh_ethstat.c */
extern	shellcmd  xsh_ethstat	(int32, char *[]);

/* in file xsh_exit.c */
extern	shellcmd  xsh_exit	(int32, char *[]);

/* in file xsh_help.c */
extern	shellcmd  xsh_help	(int32, char *[]);

/* in file xsh_ipaddr.c */
extern	shellcmd  xsh_ipaddr	(int32, char *[]);

/* in file xsh_kill.c */
extern	shellcmd  xsh_kill	(int32, char *[]);

/* in file xsh_led.c */
extern	shellcmd  xsh_led	(int32, char *[]);

/* in file xsh_memdump.c */
extern	shellcmd  xsh_memdump	(int32, char *[]);

/* in file xsh_memstat.c */
extern	shellcmd  xsh_memstat	(int32, char *[]);

/* in file xsh_nvram.c */
extern	shellcmd  xsh_nvram	(int32, char *[]);

/* in file xsh_ping.c */
extern	shellcmd  xsh_ping	(int32, char *[]);

/* in file xsh_ps.c */
extern	shellcmd  xsh_ps	(int32, char *[]);

/* in file xsh_sleep.c */
extern	shellcmd  xsh_sleep	(int32, char *[]);

/* in file xsh_udpdump.c */
extern	shellcmd  xsh_udpdump	(int32, char *[]);

/* in file xsh_udpecho.c */
extern	shellcmd  xsh_udpecho	(int32, char *[]);

/* in file xsh_udpeserver.c */
extern	shellcmd  xsh_udpeserver (int32, char *[]);

/* in file xsh_uptime.c */
extern	shellcmd  xsh_uptime	(int32, char *[]);

/* in file xsh_help.c */
extern	shellcmd  xsh_help	(int32, char *[]);

/* in file xsh_mmu.c */
extern  shellcmd xsh_mmu	(int32, char *[]);

extern shellcmd xsh_create_example(int, char *[]);
extern shellcmd xsh_polling_example(int, char *[]);
extern shellcmd xsh_semaphore_example(int, char *[]);
extern shellcmd xsh_message_example(int, char *[]);
extern shellcmd xsh_arg_example(int, char*[]);
extern shellcmd xsh_fstest(int nargs, char *args[]);

/* in file shell/xsh_hello.c */
extern shellcmd xsh_hello(int, char *[]);

/* in file shell/xsh_prodcons.c */
extern shellcmd xsh_prodcons(int, char *[]);

/* in file shell/xsh_xmalloc_test.c */
extern shellcmd xsh_xmalloc_test(int, char *[]);
