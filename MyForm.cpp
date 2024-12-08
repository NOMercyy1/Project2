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

const int P = 31;
#define ll long long
const int min = 0;
const int max = 2 << 10;
std::vector<std::pair<int, int>> v = { {179,359}, {11,89}, {127,509}, {293,  587}, {151, 907}, {137, 823}, {431, 863}, {131, 263}, {103, 619}, {107, 643} };
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
long long mod_inverse(long long a, long long m) {
	long long m0 = m, t, q;
	long long x0 = 0, x1 = 1;
	if (m == 1) return 0;
	while (a > 1) {
		q = a / m;
		t = m;
		m = a % m, a = t;
		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
	}
	if (x1 < 0) x1 += m0;
	return x1;
}
int hash_string(const std::string& str) {
	int hash = 0;
	int p_pow = 1;

	for (char c : str) {
		// Преобразуем символ в число (например, ASCII код)
		int char_value = static_cast<int>(c);

		// Вычисляем хеш
		hash = (hash + char_value * p_pow) % INT32_MAX;
		p_pow = (p_pow * P) % INT32_MAX;
	}

	return std::abs(hash);
}
void generate_keys(long long& p, long long& q, long long& g, long long& y, long long& x) {
	//q = generate_prime(10);
	int pos = std::rand() % (v.size());
	std::cout << "pos = " << pos << "\n";
	q = v[pos].first;
	ll k = 1;
	//do {
	//	//ll k = 1 + rand() % ((max - 1) / q);  // Выбираем случайное k в интервале [1, (max-1)/q]
	//	p = k * q + 1;  // Вычисляем p = k * q + 1
	//	std::cout << "p = "<<p << "\n";
	//	k++;
	//} while (p < min || p > 2<<32 || !fermat_test(p, 10));  // Проверяем, что p в заданном диапазоне и простое
	p = v[pos].second;
	long long h = 2;
	//g = 16;
	/*while (g == 1) {
		g = pow(h, (p - 1) / q) % p;
		std::cout << "g = "<<g << "\n";
		h++;
	}*/
	g = pow(h, (p - 1) / q);
	std::cout << "g = h**(p - 1)/q= " << g << std::endl;
	x = rand() % (q - 1) + 1;
	//x = 1;
	std::cout << pow(g, x) << "\n";
	y = pows(g, x, p) % p;
	std::cout << "x = " << x << ", y = " << y << "\n";
}
long long p = -1, q = -1, g = -1, y = -1, x = -1, k = -1;

std::pair<long long, long long> sign(const std::string& message, long long p, long long q, long long g, long long x) {
	std::hash<std::string> hash_fn;
	long long H = hash_string(message);
	//ll H = 10;
	//H = 4627212719276421476 / 10000;
	//H += 2;
	std::cout << "H1 = " << H << "\n";
	long long r, s;
	do {
		k = rand() % (q - 1) + 1;
		//ll k = 1;
		std::cout << "k = " << k << "\n";
		r = pows(g, k, p) % p % q;
		std::cout << "r = " << r << "\n";
		ll val = mod_inverse(k, q);
		std::cout << " val = " << val << "\n";
		ll val2 = (H + x * r);
		std::cout << "val2 = " << val2 << "\n";
		s = (val * val2) % q;
	} while (r == 0 || s == 0);
	std::cout << "r = " << r << ", s = " << s << "\n";
	return std::make_pair(r, s);
}
bool verify(const std::string& message, long long p, long long q, long long g, long long y, long long r, long long s) {
	if (r <= 0 || r >= q || s <= 0 || s >= q) {
		std::cout << "some error" << "\n";
		return false;
	}

	std::hash<std::string> hash_fn;
	long long H = hash_string(message);
	std::cout << "H2 = " << H << "\n";
	long long w = mod_inverse(s, q);
	std::cout << " w = " << w << "\n";
	long long u1 = (H * w) % q;
	long long u2 = (r * w) % q;
	std::cout << "u1 = " << u1 << ", u2 = " << u2 << "\n";
	std::cout << "ok";
	std::cout << "g**u1 = " << pows(g, u1, p) << " y**u2 = " << pows(y, u2, p) << std::endl;
	std::cout << "value = " << (pows(g, u1, p) * pows(y, u2, p) % p) << "\n";
	long long v = (pows(g, u1, p) * pows(y, u2, p) % p) % q;
	std::cout << "v = " << v << "\n";
	std::cout << "end verify" << "\n";
	return v == r;
}
void MarshalString(String^ s, std::string& os) {
	using namespace Runtime::InteropServices;
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}
String^ ConvertToManagedString(const std::string& stdString)
{
	// Преобразуем std::string в std::wstring
	std::wstring wstr(stdString.begin(), stdString.end());

	// Преобразуем std::wstring в String^
	return gcnew String(wstr.c_str());
}
System::Void Project2::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e) {
	srand(time(0));
	generate_keys(p, q, g, y, x);
	textBox3->Text = "" + x;
	textBox4->Text = "" + y;
}
std::pair<ll, ll> signature;
System::Void Project2::MyForm::button2_Click(System::Object^ sender, System::EventArgs^ e) {
	if (p <= 0) {
		label8->Text = "Не были сгенерированны ключи!";
		return;
	}
	std::string message;
	MarshalString(textBox1->Text, message);
	if (message.length() < 1) {
		label8->Text = "Пустое поле для исходного текста";
		return;
	}
	label8->Text = "";
	//textBox2->Text = ConvertToManagedString(message);
	signature = sign(message, p, q, g, x);
	textBox5->Text = "" + k;
	textBox6->Text = "(" + signature.first + ", " + signature.second + ")";
}
System::Void Project2::MyForm::button3_Click(System::Object^ sender, System::EventArgs^ e) {
	std::string messageToVerify;
	MarshalString(textBox2->Text, messageToVerify);
	if (messageToVerify.length() <= 0) {
		label8->Text = "Пустое поле для принятого текста";
		return;
	}
	label8->Text = "";
	bool verification = verify(messageToVerify, p, q, g, y, signature.first, signature.second);
	if (verification) {
		label7->Text = "OK";
		label7->ForeColor = Color::Green;
	}
	else {
		label7->Text = "FAILED SIGNATURE";
		label7->ForeColor = Color::Red;
	}
}