


#pragma once

#include "../../engine/LConfiguration.h"
#include <iostream>


using namespace std;

namespace diversifiers
{


    class LBaseDiversifier
    {

    public :

        LBaseDiversifier()
        {
            
        }

        ~LBaseDiversifier()
        {
            
        }

        virtual void run( engine::LConfiguration* pConfiguration ) = 0;


    };



}
