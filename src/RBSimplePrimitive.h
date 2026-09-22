#include "RBModel.h"
namespace RottenBamboo
{
    class RBSimplePrimitive
    {
        public:
        RBSimplePrimitive();
        void CreateCube();
        void CreateSphere();
        void CreateCylinder();
        void CreateCapsule();
        void CreatePlane();
        private:
        RBModel* model;
    };
}