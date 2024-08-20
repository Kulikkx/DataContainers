#include<iostream>
#include<list>
using namespace std;
using std::cin;
using std::cout;
using std::endl;

#define tab "\t"
#define delimiter "\n-----------------------------------------------------\n"
class Element
{
	int Data;
	Element* pNext;
	static int count;
public:
	Element(int Data, Element* pNext = nullptr) : Data(Data), pNext(pNext)
	{
		count++;
		cout << "EConstructor:\t" << this << endl;
	}
	~Element()
	{
		count--;
		cout << "EDestructor:\t" << this << endl;
	}
	friend class Iterator;
	friend class ForwardList; // �������� �������
};
int Element::count = 0;

class Iterator  // ��������� �� �������
{
	Element* Temp;
public:
	Iterator(Element* Temp) :Temp(Temp)
	{
		cout << "IConstructor:\t" << this << endl;
	}
	~Iterator()
	{
		cout << "IDestructor:\t" << this << endl;
	}
	Iterator& operator++()
	{
		Temp = Temp->pNext;
		return *this;
	}
	Iterator operator++(int)
	{
		Iterator old = *this;
		Temp = Temp->pNext;
		return old;
	}
	bool operator ==(const Iterator& other)const
	{
		return this->Temp == other.Temp;
	}
	bool operator !=(const Iterator& other)const
	{
		return this->Temp != other.Temp;
	}
	const int operator*()const
	{
		return Temp->Data;
	}
	int& operator*()
	{
		return Temp->Data;
	}
};

class ForwardList
{
	Element* Head; // ������ ������, ��������� �� ��������� ������� ������
	int size;
public:
	Iterator begin()
	{
		return Head;
	}
	Iterator end()
	{
		return nullptr;
	}
	int get_size()const
	{
		return size;
	}
	ForwardList()
	{ //����������� �� ��������� ������� ������� ������ ������
		Head = nullptr;
		size = 0;
		cout << "LConstructor:\t" << this << endl;
	}
	explicit ForwardList(int size) :ForwardList()
	{
		while (size--)push_front(0);
	}
	ForwardList(const ForwardList& other) : ForwardList()
	{
		*this = other;  // �������� ���������� ��� CopyAssignment
		cout << "CopyConstructor:\t" << this << endl;
	}
	ForwardList(ForwardList&& other) :ForwardList()
	{
		*this = std::move(other); // ������� std::move ������������� �������� MoveAssignment ��� ������
		cout << "MoveConstructor:\t" << this << endl; //��������� ����������
	}
	ForwardList(const initializer_list <int>& il) :ForwardList()
	{
		//initializer_list (������ �������������) - ��� ���������, ��� �� ��� � Forwardlist
		// ��������� - ��� ������, ������� ���������� �������� ������ �������� � ������
		// � ������ ���������� � ������������ ������� ���� ��� ������ begin() � end()
		// begin() - ������� �������� �� ������ ����������
		// end() - ������� �������� �� ����� ����������
		//il.
		for (const int* it = il.begin(); it != il.end(); ++it)
			push_back(*it);
	}

	~ForwardList()
	{
		while (Head)pop_back();
		cout << "LDestructor:\t" << this << endl;
	}
	//                       Operators:
	ForwardList& operator=(const ForwardList& other)
	{
		if (this == &other)return *this;   //������ ������� �� ������������.
		this->~ForwardList();              //�������� ������ �� ������ ��������
		//while (Head)pop_front();
		for (Element* Temp = other.Head; Temp; Temp = Temp->pNext)
			push_back(Temp->Data);
		cout << "CopyAssignment:\t" << this << endl;
		return *this;
	}
	ForwardList& operator=(ForwardList&& other)
	{
		if (this == &other)return *this;
		this->~ForwardList();
		this->Head = other.Head;
		this->size = other.size;
		other.Head = nullptr;
		other.size = 0;
		cout << "MoveAssignment:\t" << this << endl;
	}

