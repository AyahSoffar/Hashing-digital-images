
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct
{
    int img_arr[784];
} image;
typedef struct
{
    image data;
    int key;
    int flag;
} data_item;
typedef struct
{
    int key;
    image data;
    struct node *next;
} node;
typedef struct
{
    node*head;
    node*tail;
} LinkedList;

int coll=0, dir=0;
data_item hashList[1000];
LinkedList list[1000];
image img;

int hashcode(image img)
{
    int r=1,i;
    int sum=0;
    int t=0;
    for(i=0; i<784; i++)
    {
        if(i%28==0)
        {
            t=t+(sum*r);
            r++;
            sum=0;
        }
        sum=sum+img.img_arr[i];
    }
    return t%1000;
}
void put (int key,image currentimage)
{
    int index;
    index=hashcode(currentimage);
    if(hashList[index].flag==0)
    {
        hashList[index].data=currentimage;
        hashList[index].key=key;
        hashList[index].flag=1;
        // dir++;
    }
    else
    {
        int temp=index+1;
        while(temp!=index)
        {
            if(hashList[temp].flag==0)
            {
                hashList[temp].data=currentimage;
                hashList[temp].key=key;
                hashList[temp].flag=1;
                coll++;
                break;
            }
            else
                temp=(temp+1)%1000;
        }

    }
    dir++;
    // printf("collision: %d and insertion %d\n",coll,dir);
}
void readfile()
{
    //image img;
    int key;
    FILE*bf;
    bf=fopen("files.txt","r");
    if(bf!= NULL)
    {
        while(!feof(bf))
        {
            for(int i=0; i<784; i++)
            {
                fscanf(bf,"%d",&img.img_arr[i]);
            }
            fscanf(bf,"%d",&key);
            // printf("%d",key);
            put(key,img);
        }
        fclose(bf);
    }
    else
        printf("File not found!");
}

int compare(image s,image c)
{
    for(int i=0; i<784; i++)
    {
        if(s.img_arr[i]!=c.img_arr[i])
            return 0;
    }
    return 1;
}
int getL(image img)
{
    int loc=hashcode(img);
    while(hashList[0].flag!=0)
    {
        if(hashList[loc].flag==1)
        {

            if(compare(img,hashList[loc].data)==1)
                //if(hashList[loc].data.img_arr==img.img_arr)
                return hashList[loc].key;
            else
            {
                int temp=loc+1;
                while(temp!=loc)
                {
                    temp++;
                    if(hashList[temp].flag==1)
                    {
                        if(compare(img,hashList[temp].data)==1)
                            // if(hashList[temp].data.img_arr==img.img_arr)
                            return hashList[temp].key;
                    }
                    else
                        return -1;
                }
            }
        }
        else if(hashList[loc].flag==0)
        {
            return -1;
        }
    }
}

int removeL(image img)
{
    int loc=hashcode(img);
    int c=compare(hashList[loc].data,img);
    if(hashList[loc].flag==0)
        return 0;
    else if(c==1)
    {
        int temp=hashList[loc].key;
        hashList[loc].flag=0;
        return temp;
    }
    else
    {
        int n=loc+1;
        while(n!=loc)
        {
            int c2=compare(hashList[n].data,img);
            if(hashList[n].flag==0)
            {
                return 0;
            }
            else if (c2==1)
            {
                int temp=hashList[n].key;
                hashList[n].flag=0;
                return temp;
            }
            else
                n=(n+1)%1000;
        }
    }
    return 0;
}


void init(LinkedList*l)
{
    l->head=l->tail=NULL;
}

node* newNode(image value, int key)
{
    node*n= (node*) malloc(sizeof(node));
    n->key=key;
    n->data=value;
    n->next=NULL;
    return n;

}

void addTail(LinkedList *list,image value,int key)
{
    node *n=newNode(value,key);
    if(list->head==NULL)
        list->head=list->tail=n;
    else
    {
        node *OldTail=list->tail;
        list->tail=n;
        OldTail->next=n;
    }
}
void putSch(image current, int key)
{
    int loc=hashcode(current);
    addTail(&list[loc],current,key);
}
int getSch(image img)
{
    int loc=hashcode(img);
    if(list->head==NULL)
        return -1;
    else if(compare(list->head->data,img)==1)
        return list->head->key;
    else
    {
        //  node *temp=list->head
        list->head=list->head->next;
        //return getSch(image img);
    }
}

int  removeSch(image img)
{
    LinkedList*l;
    int x=hashcode(img);
    node* previous=NULL;
    node* current=l->head;
    while(current!=NULL)
    {
        if(compare(current->data,img)==1)
        {
            if(previous==NULL)
            {
                l->head=current->next;
                if(l->head==NULL)
                {
                    l->tail=NULL;
                }
            }
            else
                previous->next=current->next;
            free(current);
            return current->key;
        }
        previous=current;
        current=current->next;
    }
}
int main()
{

    readfile();
    image im[5];
    int s;
    FILE*f;
    f=fopen("filess.txt","r");
    if(f!= NULL)
    {
        while(!feof(f))
        {
            for(int j=0; j<5; j++)
            {
                for(int i=0; i<784; i++)
                {
                    fscanf(f,"%d ",&im[j].img_arr[i]);
                }
            }
        }

        int choice,z,x,y,w;
        while(1)
        {
            printf("\n\t\t\Images\n");
            printf("\n\n");
            printf("1]Linear Probing\n");
            printf("2]Separate chaining\n");
            printf("Please enter your choice: ");
            scanf("%d",&choice);
            if(choice!=1 && choice!=2)
                printf("Wrong choice , you have to choose one or two");
            switch(choice)
            {
            case 1 :
                printf("\n1]Get image");
                printf("\n2]Remove image");
                printf("\nEnter your choice:");
                scanf("%d",&x);
                if(x==1)
                {
                    for(int k=0; k<5; k++)
                    {
                        s=getL(im[k]);
                        if(s==-1)
                            printf("Not Found\n");
                        else
                        {
                            printf("%d\n",s);
                        }
                    }
                    printf("\nNumber of collisions are %d, pictures inserted in one time equal %d", coll,dir);

                }
                if(x==2)
                {
                    int t,s;
                    printf("Choose the index of element to be deleted:");
                    scanf("%d",&t);
                    if(t>4)
                    {
                        printf("There are only 5 elements! Nothing to be deleted!");
                    }
                    else
                    {
                        int rem;
                        rem=removeL(im[t]);
                        for(int k=0; k<5; k++)
                        {
                            s=getL(im[k]);
                            if(s==-1)
                                printf("Not Found\n");
                            else
                                printf("%d\n",s);

                        }
                    }
                    break;
                }
            case 2:
                printf("\n1]Get image");
                printf("\n2]Remove image");
                printf("\nEnter your choice:");
                scanf("%d",&y);
                if(y==1)

                    if(y==2)

                        break;
            }
        }
        return 0;

    }
}
