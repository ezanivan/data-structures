# data structures
## A C library implementing data structures ranging from common to more advanced ones

### Current structures:   
- Mutable Dynamic List

### Features  
- Written in standard C
- Modular project structure
- Static library output (```libds.a```)
- Dependency-aware Makefile
- Optional debug builds
- Install/uninstall targets
---
### Build 
Build the static library:  
    make   

### Output:
    build/libds.a

### Debug:  
    make DEBUG=1

### Run example:
    make-run-example

### Instalation:
install the library and headers system-wide:
    
    sudo make install

### Default install location:
    /usr/local/lib/libds.a
    /usr/local/include/*.h

### Custom prefix: 
    make install PREFIX=/your/path

### Cleaning
    make clean

---
### Goals
#### This project exists primarily for:
- practicing low-level systems programming
- learning data structure implementation in C
- experimenting with API and library design
- building reusable C components

### Future Plans
- Stack
- Queue
- Linked list
- Hash map
- Binary trees
- B-trees
- Graphs
- Unit tests
- Shared library support

---
### License
MIT License