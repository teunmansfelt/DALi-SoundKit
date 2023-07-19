
extern void dl_connect_app_initialize(void);
extern void dl_connect_app_run(void);
extern void dl_connect_app_terminate(void);

int main(void) {
    dl_connect_app_initialize();
    dl_connect_app_run();
    dl_connect_app_terminate();
    return 0;   // Superfluous, since 'dl_connect_app_terminate()' will call 'exit()'.
}
