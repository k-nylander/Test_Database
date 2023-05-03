#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
/*
Cool ideas:
- Safe mode(Maybe ain't good in C)
- Multiple Tags(Char matrix)
*/
// tableView()
#define PRINT_ALL 0,-1
#define END_ARG -1

#define fPRINTTEST 1
#define fPRINTSOLVED 2
#define fPRINTLINE 3
#define fPRINTLINE 3


typedef struct{//Question structure
    char statement[2048];
    char answer[2048];
    char tag[32];
}question;

// --- Global variables (made that way to be easier to build Safe Mode)
FILE *fp;
char fileName[64] = "";
char smallBuff[8]; //I think it's some kind of "System Vulnerability" but in this program it ain't relevant.
char bigBuff[8192];

// Usefull functions
void stdinClear(){
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
    // Ps. Search why u need to press Space
}

void consoleClear(){
    printf("\e[1;1H\e[2J");
}

bool cmplist(int compare, int init, va_list args){
    int argn;
    if(compare == init)
        return true;
    while((argn = va_arg(args, int)) != -1){
        if(compare == argn)
            return true;
    }
    return false;
}

void find(char *strfont, question *dest){ // Finds and separe
    /* This way used to had a bug but a don't know why, so i made it diffrent.
    char *bar = strchr(strfont, '|');
    bar++;
    char *bar2 = strrchr(strfont,'|');
    bar2++;
    strcpy(dest->tag, bar2);
    strncpy(dest->answer, bar, strlen(bar) - strlen(bar2) - 1);
    strncpy(dest->statement, strfont, strlen(strfont) - (strlen(bar) + 2));
    */
    
    if(strcmp(strfont, "\n") == 0 || strcmp(strfont, "\0") == 0)
        return;
    int walkfont, walkdest = 0, step = 0;
    for(walkfont = 0; strfont[walkfont] != '\0' || strfont[walkfont] != -1; walkfont++){ // Walks by the buffer.
        if(strfont[walkfont] != '|' && strfont[walkfont] != '\n'){
            switch(step){
            case 0:
                dest->statement[walkdest++] = strfont[walkfont];
                break;
            case 1:
                dest->answer[walkdest++] = strfont[walkfont];
                break;
            case 2:
                dest->tag[walkdest++] = strfont[walkfont];
                break;
            }
        }else{
            switch (step){
                case 0:
                    dest->statement[walkdest] = '\0';
                    break;
                case 1:
                    dest->answer[walkdest] = '\0';
                    break;
                case 2:
                    dest->tag[walkdest++] = '\n';
                    dest->tag[walkdest] = '\0';
                    return;
                    break;
            }  
            walkdest = 0;
            step++;
        }
    }
}

void tableView(int id, ...){
    /*The question disposing in the file must be one per line because it makes all a lot easier 
    to work with since fgets reads a whole line. This saves me time but cost some limitations
    to the app*/

    //Var declaration
    int i = 1;
    question aux;
    va_list args, args2;
    va_start(args, id);
    //Prepare the space

    consoleClear();
    printf("\n⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇ -- THE TABLE -- ⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇⬇\n\n");

    // Reads all the file characters one by one, searching for the reserved ones. 
    fp = fopen(fileName, "r");
    while(fgets(bigBuff, 8191, fp) != NULL && strcmp(bigBuff,"\n") != 0){
        va_copy(args2,args);
        if(cmplist(i++, id, args2) == false){
            find(bigBuff,&aux);
            printf("%d-) %s\n>>> %s\nTag: %s\n",i-1 , aux.statement, aux.answer, aux.tag);
        }
    }
    fclose(fp);
}

