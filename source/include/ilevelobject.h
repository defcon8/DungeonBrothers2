#ifndef ILEVELOBJECT_H_
#define ILEVELOBJECT_H_

class iLevelObject {
public:
    virtual ~iLevelObject() {}
    virtual void update() = 0;
    virtual void render() = 0;
    virtual bool isAlive() = 0;
};

#endif /* ILEVELOBJECT_H_ */

