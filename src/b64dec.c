#include "mref.h"

char inode(char ch)		//编码，按照索引表进行替换
{
	if(ch<'\32')		//用ASCII表示字符只能用八进制，血的教训啊，浪费了我两个小时
		ch=ch+'A';
	else if(ch>='\32' && ch<='3')
		ch=ch+'G';
	else if(ch>='4' && ch<='=')
		ch=ch-'\4';
	else if(ch=='>')
		ch='+';
	else if(ch=='?')
		ch='/';
	return ch;
}
void encode(char *s1,char *s2)		//将s1编码，存储在s2中
{
	char opr='?';
	s2[3]=inode(s1[2] & opr);	//移位后还要按索引表替换，受人误导，没注意这点，此处浪费一个小时
	s2[2]=inode(((s1[1]<<2) | (s1[2]>>6)) & opr);//&优先级高于|,故要在前面加括号
	s2[1]=inode(((s1[1]>>4) | (s1[0]<<4)) & opr);
	s2[0]=inode(s1[0]>>2);
}



unsigned char unode(unsigned char ch) {
	if (ch>='A' && ch<='Z')
		ch -= 65;
	else if(ch>='a' && ch<='z')
		ch -= 71;
	else if(ch>='0' && ch<='9')
		ch += 4;
	else if(ch=='+')
		ch=62;
	else if(ch=='/')
		ch=63;
	return ch;
}

static void decode(unsigned char *s1, char *s2) {
	s1[0]=unode(s2[0])<<2 | unode(s2[1])>>4;
        if (s2[2] != '=') {
            s1[1]=unode(s2[1])<<4 | unode(s2[2])>>2;
            if (s2[3] != '=')
                s1[2]=unode(s2[2])<<6 | unode(s2[3]);
        }
}

void _mref_b64dec(unsigned char *out, unsigned char *in, size_t len) {
    size_t i, o;

    for (i = 0, o = 0; i < len; i += 4, o += 3)
        decode(out + o, in + i);
}
