#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define MAX 100

FILE *fd;

typedef struct{
    char nick[21];
    int pts;
}DadosPlayer;

DadosPlayer player[1];

DadosPlayer players[MAX];

int dificuldade = 1, fase = 0, terminou = 0;

void clearScreen();

void telaInicial(DadosPlayer player[]);

int menu();

int compare(const void *a, const void *b);

void mudarDificuldade();

void zerarRanking();

void carregaRanking();

void atualizarPontuacao();

void salvaRanking();

void adicionarRanking();

void configuracoes();

void instrucoes();

void jogarIniciante(int matriz[][7][2], int somaLinha[], int somaColuna[]);

void jogarIntermediario(int matriz[][7][2], int somaLinha[], int somaColuna[]);

void jogarAvancado(int matriz[][7][2], int somaLinha[], int somaColuna[]);

void jogar();

int main(){

    int loop = 1;

    telaInicial(player);

    while(loop != 0){
    loop = menu();
    }

    return 0;
}

int compare(const void *a, const void *b) {
    DadosPlayer *playerA = (DadosPlayer *)a;
    DadosPlayer *playerB = (DadosPlayer *)b;

    if (playerA->pts > playerB->pts) return -1;
    if (playerA->pts < playerB->pts) return 1;
    return strcmp(playerA->nick, playerB->nick);
}

int menu(){
    int op;

    clearScreen();

    do{
    printf("### Crazy Sums ###\n\n");
    printf("1 - Jogar\n");
    printf("2 - Configuracoes\n");
    printf("3 - Instrucoes\n");
    printf("4 - Ranking\n");
    printf("5 - Sair\n\n");
    printf("Digite a opcao desejada: ");
    scanf("%d", &op);
    getchar();

    switch (op){
            case 1:
                jogar();
                break;
            case 2:
                configuracoes();
                break;
            case 3:
               instrucoes();
               clearScreen();
                break;
            case 4:
                carregaRanking();
                break;
            case 5:
                return 0;
                break;
            default:
                clearScreen();
                printf("Opcao invalida!\n");
                break;
        }
    }while(op != 5);

    return 0;
}

void mudarDificuldade(){
    int op;

    clearScreen();
    
    while(op != 4){
    printf("### MUDAR DIFICULDADE ###\n\n");
    printf("Dificuldade atual: ");
    switch (dificuldade){
        case 1:
            printf("Iniciante\n");
            break;
        case 2:
            printf("Intermediario\n");
            break;
        case 3:
            printf("Avancado\n");
            break;
    }
    printf("\n\n");
    printf("1 - Iniciante\n");
    printf("2 - Intermediario\n");
    printf("3 - Avancado\n");
    printf("4 - Voltar\n\n");
    printf("Digite a opcao desejada: ");
    scanf("%d", &op);
    getchar();

    switch (op){
        case 1:
            dificuldade = 1;
            fase = 0;
            clearScreen();
            break;
        case 2:
            dificuldade = 2;
            fase = 0;
            clearScreen();
            break;
        case 3:
            dificuldade = 3;
            fase = 0;
            clearScreen();
            break;
        case 4:
            break;
        default:
            clearScreen();
            printf("Opcao invalida!\n");
            break;
        }
    }
}

void zerarRanking(){
    char op;
    printf("Confirma reiniciar o ranking? (S/N): ");
    scanf("%c", &op);
    getchar();

    if(op == 'S' || op == 's'){
        char file[] = "ranking.bin";
        FILE * fb = fopen(file, "wb");
        int qtd = 0;
        fwrite(&qtd, sizeof(int), 1, fb);
        fclose(fb);
        printf("Ranking zerado com sucesso!\n");
        system("pause");
    }
    else{
        printf("Operacao cancelada!\n");
        system("pause");
    }
}

