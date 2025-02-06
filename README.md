## Recent Changes

### 06/02/25
- ✅ Bug fixing
- ✅ Implemented Light/Dark feature

### 05/02/25
- ✅ The Arial.ttf file is now included in the project, so there's no need to download it manually.
- ✅ The .dll files are now included, eliminating the need to download them separately.
- ✅ The project structure has been reorganized: Source code is now located in `src/`, Header files are stored in `include/`
- ✅ Added the Network monitoring feature
- ✅ Various BugFixing

## To-Do List
- [X] Improve UI design to enhance readability
- [X] Display CPU & RAM usage percentage
- [X] Show network upload & download speed
- [ ] Add detailed CPU, GPU, and OS information
- [ ] Improve error handling and exception management
- [ ] Make the code more modular for better maintainability
- [ ] Refactoring, simplify complex logic
- [ ] Better UI, background, etc.
- [ ] More functionality?

## Features
- Real-time monitoring of CPU, RAM, and network usage
- Graphical interface for better data visualization
- Animated color-coded bars for CPU and RAM usage
- Displays CPU and RAM percentage values next to the bars
- Shows upload and download speed in KB/s

## Installation
### 📌 Required Dependencies:
- MinGW (GCC/G++)

### 🚀 Compilation
Run the following command in the project folder:
```sh
g++ -o system_monitor.exe src/main.cpp src/NetworkMonitor.cpp src/SystemMonitor.cpp src/UIRenderer.cpp \
-I include \
-I C:/SDL3-3.2.2/x86_64-w64-mingw32/include \
-I C:/SDL3_ttf-3.1.0/x86_64-w64-mingw32/include \
-L C:/SDL3-3.2.2/x86_64-w64-mingw32/lib \
-L C:/SDL3_ttf-3.1.0/x86_64-w64-mingw32/lib \
-lmingw32 -lSDL3 -lSDL3_ttf -liphlpapi
🏃‍♂️ Run the Application
Make sure the SDL3.dll and SDL3_ttf.dll files are in the same folder as the executable, then run:

./system_monitor.exe
🤝 Contributing
If you want to contribute, feel free to clone the repository and open a Pull Request on GitHub: GitHub Repository

🚀 All contributions are welcome!


---

### **Why This Fix Works**
- **Lists** (`-` or `*`) are properly separated into new lines.
- **Checkmarks** (`✅`) are placed in **separate list items**.
- **Markdown Headers (`##`)** are used properly.
- **Code Blocks (` ```sh `)** are correctly formatted for shell commands.

After modifying, commit and push it back:
```sh
git add README.md
git commit -m "Fixed Markdown formatting for lists and code blocks"
git push origin main
Let me know if you need further help! 🚀
