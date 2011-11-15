#include "tk.h"

extern int Tbl_AppInit ();

int main(argc, argv)
    int argc;			/* Number of command-line arguments. */
    char **argv;		/* Values of command-line arguments. */
{
    Tk_Main(argc, argv, Tbl_AppInit);
    return 0;			/* Needed only to prevent compiler warning. */
}