void configuracoes(){
    int op;

    clearScreen();

    while(op != 3){
        printf("### CONFIGURACOES ###\n\n");
        printf("1 - Zerar Ranking\n");
        printf("2 - Mudar Dificuldade\n");
        printf("3 - Voltar ao menu principal\n\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &op);
        getchar();

        switch (op){
            case 1:
                zerarRanking();
                clearScreen();
                break;
            case 2:
                mudarDificuldade();
                clearScreen();
                break;
            case 3:
                clearScreen();
                break;
            default:
                clearScreen();
                printf("Opcao invalida!\n");
                break;
        }
    }
}

void clearScreen(){
    system(CLEAR);
}

void telaInicial(DadosPlayer player[]){
    printf("Bem vindo(a) ao Crazy Sums!\n\n\n");
    printf("Digite seu nick(max 20 caracteres): ");
    scanf("%s", player[0].nick);
    atualizarPontuacao();
}

void instrucoes(){
    clearScreen();
    printf("### INSTRUCOES ###\n\n");
    printf("O jogo consiste em eliminar numeros em uma matriz.\n");
    printf("Em cada nivel eh apresentada uma matriz, onde a frente de cada linha e coluna\n");
    printf("eh apresentado o resultado da soma dos numeros daquela linha ou coluna.\n");
    printf("Porem, ha numeros na matriz que nao satisfazem essas somas.\n");
    printf("O jogador deve encontrar esses numeros e elimina-los.\n");
    printf("O jogador ganha pontos a cada nivel completado.\n");
    printf("A quantidade de pontos ganha depende da dificuldade escolhida:\n");
    printf("Iniciante - 50 pontos\nIntermediario - 100 pontos\nAvancado - 200 pontos\n");
    printf("Ha 4 fases por nivel de dificuldade, e o jogador deve completar todas as fases para avancar de nivel.\n");
    printf("O jogo acaba quando o jogador erra 5 vezes por fase.\n\n");
    printf("Boa sorte!\n\n");
    system("pause");
}

void jogarIniciante(int matriz[][7][2], int somaLinha[], int somaColuna[]){

    char inputMatriz[5], inputSoma[9];

    fd = fopen("../iniciante.txt", "r");

    
        int vidas = 5;

        fseek(fd, fase * 60, SEEK_SET);

        for(int i = 0 ; i < 4 ; i++){
            fscanf(fd, "%s", inputMatriz);
            for(int j = 0 ; j < 4 ; j++){
                matriz[i][j][0] = inputMatriz[j] - '0';
            }
            fgetc(fd);
        }

        fscanf(fd, "%s", inputSoma);
        for(int i = 0 ; i < 8 ; i+=2){
            somaColuna[i/2] = (inputSoma[i] - '0') * 10 + (inputSoma[i+1] - '0');
        }
        fgetc(fd);

        fscanf(fd, "%s", inputSoma);
        for(int i = 0 ; i < 8 ; i+=2){
            somaLinha[i/2] = (inputSoma[i] - '0') * 10 + (inputSoma[i+1] - '0');
        }
        fgetc(fd);

        for(int i = 0 ; i < 4 ; i++){
            fscanf(fd, "%s", inputMatriz);
            for(int j = 0 ; j < 4 ; j++){
                matriz[i][j][1] = inputMatriz[j] - '0';
            }
            fgetc(fd);
        }

        fclose(fd);

        while(vidas > 0){
            printf("### Fase %d ###\n\n", fase + 1);

            printf("    ");
            for(int i = 0; i < 4; i++){
                if(somaColuna[i] > 0){
                    if(somaColuna[i] < 10){
                        printf("0%d ", somaColuna[i]);
                    }
                    else{
                        printf("%2d ", somaColuna[i]);
                    }
                }
                else{
                    printf("   ");
                }
            }
            printf("\n   ______________\n");

            for(int i = 0; i < 4; i++){
                if(somaLinha[i] > 0){
                    if(somaLinha[i] < 10){
                        printf("0%d |", somaLinha[i]);
                    }
                    else{
                        printf("%2d |", somaLinha[i]);
                    }
                }
                else{
                    printf("   |");
                }
               
            for(int j = 0; j < 4; j++){
                if(matriz[i][j][0] > 0){
                    printf("%d  ",(matriz[i][j][0]));
                }
                else{
                    printf("   ");
                }
            }
            printf("\n");
            }

            printf("\n");
            printf("### Voce tem %d vidas ###\n\n", vidas);
            printf("Digite a linha e a coluna do numero que deseja eliminar: ");
            
            int linha, coluna;

            scanf("%d %d", &linha, &coluna);
            getchar();

            linha--; coluna--;

            if(linha < 0 || linha > 3 || coluna < 0 || coluna > 3){
                clearScreen();
                printf("Posicao invalida!\n\n");
                continue;
            }

            if(matriz[linha][coluna][1] == 1){
                matriz[linha][coluna][1] = 3;
                vidas--;
                clearScreen();
                printf("Voce errou!\n\n");
            }
            else if(matriz[linha][coluna][1] == 0){
                matriz[linha][coluna][0] = 0;
                matriz[linha][coluna][1] = 2;
                clearScreen();
                printf("Numero eliminado com sucesso!\n\n");
            }
            else if(matriz[linha][coluna][1] == 2){
                clearScreen();
                printf("Numero ja eliminado!\n\n");

            }
            else{
                clearScreen();
                printf("Voce ja tentou esse numero!\n\n");
            }

            for(int i = 0; i < 4; i++){
                if((matriz[i][0][1] * matriz[i][1][1] * matriz[i][2][1] * matriz[i][3][1] != 0) && (somaLinha[i] != 0)){
                    somaLinha[i] = -1;
                }
                if((matriz[0][i][1] * matriz[1][i][1] * matriz[2][i][1] * matriz[3][i][1] != 0) && (somaColuna[i] != 0)){
                    somaColuna[i] = -1;
                }
                if(somaColuna[i] == -1){
                    printf("Parabens, voce fechou a coluna %d!\n", i + 1);
                    somaColuna[i] = 0;
                }
                if(somaLinha[i] == -1){
                    printf("Parabens, voce fechou a linha %d!\n", i + 1);
                    somaLinha[i] = 0;
                }
            }

            int flag = 1;

            for(int i = 0; i < 4; i++){
                for(int j = 0; j < 4; j++){
                    flag *= matriz[i][j][1];
                }
            }

            if(flag != 0){
                break;
            }
        }
        if(vidas > 0){
            clearScreen();
            printf("Parabens! Voce completou a fase %d!\n", fase + 1);
            player[0].pts += 50;
            adicionarRanking();
            fase++;
        }
        else{
            clearScreen();
            printf("Voce perdeu! Tente novamente!\n");
        }
        system("pause");
        clearScreen();
        return;
    

    if(fase == 4){
        printf("Parabens! Voce completou o nivel Iniciante!\n");
        printf("Tente o nivel Intermediario!\n");
        dificuldade++;
        system("pause");
    }
    return;
}

void jogarIntermediario(int matriz[][7][2], int somaLinha[], int somaColuna[]){

    char inputMatriz[7], inputSoma[13];

    fd = fopen("../intermediario.txt", "r");

    
        int vidas = 5;

        fseek(fd, fase * 112, SEEK_SET);

        for(int i = 0 ; i < 6 ; i++){
            fscanf(fd, "%s", inputMatriz);
            for(int j = 0 ; j < 6 ; j++){
                matriz[i][j][0] = inputMatriz[j] - '0';
            }
            fgetc(fd);
        }

        fscanf(fd, "%s", inputSoma);
        for(int i = 0 ; i < 12 ; i+=2){
            somaColuna[i/2] = (inputSoma[i] - '0') * 10 + (inputSoma[i+1] - '0');
        }
        fgetc(fd);

        fscanf(fd, "%s", inputSoma);
        for(int i = 0 ; i < 12 ; i+=2){
            somaLinha[i/2] = (inputSoma[i] - '0') * 10 + (inputSoma[i+1] - '0');
        }
        fgetc(fd);

        for(int i = 0 ; i < 6 ; i++){
            fscanf(fd, "%s", inputMatriz);
            for(int j = 0 ; j < 6 ; j++){
                matriz[i][j][1] = inputMatriz[j] - '0';
            }
            fgetc(fd);
        }

        fclose(fd);

        while(vidas > 0){
            printf("### Fase %d ###\n\n", fase + 1);

            printf("    ");
            for(int i = 0; i < 6; i++){
                if(somaColuna[i] > 0){
                    if(somaColuna[i] < 10){
                        printf("0%d ", somaColuna[i]);
                    }
                    else{
                        printf("%2d ", somaColuna[i]);
                    }
                }
                else{
                    printf("   ");
                }
            }
            printf("\n   ___________________\n");

            for(int i = 0; i < 6; i++){
                if(somaLinha[i] > 0){
                    if(somaLinha[i] < 10){
                        printf("0%d |", somaLinha[i]);
                    }
                    else{
                        printf("%2d |", somaLinha[i]);
                    }
                }
                else{
                    printf("   |");
                }
               
            for(int j = 0; j < 6; j++){
                if(matriz[i][j][0] > 0){
                    printf("%d  ",(matriz[i][j][0]));
                }
                else{
                    printf("   ");
                }
            }
            printf("\n");
            }

            printf("\n");
            printf("### Voce tem %d vidas ###\n\n", vidas);
            printf("Digite a linha e a coluna do numero que deseja eliminar: ");
            
            int linha, coluna;

            scanf("%d %d", &linha, &coluna);
            getchar();

            linha--; coluna--;

            if(linha < 0 || linha > 5 || coluna < 0 || coluna > 5){
                clearScreen();
                printf("Posicao invalida!\n\n");
                continue;
            }

            if(matriz[linha][coluna][1] == 1){
                matriz[linha][coluna][1] = 3;
                vidas--;
                clearScreen();
                printf("Voce errou!\n\n");
            }
            else if(matriz[linha][coluna][1] == 0){
                matriz[linha][coluna][0] = 0;
                matriz[linha][coluna][1] = 2;
                clearScreen();
                printf("Numero eliminado com sucesso!\n\n");
            }
            else if(matriz[linha][coluna][1] == 2){
                clearScreen();
                printf("Numero ja eliminado!\n\n");

            }
            else{
                clearScreen();
                printf("Voce ja tentou esse numero!\n\n");
            }

            for(int i = 0; i < 6; i++){
                if((matriz[i][0][1] * matriz[i][1][1] * matriz[i][2][1] * matriz[i][3][1] != 0) && (somaLinha[i] != 0)){
                    somaLinha[i] = -1;
                }
                if((matriz[0][i][1] * matriz[1][i][1] * matriz[2][i][1] * matriz[3][i][1] != 0) && (somaColuna[i] != 0)){
                    somaColuna[i] = -1;
                }
                if(somaColuna[i] == -1){
                    printf("Parabens, voce fechou a coluna %d!\n", i + 1);
                    somaColuna[i] = 0;
                }
                if(somaLinha[i] == -1){
                    printf("Parabens, voce fechou a linha %d!\n", i + 1);
                    somaLinha[i] = 0;
                }
            }

            int flag = 1;

            for(int i = 0; i < 6; i++){
                for(int j = 0; j < 6; j++){
                    flag *= matriz[i][j][1];
                }
            }

            if(flag != 0){
                break;
            }
        }
        
        if(vidas > 0){
            clearScreen();
            printf("Parabens! Voce completou a fase %d!\n", fase + 1);
            player[0].pts += 100;
            adicionarRanking();
            fase++;
        }
        else{
            clearScreen();
            printf("Voce perdeu! Tente novamente!\n");
        }
        system("pause");
        clearScreen();
        return;
    

    if(fase == 4){
        printf("Parabens! Voce completou o nivel Intermediario!\n");
        printf("Tente o nivel Avancado!\n");
        dificuldade++;
        system("pause");
    }

    return;
}

void jogarAvancado(int matriz[][7][2], int somaLinha[], int somaColuna[]){

    char inputMatriz[8], inputSoma[15];

    fd = fopen("../avancado.txt", "r");

    
        int vidas = 5;

        fseek(fd, fase * 144, SEEK_SET);

        for(int i = 0 ; i < 7 ; i++){
            fscanf(fd, "%s", inputMatriz);
            for(int j = 0 ; j < 7 ; j++){
                matriz[i][j][0] = inputMatriz[j] - '0';
            }
            fgetc(fd);
        }

        fscanf(fd, "%s", inputSoma);
        for(int i = 0 ; i < 14 ; i+=2){
            somaColuna[i/2] = (inputSoma[i] - '0') * 10 + (inputSoma[i+1] - '0');
        }
        fgetc(fd);

        fscanf(fd, "%s", inputSoma);
        for(int i = 0 ; i < 14 ; i+=2){
            somaLinha[i/2] = (inputSoma[i] - '0') * 10 + (inputSoma[i+1] - '0');
        }
        fgetc(fd);

        for(int i = 0 ; i < 7 ; i++){
            fscanf(fd, "%s", inputMatriz);
            for(int j = 0 ; j < 7 ; j++){
                matriz[i][j][1] = inputMatriz[j] - '0';
            }
            fgetc(fd);
        }

        fclose(fd);

        while(vidas > 0){

            printf("### Fase %d ###\n\n", fase + 1);

            printf("    ");
            for(int i = 0; i < 7; i++){
                if(somaColuna[i] > 0){
                    if(somaColuna[i] < 10){
                        printf("0%d ", somaColuna[i]);
                    }
                    else{
                        printf("%2d ", somaColuna[i]);
                    }
                }
                else{
                    printf("   ");
                }
            }
            printf("\n   ______________________\n");

            for(int i = 0; i < 7; i++){
                if(somaLinha[i] > 0){
                    if(somaLinha[i] < 10){
                        printf("0%d |", somaLinha[i]);
                    }
                    else{
                        printf("%2d |", somaLinha[i]);
                    }
                }
                else{
                    printf("   |");
                }
               
            for(int j = 0; j < 7; j++){
                if(matriz[i][j][0] > 0){
                    printf("%d  ",(matriz[i][j][0]));
                }
                else{
                    printf("   ");
                }
            }
            printf("\n");
            }

            printf("\n");
            printf("### Voce tem %d vidas ###\n\n", vidas);
            printf("Digite a linha e a coluna do numero que deseja eliminar: ");
            
            int linha, coluna;

            scanf("%d %d", &linha, &coluna);
            getchar();

            linha--; coluna--;

            if(linha < 0 || linha > 6 || coluna < 0 || coluna > 6){
                clearScreen();
                printf("Posicao invalida!\n\n");
                continue;
            }

            if(matriz[linha][coluna][1] == 1){
                matriz[linha][coluna][1] = 3;
                vidas--;
                clearScreen();
                printf("Voce errou!\n\n");
            }
            else if(matriz[linha][coluna][1] == 0){
                matriz[linha][coluna][0] = 0;
                matriz[linha][coluna][1] = 2;
                clearScreen();
                printf("Numero eliminado com sucesso!\n\n");
            }
            else if(matriz[linha][coluna][1] == 2){
                clearScreen();
                printf("Numero ja eliminado!\n\n");

            }
            else{
                clearScreen();
                printf("Voce ja tentou esse numero!\n\n");
            }

            for(int i = 0; i < 7; i++){
                if((matriz[i][0][1] * matriz[i][1][1] * matriz[i][2][1] * matriz[i][3][1] * matriz[i][4][1] * matriz[i][5][1] * matriz[i][6][1] != 0) && (somaLinha[i] != 0)){
                    somaLinha[i] = -1;
                }
                if((matriz[0][i][1] * matriz[1][i][1] * matriz[2][i][1] * matriz[3][i][1] * matriz[4][i][1] * matriz[5][i][1] * matriz[6][i][1] != 0) && (somaColuna[i] != 0)){
                    somaColuna[i] = -1;
                }
                if(somaColuna[i] == -1){
                    printf("Parabens, voce fechou a coluna %d!\n", i + 1);
                    somaColuna[i] = 0;
                }
                if(somaLinha[i] == -1){
                    printf("Parabens, voce fechou a linha %d!\n", i + 1);
                    somaLinha[i] = 0;
                }
            }

            int flag = 1;

            for(int i = 0; i < 7; i++){
                for(int j = 0; j < 7; j++){
                    flag *= matriz[i][j][1];
                }
            }

            if(flag != 0){
                break;
            }
        }

        
        
        if(vidas > 0){
            clearScreen();
            printf("Parabens! Voce completou a fase %d!\n", fase + 1);
            player[0].pts += 200;
            adicionarRanking();
            fase++;
        }
        else{
            clearScreen();
            printf("Voce perdeu! Tente novamente!\n");
        }
        system("pause");
        clearScreen();
        return;
    
    if(fase == 4){
        printf("Parabens! Voce completou o nivel Avancado!\n");
        printf("Voltando para o menu principal!\n");
        terminou = 1;
        system("pause");
    }

    return;
}

void jogar(){
    clearScreen();

    int matriz[7][7][2];
    int somaLinha[7], somaColuna[7];

    
    switch(dificuldade){
    case 1:
        jogarIniciante(matriz, somaLinha, somaColuna);
        break;
    case 2:
        jogarIntermediario(matriz, somaLinha, somaColuna);
        break;
    case 3:
        jogarAvancado(matriz, somaLinha, somaColuna);
        break;
    }
    

    if(terminou == 1){
        terminou = 0;
        fase = 0;
        dificuldade = 1;
    }

    return;
}

void adicionarRanking(){
    FILE *rankingFile = fopen("ranking.bin", "r+");
    if (rankingFile == NULL) {
        rankingFile = fopen("ranking.bin", "w+");
        int numPlayers = 0;
        fwrite(&numPlayers, sizeof(int), 1, rankingFile);
    }

    int numPlayers;
    fseek(rankingFile, 0, SEEK_SET);
    fread(&numPlayers, sizeof(int), 1, rankingFile);

    DadosPlayer currentPlayer = player[0];
    int found = 0;

    for (int i = 0; i < numPlayers; i++) {
        DadosPlayer tempPlayer;
        fread(&tempPlayer, sizeof(DadosPlayer), 1, rankingFile);
        if (strcmp(tempPlayer.nick, currentPlayer.nick) == 0) {
            tempPlayer.pts = currentPlayer.pts;
            fseek(rankingFile, sizeof(int) + i * sizeof(DadosPlayer), SEEK_SET);
            fwrite(&tempPlayer, sizeof(DadosPlayer), 1, rankingFile);
            found = 1;
            break;
        }
    }

    if (!found) {
        numPlayers++;
        fseek(rankingFile, 0, SEEK_SET);
        fwrite(&numPlayers, sizeof(int), 1, rankingFile);
        fseek(rankingFile, 0, SEEK_END);
        fwrite(&currentPlayer, sizeof(DadosPlayer), 1, rankingFile);
    }

    fclose(rankingFile);
}

void atualizarPontuacao(){
    FILE *rankingFile = fopen("ranking.bin", "r");
    if (rankingFile != NULL) {
        int numPlayers;
        fread(&numPlayers, sizeof(int), 1, rankingFile);

        for (int i = 0; i < numPlayers; i++) {
            DadosPlayer tempPlayer;
            fread(&tempPlayer, sizeof(DadosPlayer), 1, rankingFile);
            if (strcmp(tempPlayer.nick, player[0].nick) == 0) {
                player[0].pts = tempPlayer.pts;
                break;
            }
        }

        fclose(rankingFile);
    } else {
        player[0].pts = 0;
    }
}

void carregaRanking(){
    FILE *rankingFile = fopen("ranking.bin", "r");
    int numPlayers;
    if (rankingFile != NULL) {
        fread(&numPlayers, sizeof(int), 1, rankingFile);

        if (numPlayers == 0) {
            clearScreen();
            printf("Jogue uma partida antes de ver o ranking\n");
            return;
        }

        DadosPlayer players[MAX];
        fseek(rankingFile, sizeof(int), SEEK_SET);
        for (int i = 0; i < numPlayers ; i++) {
            fread(&players[i], sizeof(DadosPlayer), 1, rankingFile);
        }

        qsort(players, numPlayers, sizeof(DadosPlayer), compare);

        printf("\n### RANKING ###\n");
        for (int i = 0; i < numPlayers; i++) {
            printf("%d. %s - %d pontos\n", i + 1, players[i].nick, players[i].pts);
        }
        printf("\n");
    }
    else{
        clearScreen();
        printf("Jogue uma partida antes de ver o ranking\n");
        return;
    }	

    fclose(rankingFile);

    system("pause");
    clearScreen();
}