void default_alert(int alert_type, int alert_code){ // Function to save some code lines
    if(alert_type == 0){
        switch (alert_code){
        case 0:
            consoleClear();
            printf("✔ Action concluded successfully\n");
            break;
        case 1:
            printf("🧩 Tip: The file must be in the same folder as the program and the file type has to be '.tdb'.\nFile name: ");
            break;        
        case 2:
            printf("1. Try again\n2. Create file\n9. Exit\n> ");
            break;        
        }
        return;
    }
    if(alert_type == 1){    
        consoleClear();
        switch (alert_code){
        case 0:
            printf("❌ Invalid answer\n");
            stdinClear();
            break;
        case 1:
            printf("❌ File is missing!\n\n");
            break;
        case 2:
            printf("❌ Invalid file type! \n\nTry again...\n");
            break;
        case 3:
            printf("❌ Unknown problem! \n❌ Exiting program...\n");
            break;
        case 4:
            printf("❌ You need to chose one of the options\n\n");
            break;
        case 5:
            printf("❌ Conflicting files!! The app can't proceed...\n🧩 You must move the old 'Test.txt' and 'Results.txt' from the directory.\n");
            break;
        }
        return;
    }
}

int lnCount(){ // Count how much questions the file has
    int aux = 0;
    fp = fopen(fileName, "r");
    while(fgets(bigBuff, 8191, fp)!=NULL){
        if(strcmp(bigBuff,"\n") == 0)
            return aux;
        aux++;
    }
    fclose(fp);
    return aux;
}

char *qLine(int id){
    FILE *fpAux = fopen(fileName,"r+");
    int count = 1, max = lnCount();
    if(id < 1 || id > max)
        return "";
    rewind(fpAux);
    fseek(fpAux,0,SEEK_SET);
    do{
        fgets(bigBuff,8191, fpAux);
    }while(count++ != id);
    fclose(fpAux);
    return bigBuff;
}

int selectId(char *inputName){ // Read user input and return the id to use
    int max = lnCount(), id = 0;
    do{
        tableView(PRINT_ALL);
        printf("\nId to %s: ", inputName);
        if(sscanf(fgets(smallBuff, 7, stdin), "%d", &id) == 0)
            default_alert(1,0);
        else if(id < 1 || id > max)
            default_alert(1,4);
        else
            break;            
    }while(true);
    return id;
}

void convert(question *dest){ // Gets a struct and turns into wanted form
    char *aux = "";
    for(int k = 0; k < 2; k++){
        aux = strchr((k == 0 ? dest->statement : dest->answer),'\n');
        *aux = '|';
    }
}

void fprintId(FILE *fpDest, int id, int questionNumber, int paramether, int lnParamether){
    fp = fopen(fileName,"r");
    question aux;
    find(qLine(id), &aux);
    switch (paramether){
    case 1:
        fprintf(fpDest,"\n%d-) %s\n", questionNumber, aux.statement);
        for(int j = 0; j < 4; j++)
            if(lnParamether == fPRINTLINE)
                fprintf(fpDest,"_______________________________________________________________\n");
            else
                fprintf(fpDest,"\n");

        break;
    default:
        fprintf(fpDest,"\n%d-) %s\n>>> %s\n", questionNumber, aux.statement, aux.answer);
        break;
    }
}

void ajustFile(){
    fp = fopen(fileName,"r");
    FILE *temp = fopen("temp","w");
    char c, last;
    while((c = fgetc(fp)) == '\n');
    fputc(c,temp);
    while((c = fgetc(fp)) != '\0' && c != EOF){
        if(last == '\n'){
            if (c == '\n')
                continue;
            else
                last = c;
        }
        if(c == '\n'){
                last = c;
        }
        fputc(c,temp);
    }
    remove(fileName);
    rename("temp", fileName);
    fclose(temp);
    fclose(fp);
}

bool verifyFile(){ // Verifies the file existence 
    int answer = 0;

    if((fp = fopen(fileName,"r+")) == NULL){ // Verify file existence.
        default_alert(1,1);
        default_alert(0,2);
        do{ // Force the user to give a spected answer.
            if (sscanf(fgets(smallBuff, 7, stdin),"%d", &answer) != 1){ // Verify user choice.
                default_alert(1,0);
                default_alert(0,2);
                continue;
                }
            switch (answer){ //
            case 1: // Returns false to restart the process.
                return false;
            break;
            case 2: // Creates a new file.
                fp = fopen(fileName, "w");
                fclose(fp);
                return true;
                break;
            case 9: // Exit the program.
                exit(1);
                break;
            default: // Wrong answer, but correct format.
                default_alert(1,4);
                default_alert(0,2);
                break;
        }
        }while(true);
    }
    else{
        fclose(fp);
        return true;
    }
}   

