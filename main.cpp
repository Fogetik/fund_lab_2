#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include "exceptions.h"

using std::map;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::fstream;

#define u8 uint8_t
#define HEAP_SIZE 128
#define HEADER 4

u8 heap[HEAP_SIZE];

struct free_memory1{
    u8* address;
    int size;
};

struct free_memory2{
    bool tag;//true = +, false = -
    u8* address;
    int size;
};

free_memory1 entries1[HEAP_SIZE / 2];

free_memory2 entries2[HEAP_SIZE];

void* malloc_cpy1(size_t size);

void free_cpy1(void* ptr);

void log1();

void task1();

void* malloc_cpy2(size_t size);

void free_cpy2(void* ptr);

void log2();

void task2();

void test();

int main() {
    entries1[0].address = heap;
    entries1[0].size = HEAP_SIZE;
    entries2[0].address = heap;
    entries2[0].size = HEAP_SIZE;
    entries2[0].tag = false;
    try{
        test();
//        task1();
    }
    catch (std::exception *e){
        cout << e->what() << endl;
    }
    return 0;
}

void test() {
    typedef struct foo
    {
        int a;
        int b;
    }foo_t;

    foo_t* foo;
    int* bazz;
    char* bar;

    foo = static_cast<foo_t *>(malloc_cpy1(sizeof(foo_t)));
    bar = static_cast<char *>(malloc_cpy1(5));
    bazz = static_cast<int *>(malloc_cpy1(sizeof(int)));
    foo->a = 5;
    foo->b = 10;
    strcpy(bar, "bar");
    memcpy(bazz, &foo->a, sizeof(int));
    free_cpy1(bar);
    free_cpy1(foo);
    char* other = static_cast<char *>(malloc_cpy1(96));
}

void* malloc_cpy2(size_t size){

}

void free_cpy2(void* ptr){

}

void log2(){

}

void task2(){

}

void task1(){
    map<string, int*> dictionary;
    string line, name, name2;
    fstream file;
    size_t pos, size_mem;
    file.open("in.txt", std::ios_base::in);
    while(!file.eof()){
        std::getline(file, line);
        if (line.length() != 0) {
            if (line.find("dealloc") != -1){
                name = "";
                pos = line.find('(');
                pos++;
                for (;pos < line.find(')'); pos++){
                    name += line[pos];
                }

                free_cpy1(dictionary[name]);
                dictionary[name] = nullptr;
            }
            else if (line.find("alloc") != -1){
                size_mem = 0;
                name = "";
                for(int i = 0; line[i] != '='; i++){
                    name += line[i];
                }

                for (size_t i = line.find('(')+1; i < line.find(')'); i++){
                    size_mem *= 10;
                    size_mem += line[i] - '0';
                }

                dictionary[name] = static_cast<int *>(malloc_cpy1(size_mem));
            }
            else if (line.find('=') != -1){
                name = "";
                name2 = "";

                for (size_t i = 0; i < line.find('='); i++) {
                    name += line[i];
                }
                for (size_t i = line.find('=')+1; line[i]!= ';'; i++) {
                    name2 += line[i];
                }

                dictionary[name] = dictionary[name2];
            }
            else{
                cout << "i don't know what it mean" << endl;
            }


        }
    }

}

void log1(){
    int sum = 0;
    cout << "Free entries1 in heap:" << endl;
    free_memory1 *f = entries1;
    while(f->address!= nullptr){
        cout << "\t" << (f - entries1) + 1 << ") Address: " << static_cast<void*>(f->address) << "; Size: " << f->size << endl;
        sum += f->size;
        f++;
    }

    cout << "All free memory: " << sum << endl << endl;
}

void* malloc_cpy1(size_t size)
{
    size += HEADER;
    if(size > HEAP_SIZE)
        throw new LargeSizeException("Size exceeds heap size");

    free_memory1* entry_ptr = entries1;
    while(entry_ptr->address != nullptr){
        if (entry_ptr->size >= size){
            break;
        }

        entry_ptr+=1;
    }

    if (entry_ptr->address == nullptr)
        throw new NoMemoryAvailableException("Heap doesn't have enough memory");

    u8* user_ptr = entry_ptr->address + HEADER;
    u8* f = entry_ptr->address;
    *f = size;

    if (entry_ptr->size > size) {
        entry_ptr->size-=size;
        entry_ptr->address += size;
    }
    else{
        if ((entry_ptr + 1)->address == nullptr){
            entry_ptr->address = nullptr;
            entry_ptr->size = 0;
        }
        else{
            while(entry_ptr->address != nullptr){
                entry_ptr->address = (entry_ptr+1)->address;
                entry_ptr->size = (entry_ptr+1)->size;
                entry_ptr+=1;
            }
        }
    }

    log1();
    return user_ptr;
}

void free_cpy1(void* ptr)
{
    u8* free_ptr;
    free_ptr = (u8*)ptr - HEADER;
    free_memory1* entry_ptr = entries1;
    if (entry_ptr->address == nullptr){
        entry_ptr->address = free_ptr;
        entry_ptr->size = *free_ptr;
        log1();
        return;
    }
    while(entry_ptr){
        if (entry_ptr->address > free_ptr)
            break;

        entry_ptr += 1;
    }

    if (entry_ptr == entries1){
        if (free_ptr + *free_ptr == entry_ptr->address)
        {
            entry_ptr->address = free_ptr;
            entry_ptr->size += *free_ptr;
        }
        else{
            free_memory1 entry_ptr_cpy;
            u8* f;
            entry_ptr_cpy.address = entry_ptr->address;
            entry_ptr_cpy.size = entry_ptr->size;
            entry_ptr->address = free_ptr;
            entry_ptr->size = *free_ptr;
            entry_ptr+=1;
            while(entry_ptr->address != nullptr){
                f = entry_ptr_cpy.address;
                entry_ptr_cpy.address = entry_ptr->address;
                entry_ptr->address = f;

                *f = entry_ptr_cpy.size;
                entry_ptr_cpy.size = entry_ptr->size;
                entry_ptr->size = *f;

                entry_ptr+=1;
            }

            entry_ptr->address = entry_ptr_cpy.address;
            entry_ptr->size = entry_ptr_cpy.size;
        }

    }
    else{
        entry_ptr-=1;
        if (entry_ptr->address + entry_ptr->size == free_ptr)
        {
            entry_ptr->size += *free_ptr;
            if (entry_ptr->address + entry_ptr->size == (entry_ptr+1)->address)
            {
                entry_ptr->size += (entry_ptr+1)->size;
                entry_ptr+=1;
                while(entry_ptr->address != nullptr){
                    entry_ptr->address = (entry_ptr+1)->address;
                    entry_ptr->size = (entry_ptr+1)->size;
                }
            }
        }
        else{
            entry_ptr+=1;
            free_memory1 entry_ptr_cpy;
            u8* f;
            entry_ptr_cpy.address = entry_ptr->address;
            entry_ptr_cpy.size = entry_ptr->size;
            entry_ptr->address = free_ptr;
            entry_ptr->size = *free_ptr;
            entry_ptr+=1;
            while(entry_ptr->address != nullptr){
                f = entry_ptr_cpy.address;
                entry_ptr_cpy.address = entry_ptr->address;
                entry_ptr->address = f;

                *f = entry_ptr_cpy.size;
                entry_ptr_cpy.size = entry_ptr->size;
                entry_ptr->size = *f;

                entry_ptr+=1;
            }

            entry_ptr->address = entry_ptr_cpy.address;
            entry_ptr->size = entry_ptr_cpy.size;


        }
    }

    log1();
}
