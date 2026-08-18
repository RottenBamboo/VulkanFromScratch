#include "RBFileSystem.h"
namespace RottenBamboo
{
    class MetaFile : public RBFileSystem
    {
        public:
        bool Save(const std::string& savePath) override;
        bool Load(const std::string& loadPath) override;
        ~MetaFile() override;
        MetaFile();
    };
}