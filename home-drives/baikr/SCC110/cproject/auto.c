#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GAP 60

typedef struct act{
    int stage,finHr,finMin,priority,dur,hr,min;
    unsigned char band[70];
}act;
typedef struct joint{
    struct joint *next;
    act act;
}joint;
typedef struct timeSchedule{
    joint*start;
    int slot;
}start;

int countAct(joint* start){
    int count = 0;
    while(start!= NULL){
        start = start->next;
        count++;
    }return count;
}

void static supply(joint **start,act act){
    joint *newJoint = (joint*) malloc(sizeof(joint));
    joint *final= *start;
    newJoint->act=act;

    if(*start==NULL){
        *start=newJoint;
        return;
    }
    while(final->next!=NULL){
        final=final->next;
    }
    final->next=newJoint;
    return;
}
void timeSchedule(joint*start){
    joint*indicate=start;
    joint*breaker=start;
    int slot;
    
    for(int x=0; x<countAct(start); x++){
        for(int y=0; y<countAct(start); y++){
            if((indicate->act.hr==breaker->act.finHr)&&(strcmp(breaker->act.band,indicate->act.band)!=0)
            &&((indicate->act.min< breaker->act.finMin)||(indicate->act.min==breaker->act.finMin))
            &&(breaker->act.priority<=indicate->act.priority)){
                slot=indicate->act.finMin - breaker->act.min;
                indicate->act.min-=(slot+GAP);
                indicate->act.finMin-=(slot+GAP);

                if(indicate->act.min<0){
                    indicate->act.hr--;
                    indicate->act.min=60+indicate->act.min;
                }

                if(indicate->act.finMin<0){
                    indicate->act.finHr--;
                    indicate->act.finMin=60+indicate->act.finMin;
                }
            }
            breaker=breaker->next;
        }
       
        indicate=indicate->next;
        breaker=start;
        
    }
}
void prioritySchedule(joint *start){
    joint*indicate=start;
    act extra;
    while(indicate!=NULL){
        joint *breaker=indicate;
        while(breaker->next!=NULL){
            if(breaker->next->act.priority<breaker->act.priority){
              
                breaker->act=breaker->next->act;
                extra=breaker->act;
                breaker->next->act=extra;
            }
            breaker=breaker->next;
        }
        indicate=indicate->next;
    }
}
void check(joint*start){
    while(start!=NULL){
      
        if(start->act.finMin>=60){
            start->act.finMin-=60;
            start->act.finHr++;
        }
        if(start->act.min>=60){
            start->act.min-=60;
            start->act.hr++;
        }
        start=start->next;
    }
}
int home(){
    printf("Pick among following:\n");
    printf("1)Display all acts\n2)Add an act\n3)Close scheduling program\n");
    int enter;
    scanf("%d",&enter);
    return enter;
}
void createAct(joint** start){
    act* newact=malloc(sizeof(act));
    printf("Input the name of the performing band:\n");
    scanf("%70s",newact->band);

    actTime:printf("What time do you want the act to be held?\n(Enter in 24 hour format and press 2 to continue adding):\n");
    scanf("%d",&newact->min);
    while((getchar())!='\n')
    scanf("%d",&newact->hr);
    while((getchar())!='\n');

    if((newact->hr==0||newact->min==0||(newact->hr<0||newact->hr>24||newact->min<0||newact->min>24))){
        printf("Entered time is not valid, please re-enter(in 24hours format\n");
        goto actTime;
    }

    actDuration:printf("How long do you want the act be held?(Enter in minute format)\n");
    scanf("%d",&newact->dur);
    while((getchar())!='\n');
    if(newact->dur<=0){
        printf("Entered duration is not valid, please re-enter(in minute format)\n");
        goto actDuration;
    }
    else if((newact->min+newact->dur)>60){
    newact->finMin=(newact->min+newact->dur)-60;
    newact->finHr=newact->hr+1;
    }
     
    else{
    newact->finMin=newact->min+newact->dur;
    newact->finHr=newact->hr;
    }

    actPriority:printf("What is the priority of this act? Choose from 1 to 3(1 being the most important, 3 being the least\n");
    scanf("%d",&newact->priority);
    while((getchar())!='\n');
    if(newact->priority==0||newact->priorit5y<1||newact->priority>3){
        printf("Entered priority is not valid, please re-enter(1-3)\n");
        goto actPriority;
    }
    supply(start, *newact);

    actStage:printf("which stage do you prefer? (Please choose from 1 to 3)\n");
    scanf("%d",&newact->stage);
    while((getchar())!='\n');
    if(newact->stage==0||newact->stage<1||newact->stage>3){
        printf("Entered stage number is not valid, please re-enter(1-3)\n");
        goto actStage;
    }
}

int main(){
    int enter;
    int actScheduler=0;

    joint*start=(joint*) malloc(sizeof(joint));
    start=NULL;
    printf("Automatic Event Scheduler\n");
    startingPage: enter=home();
    if(enter==1){
        if(start==NULL){
            printf("There is no data, would you like to add an act?\n");
            goto startingPage;
        }
        
        timeSchedule(start);
        timeSchedule(start);
        prioritySchedule(start);
        prioritySchedule(start);
        check(start);

        
        joint*breaker=(joint*)malloc(sizeof(joint));
        breaker=start;
        while(breaker!=NULL){
            printf("\nPerforming Band:%s\nPriority:%d\nTime:%d:%d\nFinished Hr:%d:%d\n", breaker->act.band, breaker->act.priority,
            breaker->act.hr,breaker->act.min,breaker->act.finHr,breaker->act.finMin);
            breaker=breaker->next;
        }
        free(breaker);
        goto startingPage;
    }else if(enter==2){
        createAct(&start);
        goto startingPage;
    }
}