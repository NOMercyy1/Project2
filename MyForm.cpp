#include "MyForm.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <msclr\marshal_cppstd.h> // Для msclr::interop::marshal_as
#include <sstream>
#include <iomanip>
#include <bitset>
#include <array>
#include <string>
#include <random>
#include <vector>
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;

//[STAThread]
[STAThreadAttribute]
void main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Project2::MyForm form;
	Application::Run(% form);
}

// Алгоритм Евклида
int GCD(unsigned int a, unsigned int b) {
	if (b == 0)
		return a;
	return GCD(b, a % b);
}

// Алгоритм возведения в степень без переполнения
long long pows(long long a, long long b, long long m) {
	long long result = 1;
	a = a % m;
	while (b > 0) {
		if (b % 2 == 1) {   //Если b нечётное,то данная степень двойки (текущий a) должна присутствовать в разложении
			result = (result * a) % m;
		}         //Если b чётное, то данная степень двойки не нужна, и мы её просто игнорируем
		a = (a * a) % m;
		b /= 2;  //неполное частное с округлением вниз
	}
	return result;
}

// Тест Ферма 
bool Ferma(unsigned int q) {
	if (q == 2)
		return true;
	for (int i = 0; i < 10; i++) {
		unsigned int a = (rand() % (q - 2)) + 2; //случайное a от 2 до q-1
		if (GCD(a, q) != 1)
			return false;
		if (pows(a, q - 1, q) != 1)
			return false;
	}
	return true;
}

System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) // генерация ключей 
{
	unsigned int p, q;

	do {
		q = rand() % (65536);
		if ((q % 2) == 0)
		{
			q--;
		}
	} while (Ferma(q) == false);

}
System::Void button2_Click(System::Object^ sender, System::EventArgs^ e)
{

}
System::Void button3_Click(System::Object^ sender, System::EventArgs^ e)
{

}