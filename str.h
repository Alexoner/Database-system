#ifndef STR_H
#define STR_H


typedef struct strs
{
	char *data;
	struct strs *last;
	struct strs *next;
}strs;


typedef struct nums
{
	int data;
	struct nums *next;
}nums;

extern strs *strs_head;
extern strs *strs_crt;
extern nums *nums_head;
extern nums *nums_crt;


strs *str_break(char *str);//�������ؼ��ֶ�Сд
strs *strs_break(char *str);
char *str_merge(strs *hstr,int n);//�����źϲ��ˣ��޿ո񣬲������ܳ��������������ӵ����
int str_len();
int strs_len(strs *hs);
strs *str_adr(int n);//����ڼ�������1��ʼ����������ĵ�ַ,�������NULL
strs *strs_adr(strs *hs,int n);
void str_std(char *str);
char *str_tolower(char *str);
char *str_bracket(char *str);//��������ŵı��ʽ�����ص�һ����������ͷ�������ŵ�����,������ַ�����һ������Ϊ'('��

int match_key(char *str);

nums *num_and(nums *n1,nums *n2);
nums *num_or(nums *n1,nums *n2);
int num_len(nums *nh);
void num_sort(nums *nh,int order);//0Ϊ����1Ϊ����
nums *num_adr(nums *nh,int n);//��str_adrͬ��
int match_num(nums *nh,nums *ni);//����ni��nh���е�λ�ã���1��ʼ�����������򷵻�0��

int match_str(strs *sh,strs *si);//ͬ��

int str_pos(char *str,char c,int t);//����c��text�е�t�γ��ֵ�λ��(��0��ʼ)��t>0,�����ڷ���-1

int wildcard(const char *text, const char *wi, int flag);//ͨ�������ʼflag=0��ǰ����column �����Ǵ�*�򣿵��ַ���		����0ƥ��

//void val_d(struct column *hc,int nc);//ɾ��������

struct value *value_adr(struct value *vh,int n);

int is_str(char *src);
int is_text(char *src);
char * chfloat(char *str1);

#endif
