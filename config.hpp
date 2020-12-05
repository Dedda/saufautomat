#ifndef CONFIG
#define CONFIG

class Config {
    public:
        int rotationSpeed;
        int splashTime;
        int gitHubAdTime;
        int exportInfoTime;
        int wowTime;
        Config();
        void list();
        void setDefault();
        int rotationWaitTime();
};

#endif