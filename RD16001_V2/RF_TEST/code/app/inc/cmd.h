#ifndef _CMD_H
#define _CMD_H
/*
  1��//д�������ָ�������Ӳ���汾��
send��FE 03 00 00 00 00 FE
response��app set flag ok\r\n

2��//����ָ��
send��FE 07 00 00 00 00 FE
response��app restart\r\n


3��//��������ģʽָ��
send��FE 01 00 00 00 00 FE
response��boot enter update mode,please send 1 package\r\n
����������������������
����������������������
����������������������
��������ÿ��������1024�ֽڣ�Ӧ����boot receive = %d, write = %d, please send %d package\r\n

4��//��ȡ�����ļ�����ָ��
send��FE 08 00 00 00 00 FE
response��boot update = %d Bytes, Sum = 0x%x\r\n

5��//�����־
send��FE 09 00 00 00 00 FE
response��boot erase flag ok\r\n������boot erase flag error\r\n��ǰ����û�б�ǲŻ���ˣ�

//����ָ��
send��FE 07 00 00 00 00 FE
response��boot restart\r\n

*/
struct CmdType{
  char start;
  char cmd;
  char para[20];
  char end;
};

unsigned char cmdExtract(char *datstring, struct CmdType *cmdstructure);
void cmdProc(struct CmdType cmdstructure);
void cmdTask(void);

#endif  