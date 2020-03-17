#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

void inversion(double** A, int N);
void print(double** temp, int n, int k);
double Lagranzh(double x, int y1, int y2, int month, double** tempmonth);
double firstNewton(double x, int y1, int y2, int month, double** tempmonth);
double secondNewton(double i, int y1, int y2, int month, double** tempmonth);
double Steppolinom(double x, int s, double** koef);
int factor(int n);
double delta(double i, double j, int n, double* y);
void multiply(double** A, double** B, double** C, int c, int d, int f);
int main()
{
	///Записать значения в двумерный массив
	double inp; // для ввода
	string line;
	int j;

	double** temp = new double* [177];
	for (int i = 0; i < 177; i++)  temp[i] = new double[13];
	for (int i = 0; i < 177; i++) for (int j = 0; j < 13; j++) temp[i][j] = 0;
	int n = 0; ///номер строки, временная переменная, пока не узнаем, как отфильтровать первую строку сразу

	ifstream file;
	file.open("petrKamch.csv");

	if (!file.is_open()) cout << "Unknown Error \n";

	while (file.good())
	{
		getline(file, line);
		if (n != 0)
		{
			for (long long unsigned int i = 0; i < line.size(); i++) { if (line[i] == ',') line[i] = ' '; };
			stringstream ss(line);
			j = 0;
			while ((ss >> inp) && j < 13)
			{
				temp[n - 1][j] = inp;
				j++;
			}
		}//if
		n++;
	}
	n = 177;

	///Lagranzh's polinom:
	ofstream file2;
	file2.open("Lagranzh.txt");
	double f;
	int m = 7; //7 месяц
	for (double i = 0; i <= 11; i += 0.1)
	{
		f = Lagranzh(i, 70, 81, m, temp);
		file2 << i << "\t " << f << endl;
	}
	file2.close();

	/// First Newton's formula:
	file2.open("FirstNewton.txt");
	for (double i = 0; i <= 5; i += 0.1)
	{
		f = firstNewton(i, 54, 59, m, temp);
		file2 << i << "\t " << f << endl;
	}
	file2.close();


	///Second Newton's formula:
	file2.open("SecondNewton.txt");
	for (double i = 0; i <= 5; i += 0.1)
	{
		f = secondNewton(i, 65, 70, m, temp);
		file2 << i << "\t " << f << endl;
	}
	file2.close();
	
	
	file2.open("Ishod.txt");
	for (int i = 0; i < 177; i ++)
	{if(temp[i][7] != 999.9){
		f = temp[i][7];
		file2 << i << "\t " << f << endl;
	}
	}
	file2.close();
	
	
	
	
	
	
	///Stepennoy polinom:
	file2.open("Steppolinom.txt");
	cout << "Enter pow of polinom you want to have: ";
	int s;
	cin >> s;
	cout << "Enter month you want to see: ";
	int month;
	cin >> month;

	vector<double>  temperature;
	vector<int> stroki;
	int nstrok = 0;
	for (int i = 0; i < 177; i++) if (temp[i][month] != 999.9) { temperature.push_back(temp[i][month]); nstrok++; stroki.push_back(nstrok); }

	cout << "Amount of right degrees: " << temperature.size() << endl;

	int n1 = stroki.size(); /// количество переменных

	///Создадим матрицу для значений y (температур) || ПРАВАЯ ЧАСТЬ УРАВНЕНИЯ
	double** secondmat = new double* [s + 1];
	for (int i = 0; i < s + 1; i++) secondmat[i] = new double[1];	///матрица решений
	for (int i = 0; i < s + 1; i++) secondmat[i][0] = 0;

	for (int i = 0; i < s + 1; i++)
	{
		for (int j = 0; j < n1; j++) secondmat[i][0] += temperature[j] * pow(stroki[j], i);
	}


	/// Создадим матрицу сумм произведений х в разных степенях|| САМАЯ БОЛЬШАЯ МАТРИЦА В УРАВНЕНИИ
	double** firstmat = new double* [s + 1];
	for (int i = 0; i < s + 1; i++) firstmat[i] = new double[s + 1];

	for (int i = 0; i < s + 1; i++)
	{
		for (int j = 0; j < s + 1; j++)
		{
			if (i == 0 && j == 0) firstmat[i][j] = n1;
			else { for (int k = 0; k < n1; k++) firstmat[i][j] += pow(stroki[k], i + j); }
		}
	}

	///Создадим матрицу коэффициентов || ТО, ЧТО НУЖНО НАЙТИ
	double** koef = new double* [s + 1];
	for (int i = 0; i < s + 1; i++) koef[i] = new double[1];
	for (int i = 0; i < s + 1; i++) koef[i][0] = 0.0;

	inversion(firstmat, s + 1);


	multiply(firstmat, secondmat, koef, s + 1, s + 1, 1);
	cout << "Matryx koef: " << endl;
	print(koef, s + 1, 1);
	cout << endl;

	int k = stroki[n1 - 1];
	for (double i = 0; i < k; i += 0.1)
		file2 << i << "\t  " << Steppolinom(i, s, koef) << endl;

	file2.close();
	///Печать массива
	print(temp, 177, 12);
	delete[] temp;
	system("python3 lab3.py");
	return 0;
}



///Печать массива
void print(double** temp, int n, int k)
{
	for (int i = 0; i < n; i++)
	{
		cout << i << "\t |";
		for (int j = 0; j < k; j++) cout << temp[i][j] << "\t |";
		cout << endl;
	}

}

