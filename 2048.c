#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// FUNCTIE PENTRU A AFISA MENIUL
void meniu(int selectie, char optiune[][9]) {
    clear();
    int y, x, l, h;

    // Calculează dimensiunile și poziționarea ferestrei pentru meniu
    h = 8;
    l = 15;
    y = (LINES - h) / 2;  // Centrare pe axa Y
    x = (COLS - l) / 2;   // Centrare pe axa X

    WINDOW *meniu_win = newwin(h, l, y, x);
    box(meniu_win, 0, 0);
    refresh();

    // Afisează opțiunile din meniu
    for (int i = 0; i < 3; i++) {
        if (i == selectie) {
            wattron(meniu_win, A_BOLD);
            // Opțiunea selectată este evidențiată
            mvwprintw(meniu_win, i + 1, 2, "-> %s", optiune[i]);
            wattroff(meniu_win, A_BOLD);
        } else {
            mvwprintw(meniu_win, i + 1, 2, "   %s", optiune[i]);
        }
    }

    wrefresh(meniu_win);
    refresh();
}

// FUNCTIE PENTRU NAVIGARE
int navigare(char optiune[][9]) {
    flushinp();
    int selectie = 0;
    int tasta;

    while (1) {
        clear();
        meniu(selectie, optiune);
        flushinp();
        tasta = getch();

        switch (tasta) {
            case KEY_UP:

                selectie = (selectie - 1 + 3) % 3;
                break;

            case KEY_DOWN:

                selectie = (selectie + 1) % 3;
                break;

            case 10:
                return selectie;

            default:

                break;
        }
    }
}
// FUNCTIE CARE NUMARA CATE ELEMENTE EGALE CU 0 SUNT
int nr_celule_goale(int joc[][4]) {
    int i, j;
    int count = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (joc[i][j] == 0) {
                count++;
            }
        }
    }
    return count;
}
// FUNCTIE DE GENERARE A UNUI NUMARU ALEATORIU(2 SAU 4)
void nraleatoriu(int joc[][4]) {
    int gol[16][2];  // matrice care retine indicii de coloana si linie a
                     // casutelor goale
    int nr = 0;
    int i, j;
    int random, value;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (joc[i][j] == 0) {
                gol[nr][0] = i;
                gol[nr][1] = j;
                nr++;
            }
        }
    }

    if (nr > 0) {
        random = rand() % nr;
        value = (rand() % 2 + 1) * 2;  // Generează 2 sau 4
        joc[gol[random][0]][gol[random][1]] = value;
    }
}
// FUNCTIE DE START AL JOCULUI- SCOR =0 SI GENERARE 2 ELEMENTE ALEATORII
void start(int joc[][4], int *scor) {
    int i, j;
    // linii si coloane pentru matricea de joc
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            joc[i][j] = 0;
        }
    }
    *scor = 0;
    nraleatoriu(joc);
    nraleatoriu(joc);
}
// FUNCTIE AFISARE TABLA
void afisare_tabla(WINDOW *window, int joc[][4]) {
    wclear(window);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    int i, j;
    int l = 8;         // latimea unui element din matrice
    int h = 3;         // inaltimea unui element din matrice
    int margineX = 2;  // Spațiul orizontal între chenarele celulelor
    int margineY = 1;  // Spațiul vertical între chenarele celulelor

    // Dimensiunile matricei
    int L = 4 * (l + margineX) - margineX;
    int H = 4 * (h + margineY) - margineY;

    // Calcularea poziției ferestrelor în functie de dimensiunile matricei
    int windowX = (COLS - L) / 2;   // Centrare pe axa X
    int windowY = (LINES - H) / 2;  // Centrare pe axa Y

    // Setează dimensiunile și poziționarea ferestrei
    wresize(window, H, L);
    mvwin(window, windowY, windowX);

    // Desenează chenarele și elementele matricei în fereastră
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            // Calculează poziția pentru fiecare chenar și element
            int x = j * (l + margineX);
            int y = i * (h + margineY);

            // Desenează chenarul alb
            wattron(window, COLOR_PAIR(1) | A_REVERSE);
            box(window, 0, 0);
            wattroff(window, COLOR_PAIR(1) | A_REVERSE);
            if (joc[i][j] == 0)
                mvwprintw(window, y + 1, x + 2, " ", joc[i][j]);
            else
                // Desenează elementul matricei
                mvwprintw(window, y + 1, x + 2, "%d", joc[i][j]);
        }
    }

    wrefresh(window);
    refresh();
}
// MISCARE IN SUS
void move_up(int joc[][4], int *scor) {
    int i, j;
    int temp;
    for (j = 0; j < 4; j++) {
        int celdif0 = 0;
        for (i = 0; i < 4; i++) {
            if (joc[i][j] != 0) {
                if (celdif0 != i) {
                    // interschimba valoarea nenula cu cea mai de sus valoare nula 
                    temp = joc[i][j];
                    joc[i][j] = joc[celdif0][j];
                    joc[celdif0][j] = temp;
                }
                celdif0++;
            }
        }

        // muta zerourile la sfarsit
        for (i = 3; i >= celdif0; i--) {
            joc[i][j] = 0;
        }
    }

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 3; i++) {
            if (joc[i][j] == joc[i + 1][j]) {
                *scor += 2 * joc[i][j];
                joc[i][j] = 2 * joc[i][j];
                joc[i + 1][j] = 0;
            }
        }
    }

    for (j = 0; j < 4; j++) {
        int celdif0 = 0;
        for (i = 0; i < 4; i++) {
            if (joc[i][j] != 0) {
                if (celdif0 != i) {
                    // interschimba valoarea nenula cu cea mai de sus celula cu
                    // valoarea 0
                    temp = joc[i][j];
                    joc[i][j] = joc[celdif0][j];
                    joc[celdif0][j] = temp;
                }
                celdif0++;
            }
        }

        // Muta zero-urile la sfarsit
        for (i = 3; i >= celdif0; i--) {
            joc[i][j] = 0;
        }
    }

    // Adaugă un nou element aleatoriu
    nraleatoriu(joc);

    refresh();
}
// MISCARE IN JOS
void move_down(int joc[][4], int *scor) {
    int i, j, temp;
    for (j = 0; j < 4; j++) {
        int celdif0 = 3;
        for (i = 3; i >= 0; i--) {
            if (joc[i][j] != 0) {
                if (celdif0 != i) {
                    // interschimba valoarea nenula cu cea mai de jos valoare nula 
                    temp = joc[i][j];
                    joc[i][j] = joc[celdif0][j];
                    joc[celdif0][j] = temp;
                }
                celdif0--;
            }
        }

        // muta zerourile la inceput
        for (i = 0; i <= celdif0; i++) {
            joc[i][j] = 0;
        }
    }

    for (j = 0; j < 4; j++) {
        for (i = 3; i > 0; i--) {
            if (joc[i][j] == joc[i - 1][j]) {
                *scor += 2 * joc[i][j];
                joc[i][j] = 2 * joc[i][j];
                joc[i - 1][j] = 0;
            }
        }
    }

    for (j = 0; j < 4; j++) {
        int celdif0 = 3;
        for (i = 3; i >= 0; i--) {
            if (joc[i][j] != 0) {
                if (celdif0 != i) {
                    // interschimba valoarea nenula cu cea mai de sus valoare nula 
                    temp = joc[i][j];
                    joc[i][j] = joc[celdif0][j];
                    joc[celdif0][j] = temp;
                }
                celdif0--;
            }
        }

        // muta zerourile la inceput
        for (i = 0; i <= celdif0; i++) {
            joc[i][j] = 0;
        }
    }
    nraleatoriu(joc);
    refresh();
}
// MISCARE IN STANGA
void move_left(int joc[][4], int *scor) {
    int i, j, temp;
    for (i = 0; i < 4; i++) {
        int celdif0 = 0;
        for (j = 0; j < 4; j++) {
            if (joc[i][j] != 0) {
                if (celdif0 != j) {
                    // interschimba valoarea nenula cu cea mai din stanga valoare nula 
                    temp = joc[i][j];
                    joc[i][j] = joc[i][celdif0];
                    joc[i][celdif0] = temp;
                }
                celdif0++;
            }
        }

        // muta zerourile la sfarsit
        for (j = 3; j >= celdif0; j--) {
            joc[i][j] = 0;
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (joc[i][j] == joc[i][j + 1]) {
                *scor += 2 * joc[i][j];
                joc[i][j] = 2 * joc[i][j];
                joc[i][j + 1] = 0;
            }
        }
    }

    for (i = 0; i < 4; i++) {
        int celdif0 = 0;
        for (j = 0; j < 4; j++) {
            if (joc[i][j] != 0) {
                if (celdif0 != j) {
                    // interschimba valoarea nenula cu cea mai din stanga valoare nula 
                    temp = joc[i][j];
                    joc[i][j] = joc[i][celdif0];
                    joc[i][celdif0] = temp;
                }
                celdif0++;
            }
        }

        // Muta zerourile la sfarsit
        for (j = 3; j >= celdif0; j--) {
            joc[i][j] = 0;
        }
    }
    nraleatoriu(joc);
    refresh();
}
// MISCARE IN DREAPTA
void move_right(int joc[][4], int *scor) {
    int i, j, temp;
    for (i = 0; i < 4; i++) {
        int celdif0 = 3;
        for (j = 3; j >= 0; j--) {
            if (joc[i][j] != 0) {
                if (celdif0 != j) {
                    // interschimba valoarea nenula cu cea mai din dreapta valoare nula 
                    temp = joc[i][j];
                    joc[i][j] = joc[i][celdif0];
                    joc[i][celdif0] = temp;
                }
                celdif0--;
            }
        }

        // Muta zerourile la inceput
        for (j = 0; j <= celdif0; j++) {
            joc[i][j] = 0;
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 3; j > 0; j--) {
            if (joc[i][j] == joc[i][j - 1]) {
                *scor += 2 * joc[i][j];
                joc[i][j] = 2 * joc[i][j];
                joc[i][j - 1] = 0;
            }
        }
    }

    for (i = 0; i < 4; i++) {
        int celdif0 = 3;
        for (j = 3; j >= 0; j--) {
            if (joc[i][j] != 0) {
                if (celdif0 != j) {
                    // interschimba valoarea nenula cu cea mai din dreapta valoare nula 
                    temp = joc[i][j];
                    joc[i][j] = joc[i][celdif0];
                    joc[i][celdif0] = temp;
                }
                celdif0--;
            }
        }

        // Mouta zerourile la inceput
        for (j = 0; j <= celdif0; j++) {
            joc[i][j] = 0;
        }
    }
    nraleatoriu(joc);
    refresh();
}
// FUNCTIE DE AFISARE A TIMPULUI CURENT
void timp_curent(char *timp) {
    time_t tmp;
    struct tm *timeInfo;
    time(&tmp);
    timeInfo = localtime(&tmp);
    strftime(timp, 20, "%Y-%m-%d %H:%M:%S", timeInfo);
}
// FUNCTIE PENTRU A ARATA TIMPUL CURENT IN MOD CONTINUU
void timp_continuu(WINDOW *panou) {
    start_color();
    init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
    char timp[20];
    timp_curent(timp);
    wmove(panou, 6, 1);
     wattron(panou, COLOR_PAIR(3) | A_REVERSE);
    wprintw(panou, "Time: %s", timp);
    wattroff(panou, COLOR_PAIR(3) | A_REVERSE);
    wrefresh(panou);
}
// VERIFICARE DACA JOCUL S-A INCHEIAT(pierdut sau câștigat)
int joc_incheiat(int joc[][4]) {
    // Verifică dacă există celule libere
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (joc[i][j] == 0) {
                return 1;
            }
        }
    }

    // Verifică dacă există mutări posibile
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // Verifică mutări posibile în sus
            if (i > 0 && joc[i][j] == joc[i - 1][j]) {
                return 0;
            }
            // Verifică mutări posibile în jos
            if (i < 3 && joc[i][j] == joc[i + 1][j]) {
                return 0;
            }
            // Verifică mutări posibile la stânga
            if (j > 0 && joc[i][j] == joc[i][j - 1]) {
                return 0;
            }
            // Verifică mutări posibile la dreapta
            if (j < 3 && joc[i][j] == joc[i][j + 1]) {
                return 0;
            }
        }
    }

    // Verifică dacă există un bloc cu valoarea 2048
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (joc[i][j] == 2048) {
                return 1;  // Jocul a fost câștigat
            }
        }
    }

    // Dacă niciuna dintre condițiile de mai sus nu este îndeplinită, jocul este
    // pierdut
    return 1;
}
// PANOUL DE CONTROL CU CARACTERISTICILE LUI
void panou_control(WINDOW *panou, int scor) {
    start_color();
    init_pair(2,COLOR_YELLOW , COLOR_BLACK);
    wclear(panou);
    wattron(panou, COLOR_PAIR(2) | A_REVERSE);
    wprintw(panou, " Score: %d\n", scor);
    wattroff(panou, COLOR_PAIR(2) | A_REVERSE);
    wprintw(panou, " -> sageata dreapta\n");
    wprintw(panou, " <- sageata stanga\n");
    wprintw(panou, " | sageata sus\n");
    wprintw(panou, " | sageata jos\n");
    timp_continuu(panou);
    wprintw(panou, "\n");
    wprintw(panou, "\n");
    wprintw(panou, "Pentru a va intoarce la meniu apasati tasta Q");
    wrefresh(panou);
    refresh();
}
// FUNCTIE CARE SALVEAZA INTR-UN VECTOR SEPARAT ELEMENTELE SI SCORUL DIN TABLA
// DE JOC ACTUALA
void salveaza_joc(int joc_salvat[][4], int joc[][4], int *scor_salvat,
                  int *scor, int *pauza) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            joc_salvat[i][j] = joc[i][j];
        }
    }
    *scor_salvat = *scor;
    *pauza = 1;
}
// FUNCTIE CARE TRANSPORTA JOCUL SI SCORUL DIN JOCUL ULTERIOR IN TABLA ACTUALA
void incarca_joc(int joc_salvat[][4], int joc[][4], int *scor_salvat, int *scor,
                 int *pauza) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            joc[i][j] = joc_salvat[i][j];
        }
    }
    *scor = *scor_salvat;
    *pauza = 0;
}
int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    srand(time(NULL));
    start_color();
    init_pair(4,COLOR_RED , COLOR_BLACK);
    init_pair(2,COLOR_YELLOW , COLOR_BLACK);
    int scor = 0;
    int scor_salvat = 0;
    int joc[4][4];
    int joc_salvat[4][4];
    int pauza = 0;
    int i, j;
    int input;
    char optiuni[3][9] = {"New Game", "Resume", "Quit"};
    char timp[20];
    WINDOW *window = newwin(5, 80, 1, 30);
    WINDOW *panou = newwin(11, 40, 1, 70);
    WINDOW *mesaj_fereastra = newwin(5, 20, 1, 15);
    WINDOW *no_resume = newwin(5, 50, 0, 0);
    timeout(1000);
    refresh();
    keypad(window, TRUE);
    while (1) {
        int selectie = navigare(optiuni);
        switch (selectie) {
            case 0:  // NEW GAME
                clear();
                start(joc, &scor);
                afisare_tabla(window, joc);
                panou_control(panou, scor);
                afisare_tabla(window, joc);
                input = getch();
                keypad(mesaj_fereastra, TRUE);
                refresh();
                // Cat timp tasta este diferita de tasta de iesire, sa o
                // citeasca cand este egala cu Q, salveaza tabla de joc si intra
                // in meniu
                while (input != 'q') {
                    switch (input) {
                        case KEY_UP:
                            move_up(joc, &scor);
                            afisare_tabla(window, joc);
                            panou_control(panou, scor);
                            break;

                        case KEY_DOWN:
                            move_down(joc, &scor);
                            afisare_tabla(window, joc);
                            panou_control(panou, scor);
                            break;

                        case KEY_LEFT:
                            move_left(joc, &scor);
                            afisare_tabla(window, joc);
                            panou_control(panou, scor);
                            break;

                        case KEY_RIGHT:
                            move_right(joc, &scor);
                            afisare_tabla(window, joc);
                            panou_control(panou, scor);
                            break;
                        default:
                            break;
                    }
                    input = getch();
                    // Verifică condițiile pentru sfârșitul jocului
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            if (joc[i][j] == 2048) {
                                // Jocul a fost câștigat
                                wclear(mesaj_fereastra);
                                wattron(mesaj_fereastra, COLOR_PAIR(2) | A_REVERSE);
                                wprintw(mesaj_fereastra, "  You won! \n");
                                wattroff(mesaj_fereastra, COLOR_PAIR(2) | A_REVERSE);
                                wprintw(mesaj_fereastra, "\n");
                                wprintw(
                                    mesaj_fereastra,
                                    "Apasa tasta Q pestru a reveni la meniu\n");
                               
                                wrefresh(mesaj_fereastra);

                                break;
                            } else if (nr_celule_goale(joc) == 0 &&
                                       joc_incheiat(joc) != 0) {
                                // Jocul a fost pierdut
                                wclear(mesaj_fereastra);
                                wattron(mesaj_fereastra, COLOR_PAIR(4) | A_REVERSE);
                                wprintw(mesaj_fereastra, "  Game Over! \n");
                                wattroff(mesaj_fereastra, COLOR_PAIR(4) | A_REVERSE);
                                wprintw(mesaj_fereastra, "\n");
                                wprintw(
                                    mesaj_fereastra,
                                    "Apasa tasta Q pestru a reveni la meniu\n");
                                wrefresh(mesaj_fereastra);

                                break;
                            }
                        }
                    }
                    timp_curent(timp);
                    napms(100);
                    panou_control(panou, scor);
                    if (input == 'q')
                        salveaza_joc(joc_salvat, joc, &scor_salvat, &scor,
                                     &pauza);
                }
                break;

            case 1:  // RESUME
                clear();
                // Păstrează starea curentă a jocului
                // Variabila pauza retine daca a fost salvat jocul anterior
                if (pauza == 1) {
                    incarca_joc(joc_salvat, joc, &scor_salvat, &scor, &pauza);
                    afisare_tabla(window, joc);
                    panou_control(panou, scor);
                    afisare_tabla(window, joc);
                    int input_resume = getch();  // Așteaptă input continuu
                    // Cat timp tasta este diferita de tasta de iesire, sa o
                    // citeasca cand este egala cu Q, salveaza tabla de joc si
                    // intra in meniu
                    while (input_resume != 'q') {
                        switch (input_resume) {
                            case KEY_UP:
                                move_up(joc, &scor);
                                afisare_tabla(window, joc);
                                panou_control(panou, scor);
                                break;

                            case KEY_DOWN:
                                move_down(joc, &scor);
                                afisare_tabla(window, joc);
                                panou_control(panou, scor);
                                break;

                            case KEY_LEFT:
                                move_left(joc, &scor);
                                afisare_tabla(window, joc);
                                panou_control(panou, scor);
                                break;

                            case KEY_RIGHT:
                                move_right(joc, &scor);
                                afisare_tabla(window, joc);
                                panou_control(panou, scor);
                                break;
                        }

                        // Verifică condițiile pentru sfârșitul jocului
                        for (i = 0; i < 4; i++) {
                            for (j = 0; j < 4; j++) {
                                if (joc[i][j] == 2048) {
                                    // Jocul a fost câștigat
                                    wclear(mesaj_fereastra);
                                    wattron(mesaj_fereastra, COLOR_PAIR(2) | A_REVERSE);
                                    wprintw(mesaj_fereastra, "  You won! \n");
                                    wattroff(mesaj_fereastra, COLOR_PAIR(2) | A_REVERSE);
                                    wprintw(mesaj_fereastra, "\n");
                                    wprintw(mesaj_fereastra,
                                            "Apasa tasta Q pestru a reveni la "
                                            "meniu\n");
                                    wrefresh(mesaj_fereastra);
                                    napms(200);
                                    input = 'q';
                                    break;
                                } else if (nr_celule_goale(joc) == 0 &&
                                           joc_incheiat(joc) != 0) {
                                    // Jocul a fost pierdut
                                    wclear(mesaj_fereastra);
                                    wattron(mesaj_fereastra, COLOR_PAIR(4) | A_REVERSE);
                                    wprintw(mesaj_fereastra,
                                            "  Game Over! \n");
                                    wattroff(mesaj_fereastra, COLOR_PAIR(4) | A_REVERSE);
                                    wprintw(mesaj_fereastra, "\n");
                                    wprintw(mesaj_fereastra,
                                            "Apasa tasta Q pestru a reveni la "
                                            "meniu\n");
                                    wrefresh(mesaj_fereastra);
                                    napms(200);
                                    input = 'q';
                                    break;
                                }
                            }
                        }
                        // Afișează timpul și panoul de control
                        timp_continuu(panou);
                        panou_control(panou, scor);
                        input_resume = getch();
                        if (input_resume == 'q') {
                            salveaza_joc(joc_salvat, joc, &scor_salvat, &scor,
                                         &pauza);
                            break;
                        }
                        refresh();
                    }
                }
                // In cazul in care nu este salvata niciun joc anterior
                if (pauza == 0) {
                    clear();
                    wclear(no_resume);
                    wprintw(no_resume, "NU EXISTA UN JOC ANTERIOR!");
                    refresh();
                    wrefresh(no_resume);
                    napms(500);
                }
                break;
            case 2:  // QUIT

                endwin();
                return 0;
        }
    }
    endwin();

    return 0;
}