//"Safe mode": File should be binary, making harder to read without the correct parameters(Has a lot to do)
/*
void configFile(){
    // Variables
    bool flag;
    char buffer[16];
    char backup[sizeof(cfg_parameter)];
    // Chekcing for an existing configuration
    if(fgetc(fp) == 'V'){
        fread(cfg_parameter, sizeof(cfg_parameter), 1, fp);
        // Backup desc.
        // Char bkp = "Vpat|patPAT"
        for(int i = 0; i < 3; i++){
            if(cfg_parameter[i] <= 0){
                rewind(fp);
                while(fgetc(fp) != '|');
                fseek(fp, i*sizeof(short int), SEEK_CUR);
                if(fread(&cfg_parameter[i], sizeof(short int), 1, fp) == 0 || cfg_parameter[i] < 0){
                    printf("❌ Failed to read backup.\n❌ The file is corrupted...\n");
                    exit(1);
                }
            }
        }
    }else{ // Configurating a new file
        for(int j = 0; j < 3; j++){
            do{
                printf("Insert the %s: ", j == 0 ? "statement lenght(>=10)" : (j == 1 ? "answer lenght(>=10)" : "max number of tags(<=5)"));
                if(sscanf(fgets(buffer, 15, stdin), "%hd", &cfg_parameter[j]) == 0 || cfg_parameter[j] <= 0 || (j < 2 ? cfg_parameter[j] < 10 : cfg_parameter[j] > 5))
                    default_alert(1,0);
                else
                    break;
            }while(true);
        }
        rewind(fp);
        fputs();
        fwrite(cfg_parameter, sizeof(short int), 3, fp);
        fwrite(cfg_parameter, sizeof(short int), 3, fp);
        return;
    }
}
*/

//Project functions
void qInsert(){ // Insert a new question to the file
    // Var. declaration section
    question *insert = malloc(sizeof(question));
    int edit = 0;
    bool flag = true;
    // stdinClear();
    for(int i = 0; (i < 3 && flag) || edit != 0; i ++){ // Might be a better way to make it work.
        consoleClear();
        printf("%s:", (i == 0 ? "Statement" : (i == 1 ? "Answer" : "Tag"))); // Just a way to write everything in just one line.
        fgets((i == 0 ? insert->statement : ( i == 1 ? insert->answer : insert->tag)), (i < 2 ? 2047 : 31), stdin); // Same as above.
        if(i == 2 || edit != 0){ // Verifies if it's the last step or if it was an edition.
            consoleClear();
            do{
                printf("%sA: %sTag: %s\nDo you want to edit something?\n1. Statement\n2. Answer\n3. Tags\n\n9. Continue\n> ", insert->statement, insert->answer, insert->tag);
                if(sscanf(fgets(smallBuff, 7, stdin), "%d\n", &edit) == 0){
                    flag = true;
                    default_alert(1,0);
                }else{
                    flag = false;
                    if(edit > 0 && edit < 4) // Sets the edit.
                        i = edit - 2;
                    else if(edit == 9) // Exit.
                        edit = 0;
                    else{ // Wrong choice, right format. 
                        default_alert(1,4);
                        flag = true;
                    }
                }
            }while(flag);
        }
    }
    // Format and insert the string into the file.
    convert(insert);
    fp = fopen(fileName, "a+");
    fseek(fp, -1, SEEK_END);
    if(fgetc(fp)!='\n')
        fputc('\n',fp);
    fprintf(fp, "%s%s%s", insert->statement, insert->answer, insert->tag);
    fclose(fp);
    free(insert);
    default_alert(0,0);
    return;
}

