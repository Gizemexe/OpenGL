#include <GL/freeglut.h>
#include <vector>
#include <string> 
#include <iostream> // Skor yazdýrmak için
#include <cstdlib>

// Pencere boyutlarý
int windowWidth = 800;
int windowHeight = 600;

// Pencere baþlýðý
const char* windowTitle = "Space Shooter";
    
// Oyuncu gemisi koordinatlarý 
float playerX = 400.0f;
float playerY = 50.0f;
float playerSpeed = 20.0f; // Oyuncu gemisinin hareket hýzý
int playerHealth = 100; // Oyuncu saðlýk puaný

// Oyuncu mermi listesi
std::vector<std::pair<float, float>> bullets;
// Mermi hýzý
float bulletSpeed = 8.0f;

// Düþman listesi
std::vector<std::pair<float, float>> enemies;
// Düþman hýzý
float enemySpeed = 3.0f;

// Skor
int score = 0;
int lastScore = 0; // En son yapýlan skor

// Çarpýþan düþmanýn indeksi
int collidedEnemyIndex = -1;

// Oyun durumu
bool gameOver = false;

// Pencere oluþturma fonksiyonu
void createWindow() {
    // Pencereyi oluþtur
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(windowTitle);
}

// Pencere yeniden boyutlandýrma fonksiyonu
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

// Rastgele renk oluþturma fonksiyonu
void randomColor() {
    float r = static_cast<float>(rand()) / RAND_MAX; // Rastgele bir kýrmýzý bileþen
    float g = static_cast<float>(rand()) / RAND_MAX; // Rastgele bir yeþil bileþen
    float b = static_cast<float>(rand()) / RAND_MAX; // Rastgele bir mavi bileþen
    glColor3f(r, g, b); // Rastgele renk ayarla
}

// Ana render fonksiyonu
void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Oyuncu gemisini çiz
    glColor3f(1.0f, 1.0f, 1.0f); // Beyaz renk
    glBegin(GL_QUADS);
    glVertex2f(playerX - 25, playerY); // Sol alt
    glVertex2f(playerX + 25, playerY); // Sað alt
    glVertex2f(playerX + 25, playerY + 50); // Sað üst
    glVertex2f(playerX - 25, playerY + 50); // Sol üst
    glEnd();

    // Mermileri çiz
    glColor3f(1.0f, 0.0f, 0.0f); // Kýrmýzý renk
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
        {1.0f, 0.0f, 0.0f},  // Kýrmýzý
        {0.0f, 1.0f, 0.0f},  // Yeþil
        {0.0f, 0.0f, 1.0f},  // Mavi
        {1.0f, 1.0f, 0.0f},  // Sarý
        {1.0f, 0.0f, 1.0f},  // Pembe
        {0.0f, 1.0f, 1.0f}   // Cyan
    };

    // Düþmanlarý çiz
    for (size_t i = 0; i < enemies.size(); ++i) {
        glBegin(GL_POLYGON); // POLYGON modunu kullan
        const auto& enemy = enemies[i];
        const auto& color = colorPalette[i % colorPalette.size()]; // Paletten bir renk seç
        glColor3f(color[0], color[1], color[2]); // Seçilen rengi ayarla
        glVertex2f(enemy.first, enemy.second - 20); // Alt orta
        glVertex2f(enemy.first + 20, enemy.second + 10); // Sað alt
        glVertex2f(enemy.first + 10, enemy.second + 30); // Sað üst
        glVertex2f(enemy.first - 10, enemy.second + 30); // Sol üst
        glVertex2f(enemy.first - 20, enemy.second + 10); // Sol alt
        glEnd();
    }



    // Saðlýk çubuðunu çiz
    glColor3f(1.0f, 0.0f, 0.0f); // Kýrmýzý renk
    glBegin(GL_QUADS);
    glVertex2f(10, windowHeight - 10); // Sol üst
    glVertex2f(10 + playerHealth * 2, windowHeight - 10); // Sað üst
    glVertex2f(10 + playerHealth * 2, windowHeight - 30); // Sað alt
    glVertex2f(10, windowHeight - 30); // Sol alt
    glEnd();


    // Skoru ekrana yazdýr
    glColor3f(1.0f, 1.0f, 1.0f); // Beyaz renk
    glRasterPos2f(windowWidth - 100, windowHeight - 20); // Sað üst köþe
    std::string scoreText = "Score: " + std::to_string(score);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)scoreText.c_str());

    // Oyun bitti mi kontrol et
    
    // Game Over yazýsýný ekrana yazdýr
    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f); // Kýrmýzý renk
        glRasterPos2f(windowWidth / 2 - 50, windowHeight / 2 + 20); // Ortalanmýþ pozisyon
        std::string gameOverText = "Game Over";
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)gameOverText.c_str());

        // Toplam skoru ekrana yazdýr
        glRasterPos2f(windowWidth / 2 - 80, windowHeight / 2 - 20); // Ortalanmýþ pozisyonun altýnda
        std::string totalScoreText = "Toplam Skor: " + std::to_string(lastScore);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)totalScoreText.c_str());
    }

    glutSwapBuffers();
}



