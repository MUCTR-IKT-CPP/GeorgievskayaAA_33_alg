// Очередь через односвязный список
#include <iostream>
#include <stdexcept>
#include <random>
#include <limits> 
#include <string>
#include <ctime>
#include <chrono>
#include <sstream>
#include <memory>

// Этот класс реализует узел для связного списка. Каждый узел хранит данные типа T и указатель на следующий узел. Конструктор инициализирует данные и указывает на отсутствие следующего узла.
template <typename T>
class Node {
    public:
        T data;
        Node* next;
        
        Node(const T& data) : data(data), next(nullptr) {}
};

//
template <typename T>
class Queue {
    private:
        Node<T>* front;
        Node<T>* back;
        size_t size;
    
    public:
        // Конструктор
        Queue() : front(nullptr), back(nullptr), size(0) {}
        
        //деструктор
        ~Queue() {
            while (!isEmpty()) {
                dequeue();
            }
        }
        
        //проверка на пустоту
        bool isEmpty() const {
            return size == 0;
        }
        
        //подсчет элементов
        size_t getSize() const {
            return size;
        }
        
        //добавление в конец
        void enqueue(const T& value) {
            Node<T>* newNode = new Node<T>(value);
            if (isEmpty()) {
                front = back = newNode;
            } 
            else {
                back->next = newNode;
                back = newNode;
            }
            size++;
        }
        
        //взятие из начала
        T dequeue() {
            if (isEmpty()) {
                throw std::out_of_range("Queue is empty");
            }
            Node<T>* temp = front;
            T data = front->data;
            front = front->next;
            delete temp;
            size--;
            if (isEmpty()) {
                back = nullptr;
            }
            return data;
        }
        
        //итератор
        class Iterator {
            private:
                Node<T>* current;

            public:
            Iterator(Node<T>* node) : current(node) {}
    
            T& operator*() {
                return current->data;
            }
    
            Iterator& operator++() {
                if (current) {
                    current = current->next;
                }
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return current != other.current;
            }
        };
        
        //начало итератора
        Iterator begin(){
            return Iterator(front);
        }
        
        //конец итератора
        Iterator end(){
            return Iterator(nullptr);
        }
        
        // Функция для обхода очереди
        void forEach(void (*func)(T&)) {
            for (Iterator it = begin(); it != end(); ++it){
                func(*it);
            }
        }
};

// =======================================
void test1() {
    Queue<int> q;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-1000, 1000);
    
    //добавление 1000 эл-тов в очередь
    for (int i = 0; i < 1000; ++i) {
        q.enqueue(dis(gen));
    }
    
    // Ининциализация переменных для подсчета
    long long sum = 0;
    int min = std::numeric_limits<int>::max();
    int max = std::numeric_limits<int>::min();
    
    // Подсчет суммы, минимума, максимума
    for (auto it = q.begin(); it != q.end(); ++it) {
        sum += *it;
        if (*it < min) min = *it;
        if (*it > max) max = *it;
    }

    double average = static_cast<double>(sum) / q.getSize();
    
    // Вывод результатов
    std::cout << "====== TEST 1 ======" << std::endl;
    std::cout << "Queue statistics after adding 1000 elements:" << std::endl;
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Average: " << average << std::endl;
    std::cout << "Min: " << min << std::endl;
    std::cout << "Max: " << max << std::endl;

    // Очистка очереди
    while (!q.isEmpty()) {
        q.dequeue();
    }
}

// =======================================
void test2() {
    Queue<std::string> q;
    
    q.enqueue("First");
    q.enqueue("Second");
    q.enqueue("Third");
    q.enqueue("Fourth");
    q.enqueue("Fifth");
    q.enqueue("Sixth");
    q.enqueue("Seventh");
    q.enqueue("Eighth");
    q.enqueue("Ninth");
    q.enqueue("Tenth");
    
    std::cout << "====== TEST 2 ======" << std::endl;
    std::cout << "Queue after enqueueing 10 string elements:" << std::endl;
    
    // Извлечение и вывод строк из очереди
    while (!q.isEmpty()) {
        std::cout << q.dequeue() << std::endl;
    }
}

// =======================================

struct Person {
    std::string lastName;
    std::string firstName;
    std::string patronymic;
    std::string birthDate; // Формат: "ДД.ММ.ГГГГ"
};

class RandomDataGeneration {
    private:
        std::vector<std::string> lastNames;
        std::vector<std::string> firstNames;
        std::vector<std::string> patronymics;
        std::mt19937 gen;
        
    public:
        RandomDataGeneration() {
            std::random_device rd;
            gen = std::mt19937(rd());
    
            lastNames = {"Иванов", "Петров", "Сидоров", "Кузнецов", "Новиков"};
            firstNames = {"Иван", "Петр", "Алексей", "Дмитрий", "Максим"};
            patronymics = {"Иванович", "Петрович", "Алексеевич", "Дмитриевич", "Максимович"};
        }
        
