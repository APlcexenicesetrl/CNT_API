#pragma once

#include <iostream>
#include <string>

namespace cnt
{
    struct Vector3
    {
        int X;
        int Y;
        int Z;
        void Zero()
        {
            X = 0;
            Y = 0;
            Z = 0;
        }
        void transformX()
        {
            Zero();
            X = 1;
        }
        void transformY()
        {
            Zero();
            Y = 1;
        }
        void transformZ()
        {
            Zero();
            Z = 1;
        }
    };

    struct Vector2
    {
        int X;
        int Y;
        void Zero()
        {
            X = 0;
            Y = 0;
        }
        void transformX()
        {
            Zero();
            X = 1;
        }
        void transformY()
        {
            Zero();
            Y = 1;
        }
    };
}
