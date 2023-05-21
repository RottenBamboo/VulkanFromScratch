//
// Created by rottenbamboo on 2023/5/16.
//

#include "RBApplication.h"

namespace RottenBamboo
{
    void RBApplication::run()
    {
        while (!windows.shouldClose())
        {
            glfwPollEvents();
        }
    }
}