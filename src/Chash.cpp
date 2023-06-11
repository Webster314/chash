#include <map>
#include <vector>
#include <string>
#include <initializer_list> 
#include <iterator>
#include <iostream>
#include <murmur3.h>

class Chash
{
private:
    const static std::string prefix;
    const static uint32_t hash_seed;
    const static int virtual_count = 30;
    std::map<__uint128_t, std::string> virtual_map;
public:
    ~Chash();
    Chash();
    Chash(std::initializer_list<std::string> nodes);
    void initMap(const std::initializer_list<std::string> & nodes);
    void addNode(const std::initializer_list<std::string> & nodes);
    std::string findNode(const std::string & key);
    void removeNode(const std::string & node);
};

Chash::Chash(std::initializer_list<std::string> nodes){
    initMap(nodes);
}

Chash::Chash()
{

}

Chash::~Chash()
{

}

void
Chash::removeNode(const std::string & node){
    __uint128_t out;
    // erase virtual node.
    for(int i = 0; i < virtual_count; i++){
        std::string virtual_node = node + prefix + std::to_string(i);
        MurmurHash3_x86_128(&virtual_node[0], virtual_node.length(), hash_seed, &out);
        virtual_map.erase(out);
    }
}

std::string 
Chash::findNode(const std::string & key){
    __uint128_t out;
    MurmurHash3_x86_128(&key[0], key.length(), hash_seed, &out);
    if(!virtual_map.count(out)){
        std::map<__uint128_t, std::string>::iterator ret = virtual_map.upper_bound(out);
        return ret == virtual_map.end() ? virtual_map[0] : ret->second.substr(0, ret->second.find(prefix));
    }
    std::string virtual_node = virtual_map[out];
    return virtual_map[out].substr(0, virtual_node.find(prefix));
}

void
Chash::addNode(const std::initializer_list<std::string> & nodes){
    __uint128_t out;
    for(std::string node : nodes){
        // virtual node mapping.
        for(int i = 0; i < virtual_count; i++){
            std::string virtual_node = node + prefix + std::to_string(i);
            MurmurHash3_x86_128(&virtual_node[0], virtual_node.length(), hash_seed, &out);
            virtual_map[out] = virtual_node;
            virtual_map[out] = virtual_node;
        }
    }  
}
void
Chash::initMap(const std::initializer_list<std::string> & nodes){
    addNode(nodes);
}

// 虚拟节点名字不能有'#'
const std::string Chash::prefix = "#";
const uint32_t Chash::hash_seed = 12;

int
main(void)
{
    Chash myhash({"a", "b", "c"});
    std::cout << myhash.findNode("asdf.mp4") << " ";
    std::cout << myhash.findNode("wbxpage.html") << " ";
    std::cout << myhash.findNode("default.html") << " ";
    std::cout << myhash.findNode("david.jpg") << " ";   
    std::cout << std::endl;
    myhash.addNode({"d", "e", "f"});
    std::cout << myhash.findNode("asdf.mp4") << " ";
    std::cout << myhash.findNode("wbxpage.html") << " ";
    std::cout << myhash.findNode("default.html") << " ";
    std::cout << myhash.findNode("david.jpg") << " ";   
    std::cout << std::endl;
    myhash.removeNode("b");
    myhash.removeNode("e");
    std::cout << myhash.findNode("asdf.mp4") << " ";
    std::cout << myhash.findNode("wbxpage.html") << " ";
    std::cout << myhash.findNode("default.html") << " ";
    std::cout << myhash.findNode("david.jpg") << " ";   
    std::cout << std::endl;
    return 0;
}
