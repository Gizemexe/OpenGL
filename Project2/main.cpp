#include <GL/freeglut.h>
#include <vector>
#include <string> 
#include <iostream> 
#include <cstdlib>

// Window size  
int windowWidth = 800;
int windowHeight = 600;

// Window title
const char* windowTitle = "Space Shooter";
    
// Player's coordinates 
float playerX = 400.0f;
float playerY = 50.0f;
float playerSpeed = 20.0f; // Player's speed 
int playerHealth = 100; // Player's Health

// bullets
std::vector<std::pair<float, float>> bullets;
// Bullet's speed 
float bulletSpeed = 8.0f;

// Enemies
std::vector<std::pair<float, float>> enemies;
// Enemies speed 
float enemySpeed = 3.0f;

// Score
int score = 0;
int lastScore = 0; // Last score

// Collided Enemy Index 
int collidedEnemyIndex = -1;

// Game 
bool gameOver = false;

// Create a window 
void createWindow() {
    // Pencereyi oluþtur
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(windowTitle);
}

// Fixing Window size 
void reshapeWindow(int width, int height) {
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// create random colors
void randomColor() {
    float r = static_cast<float>(rand()) / RAND_MAX; // Random red 
    float g = static_cast<float>(rand()) / RAND_MAX; // Random green 
    float b = static_cast<float>(rand()) / RAND_MAX; // Random blue 
    glColor3f(r, g, b); // random arrange a color 
}

// Main render function
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Drawing Player's ship 
    glColor3f(1.0f, 1.0f, 1.0f); // White 
    glBegin(GL_QUADS);
    glVertex2f(playerX - 25, playerY); // bottom left 
    glVertex2f(playerX + 25, playerY); // bottom right 
    glVertex2f(playerX + 25, playerY + 50); // up right 
    glVertex2f(playerX - 25, playerY + 50); // up left 
    glEnd();

    // Drawing Bullets
    glColor3f(1.0f, 0.0f, 0.0f); // red 
    glBegin(GL_QUAD_STRIP);
    for (const auto& bullet : bullets) {
        glVertex2f(bullet.first - 2, bullet.second - 5);
        glVertex2f(bullet.first + 2, bullet.second - 5);
        glVertex2f(bullet.first + 2, bullet.second + 5);
        glVertex2f(bullet.first - 2, bullet.second + 5);
    }
    glEnd();

    // Color pallet
    std::vector<std::vector<float>> colorPalette = {
        {1.0f, 0.0f, 0.0f},  // Red
        {0.0f, 1.0f, 0.0f},  // Green
        {0.0f, 0.0f, 1.0f},  // Blue
        {1.0f, 1.0f, 0.0f},  // Yellow
        {1.0f, 0.0f, 1.0f},  // Pink
        {0.0f, 1.0f, 1.0f}   // Cyan
    };

    // Drawing enemies 
    for (size_t i = 0; i < enemies.size(); ++i) {
        glBegin(GL_POLYGON); // POLYGON mode
        const auto& enemy = enemies[i];
        const auto& color = colorPalette[i % colorPalette.size()]; // Choose a color from palette 
        glColor3f(color[0], color[1], color[2]); // setting the color 
        glVertex2f(enemy.first, enemy.second - 20); // bottom mid 
        glVertex2f(enemy.first + 20, enemy.second + 10); // bottom right 
        glVertex2f(enemy.first + 10, enemy.second + 30); // up right
        glVertex2f(enemy.first - 10, enemy.second + 30); // up left 
        glVertex2f(enemy.first - 20, enemy.second + 10); // bottom left
        glEnd();
    }



    // Health board 
    glColor3f(1.0f, 0.0f, 0.0f); // Red color 
    glBegin(GL_QUADS);
    glVertex2f(10, windowHeight - 10); // up left 
    glVertex2f(10 + playerHealth * 2, windowHeight - 10); // up right 
    glVertex2f(10 + playerHealth * 2, windowHeight - 30); // bottom right 
    glVertex2f(10, windowHeight - 30); // bottom left 
    glEnd();


    // print score to window 
    glColor3f(1.0f, 1.0f, 1.0f); // white 
    glRasterPos2f(windowWidth - 100, windowHeight - 20); // up right corner 
    std::string scoreText = "Score: " + std::to_string(score);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)scoreText.c_str());

    // Check if the game is over or not 
    
    // print "Game Over" text 
    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f); // red 
        glRasterPos2f(windowWidth / 2 - 50, windowHeight / 2 + 20); 
        std::string gameOverText = "Game Over";
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)gameOverText.c_str());

        // print Total score on window 
        glRasterPos2f(windowWidth / 2 - 80, windowHeight / 2 - 20); 
        std::string totalScoreText = "Toplam Skor: " + std::to_string(lastScore);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)totalScoreText.c_str());
    }

    glutSwapBuffers();
}  

