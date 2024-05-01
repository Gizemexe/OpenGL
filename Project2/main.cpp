#include <GL/freeglut.h>
#include <vector>
#include <string> 
#include <iostream> // Skor yazd�rmak i�in
#include <cstdlib>

// Pencere boyutlar�
int windowWidth = 800;
int windowHeight = 600;

// Pencere ba�l���
const char* windowTitle = "Space Shooter";
    
// Oyuncu gemisi koordinatlar� 
float playerX = 400.0f;
float playerY = 50.0f;
float playerSpeed = 20.0f; // Oyuncu gemisinin hareket h�z�
int playerHealth = 100; // Oyuncu sa�l�k puan�

// Oyuncu mermi listesi
std::vector<std::pair<float, float>> bullets;
// Mermi h�z�
float bulletSpeed = 8.0f;

// D��man listesi
std::vector<std::pair<float, float>> enemies;
// D��man h�z�
float enemySpeed = 3.0f;

// Skor
int score = 0;
int lastScore = 0; // En son yap�lan skor

// �arp��an d��man�n indeksi
int collidedEnemyIndex = -1;

// Oyun durumu
bool gameOver = false;

// Pencere olu�turma fonksiyonu
void createWindow() {
    // Pencereyi olu�tur
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(windowTitle);
}

// Pencere yeniden boyutland�rma fonksiyonu
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

// Rastgele renk olu�turma fonksiyonu
void randomColor() {
    float r = static_cast<float>(rand()) / RAND_MAX; // Rastgele bir k�rm�z� bile�en
    float g = static_cast<float>(rand()) / RAND_MAX; // Rastgele bir ye�il bile�en
    float b = static_cast<float>(rand()) / RAND_MAX; // Rastgele bir mavi bile�en
    glColor3f(r, g, b); // Rastgele renk ayarla
}

// Ana render fonksiyonu
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Oyuncu gemisini �iz
    glColor3f(1.0f, 1.0f, 1.0f); // Beyaz renk
    glBegin(GL_QUADS);
    glVertex2f(playerX - 25, playerY); // Sol alt
    glVertex2f(playerX + 25, playerY); // Sa� alt
    glVertex2f(playerX + 25, playerY + 50); // Sa� �st
    glVertex2f(playerX - 25, playerY + 50); // Sol �st
    glEnd();

    // Mermileri �iz
    glColor3f(1.0f, 0.0f, 0.0f); // K�rm�z� renk
    glBegin(GL_QUAD_STRIP);
    for (const auto& bullet : bullets) {
        glVertex2f(bullet.first - 2, bullet.second - 5);
        glVertex2f(bullet.first + 2, bullet.second - 5);
        glVertex2f(bullet.first + 2, bullet.second + 5);
        glVertex2f(bullet.first - 2, bullet.second + 5);
    }
    glEnd();

    // Renk paleti
    std::vector<std::vector<float>> colorPalette = {
        {1.0f, 0.0f, 0.0f},  // K�rm�z�
        {0.0f, 1.0f, 0.0f},  // Ye�il
        {0.0f, 0.0f, 1.0f},  // Mavi
        {1.0f, 1.0f, 0.0f},  // Sar�
        {1.0f, 0.0f, 1.0f},  // Pembe
        {0.0f, 1.0f, 1.0f}   // Cyan
    };

    // D��manlar� �iz
    for (size_t i = 0; i < enemies.size(); ++i) {
        glBegin(GL_POLYGON); // POLYGON modunu kullan
        const auto& enemy = enemies[i];
        const auto& color = colorPalette[i % colorPalette.size()]; // Paletten bir renk se�
        glColor3f(color[0], color[1], color[2]); // Se�ilen rengi ayarla
        glVertex2f(enemy.first, enemy.second - 20); // Alt orta
        glVertex2f(enemy.first + 20, enemy.second + 10); // Sa� alt
        glVertex2f(enemy.first + 10, enemy.second + 30); // Sa� �st
        glVertex2f(enemy.first - 10, enemy.second + 30); // Sol �st
        glVertex2f(enemy.first - 20, enemy.second + 10); // Sol alt
        glEnd();
    }



    // Sa�l�k �ubu�unu �iz
    glColor3f(1.0f, 0.0f, 0.0f); // K�rm�z� renk
    glBegin(GL_QUADS);
    glVertex2f(10, windowHeight - 10); // Sol �st
    glVertex2f(10 + playerHealth * 2, windowHeight - 10); // Sa� �st
    glVertex2f(10 + playerHealth * 2, windowHeight - 30); // Sa� alt
    glVertex2f(10, windowHeight - 30); // Sol alt
    glEnd();


    // Skoru ekrana yazd�r
    glColor3f(1.0f, 1.0f, 1.0f); // Beyaz renk
    glRasterPos2f(windowWidth - 100, windowHeight - 20); // Sa� �st k��e
    std::string scoreText = "Score: " + std::to_string(score);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)scoreText.c_str());

    // Oyun bitti mi kontrol et
    
    // Game Over yaz�s�n� ekrana yazd�r
    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f); // K�rm�z� renk
        glRasterPos2f(windowWidth / 2 - 50, windowHeight / 2 + 20); // Ortalanm�� pozisyon
        std::string gameOverText = "Game Over";
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)gameOverText.c_str());

        // Toplam skoru ekrana yazd�r
        glRasterPos2f(windowWidth / 2 - 80, windowHeight / 2 - 20); // Ortalanm�� pozisyonun alt�nda
        std::string totalScoreText = "Toplam Skor: " + std::to_string(lastScore);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)totalScoreText.c_str());
    }

    glutSwapBuffers();
}



