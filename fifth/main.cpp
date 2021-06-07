#include <iostream>
#include <vector>
#include <string>
#include "mpi.h"


using namespace std;

// класс "комплексное число"
class Complex {
 public:
  int real;

  int imag;

  Complex(int r, int i) {
	real = r;
	imag = i;
  }

  Complex() {
	real = 1;
	imag = 0;
  }

  void null() {
	real = 0;
	imag = 0;
  }

  string print() {
	string str = "(" + to_string(real) + "," + to_string(imag) + ")";
	return str;
  }

  Complex& operator=(const Complex& num) {
	real = num.real;
	imag = num.imag;

	return *this;
  }

  void operator+=(const Complex& num) {
	real += num.real;
	imag += num.imag;
  }

  bool operator==(const Complex& num) {
	return (real == num.real) && (imag == num.imag);
  }

  Complex operator+(const Complex& num) {
	int r = real + num.real;
	int i = imag + num.imag;
	return Complex(r, i);
  }

  Complex operator-(const Complex& num) {
	int r = real - num.real;
	int i = imag - num.imag;
	return Complex(r, i);
  }

  friend Complex operator*(const Complex& num1, const Complex& num2);
};

Complex operator*(const Complex& num1, const Complex& num2) {
  int r = num1.real * num2.real - num1.imag * num2.imag;
  int i = num1.imag * num2.real + num1.real * num2.imag;
  return Complex(r, i);
}

//класс "комплексная матрица"
class ComplexMatrix {
 private:
  vector<vector<Complex>> complex_array;

 public:
  // конструктор с размером 
  ComplexMatrix(const size_t& capacity, const int& a) {
	vector<Complex> buf_vec;
	Complex buf_num = {0, 0};
	complex_array.reserve(capacity);
	for (size_t i = 0; i < capacity; ++i) {
	  buf_vec.push_back(buf_num);
	}
	for (size_t i = 0; i < capacity; ++i) {
	  complex_array.push_back(buf_vec);
	}
	for (size_t i = 0; i < capacity; ++i) {

	  for (size_t j = 0; j < capacity; ++j) {
		if (i == j) {
		  complex_array[i][j].real = a;
		  complex_array[i][j].imag = 0;
		}
	  }
	}
  }

  ComplexMatrix(const size_t& capacity) {
	vector<Complex> buf_vec;
	Complex buf_num = {0, 0};
	complex_array.reserve(capacity);
	for (size_t i = 0; i < capacity; ++i) {
	  buf_vec.push_back(buf_num);
	}
	for (size_t i = 0; i < capacity; ++i) {
	  complex_array.push_back(buf_vec);
	}
	for (size_t i = 0; i < capacity; ++i) {

	  for (size_t j = 0; j < capacity; ++j) {
		if (i == j) {
		  complex_array[i][j].real = 1;
		  complex_array[i][j].imag = 0;
		}
	  }
	}
  }

  // конструктор принимающий ссылку на развернутый в памяти массив
  ComplexMatrix(int& ptr) {
	for (size_t i = 0; i < complex_array.size(); i++) {
	  for (size_t j = 0; j < complex_array.size(); j++) {
		complex_array[i][j].real = ptr++;
		complex_array[i][j].imag = ptr++;
	  }
	}
  }

  ComplexMatrix() {
	complex_array.reserve(0);
  }

  //собирает матрицу из блоков
  ComplexMatrix(ComplexMatrix C11, ComplexMatrix C12, ComplexMatrix C21, ComplexMatrix C22) {
	size_t N = C11.size();
	vector<Complex> buf;
	//complex_array.reserve(N * 2);
	for (size_t i = 0; i < N * 2; ++i) {
	  buf.push_back({0, 0});
	}
	for (size_t i = 0; i < N * 2; ++i) {
	  complex_array.push_back(buf);
	}

	for (size_t i = 0; i < N; ++i) {
	  for (size_t j = 0; j < N; ++j) {
		complex_array[i][j] = C11[i][j];
	  }
	}
	for (size_t i = 0; i < N; ++i) {
	  for (size_t j = 0; j < N; ++j) {
		complex_array[i][j + N] = C12[i][j];
	  }
	}
	for (size_t i = 0; i < N; ++i) {
	  for (size_t j = 0; j < N; ++j) {
		complex_array[i + N][j] = C21[i][j];
	  }
	}
	for (size_t i = 0; i < N; ++i) {
	  for (size_t j = 0; j < N; ++j) {
		complex_array[i + N][j + N] = C22[i][j];
	  }
	}
  }

  //оператор присваивания к комплексному массиву
  void operator=(ComplexMatrix& CM) {
	complex_array = CM.complex_array;
  };

  void operator=(ComplexMatrix CM) {
	complex_array = CM.complex_array;
  };

  //оператор присваивания к развернутому массиву
  void operator=(int& static_array) {
	for (int i = 0; i < complex_array.size(); ++i) {
	  for (int j = 0; j < complex_array.size(); ++j) {
		complex_array[i][j].real = static_array++;
		complex_array[i][j].imag = static_array++;
	  }
	}
  };

  vector<Complex>& operator[](const size_t& i) {
	return complex_array[i];
  }

  size_t size() {
	return complex_array.size();
  }

  void print() {
	for (size_t i = 0; i < complex_array.size(); ++i) {
	  for (size_t j = 0; j < complex_array[i].size(); ++j) {
		cout << complex_array[i][j].print() << " ";
	  }
	  cout << endl;
	}
  };

