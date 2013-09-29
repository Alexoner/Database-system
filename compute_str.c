#include "database.h"
#include "str.h"
int select_find_type(char *table_name,char *column_name)
{
    Table *t_head=NULL;
    t_head=current_db->head_table;
    while(t_head)
    {
        if(strcmp(t_head->table_name,table_name)==0)
        {
            Column *c_head=t_head->head_column;
            while(c_head)
            {
                if(strcmp(c_head->column_name,column_name)==0)
                {
                    if(c_head->datatype==INT)return INT;
                    if(c_head->datatype==FLOAT)return FLOAT;
                    if(c_head->datatype==TEXT)return TEXT;
                }
                c_head=c_head->next;
            }
        }
        t_head=t_head->next;
    }
    return -1;
}
//��right_value��Ҫ������λ�����пո񣬵�һ���ַ�ֻ�����������'('����'['����'\''
//�����ҵ�right_value����ֻ�������ָ�ʽ����()��ͷ��β��selectǶ�ף����ԡ�����ͷ��β��������ֵ������[]��ͷ��β��like�ͣ�
//������ֵ�����������ͣ�һ����selectǶ�ף�һ����like�͵ģ�һ������������
nums *compute_str(struct column *pc,char *optr,char *right_value)
{//printf("pc=%s\toptr='%s'\tright=%s\n",pc->column_name,optr,right_value);
//printf("\nd=%s\tt=%s\tc=%s\tt=%d\tv=%s\n\n",pc->db_name,pc->table_name,pc->column_name,pc->datatype,pc->head_value->data);
    //��selectǶ�����Ļ�������select�е�column�Ƿ���������֪��pc������ƥ��
        //char *right_value_copy=(char *)malloc(strlen(right_value)+1);
        //memset(right_value_copy,'\0',sizeof(right_value_copy));
        //strcpy(right_value_copy,right_value);
        //if(right_value[0]=='(')
            //right_value_copy=str_bracket(right_value);//ȥ�����ţ��ó���������Ķ���
//printf("right_copy==%s\n",right_value_copy);
        //strs *command=NULL;
        //command=str_break(right_value_copy);
        //strs *cmd_p=command;
    if(right_value[0]=='(')//����select���
    {
        char *right_value_copy=(char *)malloc(strlen(right_value)+1);
        memset(right_value_copy,'\0',sizeof(right_value_copy));
        right_value_copy=str_bracket(right_value);
        strs *command=NULL;//printf("right_copy==%s\n",right_value_copy);
        command=str_break(right_value_copy);//ȥ�����ţ��ó���������Ķ���
        strs *cmd_p=command;
        if(strcmp(cmd_p->data,"select")!=0)return NULL;
        if(cmd_p->next==NULL)return NULL;
        char *column_name=cmd_p->next->data;
        int check_i=0;
        for(check_i=0;column_name[check_i]!='\0';check_i++)//��ʱ����ڶ���������һ�����ʣ������ж�����ʣ�������*
            if(!((column_name[check_i]>='a' && column_name[check_i]<='z') || (column_name[check_i]>='A' && column_name[check_i]<='Z') || column_name[check_i]=='_'))return NULL;

        if(cmd_p->next->next==NULL)return NULL;//��ʱ�����������ֵ������from
        if(strcmp(cmd_p->next->next->data,"from")!=0)return NULL;

        if(cmd_p->next->next->next==NULL)return NULL;//ƥ��pc����������select��������������
        int select_type=select_find_type(cmd_p->next->next->next->data,column_name);
        if(select_type==INT || select_type==FLOAT)
        {
            if(!(pc->datatype==INT || pc->datatype==FLOAT))return NULL;
        }
        else if(select_type==TEXT)
        {
            if(pc->datatype!=TEXT)return NULL;
        }
        else return NULL;


        right_value_copy=(char *)realloc(right_value_copy,strlen(right_value_copy)+1);
        strcat(right_value_copy,";");
        char *select_return=NULL;
        select_return=str_adjust(right_value_copy);

        if(strcmp(select_return,"error")==0)return NULL;
        int s_r_i=0;
        for(s_r_i=0;select_return[s_r_i]!='\0';s_r_i++)
            if(select_return[s_r_i]==' ')return NULL;

        nums *result=NULL;
        nums **resu_p=&result;
        Value *pv=pc->head_value;
        int index=0;
        while(pv)
        {
            index++;
            switch(match_key(optr))//�˴���ʱû�п���like�Լ�between
            {
                case 39://=
                {
                    if(strcmp(pv->data,select_return)==0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 40://==
                {
                    if(strcmp(pv->data,select_return)==0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 42://~=
                {
                    if(strcmp(pv->data,select_return)!=0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 43://>=
                {
                    if(strcmp(pv->data,select_return)>=0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 44://<=
                {
                    if(strcmp(pv->data,select_return)<=0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 45://>
                {
                    if(strcmp(pv->data,select_return)>0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 46://<
                {
                    if(strcmp(pv->data,select_return)<0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 28://like
                {
                    int ok=1;
                    ok=wildcard(pv->data,select_return,0);
                    if(ok==0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                default :
                    return NULL;
                    break;
            }
            pv=pv->next;
        }
        if(result==NULL)error=EMPTY;
        return result;
    }
    else if(match_key(optr)==28)//����like�����
    {//printf("like\n\n");
    //printf("������optr=%s\tright=%s\n\n",optr,right_value);
        if(right_value[0]!='[')return NULL;
        if(pc->datatype!=TEXT)return NULL;
        char word[strlen(right_value)];
        int i=0,j=0;
        for(i=1;right_value[i]!='\0';i++)
            if(right_value[i]!=']')word[j++]=right_value[i];
        word[j]='\0';//printf("word==%s\n",word);
        nums *result=NULL;
        nums **resu_p=&result;
        Value *pv=pc->head_value;
        int index=0;
        while(pv)
        {
            index++;
            int ok=1;
            ok=wildcard(pv->data,word,0);printf("data=%s\tword=%s\tok=%d\n",pv->data,word,ok);
            if(ok==0)
            {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
            }
            pv=pv->next;
        }

        /*nums *test=NULL;
        test=result;if(test->next==NULL)printf("ok!!!!\n");
        while(test)
        {
            printf("test==%d\n",test->data);
            test=test->next;
        }*/
        if(result==NULL)error=EMPTY;
        return result;
    }
    else if(right_value[0]=='\'')//���ǵ�ֵ�Ƚ�ʱ������һ�����ơ�Beijing���Ľṹ
    {
        //printf("danzhi\n\n");
        if(pc->datatype!=TEXT)return NULL;
        char word[strlen(right_value)+1];
        int i=0,j=0;
        for(i=0;right_value[i]!='\0';i++)//if(right_value[i]!='\'')
            word[j++]=right_value[i];
        word[j]='\0';//printf("word=%s\n\n",word);
        nums *result=NULL;
        nums **resu_p=&result;
        Value *pv=NULL;
        //if(pc==NULL)printf("nimei\n");
        pv=pc->head_value;
        //if(pv==NULL)printf("wocao!!!!\n");
        //else printf("nimei\t%s\n",pv->data);
        int index=0;
        while(pv)
        {
            index++;
            switch(match_key(optr))
            {
                case 39://=
                {//printf("optr=%s\n",optr);
                    if(strcmp(pv->data,word)==0)
                    {//printf("optr=%s\n",optr);
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 40://==
                {

                    //printf("pv:%s\n",pv->data);
                    if(strcmp(pv->data,word)==0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                        //printf("index==%d\n",index);
                        //nums *test=result;
                        //printf("result=%d\n",test->data);
                    }

                    break;
                }
                case 42://~=
                {//printf("an\n");
                    if(strcmp(pv->data,word)!=0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 43://>=
                {
                    if(strcmp(pv->data,word)>=0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 44://<=
                {
                    if(strcmp(pv->data,word)<=0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 45://>
                {
                    if(strcmp(pv->data,word)>0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                case 46://<
                {
                    if(strcmp(pv->data,word)<0)
                    {
                        (*resu_p)=(nums *)malloc(sizeof(nums));
                        (*resu_p)->data=index;
                        (*resu_p)->next=NULL;
                        resu_p=&(*resu_p)->next;
                    }
                    break;
                }
                default :
                    return NULL;
                    break;
            }
            pv=pv->next;
        }
        /*
        nums *test=result;
        if(result->next==NULL)printf("right!!!!!!!!!!!!!!!!!!\n");
        while(test)
        {
            printf("test!!!!!=%d\n",test->data);
            test=test->next;
        }*/
        if(result==NULL)error=EMPTY;
        return result;
    }
    else return NULL;
    return NULL;
}
