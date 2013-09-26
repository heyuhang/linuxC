#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#ifdef TIOCGWINSZ
#include <sys/ioctl.h>
#endif
#include <asm/ioctls.h>

struct winsize {
        unsigned short ws_row;
        unsigned short ws_col;
        unsigned short ws_xpixel;
        unsigned short ws_ypixel;
};

static void pr_winsize(int), sig_winch(int);

#define STDIN_FILENO 0

main()
{
        if(isatty(STDIN_FILENO) == 0)
                exit(0);

        if(signal(SIGWINCH, sig_winch) == SIG_ERR)
                exit(0);

        pr_winsize(STDIN_FILENO);
        for(;;)
                pause();
}

static void pr_winsize(int fd)
{
        struct winsize size;
        if(ioctl(fd, TIOCGWINSZ, (char*)&size) < 0)
                exit(0);
        printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
}

static void sig_winch(int signo)
{
        printf("SIGWINCH recevied\n");
        pr_winsize(STDIN_FILENO);
        return;
}
