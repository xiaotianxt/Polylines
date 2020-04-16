#include "util.h"

const double eps = 1e-8; // double precision

bool equal(double a, double b) // �����ж������������Ƿ���ȡ�����Ϊeps.
{
	if ((a - b) > -eps && (a - b) < eps) // ��ֹ���
		return true;
	else
		return false;
}

int compare(double a, double b)
{
	// a �� b �󷵻� -1
	if ((a - b) - eps > 0) // �����ȥeps����0���ǲ�����Ĵ�
		return -1;
	// b �� a �󷵻� 1
	if ((b - a) - eps > 0) // ͬ��
		return 1;
	return 0;
}

vector<string> GetFiles(string cate_dir)
{
	vector<string> files; //����ļ���
	_finddata_t file;
	long lf;
	//�����ļ���·��
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1)
	{
		cout << cate_dir << " not found!!!" << endl;
	}
	else //
	{
		while (_findnext(lf, &file) == 0)
		{
			//����ļ���
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
	//���򣬰���С��������
	sort(files.begin(), files.end());
	return files;
}

vector<string> FileName(string pattern)
{
	char current_address[100];
	memset(current_address, 0, 100);
	getcwd(current_address, 100); //��ȡ��ǰ·��
	cout << current_address << endl;
	strcat(current_address, "\\*");

	vector<string> files = GetFiles((string)current_address);
	vector<string> targetfiles;

	for (int i = 0; i < files.size(); i++)
	{
		cout << files[i] << endl;
	}

	cout << "end..." << endl;
	cin.get();
	return targetfiles;
}
