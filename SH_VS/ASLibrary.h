#ifndef __ASLibrary__
#define __ASLibrary__
#define CommentLen 20			//����� ����������� � ���������
#define AS_NRF24L01_payload 5   //������������ ������ ������ � ������ ��� ���������� NRF24L01
								//������ ���� �� ������ ������������ ����� ������������ ������
								//� �� ������ 32 (��������� _AS_COMMAND � _AS_ANSWER)
#define AS_NRF24L01_channel 77  //�����, �� ������� �������� ��������� NRF24L01
								//RF Channel 0 - 127 or 0 - 84 in the US, default 0.

// ������� ������� �� 1 ������� � 1 ���������
// 0 x - ������������
// 1 x - �������� ���������� ��������
// 2 x - ��������� ��������� �� ���� �������� (�������� � ���������� ����������, ���� �� ���������)
// 3 N - ��������� ��������� �� ������� N (�������� � ���������� ����������, ���� �� ���������)
// 4 N - �������� ������ � ������� N (������� �� ���������� ����������)

typedef struct _AS_COMMAND {
	byte  Command; //�������
	byte  Parametr; //��������
} AS_Command;

// ������ ������� �� 1 ������� � 1 ��������
// 0 x - ������
// 1 N - ������ �������� ���������, N ��������

typedef struct _AS_ANSWER {
	byte   Status; //������
	float  Value; //��������
	char   Comment[CommentLen]; //��������
} AS_Answer;


// ��������� ��������
// 0 x - ������
// 1 N - ������ ���������, �������� N

typedef struct _AS_SENSORSTATUS
{
	byte	Status; //������
	float	Value; //��������
	char	Comment[CommentLen]; //��������
} AS_SensorStatus;

#endif // __ASLibrary__
