#include "stdafx.h"
#include "Math.h"
#include "cstdlib"
#include "ctime"
#include "iostream"

#define ALEF 1.0
#define MAX_NUM_OF_ITER 100000
#define FAULT 0.0000001

double sqr(double x);

#pragma region default values
int EnteredImgs[10][28] =
{
	{ 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,1,1,1 },//0
	{ 0,0,0,1, 0,0,1,1, 0,1,0,1, 1,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,0,1 },//1
	{ 1,1,1,1, 0,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,1,0, 0,1,0,0, 1,1,1,1 },//2
	{ 1,1,1,1, 0,0,1,0, 0,1,0,0, 1,1,1,1, 0,0,1,0, 0,1,0,0, 1,0,0,0 },//3
	{ 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,0,0,1 },//4
	{ 1,1,1,1, 1,0,0,0, 1,0,0,0, 1,1,1,1, 0,0,0,1, 0,0,0,1, 1,1,1,1 },//5
	{ 1,1,1,1, 1,0,0,0, 1,0,0,0, 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1 },//6
	{ 1,1,1,1, 0,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,0,1 },//7
	{ 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1 },//8
	{ 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,1, 1,1,1,1 } //9
};

float chisla[10][10] =
{
	{ 1,0,0,0,0,0,0,0,0,0 },
	{ 0,1,0,0,0,0,0,0,0,0 },
	{ 0,0,1,0,0,0,0,0,0,0 },
	{ 0,0,0,1,0,0,0,0,0,0 },
	{ 0,0,0,0,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,0,0,0,0 },
	{ 0,0,0,0,0,0,1,0,0,0 },
	{ 0,0,0,0,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,0 },
	{ 0,0,0,0,0,0,0,0,0,1 }
};
#pragma endregion

class Perceptron
{
	
	int n;		// количество входных сигналов
	int m;		// количество нейронов скрытого слоя
	int* x;		// входящией нейроны
	float* y;	// выходящией нейроны
	float** w;	// матрица весов w

	float ScalarMult(int);
	void CountExitNeurons();
	float ActivationFunc(float);
	
	int K;			// количество элементов обучающей выборки	
	int** X;		// массив входных векторов для обучения	
	float** D;		// массив заданных выходных векторов для обучения	
	float** Y;		// массив вычисленных выходных векторов для обучения

	void GetExitNeuron();
	float EuclidDist(int);
	void initW();
	float fD(float);
	float fD(int);
	float MaxNorma(float **, float **);

public:
	Perceptron(int n, int m) : n(n), m(m)
	{
		w = new float*[m];
		for (int i = 0; i < m; i++)
			w[i] = new float[n];
	}

	~Perceptron()
	{
		for (int i = 0; i < m; i++)
			delete[] w[i];
		delete[] w;
	}
	void calc(int*, float*);
	void DefStudyParams(int**, float**, int);
	int Study(int, float, float dop = ALEF);
};


int main()
{
	int n(28), m(10);

	Perceptron obj = Perceptron(n, m);

	int** X = new int*[m];
	for (int k = 0; k < m; k++)
		X[k] = EnteredImgs[k];

	float** D = new float*[m];
	for (int k = 0; k < m; k++)
		D[k] = chisla[k];

	obj.DefStudyParams(X, D, m);
	int countOfIter = obj.Study(MAX_NUM_OF_ITER, FAULT, 1);
	std::cout << countOfIter << std::endl;

	float* y = new float[m];
	for (int i = 0; i < m; i++)
	{
		obj.calc(X[i], y);
		float max = 0;
		int maxI = 0;
		for (int i = 0; i < m; i++)
		{
			if (y[i] > max)
			{
				max = y[i];
				maxI = i;
			}
		}
		std::cout << "You've given the number: " << maxI;
		std::cout << std::endl;
	}

	delete[] X;
	delete[] D;

	system("Pause");
	
	return 0;

}

#pragma region PrototypesDefenition

inline float Perceptron::ScalarMult(int i)
{
	float s = 0.0f;
	for (int j = 0; j < n; j++)
		s += w[i][j] * x[j];
	return s;
}
inline void Perceptron::CountExitNeurons()
{
	for (int i = 0; i < m; i++)
		y[i] = ActivationFunc(ScalarMult(i));
}
inline float Perceptron::ActivationFunc(float s)
{
	return 1.0f / (1.0f + expf(-ALEF * s));
}
inline void Perceptron::calc(int* x, float* ya)
{
	this->x = x;
	this->y = ya;
	CountExitNeurons();
}

inline void Perceptron::GetExitNeuron()
{
	for (int k = 0; k < K; k++)
	{
		x = X[k];
		y = Y[k];
		CountExitNeurons();
	}
}
inline void Perceptron::DefStudyParams(int** X, float** D, int K)
{
	this->X = X;
	this->D = D;
	this->K = K;
}

inline float Perceptron::EuclidDist(int k)
{
	float s = 0.0f;
	for (int i = 0; i < m; i++)
	{
		s += (float)sqr((double)(D[k][i] - Y[k][i]));
	}
	return sqrtf(s);
}
inline float Perceptron::fD(float s)
{
	float e = expf(-ALEF * s);
	return ALEF * e / (1.0f + e * e);
}
inline float Perceptron::fD(int i)
{
	float al = 1.0f;
	return al * y[i] * (1.0f - y[i]);
}
inline void Perceptron::initW()
{
	srand(time(NULL));
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			w[i][j] = 2.0f / RAND_MAX * rand() - 1.0f;
}
int Perceptron::Study(int maxIter, float eps, float dop)
{
	Y = new float*[K];
	for (int k = 0; k < K; k++)
		Y[k] = new float[m];

	float** w1 = new float*[m];
	for (int i = 0; i < m; i++)
		w1[i] = new float[n];
	initW();	// установим рандомными начальные веса
	int s = 0;	// счетчик
	float** w2;
	float** wInit;
	wInit = w;
	w2 = w1;
	w1 = w;
	w = w2;
	do
	{
		w2 = w1;
		w1 = w;
		w = w2;

		GetExitNeuron();	// заполним выходные нейроны имеющимися весами

		for (int i(0); i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				float sum = 0.0f;
				for (int k = 0; k < K; k++)
				{
					x = X[k];
					//sum += (D[k][i] - Y[k][i])*fD(S(i))*x[j];
					sum += (D[k][i] - Y[k][i])*fD(i)*x[j];
				}
				w1[i][j] = w[i][j] + dop * sum;
			}
		}
		s++;
	} while (MaxNorma(w, w1) > eps && s < maxIter);

	// очищение памяти
	for (int k = 0; k < K; k++)
		delete[] Y[k];
	delete[] Y;
	/*if (w != wInit)
	{
		w1 = w;
		w = wInit;
	};*/
	for (int i = 0; i < m; i++)
		delete[] w1[i];
	delete[] w1;

	// возвращаем число итераций
	return s;
}
float Perceptron::MaxNorma(float **w, float **w1)
{
	float s = 0.0f;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			float r = fabsf(w[i][j] - w1[i][j]);
			if (r > s) s = r;
		}
	return s;
}

#pragma endregion

#pragma region SideFunctions
double sqr(double x)
{
	return x * x;
}
#pragma endregion