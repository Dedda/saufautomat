#ifndef CONFIG
#define CONFIG

class Config {
    public:
        int rotationTime;
        int splashTime;
        int gitHubAdTime;
        int exportInfoTime;
        int wowTime;
        Config();
        void list();
        void setDefault();
};

#endif