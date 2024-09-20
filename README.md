# Bot Platform
#### A project to simplify bot scripting for any platform

The **Bot Platform** project aims to streamline the creation of bots using the Pawn scripting language. The core of the platform supports plugins, allowing developers to create custom plugins tailored to their needs.

### Official Plugins
- Coming soon.

---

### Clone the Repository
```bash
git clone https://github.com/borz7zy/bot_platform.git
cd bot_platform
```

### Pre-Build Steps
```bash
git submodule init
git submodule update --init --recursive
mkdir build && cd build
cmake ..
```

### Build Instructions
**Windows:**
```powershell
cmake --build .
```

**Linux/MacOS:**
```bash
make
```

---

### Third-Party Libraries Used in the Project
- [compuphase/pawn](https://github.com/compuphase/pawn)