// Ana g�ncelleme fonksiyonu
void update(int value) {
    // Mermilerin hareketi
    for (auto& bullet : bullets) {
        bullet.second += bulletSpeed;
    }


    // D��manlar�n hareketi
    for (auto& enemy : enemies) {
        enemy.second -= enemySpeed;
    }

    // Yeni d��manlar olu�turma
    if (rand() % 100 < 5) { // Belirli bir olas�l�kla
        float x = rand() % (windowWidth - 60) + 30; // Rastgele x koordinat�, d��man�n pencerenin i�inde kalmas�n� sa�lar
        float y = windowHeight; // �st s�n�rdan ba�la
        enemies.push_back(std::make_pair(x, y));
    }

    // Mermi-d��man �arp��mas�n� kontrol et
    for (auto bullet = bullets.begin(); bullet != bullets.end();) {
        bool hit = false;
        for (auto enemy = enemies.begin(); enemy != enemies.end();) {
            if (bullet->first >= enemy->first - 15 && bullet->first <= enemy->first + 15 &&
                bullet->second >= enemy->second - 10 && bullet->second <= enemy->second + 10) {
                // Mermi d��mana �arpt�
                bullet = bullets.erase(bullet); // Mermiyi kald�r
                enemy = enemies.erase(enemy); // D��man� kald�r
                hit = true;
                score += 5; // Her ba�ar�l� vuru�ta skoru 5 art�r
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

    // Oyuncu-d��man �arp��mas�n� kontrol et
    int collidedEnemyIndex = -1; // �arp��an d��man�n indeksini sakla
    for (size_t i = 0; i < enemies.size(); ++i) {
        float enemyX = enemies[i].first;
        float enemyY = enemies[i].second;

        // Oyuncunun koordinatlar�na g�re �arp��may� kontrol et
        if (playerX + 25 >= enemyX - 15 && playerX - 25 <= enemyX + 15 &&
            playerY + 50 >= enemyY - 10 && playerY <= enemyY + 10) {
            collidedEnemyIndex = i; // �arp��an d��man�n indeksini kaydet
            break;
        }
    }

    // �arp��ma olduysa
    if (collidedEnemyIndex != -1) {
        playerHealth -= 10; // Oyuncu can�n� azalt
        enemies.erase(enemies.begin() + collidedEnemyIndex); // �arp��an d��man� kald�r
    }

    // Oyun bitti mi kontrol et
    if (playerHealth <= 0) {
        // Oyunu yeniden ba�lat
        playerHealth = 100;
        lastScore = score; // Son skoru kaydet
        score = 0;
        bullets.clear();
        enemies.clear();
        // Oyuncuyu ba�lang�� konumuna yerle�tir
        playerX = 400.0f;
        playerY = 50.0f;
        gameOver = true;
    }

    // Mermi s�n�rlar�n� kontrol et (ekran d���na ��kan mermileri kald�r)
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const std::pair<float, float>& bullet) {
        return bullet.second > windowHeight;
        }), bullets.end());

    // D��man s�n�rlar�n� kontrol et (ekran d���na ��kan d��manlar� kald�r)
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::pair<float, float>& enemy) {
        return enemy.second < -20; // E�er d��man ekran�n alt�na kadar inerse onu listeden ��kar
        }), enemies.end());

    // Oyun bitti mi kontrol et
    if (gameOver) {
        glutPostRedisplay(); // Game Over yaz�s�n� g�stermek i�in yeniden �iz
        return; // Oyun durdu, g�ncelleme i�lemi yapma
    }
    // Ana d�ng�y� g�ncelle
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Fare t�klama i�lemini i�leme fonksiyonu
void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Yeni bir mermi ekle
        bullets.push_back(std::make_pair(playerX, playerY + 50));
    }
}

// Klavye giri�ini i�leme fonksiyonu
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
    case 'A':
        if (playerX - playerSpeed > 0) // Sol s�n�ra ula�mad�ysa
            playerX -= playerSpeed; // Sol y�nde hareket
        break;
    case 'd':
    case 'D':
        if (playerX + playerSpeed < windowWidth) // Sa� s�n�ra ula�mad�ysa
            playerX += playerSpeed; // Sa� y�nde hareket
        break;
    case 'w':
    case 'W':
        if (playerY + playerSpeed < windowHeight) // �st s�n�ra ula�mad�ysa
            playerY += playerSpeed; // Yukar� y�nde hareket
        break;
    case 's':
    case 'S':
        if (playerY - playerSpeed > 0) // Alt s�n�ra ula�mad�ysa
            playerY -= playerSpeed; // A�a�� y�nde hareket
        break;
   
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    createWindow();

    enemies.push_back(std::make_pair(100.0f, windowHeight - 50.0f)); // �rnek bir d��man ekleyelim

    // Callback fonksiyonlar�n� tan�mla
    glutDisplayFunc(render);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(handleKeypress); // Klavye giri�ini i�leme fonksiyonu
    glutMouseFunc(handleMouseClick); // Fare t�klama i�lemini i�leme fonksiyonu

    // Oyun g�ncelleme fonksiyonunu ba�lat
    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}