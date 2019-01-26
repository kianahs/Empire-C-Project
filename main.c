#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <math.h>

int people=50 , court=50 , treasury=50 , listcounter=0 , condition=1;

struct node{

    char problem[200];
    char decision1[200];
    int t1people;
    int t1court;
    int t1treasury;
    char decision2[200];
    int t2people;
    int t2court;
    int t2treasury;
    int probability;
    struct node * next;

};

struct savegame{

    char bname[20];
    int bcondition;
    int bprobablitiy[200];
    int bpeople;
    int bcourt;
    int btreasury;

};

struct user{

    char name[20];
    double average;
    int apeople;
    int acourt;
    int atreasury;

};

void exitmenu(char name[20] , struct node * list){

    int choice;
    printf("1)save & exit\n2)exit\n");
    scanf("%d",&choice);

    if (choice==2)
        exit(-1);

    else if (choice==1){
       save(name,list);
       best_users(name);
       exit(-1);
    }

    else{
        printf("Unknown input");
        exit(-1);
    }

}

void show(){

    printf("\n1)New game\n");
    printf("2)Resume game\n");
    printf("3)Exit\n");
    printf("4)Best users up to now\n");

}

void deleteHead(struct node **head){

    struct node *temp = *head;
    temp = temp->next;
    *head = temp;

}

void delete_node(struct node * list){

    struct node * head;
    struct node * current=list;
    struct node * end=list;

    for (end=list ; end->next!=NULL ; end=end->next);

        if (list->probability==-1){

            deleteHead(&list);
            listcounter++;
    }

    else if (end->probability==-1){

        while (current->next->next!=NULL)

            current = current->next;
            free(current->next);
            current->next = NULL;
            listcounter++;

    }

    else{

        for (head=list->next ; head!=NULL ; head=head->next , current=current->next){

            if (head->probability==-1){

                current->next=head->next;
                free(head);
                listcounter++;
                return ;

        }
      }
    }

    return;

}

void add_front(struct node *new_node , struct node **list){

    new_node->next = *list;
    *list = new_node;

}

void add_end(struct node *new_node , struct node *list){

    struct node *current;

    for (current = list ; current->next!=NULL ; current=current->next);

        current->next = new_node;
        new_node->next = NULL;

}

void file(char *filename , FILE * fp , char * problem , char * decision1 , int * t1people , int * t1court , int * t1treasury , char * decision2 , int * t2people , int * t2court , int * t2treasury , int * probability ){

    char * c, ignore;
    int save;
    fp=fopen(filename,"r");

    if (fp==NULL)
        return -1;

    c=fgets(problem,200,fp);

    if (c==NULL)
        return -1;

    c=fgets(decision1,200,fp);

    if (c==NULL)
        return -1;

    fscanf(fp,"%d",&save);
    *t1people=save;
    fscanf(fp,"%d",&save);
    *t1court=save;
    fscanf(fp,"%d",&save);
    *t1treasury=save;
    ignore=fgetc(fp);
    c=fgets(decision2,200,fp);

    if (c==NULL)
        return -1;

    fscanf(fp,"%d",&save);
    *t2people=save;
    fscanf(fp,"%d",&save);
    *t2court=save;
    fscanf(fp,"%d",&save);
    *t2treasury=save;
    *probability=3;
    fclose(fp);

}

int filecounter(){

    int count=0;
    FILE * fp;
    char len[200];
    fp=fopen("CHOICES.txt","r");

    if (fp==NULL)
        return -1;

    while (fgets(len,200,fp)!=NULL)
        count++;

    fclose(fp);
    return count;

}

int creat_random(int file_num){

    srand(time(NULL));
    int random=rand()%(file_num+1);

    if (random==0)
        random=rand()%(file_num+1);

    return random;

}