void qDelete(){ // Delete a question from the file
    // Var declaration
    int count = 1, id = selectId("delete");
    fp = fopen(fileName, "r");
    FILE *fpDel = fopen("temp", "w");
    while (fgets(bigBuff, 8191, fp) != NULL){
        if(count++ != id)
            fprintf(fpDel,"%s", bigBuff);
    }
    remove(fileName);
    rename("temp", fileName);
    fclose(fpDel);
    fclose(fp);
    default_alert(0,0);
}

void qEdit(){ // Edit a question from the file
    int id = selectId("edit");

    FILE *fpEdited = fopen("temp","w");

    for(int i = 1; i <= lnCount(); i++ ){
        if(i == id){
            question aux;
            // Formats the line into the right format to process
            find(qLine(i), &aux);
            strcat(aux.statement,"\n"); // I had to do this to fits
            strcat(aux.answer,"\n");    // into convert().
            strcat(aux.tag,"\n");
            consoleClear();
            do{
                printf("Select what you want to change:\n1. Statement: %s2. Answer: %s3. Tag: %s\n9. Back to menu\n> ", aux.statement, aux.answer, aux.tag);
                if(sscanf(fgets(smallBuff, 7, stdin), "%d", &id) == 0){
                    default_alert(1,0);
                    continue;
                }if(id > 0 && id < 4)
                    break;
                if(id == 9){
                    consoleClear();
                    return;
                }
                default_alert(1,4);
            }while(true);
            printf("\nOld: %sNew: ",(id == 1 ? aux.statement : (id == 2 ? aux.answer : aux.tag)));
            fgets((id == 1 ? aux.statement : ( id == 2 ? aux.answer : aux.tag)), (id < 2 ? 2047 : 31), stdin);
            convert(&aux);
            fprintf(fpEdited, "%s%s%s", aux.statement, aux.answer, aux.tag);
        }else{
            fprintf(fpEdited,"%s",qLine(i));
        }
    }
    // Manage the files
    remove(fileName);
    rename("temp", fileName);
    // Finishes
    fclose(fpEdited);
    default_alert(0,0);
}

void qFilter(){ // Shows every question that has a determined tag(i'll make with multiple tags later)
    /* I made this function before*/
    
    question aux;
    bool flag = false;
    char findtag[sizeof(aux.tag)];

    consoleClear();
    printf("Tag to filter:");
    scanf("%s", findtag);
    strcat(findtag,"\n\0");
    for(int i = 1; i <= lnCount(); i++){
        find(qLine(i),&aux);
        if(strcmp(aux.tag, findtag) == 0){
            flag = true;
            printf("\n%d-) %s\n%s\n", i, aux.statement, aux.answer);
        }
    }
    if(flag == false){
        printf("❌ Impossible to find this tag\n");
        stdinClear();
    }
}

