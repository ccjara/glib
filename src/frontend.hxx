#ifndef GLIB_FRONTEND_HXX
#define GLIB_FRONTEND_HXX

const char *get_usage() {
    return "glib, a set of tools to work with glib archives\n"
           "  usage: glib [operation] [path to archive]\n"
           "  \n"
           "  -h -? --help\t\tprint this screen\n"
           "  -l --list\t\tlist files and their size in the archive\n"
           "  -v --version\t\tdisplay the version of the glib library\n";
}

#endif
