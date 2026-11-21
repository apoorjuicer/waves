# Waves: 2D Stealth Maze Game in C++

A suspenseful maze game: the player's vision is limited to a small radius,
so they throw vision rocks that send out sound waves to reveal the maze,
but the waves can also draw the enemy. Find the key and reach the exit.
5-person team, 1 trimester (Spring 2025), DigiPen Institute of Technology
Singapore.

## My part (enemy AI)
- **A\* pathfinding** on the maze grid, with frame-rate independent movement
- **Enemy state machine**: patrol, return to patrol route, chase, with
  tuned transitions, speed and pursuit timer
- **Sound-driven behaviour**: enemies hit by the player's directional
  sound wave start chasing the player
- **Data-driven patrols**: patrol points loaded from each level's maze file
- Refactored the enemy from a struct into a class

## Tech
C++, Visual Studio

## Team
- Jordain Ng Ying Zhe
- Chewn Thing Kwan
- Ban Kai Wei Benjamin
- Guan Shao Jun
- Hu Zhiheng Henrey

Original repository hosted on DigiPen's Git server; this is a mirror for
portfolio purposes.
