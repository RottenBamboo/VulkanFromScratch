#include "uuid.h"
namespace RottenBamboo 
{
    class RBFileSystem
    {
        public:
        virtual bool Save(const std::string& savePath) = 0;
        virtual bool Load(const std::string& loadPath) = 0;
        virtual ~RBFileSystem();
        RBFileSystem();
    };
}