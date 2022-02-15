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
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        void _cat(String path);        
        void _ls(String file);
        void _cd(String dir);
        void _rm(String file);
        void _mkdir(String path);
#endif        
};
#endif