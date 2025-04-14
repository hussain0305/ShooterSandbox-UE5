# 🛠️ Shooter Sandbox – Unreal Engine 5 (UE5)

A third-person shooter sandbox with a unique **build-and-deploy mechanic**. Players can construct gameplay objects like turrets, towers, and cannons directly on surfaces — including walls and ceilings — then interact with or occupy them during combat. This solo project focuses on modular systems, AI behavior, and flexible mechanics for rapid prototyping and gameplay iteration.

> ⚠️ **Work in Progress:** This game is currently in development. New features, visuals, and polish are still being added.  
>
> 🎨 This is a solo project — all design, programming, and visual effects were done without an artist. The game's appearance is driven entirely by custom shaders, code-based visuals, and gameplay-driven ideas.

---

## 🧠 Core Mechanic – Build & Deploy

At the heart of the game is a **surface-aware construction system** that lets players dynamically place functional game objects:

- 🧱 **Surface-Based Placement**: Build on floors, walls, or ceilings with alignment logic for orientation and collision.
- 🏗️ **Deployables**: Create structures like:
  - **Turrets** – Auto-fire or player-operated
  - **Cannon Guns** – Large, mountable weapons
  - **Towers** – For elevation or vantage points
- 🧍‍♂️ **Player Interaction**: Players can **occupy cannons** or other objects to gain new shooting modes or tactical advantages.
- 🎯 **Modular Blueprint System**: Each buildable item is a standalone blueprint class, enabling rapid experimentation with different types.

---

## 🎮 Additional Gameplay Features

- 🕹️ **Third-Person Controller**
  - Smooth movement system with sprinting, crouching, and jumping.
  
- 🔫 **Weapon System**
  - Modular and extendable; supports custom weapon logic per deployable.
  
- 🤖 **Enemy AI**
  - Patrol, detect, and attack logic with basic decision-making.
  
- ⚔️ **Combat Systems**
  - Shooting, hit detection, and damage processing across both player and deployed units.

---

## 🔧 Tech Stack

- **Engine**: Unreal Engine 5
- **Language**: C++
- **Tools**: Blueprint + C++ hybrid, Visual Studio, Unreal Editor

---