void buildFile(){ // Makes the magic happen
    // Var section
    int mode, shuffle;
    bool flag = false;
    
    // User input section: Chosing test type.
    consoleClear();
    do{
        printf("The test might have:\n1. All the questions.\n2. A number of questions.\n3. Questions with similar tags.\n9. Back to menu.\n");
        if(sscanf(fgets(smallBuff, 7, stdin), "%d", &mode) == 0){ // Reads the  selected
            default_alert(1,0);
            continue;
        }if((mode < 1 || mode > 3) && mode != 9)
            default_alert(1,4);
    }while((mode < 1 || mode > 3) && mode != 9);
    if(mode == 9)
        return;
    
    consoleClear();
    do{
        printf("The questions will be:\n1. Normally ordered.\n2. Shuffled.\n9. Back to menu.\n");
        if(sscanf(fgets(smallBuff, 7, stdin), "%d", &shuffle) == 0){ // Reads the  selected
            default_alert(1,0);
            continue;
        }if(shuffle != 1 && shuffle != 2 && shuffle != 9)
            default_alert(1,4);
    }while((shuffle < 1 || shuffle > 2) && shuffle != 9);
    if(shuffle == 2)
        flag = true;
    if(shuffle == 9)
        return;
    
    FILE *fpTest, *fpResult;
    if(((fpTest = fopen("Test.txt", "r")) != NULL)){
        default_alert(1,5);
        fclose(fpTest);
        return;
    }

    if(((fpResult = fopen("Result.txt", "r")) != NULL)){
        default_alert(1,5);
        fclose(fpResult);
        return;
    }

    int order[lnCount()];
    int step = 0;
    switch (mode){
    case 1:
        for (step = 0; step < lnCount(); step++)
            order[step] = step+1;
        break;
    case 2:
        tableView(PRINT_ALL);
        printf("IDs (To finish, press Enter in a empty line):\n");
        step = -1;
        while(step < lnCount() && sscanf(fgets(bigBuff, 8191, stdin),"%d\n",&order[++step]) != 0 && strcmp(bigBuff,"\n") != 0){
            if(order[step] < 1 || order[step] > lnCount()){
                tableView(PRINT_ALL);
                default_alert(1,4);
                printf("Starting again...(To finish, press Enter in a empty line):\n");
                step = -1;
            }
        }
        break;
    case 3:
        question aux;
        char findtag[sizeof(aux.tag)];
        consoleClear();
        printf("\nThe questions might have tag:");
        scanf("%s", findtag);
        stdinClear();
        strcat(findtag,"\n\0");
        for(int h = 1; h <= lnCount(); h++){
            find(qLine(h),&aux);
            if(strcmp(aux.tag, findtag) == 0){
                order[step++] = h;
            }
        }
        break;
    }
    int qtd = 0;
    consoleClear();
    if(step > 1 && flag)
        while(qtd < 1 || qtd > step){  
            printf("Number of diferent tests(MAX = %d): ", step*(step-1));
            if(sscanf(fgets(smallBuff, 7, stdin), "%d", &qtd) == 0)
                default_alert(1,0);
        }
    else
        qtd = 1;
    char itoa[3];
    for(int i = 0; i < qtd; i++){
        char testName[16] = "Test";
        sprintf(itoa, "%d", i+1);
        strcat(testName,itoa);
        strcat(testName,".txt\0");
        fpTest = fopen(testName, "w");
        if(flag){
            srand(time(NULL));
            int nRand = 0;
            for(int j = 0; j < step; j++){
                nRand = rand()%(step - 1);
                if(nRand == j){
                    j--;
                    continue;
                }
                // order[j] troca com order[nRand];
                order[nRand] += order[j];
                order[j] = order[nRand] - order[j];
                order[nRand] -= order[j];
            }
        }
        for (int k = 0; k < step; k++)
            fprintId(fpTest,order[k],k+1,fPRINTTEST, 0);
        fclose(fpTest);
    }
    fpResult = fopen("Result.txt", "w");
    for (int l = 0; l < step; l++)
        fprintId(fpResult, order[l], l+1, fPRINTSOLVED, 0);
    fclose(fpResult);
    default_alert(0,0);
}

int main(){
    int aux;
    bool flag = false;
    
    do{ // Forces a valid filename.
        default_alert(0,1);
        scanf("%63s", fileName);
        stdinClear();
        if(strstr(fileName,".tdb") != NULL && strchr(fileName , '.') == strrchr(fileName , '.')) // Verify is has one, and only one, dot in filename and if it's the right type.
            flag = verifyFile();
        else
            default_alert(1,2);
    }while(flag == false);
    consoleClear();
    ajustFile();
    printf("✔ File found\n");
    do{
        printf("\n1. Insert\n2. Edit\n3. Delete\n4. Visualize\n5. Filter tags\n6. Build test\n\n9. Exit\n>");
        if(sscanf(fgets(smallBuff, 7, stdin), "%d", &aux) == 0){ // Gets the action selected
            default_alert(1,0);
            continue;
        }
        switch (aux){
        case 1:
            qInsert();
            break;
        case 2:
            qEdit();
            break;
        case 3:
            qDelete();
            break;
        case 4:
            tableView(PRINT_ALL);
            printf("%d", lnCount());
            break;
        case 5:
            qFilter();
            stdinClear();
            break;
        case 6:
            buildFile();
            break;
        case 9:
            return 0;
            break;
        default: // Right format, wrong choice
            default_alert(1,4);
            break;
        }
    }while(true);
}