	const int& operator[](int index)const
	{
		Element* Temp = Head;
		for (int i = 0; i < index; i++)Temp = Temp->pNext;
		return Temp->Data;
	}
	int& operator[](int index)
	{
		Element* Temp = Head;
		for (int i = 0; i < index; i++)Temp = Temp->pNext;
		return Temp->Data;
	}
	//        Adding elements;
	void push_front(int Data)
	{
		//1) ������� ����� �������
		//Element* New = new Element(Data);
		//2) �������������� ����� ������� � ������ ������
		//New->pNext = Head;
		//3) ������ �������������� �� ����� �������
		Head = new Element(Data, Head);
		size++;
	}
	void push_back(int Data)
	{
		//��������� push_back() �� ����� �������� � ������ ������� �� ��������� � ���� ������ ���� �������� ����� push_front, ������� ����� �������� � ������ �������
		//1) ������� ����� �������
		//Element* New = new Element(Data);
		//2) ������� �� ����� ������:
		//3) ����� ���� ��� �� ��������� � ����� ������, ����� ��������� �������� � �����.
		if (Head == nullptr)return push_front(Data);
		Element* Temp = Head;
		//while ((*Temp).pNext)
		while (Temp->pNext)Temp = Temp->pNext;

		Temp->pNext = new Element(Data);
		size++;


	}
	void insert(int Index, int Data)
	{
		//1) ������� ����� �������:
		//2) ������� �� ������� ��������
		//3)��������� ������� � ������
		if (Index > size)
		{
			cout << "Error: out of range" << endl;
			return;
		}
		if (Index == 0)return push_front(Data);
		Element* New = new Element(Data);
		Element* Temp = Head;
		for (int i = 0; i < Index - 1; i++)
		{
			//if (Temp->pNext == nullptr)break;
			Temp = Temp->pNext;
		}
		//New->pNext = Temp->pNext;
		//Temp->pNext = New
		Temp->pNext = new Element(Data, Temp->pNext);
		size++;
	}
	void erase(int Index)                                  //������� �������� �� ������ �� ��������� �������
	{
		if (Index > size) { cout << "Error: " << endl; return; }
		if (Index == size)return pop_back();
		else
		{
			Element* Temp = Head;                                       //�������� 
			for (int i = 0; i < Index - 2; i++) { Temp = Temp->pNext; } //-2 �.�. ����� ���������
			Element* erased = Temp->pNext;                              //���������  �� ������� �������
			Temp->pNext = Temp->pNext->pNext;
			delete erased;
			size--;
		}
	}
	//                Removing elemets:
	void pop_front()
	{
		if (Head == nullptr)return;
		//1)���������� ����� ���������� ��������
		Element* Erased = Head;
		//2)��������� ��������� ������� �� ������
		Head = Head->pNext;
		//3)������� ������� �� ������
		delete Erased;
		size--;
		/*
		//�������� �� ������(�������) ������.
		if (Head == nullptr)return;
		// ������� ������ ������� ������
		Element* OldHead = Head;
		Head = Head->pNext;
		// ������� ������ ������ �������
		delete OldHead;
		------------
		new      -  ������� ������ � ������ ������
		new[]    -  ������� ������ �������� � ������ ������

		delete   - ������� 1 ������ �� ������� ������
		delete[] - ������� ������ �������� �� ������� ������
		delete new ....[...];  //behavior is undefined ����� �� ����������
		delete [] new ....;  //behavior is undefined ����� �� ����������

		*/
	}
	void pop_back()
	{
		if (Head->pNext == nullptr)return pop_front();
		//1)������� �� �������� ��������
		Element* Temp = Head;
		while (Temp->pNext->pNext)Temp = Temp->pNext;
		//2)������� ��������� �������
		delete Temp->pNext;
		//3) �������� ��������� �� ���� �������
		Temp->pNext = nullptr;
		size--;
		/*
		//1)�������� �� ������(�������) ������.
		if (Head == nullptr)return;
		//2) ������� �� ����� ������:
		Element* The_end = Head;
		while (The_end->pNext)The_end = The_end->pNext;    //������ (The_end->pNext) ����������� (The_end->pNext!=nullptr)
		//3) ������� ��������� ������� � ������ ���������� �� nullptr
		if (The_end == Head)
		{
			delete Head; Head = nullptr; // ���� ������ ��� �� ������ ���������
		}
		//4) ������� ��������� ������� � ���������� ��������� ����������� �� nullptr
		else
		{
			Element* Pred = Head;
			while (Pred->pNext != The_end)
			{
				Pred = Pred->pNext;
			}
			Pred->pNext = nullptr;
			delete The_end;
		}*/
	}



	//                       Metods:
	void print()const
	{
		//Element* Temp = Head; // TEMP ��� ��������, �������� - ��� ���������, ��� ������ �������� ����� �������� ������ � ��������� ��������� ������.
		//// Temp - ��������� Temp;
		//// Temp-> - ������� Temp;
		//while (Temp)
		//{
		//	cout << Temp << tab << Temp->Data << tab << Temp->pNext << endl;
		//	Temp = Temp->pNext; // ������� �� ��������� �������.
		//}
		for (Element* Temp = Head; Temp; Temp = Temp->pNext)
			cout << Temp << tab << Temp->Data << tab << Temp->pNext << endl;

		cout << "���-�� ����-�� ������: " << size << endl;
		cout << "����� ���-�� ����-��: " << Element::count << endl;
	}
};