        std::string getRandomElement(const std::vector<std::string>& vec) {
            std::uniform_int_distribution<int> dis(0, vec.size() - 1);
            return vec[dis(gen)];
        }
        
        std::string getRandomBirthDate() {
            std::uniform_int_distribution<int> dayDis(1, 28);  // день от 1 до 28 (чтобы избежать сложностей с месяцами)
            std::uniform_int_distribution<int> monthDis(1, 12); // месяц от 1 до 12
            std::uniform_int_distribution<int> yearDis(1980, 2020); // год от 1980 до 2020
    
            int day = dayDis(gen);
            int month = monthDis(gen);
            int year = yearDis(gen);
    
            std::ostringstream oss;
            oss << (day < 10 ? "0" : "") << day << "."
                << (month < 10 ? "0" : "") << month << "."
                << year;
            return oss.str();
        }
    
        Person generateRandomPerson() {
            Person p;
            p.lastName = getRandomElement(lastNames);
            p.firstName = getRandomElement(firstNames);
            p.patronymic = getRandomElement(patronymics);
            p.birthDate = getRandomBirthDate();
            return p;
        }
};

int calculateAge(const std::string& birthDate) {
    // Разбираем дату на день, месяц и год
    int day, month, year;
    std::sscanf(birthDate.c_str(), "%d.%d.%d", &day, &month, &year);

    // Получаем текущую дату
    auto now = std::chrono::system_clock::now();
    auto now_tm = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now = *std::localtime(&now_tm);

    int age = tm_now.tm_year + 1900 - year; // текущий год минус год рождения

    // Корректировка по месяцам и дням
    if (tm_now.tm_mon + 1 < month || (tm_now.tm_mon + 1 == month && tm_now.tm_mday < day)) {
        age--;
    }

    return age;
}

void test3() {
    RandomDataGeneration generator;
    Queue<Person> people;
    Queue<Person> under20, over30;

    // Генерация 100 случайных людей
    for (int i = 0; i < 100; ++i) {
        people.enqueue(generator.generateRandomPerson());
    }

    // Фильтрация людей младше 20 лет и старше 30 лет
    while (!people.isEmpty()) {
        Person person = people.dequeue();
        if (person.age < 20) {
            under20.enqueue(person);
        } else if (person.age > 30) {
            over30.enqueue(person);
        }
    }

    // Вывод результатов
    std::cout << "====== TEST 3 ======" << std::endl;
    std::cout << "People under 20 years: " << under20.size() << std::endl;
    std::cout << "People over 30 years: " << over30.size() << std::endl;

    // Подсчет людей, которые не попали в фильтрацию
    int notMatched = people.size() - under20.size() - over30.size();
    std::cout << "People not matched (between 20 and 30 years): " << notMatched << std::endl;
}
// =======================================
// Функция для инверсии содержимого очереди
template <typename T>
void invertQueue(Queue<T>& q) {
    size_t n = q.getSize();
    Queue<T> tempQueue;

    for (size_t i = 0; i < n; ++i) {
        T value = q.dequeue();  // Извлекаем элемент
        tempQueue.enqueue(value);  // Вставляем его в новую очередь
    }

    // Теперь элементы в tempQueue инвертированы по порядку
    // Переносим обратно в исходную очередь
    while (!tempQueue.isEmpty()) {
        q.enqueue(tempQueue.dequeue());
    }
}

// Функция для измерения времени выполнения операций
template <typename T>
void testQueueOperations() {
    Queue<T> q;

    // Измеряем время выполнения операции вставки
    auto startInsert = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= 10000; ++i) {
        q.enqueue(i);  // Заполняем очередь отсортированными элементами
    }
    auto endInsert = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> insertDuration = endInsert - startInsert;
    std::cout << "Time to insert 10000 elements: " << insertDuration.count() << " seconds" << std::endl;

    // Измеряем время выполнения операции изъятия
    auto startDequeue = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= 10000; ++i) {
        q.dequeue();  // Извлекаем элементы
    }
    auto endDequeue = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dequeueDuration = endDequeue - startDequeue;
    std::cout << "Time to dequeue 10000 elements: " << dequeueDuration.count() << " seconds" << std::endl;

    // Память, занимаемая очередью
    size_t memoryUsage = sizeof(Queue<T>) + sizeof(Node<T>) * q.getSize();
    std::cout << "Memory usage for 10000 elements: " << memoryUsage << " bytes" << std::endl;
}



int main(){
    test1();
    test2();
    test3();
    
    // Заполнение очереди отсортированными элементами и инвертирование
    Queue<int> q;
    for (int i = 1; i <= 10000; ++i) {
        q.enqueue(i);  // Заполняем очередь отсортированными по возрастанию элементами
    }

    // Инвертируем очередь
    invertQueue(q);

    // Тестирование вставки и изъятия
    std::cout << "====== TEST 4 ======" << std::endl;
    testQueueOperations<int>();
    return 0;
}
