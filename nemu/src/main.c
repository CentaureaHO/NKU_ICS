int  init_monitor(int, char* []);
void ui_mainloop(int);
void resource_clean();

int main(int argc, char* argv[])
{
    /* Initialize the monitor. */
    int is_batch_mode = init_monitor(argc, argv);

    /* Receive commands from user. */
    ui_mainloop(is_batch_mode);

<<<<<<< HEAD
=======
    /* Quit the monitor. */
    resource_clean();

>>>>>>> master
    return 0;
}
