

CPPHelper ��һ��C++����������⣬Ŀ�����ڸ��Ӹ�Ч����ȫ�Ľ��п�����


ʹ�÷�����
1����CPPHelper���Ƶ�����Ŀ¼

����D:\Visual Studio 2015\Projects\MySln\CPPHelper (MySln������Ҫ����Ĺ���)

2���� VC++Ŀ¼ -- ����·�� ��� $(SolutionDir)CPPHelper\
3���� VC++Ŀ¼ -- ��Ŀ¼   ��� $(SolutionDir)CPPHelper\$(PlatformShortName)\

4����stdafx.h ����� #include <BaseFunction.h>
5����stdafx.cpp ����� #include <BaseFunction.cpp>

���ˣ���������Ѿ���������ʹ�ã���������Thread���࣬��������ʹ��ʱ include


��ܰ��ʾ������������֧��C++ 0X,���ұ�����Ҫ���ÿ��ַ�������������




Ŀǰ����������¼������ܣ�

///////////////////////////////////////////////rapidxml//////////////////////////////////////////////////////
�������޸ĺ��һ��rapidxml�⣬���й淶��ԭ���rapidxmlһ�������Ǵ˰汾����֧��UTF8�Լ�UTF16��������ִ��printʱ�ṩ�˸������ܡ�

rapidxml::XMLDocument Document;

//��һ���ļ�·����xml���Զ����б����ж�
XMLCreateXMLDocumentByFile(L"D:\test.xml",&Document);


//�Ӷ��������ݴ�XML���Զ����б����ж�
XMLCreateXMLDocumentByData(pBinary,cbBinary,&Document);


//���ַ�����XML
XMLCreateXMLDocumentByString(L"<Root></Root>",&Document)


//��Root/Files�ڵ�
auto pFiles= XMLOpenNote(&Document,L"Root/Files");

<Root>
	<Files/>
</Root>

����




///////////////////////////////////////////MsIsoReader////////////////////////////////////////////////////////
����һ��΢���һ��ISO��ȡ�⣬ԭ������C#�棬�ҽ�����ֲ����C++�ϣ�һ�����ù���ȥ��Dism++��ֱ���ͷ�ISO����Ҳ�Ǵ˿�֧��









////////////////////////////////////////////Thread��////////////////////////////////////////////////////////

Thread��
�첽�࣬����ִ���첽����
ͷ�ļ���Thread.h
ʹ�÷�����

Thread([]()
{
	/*����Ҫ�첽ִ�е����Thread���Զ�����ɾ���ͷš�
	������һ��Lambda���ʽ������Բο�Lambda���ʽ��ʹ�á�
	*/
});



Task��
�첽����������ִ���첽���񣬵������̴߳������̳߳أ����ٳ��򴴽������̵߳Ŀ�����
ͷ�ļ���Thread.h
ʹ�÷�����

Task([]()
{
	/*����Ҫ�첽ִ�е����
	������һ��Lambda���ʽ������Բο�Lambda���ʽ��ʹ�á�
	*/
});



ThreadPool��
�ƶ��̳߳أ�����ִ�д��������񣬺��ʼ����ܼ��Ͳ��м��㡣
ͷ�ļ���Thread.h
ʹ�÷�����


ThreadPool WorkPool;

//���ʮ���̲߳�����Ĭ��ֵ500��΢��涨�ģ�
WorkPool.SetThreadMaximum(10);

int Datas[100];


WorkPool.For(Data,100,[](int& Data)
{
	//10�̲߳������Datas�е�����
	wprinf(L"%d\r\n",Data);
});






////////////////////////////////////////////hanlde���ܾ��////////////////////////////////////////////////

hanlde:
���ܾ���������Զ��رո��־����
ͷ�ļ���handle.h
ʹ�÷�����

CHKEY - ע������ܾ��

//�Զ���ʼ��ΪNULL
CHKEY Test;

RegOpenKey(HKEY_LOCAL_MACHINE,L"SOFTWARE",&Test);

//����������ر�Test��Test������ʱ�Զ��ر�HKEY


CHFile - �ļ����ܾ��

//�Զ���ʼ��ΪINVALID_HANDLE_VALUE
CHFile Test;

Test=CreateFile(����);

//����������ر�Test��Test������ʱ�Զ��ر�HANDLE

�ȵȡ���



/////////////////////////////////////////////////��Դ����///////////////////////////////////////////////
ͷ�ļ���Guard.h

int Test()
{
	auto pData=new int;

	//RunOnExit�еĴ������˳�ʱִ��
	RunOnExit([pData]()
	{
		//Test��������ʱ�����Զ�ִ�д˴���
		delete pData;
	});



	//RunOnCrash�еĻ��ڳ������ʱִ��
	RunOnCrash([]()
	{
		MessageBox(NULL,L"�ҵĳ��������",L"Sorry",0);
	});

	return 0;
}




//////////////////////////////////////////////����/////////////////////////////////////////
1��ComHelper���ڸ�����дcom�ӿ�
2��CabinetHelper���ڴ��Լ�����cab�ļ�
3��DiskHelper�����޸Ĵ���MBR
4��FileHelper�ṩ�����ļ������������ļ�ɾ���������޸ġ���С����
5��IniHelper���ṩ������ini��д
6��RegHelper���ṩ������ע������
7��StringHelper���ṩ�������ַ�������
8��VirtDiskHelper���˿��Ѿ����ã�ûɶ����
9��WinInetHelper���ṩ��������
10��WinnlsHelper��ûʲô���ܣ���������
11��StreamEx��ûʲô���ܣ�ͨ�ò�����������ʹ�ã���������

