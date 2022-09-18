#include YSI_Coding\y_hooks

enum e_quizData {
    bool:quiz_active,
    quiz_type,
    quiz_buffer[50],
    quiz_result,
    quiz_tick,
    quiz_timer,
    quiz_saveBuffer[50],
    quiz_count,
};
new quizData[e_quizData];

#define QUIZ_CODE       0
#define QUIZ_MATH       1
#define QUIZ_WORD       2

stock Init_Quiz()
{
    m_pquery_format("UPDATE es_accounts SET daily=-1, daily_status=0, daily_done=0, daily_reward='0,0';"); // sadeg XDDD

    quizData[quiz_active] = false;
    SetTimer("StartQuiz", 1000 * 60 * 3, false);
}

stock UpdateQuizDraw(playerid)
{
    new qstr[256];
    switch(quizData[quiz_type]) {
        case QUIZ_CODE: format(qstr, sizeof(qstr), "PRZEPISZ KOD:~n~~w~~h~%s", quizData[quiz_buffer]);
        case QUIZ_MATH: format(qstr, sizeof(qstr), "OBLICZ:~n~~w~~h~%s", quizData[quiz_saveBuffer]);
        case QUIZ_WORD: format(qstr, sizeof(qstr), "ROZSYPANKA:~n~~w~~h~%s", quizData[quiz_saveBuffer]);
    }

    PlayerTextDrawSetString(playerid, tdQuiz, qstr);
    return 1;
}

function StartQuiz()
{
    if(quizData[quiz_active]) return 1;

    quizData[quiz_count] = 0;
    quizData[quiz_active] = true;
    quizData[quiz_type] = random(3);
    quizData[quiz_buffer][0] = EOS;
    quizData[quiz_tick] = gettime();

    new qstr[150];
    switch(quizData[quiz_type]) {
        case QUIZ_CODE: {
            static const codeLetters[] = {
                'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'O', 'P', 'A', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', 
                'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 'd', 'f', 'g', 'h', 'j', 'k', 's', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
                '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
            };

            for(new i = 0, len = math_random(7, 15); i != len; i++) format(quizData[quiz_buffer][i], 2, "%c", codeLetters[random(sizeof(codeLetters))]);
            format(qstr, sizeof(qstr), "PRZEPISZ KOD:~n~~w~~h~%s", quizData[quiz_buffer]);
            msgAll(C_CREAM, "Pojawi³ siê {c}kod{/c} do przepisania! Kod: {c}%s{/c}.", quizData[quiz_buffer]);
        }

        case QUIZ_MATH: {
            new numbers[3];
            numbers[0] = math_random(1, 7);
            numbers[1] = math_random(3, 10);
            numbers[2] = math_random(2, 6);

            switch(random(4)) {
                case 0: {
                    quizData[quiz_result] = numbers[0] + numbers[1] + numbers[2];
                    format(quizData[quiz_saveBuffer], 50, "%d + %d + %d", numbers[0], numbers[1], numbers[2]);
                }

                case 1: {
                    quizData[quiz_result] = numbers[0] - numbers[1] - numbers[2];
                    format(quizData[quiz_saveBuffer], 50, "%d - %d - %d", numbers[0], numbers[1], numbers[2]);
                }

                case 2: {
                    quizData[quiz_result] = numbers[0] * numbers[1] + numbers[2];
                    format(quizData[quiz_saveBuffer], 50, "%d x %d + %d", numbers[0], numbers[1], numbers[2]);
                }

                case 3: {
                    quizData[quiz_result] = numbers[0] * numbers[1] - numbers[2];
                    format(quizData[quiz_saveBuffer], 50, "%d x %d - %d", numbers[0], numbers[1], numbers[2]);
                }
            }

            format(quizData[quiz_buffer], 50, "%d", quizData[quiz_result]);
            format(qstr, sizeof(qstr), "OBLICZ:~n~~w~~h~%s", quizData[quiz_saveBuffer]);
            msgAll(C_CREAM, "Pojawi³o siê {c}równanie{/c} matematyczne! Oblicz: {c}%s{/c}.", quizData[quiz_saveBuffer]);
        }

        case QUIZ_WORD: {
            static const wordList[][] = {
                "Broki", "ES", "Gamemode", "Serwer", "Owoc", "Warzywo", "Piornik",
                "Komputer", "Procesor", "Pamiec", "Metamorfoza", "Reinkarnacja", "Zel",
                "Religia", "Samochod", "Auto", "Traktor", "Motor", "Samolot", "Szampon", "Keef",
                "Helikopter", "Pistolet", "Karabin", "Snajperka", "Vester", "Sloneczki", "Zaczoles",
                "Prokurator", "Policjant", "Konfident", "Pedro", "Lou", "Chipsy", "Plecak",
                "Carbonara", "Tenshin", "Impreza", "Party", "Chlopak", "Lays", "Szkola", "Ksiazka",
                "Dziewczyna", "Dziwka", "Szmata", "Suka", "Pies", "Kot", "Mysz", "Sperma", "Kanal",
                "Swinia", "Krowa", "Owca", "Kurczak", "Kebab", "Pizza", "Delicje", "Ruchanie",
                "Taco", "Burger", "Cheeseburger", "KFC", "McDonalds", "Wrap", "Sex", "Kutas", "Cipa",
                "Twister", "Drwal", "Nuggets", "Frytki", "Ketchup", "Majonez", "Baklazan", "Banan",
                "Musztarda", "Slodki", "Kwasny", "Mieszany", "Wolowina", "Baranina", "Zalno", "Mati Smakosz",
                "Spaghetti", "Sranie", "Sraka", "Kupsztal", "Rollo", "Bulka", "Pita", "Adrian", "uhqqow",
                "Chleb", "Piwo", "Kasztelan", "Somerek", "Stock", "Soplica", "Kac", "Nowak", "Vacex",
                "CocaCola", "Pepsi", "Fanta", "Sprite", "Polaris", "Magnevita", "Maltanki", "Vcxz",
                "Mateusz", "Kuba", "Blazej", "Kamil", "Oliwia", "Nikola", "Weronika", "Karolina",
                "Marta", "Julia", "Piotr", "Zuzia", "Szymon", "Kacper", "Roksana", "Nadia", "Bartek",
                "Jozef", "Michal", "Konrad", "Kunekunda", "Algida", "Dupogrzebek", "Hatty", "Mikolaj",
                "Ola", "Aleksandra", "Henryk", "Janek", "Janusz", "Agnieszka", "Bialek", "Grzyb", "Chuj",
                "Renata", "Stanislawa", "Wieslawa", "Oskar", "Dominika", "Pierdolenie", "Kurwa", "Gowno",
                "Natalia", "Szczur", "Diament", "Szafir", "Szmaragd", "Brylant", "Wegiel", "Zelazo", "Wagina",
                "Zloto", "Skarb", "Milosc", "Stulejarz", "Simp", "DeathMatch", "RolePlay", "2115", "2137",
                "1337", "420", "Weed", "Marihuana", "Kokaina", "LSD", "Mefedron", "Heroina", "Twarde", "Miekkie",
                "Fantazja", "Gratka", "Danio", "Monte", "Papryka", "Pomarancza", "Pomidor", "Cebula", "Napad",
                "Fromage", "Mleko", "Napoj", "Ubranie", "Jedzenie"
            };

            format(quizData[quiz_buffer], 50, "%s", wordList[random(sizeof(wordList))]);
            quizData[quiz_buffer][0] = toupper(quizData[quiz_buffer][0]);

            mix_string(quizData[quiz_buffer], quizData[quiz_saveBuffer]);
            format(qstr, sizeof(qstr), "ROZSYPANKA:~n~~w~~h~%s", quizData[quiz_saveBuffer]);
            msgAll(C_CREAM, "Pojawi³ siê {c}wyraz{/c} do odszyfrowania. Wyraz: {c}%s{/c}.", quizData[quiz_saveBuffer]);
        }
    }

    foreach(new playerid : Player) {
        if(playerData[playerid][player_logged] && IsPlayerSpawned(playerid)) {
            playSound(playerid, 1085);
            PlayerTextDrawSetString(playerid, tdQuiz, qstr);
            PlayerTextDrawShow(playerid, tdQuiz);
        }
        playerData[playerid][player_quizDone] = false;
    }

    KillTimer(quizData[quiz_timer]);
    quizData[quiz_timer] = SetTimerEx("StopQuiz", 1000 * 60 * 10, false, "b", false);
    return 1;
}

function StopQuiz(bool:done)
{
    if(!quizData[quiz_active]) return 1;

    quizData[quiz_active] = false;
    KillTimer(quizData[quiz_timer]);

    new time = math_random(3, 7);
    if(!done) msgAll(C_VIOLET2, "Nikt nie rozwi¹za³ testu reakcji. Nastêpny pojawi siê po up³ywie {c}%d{/c} %s.", time, ConvertTimeFormat(time,  "minut", "minutê", "minuty", "minut"));
    foreach(new i : Player) PlayerTextDrawHide(i, tdQuiz);

    quizData[quiz_timer] = SetTimer("StartQuiz", 1000 * 60 * time, false);
    return 1;
}
