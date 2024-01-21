#ifndef __FLAG_MANAGER__
#define __FLAG_MANAGER__

class FlagManager
{
private:
    bool isAutomatic;

public:
    FlagManager();
    bool getIsAutomatic();
    void setIsAutomatic(bool isAutomatic);
};

#endif