ForwardList operator+(const ForwardList& left, const ForwardList& right)
{
	ForwardList buffer = left; // buffer  -  ��� ��������� ������, ������������ ������ � �������� ������� ��������� +()
	//for (int i = 0; i < left.get_size(); i++) buffer.push_back(left[i]);
	for (int i = 0; i < right.get_size(); i++) buffer.push_back(right[i]);
	return buffer;

}
void Print(int arr[])
{
	cout << typeid(arr).name() << endl;
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		cout << arr[i] << tab;
	}
	cout << endl;
}
//#define BASE_CHECK
//#define COUNT_CHECK
//#define SIZE_CONSTRUCTOR_CHECK
//#define OPERATOR_PLUS_CHECK
//#define INITIALIZER_LIST_CONSTRUCTOR
//#define RANGE_BASED_FOR_ARRAY
#define RANGE_BASED_FOR_LIST
#define ITERATOR_CHECK
void main()
{
	setlocale(LC_ALL, "Rus");
#ifdef BASE_CHECK
	int n;
	cout << "������� ���-�� ��������� ������: "; cin >> n;
	ForwardList list;
	for (int i = 0; i < n; i++)
	{
		//list.push_front(rand() % 100);
		list.push_back(rand() % 100);
	}
	list.print();
	cout << delimiter << endl;
	list.push_back(123);
	list.print();
	cout << delimiter << endl;
	list.pop_front();
	list.print();
	cout << delimiter << endl;
	list.pop_back();
	list.print();
	cout << delimiter << endl;
	int index;
	int value;
	cout << "������� ������ ������������ ��������: "; cin >> index;
	cout << "������� �������� ������������ ��������: "; cin >> value;
	list.insert(index, value);
	list.print();
#endif // BASE_CHECK
#ifdef COUNT_CHECK
	ForwardList list1;
	list1.push_back(3);
	list1.push_back(5);
	list1.push_back(8);
	list1.push_back(13);
	list1.push_back(21);
	list1.print();
#endif // COUNT_CHECK
#ifdef SIZE_CONSTRUCTOR_CHECK
	ForwardList list(5);
	for (int i = 0; i < list.get_size(); i++)
	{
		list[i] = rand() % 100;
	}
	for (int i = 0; i < list.get_size(); i++)
	{
		cout << list[i] << tab;
	}
	cout << endl;
#endif // SIZE_CONSTRUCTOR_CHECK

#ifdef OPERATOR_PLUS_CHECK
	ForwardList list1;
	list1.push_back(3);
	list1.push_back(5);
	list1.push_back(8);
	list1.push_back(13);
	list1.push_back(21);
	list1.print();

	ForwardList list2;
	list2.push_front(34);
	list2.push_front(55);
	list2.push_front(89);
	list2.print();

	cout << delimiter << endl;
	//ForwardList list3 = list1 + list2;     // COPYCONSTRUCTOR
	cout << delimiter << endl;
	ForwardList list3;
	cout << delimiter << endl;
	list3 = list1 + list2;               // COPYASSIGNMENT
	cout << delimiter << endl;
	list3.print();
#endif // OPERATOR_PLUS_CHECK

#ifdef INITIALIZER_LIST_CONSTRUCTOR
	ForwardList list1 = { 3, 5, 8, 13, 21 }; //instance - ���������
	list1.print();
#endif // INITIALIZER_LIST_CONSTRUCTOR
#ifdef RANGE_BASED_FOR_ARRAY
	int arr[] = { 3, 5, 8, 13, 21 };
	//int* arr = new int[] {3, 5, 8, 13, 21};
	for (int i = 0; i < sizeof(arr) / sizeof(int); i++)
		//������ ������� � ������ ����� �� ������ ��� � ������
	{
		cout << arr[i] << tab;
	}
	cout << endl;
	for (int i : arr)
		//Range - besed for: ��������. ��� ������ �������� � ���� ��������� �������� ���������. 
	{
		cout << i << tab;
	}
	cout << endl;
	cout << typeid(arr).name() << endl;
	Print(arr);
	//delete[] arr;  
#endif // RANGE_BASED_FOR_ARRAY

#ifdef RANGE_BASED_FOR_LIST
	ForwardList list = { 3, 5, 8, 13, 21 };
	//list.print();
	for (int i : list)
	{
		cout << i << tab;
	}
	cout << endl;
	for (Iterator it = list.begin(); it != list.end(); ++it)
	{
		cout << *it << endl;
	}
#endif // RANGE_BASED_FOR_LIST


}