# ♟️ fr0sty Chess

A modern, lightweight chess application built with **C++** and **SFML**, focused on clean architecture, modular design, and real-time configurability.

---

## ✨ Features

* ♟️ Fully functional chess board
* 🎨 Multiple board themes (config-based)
* 🔄 **Hot reload config system** (no restart required)
* 🖱️ Drag & drop piece movement
* 🎯 Smooth piece animations
* 📍 Board coordinates
* 🧩 Clean and modular code structure

---

## 🧠 Architecture

This project is not just a chess game — it's designed with **engine-like structure** in mind:

* `Board` → Rendering & board state
* `Game` → Main loop & orchestration
* `Assets` → Texture & font management (singleton)
* `Config` → Runtime configuration system (hot-reloadable)

---

## ⚙️ Configuration System

The game uses a custom `.ini` config system:

```ini
[board]
theme = teal
pieces = modern
```

### 🔥 Hot Reload

Press **F5** to reload the config instantly while the game is running.

---

## 🎨 Themes

Currently supported themes:

* `green`
* `red`
* `blue`
* `orange`
* `black`
* `dark`
* `purple`
* `teal`
* `gray`
* `chesscom`
* `lichess`

Currently supported pieces:

* `modern`
* `cute`
* `cute-rise`

Easily changeable via config system.

---

## 🕹️ Controls

* 🖱️ **Click & Drag** → Move pieces
* ⌨️ **F5** → Reload config
* ❌ Close window → Exit game

---

## 📁 Project Structure

```
/assets        → textures & fonts
/src           → source files
config.ini     → runtime config
```

---

## 🛠️ Build

### Requirements

* C++17+
* SFML 3.x.x

### Example compiling (Linux)

```bash
mkdir build
cd build
cmake ..
cmake --build . / make
```

---

## 🚀 Future Plans

* ✅ Legal move highlighting
* 🎯 Move validation system
* ♟️ AI opponent
* 🎛️ In-game settings UI
* 💾 Save/Load system
* 🎨 Theme packs / mod support

---

## 💡 Why this project?

This project is built as a **learning-focused engine-style application**, exploring:

* Input systems
* Real-time config systems
* Clean C++ architecture

---

## 🧑‍💻 Author

Developed by **fr0sty**

---

## ⭐ Notes

This project is actively evolving — expect frequent improvements and new systems.

---
