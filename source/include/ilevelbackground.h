#ifndef ILEVELBACKGROUND_H_
#define ILEVELBACKGROUND_H_

class iLevelBackground {
public:
    virtual ~iLevelBackground() {}
    virtual void render(signed int camx, signed int camy) = 0;
};

#endif /* ILEVELBACKGROUND_H_ */

