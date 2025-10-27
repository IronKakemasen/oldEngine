#pragma once
#include "../../../MyLib/Utilities/CameraStructures/CameraBehavior.h"
#include <memory> 


class SampleCamera :public CameraBehavior
{
private:


public:

    //Constructor
    SampleCamera();
    //Destructor
    ~SampleCamera() {}
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Debug()override {};

};

