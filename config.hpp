#ifndef CONFIG
#define CONFIG

class Config {
    public:
        unsigned int rotationTime;
        unsigned int splashTime;
        unsigned int gitHubAdTime;
        unsigned int exportInfoTime;
        unsigned int wowTime;
        Config();
        void list();
        void setDefault();
};

#endif