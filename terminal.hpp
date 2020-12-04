#ifndef TERMINAL
#define TERMINAL

#include <WString.h>

class Terminal {
    public:
        void run();
        Terminal();
    private:
        String _cwd;
        String _path(String file);
        void _cd(String dir);
        void _ls(String file);
        void _rm(String file);
        void _mkdir(String path);
        void _cat(String path);
};

#endif