// Ana güncelleme fonksiyonu
void update(int value) {
    // Mermilerin hareketi
    for (auto& bullet : bullets) {
        bullet.second += bulletSpeed;
    }


    // Düþmanlarýn hareketi
    for (auto& enemy : enemies) {
        enemy.second -= enemySpeed;
    }

    // Yeni düþmanlar oluþturma
    if (rand() % 100 < 5) { // Belirli bir olasýlýkla
        float x = rand() % (windowWidth - 60) + 30; // Rastgele x koordinatý, düþmanýn pencerenin içinde kalmasýný saðlar
        float y = windowHeight; // Üst sýnýrdan baþla
        enemies.push_back(std::make_pair(x, y));
    }

    // Mermi-düþman çarpýþmasýný kontrol et
    for (auto bullet = bullets.begin(); bullet != bullets.end();) {
        bool hit = false;
        for (auto enemy = enemies.begin(); enemy != enemies.end();) {
            if (bullet->first >= enemy->first - 15 && bullet->first <= enemy->first + 15 &&
                bullet->second >= enemy->second - 10 && bullet->second <= enemy->second + 10) {
                // Mermi düþmana çarptý
                bullet = bullets.erase(bullet); // Mermiyi kaldýr
                enemy = enemies.erase(enemy); // Düþmaný kaldýr
                hit = true;
                score += 5; // Her baþarýlý vuruþta skoru 5 artýr
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

    // Oyuncu-düþman çarpýþmasýný kontrol et
    int collidedEnemyIndex = -1; // Çarpýþan düþmanýn indeksini sakla
    for (size_t i = 0; i < enemies.size(); ++i) {
        float enemyX = enemies[i].first;
        float enemyY = enemies[i].second;

        // Oyuncunun koordinatlarýna göre çarpýþmayý kontrol et
        if (playerX + 25 >= enemyX - 15 && playerX - 25 <= enemyX + 15 &&
            playerY + 50 >= enemyY - 10 && playerY <= enemyY + 10) {
            collidedEnemyIndex = i; // Çarpýþan düþmanýn indeksini kaydet
            break;
        }
    }

    // Çarpýþma olduysa
    if (collidedEnemyIndex != -1) {
        playerHealth -= 10; // Oyuncu canýný azalt
        enemies.erase(enemies.begin() + collidedEnemyIndex); // Çarpýþan düþmaný kaldýr
    }

    // Oyun bitti mi kontrol et
    if (playerHealth <= 0) {
        // Oyunu yeniden baþlat
        playerHealth = 100;
        lastScore = score; // Son skoru kaydet
        score = 0;
        bullets.clear();
        enemies.clear();
        // Oyuncuyu baþlangýç konumuna yerleþtir
        playerX = 400.0f;
        playerY = 50.0f;
        gameOver = true;
    }

    // Mermi sýnýrlarýný kontrol et (ekran dýþýna çýkan mermileri kaldýr)
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const std::pair<float, float>& bullet) {
        return bullet.second > windowHeight;
        }), bullets.end());

    // Düþman sýnýrlarýný kontrol et (ekran dýþýna çýkan düþmanlarý kaldýr)
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::pair<float, float>& enemy) {
        return enemy.second < -20; // Eðer düþman ekranýn altýna kadar inerse onu listeden çýkar
        }), enemies.end());

    // Oyun bitti mi kontrol et
    if (gameOver) {
        glutPostRedisplay(); // Game Over yazýsýný göstermek için yeniden çiz
        return; // Oyun durdu, güncelleme iþlemi yapma
    }
    // Ana döngüyü güncelle
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Fare týklama iþlemini iþleme fonksiyonu
void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Yeni bir mermi ekle
        bullets.push_back(std::make_pair(playerX, playerY + 50));
    }
}

// Klavye giriþini iþleme fonksiyonu
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
    case 'A':
        if (playerX - playerSpeed > 0) // Sol sýnýra ulaþmadýysa
            playerX -= playerSpeed; // Sol yönde hareket
        break;
    case 'd':
    case 'D':
        if (playerX + playerSpeed < windowWidth) // Sað sýnýra ulaþmadýysa
            playerX += playerSpeed; // Sað yönde hareket
        break;
    case 'w':
    case 'W':
        if (playerY + playerSpeed < windowHeight) // Üst sýnýra ulaþmadýysa
            playerY += playerSpeed; // Yukarý yönde hareket
        break;
    case 's':
    case 'S':
        if (playerY - playerSpeed > 0) // Alt sýnýra ulaþmadýysa
            playerY -= playerSpeed; // Aþaðý yönde hareket
        break;
   
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    createWindow();

    enemies.push_back(std::make_pair(100.0f, windowHeight - 50.0f)); // Örnek bir düþman ekleyelim

    // Callback fonksiyonlarýný tanýmla
    glutDisplayFunc(render);
    glutReshapeFunc(reshapeWindow);
    glutKeyboardFunc(handleKeypress); // Klavye giriþini iþleme fonksiyonu
    glutMouseFunc(handleMouseClick); // Fare týklama iþlemini iþleme fonksiyonu

    // Oyun güncelleme fonksiyonunu baþlat
    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}