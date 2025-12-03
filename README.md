# Minesweeper - SFML

A classic Minesweeper game implementation using C++ and SFML (Simple and Fast Multimedia Library).

## Team Members

1. Brennan Hart - Lab Section 2 (Dylan P.)
2. Micha Mettler - Lab Section 2 (Dylan P.)

## Overview

This is a fully functional Minesweeper game with a graphical user interface. Navigate the grid, reveal tiles, flag potential mines, and try to clear the board without hitting a mine!

## How to Play

### Game Rules

1. **Objective**: Reveal all tiles that don't contain mines
2. **Numbers**: Each revealed tile shows how many mines are in the 8 surrounding tiles
3. **Flagging**: Mark tiles you think contain mines with flags
4. **Winning**: Reveal all safe tiles or flag all mines correctly
5. **Losing**: Revealing a mine ends the game

### Controls

| Key | Action |
|-----|--------|
| **Arrow Keys** (↑ ↓ ← →) | Move the yellow cursor around the grid |
| **SPACE** | Reveal the tile at the cursor position |
| **F** | Place or remove a flag on the current tile |
| **R** | Restart the game (works anytime) |

### Visual Guide

- **Gray tiles** - Unrevealed tiles
- **Light gray tiles** - Revealed safe tiles
- **Numbers (1-8)** - Count of adjacent mines (color-coded)
- **Yellow flag** - Flagged tile (you think it's a mine)
- **Red tile with mine** - Revealed mine (game over!)
- **Yellow highlight** - Current cursor position
