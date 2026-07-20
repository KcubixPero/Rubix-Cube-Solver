# 🧩 Rubik's Cube Solver & Visualizer

An end-to-end Rubik's Cube project built from scratch, featuring a C++ simulation engine, solving algorithms, and an interactive web-based 3D visualizer.

The goal of this project is not only to simulate a Rubik's Cube but also to provide an intuitive platform for visualizing cube states, generating scrambles, executing moves, and solving the cube using standard algorithms.

---

## ✨ Features

### Core Cube Engine (C++)

* 3×3 Rubik's Cube representation
* Complete implementation of basic cube moves
* Inverse and double moves
* Cube state validation
* Scramble generation
* Efficient cube state manipulation

### Solver

* CFOP Solver *(Planned)*
* Kociemba Two-Phase Solver *(Planned)*
* Solution generation from any valid cube state

### Web Application

* Interactive 3D Rubik's Cube
* Smooth move animations
* Rotate and inspect the cube
* Manual move controls
* Keyboard shortcuts
* Random scramble generation
* One-click solve
* Move history
* Undo / Redo
* Responsive design

---

## 🛠️ Tech Stack

### Backend

* C++
* STL

### Frontend

* React
* Node.js
* Tailwind CSS

### Tools

* Git
* GitHub

---

## 📂 Project Structure

```text
Rubiks-Cube-Solver/
│
├── backend/
│   ├── cube/
│   ├── solver/
│   ├── utils/
│   └── main.cpp
│
├── frontend/
│   ├── src/
│   ├── components/
│   ├── pages/
│   ├── assets/
│   └── public/
│
├── README.md
```

---

## 📌 Cube Orientation

The project uses the following fixed orientation throughout the implementation.

```text
         RED (Up)

BLUE   WHITE   GREEN   YELLOW

       ORANGE (Down)
```

| Face  | Color  |
| ----- | ------ |
| Front | White  |
| Back  | Yellow |
| Up    | Red    |
| Down  | Orange |
| Left  | Blue   |
| Right | Green  |

---

## 🚀 Roadmap

### Phase 1 — Cube Engine

* [ ] Cube representation
* [ ] Face rotations
* [ ] Cube printing
* [ ] Scramble generator

### Phase 2 — Solver

* [ ] CFOP Solver
* [ ] Kociemba Solver

### Phase 3 — Web Visualizer

* [ ] Interactive 3D Cube
* [ ] Cube animations
* [ ] Move controls
* [ ] Move history
* [ ] Timer
* [ ] Keyboard shortcuts

### Phase 4 — Polish

* [ ] Dark mode
* [ ] Performance optimizations
* [ ] Deployment

---

## 🎯 Objectives

This project is being developed to strengthen understanding of:

* Object-Oriented Programming
* Data Structures
* Algorithms
* Matrix transformations
* Computational geometry
* Software architecture
* Frontend development
* Backend integration

---

## 📸 Preview

*Screenshots and demo GIFs will be added as development progresses.*

---

## 🤝 Contributing

Contributions, suggestions, and feature requests are welcome. Feel free to fork the repository, open an issue, or submit a pull request.

---

---

## ⭐ Future Enhancements

* Multiple cube sizes (2×2, 4×4, 5×5)
* Custom cube color themes
* Save and load cube states
* Import/export scramble notation
* Speedcubing timer with statistics
* AI-assisted solving strategies
* Algorithm visualizer
* Pattern generator (Checkerboard, Superflip, etc.)
* Move notation parser
* Multiplayer solving challenges
