# Chapter 1: Introduction to C++ (Interview Revision)

## Core Concepts

### What is C++?
- **Compiled language**: Source code → Machine code
- **Statically typed**: Type checking at compile time
- **Multi-paradigm**: Procedural, OOP, Generic, Functional
- **Low-level manipulation**: Direct memory access
- **Performance-critical**: Used in systems, games, HPC

### Why use C++?
1. **Performance**: Fast execution, minimal overhead
2. **Memory control**: Direct memory management
3. **System programming**: OS, drivers, embedded systems
4. **Game development**: Unreal Engine, game engines
5. **High-frequency trading**: Low latency applications

### C vs C++

| Feature | C | C++ |
|---------|---|-----|
| Paradigm | Procedural | Multi-paradigm |
| OOP | ❌ No | ✅ Yes (classes, inheritance) |
| Templates | ❌ No | ✅ Yes (generic programming) |
| STL | ❌ No | ✅ Yes (containers, algorithms) |
| Exception handling | ❌ No | ✅ Yes (try-catch) |
| RAII | ❌ No | ✅ Yes (resource management) |

## Key Interview Points

### Must-Know Facts
- C++ is **NOT** pure OOP (supports C-style programming)
- **Backward compatible** with C code
- **Zero-cost abstractions**: Features don't impact performance
- **RAII principle**: Resource acquisition is initialization

### Common Misconceptions
- ❌ "C++ is just C with classes" - Wrong: Templates, STL, exceptions, etc.
- ❌ "C++ is always faster than C" - Wrong: Depends on usage
- ❌ "C++ is outdated" - Wrong: Still actively developed (C++23, C++26)

## Interview Questions

### Q1: What are the main differences between C and C++?
**Answer:**
- C is procedural, C++ is multi-paradigm (OOP, generic, functional)
- C++ has classes, inheritance, polymorphism
- C++ has templates and STL
- C++ has exception handling
- C++ supports RAII and smart pointers
- C++ has function overloading and operator overloading

### Q2: Why would you choose C++ over C for a project?
**Answer:**
- **OOP features**: Better code organization for large projects
- **STL**: Pre-built data structures and algorithms
- **Exception handling**: Better error management
- **Templates**: Generic programming without code duplication
- **RAII**: Automatic resource management
- **Smart pointers**: Safer memory management

### Q3: What does zero-cost abstraction mean in C++?
**Answer:**
- Features like templates, classes, RAII have **no runtime overhead**
- All work done at **compile time**
- Performance equivalent to hand-written C code
- Example: Template instantiation generates optimized code

### Q4: Is C++ a pure object-oriented language?
**Answer:**
- **No**, C++ is **multi-paradigm**
- Can write C-style procedural code
- Supports multiple programming paradigms
- Pure OOP would force everything to be objects (like Java)

## Code Examples

### Basic C++ vs C Comparison

```cpp
// C-style (valid in both C and C++)
#include <stdio.h>

struct Point {
    int x, y;
};

void print_point(struct Point p) {
    printf("Point: (%d, %d)\n", p.x, p.y);
}
```

```cpp
// C++ style
#include <iostream>

class Point {
private:
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) {}

    void print() const {
        std::cout << "Point: (" << x << ", " << y << ")" << std::endl;
    }
};
```

### RAII Example

```cpp
#include <fstream>

class FileHandler {
    std::ifstream file;
public:
    FileHandler(const std::string& filename) : file(filename) {
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }
    }

    // Destructor automatically closes file
    ~FileHandler() {
        if (file.is_open()) {
            file.close();
        }
    }

    // No need for manual close() calls
};

int main() {
    FileHandler fh("data.txt");  // File opens here
    // Use the file...
    // File automatically closes when fh goes out of scope
}
```

## Common Mistakes & Solutions

### Mistake 1: Using C features in C++ unnecessarily
```cpp
// ❌ Bad: Using malloc/free in C++
int* arr = (int*)malloc(10 * sizeof(int));
free(arr);

// ✅ Good: Using new/delete in C++
int* arr = new int[10];
delete[] arr;

// ✅ Better: Using vectors (STL)
std::vector<int> arr(10);
```

### Mistake 2: Not using RAII
```cpp
// ❌ Bad: Manual resource management
void process_file() {
    FILE* f = fopen("data.txt", "r");
    if (f) {
        // Process file...
        fclose(f);  // Can be forgotten
    }
}

// ✅ Good: RAII approach
void process_file() {
    std::ifstream f("data.txt");
    // Process file...
    // File automatically closed
}
```

## Memory Layout (Interview Important)

```cpp
class Example {
    int x;          // 4 bytes
    double y;       // 8 bytes
    char z;         // 1 byte
    // Padding: 3 bytes (alignment)
};                 // Total: 16 bytes (due to padding)

// sizeof(Example) = 16, not 13!
```

## Quick Reference

### C++ Standards Timeline
- **C++98**: First standard
- **C++03**: Bug fixes
- **C++11**: Major update (auto, lambda, smart pointers)
- **C++14**: Improvements
- **C++17**: More features
- **C++20**: Concepts, ranges, coroutines
- **C++23**: Latest standard

### Key C++ Features Not in C
- Classes and inheritance
- Templates and generic programming
- STL containers and algorithms
- Exception handling
- Function overloading
- Operator overloading
- Smart pointers
- References
- Namespaces

## Final Interview Tips

1. **Always mention RAII** - it's a core C++ concept
2. **Know the difference** between C and C++ clearly
3. **Understand zero-cost abstractions**
4. **Mention STL** as a key advantage
5. **Remember C++ is multi-paradigm**, not pure OOP
6. **Know the standards** (C++11, C++14, C++17, C++20)

---

**Remember**: C++ gives you control AND safety when used properly. It's about choosing the right tool for the job!