  //возвращает 1/4 массива для Штрассена
  ComplexMatrix block(size_t line, size_t column) {
	size_t N = complex_array.size() / 2;
	ComplexMatrix buf(N, 0);
	if (line == 1 && column == 1) {
	  for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
		  buf[i][j] = complex_array[i][j];
		}
	  }
	  return buf;
	}
	if (line == 1 && column == 2) {
	  for (size_t i = 0; i < N; ++i) {
		for (size_t j = N; j < 2 * N; ++j) {
		  buf[i][j - N] = complex_array[i][j];
		}
	  }
	  return buf;
	}
	if (line == 2 && column == 2) {
	  for (size_t i = N; i < 2 * N; ++i) {
		for (size_t j = N; j < 2 * N; ++j) {
		  buf[i - N][j - N] = complex_array[i][j];
		}
	  }
	  return buf;
	}
	if (line == 2 && column == 1) {
	  for (size_t i = N; i < 2 * N; ++i) {
		for (size_t j = 0; j < N; ++j) {
		  buf[i - N][j] = complex_array[i][j];
		}
	  }
	  return buf;
	}
  }

  // переопределенные арифметические опертаоры
  friend ComplexMatrix operator-(ComplexMatrix& CM1, ComplexMatrix& CM2);

  friend ComplexMatrix operator+(ComplexMatrix& CM1, ComplexMatrix& CM2);

//  friend ComplexMatrix operator*(ComplexMatrix& CM1, ComplexMatrix& CM2);
  friend ComplexMatrix operator*(ComplexMatrix CM1, ComplexMatrix CM2);
};

//ComplexMatrix operator*(ComplexMatrix& CM1, ComplexMatrix& CM2)
//{
//  ComplexMatrix CM_buf(CM1.size());
//  Complex buf(0, 0);
//  for (size_t i = 0; i < CM1.size(); ++i)
//  {
//    for (size_t j = 0; j < CM1.size(); ++j)
//    {
//      for (size_t k = 0; k < CM1.size(); ++k)
//      {
//        buf += CM1[i][k] * CM2[k][j];
//      }
//      CM_buf[i][j] = buf;
//      buf.null();
//    }
//  }
//
//  return CM_buf;
//};

ComplexMatrix operator*(ComplexMatrix CM1, ComplexMatrix CM2) {
  ComplexMatrix CM_buf(CM1.size());
  Complex buf(0, 0);
  for (size_t i = 0; i < CM1.size(); ++i) {
	for (size_t j = 0; j < CM1.size(); ++j) {
	  for (size_t k = 0; k < CM1.size(); ++k) {
		buf += CM1[i][k] * CM2[k][j];
	  }
	  CM_buf[i][j] = buf;
	  buf.null();
	}
  }

  return CM_buf;
};

ComplexMatrix operator+(ComplexMatrix& CM1, ComplexMatrix& CM2) {
  ComplexMatrix CM_buf(CM1.size());
  for (size_t i = 0; i < CM1.size(); ++i) {
	for (size_t j = 0; j < CM1.size(); ++j) {
	  CM_buf[i][j] = CM1[i][j] + CM2[i][j];
	}
  }
  return CM_buf;
};

ComplexMatrix operator-(ComplexMatrix& CM1, ComplexMatrix& CM2) {
  ComplexMatrix CM_buf(CM1.size());
  for (size_t i = 0; i < CM1.size(); ++i) {
	for (size_t j = 0; j < CM1.size(); ++j) {
	  CM_buf[i][j] = CM1[i][j] + CM2[i][j];
	}
  }
  return CM_buf;
};

//рекурентный алгоритм Штрассена для умножения комплексных матриц
//перемножает матрицы за O(n^log7), а не за O(n^3)
ComplexMatrix Strassen(ComplexMatrix A, ComplexMatrix B) {
  ComplexMatrix С(A.size());

  //выход
  if (A.size() <= 2) {
	С = A * B;
	return С;
  }
  ComplexMatrix A11 = A.block(1, 1);
  ComplexMatrix A12 = A.block(1, 2);
  ComplexMatrix A21 = A.block(2, 1);
  ComplexMatrix A22 = A.block(2, 2);

  /*A12.print();
  A21.print();*/



  ComplexMatrix B11 = B.block(1, 1);
  ComplexMatrix B12 = B.block(1, 2);
  ComplexMatrix B21 = B.block(2, 1);
  ComplexMatrix B22 = B.block(2, 2);

  ComplexMatrix P1 = Strassen(A11 + A22, B11 + B22);
  ComplexMatrix P2 = Strassen(A21 + A22, B11);
  ComplexMatrix P3 = Strassen(A11, B12 - B22);
  ComplexMatrix P4 = Strassen(A22, B21 - B11);
  ComplexMatrix P5 = Strassen(A11 + A12, B22);
  ComplexMatrix P6 = Strassen(A21 - A11, B11 + B12);
  ComplexMatrix P7 = Strassen(A12 - A22, B21 + B22);

  ComplexMatrix m1 = P1 + P4;
  ComplexMatrix m2 = m1 - P5;
  ComplexMatrix m3 = P1 - P2;
  ComplexMatrix m4 = m3 - P3;

  ComplexMatrix C11 = m2 + P7;
  ComplexMatrix C12 = P3 + P5;
  ComplexMatrix C21 = P2 + P4;
  ComplexMatrix C22 = m4 + P6;

  return ComplexMatrix(C11, C12, C21, C22);
}

int main() {

  ComplexMatrix L1(8, 2);
  ComplexMatrix L2(8, 2);
  ComplexMatrix L = Strassen(L1, L2);


  L.print();

  return 0;
}