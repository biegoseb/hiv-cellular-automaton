#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>

template<typename Resource>
class ResourceManager {
    using CrString = const std::string&;
    private:
        std::string getFullname(CrString name);
        const std::string myFolder;
        const std::string myExtention;
        std::unordered_map<std::string, Resource> myResources;
    public:
        ResourceManager (CrString folder, CrString extention);
        const Resource& get(CrString name);
        bool exists(CrString name) const;
        void add(CrString name);  
};

template<typename Resource>
std::string ResourceManager<Resource>::getFullname(CrString name) {
    return myFolder + name + myExtention;
}

template<typename Resource>
ResourceManager<Resource>::ResourceManager(CrString folder, CrString extention)
    : myFolder("resources/" + folder + "/"), myExtention("." + extention) { }

template<typename Resource>
const Resource& ResourceManager<Resource>::get(CrString name) {
    if (!exists(name)) {
        add(name);
    }
    return myResources.at(name);
}

template<typename Resource>
bool ResourceManager<Resource>::exists(CrString name) const {
    return myResources.find(name) != myResources.end();
}

template<typename Resource>
void ResourceManager<Resource>::add(CrString name) {
    Resource r;
    //if the resource fails to load, then it adds a default "fail" resource
    if(!r.loadFromFile(getFullname(name))) {
        Resource fail;
        fail.loadFromFile(myFolder + "_fail_" + myExtention);
        myResources.insert(std::make_pair(name, fail));
    } else {
        myResources.insert(std::make_pair(name, r));
    }
} 

#endif // RESOURCE_MANAGER_H