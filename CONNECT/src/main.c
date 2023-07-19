
extern void dali_connect_app_initialize(void);
extern void dali_connect_app_run(void);
extern void dali_connect_app_terminate(void);

int main(void) {
    dali_connect_app_initialize();
    dali_connect_app_run();
    dali_connect_app_terminate();
    return 0;
}