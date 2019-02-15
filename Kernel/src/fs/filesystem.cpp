#include <filesystem.h>

namespace fs{
    fs_node_t root;

    uint32_t read(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t *buffer){
        if(node->read)
            return node->read(node,offset,size,buffer);
        else return 0;
    }

    uint32_t write(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t *buffer){
        if(node->write)
            return node->write(node,offset,size,buffer);
        else return 0;
    }

    void open(fs_node_t* node, uint32_t flags){
        if(node->write)
            return node->open(node,flags);
    }

    void close(fs_node_t* node){
        if(node->write)
            return node->close(node);
    }

    fs_dirent_t* read_dir(fs_node_t* node, uint32_t index){
        if(node->read_dir)
            return node->read_dir(node,index);
        else return 0;
    }

    fs_node_t* find_dir(fs_node_t* node, char* name){
        if(node->find_dir)
            return node->find_dir(node,name);
        else return 0;
    }

    void init(){
        
    }
}