# OpenGL Project
This project describes a space shooter game developed in C++ using the FreeGLUT library on Visual Studio. The game is a recreation of the game previously developed using the SFML library, with the same graphics and gameplay mechanics. The main difference between the two versions lies in the use of different libraries, which results in some minor variations in the code.
Gameplay Mechanics: 
   - The player controls a spaceship that can move freely around the screen.
   - The player can shoot unlimited bullets to destroy enemies.
   - The player must avoid collision with enemies and their bullets.
   - The game ends when the player runs out of hp.

FreeGLUT:
```
createWindow():
Initialize the display mode to double buffering, RGBA color, and depth buffering
Set the window size to (windowWidth, windowHeight)
Create a window with the title "windowTitle"
```

SFML:
```
initWindow():
Create a new render window with the following properties:
- Video mode: 800x600
- Title: "Space Shoot Game"
- Style: Close | Titlebar
Set the frame rate limit to 144
Disable vertical sync
```
Differences:
- FreeGLUT takes the window size and title as parameters, while SFML uses fixed values.
- FreeGLUT determines the window style with OpenGL-defined constants, while SFML determines the window style with its own enum type.
- SFML offers additional functions such as limiting the frame rate and disabling vertical synchronization.Oct.

FreeGLUT:
C++
```
// draw player ship
glColor3f(1.0f, 1.0f, 1.0f); // White color
glBegin(GL_QUADS);
glVertex2f(playerX - 25, playerY); // Bottom left corner
glVertex2f(playerX + 25, playerY); // Bottom right corner
glVertex2f(playerX + 25, playerY + 50); // Top right corner
glVertex2f(playerX - 25, playerY + 50); // Top left corner
glEnd(); 
```

SFML:
C++
```
void Player::initShape()
{
shape.setPointCount(4); // The square has four corners
shape.setPoint(0, sf::Vector2f(0.f, 0.f)); // Top left corner
shape.setPoint(1, sf::Vector2f(100.f, 0.f)); // Top right corner
shape.setPoint(2, sf::Vector2f(100.f, 100.f)); // Bottom right corner
shape.setPoint(3, sf::Vector2f(0.f, 100.f)); // Bottom left corner
shape.setFillColor(sf::Color::White); // White color
}
```
Differences:
- FreeGLUT draws the player ship directly using the OpenGL API, while SFML draws it in a more abstract way using the Shape class.
- In FREEGLUT, the ship size is fixed, while in SFML it can be changed.
- In FREEGLUT, the vertices are fixed, while in SFML they can be determined.

<h3>Images From Game:</h3> 

  ![image](https://github.com/Gizemexe/OpenGL/assets/108283247/90635de4-5be0-48f6-b14b-96c8bdf9aa93)
  (picture 1)

  ![image](https://github.com/Gizemexe/OpenGL/assets/108283247/39078eab-bcc6-467d-a360-212e2fc967b5)
  (picture 2)

  ![image](https://github.com/Gizemexe/OpenGL/assets/108283247/045eb01a-4f0e-492a-8a22-2eb21d32f88f)
  (picture 3)