// Main update function
void update(int value) {
    // Movement of bullets 
    for (auto& bullet : bullets) {
        bullet.second += bulletSpeed;
    }


    // Movement of enemies
    for (auto& enemy : enemies) {
        enemy.second -= enemySpeed;
    }

    // Creating new enemies
    if (rand() % 100 < 5) { 
        float x = rand() % (windowWidth - 60) + 30; // The random x coordinate ensures that the enemy stays inside the window
        float y = windowHeight; 
        enemies.push_back(std::make_pair(x, y));
    }

    // Check the bullet-enemy collision.
    for (auto bullet = bullets.begin(); bullet != bullets.end();) {
        bool hit = false;
        for (auto enemy = enemies.begin(); enemy != enemies.end();) {
            if (bullet->first >= enemy->first - 15 && bullet->first <= enemy->first + 15 &&
                bullet->second >= enemy->second - 10 && bullet->second <= enemy->second + 10) {
                // The bullet hit the enemy
                bullet = bullets.erase(bullet); // delete bullet 
                enemy = enemies.erase(enemy); // delete enemy 
                hit = true;
                score += 5; //Increase the score by 5 for each successful hit
                break;
            }
            else {
                ++enemy;
            }
        }
        if (!hit) {
            ++bullet;
        }
    }

    // Control the player-enemy collision
    int collidedEnemyIndex = -1; 
    for (size_t i = 0; i < enemies.size(); ++i) {
        float enemyX = enemies[i].first;
        float enemyY = enemies[i].second;

        // Check the collision according to the player's coordinates
        if (playerX + 25 >= enemyX - 15 && playerX - 25 <= enemyX + 15 &&
            playerY + 50 >= enemyY - 10 && playerY <= enemyY + 10) {
            collidedEnemyIndex = i; // Save the index of the colliding enemy
            break;
        }
    }

    // If there was a collision
    if (collidedEnemyIndex != -1) {
        playerHealth -= 10; // decrease the player's health 
        enemies.erase(enemies.begin() + collidedEnemyIndex); // Remove the colliding enemy
    }

    // Check if the game is over
    if (playerHealth <= 0) {
        // Restart the game
        playerHealth = 100;
        lastScore = score; //Save the last score
        score = 0;
        bullets.clear();
        enemies.clear();
        // Place the player in the starting position
        playerX = 400.0f;
        playerY = 50.0f;
        gameOver = true;
    }

    // Check the bullet limits (remove bullets that go out of the screen)
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const std::pair<float, float>& bullet) {
        return bullet.second > windowHeight;
        }), bullets.end());

    // Check enemy boundaries (remove enemies that go off-screen)
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::pair<float, float>& enemy) {
        return enemy.second < -20; // If the enemy gets down to the bottom of the screen, remove him from the list
        }), enemies.end());

    // Check if the game is over or not 
    if (gameOver) {
        glutPostRedisplay(); // Redraw to show the Game Over text
        return; // The game has stopped, do not update
    }
    // Update the main loop
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Mouse click operation processing function
void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Add a new bullet Oct.
        bullets.push_back(std::make_pair(playerX, playerY + 50));
    }
}

// Keyboard input processing function
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
    case 'A':
        if (playerX - playerSpeed > 0) // If it has not reached the left border
            playerX -= playerSpeed; // Moving in the left direction
        break;
    case 'd':
    case 'D':
        if (playerX + playerSpeed < windowWidth) // If it has not reached the right limit
            playerX += playerSpeed; // Moving in the right direction
        break;
    case 'w':
    case 'W':
        if (playerY + playerSpeed < windowHeight) // Has not reached the upper limit
            playerY += playerSpeed; // Move up
        break;
    case 's':
    case 'S':
        if (playerY - playerSpeed > 0) // If it has not reached the lower limit
            playerY -= playerSpeed; // Movement in the downward direction
        break;
   
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    createWindow();

    enemies.push_back(std::make_pair(100.0f, windowHeight - 50.0f)); // Let's add an example enemy

    // Define callback functions
    glutDisplayFunc(render);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(handleKeypress); // Keyboard input processing function
    glutMouseFunc(handleMouseClick); // Mouse click operation processing function

    // Start the game update function
    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}