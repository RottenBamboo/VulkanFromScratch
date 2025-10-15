#include <string>
namespace RottenBamboo {

    class RBResource {
    public:
        explicit RBResource(const std::string& path) : path(path) {}
        virtual void Load(const std::string& path) = 0;
        //virtual void* Get() = 0;
        virtual ~RBResource() = default;
    protected:
        std::string path;
    };
}