int print_decisions(struct node * list , int random , char name[20]){

    struct node * current=list;

    for (int i=1 ; i<random ; i++){
        current=current->next;

        if (current==NULL)
            return 2;

    }

    (current->probability)--;

    if ((current->probability)<-1)
        return ;

    if ((current->probability)==-1 ){
        delete_node(list);
        return ;

    }

    printf("people=%d court=%d treasury=%d\n\n",people,court,treasury);
    Sleep(300);
    printf("%s",current->problem);
    printf("\n");
    printf("1)");
    printf("%s",current->decision1);
    printf("\n");
    printf("2)");
    printf("%s",current->decision2);
    printf("\n");
    printf("please choose one item\n");
    int choice;
    scanf("%d",&choice);
    system("cls");

    if (choice==1){
        people+=current->t1people;
        court+=current->t1court;
        treasury+=current->t1treasury;

    }

    else if (choice==2){
        people+=current->t2people;
        court+=current->t2court;
        treasury+=current->t2treasury;

    }

    else if (choice==3){
      exitmenu(name,list);

    }

    else{
        printf("Unknown input");
        exit(-1);
    }

    double average=(people+court+treasury)/3;

    if ((people<=0) || (court<=0) || (treasury<=0) || (average<10)){
        printf("You loose\n");
        condition=0;
        exitmenu(name,list);
    }

    return 0;
}

void getfilename(int file_num , char array[file_num*2][7]){

    FILE * fp;
    char * c;
    fp=fopen("CHOICES.txt","r");

    for (int i=0 ; i<file_num*2 ; i++){
       c=fgets(array[i],7,fp);

       if (c==NULL){
            return;

       }

    }
}

void initialize(char array[200]){

    for (int i=0 ; i<200 ; i++)
        array[i]='*';

}

struct node * creatlinkedlist(){

    struct node * list=NULL;
    FILE* fp;
    int t1people , t1court , t1treasury , t2people , t2court , t2treasury , probability , file_num ;
    char problem[200] , decision1[200] , decision2[200] , name[20] ;
    initialize(problem);
    initialize(decision1);
    initialize(decision2);
    file_num=filecounter();
    char filename[file_num*2][7];
    getfilename(file_num , filename );

    for (int i=0 ; i<(2*file_num) ; i=i+2){
        file(filename[i],fp,problem,decision1,&t1people,&t1court,&t1treasury,decision2,&t2people,&t2court,&t2treasury,&probability);
        struct node *new_node = (struct node*)malloc(sizeof(struct node));

        if (new_node == NULL){
            printf("Cannot create node\n");
            exit(-1);
        }
        strcpy(new_node->problem,problem);
        strcpy(new_node->decision1,decision1);
        new_node->t1people=t1people;
        new_node->t1court=t1court;
        new_node->t1treasury=t1treasury;
        strcpy(new_node->decision2,decision2);
        new_node->t2people=t2people;
        new_node->t2court=t2court;
        new_node->t2treasury=t2treasury;
        new_node->probability=probability;
        new_node->next=NULL;

        if (list == NULL)
            add_front(new_node, &(list));
        else
            add_end(new_node, list);

    }
    return list;
}


void save(char name[20] , struct node * list){

    struct node * current;
    struct savegame st1;
    int i=0;
    strcpy(st1.bname,name);
    st1.bpeople=people;
    st1.bcourt=court;
    st1.btreasury=treasury;
    st1.bcondition=condition;

    for (current=list ; current->next!=NULL ; current=current->next){
        st1.bprobablitiy[i]=current->probability;
        i++;
    }

    FILE * fp;
    fp=fopen("save.bin","wb");

    if (fp == NULL){
        printf("Cannot create file\n");
        return -1;
    }

    fwrite(&st1,sizeof(st1),1,fp);

    fclose(fp);

}


