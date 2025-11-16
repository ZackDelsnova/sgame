# sgame (WIP)

A small experimental survival / bullet-hell prototype built with **C++**, **CMake**, and **raylib**.  
This project focuses on gameplay-first development, clean code structure, and learning modern C++ game architecture.

## Features

- Basic window + rendering loop  
- CMake-based build system  
- Cross-platform (Windows/Linux)  
- Simple entity structure (coming soon)  
- Basic player/god-controller (coming soon)  
- Bullet-hell simulation prototype (planned)

## 🛠 Requirements

### **1. Visual Studio 2022**  
(Desktop Development with C++ workload)

Download:  
https://visualstudio.microsoft.com/

---

### **2. CMake (Latest version)**  
Download:  
https://cmake.org/download/

Make sure `cmake` works in your terminal:
```sh
cmake --version
```

---

### **3. raylib (Prebuilt for Windows)**  
Download prebuilt binaries (.zip):  
https://github.com/raysan5/raylib/releases

You only need these:

- `raylib/include/`
- `raylib/lib/`

Place them inside:

- `external/raylib/include/`
- `external/raylib/lib/`

*(These files are intentionally not pushed to the repo.)*

---

## 🚀 Build(for Windows with Visual Studio))

### **1. Create a build folder**

```sh
mkdir build
cd build
```

### **2. Generate Visual Studio project files**
```sh
cmake ..
```

### **3. Build**
```sh
cmake --build . --config Debug
```

### **4. Run**

The executable will appear in:

`build/Debug/`

---

## 🧾 Notes

- This project is intentionally minimal.
- The goal is to focus on gameplay logic and learning.
- raylib binaries are not included to keep the repository clean.