/// Метод Лагранжа
double Lagranzh(double x, int y1, int y2, int month, double** tempmonth)
{
	int* yearind = new int[y2 - y1 + 1]; // индекс строки года
	for (int i = y1; i <= y2; i++)  yearind[i - y1] = i - y1;
	double* temp = new double[y2 - y1 + 1];
	for (int i = y1; i <= y2; i++) temp[i - y1] = tempmonth[i][month];

	double fx, sum = 0.0;
	for (int i = 0; i <= 11; i++)
	{
		fx = 1;
		for (int j = 0; j <= 11; j++)
		{
			if (i != j) fx *= (x - (double)yearind[j]) / ((double)yearind[i] - (double)yearind[j]);
		}
		fx *= temp[i];
		sum += fx;
	}
	ofstream uzly;
	uzly.open("Uzly.txt");
	for (int i = 0; i < 12; i++) uzly << yearind[i] << " " << temp[i] << endl;
	uzly.close();


	return sum;
}
///Первая интерполяционная формула Ньютона

double firstNewton(double x, int y1, int y2, int month, double** tempmonth)
{
	int* yearind = new int[y2 - y1 + 1]; /// индекс строки года
	for (int i = y1; i <= y2; i++)  yearind[i - y1] = i - y1;
	double* temp = new double[y2 - y1 + 1]; ///значения температур в выбранном месяце
	for (int i = y1; i <= y2; i++) temp[i - y1] = tempmonth[i][month];


	double sum = 0.0;	// будущий корень
	double fx=0.0;

	for (int i = 0; i <= 5; i++)
	{
		if (i == 0) { sum += temp[0]; }
		else
		{
			fx = 1;
			for (int j = 0; j < i; j++) fx *= (x - (double)yearind[j]);

			fx /= factor(i);
			fx *= delta(1, 0, i, temp);

		}
		sum += fx;
	}

	ofstream uzly;
	uzly.open("Uzly2.txt");
	for (int i = 0; i < 6; i++) uzly << yearind[i] << " " << temp[i] << endl;
	uzly.close();

	return sum;
}

///Формула для подсчета факториала
int factor(int n)
{
	int k = 1;
	for (int i = 2; i <= n; i++) k *= i;
	return k;
}
/// Формула для подсчета конечной разности
// из i вычитаем j
double delta(double i, double j, int n, double* y)
{
	if (n == 0) return i - j;
	else if (n == 1) return (y[(int)i] - y[(int)j]);
	else return delta(delta(i + 1, j + 1, n - 1, y), delta(i, j, n - 1, y), 0, y);
}



///Вторая интерполяционная формула Ньютона
double secondNewton(double x, int y1, int y2, int month, double** tempmonth)
{
	int* yearind = new int[y2 - y1 + 1]; /// индекс строки года
	for (int i = y1; i <= y2; i++)  yearind[i - y1] = i - y1;
	double* temp = new double[y2 - y1 + 1]; ///значения температур в выбранном месяце
	for (int i = y1; i <= y2; i++) temp[i - y1] = tempmonth[i][month];

	double sum = 0.0;
	double fx;

	for (int i = 0; i <= 5; i++)
	{
		fx = 1;
		if (i == 0) { sum += temp[5]; }
		else {
			for (int j = 5; j >= 5 - i + 1; j--)  fx *= (x - (double)yearind[j]);
			fx /= factor(i);
			fx *= delta(5 - i + 1, 5 - i, i, temp);
			sum += fx;
		}
	}

	ofstream uzly;
	uzly.open("Uzly3.txt");
	for (int i = 0; i < 6; i++) uzly << yearind[i] << " " << temp[i] << endl;
	uzly.close();
	return sum;
}



/// Аппроксимация степенным полнином:

double Steppolinom(double x, int s, double** koef)
{
	double y = 0.0;
	for (int i = 0; i <= s; i++)  y += koef[i][0] * pow(x, i);

	return y;
}

//ОБРАТНАЯ МАТРИЦА
void inversion(double** A, int N)
{
	double temp;

	double** E = new double* [N];

	for (int i = 0; i < N; i++)
		E[i] = new double[N];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			E[i][j] = 0.0;

			if (i == j)
				E[i][j] = 1.0;
		}

	for (int k = 0; k < N; k++)
	{
		temp = A[k][k];

		for (int j = 0; j < N; j++)
		{
			A[k][j] /= temp;
			E[k][j] /= temp;
		}

		for (int i = k + 1; i < N; i++)
		{
			temp = A[i][k];

			for (int j = 0; j < N; j++)
			{
				A[i][j] -= A[k][j] * temp;
				E[i][j] -= E[k][j] * temp;
			}
		}
	}

	for (int k = N - 1; k > 0; k--)
	{
		for (int i = k - 1; i >= 0; i--)
		{
			temp = A[i][k];

			for (int j = 0; j < N; j++)
			{
				A[i][j] -= A[k][j] * temp;
				E[i][j] -= E[k][j] * temp;
			}
		}
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			A[i][j] = E[i][j];

	for (int i = 0; i < N; i++)
		delete[] E[i];

	delete[] E;

}
///Перемножение матриц с d - строки и столбцы первой матрицы, f - столбец второй

void multiply(double** A, double** B, double** C, int c, int d, int f)
{
	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < f; j++)
		{
			for (int r = 0; r < d; r++)
				C[i][j] += A[i][r] * B[r][j];
		}
	}
}