void choose(char name[20]){

    int  random , file_num , choice;
    struct node * list=NULL;
    file_num=filecounter();
    scanf("%d",&choice);

    if (choice==1){
        system("cls");
        list=creatlinkedlist();

        while (1){
            random=creat_random(file_num);
            print_decisions(list,random,name);
            Sleep(500);

            if (listcounter==file_num){
                listcounter=0;
                list=NULL;
                list=creatlinkedlist();

      }
     }
    }

   else if (choice==2){
        system("cls");
        FILE * fp;
        struct savegame s1;
        fp=fopen("save.bin","rb");

        if (fp == NULL){
            printf("There is no saved game\n");
            exit(-1);
        }

        int c=fread(&s1,sizeof(s1),1,fp);

        if (s1.bcondition==0){
            printf("people=%d court=%d treasury=%d\n\n",people,court,treasury);
            list=creatlinkedlist();

            while (1){
                random=creat_random(file_num);
                print_decisions(list,random,name);
                Sleep(500);

                if (listcounter==file_num){
                    listcounter=0;
                    list=NULL;
                    list=creatlinkedlist();

                }
            }
        }

        if (s1.bcondition==1){
            people=s1.bpeople;
            court=s1.bcourt;
            treasury=s1.btreasury;
            list=creatlinkedlist();
            struct node * current=list;

            for (int i=0 ; i<file_num ; i++){
                current->probability=s1.bprobablitiy[i];
                current=current->next;
            }

            while (1){
                random=creat_random(file_num);
                print_decisions(list,random,name);
                Sleep(500);

                if (listcounter==file_num){
                    listcounter=0;
                    list=NULL;
                    list=creatlinkedlist();

                }
            }

        }
   }

    else if (choice==3)
        exit(-1);

    else if (choice==4){
        show_users();
        exit(-1);
    }

    else{
        printf("Unknown Input");
        exit(-1);
    }
}

void best_users(char name[20]){

    struct user u1;
    strcpy(u1.name,name);
    u1.average=(people+treasury+court)/(double)3;
    u1.acourt=court;
    u1.apeople=people;
    u1.atreasury=treasury;
    FILE * fp;
    fp=fopen("user.bin","ab");

    if (fp==NULL){
        printf("Cannot creat file\n");
        exit(-1);
    }

    fwrite(&u1,sizeof(u1),1,fp);
    fclose(fp);

}

void show_users(){

    FILE * fp;
    fp=fopen("user.bin","rb");

    if (fp==NULL){
        printf("Cannot open file\n");
        exit(-1);
    }

    int k=0;
    struct user u1;
    struct user array[50];

    for (int i=0 ; i<50 ; i++){
       int c=fread(&u1,sizeof(u1),1,fp);

            if (c!=NULL){
                array[k]=u1;
                k++;
        }
    }

    for (int i=0 ; i<k-1 ; i++){

        for (int j=i+1 ; j<k ; j++){

            if (array[i].average < array[j].average)
                swap(&array[i],&array[j]);
        }
    }

    printf("Basic Comparsion\n\n");

    for (int i=0 ; i<fmin(10,k) ; i++)
        printf("name=%s    average=%f\n",array[i].name,array[i].average);

        printf("\n");

    for (int i=0 ; i<k-1 ; i++){

        for (int j=i+1 ; j<k ; j++){

            if (array[i].apeople < array[j].apeople){
                swap(&array[i],&array[j]);

            }

             else if (array[i].apeople == array[j].apeople){

                if (array[i].acourt < array[j].acourt)
                    swap(&array[i],&array[j]);

                 else if (array[i].acourt == array[j].acourt) {

                    if (array[i].atreasury < array[j].atreasury)
                        swap(&array[i],&array[j]);
                 }
               }
            }
        }

    printf("Advanced Comparsion\n\n");

    for (int i=0 ; i<fmin(10,k) ; i++)
        printf("name=%s    people=%d   court=%d  treasury=%d\n",array[i].name,array[i].apeople,array[i].acourt,array[i].atreasury);


    fclose(fp);

}

void swap(struct user *u1 , struct user * u2){

    struct user temp;
    temp=*u1;
    *u1=*u2;
    *u2=temp;

}

int main()
{
    char  name[20] ;
    printf("Please enter your name\n");
    scanf("%s",name);
    printf("Welcome %s,select one of these options\nBy pressing 3 you can exit from game and save it\n\n ",name);
    show();
    choose(name);
    return 0;
}
