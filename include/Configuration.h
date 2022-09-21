struct Configuration
{
    float playerHPMod = 1.0;
    float playerSpeedMod = 1.0;

    float projectileSpeedMod = 1.0;
    float projectileSizeMod = 1.0;

    float enemyHPMod = 1.0;
    float enemySpeedMod = 1.0;
    float enemyMaxSpeedMod = 1.0;
    float framesPerSecond = 60.0;
    float t = 1.0 / framesPerSecond;

    Configuration(float playerHPMod = 1.0,float playerSpeedMod = 1.0,float enemyHPMod = 1.0,float enemySpeedMod = 1.0,float enemyMaxSpeedMod = 1.0):
        playerHPMod(playerHPMod),playerSpeedMod(playerSpeedMod),enemyHPMod(enemyHPMod),enemySpeedMod(enemySpeedMod),enemyMaxSpeedMod(enemyMaxSpeedMod){};
};

Configuration g_config(1.0);
