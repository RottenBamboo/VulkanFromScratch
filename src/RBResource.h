#include <string>
namespace RottenBamboo {

    class RBResource {
    public:
        virtual void Load(const std::string& path) = 0;
        //virtual void* Get() = 0;
        virtual ~RBResource() = default;
    };
}